#include "user.h"

int main(int argc, char *argv[]) {
    // Sprawdzenie liczby argumentów
    if (argc != 4) {
        // Jeśli argumenty są niepoprawne, wyświetl komunikat i zakończ program
        printf(1, "Usage: expr <num1> <operator> <num2>\n");
        exit();
    }

    // Pobranie liczb z argumentów wejściowych
    int num1 = atoi(argv[1]);  // Konwersja pierwszego argumentu na liczbę całkowitą
    int num2 = atoi(argv[3]);  // Konwersja trzeciego argumentu na liczbę całkowitą

    // Pobranie operatora z drugiego argumentu
    char operator = argv[2][0];

    int result;  // Zmienna przechowująca wynik operacji

    // Wykonanie operacji arytmetycznej na podstawie operatora
    switch (operator) {
        case '+':
            result = num1 + num2;  // Dodawanie
            break;
        case '-':
            result = num1 - num2;  // Odejmowanie
            break;
        case '*':
            result = num1 * num2;  // Mnożenie
            break;
        case '/':
            // Sprawdzenie dzielenia przez zero
            if (num2 == 0) {
                printf(1, "Error: Division by zero\n");
                exit();
            }
            result = num1 / num2;  // Dzielenie całkowite
            break;
        default:
            // Obsługa nieznanego operatora
            printf(1, "Error: Unknown operator '%c'\n", operator);
            exit();
    }

    // Wyświetlenie wyniku operacji
    printf(1, "%d\n", result);

    exit();
}
