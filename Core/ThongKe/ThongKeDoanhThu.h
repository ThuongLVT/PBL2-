#ifndef THONGKEDOANHTHU_H
#define THONGKEDOANHTHU_H
#include <string>

#include "ThongKe.h"
#include "../CauTrucDuLieu/MangDong.h"

// Forward declaration
class HeThongQuanLy;

// Doanh thu cua mot ngay cu the (dung cho chart)
struct DoanhThuTheoNgay
{
    NgayThang ngay;
    double doanhThu;
    double doanhThuSan;
    double doanhThuDichVu;
    int soBooking;

    DoanhThuTheoNgay() : doanhThu(0.0), doanhThuSan(0.0), doanhThuDichVu(0.0), soBooking(0) {}
};

// Doanh thu cua mot thang (dung cho chart)
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

// Lop thong ke doanh thu - Enhanced version
// Cung cap: Tong DT, DT san, DT dich vu, So sanh ky truoc, DT theo ngay/thang, DT theo nguon, DT theo khung gio
class ThongKeDoanhThu : public ThongKe
{
private:
    // ===== METRICS CO BAN =====
    double tongDoanhThu;      // Tong doanh thu
    double doanhThuTienSan;   // Doanh thu tu tien san
    double doanhThuDichVu;    // Doanh thu tu dich vu
    int soLuongDonDat;        // So luong don dat
    int soLuongDonHoanThanh;  // So don hoan thanh
    int soLuongDonHuy;        // So don huy
    double doanhThuTrungBinh; // Doanh thu trung binh/ngay

    // ===== SO SANH KY TRUOC =====
    double doanhThuKyTruoc;  // Doanh thu ky truoc (cung so ngay)
    double phanTramTangGiam; // % tang/giam so voi ky truoc
    double doanhThuSanKyTruoc;
    double doanhThuDVKyTruoc;
    double phanTramTangSan;
    double phanTramTangDV;

    // ===== DOANH THU CHI TIET THEO LOAI DV =====
    double doanhThuDoUong;  // Doanh thu do uong
    double doanhThuDoAn;    // Doanh thu do an
    double doanhThuThietBi; // Doanh thu thiet bi

    // ===== DOANH THU THEO THOI GIAN (CHO CHARTS) =====
    MangDong<DoanhThuTheoNgay> doanhThuTheoNgay;   // Doanh thu tung ngay
    MangDong<DoanhThuTheoThang> doanhThuTheoThang; // Doanh thu tung thang

    // ===== DOANH THU THEO KHUNG GIO =====
    double doanhThuGioVang;   // 18:00 - 21:00
    double doanhThuGioThuong; // Con lai

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

    // ===== GETTERS - CO BAN =====
    double getTongDoanhThu() const;

    // Du bao doanh thu ngay mai (su dung Linear Regression)
    double duBaoDoanhThuNgayMai() const;
    double getDoanhThuTienSan() const;
    double getDoanhThuDichVu() const;
    int getSoLuongDonDat() const;
    int getSoLuongDonHoanThanh() const;
    int getSoLuongDonHuy() const;
    double getDoanhThuTrungBinh() const;

    // ===== GETTERS - SO SANH =====
    double getDoanhThuKyTruoc() const;
    double getPhanTramTangGiam() const;
    double getDoanhThuSanKyTruoc() const;
    double getDoanhThuDVKyTruoc() const;
    double getPhanTramTangSan() const;
    double getPhanTramTangDV() const;

    // ===== GETTERS - CHI TIET LOAI DV =====
    double getDoanhThuDoUong() const;
    double getDoanhThuDoAn() const;
    double getDoanhThuThietBi() const;

    // ===== GETTERS - THEO THOI GIAN =====
    const MangDong<DoanhThuTheoNgay> &getDoanhThuTheoNgayList() const;
    const MangDong<DoanhThuTheoThang> &getDoanhThuTheoThangList() const;

    // ===== GETTERS - KHUNG GIO =====
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
    void tinhSoSanhKyTruoc();        // Tinh so sanh voi ky truoc
    void tinhDoanhThuTheoTungNgay(); // Tinh DT theo tung ngay trong khoang
    void tinhDoanhThuTheoTungThang(); // Tinh DT theo tung thang trong khoang
    void tinhDoanhThuTheoKhungGio(); // Tinh DT theo khung gio
    void tinhDoanhThuChiTietDichVu(); // Tinh DT chi tiet theo loai dich vu
};

#endif // THONGKEDOANHTHU_H
