;   Anna Grelewska
;   zadanie 3 lista 5
;   x86 (64-bit)

;   nasm -f elf64 zad3.asm -o zad3.o
;   ld -o zad3 zad3.o
;   ./zad3


section .data
    mes:    db '0'                ; Bufor na jeden znak do wypisania
    len:    equ $-mes             ; Długość bufora (zawsze 1 znak)
    nl:     db 0xA                ; Znak nowej linii (kod ASCII 0xA)

section .text
    global _start

_start:
    ; Inicjalizacja zmiennych
    mov r10, 0                   ; Licznik znaków (ilość cyfr i liter w liczbie w systemie szesnastkowym)
    mov eax, 123456              ; Liczba 32-bitowa, którą należy przekonwertować na system szesnastkowy

    ; Pętla konwersji liczby na system szesnastkowy
convert_to_hex:
    xor rdx, rdx                 ; Zerowanie rejestru rdx, aby był gotowy na resztę z dzielenia
    mov r8, 16                   ; Ustawienie bazy systemu liczbowego (16 dla systemu szesnastkowego)
    div r8                       ; Dzielenie eax przez 16: eax = eax / 16, reszta zapisana w rdx
    push rdx                     ; Zapisz resztę (pojedynczą cyfrę lub literę hex) na stos
    inc r10                      ; Zwiększ licznik znaków, ponieważ mamy nową cyfrę/literę
    cmp rax, 0                   ; Sprawdź, czy dzielenie zakończone (czy wartość w eax = 0)
    jne convert_to_hex           ; Jeśli nie (eax != 0), powtórz pętlę dla dalszej konwersji

    ; Pętla wypisywania znaków w odwrotnej kolejności (ze stosu)
print_hex:
    xor r9, r9                   ; Zerowanie r9 przed pobraniem wartości ze stosu
    pop r9                       ; Pobierz ostatnią resztę (cyfrę lub literę hex) ze stosu
    cmp r9, 10                   ; Sprawdź, czy wartość < 10 (cyfra w systemie dziesiętnym)
    jl is_digit                  ; Jeśli tak, przekształć ją na cyfrę ASCII (0-9)
    add r9, 55                   ; Jeśli nie, przekształć ją na literę ASCII (A-F)
    jmp write_char               ; Skocz do wypisania znaku

is_digit:
    add r9, 48                   ; Zamień cyfrę (0-9) na odpowiadający jej znak ASCII

write_char:
    mov [mes], r9b               ; Zapisz wynikowy znak w buforze `mes`
    mov rax, 1                   ; Syscall: write (systemowe wypisywanie)
    mov rdi, 1                   ; Deskryptor pliku: stdout (standardowe wyjście)
    mov rsi, mes                 ; Adres bufora z znakiem do wypisania
    mov rdx, 1                   ; Długość danych do wypisania (1 znak)
    syscall                      ; Wywołanie systemowe wypisania znaku
    dec r10                      ; Zmniejsz licznik znaków (wskazuje, ile znaków pozostało do wypisania)
    cmp r10, 0                   ; Sprawdź, czy wszystkie znaki zostały wypisane
    jne print_hex                ; Jeśli nie (r10 > 0), kontynuuj wypisywanie znaków

    ; Dodanie znaku nowej linii po wypisaniu całej liczby
    mov rax, 1                   ; Syscall: write
    mov rdi, 1                   ; Deskryptor pliku: stdout
    mov rsi, nl                  ; Adres bufora z znakiem nowej linii
    mov rdx, 1                   ; Długość danych do wypisania (1 znak)
    syscall                      ; Wywołanie systemowe wypisania nowej linii

    ; Zakończenie programu
    mov rax, 60                  ; Syscall: exit (zakończenie programu)
    xor rdi, rdi                 ; Kod wyjścia: 0 (sukces)
    syscall                      ; Wywołanie systemowe zakończenia programu
