#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <utility>

typedef unsigned long long uint64;
typedef long long int64;

template <typename T>
class List
{
public:
    struct Node
    {
        Node() = default;

        Node(const T& value);

        ~Node();

        T getData() const;

        T* data = nullptr;
        Node* prev = nullptr;
        Node* next = nullptr;
    };

public:
    List();

    List(const List<T>& other);

    List(List<T>&& other);

    List<T>& operator=(const List<T>& other);

    List<T>& operator=(List<T>&& other);

    ~List();

    void copy(const List<T>& other);

    void move(List<T>&& other);

    void insert(uint64 index, const T& value);

    void erase(uint64 index);

    void swap(uint64 first, uint64 second);

    template <typename Pred>
    void sort(uint64 from, uint64 to, Pred pred);

    void sortLtoB(uint64 from = 0, uint64 to = -1);

    void sortBtoL(uint64 from = 0, uint64 to = -1);

    uint64 size() const;

    bool empty() const;

    T front() const;

    T back() const;

    void clear();

    T operator[](uint64 index) const;

    void pushFront(const T& value);

    void pushBack(const T& value);

    void popFront();

    void popBack();

    void print(uint64 from = 0, uint64 to = -1,
               std::ostream& out = std::cout) const;

private:
    void init();

    void add(Node* cur, const T& value);

    void remove(Node* node);

    Node* shift(Node* node, int64 n) const;

    Node* find(uint64 index) const;

    void swap(Node* first, Node* second);

    void connect(Node* left, Node* right);

    template <typename Pred>
    Node* mergeSort(Node* head, Pred pred);

    Node* split(Node* head);

    template <typename Pred>
    Node* merge(Node* first, Node* second, Pred pred);

private:
    Node* mHead;
    Node* mTail;
    uint64 mSize;
};

#endif // LIST_H

template <typename T>
List<T>::Node::Node(const T& value)
    : data(new T{value})
    , prev(nullptr)
    , next(nullptr)
{

}

template <typename T>
List<T>::Node::~Node()
{
    if (data != nullptr) {
        delete data;
    }
}

template <typename T>
T List<T>::Node::getData() const
{
    if (data != nullptr) {
        return *data;
    }
    return T{};
}

template <typename T>
List<T>::List()
{
    init();
}

template <typename T>
List<T>::List(const List<T>& other)
{
    init();
    copy(other);
}

template <typename T>
List<T>::List(List<T>&& other)
{
    init();
    move(std::move(other));
}

template <typename T>
List<T>& List<T>::operator=(const List<T>& other)
{
    copy(other);
    return *this;
}

template <typename T>
List<T>& List<T>::operator=(List<T>&& other)
{
    move(std::move(other));
    return *this;
}

template <typename T>
List<T>::~List()
{
    clear();
    delete mHead;
    delete mTail;
}

template <typename T>
void List<T>::copy(const List<T>& other)
{
    if (&other != this) {
        clear();
        for (Node* node = other.mHead->next; node != other.mTail; node = shift(node, 1)) {
            pushBack(node->getData());
        }
    }
}

template <typename T>
void List<T>::move(List<T>&& other)
{
    if (&other != this) {
        clear();
        connect(mHead, other.mHead->next);
        connect(other.mTail->prev, mTail);
        mSize = other.mSize;
        connect(other.mHead, other.mTail);
        other.mSize = 0;
    }
}

template <typename T>
void List<T>::insert(uint64 index, const T& value)
{
    add(find(index)->prev, value);
}

template <typename T>
void List<T>::erase(uint64 index)
{
    remove(find(index));
}

template <typename T>
void List<T>::swap(uint64 first, uint64 second)
{
    swap(find(first), find(second));
}

template <typename T>
template <typename Pred>
void List<T>::sort(uint64 from, uint64 to, Pred pred)
{
    if (to > mSize) {
        to = (mSize != 0) ? mSize - 1 : 0;
    }
    if (from < to) {
        Node* begin = find(from);
        Node* end = find(to);
        Node* beginPrev = begin->prev;
        Node* endNext = end->next;
        endNext->prev->next = nullptr;
        endNext->prev = nullptr;
        connect(beginPrev, mergeSort(begin, pred));
        Node* node = beginPrev;
        while (node != endNext) {
            if (node->next == nullptr) {
                connect(node, endNext);
            }
            node = node->next;
        }
    }
}

template <typename T>
void List<T>::sortLtoB(uint64 from, uint64 to)
{
    sort(from, to, [](const T& left, const T& right) {
        return left < right;
    });
}

