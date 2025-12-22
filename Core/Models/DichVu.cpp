/**
 * @file DichVu.cpp
 * @brief Implementation của lớp DichVu
 */

#include "DichVu.h"

using namespace std;

// ========== CONSTRUCTORS ==========

DichVu::DichVu()
    : maDichVu(""), tenDichVu(""), donGia(0.0),
      loaiDichVu(LoaiDichVu::DO_UONG), moTa(""), conHang(true),
      donVi(""), soLuongBan(0), soLuongTon(50), hinhAnh("")
{
}

DichVu::DichVu(const std::string &maDV, const std::string &tenDV,
               double gia, LoaiDichVu loai)
    : maDichVu(maDV), tenDichVu(tenDV), donGia(gia),
      loaiDichVu(loai), moTa(""), conHang(true),
      donVi(""), soLuongBan(0), soLuongTon(50), hinhAnh("")
{
}

DichVu::DichVu(const DichVu &other)
    : maDichVu(other.maDichVu), tenDichVu(other.tenDichVu),
      donGia(other.donGia), loaiDichVu(other.loaiDichVu),
      moTa(other.moTa), conHang(other.conHang),
      donVi(other.donVi), soLuongBan(other.soLuongBan),
      soLuongTon(other.soLuongTon),
      hinhAnh(other.hinhAnh)
{
}

DichVu::~DichVu()
{
    // Destructor
}

// ========== GETTERS ==========

std::string DichVu::layMaDichVu() const
{
    return maDichVu;
}

std::string DichVu::layTenDichVu() const
{
    return tenDichVu;
}

double DichVu::layDonGia() const
{
    return donGia;
}

LoaiDichVu DichVu::layLoaiDichVu() const
{
    return loaiDichVu;
}

std::string DichVu::layMoTa() const
{
    return moTa;
}

bool DichVu::coConHang() const
{
    return conHang;
}

std::string DichVu::layTenLoaiDichVu() const
{
    switch (loaiDichVu)
    {
    case LoaiDichVu::DO_UONG:
        return std::string("Do uong");
    case LoaiDichVu::DO_AN:
        return std::string("Do an");
    case LoaiDichVu::THIET_BI:
        return std::string("Thiet bi");
    default:
        return std::string("Khong xac dinh");
    }
}

std::string DichVu::layDonVi() const
{
    return donVi;
}

int DichVu::laySoLuongBan() const
{
    return soLuongBan;
}

int DichVu::laySoLuongTon() const
{
    return soLuongTon;
}

std::string DichVu::layHinhAnh() const
{
    return hinhAnh;
}

// ========== SETTERS ==========

void DichVu::datTenDichVu(const std::string &ten)
{
    tenDichVu = ten;
}

void DichVu::datDonGia(double gia)
{
    if (gia >= 0)
    {
        donGia = gia;
    }
}

void DichVu::datMoTa(const std::string &mt)
{
    moTa = mt;
}

void DichVu::datConHang(bool ch)
{
    conHang = ch;
}

void DichVu::datDonVi(const std::string &dv)
{
    donVi = dv;
}

void DichVu::datSoLuongBan(int sl)
{
    soLuongBan = sl;
}

void DichVu::datSoLuongTon(int sl)
{
    soLuongTon = sl;
}

void DichVu::datHinhAnh(const std::string &ha)
{
    hinhAnh = ha;
}

// ========== METHODS ==========

double DichVu::tinhTien(int soLuong) const
{
    if (soLuong <= 0)
        return 0.0;
    return donGia * soLuong;
}

void DichVu::hienThiThongTin() const
{
    std::cout << "===== DICH VU =====" << std::endl;
    std::cout << "Ma dich vu: " << maDichVu << std::endl;
    std::cout << "Ten dich vu: " << tenDichVu << std::endl;
    std::cout << "Loai: " << layTenLoaiDichVu() << std::endl;
    std::cout << "Don gia: " << donGia << " VND" << std::endl;
    std::cout << "Trang thai: " << (conHang ? "Con hang" : "Het hang") << std::endl;
    if (moTa.length() > 0)
    {
        std::cout << "Mo ta: " << moTa << std::endl;
    }
}

// ========== OPERATORS ==========

DichVu &DichVu::operator=(const DichVu &other)
{
    if (this != &other)
    {
        maDichVu = other.maDichVu;
        tenDichVu = other.tenDichVu;
        donGia = other.donGia;
        loaiDichVu = other.loaiDichVu;
        moTa = other.moTa;
        conHang = other.conHang;
    }
    return *this;
}

bool DichVu::operator==(const DichVu &other) const
{
    return maDichVu == other.maDichVu;
}

bool DichVu::operator<(const DichVu &other) const
{
    return donGia < other.donGia;
}

std::ostream &operator<<(std::ostream &os, const DichVu &dv)
{
    os << dv.maDichVu << ": " << dv.tenDichVu
       << " (" << dv.layTenLoaiDichVu() << ") - "
       << dv.donGia << " VND";
    return os;
}
