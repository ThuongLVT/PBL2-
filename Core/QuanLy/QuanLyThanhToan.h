/**
 * @file QuanLyThanhToan.h
 * @brief Lớp quản lý thanh toán
 * @details Tách logic quản lý thanh toán từ HeThongQuanLy
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#ifndef QUANLYTHANHTOAN_H
#define QUANLYTHANHTOAN_H

#include "../Models/ThanhToan.h"
#include "../Models/DatSan.h"
#include "../CauTrucDuLieu/MangDong.h"
#include <string>
#include <fstream>

/**
 * @class QuanLyThanhToan
 * @brief Lớp quản lý thanh toán
 *
 * Chức năng:
 * - Tạo thanh toán
 * - Tìm kiếm thanh toán
 * - Thống kê doanh thu
 */
class QuanLyThanhToan
{
private:
    MangDong<ThanhToan *> danhSachThanhToan; ///< Danh sách thanh toán

public:
    // ========== CONSTRUCTORS ==========
    QuanLyThanhToan();
    ~QuanLyThanhToan();

    // ========== CRUD OPERATIONS ==========
    ThanhToan *taoThanhToan(DatSan *datSan, PhuongThucThanhToan phuongThuc);
    bool xoaThanhToan(const std::string &maTT);
    ThanhToan *timThanhToan(const std::string &maTT);
    ThanhToan *timThanhToanTheoDatSan(const std::string &maDatSan);
    const MangDong<ThanhToan *> &layDanhSachThanhToan() const;

    // ========== DISPLAY ==========
    void hienThiDanhSachThanhToan() const;

    // ========== STATISTICS ==========
    int tongSoThanhToan() const;
    double tongDoanhThu() const;

    // ========== FILE I/O ==========
    // TODO Week 1: Convert to std::ofstream/ifstream or implement differently
    // bool ghiFile(std::ofstream &file) const;
    // bool docFile(std::ifstream &file);
    void xoaTatCa();
};

#endif // QUANLYTHANHTOAN_H
