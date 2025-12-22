#ifndef DANHSACHLIKET_H
#define DANHSACHLIKET_H

#include <iostream>

// Template class DanhSachLienKet - Danh sach lien ket don
template <typename T>
class DanhSachLienKet
{
private:
    // Node trong danh sach
    struct Node
    {
        T data;
        Node *next;
        Node(const T &d) : data(d), next(nullptr) {}
    };

    Node *head;      // Con tro dau
    Node *tail;      // Con tro cuoi
    int kichThuoc;   // So phan tu

public:
    // Constructors & Destructor
    DanhSachLienKet();
    DanhSachLienKet(const DanhSachLienKet<T> &other);
    ~DanhSachLienKet();

    // Operators
    DanhSachLienKet<T> &operator=(const DanhSachLienKet<T> &other);

    // Capacity
    int size() const { return kichThuoc; }
    bool isEmpty() const { return kichThuoc == 0; }

    // Modifiers
    void push_front(const T &value);        // Them vao dau
    void push_back(const T &value);         // Them vao cuoi
    void pop_front();                       // Xoa dau
    void pop_back();                        // Xoa cuoi
    void insert(int index, const T &value); // Chen tai vi tri
    void erase(int index);                  // Xoa tai vi tri
    void clear();                           // Xoa tat ca

    // Access
    T &at(int index);
    const T &at(int index) const;
    T &front() { return head->data; }
    const T &front() const { return head->data; }
    T &back() { return tail->data; }
    const T &back() const { return tail->data; }

    // Search
    int indexOf(const T &value) const;   // Tim vi tri phan tu
    bool contains(const T &value) const; // Kiem tra co chua

    // Iterator support (simple)
    class Iterator
    {
    private:
        Node *current;

    public:
        Iterator(Node *node) : current(node) {}
        T &operator*() { return current->data; }
        Iterator &operator++()
        {
            current = current->next;
            return *this;
        }
        bool operator!=(const Iterator &other) { return current != other.current; }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
};

// ==================== IMPLEMENTATION ====================

template <typename T>
DanhSachLienKet<T>::DanhSachLienKet() : head(nullptr), tail(nullptr), kichThuoc(0) {}

template <typename T>
DanhSachLienKet<T>::DanhSachLienKet(const DanhSachLienKet<T> &other) : head(nullptr), tail(nullptr), kichThuoc(0)
{
    Node *current = other.head;
    while (current)
    {
        push_back(current->data);
        current = current->next;
    }
}

template <typename T>
DanhSachLienKet<T>::~DanhSachLienKet()
{
    clear();
}

template <typename T>
DanhSachLienKet<T> &DanhSachLienKet<T>::operator=(const DanhSachLienKet<T> &other)
{
    if (this != &other)
    {
        clear();
        Node *current = other.head;
        while (current)
        {
            push_back(current->data);
            current = current->next;
        }
    }
    return *this;
}

template <typename T>
void DanhSachLienKet<T>::push_front(const T &value)
{
    Node *newNode = new Node(value);
    if (!head)
    {
        head = tail = newNode;
    }
    else
    {
        newNode->next = head;
        head = newNode;
    }
    kichThuoc++;
}

template <typename T>
void DanhSachLienKet<T>::push_back(const T &value)
{
    Node *newNode = new Node(value);
    if (!tail)
    {
        head = tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }
    kichThuoc++;
}

template <typename T>
void DanhSachLienKet<T>::pop_front()
{
    if (!head)
        return;

    Node *temp = head;
    head = head->next;
    delete temp;

    if (!head)
        tail = nullptr;

    kichThuoc--;
}

template <typename T>
void DanhSachLienKet<T>::pop_back()
{
    if (!head)
        return;

    if (head == tail)
    {
        delete head;
        head = tail = nullptr;
    }
    else
    {
        Node *current = head;
        while (current->next != tail)
        {
            current = current->next;
        }
        delete tail;
        tail = current;
        tail->next = nullptr;
    }
    kichThuoc--;
}

template <typename T>
void DanhSachLienKet<T>::insert(int index, const T &value)
{
    if (index <= 0)
    {
        push_front(value);
        return;
    }
    if (index >= kichThuoc)
    {
        push_back(value);
        return;
    }

    Node *current = head;
    for (int i = 0; i < index - 1; i++)
    {
        current = current->next;
    }

    Node *newNode = new Node(value);
    newNode->next = current->next;
    current->next = newNode;
    kichThuoc++;
}

template <typename T>
void DanhSachLienKet<T>::erase(int index)
{
    if (index < 0 || index >= kichThuoc)
        return;

    if (index == 0)
    {
        pop_front();
        return;
    }

    Node *current = head;
    for (int i = 0; i < index - 1; i++)
    {
        current = current->next;
    }

    Node *temp = current->next;
    current->next = temp->next;

    if (temp == tail)
        tail = current;

    delete temp;
    kichThuoc--;
}

template <typename T>
void DanhSachLienKet<T>::clear()
{
    while (head)
    {
        Node *temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
    kichThuoc = 0;
}

template <typename T>
T &DanhSachLienKet<T>::at(int index)
{
    if (index < 0 || index >= kichThuoc)
    {
        throw "Index out of range";
    }

    Node *current = head;
    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }
    return current->data;
}

template <typename T>
const T &DanhSachLienKet<T>::at(int index) const
{
    if (index < 0 || index >= kichThuoc)
    {
        throw "Index out of range";
    }

    Node *current = head;
    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }
    return current->data;
}

template <typename T>
int DanhSachLienKet<T>::indexOf(const T &value) const
{
    Node *current = head;
    int index = 0;
    while (current)
    {
        if (current->data == value)
            return index;
        current = current->next;
        index++;
    }
    return -1;
}

template <typename T>
bool DanhSachLienKet<T>::contains(const T &value) const
{
    return indexOf(value) != -1;
}

#endif // DANHSACHLIKET_H
