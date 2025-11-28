/**
 * @file QuanLyDonHangDichVu.h
 * @brief Lớp quản lý đơn hàng dịch vụ
 * @details Quản lý các đơn hàng dịch vụ riêng (không liên quan đến đặt sân)
 *
 * @author khninh22 - Service Module
 * @date 2025-11-14
 */

#ifndef QUANLYDONHANGDICHVU_H
#define QUANLYDONHANGDICHVU_H

#include "../Models/DonHangDichVu.h"
#include "../Models/KhachHang.h"
#include "../CauTrucDuLieu/MangDong.h"
#include "../CauTrucDuLieu/NgayGio.h"
#include <string>
#include <fstream>

/**
 * @class QuanLyDonHangDichVu
 * @brief Lớp quản lý đơn hàng dịch vụ
 *
 * Chức năng:
 * - CRUD đơn hàng dịch vụ
 * - Tìm kiếm theo khách hàng, ngày
 * - Thống kê doanh thu
 */
class QuanLyDonHangDichVu
{
private:
    MangDong<DonHangDichVu *> danhSachDonHang; ///< Danh sách đơn hàng
    int maxOrderId;                            ///< ID đơn hàng lớn nhất (auto-increment)

public:
    // ========== CONSTRUCTORS ==========

    /**
     * @brief Constructor mặc định
     */
    QuanLyDonHangDichVu();

    /**
     * @brief Destructor
     */
    ~QuanLyDonHangDichVu();

    // ========== CRUD OPERATIONS ==========

    /**
     * @brief Tạo mã đơn hàng mới (DHD001, DHD002...)
     * @return Mã đơn hàng mới
     */
    std::string taoMaDonHangMoi();

    /**
     * @brief Tạo đơn hàng mới
     * @param kh Khách hàng (có thể NULL cho khách vãng lai)
     * @return Pointer tới đơn hàng mới tạo
     */
    DonHangDichVu *taoDonHang(KhachHang *kh);

    /**
     * @brief Hủy đơn hàng
     * @param maDH Mã đơn hàng
     * @return true nếu hủy thành công
     */
    bool huyDonHang(const std::string &maDH);

    /**
     * @brief Cập nhật trạng thái đơn hàng
     * @param maDH Mã đơn hàng
     * @param trangThai Trạng thái mới
     * @return true nếu cập nhật thành công
     */
    bool capNhatTrangThai(const std::string &maDH, TrangThaiDonHang trangThai);

    /**
     * @brief Tìm đơn hàng theo mã
     * @param maDH Mã đơn hàng
     * @return Pointer tới đơn hàng hoặc nullptr
     */
    DonHangDichVu *timDonHang(const std::string &maDH);

    /**
     * @brief Lấy danh sách tất cả đơn hàng
     * @return Danh sách đơn hàng
     */
    const MangDong<DonHangDichVu *> &layDanhSachDonHang() const;

    // ========== SEARCH ==========

    /**
     * @brief Tìm đơn hàng theo khách hàng
     * @param maKH Mã khách hàng
     * @return Danh sách đơn hàng của khách
     */
    MangDong<DonHangDichVu *> timDonHangTheoKhachHang(const std::string &maKH);

    /**
     * @brief Tìm đơn hàng theo ngày
     * @param ngay Ngày cần tìm
     * @return Danh sách đơn hàng trong ngày
     */
    MangDong<DonHangDichVu *> timDonHangTheoNgay(const NgayThang &ngay);

    /**
     * @brief Tìm đơn hàng theo trạng thái
     * @param trangThai Trạng thái cần tìm
     * @return Danh sách đơn hàng có trạng thái
     */
    MangDong<DonHangDichVu *> timDonHangTheoTrangThai(TrangThaiDonHang trangThai);

    // ========== DISPLAY ==========

    /**
     * @brief Hiển thị danh sách đơn hàng
     */
    void hienThiDanhSachDonHang() const;

    // ========== STATISTICS ==========

    /**
     * @brief Tổng số đơn hàng
     * @return Số lượng đơn hàng
     */
    int tongSoDonHang() const;

    /**
     * @brief Tính tổng doanh thu
     * @return Tổng doanh thu từ tất cả đơn hàng
     */
    double tongDoanhThu() const;

    /**
     * @brief Tính doanh thu theo ngày
     * @param ngay Ngày cần tính
     * @return Doanh thu trong ngày
     */
    double doanhThuTheoNgay(const NgayThang &ngay) const;

    // ========== FILE I/O ==========

    /**
     * @brief Ghi danh sách ra file
     * @param file File stream để ghi
     * @return true nếu ghi thành công
     */
    bool ghiFile(std::ofstream &file) const;

    /**
     * @brief Đọc danh sách từ file
     * @param file File stream để đọc
     * @return true nếu đọc thành công
     */
    bool docFile(std::ifstream &file);

    /**
     * @brief Lưu đơn hàng dịch vụ ra file CSV
     * @param filePath Đường dẫn file CSV
     * @return true nếu lưu thành công
     */
    bool luuCSV(const std::string &filePath) const;

    /**
     * @brief Đọc đơn hàng dịch vụ từ file CSV
     * @param filePath Đường dẫn file CSV
     * @return true nếu đọc thành công
     */
    bool docCSV(const std::string &filePath);

    /**
     * @brief Xóa tất cả đơn hàng
     */
    void xoaTatCa();
};

#endif // QUANLYDONHANGDICHVU_H
