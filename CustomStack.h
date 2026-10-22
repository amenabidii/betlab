#ifndef CUSTOMSTACK_H
#define CUSTOMSTACK_H
// Simple linked stack; last saved value comes out first.
template <typename T>
class CustomStack {
private:
    struct Node {
        T value;
        Node* next;
        Node(T item, Node* oldTop) { value = item; next = oldTop; }
    };
    Node* topNode;
    int count;
public:
    CustomStack() { topNode = nullptr; count = 0; }
    // We avoid copying the undo stack by mistake. Only the world snapshots are copied.
    CustomStack(const CustomStack&) = delete;
    CustomStack& operator=(const CustomStack&) = delete;
    ~CustomStack() { clear(); }
    void push(T value) { topNode = new Node(value, topNode); count++; }
    bool pop(T& result) {
        if (topNode == nullptr) return false;
        Node* old = topNode;
        result = old->value;
        topNode = old->next;
        delete old; count--; return true;
    }
    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
    void clear() { T ignored; while (pop(ignored)) {} }
};
#endif
