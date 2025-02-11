;   Anna Grelewska
;   zadanie 1 lista 5
;   x86 (32-bit)

;   nasm -f elf32 -o sum.o sum.asm
;   ld -m elf_i386 -o sum sum.o
;   ./sum


section .bss
    liczba_bufor resb 12    ; Bufor na wczytaną liczbę (maksymalnie 11 cyfr + null)
    suma_cyfr   resd 1      ; Zmienna do przechowywania sumy cyfr
    suma_bufor  resb 12     ; Bufor na tekstową reprezentację sumy cyfr

section .data
    komunikat_podaj db "Podaj liczbe: ", 0    ; Tekst wyświetlany przed wprowadzeniem liczby
    komunikat_wynik db "Suma cyfr: ", 0       ; Tekst wyświetlany przed wynikiem
    nowa_linia db 0xA, 0                      ; Znak nowej linii

section .text
    global _start

_start:
    ; Wyświetl komunikat "Podaj liczbe: "
    mov eax, 4                  ; Syscall: write
    mov ebx, 1                  ; Deskryptor pliku: stdout
    mov ecx, komunikat_podaj    ; Adres komunikatu
    mov edx, 14                 ; Długość komunikatu
    int 0x80                    ; Wywołanie systemowe

    ; Wczytaj liczbę z wejścia standardowego
    mov eax, 3                  ; Syscall: read
    mov ebx, 0                  ; Deskryptor pliku: stdin
    mov ecx, liczba_bufor       ; Adres bufora na dane wejściowe
    mov edx, 12                 ; Maksymalna długość wejścia
    int 0x80                    ; Wywołanie systemowe

    ; Oblicz sumę cyfr
    xor ebx, ebx                ; Inicjalizacja: suma cyfr = 0
    xor ecx, ecx                ; Inicjalizacja: indeks = 0

petla_suma:
    mov al, [liczba_bufor + ecx] ; Pobierz aktualny znak z bufora
    cmp al, 0xA                 ; Sprawdź, czy to znak nowej linii
    je wyswietl_wynik           ; Jeśli tak, zakończ pętlę
    sub al, '0'                 ; Zamień znak ASCII na cyfrę
    add ebx, eax                ; Dodaj cyfrę do sumy
    inc ecx                     ; Przejdź do kolejnego znaku
    jmp petla_suma              ; Powtórz pętlę

wyswietl_wynik:
    mov [suma_cyfr], ebx        ; Zapisz wynik sumy cyfr w zmiennej

    ; Wyświetl komunikat "Suma cyfr: "
    mov eax, 4                  ; Syscall: write
    mov ebx, 1                  ; Deskryptor pliku: stdout
    mov ecx, komunikat_wynik    ; Adres komunikatu
    mov edx, 12                 ; Długość komunikatu
    int 0x80                    ; Wywołanie systemowe

    ; Konwertuj sumę cyfr na tekst i wyświetl
    mov eax, [suma_cyfr]        ; Pobierz wartość sumy cyfr
    lea ecx, [suma_bufor]       ; Użyj oddzielnego bufora na wynik
    call konwertuj_na_ascii     ; Wywołaj funkcję konwersji liczby na ASCII
    mov eax, 4                  ; Syscall: write
    mov ebx, 1                  ; Deskryptor pliku: stdout
    mov ecx, suma_bufor         ; Adres tekstu reprezentującego wynik
    mov edx, 12                 ; Długość tekstu (maksymalnie 12 znaków)
    int 0x80                    ; Wywołanie systemowe

    ; Dodaj znak nowej linii
    mov eax, 4                  ; Syscall: write
    mov ebx, 1                  ; Deskryptor pliku: stdout
    mov ecx, nowa_linia         ; Adres znaku nowej linii
    mov edx, 1                  ; Długość danych (1 bajt)
    int 0x80                    ; Wywołanie systemowe

    ; Zakończenie programu
    mov eax, 1                  ; Syscall: exit
    xor ebx, ebx                ; Kod wyjścia: 0
    int 0x80                    ; Wywołanie systemowe

; Funkcja: Konwertuje liczbę całkowitą na tekst ASCII
; Argument: eax - liczba
; Wynik: zapis w buforze wskazanym w ecx
konwertuj_na_ascii:
    mov edx, 0                  ; Zeruj edx przed div
    mov esi, 10                 ; Podstawa dziesiętna
    lea edi, [ecx + 11]         ; Wskaźnik na koniec bufora
    mov byte [edi], 0           ; Zakończenie stringa null-terminatorem

konwertuj_petla:
    xor edx, edx                ; Zeruj edx przed dzieleniem
    div esi                     ; eax / 10, reszta w edx
    add dl, '0'                 ; Zamień cyfrę na znak ASCII
    dec edi                     ; Przesuń wskaźnik w lewo
    mov [edi], dl               ; Zapisz znak w buforze
    test eax, eax               ; Sprawdź, czy liczba wynosi 0
    jnz konwertuj_petla         ; Jeśli nie, kontynuuj pętlę

    lea ecx, [edi]              ; Ustaw wskaźnik na początek liczby
    ret                         ; Powrót z funkcji
