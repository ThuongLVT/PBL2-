// Lop quan ly khach hang - CRUD, tim kiem theo ma/SDT/ten, quan ly diem tich luy

#ifndef QUANLYKHACHHANG_H
#define QUANLYKHACHHANG_H

#include "../Models/KhachHang.h"
#include "../CauTrucDuLieu/MangDong.h"
#include "../ThuatToan/HashTable.h"
#include <string>

class QuanLyKhachHang
{
private:
    MangDong<KhachHang *> danhSachKhachHang;
    HashTable<std::string, KhachHang *> *hashTableKhachHang; // Hash Table tim kiem nhanh theo ma
    int maxCustomerId;     // Ma KH cao nhat (khong tai su dung)
    bool isLoadingFromCSV; // Flag tranh auto-save khi load CSV

public:
    // ========== CONSTRUCTORS ==========
    QuanLyKhachHang();
    ~QuanLyKhachHang();

    // ========== CRUD OPERATIONS ==========
    bool themKhachHang(KhachHang *kh);
    bool xoaKhachHang(const std::string &maKH);
    bool capNhatKhachHang(const std::string &maKH, const KhachHang &khMoi);
    KhachHang *timKhachHang(const std::string &maKH);
    const MangDong<KhachHang *> &layDanhSachKhachHang() const;

    // ========== SEARCH ==========
    KhachHang *timKhachHangTheoSDT(const std::string &sdt);
    MangDong<KhachHang *> timKhachHangTheoTen(const std::string &ten);

    // ========== DISPLAY ==========
    void hienThiDanhSachKhachHang() const;
    void hienThiThongTinKhachHang(const std::string &maKH) const;

    // ========== STATISTICS ==========
    int tongSoKhachHang() const;

    // ========== ID GENERATION ==========
    std::string taoMaKhachHangMoi();                                                    // Tao ma KH moi (KH001, KH002...)
    bool kiemTraSDTTonTai(const std::string &sdt, const std::string &excludeMaKH = "") const; // Kiem tra SDT trung

    // ========== CSV I/O ==========
    bool luuCSV(const std::string &filename) const;
    bool docCSV(const std::string &filename);
    void xoaTatCa();
};

#endif // QUANLYKHACHHANG_H
