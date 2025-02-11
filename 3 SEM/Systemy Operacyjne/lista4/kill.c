#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>

int main() {
    pid_t init_pid = 1;  // PID procesu init
    int signal_to_send = SIGKILL;  // Sygnał do wysłania

    printf("Wysyłanie sygnału %d do procesu init (PID %d)...\n", signal_to_send, init_pid);

    if (kill(init_pid, signal_to_send) == -1) {
        perror("Nie udało się wysłać sygnału");
    } else {
        printf("Sygnał %d został wysłany do init (PID %d).\n", signal_to_send, init_pid);
    }

    return 0;
}
