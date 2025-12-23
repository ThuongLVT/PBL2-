// Lop quan ly don hang dich vu (khong lien quan dat san)

#ifndef QUANLYDONHANGDICHVU_H
#define QUANLYDONHANGDICHVU_H

#include "../Models/DonHangDichVu.h"
#include "../Models/KhachHang.h"
#include "../CauTrucDuLieu/MangDong.h"
#include "../CauTrucDuLieu/NgayGio.h"
#include <string>
#include <fstream>

// Lop quan ly don hang dich vu
// Chuc nang: CRUD, tim kiem theo KH/ngay, thong ke doanh thu
class QuanLyDonHangDichVu
{
private:
    MangDong<DonHangDichVu *> danhSachDonHang;
    int maxOrderId; // ID don hang lon nhat (auto-increment)

public:
    // ========== CONSTRUCTORS ==========
    QuanLyDonHangDichVu();
    ~QuanLyDonHangDichVu();

    // ========== CRUD OPERATIONS ==========
    std::string taoMaDonHangMoi();                                       // Tao ma don hang moi (SV001, SV002...)
    DonHangDichVu *taoDonHang(KhachHang *kh);                            // Tao don hang moi (kh co the NULL)
    bool huyDonHang(const std::string &maDH);                            // Huy don hang theo ma
    bool capNhatTrangThai(const std::string &maDH, TrangThaiDonHang tt); // Cap nhat trang thai
    DonHangDichVu *timDonHang(const std::string &maDH);                  // Tim don hang theo ma
    const MangDong<DonHangDichVu *> &layDanhSachDonHang() const;         // Lay toan bo danh sach

    // ========== SEARCH ==========
    MangDong<DonHangDichVu *> timDonHangTheoKhachHang(const std::string &maKH);
    MangDong<DonHangDichVu *> timDonHangTheoNgay(const NgayThang &ngay);
    MangDong<DonHangDichVu *> timDonHangTheoTrangThai(TrangThaiDonHang trangThai);

    // ========== DISPLAY ==========
    void hienThiDanhSachDonHang() const;

    // ========== STATISTICS ==========
    int tongSoDonHang() const;
    double tongDoanhThu() const;
    double doanhThuTheoNgay(const NgayThang &ngay) const;

    // ========== CSV I/O ==========
    bool luuCSV(const std::string &filePath) const;
    bool docCSV(const std::string &filePath);
    void xoaTatCa();
};

#endif // QUANLYDONHANGDICHVU_H
