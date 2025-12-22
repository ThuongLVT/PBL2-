#ifndef THONGKEKHACHHANG_H
#define THONGKEKHACHHANG_H
#include <string>

#include "ThongKe.h"
#include "../CauTrucDuLieu/MangDong.h"
#include "../Models/KhachHang.h"

// Forward declaration
class HeThongQuanLy;
class KhachHang;

// Thong tin khach hang top
struct ThongTinKhachHangTop
{
    KhachHang *khachHang;
    int soDonDat;
    double tongChiTieu;

    ThongTinKhachHangTop() : khachHang(nullptr), soDonDat(0), tongChiTieu(0.0) {}
};

// Thong ke theo hang khach hang
struct ThongKeHangKhachHang
{
    HangKhachHang hang;
    int soLuong;
    double tyLe;             // % so voi tong
    double chiTieuTrungBinh; // Chi tieu TB cua hang nay

    ThongKeHangKhachHang() : hang(HangKhachHang::THUONG), soLuong(0), tyLe(0.0), chiTieuTrungBinh(0.0) {}
    ThongKeHangKhachHang(HangKhachHang h) : hang(h), soLuong(0), tyLe(0.0), chiTieuTrungBinh(0.0) {}
};

// Thong ke khach hang theo thang (cho chart)
struct ThongKeKhachHangTheoThang
{
    int thang;
    int nam;
    int khachMoi;
    int khachQuayLai;
    double doanhThu;

    ThongKeKhachHangTheoThang() : thang(0), nam(0), khachMoi(0), khachQuayLai(0), doanhThu(0.0) {}
};

// Lop thong ke khach hang - Enhanced version
// Cung cap: Tong so KH, khach moi, khach quay lai, Top KH, Phan bo theo hang, TK theo thang, Customer insights
class ThongKeKhachHang : public ThongKe
{
private:
    // ===== METRICS CO BAN =====
    int tongSoKhachHang;     // Tong so khach hang
    int soKhachHangMoi;      // So khach hang moi (dang ky trong ky)
    int soKhachHangQuayLai;  // So khach hang quay lai (da dat > 1 lan)
    int soKhachHangHoatDong; // So KH co dat san trong ky
    double chiTieuTrungBinh; // Chi tieu trung binh

    // ===== SO SANH KY TRUOC =====
    int soKhachHangMoiKyTruoc;
    double phanTramTangKhachMoi;

    // ===== TOP KHACH HANG =====
    MangDong<ThongTinKhachHangTop> topKhachHang;

    // ===== THONG KE THEO HANG =====
    MangDong<ThongKeHangKhachHang> thongKeTheoHang;

    // ===== THONG KE THEO THANG =====
    MangDong<ThongKeKhachHangTheoThang> thongKeTheoThang;

    // ===== CUSTOMER INSIGHTS =====
    double customerLifetimeValue; // CLV trung binh
    double tanSuatDatSanTB;       // Tan suat dat san TB (lan/thang)
    int soKhachHangRoiBo;         // So KH khong dat > 3 thang

    // ===== REFERENCE =====
    HeThongQuanLy *heThong;

public:
    // Constructor & Destructor
    ThongKeKhachHang();
    ThongKeKhachHang(const NgayThang &tu, const NgayThang &den, HeThongQuanLy *ht);
    ThongKeKhachHang(const ThongKeKhachHang &other);
    ~ThongKeKhachHang();

    // Operators
    ThongKeKhachHang &operator=(const ThongKeKhachHang &other);

    // ===== GETTERS - CO BAN =====
    int getTongSoKhachHang() const;
    int getSoKhachHangMoi() const;
    int getSoKhachHangQuayLai() const;
    int getSoKhachHangHoatDong() const;
    double getChiTieuTrungBinh() const;

    // ===== GETTERS - SO SANH =====
    int getSoKhachHangMoiKyTruoc() const;
    double getPhanTramTangKhachMoi() const;

    // ===== GETTERS - TOP & PHAN LOAI =====
    const MangDong<ThongTinKhachHangTop> &getTopKhachHang() const;
    const MangDong<ThongKeHangKhachHang> &getThongKeTheoHang() const;
    const MangDong<ThongKeKhachHangTheoThang> &getThongKeTheoThang() const;

    // ===== GETTERS - INSIGHTS =====
    double getCustomerLifetimeValue() const;
    double getTanSuatDatSanTB() const;
    int getSoKhachHangRoiBo() const;

    // Override abstract methods
    void tinhToan() override;
    void hienThi() const override;
    void xuatFile(const std::string &tenFile) const override;

    // ===== SPECIFIC METHODS =====
    void tinhTopKhachHang(int soLuong = 10);
    int demSoDonDatCuaKhachHang(KhachHang *kh);
    double tinhTongChiTieuCuaKhachHang(KhachHang *kh);

    // ===== NEW METHODS =====
    void tinhThongKeTheoHang();    // Tinh thong ke theo hang khach hang
    void tinhThongKeTheoThang();   // Tinh thong ke theo thang
    void tinhCustomerInsights();   // Tinh customer insights
    void tinhSoSanhKyTruoc();      // Tinh so sanh voi ky truoc
    std::string layTenHang(HangKhachHang hang) const; // Lay ten hang khach hang
    bool laDangKyTrongKy(KhachHang *kh);  // Kiem tra KH co dang ky trong ky
    bool laHoatDongTrongKy(KhachHang *kh); // Kiem tra KH co hoat dong trong ky
};

#endif // THONGKEKHACHHANG_H
