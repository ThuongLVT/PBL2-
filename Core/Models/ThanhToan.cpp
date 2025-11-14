#include "ThanhToan.h"
#include <iostream>

using namespace std;
#include <ctime>
#include <cstdio>

#include "../Utils/FileHelper.h"
// Constructor mặc định
ThanhToan::ThanhToan()
    : maThanhToan(""), donDatSan(nullptr), soTien(0.0),
      phuongThuc(TIEN_MAT), nguoiThu(""), ghiChu("") {}

// Constructor với tham số
ThanhToan::ThanhToan(const std::string &ma, DatSan *ds, double st, PhuongThucThanhToan pt)
    : maThanhToan(ma), donDatSan(ds), soTien(st), phuongThuc(pt),
      nguoiThu(""), ghiChu("")
{
    ngayThanhToan = NgayGio::layThoiGianHienTai();
}

// Copy constructor
ThanhToan::ThanhToan(const ThanhToan &other)
{
    maThanhToan = other.maThanhToan;
    donDatSan = other.donDatSan;
    soTien = other.soTien;
    ngayThanhToan = other.ngayThanhToan;
    phuongThuc = other.phuongThuc;
    nguoiThu = other.nguoiThu;
    ghiChu = other.ghiChu;
}

// Destructor
ThanhToan::~ThanhToan()
{
    // Không delete donDatSan vì được quản lý bởi HeThongQuanLy
}

// Assignment operator
ThanhToan &ThanhToan::operator=(const ThanhToan &other)
{
    if (this != &other)
    {
        maThanhToan = other.maThanhToan;
        donDatSan = other.donDatSan;
        soTien = other.soTien;
        ngayThanhToan = other.ngayThanhToan;
        phuongThuc = other.phuongThuc;
        nguoiThu = other.nguoiThu;
        ghiChu = other.ghiChu;
    }
    return *this;
}

// Getters
std::string ThanhToan::getMaThanhToan() const { return maThanhToan; }
DatSan *ThanhToan::getDonDatSan() const { return donDatSan; }
double ThanhToan::getSoTien() const { return soTien; }
NgayGio ThanhToan::getNgayThanhToan() const { return ngayThanhToan; }
PhuongThucThanhToan ThanhToan::getPhuongThuc() const { return phuongThuc; }
std::string ThanhToan::getNguoiThu() const { return nguoiThu; }
std::string ThanhToan::getGhiChu() const { return ghiChu; }

// Setters
void ThanhToan::setMaThanhToan(const std::string &ma) { maThanhToan = ma; }
void ThanhToan::setDonDatSan(DatSan *ds) { donDatSan = ds; }
void ThanhToan::setSoTien(double st) { soTien = st; }
void ThanhToan::setNguoiThu(const std::string &nt) { nguoiThu = nt; }
void ThanhToan::setGhiChu(const std::string &gc) { ghiChu = gc; }

// Methods
std::string ThanhToan::getPhuongThucText() const
{
    switch (phuongThuc)
    {
    case TIEN_MAT:
        return std::string("Tien mat");
    case THE:
        return std::string("The");
    case CHUYEN_KHOAN:
        return std::string("Chuyen khoan");
    default:
        return std::string("Khong xac dinh");
    }
}

void ThanhToan::inHoaDon() const
{
    std::cout << "\n========== HOA DON THANH TOAN ==========" << std::endl;
    std::cout << "Ma thanh toan: " << maThanhToan.c_str() << std::endl;
    std::cout << "Ngay thanh toan: " << ngayThanhToan.toString().c_str() << std::endl;

    if (donDatSan != nullptr)
    {
        std::cout << "Ma dat san: " << donDatSan->getMaDatSan().c_str() << std::endl;
        if (donDatSan->getKhachHang() != nullptr)
        {
            std::cout << "Khach hang: " << donDatSan->getKhachHang()->getHoTen().c_str() << std::endl;
        }
    }

    std::cout << "So tien: " << soTien << " VND" << std::endl;
    std::cout << "Phuong thuc: " << getPhuongThucText().c_str() << std::endl;

    if (nguoiThu.length() > 0)
    {
        std::cout << "Nguoi thu: " << nguoiThu.c_str() << std::endl;
    }

    if (ghiChu.length() > 0)
    {
        std::cout << "Ghi chu: " << ghiChu.c_str() << std::endl;
    }

    std::cout << "========================================" << std::endl;
}

// File I/O
void ThanhToan::ghiFile(FILE *f) const
{
    if (f == nullptr)
        return;

    // Ghi mã thanh toán
    int len = maThanhToan.length();
    fwrite(&len, sizeof(int), 1, f);
    fwrite(maThanhToan.c_str(), sizeof(char), len, f);

    // Ghi mã đơn đặt sân (tham chiếu)
    if (donDatSan != nullptr)
    {
        len = donDatSan->getMaDatSan().length();
        fwrite(&len, sizeof(int), 1, f);
        fwrite(donDatSan->getMaDatSan().c_str(), sizeof(char), len, f);
    }
    else
    {
        len = 0;
        fwrite(&len, sizeof(int), 1, f);
    }

    // Ghi số tiền, ngày thanh toán
    fwrite(&soTien, sizeof(double), 1, f);
    ngayThanhToan.ghiFile(f);

    // Ghi phương thức
    int pt = static_cast<int>(phuongThuc);
    fwrite(&pt, sizeof(int), 1, f);

    // Ghi người thu
    len = nguoiThu.length();
    fwrite(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        fwrite(nguoiThu.c_str(), sizeof(char), len, f);
    }

    // Ghi ghi chú
    len = ghiChu.length();
    fwrite(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        fwrite(ghiChu.c_str(), sizeof(char), len, f);
    }
}

void ThanhToan::docFile(FILE *f)
{
    if (f == nullptr)
        return;

    // Đọc mã thanh toán
    int len;
    fread(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        char *buffer = new char[len + 1];
        fread(buffer, sizeof(char), len, f);
        buffer[len] = '\0';
        maThanhToan = std::string(buffer);
        delete[] buffer;
    }

    // Đọc mã đơn đặt sân (cần resolve)
    fread(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        char *buffer = new char[len + 1];
        fread(buffer, sizeof(char), len, f);
        buffer[len] = '\0';
        // TODO: Resolve donDatSan pointer
        delete[] buffer;
    }

    // Đọc số tiền, ngày thanh toán
    fread(&soTien, sizeof(double), 1, f);
    ngayThanhToan.docFile(f);

    // Đọc phương thức
    int pt;
    fread(&pt, sizeof(int), 1, f);
    phuongThuc = static_cast<PhuongThucThanhToan>(pt);

    // Đọc người thu
    fread(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        char *buffer = new char[len + 1];
        fread(buffer, sizeof(char), len, f);
        buffer[len] = '\0';
        nguoiThu = std::string(buffer);
        delete[] buffer;
    }

    // Đọc ghi chú
    fread(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        char *buffer = new char[len + 1];
        fread(buffer, sizeof(char), len, f);
        buffer[len] = '\0';
        ghiChu = std::string(buffer);
        delete[] buffer;
    }
}

// Static method
std::string ThanhToan::taoMaThanhToan()
{
    time_t now = time(0);
    char buffer[50];
    sprintf(buffer, "TT%lld", static_cast<long long>(now));
    return std::string(buffer);
}
