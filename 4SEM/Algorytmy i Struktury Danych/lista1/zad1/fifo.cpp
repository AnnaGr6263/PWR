#include <iostream>

// Structure of a singly linked list node
struct Node {
    int data;
    Node* next;

    // Constructor
    Node(int value) {
        data = value;
        next = nullptr;
    }
};

// Class implementing FIFO queue
class Queue {
private:
    Node* head; // Pointer to the beginning of the queue
    Node* tail; // Pointer to the end of the queue

public:
    // Constructor
    Queue() {
        head = nullptr;
        tail = nullptr;
    }

    // Adding an element to the queue (at the end)
    void push(int value) {
        Node* newNode = new Node(value);    // Create a new node

        if(!tail) {     // if the queue is empty
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }

        std::cout << "Added to queue: " << value << std::endl;
    }

    // Removing an element from the queue (from the beginning)
    void pop() {
        if(!head) {     // If the queue is empty
            std::cerr << "Error! Attempt to remove an element from an empty queue!" << std::endl;
            return;
        }

        Node* temp = head;      // Pointer to the first node
        std::cout << "Removed from queue: " << head->data << std::endl;

        head = head->next;
        delete temp;

        if (!head) {  // If the queue is now empty
            tail = nullptr;  // Reset tail
        }
    }
    // Checking if the queue is empty
    bool isEmpty() {
        return head == nullptr;
    }
    // Destructor freeing memory
    ~Queue() {
        while (!isEmpty()) {
            pop();  // Remove all elements from the queue
        }
    }
};

int main() {
    Queue queue;  // Create a queue

    // Add 50 elements to the queue
    for (int i = 1; i <= 50; i++) {
        queue.push(i);
    }

    // Remove elements from the queue
    while (!queue.isEmpty()) {
        queue.pop();
    }

    return 0;
}

