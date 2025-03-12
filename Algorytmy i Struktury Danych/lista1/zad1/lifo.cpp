
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

// Klasa implementujaca stos LIFO
class Stack {
private:
    Node* top; // Wskaznik na szczyt stosu
   
public:
    // Konstruktor
    Stack() {
        top = nullptr;
    }

    // Dodawanie elementu na stos (na gore)
    void push(int value) {
        Node* newNode = new Node(value);    // Tworzymy nowy wezel
        newNode->next = top;       // Nowy wezel wskazuje na dotychczasowy szczyt stosu
        top = newNode;      // Nowy wezel staje sie nowym szczytem stosu

        std::cout << "Dodano na stos: " << value << std::endl;
    }

    // Usuniecie elementu ze stosu (ze szczytu)
    void pop() {
        if(!top) {     // Jesli stos jest pusty
            std::cerr << "Błąd! Próba usunięcia elementu z pustego stosu!" << std::endl;
            return;
        }

        Node* temp = top;      // Wskaznik na pierwszy wezel
        std::cout << "Usunięto ze stosu: " << top->data << std::endl;

        top = top->next;
        delete temp;
    }

    // Sprawdzenie, czy stos jest pusty
    bool isEmpty() {
        return top == nullptr;
    }
    // Destruktor zwalniający pamięć
    ~Stack() {
        while (!isEmpty()) {
            pop();  // Usuwamy wszystkie elementy ze stosu
        }
    }
};

int main() {
    Stack stack;  // Tworzymy stos

    // Dodajemy 50 elementów na stos
    for (int i = 1; i <= 50; i++) {
        stack.push(i);
    }

    // Pobieramy elementy ze stosu
    while (!stack.isEmpty()) {
        stack.pop();
    }

    return 0;
}

