#ifndef THONGKEKHACHHANG_H
#define THONGKEKHACHHANG_H
#include <string>

#include "ThongKe.h"
#include "../CauTrucDuLieu/MangDong.h"

// Forward declaration
class HeThongQuanLy;
class KhachHang;

// Struct lưu thông tin khách hàng top
struct ThongTinKhachHangTop
{
    KhachHang *khachHang;
    int soDonDat;
    double tongChiTieu;

    ThongTinKhachHangTop() : khachHang(nullptr), soDonDat(0), tongChiTieu(0.0) {}
};

// Lớp thống kê khách hàng
class ThongKeKhachHang : public ThongKe
{
private:
    int tongSoKhachHang;                         // Tổng số khách hàng
    int soKhachHangMoi;                          // Số khách hàng mới
    int soKhachHangQuayLai;                      // Số khách hàng quay lại
    double chiTieuTrungBinh;                     // Chi tiêu trung bình
    MangDong<ThongTinKhachHangTop> topKhachHang; // Top khách hàng
    HeThongQuanLy *heThong;                      // Pointer tới hệ thống

public:
    // Constructor & Destructor
    ThongKeKhachHang();
    ThongKeKhachHang(const NgayThang &tu, const NgayThang &den, HeThongQuanLy *ht);
    ThongKeKhachHang(const ThongKeKhachHang &other);
    ~ThongKeKhachHang();

    // Operators
    ThongKeKhachHang &operator=(const ThongKeKhachHang &other);

    // Getters
    int getTongSoKhachHang() const;
    int getSoKhachHangMoi() const;
    int getSoKhachHangQuayLai() const;
    double getChiTieuTrungBinh() const;
    const MangDong<ThongTinKhachHangTop> &getTopKhachHang() const;

    // Override abstract methods
    void tinhToan() override;
    void hienThi() const override;
    void xuatFile(const std::string &tenFile) const override;

    // Specific methods
    void tinhTopKhachHang(int soLuong = 10);
    int demSoDonDatCuaKhachHang(KhachHang *kh);
    double tinhTongChiTieuCuaKhachHang(KhachHang *kh);
};

#endif // THONGKEKHACHHANG_H
