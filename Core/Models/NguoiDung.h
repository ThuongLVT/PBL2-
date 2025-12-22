// NguoiDung.h - Lop dai dien cho nguoi dung he thong (co tai khoan dang nhap)
// Ke thua tu ConNguoi, them thong tin dang nhap va vai tro
// Lop co so cho QuanTriVien va NhanVien

#ifndef NGUOIDUNG_H
#define NGUOIDUNG_H
#include <string>

#include "ConNguoi.h"

// Cac vai tro nguoi dung trong he thong
enum class VaiTro
{
    QUAN_TRI_VIEN, // Quan tri vien (Admin)
    NHAN_VIEN      // Nhan vien (Staff)
};

// Lop dai dien cho nguoi dung co tai khoan trong he thong
// Ke thua tu ConNguoi va them: Ten dang nhap, Mat khau, Vai tro, Trang thai hoat dong
class NguoiDung : public ConNguoi
{
protected:
    std::string tenDangNhap; // Ten dang nhap duy nhat
    std::string matKhau;     // Mat khau (nen ma hoa trong thuc te)
    VaiTro vaiTro;           // Vai tro trong he thong
    bool hoatDong;           // Trang thai tai khoan (true = hoat dong)
    std::string gioiTinh;    // Gioi tinh ("Nam", "Nu", "Khac")
    std::string ngaySinh;    // Ngay sinh (format: DD/MM/YYYY)

public:
    // ========== CONSTRUCTORS ==========

    // Constructor mac dinh
    NguoiDung();

    // Constructor co tham so (ht: Ho ten, sdt: So dien thoai, dc: Dia chi, tdn: Ten dang nhap, mk: Mat khau, vt: Vai tro)
    NguoiDung(const std::string &ht, const std::string &sdt, const std::string &dc,
              const std::string &tdn, const std::string &mk, VaiTro vt);

    // Copy constructor (other: Doi tuong NguoiDung khac)
    NguoiDung(const NguoiDung &other);

    // Destructor ao
    virtual ~NguoiDung();

    // Assignment operator (other: Doi tuong NguoiDung khac de gan)
    NguoiDung& operator=(const NguoiDung &other);

    // ========== GETTERS ==========

    // Lay ten dang nhap
    std::string layTenDangNhap() const;

    // Lay vai tro
    VaiTro layVaiTro() const;

    // Kiem tra tai khoan co hoat dong khong
    bool coHoatDong() const;

    // Lay chuoi mo ta vai tro
    std::string layTenVaiTro() const;

    // Lay mat khau (chi dung cho muc dich hien thi trong UI)
    std::string layMatKhau() const;

    // Lay gioi tinh
    std::string layGioiTinh() const;

    // Lay ngay sinh (DD/MM/YYYY)
    std::string layNgaySinh() const;

    // ========== SETTERS ==========

    // Dat ten dang nhap (tdn: Ten dang nhap moi)
    void datTenDangNhap(const std::string &tdn);

    // Dat vai tro (vt: Vai tro moi)
    void datVaiTro(VaiTro vt);

    // Dat trang thai hoat dong (hd: Trang thai hoat dong)
    void datHoatDong(bool hd);

    // Dat gioi tinh (gt: Gioi tinh)
    void datGioiTinh(const std::string &gt);

    // Dat ngay sinh (ns: Ngay sinh DD/MM/YYYY)
    void datNgaySinh(const std::string &ns);

    // ========== METHODS ==========

    // Dang nhap vao he thong (mk: Mat khau nhap vao)
    bool dangNhap(const std::string &mk) const;

    // Doi mat khau (mkCu: Mat khau cu, mkMoi: Mat khau moi)
    bool doiMatKhau(const std::string &mkCu, const std::string &mkMoi);

    // Khoa/mo khoa tai khoan
    void chuyenTrangThaiHoatDong();

    // Hien thi thong tin nguoi dung (override)
    virtual void hienThiThongTin() const override;

    // ========== OPERATORS ==========

    // Toan tu xuat ra stream (os: Output stream, nd: Doi tuong NguoiDung)
    friend std::ostream &operator<<(std::ostream &os, const NguoiDung &nd);
};

#endif // NGUOIDUNG_H
