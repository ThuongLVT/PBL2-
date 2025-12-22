// Lop quan ly san bong - CRUD, tim kiem, kiem tra san trong, quan ly lich san

#ifndef QUANLYSAN_H
#define QUANLYSAN_H

#include "../Models/San.h"
#include "../CauTrucDuLieu/MangDong.h"
#include "../CauTrucDuLieu/NgayGio.h"
#include "../Models/KhungGio.h"
#include <string>

class QuanLySan
{
private:
    MangDong<San *> danhSachSan;
    std::string csvFilePath;

    // Parse string thanh enum
    LoaiSan parseLoaiSan(const std::string &str) const;
    KhuVuc parseKhuVuc(const std::string &str) const;
    TrangThaiSan parseTrangThai(const std::string &str) const;

public:
    // ========== CONSTRUCTORS ==========
    QuanLySan();
    ~QuanLySan();

    // ========== CRUD OPERATIONS ==========
    bool themSan(San *san);
    bool xoaSan(const std::string &maSan);
    bool capNhatSan(const std::string &maSan, const San &sanMoi);
    San *timSan(const std::string &maSan);
    const MangDong<San *> &layDanhSachSan() const;

    // ========== SEARCH & FILTER ==========
    MangDong<San *> timSanTheoLoai(const std::string &loai);
    MangDong<San *> timSanTheoTen(const std::string &ten);
    MangDong<San *> laySanDangHoatDong();
    MangDong<San *> laySanBaoTri();
    MangDong<San *> laySanNgungHoatDong();

    // ========== AVAILABILITY CHECK ==========
    bool kiemTraSanTrong(San *san, const NgayGio &thoiGian, const KhungGio &khungGio);
    MangDong<San *> timSanTrong(const NgayGio &thoiGian, const KhungGio &khungGio);

    // ========== STATUS MANAGEMENT ==========
    bool datTrangThaiSan(const std::string &maSan, TrangThaiSan trangThai);
    bool chuyenSangBaoTri(const std::string &maSan);
    bool chuyenVeHoatDong(const std::string &maSan);

    // ========== DISPLAY ==========
    void hienThiDanhSachSan() const;
    void hienThiThongTinSan(const std::string &maSan) const;

    // ========== STATISTICS ==========
    int tongSoSan() const;
    int demSanHoatDong() const;
    int demSanNgungHoatDong() const;
    int demSanBaoTri() const;

    // ========== VALIDATION ==========
    bool kiemTraSanCoLichDat(const std::string &maSan) const;

    // ========== AUTO GENERATION ==========
    std::string taoMaSanTuDong(KhuVuc khuVuc, LoaiSan loaiSan); // Format: <Khu><Loai><STT> (VD: A501, B702)

    // ========== FILE I/O ==========
    bool loadFromCSV(const std::string &filePath = "");
    bool saveToCSV();
    void xoaTatCa();
};

#endif // QUANLYSAN_H
