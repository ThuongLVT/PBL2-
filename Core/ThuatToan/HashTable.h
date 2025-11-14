#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <string>
#include <type_traits>

#include "../CauTrucDuLieu/MangDong.h"
#include "../CauTrucDuLieu/DanhSachLienKet.h"

// Cặp Key-Value
template <typename K, typename V>
struct KeyValuePair
{
    K key;
    V value;

    KeyValuePair() : key(), value() {}
    KeyValuePair(const K &k, const V &v) : key(k), value(v) {}
};

// Hash Table với chaining
template <typename K, typename V>
class HashTable
{
private:
    MangDong<DanhSachLienKet<KeyValuePair<K, V>> *> table;
    int capacity;      // Kích thước bảng
    int count;         // Số phần tử hiện tại
    double loadFactor; // Load factor tối đa

    // Hàm băm cho int
    int hashInt(int key) const
    {
        return key % capacity;
    }

    // Hàm băm cho std::string
    int hashString(const std::string &key) const
    {
        unsigned long hash = 5381;
        for (int i = 0; i < key.length(); i++)
        {
            hash = ((hash << 5) + hash) + key[i]; // hash * 33 + c
        }
        return hash % capacity;
    }

    // Hàm băm chung - sử dụng template specialization trong runtime
    int hash(const K &key) const
    {
        // Sử dụng type traits để chọn hàm hash phù hợp
        if constexpr (std::is_same_v<K, int>)
        {
            return hashInt(key);
        }
        else if constexpr (std::is_same_v<K, std::string>)
        {
            return hashString(key);
        }
        else
        {
            // Default: convert to int
            return static_cast<int>(reinterpret_cast<uintptr_t>(&key)) % capacity;
        }
    }

    // Rehash khi load factor cao
    void rehash()
    {
        int oldCapacity = capacity;
        capacity *= 2;

        MangDong<DanhSachLienKet<KeyValuePair<K, V>> *> oldTable = table;
        table = MangDong<DanhSachLienKet<KeyValuePair<K, V>> *>();

        // Khởi tạo bảng mới
        for (int i = 0; i < capacity; i++)
        {
            table.push_back(new DanhSachLienKet<KeyValuePair<K, V>>());
        }

        // Chuyển dữ liệu từ bảng cũ sang bảng mới
        count = 0;
        for (int i = 0; i < oldCapacity; i++)
        {
            if (oldTable[i] != nullptr)
            {
                for (int j = 0; j < oldTable[i]->size(); j++)
                {
                    KeyValuePair<K, V> pair = oldTable[i]->at(j);
                    insert(pair.key, pair.value);
                }
                delete oldTable[i];
            }
        }
    }

public:
    // Constructor
    HashTable(int cap = 101) : capacity(cap), count(0), loadFactor(0.75)
    {
        for (int i = 0; i < capacity; i++)
        {
            table.push_back(new DanhSachLienKet<KeyValuePair<K, V>>());
        }
    }

    // Destructor
    ~HashTable()
    {
        for (int i = 0; i < table.size(); i++)
        {
            delete table[i];
        }
    }

    // Thêm hoặc cập nhật
    void insert(const K &key, const V &value)
    {
        // Kiểm tra load factor
        if ((double)count / capacity > loadFactor)
        {
            rehash();
        }

        int index = hash(key);
        DanhSachLienKet<KeyValuePair<K, V>> *chain = table[index];

        // Kiểm tra key đã tồn tại chưa
        for (int i = 0; i < chain->size(); i++)
        {
            if (chain->at(i).key == key)
            {
                // Cập nhật value - need to update in place
                chain->at(i).value = value;
                return;
            }
        }

        // Thêm mới
        chain->push_back(KeyValuePair<K, V>(key, value));
        count++;
    }

    // Tìm kiếm
    bool search(const K &key, V &value) const
    {
        int index = hash(key);
        DanhSachLienKet<KeyValuePair<K, V>> *chain = table[index];

        for (int i = 0; i < chain->size(); i++)
        {
            if (chain->at(i).key == key)
            {
                value = chain->at(i).value;
                return true;
            }
        }

        return false;
    }

    // Lấy giá trị theo key
    V get(const K &key) const
    {
        V value;
        if (search(key, value))
        {
            return value;
        }
        return V(); // Trả về giá trị mặc định
    }

    // Xóa
    bool remove(const K &key)
    {
        int index = hash(key);
        DanhSachLienKet<KeyValuePair<K, V>> *chain = table[index];

        for (int i = 0; i < chain->kichThuoc(); i++)
        {
            if (chain->lay(i).key == key)
            {
                chain->xoa(i);
                count--;
                return true;
            }
        }

        return false;
    }

    // Kiểm tra key có tồn tại không
    bool containsKey(const K &key) const
    {
        V value;
        return search(key, value);
    }

    // Lấy tất cả keys
    MangDong<K> keys() const
    {
        MangDong<K> result;
        for (int i = 0; i < table.size(); i++)
        {
            DanhSachLienKet<KeyValuePair<K, V>> *chain = table[i];
            for (int j = 0; j < chain->kichThuoc(); j++)
            {
                result.push_back(chain->lay(j).key);
            }
        }
        return result;
    }

    // Lấy tất cả values
    MangDong<V> values() const
    {
        MangDong<V> result;
        for (int i = 0; i < table.size(); i++)
        {
            DanhSachLienKet<KeyValuePair<K, V>> *chain = table[i];
            for (int j = 0; j < chain->kichThuoc(); j++)
            {
                result.push_back(chain->lay(j).value);
            }
        }
        return result;
    }

    // Số phần tử
    int size() const
    {
        return count;
    }

    // Kiểm tra rỗng
    bool isEmpty() const
    {
        return count == 0;
    }

    // Xóa tất cả
    void clear()
    {
        for (int i = 0; i < table.size(); i++)
        {
            table[i]->xoaTatCa();
        }
        count = 0;
    }
};

// Specialization cho int
template <>
inline int HashTable<int, int>::hash(const int &key) const
{
    return hashInt(key);
}

// Specialization có vấn đề với syntax, hash() sẽ tự detect type
/*
template <typename V>
inline int HashTable<int, V>::hash(const int &key) const
{
    return hashInt(key);
}

// Specialization cho std::string
template <typename V>
inline int HashTable<std::string, V>::hash(const std::string &key) const
{
    return hashString(key);
}
*/

#endif // HASHTABLE_H
