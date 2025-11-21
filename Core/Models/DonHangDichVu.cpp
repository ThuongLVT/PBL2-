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
#include "../Utils/FileHelper.h"

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

// File I/O
void DonHangDichVu::ghiFile(FILE *f) const
{
    if (f == nullptr)
        return;

    // Ghi mã đơn hàng
    size_t len = maDonHang.length();
    fwrite(&len, sizeof(size_t), 1, f);
    fwrite(maDonHang.c_str(), sizeof(char), len, f);

    // Ghi mã khách hàng
    string maKH = getMaKhachHang();
    len = maKH.length();
    fwrite(&len, sizeof(size_t), 1, f);
    fwrite(maKH.c_str(), sizeof(char), len, f);

    // Ghi số lượng dịch vụ
    int soLuongDV = danhSachDichVu.size();
    fwrite(&soLuongDV, sizeof(int), 1, f);

    // Ghi từng dịch vụ
    for (int i = 0; i < soLuongDV; i++)
    {
        danhSachDichVu[i].ghiFile(f);
    }

    // Ghi các trường khác
    fwrite(&tongTien, sizeof(double), 1, f);
    fwrite(&giamGia, sizeof(double), 1, f);
    fwrite(&thanhTien, sizeof(double), 1, f);
    fwrite(&trangThai, sizeof(TrangThaiDonHang), 1, f);

    // Ghi ngày tạo
    ngayTao.ghiFile(f);

    // Ghi ghi chú
    len = ghiChu.length();
    fwrite(&len, sizeof(size_t), 1, f);
    fwrite(ghiChu.c_str(), sizeof(char), len, f);
}

void DonHangDichVu::docFile(FILE *f)
{
    if (f == nullptr)
        return;

    // Đọc mã đơn hàng
    size_t len;
    fread(&len, sizeof(size_t), 1, f);
    char *buffer = new char[len + 1];
    fread(buffer, sizeof(char), len, f);
    buffer[len] = '\0';
    maDonHang = string(buffer);
    delete[] buffer;

    // Đọc mã khách hàng (sẽ resolve pointer sau)
    fread(&len, sizeof(size_t), 1, f);
    buffer = new char[len + 1];
    fread(buffer, sizeof(char), len, f);
    buffer[len] = '\0';
    string maKH = string(buffer);
    delete[] buffer;

    // Note: khachHang pointer cần resolve sau khi load từ QuanLyKhachHang
    khachHang = nullptr; // Temporary

    // Đọc số lượng dịch vụ
    int soLuongDV;
    fread(&soLuongDV, sizeof(int), 1, f);

    // Đọc từng dịch vụ
    danhSachDichVu.clear();
    for (int i = 0; i < soLuongDV; i++)
    {
        DichVuDat dv;
        dv.docFile(f);
        danhSachDichVu.push_back(dv);
    }

    // Đọc các trường khác
    fread(&tongTien, sizeof(double), 1, f);
    fread(&giamGia, sizeof(double), 1, f);
    fread(&thanhTien, sizeof(double), 1, f);
    fread(&trangThai, sizeof(TrangThaiDonHang), 1, f);

    // Đọc ngày tạo
    ngayTao.docFile(f);

    // Đọc ghi chú
    fread(&len, sizeof(size_t), 1, f);
    buffer = new char[len + 1];
    fread(buffer, sizeof(char), len, f);
    buffer[len] = '\0';
    ghiChu = string(buffer);
    delete[] buffer;
}

// Stream I/O
void DonHangDichVu::ghiFile(std::ofstream &file) const
{
    if (!file.is_open()) return;

    // Ghi mã đơn hàng
    FileHelper::ghiString(file, maDonHang);

    // Ghi mã khách hàng
    std::string maKH = getMaKhachHang();
    FileHelper::ghiString(file, maKH);

    // Ghi số lượng dịch vụ
    int soLuongDV = danhSachDichVu.size();
    file.write(reinterpret_cast<const char *>(&soLuongDV), sizeof(soLuongDV));

    // Ghi từng dịch vụ
    for (int i = 0; i < soLuongDV; i++)
    {
        danhSachDichVu[i].ghiFile(file);
    }

    // Ghi các trường khác
    file.write(reinterpret_cast<const char *>(&tongTien), sizeof(tongTien));
    file.write(reinterpret_cast<const char *>(&giamGia), sizeof(giamGia));
    file.write(reinterpret_cast<const char *>(&thanhTien), sizeof(thanhTien));
    file.write(reinterpret_cast<const char *>(&trangThai), sizeof(trangThai));

    // Ghi ngày tạo
    ngayTao.ghiFile(file);

    // Ghi ghi chú
    FileHelper::ghiString(file, ghiChu);
}

void DonHangDichVu::docFile(std::ifstream &file, QuanLyKhachHang *qlkh, QuanLyDichVu *qldv)
{
    if (!file.is_open()) return;

    // Đọc mã đơn hàng
    FileHelper::docString(file, maDonHang);

    // Đọc mã khách hàng
    std::string maKH;
    FileHelper::docString(file, maKH);
    if (qlkh != nullptr && maKH != "GUEST")
    {
        khachHang = qlkh->timKhachHang(maKH);
    }
    else
    {
        khachHang = nullptr;
    }

    // Đọc số lượng dịch vụ
    int soLuongDV;
    file.read(reinterpret_cast<char *>(&soLuongDV), sizeof(soLuongDV));

    // Đọc từng dịch vụ
    danhSachDichVu.clear();
    for (int i = 0; i < soLuongDV; i++)
    {
        DichVuDat dv;
        dv.docFile(file, qldv);
        danhSachDichVu.push_back(dv);
    }

    // Đọc các trường khác
    file.read(reinterpret_cast<char *>(&tongTien), sizeof(tongTien));
    file.read(reinterpret_cast<char *>(&giamGia), sizeof(giamGia));
    file.read(reinterpret_cast<char *>(&thanhTien), sizeof(thanhTien));
    file.read(reinterpret_cast<char *>(&trangThai), sizeof(trangThai));

    // Đọc ngày tạo
    ngayTao.docFile(file);

    // Đọc ghi chú
    FileHelper::docString(file, ghiChu);
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
