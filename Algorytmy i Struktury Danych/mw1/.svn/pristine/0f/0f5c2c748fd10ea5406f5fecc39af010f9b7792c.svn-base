#include <iostream>

// Struktura węzła listy jednokierunkowej
struct Node {
    int data;
    Node* next;

    // Konstruktor
    Node(int value) {
        data = value;
        next = nullptr;
    }
};

// Klasa implementująca stos LIFO
class Stack {
private:
    Node* top; // Wskaźnik na szczyt stosu
   
public:
    // Konstruktor
    Stack() {
        top = nullptr;
    }

    // Dodawanie elementu do stosu (na szczyt)
    void push(int value) {
        Node* newNode = new Node(value); // Tworzenie nowego węzła
        newNode->next = top; // Nowy węzeł wskazuje na obecny szczyt stosu
        top = newNode; // Nowy węzeł staje się nowym szczytem stosu

        std::cout << "Dodano do stosu: " << value << std::endl;
    }

    // Usuwanie elementu ze stosu (ze szczytu)
    void pop() {
        if (!top) { // Jeśli stos jest pusty
            std::cerr << "Błąd! Próba usunięcia elementu z pustego stosu!" << std::endl;
            return;
        }

        Node* temp = top; // Wskaźnik na pierwszy węzeł
        std::cout << "Usunięto ze stosu: " << top->data << std::endl;

        top = top->next;
        delete temp;
    }

    // Sprawdzanie, czy stos jest pusty
    bool isEmpty() {
        return top == nullptr;
    }
    // Destruktor zwalniający pamięć
    ~Stack() {
        while (!isEmpty()) {
            pop(); // Usuwanie wszystkich elementów ze stosu
        }
    }
};

int main() {
    Stack stack; // Tworzenie stosu

    // Dodawanie 50 elementów do stosu
    for (int i = 1; i <= 50; i++) {
        stack.push(i);
    }

    // Usuwanie elementów ze stosu
    while (!stack.isEmpty()) {
        stack.pop();
    }

    return 0;
}

