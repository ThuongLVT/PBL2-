#ifndef NGAN_H
#define NGAN_H

#include "DanhSachLienKet.h"

/**
 * @brief Template class Ngan - Stack (LIFO)
 * Sử dụng DanhSachLienKet làm cấu trúc nền
 */
template <typename T>
class Ngan
{
private:
    DanhSachLienKet<T> data;

public:
    // Constructors
    Ngan() {}

    // Capacity
    int size() const { return data.size(); }
    bool isEmpty() const { return data.isEmpty(); }

    // Modifiers
    void push(const T &value)
    {
        data.push_front(value); // Push vào đầu (LIFO)
    }

    void pop()
    {
        if (!isEmpty())
        {
            data.pop_front();
        }
    }

    // Access
    T &top()
    {
        return data.front();
    }

    const T &top() const
    {
        return data.front();
    }

    // Clear
    void clear()
    {
        data.clear();
    }

    // File I/O
    void ghiFile(std::ostream &out) const
    {
        data.ghiFile(out);
    }

    void docFile(std::istream &in)
    {
        data.docFile(in);
    }
};

#endif // NGAN_H
