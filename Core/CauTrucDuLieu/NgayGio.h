#ifndef NGAYGIO_H
#define NGAYGIO_H

#include <iostream>
#include <string>

// Class NgayThang - Quan ly ngay/thang/nam
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
    int toDays() const;                               // Chuyen sang so ngay tu 1/1/1900
    void fromDays(int totalDays);                     // Tu so ngay sang d/m/y
    NgayThang addDays(int days) const;                // Cong them ngay
    int diffDays(const NgayThang &other) const;       // Khoang cach ngay
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

// Class ThoiGian - Quan ly gio:phut:giay
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
    int toSeconds() const;                          // Chuyen sang giay
    void fromSeconds(int totalSeconds);             // Tu giay sang h:m:s
    ThoiGian add(int hours, int minutes = 0) const; // Cong them gio/phut
    int diffMinutes(const ThoiGian &other) const;   // Khoang cach phut

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

// Class NgayGio - Ket hop NgayThang va ThoiGian
// Dai dien cho mot thoi diem cu the
class NgayGio
{
private:
    NgayThang ngayThang;
    ThoiGian thoiGian;

public:
    // Constructors
    NgayGio();
    NgayGio(const NgayThang &nt, const ThoiGian &tg);
    NgayGio(int ngay, int thang, int nam, int gio, int phut, int giay = 0);

    // Getters
    NgayThang getNgayThang() const { return ngayThang; }
    ThoiGian getThoiGian() const { return thoiGian; }

    int getNgay() const { return ngayThang.getNgay(); }
    int getThang() const { return ngayThang.getThang(); }
    int getNam() const { return ngayThang.getNam(); }
    int getGio() const { return thoiGian.getGio(); }
    int getPhut() const { return thoiGian.getPhut(); }
    int getGiay() const { return thoiGian.getGiay(); }

    // Setters
    void setNgayThang(const NgayThang &nt) { ngayThang = nt; }
    void setThoiGian(const ThoiGian &tg) { thoiGian = tg; }
    void setNgayGio(int ngay, int thang, int nam, int gio, int phut, int giay = 0);

    // Operators
    bool operator==(const NgayGio &other) const;
    bool operator!=(const NgayGio &other) const;
    bool operator<(const NgayGio &other) const;
    bool operator>(const NgayGio &other) const;
    bool operator<=(const NgayGio &other) const;
    bool operator>=(const NgayGio &other) const;

    // Methods
    NgayGio addHours(int hours) const;           // Cong them gio
    NgayGio addMinutes(int minutes) const;       // Cong them phut
    NgayGio addDays(int days) const;             // Cong them ngay
    int diffMinutes(const NgayGio &other) const; // Khoang cach phut
    int diffHours(const NgayGio &other) const;   // Khoang cach gio

    // Check same day
    bool isSameDay(const NgayGio &other) const;

    // Validation
    bool isValid() const;

    // Display
    void display() const;
    std::string toString() const; // Format: "DD/MM/YYYY HH:MM:SS"

    // Static method
    static NgayGio layThoiGianHienTai(); // Lay thoi gian hien tai

    // File I/O
    void ghiFile(std::ostream &out) const;
    void docFile(std::istream &in);
    void ghiFile(FILE *f) const; // Overload for FILE*
    void docFile(FILE *f);       // Overload for FILE*

    // Friend
    friend std::ostream &operator<<(std::ostream &out, const NgayGio &ng);
    friend std::istream &operator>>(std::istream &in, NgayGio &ng);
};

#endif // NGAYGIO_H
