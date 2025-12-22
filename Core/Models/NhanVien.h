// NhanVien.h - Lop dai dien cho Nhan vien he thong
// Ke thua tu NguoiDung, quan ly cong viec hang ngay

#ifndef NHANVIEN_H
#define NHANVIEN_H
#include <string>

#include "NguoiDung.h"
#include "../CauTrucDuLieu/NgayGio.h"

// Cac ca lam viec
enum class CaLamViec
{
    SANG,  // Ca sang (6h-14h)
    CHIEU, // Ca chieu (14h-22h)
    TOI    // Ca toi (18h-02h)
};

// Lop dai dien cho Nhan vien
// Nhan vien co quyen: Quan ly dat san, Quan ly khach hang, Quan ly dich vu, Xu ly thanh toan
class NhanVien : public NguoiDung
{
private:
    std::string maNhanVien; // Ma nhan vien
    NgayThang ngayVaoLam;   // Ngay vao lam
    double luongCoBan;      // Luong co ban
    CaLamViec caLamViec;    // Ca lam viec chinh
    int soNgayLam;          // Tong so ngay da lam

public:
    // ========== CONSTRUCTORS ==========

    // Constructor mac dinh
    NhanVien();

    // Constructor co tham so (ht: Ho ten, sdt: So dien thoai, dc: Dia chi, tdn: Ten dang nhap, mk: Mat khau, maNV: Ma nhan vien, luong: Luong co ban, ca: Ca lam viec)
    NhanVien(const std::string &ht, const std::string &sdt, const std::string &dc,
             const std::string &tdn, const std::string &mk, const std::string &maNV,
             double luong, CaLamViec ca);

    // Copy constructor (other: Doi tuong NhanVien khac)
    NhanVien(const NhanVien &other);

    // Destructor
    virtual ~NhanVien();

    // Assignment operator (other: Doi tuong NhanVien khac)
    NhanVien &operator=(const NhanVien &other);

    // ========== GETTERS ==========

    // Lay ma nhan vien
    std::string layMaNhanVien() const;

    // Lay ngay vao lam
    NgayThang layNgayVaoLam() const;

    // Lay luong co ban
    double layLuongCoBan() const;

    // Lay ca lam viec
    CaLamViec layCaLamViec() const;

    // Lay so ngay da lam
    int laySoNgayLam() const;

    // Lay ten ca lam viec (dang chuoi)
    std::string layTenCaLamViec() const;

    // ========== SETTERS ==========

    // Dat luong co ban (luong: Luong moi)
    void datLuongCoBan(double luong);

    // Dat ca lam viec (ca: Ca lam viec moi)
    void datCaLamViec(CaLamViec ca);

    // ========== METHODS ==========

    // Tang so ngay lam viec
    void tangSoNgayLam();

    // Tinh luong theo so ngay lam
    double tinhLuong() const;

    // Hien thi thong tin nhan vien (override)
    virtual void hienThiThongTin() const override;

    // ========== OPERATORS ==========

    // Toan tu xuat ra stream (os: Output stream, nv: Doi tuong NhanVien)
    friend std::ostream &operator<<(std::ostream &os, const NhanVien &nv);
};

#endif // NHANVIEN_H
