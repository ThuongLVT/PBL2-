/**
 * @file QuanLySan.h
 * @brief Lớp quản lý sân bóng
 * @details Tách logic quản lý sân từ HeThongQuanLy
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#ifndef QUANLYSAN_H
#define QUANLYSAN_H

#include "../Models/San.h"
#include "../CauTrucDuLieu/MangDong.h"
#include "../CauTrucDuLieu/NgayGio.h"
#include "../Models/KhungGio.h"
#include <string>

/**
 * @class QuanLySan
 * @brief Lớp quản lý sân bóng
 *
 * Chức năng:
 * - CRUD sân
 * - Tìm kiếm sân theo loại, trạng thái
 * - Kiểm tra sân trống
 * - Quản lý lịch sân
 */
class QuanLySan
{
private:
    MangDong<San *> danhSachSan; ///< Danh sách sân
    std::string csvFilePath;     ///< Đường dẫn file CSV

    /**
     * @brief Parse string thành LoaiSan enum
     */
    LoaiSan parseLoaiSan(const std::string &str) const;

    /**
     * @brief Parse string thành KhuVuc enum
     */
    KhuVuc parseKhuVuc(const std::string &str) const;

    /**
     * @brief Parse string thành TrangThaiSan enum
     */
    TrangThaiSan parseTrangThai(const std::string &str) const;

public:
    // ========== CONSTRUCTORS ==========

    /**
     * @brief Constructor mặc định
     */
    QuanLySan();

    /**
     * @brief Destructor
     */
    ~QuanLySan();

    // ========== CRUD OPERATIONS ==========

    /**
     * @brief Thêm sân mới
     * @param san Con trỏ đến sân cần thêm
     * @return true nếu thêm thành công
     */
    bool themSan(San *san);

    /**
     * @brief Xóa sân theo mã
     * @param maSan Mã sân cần xóa
     * @return true nếu xóa thành công
     */
    bool xoaSan(const std::string &maSan);

    /**
     * @brief Cập nhật thông tin sân
     * @param maSan Mã sân cần cập nhật
     * @param sanMoi Thông tin sân mới
     * @return true nếu cập nhật thành công
     */
    bool capNhatSan(const std::string &maSan, const San &sanMoi);

    /**
     * @brief Tìm sân theo mã
     * @param maSan Mã sân cần tìm
     * @return Con trỏ đến sân (nullptr nếu không tìm thấy)
     */
    San *timSan(const std::string &maSan);

    /**
     * @brief Lấy danh sách tất cả sân
     * @return Danh sách sân
     */
    const MangDong<San *> &layDanhSachSan() const;

    // ========== SEARCH & FILTER ==========

    /**
     * @brief Tìm sân theo loại
     * @param loai Loại sân (5, 7 người)
     * @return Danh sách sân tìm được
     */
    MangDong<San *> timSanTheoLoai(const std::string &loai);

    /**
     * @brief Tìm sân theo tên (tìm gần đúng)
     * @param ten Tên sân
     * @return Danh sách sân tìm được
     */
    MangDong<San *> timSanTheoTen(const std::string &ten);

    /**
     * @brief Lấy danh sách sân đang hoạt động
     * @return Danh sách sân hoạt động
     */
    MangDong<San *> laySanDangHoatDong();

    /**
     * @brief Lấy danh sách sân đang bảo trì
     * @return Danh sách sân bảo trì
     */
    MangDong<San *> laySanBaoTri();

    /**
     * @brief Lấy danh sách sân ngừng hoạt động
     * @return Danh sách sân ngừng hoạt động
     */
    MangDong<San *> laySanNgungHoatDong();

    // ========== AVAILABILITY CHECK ==========

