#ifndef THONGKEBOOKING_H
#define THONGKEBOOKING_H

#include "ThongKe.h"
#include "../CauTrucDuLieu/MangDong.h"
#include <string>

// Forward declarations
class HeThongQuanLy;
class San;
class DatSan;

/**
 * @struct ThongTinSanTop
 * @brief Thông tin thống kê của một sân (dùng cho top sân)
 */
struct ThongTinSanTop
{
    San *san;
    int soLuotDat;
    double doanhThu;
    double tyLeSuDung; // % sử dụng trong khoảng thời gian

    ThongTinSanTop() : san(nullptr), soLuotDat(0), doanhThu(0.0), tyLeSuDung(0.0) {}
};

/**
 * @struct ThongKeKhungGio
 * @brief Thống kê booking theo khung giờ
 */
struct ThongKeKhungGio
{
    int gioBatDau;
    int gioKetThuc;
    int soLuotDat;
    double tyLe; // % so với tổng

    ThongKeKhungGio() : gioBatDau(0), gioKetThuc(0), soLuotDat(0), tyLe(0.0) {}
    ThongKeKhungGio(int bd, int kt) : gioBatDau(bd), gioKetThuc(kt), soLuotDat(0), tyLe(0.0) {}
};

/**
 * @struct ThongKeTheoNgay
 * @brief Thống kê booking theo ngày trong tuần
 */
struct ThongKeTheoNgay
{
    int ngayTrongTuan; // 0 = CN, 1 = T2, ..., 6 = T7
    int soLuotDat;
    double doanhThu;

    ThongKeTheoNgay() : ngayTrongTuan(0), soLuotDat(0), doanhThu(0.0) {}
};

/**
 * @class ThongKeBooking
 * @brief Lớp thống kê Booking và Sân
 *
 * Cung cấp các thống kê về:
 * - Tổng số booking, hoàn thành, hủy
 * - Top sân được đặt nhiều nhất
 * - Thống kê theo khung giờ (giờ vàng)
 * - Thống kê theo ngày trong tuần
 * - Tỷ lệ sử dụng sân
 * - Thống kê theo loại sân (5 người vs 7 người)
 */
class ThongKeBooking : public ThongKe
{
private:
    // ===== METRICS CƠ BẢN =====
    int tongSoBooking;      // Tổng số booking
    int soBookingHoanThanh; // Số booking hoàn thành
    int soBookingHuy;       // Số booking hủy
    int soBookingDangCho;   // Số booking đang chờ (DA_DAT)
    double tyLeHoanThanh;   // % hoàn thành
    double tyLeHuy;         // % hủy

    // ===== THỐNG KÊ THEO LOẠI SÂN =====
    int soBookingSan5;   // Số booking sân 5 người
    int soBookingSan7;   // Số booking sân 7 người
    double doanhThuSan5; // Doanh thu từ sân 5
    double doanhThuSan7; // Doanh thu từ sân 7

    // ===== THỐNG KÊ KHUNG GIỜ =====
    int soBookingGioVang;                      // Booking giờ vàng (18:00-21:00)
    int soBookingGioThuong;                    // Booking giờ thường
    MangDong<ThongKeKhungGio> thongKeKhungGio; // Chi tiết từng khung

    // ===== THỐNG KÊ THEO NGÀY =====
    MangDong<ThongKeTheoNgay> thongKeTheoNgay; // 7 ngày trong tuần

    // ===== TOP SÂN =====
    MangDong<ThongTinSanTop> topSan; // Top sân được đặt nhiều nhất

    // ===== UTILIZATION =====
    double tyLeSuDungTrungBinh; // Tỷ lệ sử dụng trung bình toàn hệ thống

    // ===== REFERENCE =====
    HeThongQuanLy *heThong;

public:
    // ========== CONSTRUCTORS ==========
    ThongKeBooking();
    ThongKeBooking(const NgayThang &tu, const NgayThang &den, HeThongQuanLy *ht);
    ThongKeBooking(const ThongKeBooking &other);
    ~ThongKeBooking();

    // ========== OPERATORS ==========
    ThongKeBooking &operator=(const ThongKeBooking &other);

    // ========== GETTERS - METRICS CƠ BẢN ==========
    int getTongSoBooking() const;
    int getSoBookingHoanThanh() const;
    int getSoBookingHuy() const;
    int getSoBookingDangCho() const;
    double getTyLeHoanThanh() const;
    double getTyLeHuy() const;

    // ========== GETTERS - LOẠI SÂN ==========
    int getSoBookingSan5() const;
    int getSoBookingSan7() const;
    double getDoanhThuSan5() const;
    double getDoanhThuSan7() const;

    // ========== GETTERS - KHUNG GIỜ ==========
    int getSoBookingGioVang() const;
    int getSoBookingGioThuong() const;
    const MangDong<ThongKeKhungGio> &getThongKeKhungGio() const;

    // ========== GETTERS - NGÀY TRONG TUẦN ==========
    const MangDong<ThongKeTheoNgay> &getThongKeTheoNgay() const;

    // ========== GETTERS - TOP SÂN ==========
    const MangDong<ThongTinSanTop> &getTopSan() const;

    // ========== GETTERS - UTILIZATION ==========
    double getTyLeSuDungTrungBinh() const;

    // ========== OVERRIDE ABSTRACT METHODS ==========
    void tinhToan() override;
    void hienThi() const override;
    void xuatFile(const std::string &tenFile) const override;

    // ========== SPECIFIC METHODS ==========

    /**
     * @brief Tính top sân được đặt nhiều nhất
     * @param soLuong Số lượng top (mặc định 5)
     */
    void tinhTopSan(int soLuong = 5);

    /**
     * @brief Tính thống kê theo khung giờ
     */
    void tinhThongKeKhungGio();

    /**
     * @brief Tính thống kê theo ngày trong tuần
     */
    void tinhThongKeTheoNgay();

    /**
     * @brief Tính tỷ lệ sử dụng sân
     */
    void tinhTyLeSuDung();

    /**
     * @brief Lấy số booking của một sân cụ thể
     * @param san Con trỏ tới sân
     * @return Số lượt booking
     */
    int demSoBookingCuaSan(San *san);

    /**
     * @brief Tính doanh thu từ một sân cụ thể
     * @param san Con trỏ tới sân
     * @return Doanh thu
     */
    double tinhDoanhThuSan(San *san);

    /**
     * @brief Kiểm tra booking có trong giờ vàng không
     * @param ds Con trỏ tới đặt sân
     * @return true nếu trong giờ vàng (18:00-21:00)
     */
    bool laGioVang(DatSan *ds);

    /**
     * @brief Lấy thống kê heatmap (ngày x khung giờ)
     * @return Mảng 2D [7 ngày][24 giờ] chứa số booking
     */
    MangDong<MangDong<int>> getHeatmapData();
};

#endif // THONGKEBOOKING_H