template <typename T>
void List<T>::sortBtoL(uint64 from, uint64 to)
{
    sort(from, to, [](const T& left, const T& right) {
        return right < left;
    });
}

template <typename T>
uint64 List<T>::size() const
{
    return mSize;
}

template <typename T>
bool List<T>::empty() const
{
    return mSize == 0;
}

template <typename T>
T List<T>::front() const
{
    return (*this)[0];
}

template <typename T>
T List<T>::back() const
{
    return (*this)[mSize - 1];
}

template <typename T>
void List<T>::clear()
{
    uint64 size = mSize;
    for (uint64 i = 0; i < size; ++i) {
        popFront();
    }
}

template <typename T>
T List<T>::operator[](uint64 index) const
{
    Node* node = find(index);
    return (node != nullptr) ? node->getData() : T{};
}

template <typename T>
void List<T>::pushFront(const T& value)
{
    insert(0, value);
}

template <typename T>
void List<T>::pushBack(const T& value)
{
    insert(mSize, value);
}

template <typename T>
void List<T>::popFront()
{
    erase(0);
}

template <typename T>
void List<T>::popBack()
{
    erase(mSize - 1);
}

template <typename T>
void List<T>::print(uint64 from, uint64 to, std::ostream& out) const
{
    if (to > mSize) {
        to = (mSize != 0) ? mSize - 1 : 0;
    }
    uint64 size = 0;
    out << "[" << from << ":" << to << "] : "<<"{ ";
    if (to >= from && mSize != 0) {
        Node* begin = find(from);
        Node* end = find(to);
        for (Node* node = begin; node->prev != end; node = shift(node, 1)) {
            ++size;
            if (node != nullptr) {
                out << node->getData() << " ";
            }
        }
    }
    out << "} : " << size << "\n" << std::endl;
}

template <typename T>
void List<T>::init()
{
    mHead = new Node();
    mTail = new Node();
    connect(mHead, mTail);
    mSize = 0;
}

template <typename T>
void List<T>::add(Node* cur, const T& value)
{
    if (cur != nullptr) {
        Node* node = new Node(value);
        Node* next = cur->next;
        connect(cur, node);
        connect(node, next);
        ++mSize;
    }
}

template <typename T>
void List<T>::remove(Node* node)
{
    if (node != nullptr) {
        connect(node->prev, node->next);
        delete node;
        --mSize;
    }
}

template <typename T>
typename List<T>::Node* List<T>::shift(Node* node, int64 n) const
{
    int64 step = (n > 0) ? 1 : -1;
    while (node != nullptr && n != 0) {
        node = (step > 0) ? node->next : node->prev;
        n -= step;
    }
    return node;
}

template <typename T>
typename List<T>::Node* List<T>::find(uint64 index) const
{
    if (index > mSize) {
        return nullptr;
    }
    return (mSize > 2 * index)
        ? shift(mHead, index + 1)
        : shift(mTail, -(mSize - index));
}

template <typename T>
void List<T>::swap(Node* first, Node* second)
{
    if (first != nullptr && second != nullptr) {
        T* data = first->data;
        first->data = second->data;
        second->data = data;
    }
}

template <typename T>
void List<T>::connect(Node* left, Node* right)
{
    if (left != nullptr) {
        left->next = right;
    }
    if (right != nullptr) {
        right->prev = left;
    }
}

template <typename T>
template <typename Pred>
typename List<T>::Node* List<T>::mergeSort(Node* head, Pred pred)
{
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    Node* mid = split(head);
    head = mergeSort(head, pred);
    mid = mergeSort(mid, pred);
    return merge(head, mid, pred);
}

template <typename T>
typename List<T>::Node* List<T>::split(Node* head)
{
    Node* slow = head;
    Node* fast = head;
    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    Node* temp = slow->next;
    slow->next = nullptr;
    return temp;
}

template <typename T>
template <typename Pred>
typename List<T>::Node* List<T>::merge(Node* first, Node* second, Pred pred)
{
    if (first == nullptr) {
        return second;
    }
    if (second == nullptr) {
        return first;
    }
    if (pred(first->getData(), second->getData())) {
        first->next = merge(first->next, second, pred);
        first->next->prev = first;
        first->prev = nullptr;
        return first;
    } else {
        second->next = merge(first, second->next, pred);
        second->next->prev = second;
        second->prev = nullptr;
        return second;
    }
}
