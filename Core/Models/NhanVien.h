/**
 * @file NhanVien.h
 * @brief Lớp đại diện cho Nhân viên hệ thống
 * @details Kế thừa từ NguoiDung, quản lý công việc hàng ngày
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#ifndef NHANVIEN_H
#define NHANVIEN_H
#include <string>

#include "NguoiDung.h"
#include "../CauTrucDuLieu/NgayThang.h"
#include "../CauTrucDuLieu/ThoiGian.h"

/**
 * @enum CaLamViec
 * @brief Các ca làm việc
 */
enum class CaLamViec
{
    SANG,  ///< Ca sáng (6h-14h)
    CHIEU, ///< Ca chiều (14h-22h)
    TOI    ///< Ca tối (18h-02h)
};

/**
 * @class NhanVien
 * @brief Lớp đại diện cho Nhân viên
 *
 * Nhân viên có quyền:
 * - Quản lý đặt sân
 * - Quản lý khách hàng
 * - Quản lý dịch vụ
 * - Xử lý thanh toán
 */
class NhanVien : public NguoiDung
{
private:
    std::string maNhanVien; ///< Mã nhân viên
    NgayThang ngayVaoLam; ///< Ngày vào làm
    double luongCoBan;    ///< Lương cơ bản
    CaLamViec caLamViec;  ///< Ca làm việc chính
    int soNgayLam;        ///< Tổng số ngày đã làm

public:
    // ========== CONSTRUCTORS ==========

    /**
     * @brief Constructor mặc định
     */
    NhanVien();

    /**
     * @brief Constructor có tham số
     * @param ht Họ tên
     * @param sdt Số điện thoại
     * @param dc Địa chỉ
     * @param tdn Tên đăng nhập
     * @param mk Mật khẩu
     * @param maNV Mã nhân viên
     * @param luong Lương cơ bản
     * @param ca Ca làm việc
     */
    NhanVien(const std::string &ht, const std::string &sdt, const std::string &dc,
             const std::string &tdn, const std::string &mk, const std::string &maNV,
             double luong, CaLamViec ca);

    /**
     * @brief Copy constructor
     * @param other Đối tượng NhanVien khác
     */
    NhanVien(const NhanVien &other);

    /**
     * @brief Destructor
     */
    virtual ~NhanVien();

    /**
     * @brief Assignment operator
     * @param other Đối tượng NhanVien khác
     * @return Reference to this
     */
    NhanVien& operator=(const NhanVien &other);

    // ========== GETTERS ==========

    /**
     * @brief Lấy mã nhân viên
     * @return Mã nhân viên
     */
    std::string layMaNhanVien() const;

    /**
     * @brief Lấy ngày vào làm
     * @return Ngày vào làm
     */
    NgayThang layNgayVaoLam() const;

    /**
     * @brief Lấy lương cơ bản
     * @return Lương cơ bản
     */
    double layLuongCoBan() const;

    /**
     * @brief Lấy ca làm việc
     * @return Ca làm việc
     */
    CaLamViec layCaLamViec() const;

    /**
     * @brief Lấy số ngày đã làm
     * @return Số ngày làm việc
     */
    int laySoNgayLam() const;

    /**
     * @brief Lấy tên ca làm việc
     * @return Tên ca dạng chuỗi
     */
    std::string layTenCaLamViec() const;

    // ========== SETTERS ==========

    /**
     * @brief Đặt lương cơ bản
     * @param luong Lương mới
     */
    void datLuongCoBan(double luong);

    /**
     * @brief Đặt ca làm việc
     * @param ca Ca làm việc mới
     */
    void datCaLamViec(CaLamViec ca);

    // ========== METHODS ==========

    /**
     * @brief Tăng số ngày làm việc
     */
    void tangSoNgayLam();

    /**
     * @brief Tính lương theo số ngày làm
     * @return Tổng lương
     */
    double tinhLuong() const;

    /**
     * @brief Hiển thị thông tin nhân viên (override)
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
     * @brief Toán tử xuất ra stream
     * @param os Output stream
     * @param nv Đối tượng NhanVien
     * @return Tham chiếu đến output stream
     */
    friend std::ostream &operator<<(std::ostream &os, const NhanVien &nv);
};

#endif // NHANVIEN_H
