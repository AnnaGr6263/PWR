#include <stdio.h>

int main() {
    // Wyświetlanie w kolorach tekstu
    for (int i = 0; i < 256; i++) {
        printf("\033[38;5;%dmHello, World!\033[0m\n", i);  // Kolory tekstu od 0 do 255
    }
    // Wyświetlanie w kolorach tła
    for (int i = 0; i < 256; i++) {
        printf("\033[48;5;%dmHello, World!\033[0m\n", i);  // Kolory tła od 0 do 255
    }
    return 0;
}

