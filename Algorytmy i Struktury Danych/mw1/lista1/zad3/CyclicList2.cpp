#include <iostream>
#include <cstdlib>
#include <ctime>

// Struktura węzła listy dwukierunkowej cyklicznej
struct Node {
    int data;
    Node* next;
    Node* prev;
};

// Struktura listy dwukierunkowej cyklicznej
struct DoublyCyclicList {
    Node* any;
    int size;

    DoublyCyclicList() {
        any = nullptr;
        size = 0;
    }
};

// Wstawianie liczby do listy dwukierunkowej cyklicznej
void insert(DoublyCyclicList &list, int value) {
    Node* newNode = new Node();
    newNode->data = value;
    
    if (!list.any) { // Jeśli lista jest pusta
        newNode->next = newNode;
        newNode->prev = newNode;
        list.any = newNode;
    } else {
        Node* last = list.any->prev; // Znajdujemy ostatni element listy
        newNode->next = list.any; // Nowy węzeł wskazuje na początek listy
        newNode->prev = last; // Nowy węzeł wskazuje na poprzedni element
        last->next = newNode; 
        list.any->prev = newNode; // Poprzedni element wskazuje na nowy węzeł
    }
    list.size++;
}

// Łączenie dwóch list dwukierunkowych cyklicznych
void merge(DoublyCyclicList& list1, DoublyCyclicList& list2) {
    if (!list1.any) {
        list1.any = list2.any;
    } else if (!list2.any) {
        return;
    } else {
        // Znajdujemy ostatnie elementy obu list
        Node* last1 = list1.any->prev; 
        Node* last2 = list2.any->prev; 

        last1->next = list2.any; // Ostatni element listy 1 wskazuje na początek listy 2
        list2.any->prev = last1; // Początek listy 2 wskazuje na ostatni element listy 1
        
        last2->next = list1.any; // Ostatni element listy 2 wskazuje na początek listy 1
        list1.any->prev = last2; // Początek listy 1 wskazuje na ostatni element listy 2
    }
    list1.size += list2.size;
    list2.any = nullptr;
    list2.size = 0;
}

// Wypisywanie elementów listy dwukierunkowej cyklicznej
void print(DoublyCyclicList& list) {
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

// Wyszukiwanie liczby w losowym kierunku
int search(DoublyCyclicList& list, int value) {
    if (!list.any) return 0;

    Node* temp = list.any;
    int comparisons = 0;
    bool forward = std::rand() % 2;
    
    do {
        comparisons++;
        if (temp->data == value) {
            return comparisons;
        }
        temp = forward ? temp->next : temp->prev;
    } while (temp != list.any);
    
    return comparisons;
}

int main() {

        // Tworzenie nowej listy
        DoublyCyclicList list3;

        // Wstawianie liczb od 1 do 10
        for (int i = 1; i <= 10; i++) {
            insert(list3, i);
        }
       
        // Wypisanie zawartości listy
        std::cout << "Lista 3 po wstawieniu elementów: ";
        print(list3);
       
    std::srand(std::time(0));

    // Tworzenie i scalanie dwóch list
    DoublyCyclicList list1, list2;
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

    // Analiza kosztu wyszukiwania
    const int T_SIZE = 10000;
    const int SEARCH_COUNT = 1000;
    const int VALUE_RANGE = 100001;
    
    int T[T_SIZE];
    DoublyCyclicList L;

    for (int i = 0; i < T_SIZE; i++) {
        T[i] = std::rand() % VALUE_RANGE;
        insert(L, T[i]);
    }

    long long totalComparisonsFound = 0;
    for (int i = 0; i < SEARCH_COUNT; i++) {
        int randomIndex = std::rand() % T_SIZE;
        totalComparisonsFound += search(L, T[randomIndex]);
    }
    double avgComparisonsFound = (double)totalComparisonsFound / SEARCH_COUNT;

    long long totalComparisonsRandom = 0;
    for (int i = 0; i < SEARCH_COUNT; i++) {
        int randomValue = std::rand() % VALUE_RANGE;
        totalComparisonsRandom += search(L, randomValue);
    }
    double avgComparisonsRandom = (double)totalComparisonsRandom / SEARCH_COUNT;

    std::cout << "\nSredni koszt wyszukiwania liczby obecnej na liscie: " << avgComparisonsFound << " porownan\n";
    std::cout << "Sredni koszt wyszukiwania losowej liczby: " << avgComparisonsRandom << " porownan\n";

    return 0;
}
