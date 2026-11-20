#pragma once

// Max-heap priority queue (array-based, grows automatically).
//   insert     : O(log n)
//   extractMax : O(log n)
//   peek       : O(1)
// Items with EQUAL priority leave in insertion order (FIFO), so results are
// deterministic.
//
// Usage in the project:
//  * Active fighters: priority = value of Enemy::CalcPriority().
//  * Killed list    : priority = -(KTS * 100000 + FD)  -> smallest KTS first,
//                     ties broken by smaller FD, and then by insertion order.
template <typename T>
class PriorityQueue
{
private:
    T*      items;     // heap array
    double* keys;      // priority of each item
    long*   seq;       // insertion order (tie-breaker)
    int     count;
    int     capacity;
    long    nextSeq;

    // true if element i should be closer to the root than element j
    bool higher(int i, int j) const
    {
        if (keys[i] != keys[j]) return keys[i] > keys[j];
        return seq[i] < seq[j];
    }

    void swapNodes(int i, int j)
    {
        T      ti = items[i]; items[i] = items[j]; items[j] = ti;
        double tk = keys[i];  keys[i]  = keys[j];  keys[j]  = tk;
        long   ts = seq[i];   seq[i]   = seq[j];   seq[j]   = ts;
    }

    void heapifyUp(int i)
    {
        while (i > 0)
        {
            int parent = (i - 1) / 2;
            if (!higher(i, parent)) break;
            swapNodes(i, parent);
            i = parent;
        }
    }

    void heapifyDown(int i)
    {
        while (true)
        {
            int left = 2 * i + 1, right = 2 * i + 2, best = i;
            if (left  < count && higher(left,  best)) best = left;
            if (right < count && higher(right, best)) best = right;
            if (best == i) break;
            swapNodes(i, best);
            i = best;
        }
    }

    void grow()
    {
        int newCap = capacity * 2;
        T*      ni = new T[newCap];
        double* nk = new double[newCap];
        long*   ns = new long[newCap];
        for (int i = 0; i < count; i++)
        {
            ni[i] = items[i];
            nk[i] = keys[i];
            ns[i] = seq[i];
        }
        delete[] items; delete[] keys; delete[] seq;
        items = ni; keys = nk; seq = ns;
        capacity = newCap;
    }

    void allocate(int cap)
    {
        capacity = cap;
        items = new T[capacity];
        keys  = new double[capacity];
        seq   = new long[capacity];
    }

public:
    PriorityQueue(int initialCapacity = 16)
        : count(0), nextSeq(0)
    {
        allocate(initialCapacity > 0 ? initialCapacity : 16);
    }

    PriorityQueue(const PriorityQueue<T>& other)
        : count(other.count), nextSeq(other.nextSeq)
    {
        allocate(other.capacity);
        for (int i = 0; i < count; i++)
        {
            items[i] = other.items[i];
            keys[i]  = other.keys[i];
            seq[i]   = other.seq[i];
        }
    }

    PriorityQueue<T>& operator=(const PriorityQueue<T>& other)
    {
        if (this != &other)
        {
            delete[] items; delete[] keys; delete[] seq;
            count = other.count;
            nextSeq = other.nextSeq;
            allocate(other.capacity);
            for (int i = 0; i < count; i++)
            {
                items[i] = other.items[i];
                keys[i]  = other.keys[i];
                seq[i]   = other.seq[i];
            }
        }
        return *this;
    }

    ~PriorityQueue()
    {
        delete[] items;
        delete[] keys;
        delete[] seq;
    }

    bool isEmpty() const { return count == 0; }
    int  getCount() const { return count; }

    // O(log n)
    bool insert(const T& newItem, double priority)
    {
        if (count == capacity) grow();
        items[count] = newItem;
        keys[count]  = priority;
        seq[count]   = nextSeq++;
        heapifyUp(count);
        count++;
        return true;
    }

    // O(log n) : removes and returns the highest-priority item
    bool extractMax(T& maxItem)
    {
        if (isEmpty()) return false;
        maxItem = items[0];
        count--;
        if (count > 0)
        {
            items[0] = items[count];
            keys[0]  = keys[count];
            seq[0]   = seq[count];
            heapifyDown(0);
        }
        return true;
    }

    // Same as extractMax but also returns the priority value
    bool extractMax(T& maxItem, double& priority)
    {
        if (isEmpty()) return false;
        priority = keys[0];
        return extractMax(maxItem);
    }

    // O(1)
    bool peek(T& maxItem) const
    {
        if (isEmpty()) return false;
        maxItem = items[0];
        return true;
    }

    void clear()
    {
        count = 0;
        nextSeq = 0;
    }

    // New array in heap order (NOT sorted). Caller must delete[].
    // Good enough for drawing / iterating over all items.
    T* toArray(int& size) const
    {
        size = count;
        if (count == 0) return nullptr;
        T* arr = new T[count];
        for (int i = 0; i < count; i++) arr[i] = items[i];
        return arr;
    }
};
