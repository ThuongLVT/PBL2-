#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <string>
#include <fstream>

namespace FileHelper
{

    // Ghi string vào file binary (length + data)
    inline bool ghiString(std::ofstream &file, const std::string &str)
    {
        if (!file.is_open())
            return false;
        size_t len = str.length();
        file.write(reinterpret_cast<const char *>(&len), sizeof(len));
        if (len > 0)
        {
            file.write(str.c_str(), len);
        }
        return file.good();
    }

    // Đọc string từ file binary (length + data)
    inline bool docString(std::ifstream &file, std::string &str)
    {
        if (!file.is_open())
            return false;
        size_t len;
        file.read(reinterpret_cast<char *>(&len), sizeof(len));
        if (len > 0)
        {
            str.resize(len);
            file.read(&str[0], len);
        }
        else
        {
            str.clear();
        }
        return file.good();
    }
}

#endif // FILEHELPER_H
