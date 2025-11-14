#ifndef NGAYGIO_H
#define NGAYGIO_H

#include "NgayThang.h"
#include "ThoiGian.h"
#include <iostream>

/**
 * @brief Class NgayGio - Kết hợp NgayThang và ThoiGian
 * Đại diện cho một thời điểm cụ thể
 */
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
    NgayGio addHours(int hours) const;           // Cộng thêm giờ
    NgayGio addMinutes(int minutes) const;       // Cộng thêm phút
    NgayGio addDays(int days) const;             // Cộng thêm ngày
    int diffMinutes(const NgayGio &other) const; // Khoảng cách phút
    int diffHours(const NgayGio &other) const;   // Khoảng cách giờ

    // Check same day
    bool isSameDay(const NgayGio &other) const;

    // Validation
    bool isValid() const;

    // Display
    void display() const;
    std::string toString() const; // Format: "DD/MM/YYYY HH:MM:SS"

    // Static method
    static NgayGio layThoiGianHienTai(); // Lấy thời gian hiện tại

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
