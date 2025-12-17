#include "DatSan.h"
#include <iostream>
#include <ctime>
#include <cstdio>

#include "../Utils/FileHelper.h"
using namespace std;

// Constructor mặc định
DatSan::DatSan()
    : maDatSan(""), khachHang(nullptr), san(nullptr),
      tongTien(0.0), trangThai(DA_DAT), ghiChu(""),
      tienCoc(0.0), trangThaiCoc(DA_COC), lyDoHuy(""), hoanCoc(false) {}

// Constructor với tham số
DatSan::DatSan(const std::string &ma, KhachHang *kh, San *s, const NgayGio &tgDat, const KhungGio &kg)
    : maDatSan(ma), khachHang(kh), san(s), thoiGianDat(tgDat), khungGio(kg),
      tongTien(0.0), trangThai(DA_DAT), ghiChu(""),
      tienCoc(0.0), trangThaiCoc(DA_COC), lyDoHuy(""), hoanCoc(false)
{
    ngayTao = NgayGio::layThoiGianHienTai();
    ngayDatCoc = NgayGio::layThoiGianHienTai();
    tinhTongTien();
    tinhTienCoc(); // Auto cọc 30% khi tạo
}

// Copy constructor
DatSan::DatSan(const DatSan &other)
{
    maDatSan = other.maDatSan;
    khachHang = other.khachHang;
    san = other.san;
    thoiGianDat = other.thoiGianDat;
    khungGio = other.khungGio;
    danhSachDichVu = other.danhSachDichVu;
    tongTien = other.tongTien;
    trangThai = other.trangThai;
    ngayTao = other.ngayTao;
    ghiChu = other.ghiChu;
    tienCoc = other.tienCoc;
    trangThaiCoc = other.trangThaiCoc;
    ngayDatCoc = other.ngayDatCoc;
    ngayThanhToan = other.ngayThanhToan;
    lyDoHuy = other.lyDoHuy;
    hoanCoc = other.hoanCoc;
}

// Destructor
DatSan::~DatSan()
{
    // Không delete khachHang và san vì chúng được quản lý bởi HeThongQuanLy
}

// Assignment operator
DatSan &DatSan::operator=(const DatSan &other)
{
    if (this != &other)
    {
        maDatSan = other.maDatSan;
        khachHang = other.khachHang;
        san = other.san;
        thoiGianDat = other.thoiGianDat;
        khungGio = other.khungGio;
        danhSachDichVu = other.danhSachDichVu;
        tongTien = other.tongTien;
        trangThai = other.trangThai;
        ngayTao = other.ngayTao;
        ghiChu = other.ghiChu;
        tienCoc = other.tienCoc;
        trangThaiCoc = other.trangThaiCoc;
        ngayDatCoc = other.ngayDatCoc;
        ngayThanhToan = other.ngayThanhToan;
        lyDoHuy = other.lyDoHuy;
        hoanCoc = other.hoanCoc;
    }
    return *this;
}

// Comparison operator
bool DatSan::operator==(const DatSan &other) const
{
    return maDatSan == other.maDatSan;
}

// Getters
std::string DatSan::getMaDatSan() const { return maDatSan; }
KhachHang *DatSan::getKhachHang() const { return khachHang; }
San *DatSan::getSan() const { return san; }
NgayGio DatSan::getThoiGianDat() const { return thoiGianDat; }
KhungGio DatSan::getKhungGio() const { return khungGio; }
const MangDong<DichVuDat> &DatSan::getDanhSachDichVu() const { return danhSachDichVu; }
double DatSan::getTongTien() const { return tongTien; }
TrangThaiDatSan DatSan::getTrangThai() const { return trangThai; }
NgayGio DatSan::getNgayTao() const { return ngayTao; }
std::string DatSan::getGhiChu() const { return ghiChu; }
double DatSan::getTienCoc() const { return tienCoc; }
TrangThaiCoc DatSan::getTrangThaiCoc() const { return trangThaiCoc; }
NgayGio DatSan::getNgayDatCoc() const { return ngayDatCoc; }
NgayGio DatSan::getNgayThanhToan() const { return ngayThanhToan; }
std::string DatSan::getLyDoHuy() const { return lyDoHuy; }
bool DatSan::isHoanCoc() const { return hoanCoc; }

