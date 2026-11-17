#pragma once
#include "Node.h"

// LIFO stack built on a singly-linked list. push / pop / peek are O(1).
template <typename T>
class LinkedStack
{
private:
    Node<T>* top;
    int      count;

    void copyFrom(const LinkedStack<T>& other)
    {
        // copy in reverse so the order is preserved
        int n = 0;
        T* arr = other.toArray(n);          // top -> bottom
        for (int i = n - 1; i >= 0; i--)
            push(arr[i]);
        delete[] arr;
    }

public:
    LinkedStack() : top(nullptr), count(0) {}

    LinkedStack(const LinkedStack<T>& other) : top(nullptr), count(0)
    {
        copyFrom(other);
    }

    LinkedStack<T>& operator=(const LinkedStack<T>& other)
    {
        if (this != &other)
        {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    ~LinkedStack() { clear(); }

    bool isEmpty() const { return count == 0; }
    int  getCount() const { return count; }

    // O(1)
    bool push(const T& newItem)
    {
        top = new Node<T>(newItem, top);
        count++;
        return true;
    }

    // O(1)
    bool pop(T& topItem)
    {
        if (isEmpty()) return false;
        Node<T>* temp = top;
        topItem = top->getItem();
        top = top->getNext();
        delete temp;
        count--;
        return true;
    }

    // O(1)
    bool peek(T& topItem) const
    {
        if (isEmpty()) return false;
        topItem = top->getItem();
        return true;
    }

    void clear()
    {
        T dummy;
        while (pop(dummy)) {}
    }

    // New array, top to bottom. Caller must delete[]. nullptr if empty.
    T* toArray(int& size) const
    {
        size = count;
        if (count == 0) return nullptr;
        T* arr = new T[count];
        int i = 0;
        for (Node<T>* p = top; p != nullptr; p = p->getNext())
            arr[i++] = p->getItem();
        return arr;
    }
};
