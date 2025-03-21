#include <iostream>
#include <cstdlib> 
#include <ctime>   

// Structure of a cyclic list node
struct Node {
    int data;
    Node* next;
};

// Structure of a cyclic list
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
    Node* newNode = new Node(); 
    newNode->data = value;

    if (list.any == nullptr) { 
        newNode->next = newNode; // Node points to itself
        list.any = newNode; // Set it as the default node
    } else {
        // Insert the new node after any node
        newNode->next = list.any->next;
        list.any->next = newNode;
    }
    list.size++;
}

// Merging two cyclic lists
void merge(CyclicList &list1, CyclicList &list2) {
    if (list1.any == nullptr) { 
        list1 = list2; // The first list becomes the second list
        return;
    }
    if (list2.any == nullptr) return; 

    // Merge the lists
    Node* temp = list1.any->next;
    list1.any->next = list2.any->next; 
    list2.any->next = temp; 
    list1.any = list2.any; 
    list1.size += list2.size; 
    list2.any = nullptr; 
    list2.size = 0;
}

// Searching for a number in the list and returning the number of comparisons
int search(const CyclicList &list, int value) {
    if (list.any == nullptr) return 0;

    int comparisons = 0; // Comparison counter
    Node* current = list.any;

    do {
        comparisons++;
        if (current->data == value) return comparisons; // Number found
        current = current->next;
    } while (current != list.any); // Traverse the entire list

    return comparisons; 
}

int main() {
    std::srand(std::time(0)); 

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
        int index = std::rand() % N;
        totalComparisons += search(L, T[index]); 
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