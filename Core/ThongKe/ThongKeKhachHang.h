#ifndef THONGKEKHACHHANG_H
#define THONGKEKHACHHANG_H
#include <string>

#include "ThongKe.h"
#include "../CauTrucDuLieu/MangDong.h"
#include "../Models/KhachHang.h"

// Forward declaration
class HeThongQuanLy;
class KhachHang;

/**
 * @struct ThongTinKhachHangTop
 * @brief Thông tin khách hàng top
 */
struct ThongTinKhachHangTop
{
    KhachHang *khachHang;
    int soDonDat;
    double tongChiTieu;

    ThongTinKhachHangTop() : khachHang(nullptr), soDonDat(0), tongChiTieu(0.0) {}
};

/**
 * @struct ThongKeHangKhachHang
 * @brief Thống kê theo hạng khách hàng
 */
struct ThongKeHangKhachHang
{
    HangKhachHang hang;
    int soLuong;
    double tyLe;             // % so với tổng
    double chiTieuTrungBinh; // Chi tiêu TB của hạng này

    ThongKeHangKhachHang() : hang(HangKhachHang::THUONG), soLuong(0), tyLe(0.0), chiTieuTrungBinh(0.0) {}
    ThongKeHangKhachHang(HangKhachHang h) : hang(h), soLuong(0), tyLe(0.0), chiTieuTrungBinh(0.0) {}
};

/**
 * @struct ThongKeKhachHangTheoThang
 * @brief Thống kê khách hàng theo tháng (cho chart)
 */
struct ThongKeKhachHangTheoThang
{
    int thang;
    int nam;
    int khachMoi;
    int khachQuayLai;
    double doanhThu;

    ThongKeKhachHangTheoThang() : thang(0), nam(0), khachMoi(0), khachQuayLai(0), doanhThu(0.0) {}
};

/**
 * @class ThongKeKhachHang
 * @brief Lớp thống kê khách hàng - Enhanced version
 *
 * Cung cấp:
 * - Tổng số KH, khách mới, khách quay lại
 * - Top khách hàng chi tiêu cao nhất
 * - Phân bổ theo hạng thành viên
 * - Thống kê theo tháng (cho chart)
 * - Customer insights (CLV, tần suất, rời bỏ)
 */
class ThongKeKhachHang : public ThongKe
{
private:
    // ===== METRICS CƠ BẢN =====
    int tongSoKhachHang;     // Tổng số khách hàng
    int soKhachHangMoi;      // Số khách hàng mới (đăng ký trong kỳ)
    int soKhachHangQuayLai;  // Số khách hàng quay lại (đã đặt > 1 lần)
    int soKhachHangHoatDong; // Số KH có đặt sân trong kỳ
    double chiTieuTrungBinh; // Chi tiêu trung bình

    // ===== SO SÁNH KỲ TRƯỚC =====
    int soKhachHangMoiKyTruoc;
    double phanTramTangKhachMoi;

    // ===== TOP KHÁCH HÀNG =====
    MangDong<ThongTinKhachHangTop> topKhachHang;

    // ===== THỐNG KÊ THEO HẠNG =====
    MangDong<ThongKeHangKhachHang> thongKeTheoHang;

    // ===== THỐNG KÊ THEO THÁNG =====
    MangDong<ThongKeKhachHangTheoThang> thongKeTheoThang;

    // ===== CUSTOMER INSIGHTS =====
    double customerLifetimeValue; // CLV trung bình
    double tanSuatDatSanTB;       // Tần suất đặt sân TB (lần/tháng)
    int soKhachHangRoiBo;         // Số KH không đặt > 3 tháng

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

    // ===== GETTERS - CƠ BẢN =====
    int getTongSoKhachHang() const;
    int getSoKhachHangMoi() const;
    int getSoKhachHangQuayLai() const;
    int getSoKhachHangHoatDong() const;
    double getChiTieuTrungBinh() const;

    // ===== GETTERS - SO SÁNH =====
    int getSoKhachHangMoiKyTruoc() const;
    double getPhanTramTangKhachMoi() const;

    // ===== GETTERS - TOP & PHÂN LOẠI =====
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
    /**
     * @brief Tính thống kê theo hạng khách hàng
     */
    void tinhThongKeTheoHang();

    /**
     * @brief Tính thống kê theo tháng
     */
    void tinhThongKeTheoThang();

    /**
     * @brief Tính customer insights
     */
    void tinhCustomerInsights();

    /**
     * @brief Tính so sánh với kỳ trước
     */
    void tinhSoSanhKyTruoc();

    /**
     * @brief Lấy tên hạng khách hàng
     */
    std::string layTenHang(HangKhachHang hang) const;

    /**
     * @brief Kiểm tra khách hàng có đăng ký trong kỳ không
     */
    bool laDangKyTrongKy(KhachHang *kh);

    /**
     * @brief Kiểm tra khách hàng có hoạt động trong kỳ không
     */
    bool laHoatDongTrongKy(KhachHang *kh);
};

#endif // THONGKEKHACHHANG_H
