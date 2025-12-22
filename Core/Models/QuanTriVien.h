// QuanTriVien.h - Lop dai dien cho Quan tri vien he thong
// Ke thua tu NguoiDung, co quyen cao nhat trong he thong

#ifndef QUANTRIVIEN_H
#define QUANTRIVIEN_H
#include <string>

#include "NguoiDung.h"
#include "../CauTrucDuLieu/NgayGio.h"

// Lop dai dien cho Quan tri vien
// Quan tri vien co quyen: Quan ly tat ca nguoi dung, Xem tat ca bao cao thong ke,
// Cau hinh he thong, Sao luu/khoi phuc du lieu
class QuanTriVien : public NguoiDung
{
private:
    NgayGio ngayTao;         // Ngay tao tai khoan admin
    int soLanDangNhap;       // So lan dang nhap
    NgayGio lanDangNhapCuoi; // Lan dang nhap cuoi cung

public:
    // ========== CONSTRUCTORS ==========

    // Constructor mac dinh
    QuanTriVien();

    // Constructor co tham so (ht: Ho ten, sdt: So dien thoai, dc: Dia chi, tdn: Ten dang nhap, mk: Mat khau)
    QuanTriVien(const std::string &ht, const std::string &sdt, const std::string &dc,
                const std::string &tdn, const std::string &mk);

    // Copy constructor (other: Doi tuong QuanTriVien khac)
    QuanTriVien(const QuanTriVien &other);

    // Destructor
    virtual ~QuanTriVien();

    // ========== GETTERS ==========

    // Lay ngay tao tai khoan
    NgayGio layNgayTao() const;

    // Lay so lan dang nhap
    int laySoLanDangNhap() const;

    // Lay lan dang nhap cuoi
    NgayGio layLanDangNhapCuoi() const;

    // ========== METHODS ==========

    // Cap nhat lan dang nhap (goi khi dang nhap thanh cong)
    void capNhatDangNhap();

    // Hien thi thong tin quan tri vien (override)
    virtual void hienThiThongTin() const override;

    // ========== OPERATORS ==========

    // Toan tu gan (other: Doi tuong QuanTriVien khac)
    QuanTriVien &operator=(const QuanTriVien &other);

    // Toan tu xuat ra stream (os: Output stream, qtv: Doi tuong QuanTriVien)
    friend std::ostream &operator<<(std::ostream &os, const QuanTriVien &qtv);
};

#endif // QUANTRIVIEN_H
