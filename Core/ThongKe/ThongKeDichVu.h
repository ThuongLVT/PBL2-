#ifndef THONGKEDICHVU_H
#define THONGKEDICHVU_H

#include "ThongKe.h"
#include "../CauTrucDuLieu/MangDong.h"
#include "../Models/DichVu.h"
#include <string>

// Forward declarations
class HeThongQuanLy;
class DichVu;

/**
 * @struct ThongTinDichVuTop
 * @brief Thông tin thống kê của một dịch vụ (dùng cho top dịch vụ)
 */
struct ThongTinDichVuTop
{
    DichVu *dichVu;
    int soLuongBan;
    double doanhThu;

    ThongTinDichVuTop() : dichVu(nullptr), soLuongBan(0), doanhThu(0.0) {}
};

/**
 * @struct ThongKeLoaiDichVu
 * @brief Thống kê theo loại dịch vụ
 */
struct ThongKeLoaiDichVu
{
    LoaiDichVu loai;
    int soLuongBan;
    double doanhThu;
    double tyLe; // % so với tổng

    ThongKeLoaiDichVu() : loai(LoaiDichVu::DO_UONG), soLuongBan(0), doanhThu(0.0), tyLe(0.0) {}
    ThongKeLoaiDichVu(LoaiDichVu l) : loai(l), soLuongBan(0), doanhThu(0.0), tyLe(0.0) {}
};

/**
 * @struct ThongKeTonKho
 * @brief Thông tin tồn kho của dịch vụ
 */
struct ThongKeTonKho
{
    DichVu *dichVu;
    int soLuongTon;
    int soLuongBan;
    double tyLeQuayVong; // Tỷ lệ quay vòng = bán / (tồn + bán)
    bool sapHet;         // < 10 items

    ThongKeTonKho() : dichVu(nullptr), soLuongTon(0), soLuongBan(0), tyLeQuayVong(0.0), sapHet(false) {}
};

/**
 * @class ThongKeDichVu
 * @brief Lớp thống kê Dịch vụ
 *
 * Cung cấp các thống kê về:
 * - Tổng số đơn dịch vụ
 * - Doanh thu theo loại (Đồ uống, Đồ ăn, Thiết bị)
 * - Top dịch vụ bán chạy
 * - Phân tích tồn kho
 * - Dịch vụ kèm theo booking phổ biến
 */
class ThongKeDichVu : public ThongKe
{
private:
    // ===== METRICS CƠ BẢN =====
    int tongSoLuongBan;        // Tổng số lượng dịch vụ đã bán
    double tongDoanhThuDichVu; // Tổng doanh thu dịch vụ
    int tongSoDonDichVu;       // Tổng số đơn hàng dịch vụ (riêng lẻ)
    int soDichVuKemBooking;    // Số dịch vụ đi kèm booking

    // ===== THỐNG KÊ THEO LOẠI =====
    MangDong<ThongKeLoaiDichVu> thongKeTheoLoai; // 3 loại: Đồ uống, Đồ ăn, Thiết bị
    double doanhThuDoUong;                       // Doanh thu đồ uống
    double doanhThuDoAn;                         // Doanh thu đồ ăn
    double doanhThuThietBi;                      // Doanh thu thiết bị

    // ===== TOP DỊCH VỤ =====
    MangDong<ThongTinDichVuTop> topDichVu; // Top dịch vụ bán chạy

    // ===== TỒN KHO =====
    MangDong<ThongKeTonKho> thongKeTonKho; // Thông tin tồn kho
    int soSanPhamSapHet;                   // Số sản phẩm sắp hết (< 10)
    int soSanPhamHetHang;                  // Số sản phẩm hết hàng (= 0)

    // ===== CROSS-SELLING =====
    MangDong<ThongTinDichVuTop> topDichVuKemBooking; // Top DV đi kèm booking

    // ===== REFERENCE =====
    HeThongQuanLy *heThong;

public:
    // ========== CONSTRUCTORS ==========
    ThongKeDichVu();
    ThongKeDichVu(const NgayThang &tu, const NgayThang &den, HeThongQuanLy *ht);
    ThongKeDichVu(const ThongKeDichVu &other);
    ~ThongKeDichVu();

    // ========== OPERATORS ==========
    ThongKeDichVu &operator=(const ThongKeDichVu &other);

    // ========== GETTERS - METRICS CƠ BẢN ==========
    int getTongSoLuongBan() const;
    double getTongDoanhThuDichVu() const;
    int getTongSoDonDichVu() const;
    int getSoDichVuKemBooking() const;

    // ========== GETTERS - THEO LOẠI ==========
    const MangDong<ThongKeLoaiDichVu> &getThongKeTheoLoai() const;
    double getDoanhThuDoUong() const;
    double getDoanhThuDoAn() const;
    double getDoanhThuThietBi() const;

    // ========== GETTERS - TOP DỊCH VỤ ==========
    const MangDong<ThongTinDichVuTop> &getTopDichVu() const;

    // ========== GETTERS - TỒN KHO ==========
    const MangDong<ThongKeTonKho> &getThongKeTonKho() const;
    int getSoSanPhamSapHet() const;
    int getSoSanPhamHetHang() const;

    // ========== GETTERS - CROSS-SELLING ==========
    const MangDong<ThongTinDichVuTop> &getTopDichVuKemBooking() const;

    // ========== OVERRIDE ABSTRACT METHODS ==========
    void tinhToan() override;
    void hienThi() const override;
    void xuatFile(const std::string &tenFile) const override;

    // ========== SPECIFIC METHODS ==========

    /**
     * @brief Tính top dịch vụ bán chạy nhất
     * @param soLuong Số lượng top (mặc định 10)
     */
    void tinhTopDichVu(int soLuong = 10);

    /**
     * @brief Tính thống kê theo loại dịch vụ
     */
    void tinhThongKeTheoLoai();

    /**
     * @brief Tính thống kê tồn kho
     */
    void tinhThongKeTonKho();

    /**
     * @brief Tính top dịch vụ đi kèm booking
     * @param soLuong Số lượng top (mặc định 5)
     */
    void tinhTopDichVuKemBooking(int soLuong = 5);

    /**
     * @brief Lấy số lượng đã bán của một dịch vụ
     * @param dv Con trỏ tới dịch vụ
     * @return Số lượng đã bán trong khoảng thời gian
     */
    int laySoLuongBanCuaDichVu(DichVu *dv);

    /**
     * @brief Tính doanh thu từ một dịch vụ
     * @param dv Con trỏ tới dịch vụ
     * @return Doanh thu trong khoảng thời gian
     */
    double tinhDoanhThuDichVu(DichVu *dv);

    /**
     * @brief Lấy số lượng DV đi kèm booking
     * @param dv Con trỏ tới dịch vụ
     * @return Số lượng đã bán kèm booking
     */
    int laySoLuongKemBooking(DichVu *dv);

    /**
     * @brief Lấy tên loại dịch vụ
     * @param loai Loại dịch vụ
     * @return Tên loại dạng chuỗi
     */
    std::string layTenLoai(LoaiDichVu loai) const;
};

#endif // THONGKEDICHVU_H
