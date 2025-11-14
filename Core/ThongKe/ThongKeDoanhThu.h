#ifndef THONGKEDOANHTHU_H
#define THONGKEDOANHTHU_H
#include <string>

#include "ThongKe.h"
#include "../CauTrucDuLieu/MangDong.h"

// Forward declaration
class HeThongQuanLy;

// Lớp thống kê doanh thu
class ThongKeDoanhThu : public ThongKe
{
private:
    double tongDoanhThu;      // Tổng doanh thu
    double doanhThuTienSan;   // Doanh thu từ tiền sân
    double doanhThuDichVu;    // Doanh thu từ dịch vụ
    int soLuongDonDat;        // Số lượng đơn đặt
    int soLuongDonHoanThanh;  // Số đơn hoàn thành
    int soLuongDonHuy;        // Số đơn hủy
    double doanhThuTrungBinh; // Doanh thu trung bình/ngày
    HeThongQuanLy *heThong;   // Pointer tới hệ thống

public:
    // Constructor & Destructor
    ThongKeDoanhThu();
    ThongKeDoanhThu(const NgayThang &tu, const NgayThang &den, HeThongQuanLy *ht);
    ThongKeDoanhThu(const ThongKeDoanhThu &other);
    ~ThongKeDoanhThu();

    // Operators
    ThongKeDoanhThu &operator=(const ThongKeDoanhThu &other);

    // Getters
    double getTongDoanhThu() const;
    double getDoanhThuTienSan() const;
    double getDoanhThuDichVu() const;
    int getSoLuongDonDat() const;
    int getSoLuongDonHoanThanh() const;
    int getSoLuongDonHuy() const;
    double getDoanhThuTrungBinh() const;

    // Override abstract methods
    void tinhToan() override;
    void hienThi() const override;
    void xuatFile(const std::string &tenFile) const override;

    // Specific methods
    double tinhDoanhThuTheoNgay(const NgayThang &ngay);
    double tinhDoanhThuTheoThang(int thang, int nam);
    void hienThiBieuDoDoanhThu() const;
};

#endif // THONGKEDOANHTHU_H
