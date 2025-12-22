// Lop quan ly dich vu - CRUD, quan ly ton kho, tim kiem theo loai

#ifndef QUANLYDICHVU_H
#define QUANLYDICHVU_H

#include "../Models/DichVu.h"
#include "../CauTrucDuLieu/MangDong.h"
#include <string>

class QuanLyDichVu
{
private:
    MangDong<DichVu *> danhSachDichVu;
    int maxServiceId; // ID lon nhat (auto-increment)

public:
    // ========== CONSTRUCTORS ==========
    QuanLyDichVu();
    ~QuanLyDichVu();

    // ========== CRUD OPERATIONS ==========
    bool themDichVu(DichVu *dv);
    std::string taoMaDichVuMoi();
    bool xoaDichVu(const std::string &maDV);
    bool capNhatDichVu(const std::string &maDV, const DichVu &dvMoi);
    DichVu *timDichVu(const std::string &maDV);
    const MangDong<DichVu *> &layDanhSachDichVu() const;

    // ========== SEARCH ==========
    MangDong<DichVu *> timDichVuTheoLoai(LoaiDichVu loai);
    MangDong<DichVu *> timDichVuConHang();

    // ========== DISPLAY ==========
    void hienThiDanhSachDichVu() const;

    // ========== STATISTICS ==========
    int tongSoDichVu() const;

    // ========== DATA MANAGEMENT ==========
    void xoaTatCa();
    bool taiDuLieuTuCSV(const std::string &filePath);
    bool luuDuLieuRaCSV(const std::string &filePath) const;
};

#endif // QUANLYDICHVU_H
