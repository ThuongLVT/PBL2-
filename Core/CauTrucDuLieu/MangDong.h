#ifndef MANGDONG_H
#define MANGDONG_H

#include <iostream>

// Dynamic array voi auto-resize
template <typename T>
class MangDong
{
private:
    T *data;       // Con tro den mang
    int kichThuoc; // So phan tu hien tai
    int dungLuong; // Dung luong da cap phat

    void capPhat(int dungLuongMoi);
    void giaiPhong();

public:
    // Constructors & Destructor
    MangDong();
    MangDong(int dungLuongBanDau);
    MangDong(const MangDong<T> &other);
    ~MangDong();

    // Operators
    MangDong<T> &operator=(const MangDong<T> &other);
    T &operator[](int index);
    const T &operator[](int index) const;

    // Capacity
    int size() const { return kichThuoc; }
    int capacity() const { return dungLuong; }
    bool isEmpty() const { return kichThuoc == 0; }
    void resize(int kichThuocMoi);
    void reserve(int dungLuongMoi);

    // Modifiers
    void push_back(const T &value);         // Them vao cuoi
    void pop_back();                        // Xoa cuoi
    void insert(int index, const T &value); // Chen tai vi tri
    void erase(int index);                  // Xoa tai vi tri
    void clear();                           // Xoa tat ca

    // Access
    T &at(int index);
    const T &at(int index) const;
    T &front() { return data[0]; }
    const T &front() const { return data[0]; }
    T &back() { return data[kichThuoc - 1]; }
    const T &back() const { return data[kichThuoc - 1]; }
    T *getData() { return data; }
    const T *getData() const { return data; }

    // Search
    int indexOf(const T &value) const;   // Tim vi tri phan tu
    bool contains(const T &value) const; // Kiem tra co chua

    // File I/O
    void ghiFile(std::ostream &out) const;
    void docFile(std::istream &in);
};

// ==================== IMPLEMENTATION ====================
// (Template phai implement trong header file)

template <typename T>
MangDong<T>::MangDong() : data(nullptr), kichThuoc(0), dungLuong(0)
{
    capPhat(16); // Dung luong mac dinh
}

template <typename T>
MangDong<T>::MangDong(int dungLuongBanDau) : data(nullptr), kichThuoc(0), dungLuong(0)
{
    capPhat(dungLuongBanDau > 0 ? dungLuongBanDau : 16);
}

template <typename T>
MangDong<T>::MangDong(const MangDong<T> &other) : data(nullptr), kichThuoc(0), dungLuong(0)
{
    capPhat(other.dungLuong);
    kichThuoc = other.kichThuoc;
    for (int i = 0; i < kichThuoc; i++)
    {
        data[i] = other.data[i];
    }
}

template <typename T>
MangDong<T>::~MangDong()
{
    giaiPhong();
}

template <typename T>
void MangDong<T>::capPhat(int dungLuongMoi)
{
    if (dungLuongMoi <= 0)
        dungLuongMoi = 16;

    T *dataNew = new T[dungLuongMoi];

    if (data)
    {
        int saoChep = (kichThuoc < dungLuongMoi) ? kichThuoc : dungLuongMoi;
        for (int i = 0; i < saoChep; i++)
        {
            dataNew[i] = data[i];
        }
        delete[] data;
        kichThuoc = saoChep;
    }

    data = dataNew;
    dungLuong = dungLuongMoi;
}

template <typename T>
void MangDong<T>::giaiPhong()
{
    if (data)
    {
        delete[] data;
        data = nullptr;
    }
    kichThuoc = 0;
    dungLuong = 0;
}

template <typename T>
MangDong<T> &MangDong<T>::operator=(const MangDong<T> &other)
{
    if (this != &other)
    {
        giaiPhong();
        capPhat(other.dungLuong);
        kichThuoc = other.kichThuoc;
        for (int i = 0; i < kichThuoc; i++)
        {
            data[i] = other.data[i];
        }
    }
    return *this;
}

template <typename T>
T &MangDong<T>::operator[](int index)
{
    return data[index];
}

template <typename T>
const T &MangDong<T>::operator[](int index) const
{
    return data[index];
}

template <typename T>
void MangDong<T>::resize(int kichThuocMoi)
{
    if (kichThuocMoi > dungLuong)
    {
        capPhat(kichThuocMoi);
    }
    kichThuoc = kichThuocMoi;
}

template <typename T>
void MangDong<T>::reserve(int dungLuongMoi)
{
    if (dungLuongMoi > dungLuong)
    {
        capPhat(dungLuongMoi);
    }
}

template <typename T>
void MangDong<T>::push_back(const T &value)
{
    if (kichThuoc >= dungLuong)
    {
        capPhat(dungLuong * 2);
    }
    data[kichThuoc++] = value;
}

template <typename T>
void MangDong<T>::pop_back()
{
    if (kichThuoc > 0)
    {
        kichThuoc--;
    }
}

template <typename T>
void MangDong<T>::insert(int index, const T &value)
{
    if (index < 0 || index > kichThuoc)
        return;

    if (kichThuoc >= dungLuong)
    {
        capPhat(dungLuong * 2);
    }

    // Dich cac phan tu sang phai
    for (int i = kichThuoc; i > index; i--)
    {
        data[i] = data[i - 1];
    }

    data[index] = value;
    kichThuoc++;
}

template <typename T>
void MangDong<T>::erase(int index)
{
    if (index < 0 || index >= kichThuoc)
        return;

    // Dich cac phan tu sang trai
    for (int i = index; i < kichThuoc - 1; i++)
    {
        data[i] = data[i + 1];
    }

    kichThuoc--;
}

template <typename T>
void MangDong<T>::clear()
{
    kichThuoc = 0;
}

template <typename T>
T &MangDong<T>::at(int index)
{
    if (index < 0 || index >= kichThuoc)
    {
        throw "Index out of range";
    }
    return data[index];
}

template <typename T>
const T &MangDong<T>::at(int index) const
{
    if (index < 0 || index >= kichThuoc)
    {
        throw "Index out of range";
    }
    return data[index];
}

template <typename T>
int MangDong<T>::indexOf(const T &value) const
{
    for (int i = 0; i < kichThuoc; i++)
    {
        if (data[i] == value)
            return i;
    }
    return -1;
}

template <typename T>
bool MangDong<T>::contains(const T &value) const
{
    return indexOf(value) != -1;
}

template <typename T>
void MangDong<T>::ghiFile(std::ostream &out) const
{
    out.write(reinterpret_cast<const char *>(&kichThuoc), sizeof(kichThuoc));
    for (int i = 0; i < kichThuoc; i++)
    {
        out.write(reinterpret_cast<const char *>(&data[i]), sizeof(T));
    }
}

template <typename T>
void MangDong<T>::docFile(std::istream &in)
{
    clear();
    int size;
    in.read(reinterpret_cast<char *>(&size), sizeof(size));
    reserve(size);
    for (int i = 0; i < size; i++)
    {
        T value;
        in.read(reinterpret_cast<char *>(&value), sizeof(T));
        push_back(value);
    }
}

#endif // MANGDONG_H
