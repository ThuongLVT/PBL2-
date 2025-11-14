/**
 * @file QuanLyDatSan.h
 * @brief Lớp quản lý đặt sân
 * @details Tách logic quản lý đặt sân từ HeThongQuanLy
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#ifndef QUANLYDATSAN_H
#define QUANLYDATSAN_H

#include "../Models/DatSan.h"
#include "../Models/KhachHang.h"
#include "../Models/San.h"
#include "../CauTrucDuLieu/MangDong.h"
#include "../CauTrucDuLieu/NgayGio.h"
#include "../CauTrucDuLieu/NgayThang.h"
#include "../Models/KhungGio.h"
#include <string>

/**
 * @class QuanLyDatSan
 * @brief Lớp quản lý đặt sân
 *
 * Chức năng:
 * - Tạo đặt sân mới
 * - Hủy đặt sân
 * - Cập nhật trạng thái
 * - Tìm kiếm đặt sân
 * - Kiểm tra trùng lịch
 */
class QuanLyDatSan
{
private:
    MangDong<DatSan *> danhSachDatSan; ///< Danh sách đặt sân

public:
    // ========== CONSTRUCTORS ==========
    QuanLyDatSan();
    ~QuanLyDatSan();

    // ========== CRUD OPERATIONS ==========
    DatSan *taoDatSan(KhachHang *kh, San *san, const NgayGio &thoiGian, const KhungGio &khung);
    bool huyDatSan(const std::string &maDatSan);
    bool capNhatTrangThaiDatSan(const std::string &maDatSan, TrangThaiDatSan trangThai);
    DatSan *timDatSan(const std::string &maDatSan);
    const MangDong<DatSan *> &layDanhSachDatSan() const;

    // ========== SEARCH ==========
    MangDong<DatSan *> timDatSanTheoKhachHang(const std::string &maKH);
    MangDong<DatSan *> timDatSanTheoSan(const std::string &maSan);
    MangDong<DatSan *> timDatSanTheoNgay(const NgayThang &ngay);
    MangDong<DatSan *> timDatSanTheoTrangThai(TrangThaiDatSan trangThai);

    // ========== VALIDATION ==========
    bool kiemTraTrungLich(San *san, const NgayGio &thoiGian, const KhungGio &khung);

    // ========== DISPLAY ==========
    void hienThiDanhSachDatSan() const;

    // ========== STATISTICS ==========
    int tongSoDatSan() const;

    // ========== FILE I/O ==========
    bool ghiFile(std::ofstream &file) const;
    bool docFile(std::ifstream &file);
    void xoaTatCa();
};

#endif // QUANLYDATSAN_H
