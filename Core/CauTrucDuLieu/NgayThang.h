#ifndef NGAYTHANG_H
#define NGAYTHANG_H

#include <iostream>

/**
 * @brief Class NgayThang - Quản lý ngày/tháng/năm
 */
class NgayThang
{
private:
    int ngay;  // 1-31
    int thang; // 1-12
    int nam;   // 1900-2100

    bool isLeapYear(int y) const;
    int daysInMonth(int m, int y) const;

public:
    // Constructors
    NgayThang();
    NgayThang(int d, int m, int y);

    // Getters
    int getNgay() const { return ngay; }
    int getThang() const { return thang; }
    int getNam() const { return nam; }

    // Setters
    void setNgay(int d);
    void setThang(int m);
    void setNam(int y);
    void setNgayThang(int d, int m, int y);

    // Operators
    bool operator==(const NgayThang &other) const;
    bool operator!=(const NgayThang &other) const;
    bool operator<(const NgayThang &other) const;
    bool operator>(const NgayThang &other) const;
    bool operator<=(const NgayThang &other) const;
    bool operator>=(const NgayThang &other) const;

    // Methods
    int toDays() const;                               // Chuyển sang số ngày từ 1/1/1900
    void fromDays(int totalDays);                     // Từ số ngày sang d/m/y
    NgayThang addDays(int days) const;                // Cộng thêm ngày
    int diffDays(const NgayThang &other) const;       // Khoảng cách ngày
    int tinhKhoangCach(const NgayThang &other) const; // Alias cho diffDays
    std::string toString() const;                     // Convert to string (dd/mm/yyyy)

    // Static methods
    static NgayThang layNgayHienTai(); // Get current date

    // Validation
    bool isValid() const;

    // Display
    void display() const;

    // File I/O
    void ghiFile(std::ostream &out) const;
    void docFile(std::istream &in);

    // Friend
    friend std::ostream &operator<<(std::ostream &out, const NgayThang &nt);
    friend std::istream &operator>>(std::istream &in, NgayThang &nt);
};

#endif // NGAYTHANG_H
