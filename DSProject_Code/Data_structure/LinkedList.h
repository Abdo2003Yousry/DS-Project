#pragma once
#include "Node.h"

// Singly-linked list with head/tail pointers.
//   insertBeginning / insertEnd : O(1)
//   remove(item)                : O(n)
//   iterate                     : O(n) using getHead()/getNext()
// Suitable for the FROZEN list: every step we walk over it, decrement
// the freeze counters and remove enemies whose freeze time ended.
template <typename T>
class LinkedList
{
private:
    Node<T>* head;
    Node<T>* tail;
    int      count;

    void copyFrom(const LinkedList<T>& other)
    {
        for (Node<T>* p = other.head; p != nullptr; p = p->getNext())
            insertEnd(p->getItem());
    }

public:
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}

    LinkedList(const LinkedList<T>& other)
        : head(nullptr), tail(nullptr), count(0)
    {
        copyFrom(other);
    }

    LinkedList<T>& operator=(const LinkedList<T>& other)
    {
        if (this != &other)
        {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    ~LinkedList() { clear(); }

    bool isEmpty() const { return count == 0; }
    int  getCount() const { return count; }

    // Read-only entry point for iteration:
    //   for (Node<T>* p = list.getHead(); p; p = p->getNext()) {...}
    Node<T>* getHead() const { return head; }

    // O(1)
    void insertBeginning(const T& newItem)
    {
        head = new Node<T>(newItem, head);
        if (tail == nullptr) tail = head;
        count++;
    }

    // O(1)
    void insertEnd(const T& newItem)
    {
        Node<T>* n = new Node<T>(newItem);
        if (tail == nullptr)
            head = tail = n;
        else
        {
            tail->setNext(n);
            tail = n;
        }
        count++;
    }

    // O(1)
    bool removeFirst(T& firstItem)
    {
        if (isEmpty()) return false;
        Node<T>* temp = head;
        firstItem = head->getItem();
        head = head->getNext();
        if (head == nullptr) tail = nullptr;
        delete temp;
        count--;
        return true;
    }

    // O(n) : removes the first node whose item == target
    bool remove(const T& target)
    {
        Node<T>* prev = nullptr;
        Node<T>* cur  = head;
        while (cur != nullptr)
        {
            if (cur->getItem() == target)
            {
                if (prev == nullptr) head = cur->getNext();
                else                 prev->setNext(cur->getNext());
                if (cur == tail) tail = prev;
                delete cur;
                count--;
                return true;
            }
            prev = cur;
            cur  = cur->getNext();
        }
        return false;
    }

    void clear()
    {
        T dummy;
        while (removeFirst(dummy)) {}
    }

    // New array (head to tail). Caller must delete[]. nullptr if empty.
    T* toArray(int& size) const
    {
        size = count;
        if (count == 0) return nullptr;
        T* arr = new T[count];
        int i = 0;
        for (Node<T>* p = head; p != nullptr; p = p->getNext())
            arr[i++] = p->getItem();
        return arr;
    }
};
