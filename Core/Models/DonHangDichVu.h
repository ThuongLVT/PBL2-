// DonHangDichVu.h - Lop dai dien cho don hang dich vu rieng
// Don hang dich vu doc lap - khong lien quan den dat san
// Khach hang chi mua do uong/thiet bi ma khong dat san

#ifndef DONHANGDICHVU_H
#define DONHANGDICHVU_H

#include "KhachHang.h"
#include "DichVuDat.h"
#include "../CauTrucDuLieu/MangDong.h"
#include "../CauTrucDuLieu/NgayGio.h"
#include <string>
#include <fstream>

// Forward declarations
class QuanLyKhachHang;
class QuanLyDichVu;

// Trang thai don hang dich vu
enum class TrangThaiDonHang
{
    CHO_XU_LY,      // Dang cho xu ly
    DANG_CHUAN_BI,  // Dang chuan bi
    HOAN_THANH,     // Hoan thanh
    DA_HUY          // Da huy
};

// Don hang dich vu rieng (khong lien quan den dat san)
// Dai dien cho don dat dich vu doc lap - khach hang chi mua do uong/thiet bi ma khong dat san
class DonHangDichVu
{
private:
    std::string maDonHang;               // Ma don (tu sinh: DH + timestamp)
    KhachHang *khachHang;                // Khach hang (co the NULL - khach vang lai)
    MangDong<DichVuDat> danhSachDichVu;  // Danh sach dich vu dat
    double tongTien;                     // Tong tien (chua giam gia)
    double giamGia;                      // Giam gia (theo membership)
    double thanhTien;                    // Thanh tien = tongTien - giamGia
    TrangThaiDonHang trangThai;          // Trang thai don hang
    NgayGio ngayTao;                     // Ngay tao don
    std::string ghiChu;                  // Ghi chu

public:
    // ========== CONSTRUCTORS ==========
    
    // Constructor mac dinh
    DonHangDichVu();
    
    // Constructor voi tham so (ma: Ma don hang, kh: Khach hang - co the NULL)
    DonHangDichVu(const std::string &ma, KhachHang *kh);
    
    // Copy constructor (other: Doi tuong DonHangDichVu khac)
    DonHangDichVu(const DonHangDichVu &other);
    
    // Destructor
    ~DonHangDichVu();

    // ========== OPERATORS ==========
    
    // Toan tu gan (other: Doi tuong DonHangDichVu khac)
    DonHangDichVu &operator=(const DonHangDichVu &other);
    
    // Toan tu so sanh theo ma don hang (other: Doi tuong DonHangDichVu khac)
    bool operator==(const DonHangDichVu &other) const;

    // ========== GETTERS ==========
    
    // Lay ma don hang
    std::string getMaDonHang() const;
    
    // Lay khach hang (pointer)
    KhachHang *getKhachHang() const;
    
    // Lay danh sach dich vu da dat
    const MangDong<DichVuDat> &getDanhSachDichVu() const;
    
    // Lay tong tien (chua giam gia)
    double getTongTien() const;
    
    // Lay so tien giam gia
    double getGiamGia() const;
    
    // Lay thanh tien (sau giam gia)
    double getThanhTien() const;
    
    // Lay trang thai don hang
    TrangThaiDonHang getTrangThai() const;
    
    // Lay ngay gio tao don
    NgayGio getNgayTao() const;
    
    // Lay ghi chu
    std::string getGhiChu() const;

    // ========== SETTERS ==========
    
    // Dat ma don hang (ma: Ma don hang moi)
    void setMaDonHang(const std::string &ma);
    
    // Dat khach hang (kh: Pointer toi khach hang)
    void setKhachHang(KhachHang *kh);
    
    // Dat trang thai (tt: Trang thai moi)
    void setTrangThai(TrangThaiDonHang tt);
    
    // Dat ghi chu (gc: Ghi chu moi)
    void setGhiChu(const std::string &gc);

    // Dat ngay tao - dung cho load du lieu (nt: Ngay tao moi)
    void setNgayTao(const NgayGio &nt);

    // ========== METHODS ==========
    
    // Them dich vu vao don hang (dv: Dich vu da dat)
    void themDichVu(const DichVuDat &dv);
    
    // Xoa dich vu khoi don hang (index: Vi tri dich vu can xoa)
    void xoaDichVu(int index);
    
    // Tinh tong tien dich vu
    void tinhTongTien();
    
    // Tinh giam gia theo membership
    void tinhGiamGia();
    
    // Tinh thanh tien = tong - giam gia
    void tinhThanhTien();
    
    // Hien thi thong tin don hang
    void hienThi() const;
    
    // Lay ten trang thai dang chuoi
    std::string getTrangThaiText() const;

    // ========== HELPER ==========
    
    // Lay ma khach hang hoac "GUEST" neu khach vang lai
    std::string getMaKhachHang() const;

    // ========== STATIC ==========
    
    // Tao ma don hang tu dong (DH + timestamp)
    static std::string taoMaDonHang();
};

#endif // DONHANGDICHVU_H
