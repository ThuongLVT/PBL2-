/**
 * @file QuanTriVien.h
 * @brief Lớp đại diện cho Quản trị viên hệ thống
 * @details Kế thừa từ NguoiDung, có quyền cao nhất trong hệ thống
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#ifndef QUANTRIVIEN_H
#define QUANTRIVIEN_H
#include <string>

#include "NguoiDung.h"
#include "../CauTrucDuLieu/NgayGio.h"

/**
 * @class QuanTriVien
 * @brief Lớp đại diện cho Quản trị viên
 *
 * Quản trị viên có quyền:
 * - Quản lý tất cả người dùng
 * - Xem tất cả báo cáo thống kê
 * - Cấu hình hệ thống
 * - Sao lưu/khôi phục dữ liệu
 */
class QuanTriVien : public NguoiDung
{
private:
    NgayGio ngayTao;         ///< Ngày tạo tài khoản admin
    int soLanDangNhap;       ///< Số lần đăng nhập
    NgayGio lanDangNhapCuoi; ///< Lần đăng nhập cuối cùng

public:
    // ========== CONSTRUCTORS ==========

    /**
     * @brief Constructor mặc định
     */
    QuanTriVien();

    /**
     * @brief Constructor có tham số
     * @param ht Họ tên
     * @param sdt Số điện thoại
     * @param dc Địa chỉ
     * @param tdn Tên đăng nhập
     * @param mk Mật khẩu
     */
    QuanTriVien(const std::string &ht, const std::string &sdt, const std::string &dc,
                const std::string &tdn, const std::string &mk);

    /**
     * @brief Copy constructor
     * @param other Đối tượng QuanTriVien khác
     */
    QuanTriVien(const QuanTriVien &other);

    /**
     * @brief Destructor
     */
    virtual ~QuanTriVien();

    // ========== GETTERS ==========

    /**
     * @brief Lấy ngày tạo tài khoản
     * @return Ngày giờ tạo
     */
    NgayGio layNgayTao() const;

    /**
     * @brief Lấy số lần đăng nhập
     * @return Số lần đăng nhập
     */
    int laySoLanDangNhap() const;

    /**
     * @brief Lấy lần đăng nhập cuối
     * @return Ngày giờ đăng nhập cuối
     */
    NgayGio layLanDangNhapCuoi() const;

    // ========== METHODS ==========

    /**
     * @brief Cập nhật lần đăng nhập (gọi khi đăng nhập thành công)
     */
    void capNhatDangNhap();

    /**
     * @brief Hiển thị thông tin quản trị viên (override)
     */
    virtual void hienThiThongTin() const override;

    /**
     * @brief Ghi thông tin ra file nhị phân (override)
     * @param file File stream để ghi
     * @return true nếu ghi thành công
     */
    virtual bool ghiFile(std::ofstream &file) const override;

    /**
     * @brief Đọc thông tin từ file nhị phân (override)
     * @param file File stream để đọc
     * @return true nếu đọc thành công
     */
    virtual bool docFile(std::ifstream &file) override;

    // ========== OPERATORS ==========

    /**
     * @brief Toán tử gán
     * @param other Đối tượng QuanTriVien khác
     * @return Tham chiếu đến đối tượng hiện tại
     */
    QuanTriVien &operator=(const QuanTriVien &other);

    /**
     * @brief Toán tử xuất ra stream
     * @param os Output stream
     * @param qtv Đối tượng QuanTriVien
     * @return Tham chiếu đến output stream
     */
    friend std::ostream &operator<<(std::ostream &os, const QuanTriVien &qtv);
};

#endif // QUANTRIVIEN_H
