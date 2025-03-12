
#include <iostream>

// Struktura węzła listy jednokierunkowej
struct Node {
    int data;
    Node* next;

    // Konstruktor - 
    Node(int value) {
        data = value;
        next = nullptr;
    }
};

// Klasa implementujaca kolejke FIFO
class Queue {
private:
    Node* head; // Wskaznik na poczatek kolejki
    Node* tail; // Wskaznik na koniec kolejki

public:
    // Konstruktor
    Queue() {
        head = nullptr;
        tail = nullptr;
    }

    // Dodawanie elementu do kolejki (na koniec)
    void push(int value) {
        Node* newNode = new Node(value);    // Tworzymy nowy wezel

        if(!tail) {     // jesli kolejka pusta
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }

        std::cout << "Dodano do kolejki: " << value << std::endl;
    }

    // Usuniecie elementu z kolejki (z poczatku)
    void pop() {
        if(!head) {     // Jesli kolejka jest pusta
            std::cerr << "Błąd! Próba usunięcia elementu z pustej kolejki!" << std::endl;
            return;
        }

        Node* temp = head;      // Wskaznik na pierwszy wezel
        std::cout << "Usunięto z kolejki: " << head->data << std::endl;

        head = head->next;
        delete temp;

        if (!head) {  // Jeśli kolejka jest teraz pusta
            tail = nullptr;  // Zerujemy tail
        }
    }
    // Sprawdzenie, czy kolejka jest pusta
    bool isEmpty() {
        return head == nullptr;
    }
    // Destruktor zwalniający pamięć
    ~Queue() {
        while (!isEmpty()) {
            pop();  // Usuwamy wszystkie elementy z kolejki
        }
    }
};

int main() {
    Queue queue;  // Tworzymy kolejkę

    // Dodajemy 50 elementów do kolejki
    for (int i = 1; i <= 50; i++) {
        queue.push(i);
    }

    // Pobieramy elementy z kolejki
    while (!queue.isEmpty()) {
        queue.pop();
    }

    return 0;
}

