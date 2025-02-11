#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <signal.h>
#include <unistd.h>


volatile sig_atomic_t signal_count = 0;

// Funkcja obsługi sygnałów
void signal_handler(int sig) {
    signal_count++;
}

int main() {
    // Rejestracja funkcji obsługi dla SIGUSR1
    signal(SIGUSR1, signal_handler);

    pid_t pid = getpid(); // Pobierz PID bieżącego procesu
    printf("PID procesu: %d\n", pid);

    // Wysyłanie 10 sygnałów SIGUSR1 do samego siebie
    for (int i = 0; i < 1000; i++) {
        kill(pid, SIGUSR1);
    }

    // Czekanie na dostarczenie sygnałów
    sleep(1);

    // Wyświetlenie liczby odebranych sygnałów
    printf("Liczba odebranych sygnałów SIGUSR1: %d\n", signal_count);

    return 0;
}
