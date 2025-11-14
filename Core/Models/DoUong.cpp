/**
 * @file DoUong.cpp
 * @brief Triển khai các phương thức của lớp DoUong
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#include "DoUong.h"
#include <iostream>
#include <iomanip>

using namespace std;

// ========== CONSTRUCTORS ==========

DoUong::DoUong()
    : DichVu("", "", 0.0, LoaiDichVu::DO_UONG),
      loaiDoUong(LoaiDoUong::KHAC),
      dungTich(0),
      coDa(false),
      soLuongTon(0)
{
}

DoUong::DoUong(const string &maDV, const string &tenDV,
               double gia, LoaiDoUong loai, int dungTich,
               bool coDa, int soLuong)
    : DichVu(maDV, tenDV, gia, LoaiDichVu::DO_UONG),
      loaiDoUong(loai),
      dungTich(dungTich),
      coDa(coDa),
      soLuongTon(soLuong)
{
    // Cập nhật trạng thái còn hàng
    conHang = (soLuongTon > 0);
}

DoUong::DoUong(const DoUong &other)
    : DichVu(other),
      loaiDoUong(other.loaiDoUong),
      dungTich(other.dungTich),
      coDa(other.coDa),
      soLuongTon(other.soLuongTon)
{
}

DoUong::~DoUong()
{
    // Destructor
}

// ========== GETTERS ==========

LoaiDoUong DoUong::layLoaiDoUong() const
{
    return loaiDoUong;
}

int DoUong::layDungTich() const
{
    return dungTich;
}

bool DoUong::laCoDa() const
{
    return coDa;
}

int DoUong::laySoLuongTon() const
{
    return soLuongTon;
}

string DoUong::layTenLoaiDoUong() const
{
    switch (loaiDoUong)
    {
    case LoaiDoUong::NUOC_NGOT:
        return "Nuoc ngot";
    case LoaiDoUong::BIA:
        return "Bia";
    case LoaiDoUong::NUOC_SUOI:
        return "Nuoc suoi";
    case LoaiDoUong::NUOC_TANG_LUC:
        return "Nuoc tang luc";
    case LoaiDoUong::TRA_SUA:
        return "Tra sua";
    case LoaiDoUong::CA_PHE:
        return "Ca phe";
    case LoaiDoUong::NUOC_EP:
        return "Nuoc ep";
    case LoaiDoUong::KHAC:
        return "Khac";
    default:
        return "Khong xac dinh";
    }
}

// ========== SETTERS ==========

void DoUong::datLoaiDoUong(LoaiDoUong loai)
{
    loaiDoUong = loai;
}

void DoUong::datDungTich(int dt)
{
    if (dt > 0)
    {
        dungTich = dt;
    }
}

void DoUong::datCoDa(bool cd)
{
    coDa = cd;
}

void DoUong::datSoLuongTon(int sl)
{
    if (sl >= 0)
    {
        soLuongTon = sl;
        conHang = (soLuongTon > 0);
    }
}

// ========== METHODS ==========

void DoUong::nhapKho(int soLuong)
{
    if (soLuong > 0)
    {
        soLuongTon += soLuong;
        conHang = true;
    }
}

bool DoUong::xuatKho(int soLuong)
{
    if (soLuong <= 0 || soLuong > soLuongTon)
    {
        return false;
    }

    soLuongTon -= soLuong;
    conHang = (soLuongTon > 0);
    return true;
}

bool DoUong::kiemTraDuHang(int soLuong) const
{
    return soLuong > 0 && soLuong <= soLuongTon;
}

double DoUong::tinhTien(int soLuong) const
{
    if (soLuong <= 0)
    {
        return 0.0;
    }
    return donGia * soLuong;
}

void DoUong::hienThiThongTin() const
{
    cout << "\n===== THONG TIN DO UONG =====" << endl;
    cout << "Ma dich vu: " << maDichVu << endl;
    cout << "Ten do uong: " << tenDichVu << endl;
    cout << "Loai: " << layTenLoaiDoUong() << endl;
    cout << "Dung tich: " << dungTich << " ml" << endl;
    cout << "Co da: " << (coDa ? "Co" : "Khong") << endl;
    cout << "Don gia: " << fixed << setprecision(0) << donGia << " VND" << endl;
    cout << "So luong ton: " << soLuongTon << endl;
    cout << "Trang thai: " << (conHang ? "Con hang" : "Het hang") << endl;
    if (!moTa.empty())
    {
        cout << "Mo ta: " << moTa << endl;
    }
    cout << "==============================" << endl;
}

bool DoUong::ghiFile(ofstream &file) const
{
    if (!file.is_open())
    {
        return false;
    }

    // Ghi base class
    if (!DichVu::ghiFile(file))
    {
        return false;
    }

    // Ghi loại đồ uống
    int loai = static_cast<int>(loaiDoUong);
    file.write(reinterpret_cast<const char *>(&loai), sizeof(loai));

    // Ghi dung tích
    file.write(reinterpret_cast<const char *>(&dungTich), sizeof(dungTich));

    // Ghi có đá
    file.write(reinterpret_cast<const char *>(&coDa), sizeof(coDa));

    // Ghi số lượng tồn
    file.write(reinterpret_cast<const char *>(&soLuongTon), sizeof(soLuongTon));

    return file.good();
}

bool DoUong::docFile(ifstream &file)
{
    if (!file.is_open())
    {
        return false;
    }

    // Đọc base class
    if (!DichVu::docFile(file))
    {
        return false;
    }

    // Đọc loại đồ uống
    int loai;
    file.read(reinterpret_cast<char *>(&loai), sizeof(loai));
    loaiDoUong = static_cast<LoaiDoUong>(loai);

    // Đọc dung tích
    file.read(reinterpret_cast<char *>(&dungTich), sizeof(dungTich));

    // Đọc có đá
    file.read(reinterpret_cast<char *>(&coDa), sizeof(coDa));

    // Đọc số lượng tồn
    file.read(reinterpret_cast<char *>(&soLuongTon), sizeof(soLuongTon));

    // Cập nhật trạng thái còn hàng
    conHang = (soLuongTon > 0);

    return file.good();
}

// ========== OPERATORS ==========

DoUong &DoUong::operator=(const DoUong &other)
{
    if (this != &other)
    {
        DichVu::operator=(other);
        loaiDoUong = other.loaiDoUong;
        dungTich = other.dungTich;
        coDa = other.coDa;
        soLuongTon = other.soLuongTon;
    }
    return *this;
}

ostream &operator<<(ostream &os, const DoUong &du)
{
    os << "Do uong: " << du.tenDichVu
       << " | Loai: " << du.layTenLoaiDoUong()
       << " | " << du.dungTich << "ml"
       << " | Gia: " << du.donGia << " VND"
       << " | Ton: " << du.soLuongTon;
    return os;
}
