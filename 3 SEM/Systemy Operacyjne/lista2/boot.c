// boot.c

void delay() {
    // opoznienie
    for (volatile int i = 0; i < 50000000; i++);
}

void cls(char *screen) {
    for (int i = 0; i < 80 * 25; i++) {
        screen[2 * i] = ' ';         // Ustawia pusty znak
        screen[2 * i + 1] = 0x0;     // Ustawia czarne tło
    }
}

void main() {
    char *video_memory = (char*) 0xB8000;
    char *message = "Hello, World!";
    int index = 0; // Zaczynamy od góry ekranu

    // Czyszczenie ekranu
    cls(video_memory);

    // Iteracja przez 8 kolorów tła i 8 kolorów tekstu
    for (int bg = 0; bg < 8; bg++) {
        for (int fg = 0; fg < 8; fg++) {
            unsigned char color = (bg << 4) | fg;  // Ustawienie koloru tła i tekstu
            for (int i = 0; message[i] != '\0'; i++) {
                video_memory[index * 2 + i * 2] = message[i];        // Wyświetlenie znaku
                video_memory[index * 2 + i * 2 + 1] = color;         // Ustawienie koloru
            }
            index += 80;  // Przejście do nowej linii
            delay();      // Opóźnienie, aby kolory zmieniały się wolniej
        }
    }

    // Zapętlenie programu, aby ekran nie zniknął
    while (1);
}

