#include "DichVuDat.h"
#include <iostream>
#include "../Utils/FileHelper.h"

// Constructor mặc định
DichVuDat::DichVuDat() : dichVu(nullptr), soLuong(0), thanhTien(0.0) {}

// Constructor với tham số
DichVuDat::DichVuDat(DichVu *dv, int sl) : dichVu(dv), soLuong(sl)
{
    tinhThanhTien();
}

// Copy constructor
DichVuDat::DichVuDat(const DichVuDat &other)
{
    dichVu = other.dichVu; // Shallow copy pointer
    soLuong = other.soLuong;
    thanhTien = other.thanhTien;
}

// Destructor
DichVuDat::~DichVuDat()
{
    // Không delete dichVu vì nó được quản lý bởi HeThongQuanLy
}

// Assignment operator
DichVuDat &DichVuDat::operator=(const DichVuDat &other)
{
    if (this != &other)
    {
        dichVu = other.dichVu;
        soLuong = other.soLuong;
        thanhTien = other.thanhTien;
    }
    return *this;
}

// Getters
DichVu *DichVuDat::getDichVu() const
{
    return dichVu;
}

int DichVuDat::getSoLuong() const
{
    return soLuong;
}

double DichVuDat::getThanhTien() const
{
    return thanhTien;
}

// Setters
void DichVuDat::setDichVu(DichVu *dv)
{
    dichVu = dv;
    tinhThanhTien();
}

void DichVuDat::setSoLuong(int sl)
{
    soLuong = sl;
    tinhThanhTien();
}

// Methods
void DichVuDat::tinhThanhTien()
{
    if (dichVu != nullptr)
    {
        thanhTien = dichVu->layDonGia() * soLuong;
    }
    else
    {
        thanhTien = 0.0;
    }
}

void DichVuDat::hienThi() const
{
    if (dichVu != nullptr)
    {
        std::cout << "  - Dich vu: ";
        dichVu->hienThiThongTin();
        std::cout << "    So luong: " << soLuong << std::endl;
        std::cout << "    Thanh tien: " << thanhTien << " VND" << std::endl;
    }
}

// File I/O
void DichVuDat::ghiFile(FILE *f) const
{
    if (f == nullptr)
        return;

    // Ghi mã dịch vụ (để tham chiếu)
    if (dichVu != nullptr)
    {
        std::string maDV = dichVu->layMaDichVu();
        int len = maDV.length();
        fwrite(&len, sizeof(int), 1, f);
        fwrite(maDV.c_str(), sizeof(char), len, f);
    }
    else
    {
        int len = 0;
        fwrite(&len, sizeof(int), 1, f);
    }

    // Ghi số lượng và thành tiền
    fwrite(&soLuong, sizeof(int), 1, f);
    fwrite(&thanhTien, sizeof(double), 1, f);
}

void DichVuDat::docFile(FILE *f)
{
    if (f == nullptr)
        return;

    // Đọc mã dịch vụ (sẽ cần resolve sau)
    int len;
    fread(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        char *buffer = new char[len + 1];
        fread(buffer, sizeof(char), len, f);
        buffer[len] = '\0';
        // TODO: Resolve dichVu pointer từ mã dịch vụ
        delete[] buffer;
    }

    // Đọc số lượng và thành tiền
    fread(&soLuong, sizeof(int), 1, f);
    fread(&thanhTien, sizeof(double), 1, f);
}

// Stream I/O
void DichVuDat::ghiFile(std::ofstream &file) const
{
    if (!file.is_open())
        return;

    // Ghi mã dịch vụ
    std::string maDV = (dichVu != nullptr) ? dichVu->layMaDichVu() : "";
    FileHelper::ghiString(file, maDV);

    // Ghi số lượng và thành tiền
    file.write(reinterpret_cast<const char *>(&soLuong), sizeof(soLuong));
    file.write(reinterpret_cast<const char *>(&thanhTien), sizeof(thanhTien));
}

void DichVuDat::docFile(std::ifstream &file, QuanLyDichVu *qldv)
{
    if (!file.is_open())
        return;

    // Đọc mã dịch vụ
    std::string maDV;
    FileHelper::docString(file, maDV);

    // Resolve pointer
    if (qldv != nullptr && !maDV.empty())
    {
        dichVu = qldv->timDichVu(maDV);
    }
    else
    {
        dichVu = nullptr;
    }

    // Đọc số lượng và thành tiền
    file.read(reinterpret_cast<char *>(&soLuong), sizeof(soLuong));
    file.read(reinterpret_cast<char *>(&thanhTien), sizeof(thanhTien));
}
