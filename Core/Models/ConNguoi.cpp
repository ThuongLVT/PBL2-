/**
 * @file ConNguoi.cpp
 * @brief Implementation của lớp ConNguoi
 */

#include "ConNguoi.h"
#include <string>

using namespace std;

// ========== CONSTRUCTORS ==========

ConNguoi::ConNguoi()
    : hoTen(""), soDienThoai(""), diaChi("")
{
}

ConNguoi::ConNguoi(const std::string &ht, const std::string &sdt, const std::string &dc)
    : hoTen(ht), soDienThoai(sdt), diaChi(dc)
{
}

ConNguoi::ConNguoi(const ConNguoi &other)
    : hoTen(other.hoTen), soDienThoai(other.soDienThoai), diaChi(other.diaChi)
{
}

ConNguoi::~ConNguoi()
{
    // Destructor - std::string tự dọn dẹp
}

// ========== GETTERS ==========

std::string ConNguoi::layHoTen() const
{
    return hoTen;
}

std::string ConNguoi::laySoDienThoai() const
{
    return soDienThoai;
}

std::string ConNguoi::layDiaChi() const
{
    return diaChi;
}

// ========== SETTERS ==========

void ConNguoi::datHoTen(const std::string &ht)
{
    hoTen = ht;
}

void ConNguoi::datSoDienThoai(const std::string &sdt)
{
    soDienThoai = sdt;
}

void ConNguoi::datDiaChi(const std::string &dc)
{
    diaChi = dc;
}

// ========== METHODS ==========

void ConNguoi::hienThiThongTin() const
{
    std::cout << "Ho ten: " << hoTen << std::endl;
    std::cout << "So dien thoai: " << soDienThoai << std::endl;
    std::cout << "Dia chi: " << diaChi << std::endl;
}

// ========== OPERATORS ==========

ConNguoi &ConNguoi::operator=(const ConNguoi &other)
{
    if (this != &other)
    {
        hoTen = other.hoTen;
        soDienThoai = other.soDienThoai;
        diaChi = other.diaChi;
    }
    return *this;
}

bool ConNguoi::operator==(const ConNguoi &other) const
{
    return hoTen == other.hoTen;
}

std::ostream &operator<<(std::ostream &os, const ConNguoi &cn)
{
    os << "Ho ten: " << cn.hoTen << ", SDT: " << cn.soDienThoai
       << ", Dia chi: " << cn.diaChi;
    return os;
}
