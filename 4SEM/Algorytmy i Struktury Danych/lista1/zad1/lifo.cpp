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

// Class implementing LIFO stack
class Stack {
private:
    Node* top; // Pointer to the top of the stack
   
public:
    // Constructor
    Stack() {
        top = nullptr;
    }

    // Adding an element to the stack (at the top)
    void push(int value) {
        Node* newNode = new Node(value);    // Create a new node
        newNode->next = top;       // New node points to the current top of the stack
        top = newNode;      // New node becomes the new top of the stack

        std::cout << "Added to stack: " << value << std::endl;
    }

    // Removing an element from the stack (from the top)
    void pop() {
        if(!top) {     // If the stack is empty
            std::cerr << "Error! Attempt to remove an element from an empty stack!" << std::endl;
            return;
        }

        Node* temp = top;      // Pointer to the first node
        std::cout << "Removed from stack: " << top->data << std::endl;

        top = top->next;
        delete temp;
    }

    // Checking if the stack is empty
    bool isEmpty() {
        return top == nullptr;
    }
    // Destructor freeing memory
    ~Stack() {
        while (!isEmpty()) {
            pop();  // Remove all elements from the stack
        }
    }
};

int main() {
    Stack stack;  // Create a stack

    // Add 50 elements to the stack
    for (int i = 1; i <= 50; i++) {
        stack.push(i);
    }

    // Remove elements from the stack
    while (!stack.isEmpty()) {
        stack.pop();
    }

    return 0;
}

