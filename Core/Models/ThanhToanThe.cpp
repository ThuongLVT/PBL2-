#include "ThanhToanThe.h"
#include <iostream>

using namespace std;

// Constructor mặc định
ThanhToanThe::ThanhToanThe() : ThanhToan(), soThe(""), loaiThe(""), maPinGiaoDich("")
{
    phuongThuc = THE;
}

// Constructor với tham số
ThanhToanThe::ThanhToanThe(const std::string &ma, DatSan *ds, double st)
    : ThanhToan(ma, ds, st, THE), soThe(""), loaiThe(""), maPinGiaoDich("") {}

// Copy constructor
ThanhToanThe::ThanhToanThe(const ThanhToanThe &other) : ThanhToan(other)
{
    soThe = other.soThe;
    loaiThe = other.loaiThe;
    maPinGiaoDich = other.maPinGiaoDich;
}

// Destructor
ThanhToanThe::~ThanhToanThe() {}

// Assignment operator
ThanhToanThe &ThanhToanThe::operator=(const ThanhToanThe &other)
{
    if (this != &other)
    {
        ThanhToan::operator=(other);
        soThe = other.soThe;
        loaiThe = other.loaiThe;
        maPinGiaoDich = other.maPinGiaoDich;
    }
    return *this;
}

// Getters
std::string ThanhToanThe::getSoThe() const
{
    return soThe;
}

std::string ThanhToanThe::getLoaiThe() const
{
    return loaiThe;
}

std::string ThanhToanThe::getMaPinGiaoDich() const
{
    return maPinGiaoDich;
}

// Setters
void ThanhToanThe::setSoThe(const std::string &st)
{
    soThe = st;
}

void ThanhToanThe::setLoaiThe(const std::string &lt)
{
    loaiThe = lt;
}

void ThanhToanThe::setMaPinGiaoDich(const std::string &mp)
{
    maPinGiaoDich = mp;
}

// Methods
std::string ThanhToanThe::maskSoThe() const
{
    if (soThe.length() < 4)
    {
        return std::string("****");
    }

    // Lấy 4 số cuối
    std::string masked("**** **** **** ");
    for (size_t i = soThe.length() - 4; i < soThe.length(); i++)
    {
        masked += soThe[i]; // Append character directly
    }

    return masked;
}

void ThanhToanThe::xacNhanThanhToan()
{
    std::cout << "Xac nhan thanh toan qua the thanh cong!" << std::endl;
    std::cout << "Loai the: " << loaiThe.c_str() << std::endl;
    std::cout << "So the: " << maskSoThe().c_str() << std::endl;
    std::cout << "Ma giao dich: " << maPinGiaoDich.c_str() << std::endl;
}

void ThanhToanThe::hienThi() const
{
    std::cout << "\n===== THANH TOAN QUA THE =====" << std::endl;
    std::cout << "Ma thanh toan: " << maThanhToan.c_str() << std::endl;
    std::cout << "Ngay thanh toan: " << ngayThanhToan.toString().c_str() << std::endl;
    std::cout << "So tien: " << soTien << " VND" << std::endl;
    std::cout << "Loai the: " << loaiThe.c_str() << std::endl;
    std::cout << "So the: " << maskSoThe().c_str() << std::endl;
    std::cout << "Ma giao dich: " << maPinGiaoDich.c_str() << std::endl;

    if (nguoiThu.length() > 0)
    {
        std::cout << "Nguoi thu: " << nguoiThu.c_str() << std::endl;
    }
}

// File I/O
void ThanhToanThe::ghiFile(FILE *f) const
{
    if (f == nullptr)
        return;

    // Ghi base class
    ThanhToan::ghiFile(f);

    // Ghi số thẻ
    int len = soThe.length();
    fwrite(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        fwrite(soThe.c_str(), sizeof(char), len, f);
    }

    // Ghi loại thẻ
    len = loaiThe.length();
    fwrite(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        fwrite(loaiThe.c_str(), sizeof(char), len, f);
    }

    // Ghi mã PIN giao dịch
    len = maPinGiaoDich.length();
    fwrite(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        fwrite(maPinGiaoDich.c_str(), sizeof(char), len, f);
    }
}

void ThanhToanThe::docFile(FILE *f)
{
    if (f == nullptr)
        return;

    // Đọc base class
    ThanhToan::docFile(f);

    // Đọc số thẻ
    int len;
    fread(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        char *buffer = new char[len + 1];
        fread(buffer, sizeof(char), len, f);
        buffer[len] = '\0';
        soThe = std::string(buffer);
        delete[] buffer;
    }

    // Đọc loại thẻ
    fread(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        char *buffer = new char[len + 1];
        fread(buffer, sizeof(char), len, f);
        buffer[len] = '\0';
        loaiThe = std::string(buffer);
        delete[] buffer;
    }

    // Đọc mã PIN giao dịch
    fread(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        char *buffer = new char[len + 1];
        fread(buffer, sizeof(char), len, f);
        buffer[len] = '\0';
        maPinGiaoDich = std::string(buffer);
        delete[] buffer;
    }
}
