#ifndef NGAN_H
#define NGAN_H

// Template class Ngan - Ngan xep (LIFO)
template <typename T>
class Ngan
{
private:
    struct Node
    {
        T data;
        Node *next;
        Node(const T &d) : data(d), next(nullptr) {}
    };

    Node *top_node;  // Con tro dinh
    int kichThuoc;   // So phan tu

public:
    // Constructor & Destructor
    Ngan() : top_node(nullptr), kichThuoc(0) {}
    ~Ngan() { clear(); }

    // Capacity
    int size() const { return kichThuoc; }
    bool isEmpty() const { return kichThuoc == 0; }

    // Modifiers
    void push(const T &value); // Them vao dinh
    void pop();                // Xoa dinh
    void clear();              // Xoa tat ca

    // Access
    T &top() { return top_node->data; }
    const T &top() const { return top_node->data; }
};

// ==================== IMPLEMENTATION ====================

template <typename T>
void Ngan<T>::push(const T &value)
{
    Node *newNode = new Node(value);
    newNode->next = top_node;
    top_node = newNode;
    kichThuoc++;
}

template <typename T>
void Ngan<T>::pop()
{
    if (!top_node)
        return;

    Node *temp = top_node;
    top_node = top_node->next;
    delete temp;
    kichThuoc--;
}

template <typename T>
void Ngan<T>::clear()
{
    while (top_node)
    {
        Node *temp = top_node;
        top_node = top_node->next;
        delete temp;
    }
    kichThuoc = 0;
}

#endif // NGAN_H
