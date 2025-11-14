#ifndef DANHSACHLIENKET_H
#define DANHSACHLIENKET_H

#include <iostream>

/**
 * @brief Template class DanhSachLienKet - Linked List
 * Danh sách liên kết đơn
 */
template <typename T>
class DanhSachLienKet
{
private:
    struct Node
    {
        T data;
        Node *next;

        Node(const T &value) : data(value), next(nullptr) {}
    };

    Node *head;
    Node *tail;
    int kichThuoc;

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
    void push_front(const T &value);        // Thêm vào đầu
    void push_back(const T &value);         // Thêm vào cuối
    void pop_front();                       // Xóa đầu
    void pop_back();                        // Xóa cuối
    void insert(int index, const T &value); // Chèn tại vị trí
    void erase(int index);                  // Xóa tại vị trí
    void clear();                           // Xóa tất cả

    // Access
    T &front();
    const T &front() const;
    T &back();
    const T &back() const;
    T &at(int index);
    const T &at(int index) const;

    // Search
    int indexOf(const T &value) const;
    bool contains(const T &value) const;

    // Iterator (simple)
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
        bool operator!=(const Iterator &other) const { return current != other.current; }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

    // File I/O
    void ghiFile(std::ostream &out) const;
    void docFile(std::istream &in);
};

// ==================== IMPLEMENTATION ====================

template <typename T>
DanhSachLienKet<T>::DanhSachLienKet() : head(nullptr), tail(nullptr), kichThuoc(0) {}

template <typename T>
DanhSachLienKet<T>::DanhSachLienKet(const DanhSachLienKet<T> &other)
    : head(nullptr), tail(nullptr), kichThuoc(0)
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
    newNode->next = head;
    head = newNode;

    if (tail == nullptr)
    {
        tail = head;
    }

    kichThuoc++;
}

template <typename T>
void DanhSachLienKet<T>::push_back(const T &value)
{
    Node *newNode = new Node(value);

    if (tail)
    {
        tail->next = newNode;
        tail = newNode;
    }
    else
    {
        head = tail = newNode;
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
    {
        tail = nullptr;
    }

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
    if (index < 0 || index > kichThuoc)
        return;

    if (index == 0)
    {
        push_front(value);
        return;
    }

    if (index == kichThuoc)
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
    {
        tail = current;
    }

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
T &DanhSachLienKet<T>::front()
{
    return head->data;
}

template <typename T>
const T &DanhSachLienKet<T>::front() const
{
    return head->data;
}

template <typename T>
T &DanhSachLienKet<T>::back()
{
    return tail->data;
}

template <typename T>
const T &DanhSachLienKet<T>::back() const
{
    return tail->data;
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
        {
            return index;
        }
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

template <typename T>
void DanhSachLienKet<T>::ghiFile(std::ostream &out) const
{
    out.write(reinterpret_cast<const char *>(&kichThuoc), sizeof(kichThuoc));

    Node *current = head;
    while (current)
    {
        out.write(reinterpret_cast<const char *>(&current->data), sizeof(T));
        current = current->next;
    }
}

template <typename T>
void DanhSachLienKet<T>::docFile(std::istream &in)
{
    clear();

    int size;
    in.read(reinterpret_cast<char *>(&size), sizeof(size));

    for (int i = 0; i < size; i++)
    {
        T value;
        in.read(reinterpret_cast<char *>(&value), sizeof(T));
        push_back(value);
    }
}

#endif // DANHSACHLIENKET_H
