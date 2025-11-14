#ifndef THOIGIAN_H
#define THOIGIAN_H

#include <iostream>

/**
 * @brief Class ThoiGian - Quản lý giờ:phút:giây
 */
class ThoiGian
{
private:
    int gio;  // 0-23
    int phut; // 0-59
    int giay; // 0-59

public:
    // Constructors
    ThoiGian();
    ThoiGian(int g, int p, int gi = 0);

    // Getters
    int getGio() const { return gio; }
    int getPhut() const { return phut; }
    int getGiay() const { return giay; }

    // Setters
    void setGio(int g);
    void setPhut(int p);
    void setGiay(int gi);
    void setThoiGian(int g, int p, int gi = 0);

    // Operators
    bool operator==(const ThoiGian &other) const;
    bool operator!=(const ThoiGian &other) const;
    bool operator<(const ThoiGian &other) const;
    bool operator>(const ThoiGian &other) const;
    bool operator<=(const ThoiGian &other) const;
    bool operator>=(const ThoiGian &other) const;

    // Methods
    int toSeconds() const;                          // Chuyển sang giây
    void fromSeconds(int totalSeconds);             // Từ giây sang h:m:s
    ThoiGian add(int hours, int minutes = 0) const; // Cộng thêm giờ/phút
    int diffMinutes(const ThoiGian &other) const;   // Khoảng cách phút

    // Validation
    bool isValid() const;

    // Display
    void display() const;
    std::string toString() const; // Format: "HH:MM:SS"

    // File I/O
    void ghiFile(std::ostream &out) const;
    void docFile(std::istream &in);

    // Friend
    friend std::ostream &operator<<(std::ostream &out, const ThoiGian &t);
    friend std::istream &operator>>(std::istream &in, ThoiGian &t);
};

#endif // THOIGIAN_H
