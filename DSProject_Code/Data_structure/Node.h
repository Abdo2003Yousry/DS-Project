#pragma once

// Generic singly-linked node. Used by LinkedQueue, LinkedStack and LinkedList.
template <typename T>
class Node
{
private:
    T        item;   // the stored value (for the project: usually Enemy*)
    Node<T>* next;   // pointer to the next node

public:
    Node() : next(nullptr) {}
    Node(const T& newItem) : item(newItem), next(nullptr) {}
    Node(const T& newItem, Node<T>* nextPtr) : item(newItem), next(nextPtr) {}

    void     setItem(const T& newItem) { item = newItem; }
    void     setNext(Node<T>* nextPtr) { next = nextPtr; }
    T        getItem() const           { return item; }
    Node<T>* getNext() const           { return next; }
};
