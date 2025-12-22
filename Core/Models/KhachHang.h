// KhachHang.h - Lop dai dien cho Khach hang
// Ke thua tu ConNguoi, khong can tai khoan dang nhap

#ifndef KHACHHANG_H
#define KHACHHANG_H
#include <string>

#include "ConNguoi.h"
#include "../CauTrucDuLieu/NgayGio.h"

// Hang khach hang dua tren tong chi tieu
enum class HangKhachHang
{
    THUONG,   // Thuong (< 1.000.000 VND) - Giam 0%
    DONG,     // Dong (1.000.000 - 4.999.999 VND) - Giam 2%
    BAC,      // Bac (5.000.000 - 9.999.999 VND) - Giam 4%
    VANG,     // Vang (10.000.000 - 19.999.999 VND) - Giam 6%
    KIM_CUONG // Kim cuong (>= 20.000.000 VND) - Giam 10%
};

// Lop dai dien cho Khach hang
// Khach hang: khong can tai khoan dang nhap, co ma khach hang rieng,
// tich luy diem qua moi lan dat san, duoc huong uu dai theo hang
class KhachHang : public ConNguoi
{
private:
    std::string maKhachHang; // Ma khach hang (VD: KH001)
    double tongChiTieu;      // Tong chi tieu (VND)
    HangKhachHang hang;      // Hang khach hang
    NgayThang ngayDangKy;    // Ngay dang ky thanh vien
    int soLanDatSan;         // Tong so lan dat san

public:
    // ========== CONSTRUCTORS ==========

    // Constructor mac dinh
    KhachHang();

    // Constructor co tham so (ht: Ho ten, sdt: So dien thoai, dc: Dia chi, maKH: Ma khach hang)
    KhachHang(const std::string &ht, const std::string &sdt, const std::string &dc,
              const std::string &maKH);

    // Copy constructor (other: Doi tuong KhachHang khac)
    KhachHang(const KhachHang &other);

    // Destructor
    virtual ~KhachHang();

    // ========== GETTERS ==========

    // Inherited from base classes - forward declaration
    std::string getMaNguoiDung() const; // Alias for compatibility

    // Lay ma khach hang
    std::string layMaKhachHang() const;

    // Lay tong chi tieu (VND)
    double layTongChiTieu() const;

    // Lay hang khach hang
    HangKhachHang layHang() const;

    // Lay ngay dang ky
    NgayThang layNgayDangKy() const;

    // Lay so lan dat san
    int laySoLanDatSan() const;

    // Lay ten hang khach hang (dang chuoi)
    std::string layTenHang() const;

    // Lay phan tram giam gia theo hang (0-15)
    int layPhanTramGiamGia() const;

    // ========== SETTERS ==========

    // Dat ma khach hang (maKH: Ma khach hang moi)
    void datMaKhachHang(const std::string &maKH);

    // Dat ngay dang ky (ngay: Ngay dang ky moi)
    void datNgayDangKy(const NgayThang &ngay);

    // Dat tong chi tieu - dung cho CSV import (tien: Tong chi tieu)
    void datTongChiTieu(double tien);

    // ========== METHODS ==========

    // Them chi tieu sau moi lan dat san (soTien: So tien chi tieu)
    void themChiTieu(double soTien);

    // Tang so lan dat san
    void tangSoLanDatSan();

    // Cap nhat hang khach hang dua tren tong chi tieu
    void capNhatHang();

    // Hien thi thong tin khach hang (override)
    virtual void hienThiThongTin() const override;

    // ========== OPERATORS ==========

    // Toan tu gan (other: Doi tuong KhachHang khac)
    KhachHang &operator=(const KhachHang &other);

    // Toan tu so sanh theo ma khach hang (other: Doi tuong KhachHang khac)
    bool operator==(const KhachHang &other) const;

    // Toan tu xuat ra stream (os: Output stream, kh: Doi tuong KhachHang)
    friend std::ostream &operator<<(std::ostream &os, const KhachHang &kh);
};

#endif // KHACHHANG_H
