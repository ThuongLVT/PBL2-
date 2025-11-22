/**
 * @file NhanVien.cpp
 * @brief Implementation của lớp NhanVien
 */

#include "NhanVien.h"

#include "../Utils/FileHelper.h"
// ========== CONSTRUCTORS ==========

NhanVien::NhanVien()
    : NguoiDung(), maNhanVien(""), ngayVaoLam(),
      luongCoBan(0.0), caLamViec(CaLamViec::SANG), soNgayLam(0)
{
    vaiTro = VaiTro::NHAN_VIEN;
}

NhanVien::NhanVien(const std::string &ht, const std::string &sdt, const std::string &dc,
                   const std::string &tdn, const std::string &mk, const std::string &maNV,
                   double luong, CaLamViec ca)
    : NguoiDung(ht, sdt, dc, tdn, mk, VaiTro::NHAN_VIEN),
      maNhanVien(maNV), ngayVaoLam(), luongCoBan(luong),
      caLamViec(ca), soNgayLam(0)
{
}

NhanVien::NhanVien(const NhanVien &other)
    : NguoiDung(other), maNhanVien(other.maNhanVien),
      ngayVaoLam(other.ngayVaoLam), luongCoBan(other.luongCoBan),
      caLamViec(other.caLamViec), soNgayLam(other.soNgayLam)
{
}

NhanVien::~NhanVien()
{
    // Destructor
}

// ========== ASSIGNMENT OPERATOR ==========

NhanVien& NhanVien::operator=(const NhanVien &other)
{
    if (this != &other)
    {
        // Copy base class members
        NguoiDung::operator=(other);
        
        // Copy NhanVien specific members
        maNhanVien = other.maNhanVien;
        ngayVaoLam = other.ngayVaoLam;
        luongCoBan = other.luongCoBan;
        caLamViec = other.caLamViec;
        soNgayLam = other.soNgayLam;
    }
    return *this;
}

// ========== GETTERS ==========

std::string NhanVien::layMaNhanVien() const
{
    return maNhanVien;
}

NgayThang NhanVien::layNgayVaoLam() const
{
    return ngayVaoLam;
}

double NhanVien::layLuongCoBan() const
{
    return luongCoBan;
}

CaLamViec NhanVien::layCaLamViec() const
{
    return caLamViec;
}

int NhanVien::laySoNgayLam() const
{
    return soNgayLam;
}

std::string NhanVien::layTenCaLamViec() const
{
    switch (caLamViec)
    {
    case CaLamViec::SANG:
        return std::string("Ca sang (6h-14h)");
    case CaLamViec::CHIEU:
        return std::string("Ca chieu (14h-22h)");
    case CaLamViec::TOI:
        return std::string("Ca toi (18h-02h)");
    default:
        return std::string("Khong xac dinh");
    }
}

// ========== SETTERS ==========

void NhanVien::datLuongCoBan(double luong)
{
    if (luong >= 0)
    {
        luongCoBan = luong;
    }
}

void NhanVien::datCaLamViec(CaLamViec ca)
{
    caLamViec = ca;
}

// ========== METHODS ==========

void NhanVien::tangSoNgayLam()
{
    soNgayLam++;
}

double NhanVien::tinhLuong() const
{
    // Công thức đơn giản: Lương cơ bản * số ngày làm / 26 (giả sử 1 tháng 26 ngày công)
    return luongCoBan * soNgayLam / 26.0;
}

void NhanVien::hienThiThongTin() const
{
    std::cout << "===== NHAN VIEN =====" << std::endl;
    NguoiDung::hienThiThongTin();
    std::cout << "Ma nhan vien: " << maNhanVien << std::endl;
    std::cout << "Luong co ban: " << luongCoBan << " VND" << std::endl;
    std::cout << "Ca lam viec: " << layTenCaLamViec() << std::endl;
    std::cout << "So ngay lam: " << soNgayLam << std::endl;
}

bool NhanVien::ghiFile(std::ofstream &file) const
{
    if (!file.is_open())
        return false;

    // Ghi thông tin lớp cha
    if (!NguoiDung::ghiFile(file))
        return false;

    // Ghi thông tin lớp con
    if (!FileHelper::ghiString(file, maNhanVien))
        return false;
    ngayVaoLam.ghiFile(file);
    file.write(reinterpret_cast<const char *>(&luongCoBan), sizeof(luongCoBan));

    int caLamViecInt = static_cast<int>(caLamViec);
    file.write(reinterpret_cast<const char *>(&caLamViecInt), sizeof(caLamViecInt));

    file.write(reinterpret_cast<const char *>(&soNgayLam), sizeof(soNgayLam));

    return file.good();
}

bool NhanVien::docFile(std::ifstream &file)
{
    if (!file.is_open())
        return false;

    // Đọc thông tin lớp cha
    if (!NguoiDung::docFile(file))
        return false;

    // Đọc thông tin lớp con
    if (!FileHelper::docString(file, maNhanVien))
        return false;
    ngayVaoLam.docFile(file);
    file.read(reinterpret_cast<char *>(&luongCoBan), sizeof(luongCoBan));

    int caLamViecInt;
    file.read(reinterpret_cast<char *>(&caLamViecInt), sizeof(caLamViecInt));
    caLamViec = static_cast<CaLamViec>(caLamViecInt);

    file.read(reinterpret_cast<char *>(&soNgayLam), sizeof(soNgayLam));

    return file.good();
}

// ========== OPERATORS ==========

std::ostream &operator<<(std::ostream &os, const NhanVien &nv)
{
    os << static_cast<const NguoiDung &>(nv)
       << ", MaNV: " << nv.maNhanVien
       << ", Luong: " << nv.luongCoBan;
    return os;
}
