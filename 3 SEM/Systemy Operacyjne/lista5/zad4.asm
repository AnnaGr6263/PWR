;   Anna Grelewska
;   zadanie 4 lista 5
;   x86 (32-bit)

;   nasm -f elf32 zad4.asm -o zad4.o
;   ld -m elf_i386 -o zad4 zad4.o
;   ./zad4

section .bss
    buffer resb 12                 ; Bufor do przechowywania liczby (maks. 10 cyfr + '\n' + null terminator)

section .text
    global _start                  ; Punkt wejścia dla linkera

_start:
    ; Inicjalizacja programu
    mov ebx, 2                    ; Rozpoczynamy od liczby 2 (pierwsza liczba pierwsza)

start:
    ; Sprawdzamy, czy liczba przekracza 100000
    cmp ebx, 100000               ; Porównanie aktualnej liczby (ebx) z limitem 100000
    ja end                        ; Jeśli liczba > 100000, zakończ program

    ; Przygotowanie do sprawdzania, czy liczba jest pierwsza
    mov ecx, 2                    ; Ustaw dzielnik na 2
    mov eax, ebx                  ; Przenieś aktualną liczbę do rejestru eax
    mov edx, 0                    ; Zerujemy edx, wymagane dla operacji dzielenia

check_prime:
    ; Sprawdzanie, czy liczba jest podzielna przez dzielnik
    cmp ecx, ebx                  ; Porównanie dzielnika z aktualną liczbą
    je print_number               ; Jeśli dzielnik == liczba, oznacza to, że liczba jest pierwsza

    mov eax, ebx                  ; Załaduj aktualną liczbę (ebx) do eax
    xor edx, edx                  ; Zerowanie edx przed dzieleniem
    div ecx                       ; Dzielenie eax przez dzielnik ecx, reszta w edx
    cmp edx, 0                    ; Sprawdzenie, czy reszta = 0
    je not_prime                  ; Jeśli reszta = 0, liczba nie jest pierwsza

    inc ecx                       ; Zwiększ dzielnik o 1
    jmp check_prime               ; Powtórz sprawdzanie

not_prime:
    ; Przejście do kolejnej liczby
    inc ebx                       ; Zwiększamy aktualną liczbę o 1
    jmp start                     ; Powrót do początku pętli

print_number:
    ; Wypisanie liczby pierwszej
    push ebx                      ; Zachowanie rejestru ebx na stosie
    mov eax, ebx                  ; Przekazujemy liczbę do funkcji konwersji na tekst
    call int_to_string            ; Konwertujemy liczbę w eax na tekst w buforze
    mov eax, 4                    ; Wywołanie systemowe write (kod systemowy 4)
    mov ebx, 1                    ; Deskryptor pliku: standardowe wyjście (stdout)
    mov ecx, buffer               ; Adres bufora z tekstem liczby
    mov edx, 12                   ; Maksymalna liczba bajtów do wypisania
    int 0x80                      ; Wywołanie systemowe write
    pop ebx                       ; Przywracamy wartość rejestru ebx ze stosu

    inc ebx                       ; Przejście do następnej liczby
    jmp start                     ; Powrót do początku pętli

end:
    ; Zakończenie programu
    mov eax, 1                    ; Wywołanie systemowe exit (kod systemowy 1)
    int 0x80                      ; Wywołanie systemowe zakończenia programu

; Funkcja konwertująca liczbę na tekst (ASCII) w buforze
; Wejście: eax = liczba do konwersji
; Wyjście: bufor `buffer` zawiera tekst liczby zakończony '\n'
int_to_string:
    mov ecx, buffer + 11          ; Wskaźnik na ostatni bajt bufora
    mov byte [ecx], 10            ; Dodanie znaku nowej linii '\n' na końcu tekstu
    dec ecx                       ; Przesunięcie wskaźnika na lewo (przed '\n')

.convert_loop:
    xor edx, edx                  ; Zerowanie edx przed dzieleniem
    div dword [ten]               ; Dzielenie eax przez 10, reszta w edx
    add dl, '0'                   ; Konwersja reszty (cyfry) na znak ASCII
    mov [ecx], dl                 ; Zapisanie znaku ASCII w buforze
    dec ecx                       ; Przesunięcie wskaźnika na lewo
    test eax, eax                 ; Sprawdzamy, czy pozostała część liczby to 0
    jnz .convert_loop             ; Jeśli nie, kontynuuj pętlę konwersji

    inc ecx                       ; Ustaw wskaźnik na pierwszy znak liczby
    ret                           ; Powrót do miejsca wywołania funkcji

section .data
    ten: dd 10                    ; Stała używana w operacjach dzielenia przez 10
