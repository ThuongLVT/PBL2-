/**
 * @file QuanLyDichVu.h
 * @brief Lớp quản lý dịch vụ
 * @details Tách logic quản lý dịch vụ từ HeThongQuanLy
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#ifndef QUANLYDICHVU_H
#define QUANLYDICHVU_H

#include "../Models/DichVu.h"
#include "../CauTrucDuLieu/MangDong.h"
#include <string>

/**
 * @class QuanLyDichVu
 * @brief Lớp quản lý dịch vụ
 *
 * Chức năng:
 * - CRUD dịch vụ
 * - Quản lý tồn kho
 * - Tìm kiếm theo loại
 */
class QuanLyDichVu
{
private:
    MangDong<DichVu *> danhSachDichVu; ///< Danh sách dịch vụ

public:
    // ========== CONSTRUCTORS ==========
    QuanLyDichVu();
    ~QuanLyDichVu();

    // ========== CRUD OPERATIONS ==========
    bool themDichVu(DichVu *dv);
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

    // ========== FILE I/O ==========
    bool ghiFile(std::ofstream &file) const;
    bool docFile(std::ifstream &file);
    void xoaTatCa();
};

#endif // QUANLYDICHVU_H
