#include <iostream>
#include <cstdlib>
#include <ctime>

// Struktura węzła listy cyklicznej
struct Node {
    int data;
    Node* next;
};

// Struktura listy cyklicznej
struct CyclicList {
    Node* any; // Wskaźnik na dowolny węzeł w liście
    int size;  // Liczba elementów w liście

    CyclicList() {
        any = nullptr;
        size = 0;
    }
};

// Wstawianie liczby do listy cyklicznej
void insert(CyclicList &list, int value) {
    Node* newNode = new Node();
    newNode->data = value;

    if (!list.any) { // Jeśli lista jest pusta
        newNode->next = newNode; // Wskaźnik na samego siebie (cykliczność)
        list.any = newNode;
    } else { 
        newNode->next = list.any->next; 
        list.any->next = newNode; 
    }
    list.size++;
}

// Łączenie dwóch list cyklicznych
void merge(CyclicList& list1, CyclicList& list2) {
    if (!list1.any) { // Jeśli lista 1 jest pusta, przypisz listę 2
        list1.any = list2.any;
    } else if (!list2.any) { // Jeśli lista 2 jest pusta, nic nie rób
        return;
    } else {
        Node* temp = list1.any;
        while (temp->next != list1.any) { // Znalezienie ostatniego elementu listy 1
            temp = temp->next;
        }
        temp->next = list2.any; // Podpięcie listy 2 do listy 1

        temp = list2.any;
        while (temp->next != list2.any) { // Znalezienie ostatniego elementu listy 2
            temp = temp->next;
        }
        temp->next = list1.any; // Domknięcie cyklu
    }
    list1.size += list2.size; // Aktualizacja rozmiaru
    list2.any = nullptr; // Czyszczenie listy 2
    list2.size = 0; 
}

// Wypisywanie elementów listy cyklicznej
void print(CyclicList& list) {
    if (!list.any) {
        std::cout << "Lista jest pusta.\n";
        return;
    }
    Node* temp = list.any;
    do {
        std::cout << temp->data << " ";
        temp = temp->next;
    } while (temp != list.any);
    std::cout << std::endl;
}

// Wyszukiwanie liczby w liście cyklicznej, zwraca liczbę porównań
int search(CyclicList& list, int value) {
    if (!list.any) return 0; // Pusta lista

    Node* temp = list.any;
    int comparisons = 0;
    
    do {
        comparisons++;
        if (temp->data == value) {
            return comparisons;
        }
        temp = temp->next;
    } while (temp != list.any);
    
    return comparisons; // Jeśli nie znaleziono, zwracamy ilość porównań
}

int main() {
    std::srand(std::time(0)); // Inicjalizacja generatora liczb losowych

    // Część 1: Tworzenie i scalanie dwóch list cyklicznych
    CyclicList list1, list2;

    // Wstawienie 10 losowych dwucyfrowych liczb do każdej listy
    for (int i = 0; i < 10; i++) {
        insert(list1, std::rand() % 90 + 10);
        insert(list2, std::rand() % 90 + 10);
    }

    std::cout << "Lista 1 przed scaleniem: ";
    print(list1);
    std::cout << "Lista 2 przed scaleniem: ";
    print(list2);

    merge(list1, list2);

    std::cout << "Lista po scaleniu: ";
    print(list1);

    // Część 2: Analiza kosztu wyszukiwania w liście cyklicznej
    const int T_SIZE = 10000;
    const int SEARCH_COUNT = 1000;
    const int VALUE_RANGE = 100001; // Liczby z przedziału [0, ..., 100000]
    
    int T[T_SIZE]; // Tablica liczb losowych
    CyclicList L;  // Lista cykliczna

    // Generowanie tablicy T i wstawianie do listy L
    for (int i = 0; i < T_SIZE; i++) {
        T[i] = std::rand() % VALUE_RANGE;
        insert(L, T[i]);
    }

    // Test wyszukiwania liczb z listy
    long long totalComparisonsFound = 0;
    for (int i = 0; i < SEARCH_COUNT; i++) {
        int randomIndex = std::rand() % T_SIZE;
        totalComparisonsFound += search(L, T[randomIndex]);
    }
    double avgComparisonsFound = (double)totalComparisonsFound / SEARCH_COUNT;

    // Test wyszukiwania losowych liczb z przedziału I
    long long totalComparisonsRandom = 0;
    for (int i = 0; i < SEARCH_COUNT; i++) {
        int randomValue = std::rand() % VALUE_RANGE;
        totalComparisonsRandom += search(L, randomValue);
    }
    double avgComparisonsRandom = (double)totalComparisonsRandom / SEARCH_COUNT;

    // Wyniki
    std::cout << "Średni koszt wyszukiwania liczby obecnej na liście: " << avgComparisonsFound << " porównań\n";
    std::cout << "Średni koszt wyszukiwania losowej liczby: " << avgComparisonsRandom << " porównań\n";

    return 0;
}