// Setters
void DatSan::setMaDatSan(const std::string &ma) { maDatSan = ma; }
void DatSan::setKhachHang(KhachHang *kh) { khachHang = kh; }
void DatSan::setSan(San *s)
{
    san = s;
    tinhTongTien();
}
void DatSan::setThoiGianDat(const NgayGio &tgDat) { thoiGianDat = tgDat; }
void DatSan::setKhungGio(const KhungGio &kg)
{
    khungGio = kg;
    tinhTongTien();
}
void DatSan::setTrangThai(TrangThaiDatSan tt) { trangThai = tt; }
void DatSan::setGhiChu(const std::string &gc) { ghiChu = gc; }
void DatSan::setTienCoc(double tc) { tienCoc = tc; }
void DatSan::setTrangThaiCoc(TrangThaiCoc ttc) { trangThaiCoc = ttc; }
void DatSan::setNgayDatCoc(const NgayGio &ndc) { ngayDatCoc = ndc; }
void DatSan::setNgayThanhToan(const NgayGio &ntt) { ngayThanhToan = ntt; }
void DatSan::setLyDoHuy(const std::string &lyDo) { lyDoHuy = lyDo; }
void DatSan::setHoanCoc(bool hoan) { hoanCoc = hoan; }

// Methods
void DatSan::themDichVu(const DichVuDat &dv)
{
    // Validate input
    if (dv.getDichVu() == nullptr || dv.getSoLuong() <= 0)
        return;

    // Check if service already exists
    for (int i = 0; i < danhSachDichVu.size(); i++)
    {
        DichVu *existingDv = danhSachDichVu[i].getDichVu();
        if (existingDv != nullptr && existingDv == dv.getDichVu())
        {
            // Update quantity
            int newQty = danhSachDichVu[i].getSoLuong() + dv.getSoLuong();
            danhSachDichVu[i].setSoLuong(newQty);
            tinhTongTien();
            return;
        }
    }

    // If not found, add new
    danhSachDichVu.push_back(dv);
    tinhTongTien();
}

void DatSan::xoaDichVu(int index)
{
    if (index >= 0 && index < danhSachDichVu.size())
    {
        danhSachDichVu.erase(index);
        tinhTongTien();
    }
}

void DatSan::tinhTongTien()
{
    tongTien = 0.0;

    // Tính tiền sân
    if (san != nullptr)
    {
        double soGio = khungGio.tinhSoGio();
        tongTien += san->getGiaThue() * soGio;
    }

    // Cộng tiền dịch vụ
    for (int i = 0; i < danhSachDichVu.size(); i++)
    {
        tongTien += danhSachDichVu[i].getThanhTien();
    }
}

std::string DatSan::getTrangThaiText() const
{
    switch (trangThai)
    {
    case DA_DAT:
        return std::string("Đã đặt");
    case HOAN_THANH:
        return std::string("Hoàn thành");
    case DA_HUY:
        return std::string("Đã hủy");
    default:
        return std::string("Không xác định");
    }
}

