#ifndef HANGDOI_H
#define HANGDOI_H

#include "DanhSachLienKet.h"

/**
 * @brief Template class HangDoi - Queue (FIFO)
 * Sử dụng DanhSachLienKet làm cấu trúc nền
 */
template <typename T>
class HangDoi
{
private:
    DanhSachLienKet<T> data;

public:
    // Constructors
    HangDoi() {}

    // Capacity
    int size() const { return data.size(); }
    bool isEmpty() const { return data.isEmpty(); }

    // Modifiers
    void enqueue(const T &value)
    {
        data.push_back(value); // Thêm vào cuối (FIFO)
    }

    void dequeue()
    {
        if (!isEmpty())
        {
            data.pop_front(); // Lấy từ đầu
        }
    }

    // Access
    T &front()
    {
        return data.front();
    }

    const T &front() const
    {
        return data.front();
    }

    T &back()
    {
        return data.back();
    }

    const T &back() const
    {
        return data.back();
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

#endif // HANGDOI_H
