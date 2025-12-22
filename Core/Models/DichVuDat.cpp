#include "DichVuDat.h"
#include <iostream>

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


