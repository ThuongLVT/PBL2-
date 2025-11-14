/**
 * @file ThietBi.cpp
 * @brief Triển khai các phương thức của lớp ThietBi
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#include "ThietBi.h"
#include <iostream>
#include <iomanip>

using namespace std;

// ========== CONSTRUCTORS ==========

ThietBi::ThietBi()
    : DichVu("", "", 0.0, LoaiDichVu::THIET_BI),
      loaiThietBi(LoaiThietBi::KHAC),
      size(""),
      tinhTrang(TinhTrangThietBi::TOT),
      soLuongTon(0),
      hangSanXuat("")
{
}

ThietBi::ThietBi(const string &maDV, const string &tenDV,
                 double gia, LoaiThietBi loai, const string &size,
                 TinhTrangThietBi tinhTrang, int soLuong,
                 const string &hang)
    : DichVu(maDV, tenDV, gia, LoaiDichVu::THIET_BI),
      loaiThietBi(loai),
      size(size),
      tinhTrang(tinhTrang),
      soLuongTon(soLuong),
      hangSanXuat(hang)
{
    // Cập nhật trạng thái còn hàng
    conHang = (soLuongTon > 0);
}

ThietBi::ThietBi(const ThietBi &other)
    : DichVu(other),
      loaiThietBi(other.loaiThietBi),
      size(other.size),
      tinhTrang(other.tinhTrang),
      soLuongTon(other.soLuongTon),
      hangSanXuat(other.hangSanXuat)
{
}

ThietBi::~ThietBi()
{
    // Destructor
}

// ========== GETTERS ==========

LoaiThietBi ThietBi::layLoaiThietBi() const
{
    return loaiThietBi;
}

string ThietBi::laySize() const
{
    return size;
}

TinhTrangThietBi ThietBi::layTinhTrang() const
{
    return tinhTrang;
}

int ThietBi::laySoLuongTon() const
{
    return soLuongTon;
}

string ThietBi::layHangSanXuat() const
{
    return hangSanXuat;
}

string ThietBi::layTenLoaiThietBi() const
{
    switch (loaiThietBi)
    {
    case LoaiThietBi::BONG:
        return "Bong da";
    case LoaiThietBi::GIAY:
        return "Giay da bong";
    case LoaiThietBi::AO:
        return "Ao thi dau";
    case LoaiThietBi::QUAN:
        return "Quan thi dau";
    case LoaiThietBi::BAO_TAY:
        return "Bao tay";
    case LoaiThietBi::BANG_SAU:
        return "Bang sau";
    case LoaiThietBi::GANG_TAY:
        return "Gang tay thu mon";
    case LoaiThietBi::KHAC:
        return "Khac";
    default:
        return "Khong xac dinh";
    }
}

string ThietBi::layTenTinhTrang() const
{
    switch (tinhTrang)
    {
    case TinhTrangThietBi::MOI:
        return "Moi";
    case TinhTrangThietBi::TOT:
        return "Tot";
    case TinhTrangThietBi::TRUNG_BINH:
        return "Trung binh";
    case TinhTrangThietBi::KEM:
        return "Kem";
    case TinhTrangThietBi::HONG:
        return "Hong";
    default:
        return "Khong xac dinh";
    }
}

// ========== SETTERS ==========

void ThietBi::datLoaiThietBi(LoaiThietBi loai)
{
    loaiThietBi = loai;
}

void ThietBi::datSize(const string &sz)
{
    size = sz;
}

void ThietBi::datTinhTrang(TinhTrangThietBi tt)
{
    tinhTrang = tt;
}

void ThietBi::datSoLuongTon(int sl)
{
    if (sl >= 0)
    {
        soLuongTon = sl;
        conHang = (soLuongTon > 0);
    }
}

void ThietBi::datHangSanXuat(const string &hang)
{
    hangSanXuat = hang;
}

// ========== METHODS ==========

void ThietBi::nhapKho(int soLuong)
{
    if (soLuong > 0)
    {
        soLuongTon += soLuong;
        conHang = true;
    }
}

bool ThietBi::xuatKho(int soLuong)
{
    if (soLuong <= 0 || soLuong > soLuongTon)
    {
        return false;
    }

    soLuongTon -= soLuong;
    conHang = (soLuongTon > 0);
    return true;
}

bool ThietBi::kiemTraDuHang(int soLuong) const
{
    return soLuong > 0 && soLuong <= soLuongTon;
}

double ThietBi::tinhTien(int soLuong) const
{
    if (soLuong <= 0)
    {
        return 0.0;
    }

    double tongTien = donGia * soLuong;

    // Giảm giá theo tình trạng
    switch (tinhTrang)
    {
    case TinhTrangThietBi::MOI:
        // Không giảm
        break;
    case TinhTrangThietBi::TOT:
        tongTien *= 0.9; // Giảm 10%
        break;
    case TinhTrangThietBi::TRUNG_BINH:
        tongTien *= 0.7; // Giảm 30%
        break;
    case TinhTrangThietBi::KEM:
        tongTien *= 0.5; // Giảm 50%
        break;
    case TinhTrangThietBi::HONG:
        tongTien = 0; // Không cho thuê
        break;
    }

    return tongTien;
}

void ThietBi::hienThiThongTin() const
{
    cout << "\n===== THONG TIN THIET BI =====" << endl;
    cout << "Ma dich vu: " << maDichVu << endl;
    cout << "Ten thiet bi: " << tenDichVu << endl;
    cout << "Loai: " << layTenLoaiThietBi() << endl;
    cout << "Size: " << size << endl;
    cout << "Tinh trang: " << layTenTinhTrang() << endl;
    if (!hangSanXuat.empty())
    {
        cout << "Hang san xuat: " << hangSanXuat << endl;
    }
    cout << "Don gia: " << fixed << setprecision(0) << donGia << " VND/lan" << endl;
    cout << "So luong ton: " << soLuongTon << endl;
    cout << "Trang thai: " << (conHang ? "Con hang" : "Het hang") << endl;
    if (!moTa.empty())
    {
        cout << "Mo ta: " << moTa << endl;
    }
    cout << "==============================" << endl;
}

bool ThietBi::ghiFile(ofstream &file) const
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

    // Ghi loại thiết bị
    int loai = static_cast<int>(loaiThietBi);
    file.write(reinterpret_cast<const char *>(&loai), sizeof(loai));

    // Ghi size
    size_t sizeLen = size.length();
    file.write(reinterpret_cast<const char *>(&sizeLen), sizeof(sizeLen));
    file.write(size.c_str(), sizeLen);

    // Ghi tình trạng
    int tt = static_cast<int>(tinhTrang);
    file.write(reinterpret_cast<const char *>(&tt), sizeof(tt));

    // Ghi số lượng tồn
    file.write(reinterpret_cast<const char *>(&soLuongTon), sizeof(soLuongTon));

    // Ghi hãng sản xuất
    size_t hangLen = hangSanXuat.length();
    file.write(reinterpret_cast<const char *>(&hangLen), sizeof(hangLen));
    file.write(hangSanXuat.c_str(), hangLen);

    return file.good();
}

bool ThietBi::docFile(ifstream &file)
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

    // Đọc loại thiết bị
    int loai;
    file.read(reinterpret_cast<char *>(&loai), sizeof(loai));
    loaiThietBi = static_cast<LoaiThietBi>(loai);

    // Đọc size
    size_t sizeLen;
    file.read(reinterpret_cast<char *>(&sizeLen), sizeof(sizeLen));
    size.resize(sizeLen);
    file.read(&size[0], sizeLen);

    // Đọc tình trạng
    int tt;
    file.read(reinterpret_cast<char *>(&tt), sizeof(tt));
    tinhTrang = static_cast<TinhTrangThietBi>(tt);

    // Đọc số lượng tồn
    file.read(reinterpret_cast<char *>(&soLuongTon), sizeof(soLuongTon));

    // Đọc hãng sản xuất
    size_t hangLen;
    file.read(reinterpret_cast<char *>(&hangLen), sizeof(hangLen));
    hangSanXuat.resize(hangLen);
    file.read(&hangSanXuat[0], hangLen);

    // Cập nhật trạng thái còn hàng
    conHang = (soLuongTon > 0);

    return file.good();
}

// ========== OPERATORS ==========

ThietBi &ThietBi::operator=(const ThietBi &other)
{
    if (this != &other)
    {
        DichVu::operator=(other);
        loaiThietBi = other.loaiThietBi;
        size = other.size;
        tinhTrang = other.tinhTrang;
        soLuongTon = other.soLuongTon;
        hangSanXuat = other.hangSanXuat;
    }
    return *this;
}

ostream &operator<<(ostream &os, const ThietBi &tb)
{
    os << "Thiet bi: " << tb.tenDichVu
       << " | Loai: " << tb.layTenLoaiThietBi()
       << " | Size: " << tb.size
       << " | Tinh trang: " << tb.layTenTinhTrang()
       << " | Gia: " << tb.donGia << " VND"
       << " | Ton: " << tb.soLuongTon;
    return os;
}
