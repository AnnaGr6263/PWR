#!/bin/bash 

export LC_NUMERIC="C"  

# Wybierz interfejs sieciowy do monitorowania
INTERFACE="enp0s3"

# Inicjalizacja zmiennych do śledzenia ruchu sieciowego
prev_rx=0  # Poprzedni stan odbioru danych
prev_tx=0  # Poprzedni stan wysyłania danych
total_rx=0 # Całkowity odbiór danych
total_tx=0 # Całkowita wysyłka danych
total_time=0  # Czas działania w sekundach

# Historia prędkości sieci
history_size=10  # Liczba pomiarów do zapamiętania
declare -a rx_history  # Historia prędkości odbioru
declare -a tx_history  # Historia prędkości wysyłania

# Inicjalizacja tablic historii zerami
for ((i = 0; i < history_size; i++)); do
    rx_history[i]=0
    tx_history[i]=0
done

# Funkcja konwertująca bajty na czytelne jednostki (B, KB, MB)
convert_to_human_readable() {
    local bytes=$1
    if (( bytes >= 1048576 )); then
        printf "%.2f MB" "$(echo "scale=2; $bytes / 1048576" | bc)"
    elif (( bytes >= 1024 )); then
        printf "%.2f KB" "$(echo "scale=2; $bytes / 1024" | bc)"
    else
        echo "$bytes B"
    fi
}

# Pobieranie danych sieciowych z pliku /proc/net/dev
get_network_data() {
    grep "$INTERFACE" /proc/net/dev | tr -s ' ' | cut -d ':' -f 2
}

# Funkcja rysująca prosty wykres słupkowy z historii danych
draw_graph() {
    local -n data=$1  # Tablica danych do rysowania (przekazywana przez referencję)
    local label=$2  # Etykieta wykresu
    echo "$label:"
    
    # Znajdź maksymalną wartość w historii danych
    local max_value=$(printf "%s\n" "${data[@]}" | sort -nr | head -n1)
    if (( max_value == 0 )); then
        max_value=1  # Zapobieganie dzieleniu przez zero
    fi
    
    local max_length=40  # Maksymalna długość słupka w znakach "*"
    
    # Generowanie słupków dla każdej wartości w historii
    for value in "${data[@]}"; do
        local bar_length=$(echo "scale=0; ($value * $max_length) / $max_value" | bc)
        if (( bar_length == 0 && value > 0 )); then
            bar_length=1  # Minimalna długość słupka dla wartości > 0
        fi
        for ((i = 0; i < bar_length; i++)); do
            echo -n "*"
        done
        echo
    done
    echo
}

# Funkcja aktualizująca tablicę historii prędkości
update_history() {
    local -n history=$1  # Tablica do zaktualizowania (przekazywana przez referencję)
    local value=$2       # Nowa wartość do dodania
    history=("${history[@]:1}" "$value")  # Usunięcie najstarszej wartości, dodanie nowej
}

# Obliczanie wykorzystania CPU dla każdego rdzenia
get_cpu_usage() {
    echo "Wykorzystanie CPU:"
    for i in $(seq 0 $(($(nproc) - 1))); do
        local cpu_data=($(grep -w "cpu$i" /proc/stat))  # Pobranie danych CPU z /proc/stat
        local total=0
        for val in "${cpu_data[@]:1}"; do
            total=$((total + val))  # Sumowanie wszystkich wartości (czas procesora)
        done
        local idle=${cpu_data[4]}  # Czas bezczynności

        local total_delta=$((total - cpu_last_total[i]))
        local idle_delta=$((idle - cpu_last_idle[i]))
        local usage=0
        if (( total_delta > 0 )); then
            usage=$((100 * (total_delta - idle_delta) / total_delta))
        fi

        local freq=$(grep "cpu MHz" /proc/cpuinfo | awk "NR==$((i + 1)) {print \$4}")
        echo "  CPU$i: $usage% @ ${freq:-N/A} MHz"

        cpu_last_total[i]=$total
        cpu_last_idle[i]=$idle
    done
}

