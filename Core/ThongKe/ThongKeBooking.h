#ifndef THONGKEBOOKING_H
#define THONGKEBOOKING_H

#include "ThongKe.h"
#include "../CauTrucDuLieu/MangDong.h"
#include <string>

// Forward declarations
class HeThongQuanLy;
class San;
class DatSan;

// Thong tin thong ke cua mot san (dung cho top san)
struct ThongTinSanTop
{
    San *san;
    int soLuotDat;
    double doanhThu;
    double tyLeSuDung; // % su dung trong khoang thoi gian

    ThongTinSanTop() : san(nullptr), soLuotDat(0), doanhThu(0.0), tyLeSuDung(0.0) {}
};

// Thong ke booking theo khung gio
struct ThongKeKhungGio
{
    int gioBatDau;
    int gioKetThuc;
    int soLuotDat;
    double tyLe; // % so voi tong

    ThongKeKhungGio() : gioBatDau(0), gioKetThuc(0), soLuotDat(0), tyLe(0.0) {}
    ThongKeKhungGio(int bd, int kt) : gioBatDau(bd), gioKetThuc(kt), soLuotDat(0), tyLe(0.0) {}
};

// Thong ke booking theo ngay trong tuan
struct ThongKeTheoNgay
{
    int ngayTrongTuan; // 0 = CN, 1 = T2, ..., 6 = T7
    int soLuotDat;
    double doanhThu;

    ThongKeTheoNgay() : ngayTrongTuan(0), soLuotDat(0), doanhThu(0.0) {}
};

// Lop thong ke Booking va San
// Cung cap: Tong booking, hoan thanh, huy, Top san, TK khung gio, TK ngay trong tuan, Ty le su dung san
class ThongKeBooking : public ThongKe
{
private:
    // ===== METRICS CO BAN =====
    int tongSoBooking;
    int soBookingHoanThanh;
    int soBookingHuy;
    int soBookingDangCho;
    double tyLeHoanThanh;
    double tyLeHuy;

    // ===== THONG KE THEO LOAI SAN =====
    int soBookingSan5;
    int soBookingSan7;
    double doanhThuSan5;
    double doanhThuSan7;

    // ===== THONG KE KHUNG GIO =====
    int soBookingGioVang;   // 18:00-21:00
    int soBookingGioThuong;
    MangDong<ThongKeKhungGio> thongKeKhungGio;

    // ===== THONG KE THEO NGAY =====
    MangDong<ThongKeTheoNgay> thongKeTheoNgay; // 7 ngay trong tuan

    // ===== TOP SAN =====
    MangDong<ThongTinSanTop> topSan;

    // ===== UTILIZATION =====
    double tyLeSuDungTrungBinh;

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

    // ========== GETTERS - METRICS CO BAN ==========
    int getTongSoBooking() const;
    int getSoBookingHoanThanh() const;
    int getSoBookingHuy() const;
    int getSoBookingDangCho() const;
    double getTyLeHoanThanh() const;
    double getTyLeHuy() const;

    // ========== GETTERS - LOAI SAN ==========
    int getSoBookingSan5() const;
    int getSoBookingSan7() const;
    double getDoanhThuSan5() const;
    double getDoanhThuSan7() const;

    // ========== GETTERS - KHUNG GIO ==========
    int getSoBookingGioVang() const;
    int getSoBookingGioThuong() const;
    const MangDong<ThongKeKhungGio> &getThongKeKhungGio() const;

    // ========== GETTERS - NGAY TRONG TUAN ==========
    const MangDong<ThongKeTheoNgay> &getThongKeTheoNgay() const;

    // ========== GETTERS - TOP SAN ==========
    const MangDong<ThongTinSanTop> &getTopSan() const;

    // ========== GETTERS - UTILIZATION ==========
    double getTyLeSuDungTrungBinh() const;

    // ========== OVERRIDE ABSTRACT METHODS ==========
    void tinhToan() override;
    void hienThi() const override;
    void xuatFile(const std::string &tenFile) const override;

    // ========== SPECIFIC METHODS ==========
    void tinhTopSan(int soLuong = 5);        // Tinh top san duoc dat nhieu (soLuong: so top)
    void tinhThongKeKhungGio();              // Tinh TK theo khung gio
    void tinhThongKeTheoNgay();              // Tinh TK theo ngay trong tuan
    void tinhTyLeSuDung();                   // Tinh ty le su dung san
    int demSoBookingCuaSan(San *san);        // Lay so booking cua san cu the
    double tinhDoanhThuSan(San *san);        // Tinh doanh thu tu san
    bool laGioVang(DatSan *ds);              // Kiem tra booking co trong gio vang (18:00-21:00)
    MangDong<MangDong<int>> getHeatmapData(); // Lay TK heatmap [7 ngay][24 gio]
};

#endif // THONGKEBOOKING_H
