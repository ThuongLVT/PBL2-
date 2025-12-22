#ifndef THONGKEDICHVU_H
#define THONGKEDICHVU_H

#include "ThongKe.h"
#include "../CauTrucDuLieu/MangDong.h"
#include "../Models/DichVu.h"
#include <string>

// Forward declarations
class HeThongQuanLy;
class DichVu;

// Thong tin thong ke cua mot dich vu (dung cho top dich vu)
struct ThongTinDichVuTop
{
    DichVu *dichVu;
    int soLuongBan;
    double doanhThu;

    ThongTinDichVuTop() : dichVu(nullptr), soLuongBan(0), doanhThu(0.0) {}
};

// Thong ke theo loai dich vu
struct ThongKeLoaiDichVu
{
    LoaiDichVu loai;
    int soLuongBan;
    double doanhThu;
    double tyLe; // % so voi tong

    ThongKeLoaiDichVu() : loai(LoaiDichVu::DO_UONG), soLuongBan(0), doanhThu(0.0), tyLe(0.0) {}
    ThongKeLoaiDichVu(LoaiDichVu l) : loai(l), soLuongBan(0), doanhThu(0.0), tyLe(0.0) {}
};

// Thong tin ton kho cua dich vu
struct ThongKeTonKho
{
    DichVu *dichVu;
    int soLuongTon;
    int soLuongBan;
    double tyLeQuayVong; // Ty le quay vong = ban / (ton + ban)
    bool sapHet;         // < 10 items

    ThongKeTonKho() : dichVu(nullptr), soLuongTon(0), soLuongBan(0), tyLeQuayVong(0.0), sapHet(false) {}
};

// Lop thong ke Dich vu
// Cung cap: Tong don DV, DT theo loai, Top DV ban chay, Phan tich ton kho, DV kem booking
class ThongKeDichVu : public ThongKe
{
private:
    // ===== METRICS CO BAN =====
    int tongSoLuongBan;        // Tong so luong dich vu da ban
    double tongDoanhThuDichVu; // Tong doanh thu dich vu
    int tongSoDonDichVu;       // Tong so don hang dich vu (rieng le)
    int soDichVuKemBooking;    // So dich vu di kem booking

    // ===== THONG KE THEO LOAI =====
    MangDong<ThongKeLoaiDichVu> thongKeTheoLoai; // 3 loai: Do uong, Do an, Thiet bi
    double doanhThuDoUong;
    double doanhThuDoAn;
    double doanhThuThietBi;

    // ===== TOP DICH VU =====
    MangDong<ThongTinDichVuTop> topDichVu; // Top dich vu ban chay

    // ===== TON KHO =====
    MangDong<ThongKeTonKho> thongKeTonKho;
    int soSanPhamSapHet;  // So san pham sap het (< 10)
    int soSanPhamHetHang; // So san pham het hang (= 0)

    // ===== CROSS-SELLING =====
    MangDong<ThongTinDichVuTop> topDichVuKemBooking; // Top DV di kem booking

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

    // ========== GETTERS - METRICS CO BAN ==========
    int getTongSoLuongBan() const;
    double getTongDoanhThuDichVu() const;
    int getTongSoDonDichVu() const;
    int getSoDichVuKemBooking() const;

    // ========== GETTERS - THEO LOAI ==========
    const MangDong<ThongKeLoaiDichVu> &getThongKeTheoLoai() const;
    double getDoanhThuDoUong() const;
    double getDoanhThuDoAn() const;
    double getDoanhThuThietBi() const;

    // ========== GETTERS - TOP DICH VU ==========
    const MangDong<ThongTinDichVuTop> &getTopDichVu() const;

    // ========== GETTERS - TON KHO ==========
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
    void tinhTopDichVu(int soLuong = 10);       // Tinh top DV ban chay (soLuong: so luong top)
    void tinhThongKeTheoLoai();                 // Tinh thong ke theo loai DV
    void tinhThongKeTonKho();                   // Tinh thong ke ton kho
    void tinhTopDichVuKemBooking(int soLuong = 5); // Tinh top DV kem booking
    int laySoLuongBanCuaDichVu(DichVu *dv);     // Lay so luong da ban cua DV
    double tinhDoanhThuDichVu(DichVu *dv);      // Tinh doanh thu tu DV
    int laySoLuongKemBooking(DichVu *dv);       // Lay so luong DV kem booking
    std::string layTenLoai(LoaiDichVu loai) const; // Lay ten loai DV
};

#endif // THONGKEDICHVU_H
