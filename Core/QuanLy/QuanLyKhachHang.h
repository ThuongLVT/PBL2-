/**
 * @file QuanLyKhachHang.h
 * @brief Lớp quản lý khách hàng
 * @details Tách logic quản lý khách hàng từ HeThongQuanLy
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#ifndef QUANLYKHACHHANG_H
#define QUANLYKHACHHANG_H

#include "../Models/KhachHang.h"
#include "../CauTrucDuLieu/MangDong.h"
#include <string>

/**
 * @class QuanLyKhachHang
 * @brief Lớp quản lý khách hàng
 *
 * Chức năng:
 * - CRUD khách hàng
 * - Tìm kiếm theo mã, SĐT, tên
 * - Quản lý điểm tích lũy
 */
class QuanLyKhachHang
{
private:
    MangDong<KhachHang *> danhSachKhachHang; ///< Danh sách khách hàng
    int maxCustomerId;                       ///< Mã KH cao nhất đã sử dụng (không tái sử dụng)

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
    /**
     * @brief Tạo mã khách hàng mới duy nhất (KH001, KH002, ...)
     * @return Mã khách hàng mới
     */
    std::string taoMaKhachHangMoi();

    /**
     * @brief Kiểm tra số điện thoại đã tồn tại chưa
     * @param sdt Số điện thoại cần kiểm tra
     * @param excludeMaKH Loại trừ mã KH này (dùng cho cập nhật)
     * @return true nếu SĐT đã tồn tại
     */
    bool kiemTraSDTTonTai(const std::string &sdt, const std::string &excludeMaKH = "") const;

    // ========== FILE I/O ==========
    bool ghiFile(std::ofstream &file) const;
    bool docFile(std::ifstream &file);

    // ========== CSV I/O ==========
    /**
     * @brief Lưu danh sách khách hàng ra file CSV
     * @param filename Đường dẫn file CSV
     * @return true nếu thành công
     */
    bool luuCSV(const std::string &filename) const;

    /**
     * @brief Đọc danh sách khách hàng từ file CSV
     * @param filename Đường dẫn file CSV
     * @return true nếu thành công
     */
    bool docCSV(const std::string &filename);

    void xoaTatCa();
};

#endif // QUANLYKHACHHANG_H
