#pragma once

template <typename T>
struct UtlMemory {
    T& operator[](int i) const noexcept { return memory[i]; }

    T* memory;
    int iAllocationCount;
    int iGrowSize;
};

template <typename Key, typename Value>
struct Node {
    int iLeft;
    int iRight;
    int iParent;
    int iType;
    Key kKey;
    Value vValue;
};

template <typename Key, typename Value>
struct UtlMap {
    auto begin() const noexcept { return memory.memory; }
    auto end() const noexcept { return memory.memory + iNumElements; }

    int find(Key kKey) const noexcept
    {
        auto curr = iRoot;

        while (curr != -1) {
            const auto el = memory[curr];

            if (el.kKey < kKey)
                curr = el.iRight;
            else if (el.kKey > kKey)
                curr = el.iLeft;
            else
                break;
        }
        return curr;
    }

    void* lessFunc;
    UtlMemory<Node<Key, Value>> memory;
    int iRoot;
    int iNumElements;
    int iFirstFree;
    int iLastAlloc;
    Node<Key, Value>* nElements;
};
