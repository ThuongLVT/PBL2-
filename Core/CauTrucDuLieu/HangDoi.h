#ifndef HANGDOI_H
#define HANGDOI_H

// Template class HangDoi - Hang doi (FIFO)
template <typename T>
class HangDoi
{
private:
    struct Node
    {
        T data;
        Node *next;
        Node(const T &d) : data(d), next(nullptr) {}
    };

    Node *front_node; // Con tro dau
    Node *rear_node;  // Con tro cuoi
    int kichThuoc;    // So phan tu

public:
    // Constructor & Destructor
    HangDoi() : front_node(nullptr), rear_node(nullptr), kichThuoc(0) {}
    ~HangDoi() { clear(); }

    // Capacity
    int size() const { return kichThuoc; }
    bool isEmpty() const { return kichThuoc == 0; }

    // Modifiers
    void enqueue(const T &value); // Them vao cuoi
    void dequeue();               // Xoa dau
    void clear();                 // Xoa tat ca

    // Access
    T &front() { return front_node->data; }
    const T &front() const { return front_node->data; }
    T &back() { return rear_node->data; }
    const T &back() const { return rear_node->data; }
};

// ==================== IMPLEMENTATION ====================

template <typename T>
void HangDoi<T>::enqueue(const T &value)
{
    Node *newNode = new Node(value);
    if (!rear_node)
    {
        front_node = rear_node = newNode;
    }
    else
    {
        rear_node->next = newNode;
        rear_node = newNode;
    }
    kichThuoc++;
}

template <typename T>
void HangDoi<T>::dequeue()
{
    if (!front_node)
        return;

    Node *temp = front_node;
    front_node = front_node->next;
    delete temp;

    if (!front_node)
        rear_node = nullptr;

    kichThuoc--;
}

template <typename T>
void HangDoi<T>::clear()
{
    while (front_node)
    {
        Node *temp = front_node;
        front_node = front_node->next;
        delete temp;
    }
    rear_node = nullptr;
    kichThuoc = 0;
}

#endif // HANGDOI_H
