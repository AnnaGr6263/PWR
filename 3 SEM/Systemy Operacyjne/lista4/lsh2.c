#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

// Jako zadanie 3 i 4 przesyłam jeden plik który jest wynikiem tych dwóch zadań

#define LSH_RL_BUFSIZE 1024 
#define LSH_TOK_BUFSIZE 64  
#define LSH_TOK_DELIM " \t\r\n\a"

// Deklaracje funkcji dla poleceń wbudowanych
int lsh_cd(char **args);
int lsh_exit(char **args);

// Lista poleceń wbudowanych i ich odpowiadających funkcji
char *builtin_str[] = {
    "cd",    // Zmiana katalogu
    "exit"   // Wyjście z powłoki
};

// Tablica wskaźników do funkcji obsługujących polecenia wbudowane
int (*builtin_func[]) (char **) = {
    &lsh_cd,
    &lsh_exit
};

// Funkcja zwraca liczbę poleceń wbudowanych
int lsh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

// Implementacja polecenia `cd`
int lsh_cd(char **args) {
    // Jeśli brak argumentu, wyświetl komunikat o błędzie
    if (args[1] == NULL) {
        fprintf(stderr, "lsh: oczekiwano argumentu dla \"cd\"\n");
    } else {
        // Zmień katalog na wskazany
        if (chdir(args[1]) != 0) {
            perror("lsh");
        }
    }
    return 1; // Kontynuuj działanie powłoki
}

// Implementacja polecenia `exit`
int lsh_exit(char **args) {
    return 0; // Zwraca 0, co kończy działanie powłoki
}

// Obsługa sygnału SIGINT (Ctrl-C)
void sigint_handler(int sig) {
    printf("\n$lsh> "); // Wyświetlenie nowej linii promptu
    fflush(stdout);
}

// Funkcja do odczytu linii wejściowej
char *lsh_read_line(void) {
    char *line = NULL;
    size_t bufsize = 0;

    // Użyj getline() do dynamicznego odczytu linii
    if (getline(&line, &bufsize, stdin) == -1) {
        if (feof(stdin)) { // Sprawdź, czy osiągnięto EOF
            exit(0); // Zakończ program
        } else {
            perror("lsh");
            exit(EXIT_FAILURE);
        }
    }
    return line; // Zwraca wczytaną linię
}

// Funkcja do podziału linii na tokeny
char **lsh_split_line(char *line) {
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *)); // Alokacja pamięci dla tokenów
    char *token;

    if (!tokens) {
        fprintf(stderr, "lsh: błąd alokacji pamięci\n");
        exit(EXIT_FAILURE);
    }

    // Użyj strtok do podziału linii na tokeny
    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position++] = token;

        // Dynamicznie zwiększ rozmiar bufora, jeśli to konieczne
        if (position >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "lsh: błąd alokacji pamięci\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL; // Oznacz koniec tokenów
    return tokens;
}

// Funkcja obsługująca przekierowania (>, <, 2>)
void lsh_handle_redirections(char **args) {
    int i = 0;

    while (args[i] != NULL) {
        if (strcmp(args[i], ">") == 0) { // Przekierowanie wyjścia
            int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                perror("lsh: błąd otwarcia pliku");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO); // Przekieruj stdout do pliku
            close(fd);
            args[i] = NULL; // Usuń `>` i nazwę pliku z argumentów
            break;
        } else if (strcmp(args[i], "2>") == 0) { // Przekierowanie błędów
            int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                perror("lsh: błąd otwarcia pliku");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDERR_FILENO); // Przekieruj stderr do pliku
            close(fd);
            args[i] = NULL;
            break;
        } else if (strcmp(args[i], "<") == 0) { // Przekierowanie wejścia
            int fd = open(args[i + 1], O_RDONLY);
            if (fd == -1) {
                perror("lsh: błąd otwarcia pliku");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO); // Przekieruj stdin z pliku
            close(fd);
            args[i] = NULL;
            break;
        }
        i++;
    }
}

