#ifndef THONGKEDOANHTHU_H
#define THONGKEDOANHTHU_H
#include <string>

#include "ThongKe.h"
#include "../CauTrucDuLieu/MangDong.h"

// Forward declaration
class HeThongQuanLy;

/**
 * @struct DoanhThuTheoNgay
 * @brief Doanh thu của một ngày cụ thể (dùng cho chart)
 */
struct DoanhThuTheoNgay
{
    NgayThang ngay;
    double doanhThu;
    double doanhThuSan;
    double doanhThuDichVu;
    int soBooking;

    DoanhThuTheoNgay() : doanhThu(0.0), doanhThuSan(0.0), doanhThuDichVu(0.0), soBooking(0) {}
};

/**
 * @struct DoanhThuTheoThang
 * @brief Doanh thu của một tháng (dùng cho chart)
 */
struct DoanhThuTheoThang
{
    int thang;
    int nam;
    double doanhThu;
    double doanhThuSan;
    double doanhThuDichVu;
    int soBooking;

    DoanhThuTheoThang() : thang(0), nam(0), doanhThu(0.0), doanhThuSan(0.0), doanhThuDichVu(0.0), soBooking(0) {}
};

/**
 * @class ThongKeDoanhThu
 * @brief Lớp thống kê doanh thu - Enhanced version
 *
 * Cung cấp:
 * - Tổng doanh thu, doanh thu sân, doanh thu dịch vụ
 * - So sánh với kỳ trước (% tăng/giảm)
 * - Doanh thu theo ngày/tháng (cho line chart)
 * - Doanh thu theo nguồn (cho pie chart)
 * - Doanh thu theo khung giờ
 */
class ThongKeDoanhThu : public ThongKe
{
private:
    // ===== METRICS CƠ BẢN =====
    double tongDoanhThu;      // Tổng doanh thu
    double doanhThuTienSan;   // Doanh thu từ tiền sân
    double doanhThuDichVu;    // Doanh thu từ dịch vụ
    int soLuongDonDat;        // Số lượng đơn đặt
    int soLuongDonHoanThanh;  // Số đơn hoàn thành
    int soLuongDonHuy;        // Số đơn hủy
    double doanhThuTrungBinh; // Doanh thu trung bình/ngày

    // ===== SO SÁNH KỲ TRƯỚC =====
    double doanhThuKyTruoc;  // Doanh thu kỳ trước (cùng số ngày)
    double phanTramTangGiam; // % tăng/giảm so với kỳ trước
    double doanhThuSanKyTruoc;
    double doanhThuDVKyTruoc;
    double phanTramTangSan;
    double phanTramTangDV;

    // ===== DOANH THU CHI TIẾT THEO LOẠI DV =====
    double doanhThuDoUong;  // Doanh thu đồ uống
    double doanhThuDoAn;    // Doanh thu đồ ăn
    double doanhThuThietBi; // Doanh thu thiết bị

    // ===== DOANH THU THEO THỜI GIAN (CHO CHARTS) =====
    MangDong<DoanhThuTheoNgay> doanhThuTheoNgay;   // Doanh thu từng ngày
    MangDong<DoanhThuTheoThang> doanhThuTheoThang; // Doanh thu từng tháng

    // ===== DOANH THU THEO KHUNG GIỜ =====
    double doanhThuGioVang;   // 18:00 - 21:00
    double doanhThuGioThuong; // Còn lại

    // ===== REFERENCE =====
    HeThongQuanLy *heThong;

public:
    // Constructor & Destructor
    ThongKeDoanhThu();
    ThongKeDoanhThu(const NgayThang &tu, const NgayThang &den, HeThongQuanLy *ht);
    ThongKeDoanhThu(const ThongKeDoanhThu &other);
    ~ThongKeDoanhThu();

    // Operators
    ThongKeDoanhThu &operator=(const ThongKeDoanhThu &other);

    // ===== GETTERS - CƠ BẢN =====
    double getTongDoanhThu() const;
    double getDoanhThuTienSan() const;
    double getDoanhThuDichVu() const;
    int getSoLuongDonDat() const;
    int getSoLuongDonHoanThanh() const;
    int getSoLuongDonHuy() const;
    double getDoanhThuTrungBinh() const;

    // ===== GETTERS - SO SÁNH =====
    double getDoanhThuKyTruoc() const;
    double getPhanTramTangGiam() const;
    double getDoanhThuSanKyTruoc() const;
    double getDoanhThuDVKyTruoc() const;
    double getPhanTramTangSan() const;
    double getPhanTramTangDV() const;

    // ===== GETTERS - CHI TIẾT LOẠI DV =====
    double getDoanhThuDoUong() const;
    double getDoanhThuDoAn() const;
    double getDoanhThuThietBi() const;

    // ===== GETTERS - THEO THỜI GIAN =====
    const MangDong<DoanhThuTheoNgay> &getDoanhThuTheoNgayList() const;
    const MangDong<DoanhThuTheoThang> &getDoanhThuTheoThangList() const;

    // ===== GETTERS - KHUNG GIỜ =====
    double getDoanhThuGioVang() const;
    double getDoanhThuGioThuong() const;

    // Override abstract methods
    void tinhToan() override;
    void hienThi() const override;
    void xuatFile(const std::string &tenFile) const override;

    // ===== SPECIFIC METHODS =====
    double tinhDoanhThuTheoNgay(const NgayThang &ngay);
    double tinhDoanhThuTheoThang(int thang, int nam);
    void hienThiBieuDoDoanhThu() const;

    // ===== NEW METHODS =====
    /**
     * @brief Tính so sánh với kỳ trước
     */
    void tinhSoSanhKyTruoc();

    /**
     * @brief Tính doanh thu theo từng ngày trong khoảng
     */
    void tinhDoanhThuTheoTungNgay();

    /**
     * @brief Tính doanh thu theo từng tháng trong khoảng
     */
    void tinhDoanhThuTheoTungThang();

    /**
     * @brief Tính doanh thu theo khung giờ
     */
    void tinhDoanhThuTheoKhungGio();

    /**
     * @brief Tính doanh thu chi tiết theo loại dịch vụ
     */
    void tinhDoanhThuChiTietDichVu();
};

#endif // THONGKEDOANHTHU_H
