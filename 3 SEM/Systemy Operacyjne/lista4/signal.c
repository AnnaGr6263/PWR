#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Funkcja obsługi sygnałów
void signal_handler(int sig) {
    printf("Otrzymano sygnał: %d\n", sig);
}

int main() {
    // Rejestracja handlera dla wszystkich sygnałów oprócz SIGKILL i SIGSTOP
    for (int i = 1; i < _NSIG; i++) {
        if (i != SIGKILL && i != SIGSTOP) {
            if (signal(i, signal_handler) == SIG_ERR) {
                printf("Nie można obsłużyć sygnału %d\n", i);
            }
        }
    }

    // Informacja o PID procesu
    printf("Czekam na sygnały... PID: %d\n", getpid());

    // Oczekiwanie na sygnały
    while (1) {
        pause();
    }

    return 0;
}
