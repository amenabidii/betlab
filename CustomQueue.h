#ifndef CUSTOMQUEUE_H
#define CUSTOMQUEUE_H
#include <iostream>
using namespace std;

// Linked queue written for the project. First event added is first processed.
template <typename T>
class CustomQueue {
private:
    struct Node {
        T value;
        Node* next;
        Node(T item) { value = item; next = nullptr; }
    };
    Node* frontNode;
    Node* backNode;
    int count;
public:
    CustomQueue() { frontNode = nullptr; backNode = nullptr; count = 0; }
    CustomQueue(const CustomQueue& other) {
        frontNode = nullptr; backNode = nullptr; count = 0;
        Node* current = other.frontNode;
        while (current != nullptr) { enqueue(current->value); current = current->next; }
    }
    CustomQueue& operator=(const CustomQueue& other) {
        if (this != &other) {
            clear();
            Node* current = other.frontNode;
            while (current != nullptr) { enqueue(current->value); current = current->next; }
        }
        return *this;
    }
    ~CustomQueue() { clear(); }
    void enqueue(T item) {
        Node* added = new Node(item);
        if (backNode == nullptr) frontNode = added;
        else backNode->next = added;
        backNode = added;
        count++;
    }
    bool dequeue(T& result) {
        if (frontNode == nullptr) return false;
        Node* old = frontNode;
        result = old->value;
        frontNode = frontNode->next;
        if (frontNode == nullptr) backNode = nullptr;
        delete old; count--;
        return true;
    }
    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
    void clear() {
        T ignored;
        while (dequeue(ignored)) {}
    }
    // Visit a copy, so viewing the queue does not change the real order.
    template <typename Visitor>
    void forEach(Visitor show) const {
        Node* current = frontNode;
        while (current != nullptr) { show(current->value); current = current->next; }
    }
};
#endif
