#!/bin/bash

# PID - identyfikator procesu
# COMM - nazwa polecenia (nazwa procesu)
# STATE - stan procesu (R: uruchomiony, S: śpiący, D: nieprzerywalny stan, Z: zombie, T: zatrzymany)
# PPID - identyfikator procesu rodzica
# PGRP - identyfikator grupy procesów
# SID - identyfikator sesji
# TTY - terminal kontrolujący (lub N/A jeśli brak)
# RSS - pamięć fizyczna w kB używana przez proces
# OPEN_FILES - liczba otwartych deskryptorów plików

(
    # Nagłówek tabeli
    echo -e "PPID\tPID\tCOMM(NAME)\tSTATE\tTTY\tRSS\tPGID\tSID\tOPEN_FILES"

    # Iteracja przez katalogi z numerami PID w /proc
    for pid in /proc/[0-9]*; do
        pid="${pid##*/}"  # Wyciągnięcie numeru PID

        # Sprawdzenie dostępności wymaganych plików
        if [[ -r /proc/$pid/status && -r /proc/$pid/stat && -r /proc/$pid/fd ]]; then

            # Pobieranie danych o procesie
            ppid=$(awk '/^PPid:/ {print $2}' /proc/$pid/status)
            comm=$(awk '/^Name:/ {print $2}' /proc/$pid/status)
            state=$(awk '/^State:/ {print $2}' /proc/$pid/status)
            rss=$(awk '/^VmRSS:/ {print $2}' /proc/$pid/status)

            # Dodatkowe informacje z /proc/$pid/stat
            tty=$(awk '{print $7}' /proc/$pid/stat)
            pgid=$(awk '{print $5}' /proc/$pid/stat)
            sid=$(awk '{print $6}' /proc/$pid/stat)

            # Liczba otwartych plików
            open_files=$(ls /proc/$pid/fd 2>/dev/null | wc -l)

            # Domyślne wartości w przypadku braku danych
            rss=${rss:-0}
            tty=${tty:-'N/A'}

            # Wyświetlanie danych procesu
            echo -e "$ppid\t$pid\t$comm\t$state\t$tty\t$rss\t$pgid\t$sid\t$open_files"
        fi
    done 
# Formatowanie tabeli za pomocą column
)| column -t -s $'\t'