// Funkcja obsługująca potoki (|)
int lsh_execute_piped(char **args) {
    int pipefd[2];
    pid_t p1, p2;
    char **cmd1, **cmd2;
    int i = 0;

    // Podziel polecenie na dwie części przed i po `|`
    while (args[i] != NULL) {
        if (strcmp(args[i], "|") == 0) {
            args[i] = NULL;
            cmd1 = args;          // Pierwsze polecenie
            cmd2 = &args[i + 1];  // Drugie polecenie
            break;
        }
        i++;
    }

    if (pipe(pipefd) == -1) { // Utwórz potok
        perror("lsh: błąd tworzenia potoku");
        return 1;
    }

    p1 = fork(); // Utwórz pierwszy proces
    if (p1 == 0) {
        dup2(pipefd[1], STDOUT_FILENO); // Przekieruj stdout do potoku
        close(pipefd[0]);
        close(pipefd[1]);
        lsh_handle_redirections(cmd1); // Obsłuż przekierowania
        if (execvp(cmd1[0], cmd1) == -1) {
            perror("lsh");
            exit(EXIT_FAILURE);
        }
    }

    p2 = fork(); // Utwórz drugi proces
    if (p2 == 0) {
        dup2(pipefd[0], STDIN_FILENO); // Przekieruj stdin z potoku
        close(pipefd[1]);
        close(pipefd[0]);
        lsh_handle_redirections(cmd2); // Obsłuż przekierowania
        if (execvp(cmd2[0], cmd2) == -1) {
            perror("lsh");
            exit(EXIT_FAILURE);
        }
    }

    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(p1, NULL, 0); // Poczekaj na zakończenie pierwszego procesu
    waitpid(p2, NULL, 0); // Poczekaj na zakończenie drugiego procesu

    return 1; // Kontynuuj działanie powłoki
}

// Funkcja uruchamiająca program z argumentami
int lsh_launch(char **args, int background) {
    pid_t pid, wpid;
    int status;

    pid = fork(); // Utwórz proces potomny
    if (pid == 0) {
        signal(SIGINT, SIG_IGN); // Ignoruj SIGINT w procesie potomnym
        lsh_handle_redirections(args); // Obsłuż przekierowania
        if (execvp(args[0], args) == -1) { // Uruchom program
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("lsh"); // Błąd tworzenia procesu
    } else {
        if (background) {
            printf("[W tle] PID: %d\n", pid); // Informacja o procesie w tle
        } else {
            do {
                wpid = waitpid(pid, &status, WUNTRACED); // Czekaj na zakończenie procesu
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }

    return 1; // Kontynuuj działanie powłoki
}

// Funkcja wykonująca polecenie
int lsh_execute(char **args) {
    int background = 0;

    if (args[0] == NULL) {
        return 1; // Brak polecenia do wykonania
    }

    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            return lsh_execute_piped(args); // Obsługa potoków
        } else if (strcmp(args[i], "&") == 0) {
            background = 1; // Obsługa procesów w tle
            args[i] = NULL;
            break;
        }
    }

    for (int i = 0; i < lsh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args); // Wykonanie polecenia wbudowanego
        }
    }

    return lsh_launch(args, background); // Uruchom program zewnętrzny
}

// Główna pętla powłoki
void lsh_loop(void) {
    char *line;
    char **args;
    int status;

    do {
        printf("$lsh> "); // Wyświetl prompt
        line = lsh_read_line();      // Odczytaj linię od użytkownika
        args = lsh_split_line(line); // Podziel linię na argumenty
        status = lsh_execute(args);  // Wykonaj polecenie

        free(line); // Zwolnij pamięć
        free(args);
    } while (status); // Powtarzaj, dopóki status != 0
}

// Funkcja główna programu
int main(int argc, char **argv) {
    signal(SIGCHLD, SIG_IGN);  // Ignoruj procesy zombie
    signal(SIGINT, sigint_handler); // Obsłuż Ctrl-C

    lsh_loop(); // Rozpocznij główną pętlę powłoki

    return EXIT_SUCCESS;
}
