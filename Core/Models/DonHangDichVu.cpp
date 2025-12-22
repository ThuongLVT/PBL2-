/**
 * @file DonHangDichVu.cpp
 * @brief Implementation của lớp DonHangDichVu
 */

#include "DonHangDichVu.h"
#include "../QuanLy/QuanLyKhachHang.h"
#include "../QuanLy/QuanLyDichVu.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

// ========== CONSTRUCTORS ==========

DonHangDichVu::DonHangDichVu()
    : maDonHang(""), khachHang(nullptr), tongTien(0.0), giamGia(0.0),
      thanhTien(0.0), trangThai(TrangThaiDonHang::CHO_XU_LY), ghiChu("")
{
    ngayTao = NgayGio::layThoiGianHienTai();
}

DonHangDichVu::DonHangDichVu(const std::string &ma, KhachHang *kh)
    : maDonHang(ma), khachHang(kh), tongTien(0.0), giamGia(0.0),
      thanhTien(0.0), trangThai(TrangThaiDonHang::CHO_XU_LY), ghiChu("")
{
    ngayTao = NgayGio::layThoiGianHienTai();
}

DonHangDichVu::DonHangDichVu(const DonHangDichVu &other)
    : maDonHang(other.maDonHang), khachHang(other.khachHang),
      danhSachDichVu(other.danhSachDichVu), tongTien(other.tongTien),
      giamGia(other.giamGia), thanhTien(other.thanhTien),
      trangThai(other.trangThai), ngayTao(other.ngayTao),
      ghiChu(other.ghiChu)
{
}

DonHangDichVu::~DonHangDichVu()
{
    // Không delete khachHang vì đó là pointer quản lý bởi QuanLyKhachHang
}

// ========== OPERATORS ==========

DonHangDichVu &DonHangDichVu::operator=(const DonHangDichVu &other)
{
    if (this != &other)
    {
        maDonHang = other.maDonHang;
        khachHang = other.khachHang;
        danhSachDichVu = other.danhSachDichVu;
        tongTien = other.tongTien;
        giamGia = other.giamGia;
        thanhTien = other.thanhTien;
        trangThai = other.trangThai;
        ngayTao = other.ngayTao;
        ghiChu = other.ghiChu;
    }
    return *this;
}

bool DonHangDichVu::operator==(const DonHangDichVu &other) const
{
    return maDonHang == other.maDonHang;
}

// ========== GETTERS ==========

std::string DonHangDichVu::getMaDonHang() const { return maDonHang; }
KhachHang *DonHangDichVu::getKhachHang() const { return khachHang; }
const MangDong<DichVuDat> &DonHangDichVu::getDanhSachDichVu() const { return danhSachDichVu; }
double DonHangDichVu::getTongTien() const { return tongTien; }
double DonHangDichVu::getGiamGia() const { return giamGia; }
double DonHangDichVu::getThanhTien() const { return thanhTien; }
TrangThaiDonHang DonHangDichVu::getTrangThai() const { return trangThai; }
NgayGio DonHangDichVu::getNgayTao() const { return ngayTao; }
std::string DonHangDichVu::getGhiChu() const { return ghiChu; }

// ========== SETTERS ==========

void DonHangDichVu::setMaDonHang(const std::string &ma) { maDonHang = ma; }
void DonHangDichVu::setKhachHang(KhachHang *kh) { khachHang = kh; }
void DonHangDichVu::setTrangThai(TrangThaiDonHang tt) { trangThai = tt; }
void DonHangDichVu::setGhiChu(const std::string &gc) { ghiChu = gc; }
void DonHangDichVu::setNgayTao(const NgayGio &nt) { ngayTao = nt; }

// ========== METHODS ==========

void DonHangDichVu::themDichVu(const DichVuDat &dv)
{
    danhSachDichVu.push_back(dv);
    tinhTongTien();
    tinhGiamGia();
    tinhThanhTien();
}

void DonHangDichVu::xoaDichVu(int index)
{
    if (index >= 0 && index < danhSachDichVu.size())
    {
        danhSachDichVu.erase(index);
        tinhTongTien();
        tinhGiamGia();
        tinhThanhTien();
    }
}

void DonHangDichVu::tinhTongTien()
{
    tongTien = 0;
    for (int i = 0; i < danhSachDichVu.size(); i++)
    {
        tongTien += danhSachDichVu[i].getThanhTien();
    }
}

void DonHangDichVu::tinhGiamGia()
{
    giamGia = 0;
    
    // Giảm giá theo membership của khách hàng
    if (khachHang != nullptr)
    {
        // TODO: Implement membership discount logic
        // Ví dụ: VIP = 10%, Gold = 5%, Silver = 2%
        // giamGia = tongTien * (membership_rate / 100.0);
        
        // Tạm thời: không giảm giá
        giamGia = 0;
    }
}

void DonHangDichVu::tinhThanhTien()
{
    thanhTien = tongTien - giamGia;
    if (thanhTien < 0)
    {
        thanhTien = 0;
    }
}

std::string DonHangDichVu::getTrangThaiText() const
{
    switch (trangThai)
    {
    case TrangThaiDonHang::CHO_XU_LY:
        return "Cho xu ly";
    case TrangThaiDonHang::DANG_CHUAN_BI:
        return "Dang chuan bi";
    case TrangThaiDonHang::HOAN_THANH:
        return "Hoan thanh";
    case TrangThaiDonHang::DA_HUY:
        return "Da huy";
    default:
        return "Khong xac dinh";
    }
}

void DonHangDichVu::hienThi() const
{
    cout << "Ma don hang: " << maDonHang << endl;
    cout << "Khach hang: " << (khachHang ? khachHang->layHoTen() : "GUEST") << endl;
    cout << "Ngay tao: " << ngayTao.toString() << endl;
    cout << "Trang thai: " << getTrangThaiText() << endl;
    cout << "So luong dich vu: " << danhSachDichVu.size() << endl;
    
    cout << "\nDanh sach dich vu:" << endl;
    for (int i = 0; i < danhSachDichVu.size(); i++)
    {
        cout << "  [" << (i + 1) << "] ";
        danhSachDichVu[i].hienThi();
    }
    
    cout << "\nTong tien: " << tongTien << " VND" << endl;
    cout << "Giam gia: " << giamGia << " VND" << endl;
    cout << "Thanh tien: " << thanhTien << " VND" << endl;
    
    if (!ghiChu.empty())
    {
        cout << "Ghi chu: " << ghiChu << endl;
    }
}

// ========== HELPER ==========

std::string DonHangDichVu::getMaKhachHang() const
{
    return (khachHang != nullptr) ? khachHang->layMaKhachHang() : "GUEST";
}

// ========== STATIC ==========

std::string DonHangDichVu::taoMaDonHang()
{
    // Tạo mã dạng: DH + timestamp
    time_t now = time(nullptr);
    ostringstream oss;
    oss << "DH" << now;
    return oss.str();
}
