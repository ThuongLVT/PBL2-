/**
 * @file NguoiDung.cpp
 * @brief Implementation của lớp NguoiDung
 */

#include "NguoiDung.h"
#include "../Utils/FileHelper.h"

// ========== CONSTRUCTORS ==========

NguoiDung::NguoiDung()
    : ConNguoi(), tenDangNhap(""), matKhau(""),
      vaiTro(VaiTro::NHAN_VIEN), hoatDong(true),
      gioiTinh("Nam"), ngaySinh("01/01/2000")
{
}

NguoiDung::NguoiDung(const std::string &ht, const std::string &sdt, const std::string &dc,
                     const std::string &tdn, const std::string &mk, VaiTro vt)
    : ConNguoi(ht, sdt, dc), tenDangNhap(tdn), matKhau(mk),
      vaiTro(vt), hoatDong(true), gioiTinh("Nam"), ngaySinh("01/01/2000")
{
}

NguoiDung::NguoiDung(const NguoiDung &other)
    : ConNguoi(other), tenDangNhap(other.tenDangNhap),
      matKhau(other.matKhau), vaiTro(other.vaiTro),
      hoatDong(other.hoatDong), gioiTinh(other.gioiTinh),
      ngaySinh(other.ngaySinh)
{
}

NguoiDung::~NguoiDung()
{
    // Destructor
}

// ========== GETTERS ==========

std::string NguoiDung::layTenDangNhap() const
{
    return tenDangNhap;
}

VaiTro NguoiDung::layVaiTro() const
{
    return vaiTro;
}

bool NguoiDung::coHoatDong() const
{
    return hoatDong;
}

std::string NguoiDung::layTenVaiTro() const
{
    switch (vaiTro)
    {
    case VaiTro::QUAN_TRI_VIEN:
        return std::string("Quan tri vien");
    case VaiTro::NHAN_VIEN:
        return std::string("Nhan vien");
    default:
        return std::string("Khong xac dinh");
    }
}

std::string NguoiDung::layMatKhau() const
{
    return matKhau;
}

std::string NguoiDung::layGioiTinh() const
{
    return gioiTinh;
}

std::string NguoiDung::layNgaySinh() const
{
    return ngaySinh;
}

// ========== SETTERS ==========

void NguoiDung::datTenDangNhap(const std::string &tdn)
{
    tenDangNhap = tdn;
}

void NguoiDung::datVaiTro(VaiTro vt)
{
    vaiTro = vt;
}

void NguoiDung::datHoatDong(bool hd)
{
    hoatDong = hd;
}

void NguoiDung::datGioiTinh(const std::string &gt)
{
    gioiTinh = gt;
}

void NguoiDung::datNgaySinh(const std::string &ns)
{
    ngaySinh = ns;
}

// ========== METHODS ==========

bool NguoiDung::dangNhap(const std::string &mk) const
{
    if (!hoatDong)
    {
        return false; // Tài khoản bị khóa
    }
    return matKhau == mk;
}

bool NguoiDung::doiMatKhau(const std::string &mkCu, const std::string &mkMoi)
{
    if (matKhau == mkCu)
    {
        matKhau = mkMoi;
        return true;
    }
    return false;
}

void NguoiDung::chuyenTrangThaiHoatDong()
{
    hoatDong = !hoatDong;
}

void NguoiDung::hienThiThongTin() const
{
    ConNguoi::hienThiThongTin(); // Gọi phương thức lớp cha
    std::cout << "Ten dang nhap: " << tenDangNhap << std::endl;
    std::cout << "Vai tro: " << layTenVaiTro() << std::endl;
    std::cout << "Trang thai: " << (hoatDong ? "Hoat dong" : "Khoa") << std::endl;
}

bool NguoiDung::ghiFile(std::ofstream &file) const
{
    if (!file.is_open())
        return false;

    // Ghi thông tin lớp cha
    if (!ConNguoi::ghiFile(file))
        return false;

    // Ghi thông tin lớp con
    if (!FileHelper::ghiString(file, tenDangNhap))
        return false;
    if (!FileHelper::ghiString(file, matKhau))
        return false;

    // Ghi vai trò
    int vaiTroInt = static_cast<int>(vaiTro);
    file.write(reinterpret_cast<const char *>(&vaiTroInt), sizeof(vaiTroInt));

    // Ghi trạng thái
    file.write(reinterpret_cast<const char *>(&hoatDong), sizeof(hoatDong));

    return file.good();
}

bool NguoiDung::docFile(std::ifstream &file)
{
    if (!file.is_open())
        return false;

    // Đọc thông tin lớp cha
    if (!ConNguoi::docFile(file))
        return false;

    // Đọc thông tin lớp con
    if (!FileHelper::docString(file, tenDangNhap))
        return false;
    if (!FileHelper::docString(file, matKhau))
        return false;

    // Đọc vai trò
    int vaiTroInt;
    file.read(reinterpret_cast<char *>(&vaiTroInt), sizeof(vaiTroInt));
    vaiTro = static_cast<VaiTro>(vaiTroInt);

    // Đọc trạng thái
    file.read(reinterpret_cast<char *>(&hoatDong), sizeof(hoatDong));

    return file.good();
}

// ========== OPERATORS ==========

NguoiDung &NguoiDung::operator=(const NguoiDung &other)
{
    if (this != &other)
    {
        ConNguoi::operator=(other); // Gọi toán tử gán lớp cha
        tenDangNhap = other.tenDangNhap;
        matKhau = other.matKhau;
        vaiTro = other.vaiTro;
        hoatDong = other.hoatDong;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const NguoiDung &nd)
{
    os << static_cast<const ConNguoi &>(nd) << ", Username: " << nd.tenDangNhap
       << ", Role: " << nd.layTenVaiTro()
       << ", Status: " << (nd.hoatDong ? "Active" : "Locked");
    return os;
}
