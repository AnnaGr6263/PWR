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

// Klasa implementująca kolejkę FIFO
class Queue {
private:
    Node* head; // Wskaźnik na początek kolejki
    Node* tail; // Wskaźnik na koniec kolejki

public:
    // Konstruktor
    Queue() {
        head = nullptr;
        tail = nullptr;
    }

    // Dodawanie elementu do kolejki (na koniec)
    void push(int value) {
        Node* newNode = new Node(value); // Tworzenie nowego węzła

        if (!tail) { // Jeśli kolejka jest pusta
            head = tail = newNode;
        } else {
            tail->next = newNode; // Podpięcie nowego węzła do końca kolejki
            tail = newNode; // Nowy węzeł staje się nowym końcem kolejki
        }

        std::cout << "Dodano do kolejki: " << value << std::endl;
    }

    // Usuwanie elementu z kolejki (z początku)
    void pop() {
        if (!head) { // Jeśli kolejka jest pusta
            std::cerr << "Błąd! Próba usunięcia elementu z pustej kolejki!" << std::endl;
            return;
        }

        Node* temp = head; // Wskaźnik na pierwszy węzeł
        std::cout << "Usunięto z kolejki: " << head->data << std::endl;

        head = head->next;
        delete temp;

        if (!head) { // Jeśli kolejka jest teraz pusta
            tail = nullptr; // Resetowanie wskaźnika tail
        }
    }
    // Sprawdzanie, czy kolejka jest pusta
    bool isEmpty() {
        return head == nullptr;
    }
    // Destruktor zwalniający pamięć
    ~Queue() {
        while (!isEmpty()) {
            pop(); // Usuwanie wszystkich elementów z kolejki
        }
    }
};

int main() {
    Queue queue; // Tworzenie kolejki

    // Dodawanie 50 elementów do kolejki
    for (int i = 1; i <= 50; i++) {
        queue.push(i);
    }

    // Usuwanie elementów z kolejki
    while (!queue.isEmpty()) {
        queue.pop();
    }

    return 0;
}