# Pobranie czasu działania systemu z /proc/uptime
get_system_uptime() {
    uptime_seconds=$(awk '{print $1}' /proc/uptime)
    days=$(echo "$uptime_seconds / 86400" | bc)
    hours=$(echo "($uptime_seconds % 86400) / 3600" | bc)
    minutes=$(echo "($uptime_seconds % 3600) / 60" | bc)
    seconds=$(echo "$uptime_seconds % 60" | bc)
    echo "Czas działania systemu: ${days}d ${hours}h ${minutes}m ${seconds}s"
}

# Pobranie stanu baterii
get_battery_status() {
    if [[ -f /sys/class/power_supply/BAT0/uevent ]]; then
        battery_level=$(grep "POWER_SUPPLY_CAPACITY=" /sys/class/power_supply/BAT0/uevent | cut -d '=' -f 2)
        echo "Stan baterii: ${battery_level}%"
    else
        echo "Bateria: Nie znaleziono danych (brak baterii?)"
    fi
}

# Pobranie obciążenia systemu z /proc/loadavg
get_system_load() {
    loadavg=$(cat /proc/loadavg)
    echo "Obciążenie systemu (load average): $loadavg"
}

# Pobranie wykorzystania pamięci z /proc/meminfo
get_memory_usage() {
    mem_total=$(grep "^MemTotal:" /proc/meminfo | awk '{print $2}')
    mem_free=$(grep "^MemFree:" /proc/meminfo | awk '{print $2}')
    mem_available=$(grep "^MemAvailable:" /proc/meminfo | awk '{print $2}')
    echo "Pamięć: Total: ${mem_total} KB, Free: ${mem_free} KB, Available: ${mem_available} KB"
}

# Pętla główna monitorowania systemu
while true; do
    raw_data=$(get_network_data)  # Pobranie danych sieciowych
    if [[ -z "$raw_data" ]]; then
        echo "Błąd: Brak danych dla interfejsu $INTERFACE."
        exit 1
    fi

    current_rx=$(echo "$raw_data" | awk '{print $1}')  # Pobranie odbioru danych
    current_tx=$(echo "$raw_data" | awk '{print $9}')  # Pobranie wysyłania danych

    if (( total_time > 0 )); then
        diff_rx=$((current_rx - prev_rx))  # Różnica w odbiorze
        diff_tx=$((current_tx - prev_tx))  # Różnica w wysyłaniu
    else
        diff_rx=0
        diff_tx=0
    fi

    update_history rx_history $diff_rx  # Aktualizacja historii odbioru
    update_history tx_history $diff_tx  # Aktualizacja historii wysyłania

    total_rx=$((total_rx + diff_rx))
    total_tx=$((total_tx + diff_tx))
    avg_rx=$((total_rx / (total_time + 1)))
    avg_tx=$((total_tx / (total_time + 1)))

    clear  # Wyczyść ekran
    echo "Monitor systemu"
    echo "====================================="
    echo "Monitor sieci ($INTERFACE)"
    echo "Aktualna prędkość odbioru: $(convert_to_human_readable $diff_rx)/s"
    echo "Aktualna prędkość wysyłania: $(convert_to_human_readable $diff_tx)/s"
    echo "Średnia prędkość odbioru: $(convert_to_human_readable $avg_rx)/s"
    echo "Średnia prędkość wysyłania: $(convert_to_human_readable $avg_tx)/s"
    echo "====================================="
    draw_graph rx_history "Historia prędkości odbioru"
    draw_graph tx_history "Historia prędkości wysyłania"
    echo "====================================="
    get_cpu_usage
    echo "====================================="
    get_system_uptime
    get_battery_status
    get_system_load
    get_memory_usage
    echo "====================================="

    prev_rx=$current_rx  # Zapisz aktualny stan odbioru
    prev_tx=$current_tx  # Zapisz aktualny stan wysyłania
    ((total_time++))
    sleep 1  
done
