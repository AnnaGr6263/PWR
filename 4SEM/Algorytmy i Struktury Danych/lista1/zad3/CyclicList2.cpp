#include <iostream>
#include <cstdlib> 
#include <ctime>  

// Structure of a doubly linked cyclic list node
struct Node {
    int data;  
    Node* next; 
    Node* prev; 
};

// Structure of a doubly linked cyclic list
struct CyclicList {
    Node* any; // Pointer to any node in the list
    int size;  // Number of elements in the list

    // Constructor
    CyclicList() {
        any = nullptr;
        size = 0;
    }
};

// Inserting a number into the list
void insert(CyclicList &list, int value) {
    Node* newNode = new Node(); // Create a new node
    newNode->data = value;

    if (list.any == nullptr) { // If the list is empty
        newNode->next = newNode; // Node points to itself
        newNode->prev = newNode; // Node points to itself
        list.any = newNode;      // Set it as any node
    } else {
        // Insert the new node after any node
        newNode->next = list.any->next;
        newNode->prev = list.any;
        list.any->next->prev = newNode;
        list.any->next = newNode;
    }
    list.size++; 
}

// Merging two doubly linked cyclic lists
void merge(CyclicList &list1, CyclicList &list2) {
    if (list1.any == nullptr) { 
        list1 = list2; // The first list becomes the second list
        return;
    }
    if (list2.any == nullptr) return; 

    // Merge the lists
    Node* temp1 = list1.any->next;
    Node* temp2 = list2.any->next; 

    list1.any->next = temp2; 
    temp2->prev = list1.any; 

    list2.any->next = temp1; 
    temp1->prev = list2.any; 

    list1.any = list2.any;
    list1.size += list2.size; 

    list2.any = nullptr; // List2 is now empty
    list2.size = 0;
}

// Searching for a number in the list and returning the number of comparisons
int search(const CyclicList &list, int value) {
    if (list.any == nullptr) return 0; 

    int comparisons = 0; // Comparison counter
    Node* current = list.any;

    // Randomly choose the search direction (forward or backward)
    bool forward = std::rand() % 2 == 0;

    if (forward) {
        // Search forward
        do {
            comparisons++;
            if (current->data == value) return comparisons; // Number found
            current = current->next;
        } while (current != list.any);
    } else {
        // Search backward
        do {
            comparisons++;
            if (current->data == value) return comparisons; // Number found
            current = current->prev;
        } while (current != list.any);
    }

    return comparisons;
}

int main() {
    std::srand(std::time(0)); // Initialize the random number generator

    // Create two lists
    CyclicList list1, list2;

    // Insert 10 random numbers into list1 and list2
    for (int i = 0; i < 10; i++) {
        insert(list1, std::rand() % 90 + 10); // Numbers in the range [10, 99]
        insert(list2, std::rand() % 90 + 10);
    }

    // Merge the lists
    merge(list1, list2);

    // Create a list with 10,000 random numbers
    const int N = 10000;
    const int I_MAX = 100000;
    int T[N];
    CyclicList L;

    for (int i = 0; i < N; i++) {
        T[i] = std::rand() % (I_MAX + 1); // Numbers in the range [0, 100000]
        insert(L, T[i]); // Insert numbers into the list
    }

    // Analyze the search for numbers present in the list
    int totalComparisons = 0;
    for (int i = 0; i < 1000; i++) {
        int index = std::rand() % N; // Random index from array T
        totalComparisons += search(L, T[index]); // Search for the number from T
    }
    std::cout << "Average cost of searching for numbers present in the list: " 
              << (double)totalComparisons / 1000 << " comparisons\n";

    // Analyze the search for random numbers not in the list
    totalComparisons = 0;
    for (int i = 0; i < 1000; i++) {
        int randomValue = std::rand() % (I_MAX + 1); // Random number in the range [0, 100000]
        totalComparisons += search(L, randomValue); // Search for the random number
    }
    std::cout << "Average cost of searching for random numbers: " 
              << (double)totalComparisons / 1000 << " comparisons\n";

    return 0;
}