void DatSan::hienThi() const
{
    std::cout << "\n===== DON DAT SAN =====" << std::endl;
    std::cout << "Ma dat san: " << maDatSan.c_str() << std::endl;

    if (khachHang)
    {
        std::cout << "Khach hang: " << khachHang->getHoTen().c_str()
                  << " (SDT: " << khachHang->getSoDienThoai().c_str() << ")" << std::endl;
    }

    if (san)
    {
        std::cout << "San: " << san->getTenSan().c_str()
                  << " (Ma: " << san->getMaSan().c_str() << ")" << std::endl;
    }

    std::cout << "Thoi gian dat: " << thoiGianDat.toString().c_str() << std::endl;
    std::cout << "Khung gio: " << khungGio.getGioBatDau().toString().c_str()
              << " - " << khungGio.getGioKetThuc().toString().c_str() << std::endl;

    std::cout << "Dich vu kem theo:" << std::endl;
    if (danhSachDichVu.size() == 0)
    {
        std::cout << "  Khong co dich vu" << std::endl;
    }
    else
    {
        for (int i = 0; i < danhSachDichVu.size(); i++)
        {
            danhSachDichVu[i].hienThi();
        }
    }

    std::cout << "Tong tien: " << tongTien << " VND" << std::endl;
    std::cout << "Trang thai: " << getTrangThaiText().c_str() << std::endl;
    std::cout << "Ngay tao: " << ngayTao.toString().c_str() << std::endl;

    if (ghiChu.length() > 0)
    {
        std::cout << "Ghi chu: " << ghiChu.c_str() << std::endl;
    }
}

// File I/O
void DatSan::ghiFile(FILE *f) const
{
    if (f == nullptr)
        return;

    // Ghi mã đặt sân
    int len = maDatSan.length();
    fwrite(&len, sizeof(int), 1, f);
    fwrite(maDatSan.c_str(), sizeof(char), len, f);

    // Ghi mã khách hàng (để tham chiếu)
    if (khachHang != nullptr)
    {
        len = khachHang->getMaNguoiDung().length();
        fwrite(&len, sizeof(int), 1, f);
        fwrite(khachHang->getMaNguoiDung().c_str(), sizeof(char), len, f);
    }
    else
    {
        len = 0;
        fwrite(&len, sizeof(int), 1, f);
    }

    // Ghi mã sân
    if (san != nullptr)
    {
        len = san->getMaSan().length();
        fwrite(&len, sizeof(int), 1, f);
        fwrite(san->getMaSan().c_str(), sizeof(char), len, f);
    }
    else
    {
        len = 0;
        fwrite(&len, sizeof(int), 1, f);
    }

    // Ghi thời gian và khung giờ
    thoiGianDat.ghiFile(f);
    khungGio.ghiFile(f);

    // Ghi danh sách dịch vụ
    int soDichVu = danhSachDichVu.size();
    fwrite(&soDichVu, sizeof(int), 1, f);
    for (int i = 0; i < soDichVu; i++)
    {
        danhSachDichVu[i].ghiFile(f);
    }

    // Ghi tổng tiền, trạng thái
    fwrite(&tongTien, sizeof(double), 1, f);
    int tt = static_cast<int>(trangThai);
    fwrite(&tt, sizeof(int), 1, f);

    // Ghi ngày tạo
    ngayTao.ghiFile(f);

    // Ghi ghi chú
    len = ghiChu.length();
    fwrite(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        fwrite(ghiChu.c_str(), sizeof(char), len, f);
    }

    // Ghi deposit fields
    fwrite(&tienCoc, sizeof(double), 1, f);
    int ttc = static_cast<int>(trangThaiCoc);
    fwrite(&ttc, sizeof(int), 1, f);
    ngayDatCoc.ghiFile(f);

    // Ghi cancel fields
    len = lyDoHuy.length();
    fwrite(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        fwrite(lyDoHuy.c_str(), sizeof(char), len, f);
    }
    fwrite(&hoanCoc, sizeof(bool), 1, f);
}