    /**
     * @brief Kiểm tra sân có trống không (dùng để kiểm tra với đặt sân)
     * @param san Con trỏ đến sân
     * @param thoiGian Thời gian cần kiểm tra
     * @param khungGio Khung giờ cần kiểm tra
     * @return true nếu sân trống
     * @note Cần tích hợp với QuanLyDatSan để kiểm tra lịch
     */
    bool kiemTraSanTrong(San *san, const NgayGio &thoiGian, const KhungGio &khungGio);

    /**
     * @brief Tìm các sân trống tại thời điểm
     * @param thoiGian Thời gian cần tìm
     * @param khungGio Khung giờ cần tìm
     * @return Danh sách sân trống
     * @note Cần tích hợp với QuanLyDatSan
     */
    MangDong<San *> timSanTrong(const NgayGio &thoiGian, const KhungGio &khungGio);

    // ========== STATUS MANAGEMENT ==========

    /**
     * @brief Đặt trạng thái sân (Hoạt động/Bảo trì/Tạm ngưng)
     * @param maSan Mã sân
     * @param trangThai Trạng thái mới
     * @return true nếu cập nhật thành công
     */
    bool datTrangThaiSan(const std::string &maSan, TrangThaiSan trangThai);

    /**
     * @brief Chuyển sân sang bảo trì
     * @param maSan Mã sân
     * @return true nếu thành công
     */
    bool chuyenSangBaoTri(const std::string &maSan);

    /**
     * @brief Chuyển sân về hoạt động
     * @param maSan Mã sân
     * @return true nếu thành công
     */
    bool chuyenVeHoatDong(const std::string &maSan);

    // ========== DISPLAY ==========

    /**
     * @brief Hiển thị danh sách tất cả sân
     */
    void hienThiDanhSachSan() const;

    /**
     * @brief Hiển thị thông tin chi tiết một sân
     * @param maSan Mã sân
     */
    void hienThiThongTinSan(const std::string &maSan) const;

    // ========== STATISTICS ==========

    /**
     * @brief Đếm tổng số sân
     * @return Tổng số sân
     */
    int tongSoSan() const;

    /**
     * @brief Đếm số sân đang hoạt động
     * @return Số sân hoạt động
     */
    int demSanHoatDong() const;

    /**
     * @brief Đếm số sân ngừng hoạt động
     * @return Số sân ngừng hoạt động
     */
    int demSanNgungHoatDong() const;

    /**
     * @brief Đếm số sân đang bảo trì
     * @return Số sân bảo trì
     */
    int demSanBaoTri() const;

    // ========== VALIDATION ==========

    /**
     * @brief Kiểm tra sân có lịch đặt trong tương lai không
     * @param maSan Mã sân
     * @return true nếu có lịch đặt
     */
    bool kiemTraSanCoLichDat(const std::string &maSan) const;

    // ========== AUTO GENERATION ==========

    /**
     * @brief Tạo mã sân tự động theo format <Khu><Loại><STT>
     * @param khuVuc Khu vực (A, B, C, D)
     * @param loaiSan Loại sân (5, 7)
     * @return Mã sân mới (VD: A501, B702)
     */
    std::string taoMaSanTuDong(KhuVuc khuVuc, LoaiSan loaiSan);

    // ========== FILE I/O ==========

    /**
     * @brief Load dữ liệu từ file CSV
     * @param filePath Đường dẫn file CSV (optional, mặc định Data/san.csv)
     * @return true nếu thành công
     */
    bool loadFromCSV(const std::string &filePath = "");

    /**
     * @brief Lưu dữ liệu vào file CSV
     * @return true nếu thành công
     */
    bool saveToCSV();

    /**
     * @brief Ghi danh sách sân ra file
     * @param file File stream
     * @return true nếu ghi thành công
     */
    bool ghiFile(std::ofstream &file) const;

    /**
     * @brief Đọc danh sách sân từ file
     * @param file File stream
     * @return true nếu đọc thành công
     */
    bool docFile(std::ifstream &file);

    /**
     * @brief Xóa tất cả dữ liệu
     */
    void xoaTatCa();
};

#endif // QUANLYSAN_H
