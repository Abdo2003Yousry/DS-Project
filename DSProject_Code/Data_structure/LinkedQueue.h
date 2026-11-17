#pragma once
#include "Node.h"

// FIFO queue built on a singly-linked list with front/rear pointers.
// enqueue / dequeue / peek are all O(1).
// NOTE: the queue stores copies of T. If T is a pointer (Enemy*), the queue
// never deletes the pointed-to object; the owner (Battle) is responsible.
template <typename T>
class LinkedQueue
{
private:
    Node<T>* front;
    Node<T>* rear;
    int      count;

    void copyFrom(const LinkedQueue<T>& other)
    {
        for (Node<T>* p = other.front; p != nullptr; p = p->getNext())
            enqueue(p->getItem());
    }

public:
    LinkedQueue() : front(nullptr), rear(nullptr), count(0) {}

    LinkedQueue(const LinkedQueue<T>& other)
        : front(nullptr), rear(nullptr), count(0)
    {
        copyFrom(other);
    }

    LinkedQueue<T>& operator=(const LinkedQueue<T>& other)
    {
        if (this != &other)
        {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    ~LinkedQueue() { clear(); }

    bool isEmpty() const { return count == 0; }
    int  getCount() const { return count; }

    // O(1)
    bool enqueue(const T& newItem)
    {
        Node<T>* n = new Node<T>(newItem);
        if (rear == nullptr)
            front = rear = n;
        else
        {
            rear->setNext(n);
            rear = n;
        }
        count++;
        return true;
    }

    // O(1)
    bool dequeue(T& frontItem)
    {
        if (isEmpty()) return false;
        Node<T>* temp = front;
        frontItem = front->getItem();
        front = front->getNext();
        if (front == nullptr) rear = nullptr;
        delete temp;
        count--;
        return true;
    }

    // O(1)
    bool peek(T& frontItem) const
    {
        if (isEmpty()) return false;
        frontItem = front->getItem();
        return true;
    }

    void clear()
    {
        T dummy;
        while (dequeue(dummy)) {}
    }

    // Returns a NEW array holding all items (front to rear). Caller must delete[].
    // Useful for filling the GUI drawing array. Returns nullptr if empty.
    T* toArray(int& size) const
    {
        size = count;
        if (count == 0) return nullptr;
        T* arr = new T[count];
        int i = 0;
        for (Node<T>* p = front; p != nullptr; p = p->getNext())
            arr[i++] = p->getItem();
        return arr;
    }
};