void DatSan::docFile(FILE *f)
{
    if (f == nullptr)
        return;

    // Đọc mã đặt sân
    int len;
    fread(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        char *buffer = new char[len + 1];
        fread(buffer, sizeof(char), len, f);
        buffer[len] = '\0';
        maDatSan = std::string(buffer);
        delete[] buffer;
    }

    // Đọc mã khách hàng - set nullptr, QuanLyDatSan sẽ resolve sau
    fread(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        char *buffer = new char[len + 1];
        fread(buffer, sizeof(char), len, f);
        buffer[len] = '\0';
        // Lưu ID vào biến tạm (không dùng vì pointer cần resolve bởi Manager)
        // QuanLyDatSan::docFile() sẽ resolve pointer sau khi đọc
        delete[] buffer;
    }
    khachHang = nullptr; // Sẽ được resolve bởi QuanLyDatSan

    // Đọc mã sân - set nullptr, QuanLyDatSan sẽ resolve sau
    fread(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        char *buffer = new char[len + 1];
        fread(buffer, sizeof(char), len, f);
        buffer[len] = '\0';
        // Lưu ID vào biến tạm (không dùng vì pointer cần resolve bởi Manager)
        delete[] buffer;
    }
    san = nullptr; // Sẽ được resolve bởi QuanLyDatSan

    // Đọc thời gian và khung giờ
    thoiGianDat.docFile(f);
    khungGio.docFile(f);

    // Đọc danh sách dịch vụ
    int soDichVu;
    fread(&soDichVu, sizeof(int), 1, f);
    danhSachDichVu = MangDong<DichVuDat>();
    for (int i = 0; i < soDichVu; i++)
    {
        DichVuDat dv;
        dv.docFile(f);
        danhSachDichVu.push_back(dv);
    }

    // Đọc tổng tiền, trạng thái
    fread(&tongTien, sizeof(double), 1, f);
    int tt;
    fread(&tt, sizeof(int), 1, f);
    trangThai = static_cast<TrangThaiDatSan>(tt);

    // Đọc ngày tạo
    ngayTao.docFile(f);

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

    // Đọc deposit fields
    fread(&tienCoc, sizeof(double), 1, f);
    int ttc;
    fread(&ttc, sizeof(int), 1, f);
    trangThaiCoc = static_cast<TrangThaiCoc>(ttc);
    ngayDatCoc.docFile(f);

    // Đọc cancel fields
    fread(&len, sizeof(int), 1, f);
    if (len > 0)
    {
        char *buffer = new char[len + 1];
        fread(buffer, sizeof(char), len, f);
        buffer[len] = '\0';
        lyDoHuy = std::string(buffer);
        delete[] buffer;
    }
    fread(&hoanCoc, sizeof(bool), 1, f);
}

// Helper methods for pointer resolution
std::string DatSan::getMaKhachHang() const
{
    return (khachHang != nullptr) ? khachHang->layMaKhachHang() : "";
}

std::string DatSan::getMaSan() const
{
    return (san != nullptr) ? san->getMaSan() : "";
}

// Static method tạo mã tự động
std::string DatSan::taoMaDatSan()
{
    time_t now = time(0);
    char buffer[50];
    sprintf(buffer, "DS%lld", static_cast<long long>(now));
    return std::string(buffer);
}

// ===== DEPOSIT METHODS =====

std::string DatSan::getTrangThaiCocText() const
{
    switch (trangThaiCoc)
    {
    case DA_COC:
        return "Đã cọc 30%";
    case HOAN_COC:
        return "Hoàn cọc";
    case MAT_COC:
        return "Mất cọc";
    default:
        return "N/A";
    }
}

void DatSan::tinhTienCoc()
{
    // Tiền cọc = 30% tổng tiền
    tienCoc = tongTien * 0.3;
}

void DatSan::huyBooking(bool hoan, const std::string &lyDo)
{
    // Hủy booking với lý do (nhân viên chọn hoàn/mất cọc)
    trangThai = DA_HUY;
    lyDoHuy = lyDo;
    hoanCoc = hoan;

    if (hoan)
    {
        // Hoàn cọc
        trangThaiCoc = HOAN_COC;
    }
    else
    {
        // Mất cọc
        trangThaiCoc = MAT_COC;
        // Cập nhật ngày thanh toán là thời điểm hủy (ghi nhận doanh thu phạt cọc)
        ngayThanhToan = NgayGio::layThoiGianHienTai();
    }
}
