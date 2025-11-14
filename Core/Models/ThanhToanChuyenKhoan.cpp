#include "ThanhToanChuyenKhoan.h"
#include <iostream>

using namespace std;

// Constructor mặc định
ThanhToanChuyenKhoan::ThanhToanChuyenKhoan()
    : ThanhToan(), soTaiKhoan(""), tenNganHang(""), maDinhDanh(""), noiDungCK("")
{
    phuongThuc = CHUYEN_KHOAN;
}

// Constructor với tham số
ThanhToanChuyenKhoan::ThanhToanChuyenKhoan(const std::string &ma, DatSan *ds, double st)
    : ThanhToan(ma, ds, st, CHUYEN_KHOAN),
      soTaiKhoan(""), tenNganHang(""), maDinhDanh(""), noiDungCK("") {}

// Copy constructor
ThanhToanChuyenKhoan::ThanhToanChuyenKhoan(const ThanhToanChuyenKhoan &other)
    : ThanhToan(other)
{
    soTaiKhoan = other.soTaiKhoan;
    tenNganHang = other.tenNganHang;
    maDinhDanh = other.maDinhDanh;
    noiDungCK = other.noiDungCK;
}

// Destructor
ThanhToanChuyenKhoan::~ThanhToanChuyenKhoan() {}

// Assignment operator
ThanhToanChuyenKhoan &ThanhToanChuyenKhoan::operator=(const ThanhToanChuyenKhoan &other)
{
    if (this != &other)
    {
        ThanhToan::operator=(other);
        soTaiKhoan = other.soTaiKhoan;
        tenNganHang = other.tenNganHang;
        maDinhDanh = other.maDinhDanh;
        noiDungCK = other.noiDungCK;
    }
    return *this;
}

// Getters
std::string ThanhToanChuyenKhoan::getSoTaiKhoan() const
{
    return soTaiKhoan;
}

std::string ThanhToanChuyenKhoan::getTenNganHang() const
{
    return tenNganHang;
}

std::string ThanhToanChuyenKhoan::getMaDinhDanh() const
{
    return maDinhDanh;
}

std::string ThanhToanChuyenKhoan::getNoiDungCK() const
{
    return noiDungCK;
}

// Setters
void ThanhToanChuyenKhoan::setSoTaiKhoan(const std::string &stk)
{
    soTaiKhoan = stk;
}

void ThanhToanChuyenKhoan::setTenNganHang(const std::string &tnh)
{
    tenNganHang = tnh;
}

void ThanhToanChuyenKhoan::setMaDinhDanh(const std::string &mdd)
{
    maDinhDanh = mdd;
}

void ThanhToanChuyenKhoan::setNoiDungCK(const std::string &nd)
{
    noiDungCK = nd;
}

// Methods
void ThanhToanChuyenKhoan::xacNhanThanhToan()
{
    std::cout << "Xac nhan thanh toan chuyen khoan thanh cong!" << std::endl;
    std::cout << "Ngan hang: " << tenNganHang.c_str() << std::endl;
    std::cout << "So tai khoan: " << soTaiKhoan.c_str() << std::endl;
    std::cout << "Ma dinh danh: " << maDinhDanh.c_str() << std::endl;
}

void ThanhToanChuyenKhoan::hienThi() const
{
    std::cout << "\n===== THANH TOAN CHUYEN KHOAN =====" << std::endl;
    std::cout << "Ma thanh toan: " << maThanhToan.c_str() << std::endl;
    std::cout << "Ngay thanh toan: " << ngayThanhToan.toString().c_str() << std::endl;
    std::cout << "So tien: " << soTien << " VND" << std::endl;
    std::cout << "Ngan hang: " << tenNganHang.c_str() << std::endl;
    std::cout << "So tai khoan: " << soTaiKhoan.c_str() << std::endl;
    std::cout << "Ma dinh danh: " << maDinhDanh.c_str() << std::endl;
    std::cout << "Noi dung: " << noiDungCK.c_str() << std::endl;

    if (nguoiThu.length() > 0)
    {
        std::cout << "Nguoi thu: " << nguoiThu.c_str() << std::endl;
    }
}

// File I/O
void ThanhToanChuyenKhoan::ghiFile(FILE *f) const
{
    if (f == nullptr)
        return;

    // Ghi base class
    ThanhToan::ghiFile(f);

    // Ghi số tài khoản
    int len = soTaiKhoan.length();
    fwrite(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        fwrite(soTaiKhoan.c_str(), sizeof(char), len, f);
    }

    // Ghi tên ngân hàng
    len = tenNganHang.length();
    fwrite(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        fwrite(tenNganHang.c_str(), sizeof(char), len, f);
    }

    // Ghi mã định danh
    len = maDinhDanh.length();
    fwrite(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        fwrite(maDinhDanh.c_str(), sizeof(char), len, f);
    }

    // Ghi nội dung CK
    len = noiDungCK.length();
    fwrite(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        fwrite(noiDungCK.c_str(), sizeof(char), len, f);
    }
}

void ThanhToanChuyenKhoan::docFile(FILE *f)
{
    if (f == nullptr)
        return;

    // Đọc base class
    ThanhToan::docFile(f);

    // Đọc số tài khoản
    int len;
    fread(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        char *buffer = new char[len + 1];
        fread(buffer, sizeof(char), len, f);
        buffer[len] = '\0';
        soTaiKhoan = std::string(buffer);
        delete[] buffer;
    }

    // Đọc tên ngân hàng
    fread(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        char *buffer = new char[len + 1];
        fread(buffer, sizeof(char), len, f);
        buffer[len] = '\0';
        tenNganHang = std::string(buffer);
        delete[] buffer;
    }

    // Đọc mã định danh
    fread(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        char *buffer = new char[len + 1];
        fread(buffer, sizeof(char), len, f);
        buffer[len] = '\0';
        maDinhDanh = std::string(buffer);
        delete[] buffer;
    }

    // Đọc nội dung CK
    fread(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        char *buffer = new char[len + 1];
        fread(buffer, sizeof(char), len, f);
        buffer[len] = '\0';
        noiDungCK = std::string(buffer);
        delete[] buffer;
    }
}
