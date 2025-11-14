/**
 * @file QuanTriVien.cpp
 * @brief Implementation của lớp QuanTriVien
 */

#include "QuanTriVien.h"

// ========== CONSTRUCTORS ==========

QuanTriVien::QuanTriVien()
    : NguoiDung(), ngayTao(), soLanDangNhap(0), lanDangNhapCuoi()
{
    vaiTro = VaiTro::QUAN_TRI_VIEN;
}

QuanTriVien::QuanTriVien(const std::string &ht, const std::string &sdt, const std::string &dc,
                         const std::string &tdn, const std::string &mk)
    : NguoiDung(ht, sdt, dc, tdn, mk, VaiTro::QUAN_TRI_VIEN),
      ngayTao(), soLanDangNhap(0), lanDangNhapCuoi()
{
    // Đặt ngày tạo là ngày hiện tại
    // Trong thực tế cần lấy thời gian hệ thống
}

QuanTriVien::QuanTriVien(const QuanTriVien &other)
    : NguoiDung(other), ngayTao(other.ngayTao),
      soLanDangNhap(other.soLanDangNhap),
      lanDangNhapCuoi(other.lanDangNhapCuoi)
{
}

QuanTriVien::~QuanTriVien()
{
    // Destructor
}

// ========== GETTERS ==========

NgayGio QuanTriVien::layNgayTao() const
{
    return ngayTao;
}

int QuanTriVien::laySoLanDangNhap() const
{
    return soLanDangNhap;
}

NgayGio QuanTriVien::layLanDangNhapCuoi() const
{
    return lanDangNhapCuoi;
}

// ========== METHODS ==========

void QuanTriVien::capNhatDangNhap()
{
    soLanDangNhap++;
    // Trong thực tế: lanDangNhapCuoi = NgayGio hiện tại
}

void QuanTriVien::hienThiThongTin() const
{
    std::cout << "===== QUAN TRI VIEN =====" << std::endl;
    NguoiDung::hienThiThongTin();
    std::cout << "So lan dang nhap: " << soLanDangNhap << std::endl;
}

bool QuanTriVien::ghiFile(std::ofstream &file) const
{
    if (!file.is_open())
        return false;

    // Ghi thông tin lớp cha
    if (!NguoiDung::ghiFile(file))
        return false;

    // Ghi thông tin lớp con
    ngayTao.ghiFile(file);
    file.write(reinterpret_cast<const char *>(&soLanDangNhap), sizeof(soLanDangNhap));
    lanDangNhapCuoi.ghiFile(file);

    return file.good();
}

bool QuanTriVien::docFile(std::ifstream &file)
{
    if (!file.is_open())
        return false;

    // Đọc thông tin lớp cha
    if (!NguoiDung::docFile(file))
        return false;

    // Đọc thông tin lớp con
    ngayTao.docFile(file);
    file.read(reinterpret_cast<char *>(&soLanDangNhap), sizeof(soLanDangNhap));
    lanDangNhapCuoi.docFile(file);

    return file.good();
}

// ========== OPERATORS ==========

QuanTriVien &QuanTriVien::operator=(const QuanTriVien &other)
{
    if (this != &other)
    {
        NguoiDung::operator=(other);
        ngayTao = other.ngayTao;
        soLanDangNhap = other.soLanDangNhap;
        lanDangNhapCuoi = other.lanDangNhapCuoi;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const QuanTriVien &qtv)
{
    os << static_cast<const NguoiDung &>(qtv)
       << ", Logins: " << qtv.soLanDangNhap;
    return os;
}
