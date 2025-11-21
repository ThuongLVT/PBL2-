/**
 * @file KhachHang.h
 * @brief Lớp đại diện cho Khách hàng
 * @details Kế thừa từ ConNguoi, không cần tài khoản đăng nhập
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#ifndef KHACHHANG_H
#define KHACHHANG_H
#include <string>

#include "ConNguoi.h"
#include "../CauTrucDuLieu/NgayThang.h"

/**
 * @enum HangKhachHang
 * @brief Hạng khách hàng dựa trên tổng chi tiêu
 */
enum class HangKhachHang
{
    THUONG,   ///< Thường (< 1.000.000 VND) - Giảm 0%
    DONG,     ///< Đồng (1.000.000 - 4.999.999 VND) - Giảm 2%
    BAC,      ///< Bạc (5.000.000 - 9.999.999 VND) - Giảm 4%
    VANG,     ///< Vàng (10.000.000 - 19.999.999 VND) - Giảm 6%
    KIM_CUONG ///< Kim cương (>= 20.000.000 VND) - Giảm 10%
};

/**
 * @class KhachHang
 * @brief Lớp đại diện cho Khách hàng
 *
 * Khách hàng:
 * - Không cần tài khoản đăng nhập
 * - Có mã khách hàng riêng
 * - Tích lũy điểm qua mỗi lần đặt sân
 * - Được hưởng ưu đãi theo hạng
 */
class KhachHang : public ConNguoi
{
private:
    std::string maKhachHang; ///< Mã khách hàng (VD: KH001)
    double tongChiTieu;      ///< Tổng chi tiêu (VND)
    HangKhachHang hang;      ///< Hạng khách hàng
    NgayThang ngayDangKy;    ///< Ngày đăng ký thành viên
    int soLanDatSan;         ///< Tổng số lần đặt sân

public:
    // ========== CONSTRUCTORS ==========

    /**
     * @brief Constructor mặc định
     */
    KhachHang();

    /**
     * @brief Constructor có tham số
     * @param ht Họ tên
     * @param sdt Số điện thoại
     * @param dc Địa chỉ
     * @param maKH Mã khách hàng
     */
    KhachHang(const std::string &ht, const std::string &sdt, const std::string &dc,
              const std::string &maKH);

    /**
     * @brief Copy constructor
     * @param other Đối tượng KhachHang khác
     */
    KhachHang(const KhachHang &other);

    /**
     * @brief Destructor
     */
    virtual ~KhachHang();

    // ========== GETTERS ==========

    // Inherited from base classes - forward declaration
    std::string getMaNguoiDung() const; // Alias for compatibility

    /**
     * @brief Lấy mã khách hàng
     * @return Mã khách hàng
     */
    std::string layMaKhachHang() const;

    /**
     * @brief Lấy tổng chi tiêu
     * @return Tổng chi tiêu (VND)
     */
    double layTongChiTieu() const;

    /**
     * @brief Lấy hạng khách hàng
     * @return Hạng khách hàng
     */
    HangKhachHang layHang() const;

    /**
     * @brief Lấy ngày đăng ký
     * @return Ngày đăng ký
     */
    NgayThang layNgayDangKy() const;

    /**
     * @brief Lấy số lần đặt sân
     * @return Số lần đặt sân
     */
    int laySoLanDatSan() const;

    /**
     * @brief Lấy tên hạng khách hàng
     * @return Tên hạng dạng chuỗi
     */
    std::string layTenHang() const;

    /**
     * @brief Lấy phần trăm giảm giá theo hạng
     * @return Phần trăm giảm giá (0-15)
     */
    int layPhanTramGiamGia() const;

    // ========== SETTERS ==========

    /**
     * @brief Đặt mã khách hàng
     * @param maKH Mã khách hàng mới
     */
    void datMaKhachHang(const std::string &maKH);

    /**
     * @brief Đặt ngày đăng ký
     * @param ngay Ngày đăng ký mới
     */
    void datNgayDangKy(const NgayThang &ngay);

    /**
     * @brief Đặt tổng chi tiêu (dùng cho CSV import)
     * @param tien Tổng chi tiêu
     */
    void datTongChiTieu(double tien);

    // ========== METHODS ==========

    /**
     * @brief Thêm chi tiêu (sau mỗi lần đặt sân)
     * @param soTien Số tiền chi tiêu
     */
    void themChiTieu(double soTien);

    /**
     * @brief Tăng số lần đặt sân
     */
    void tangSoLanDatSan();

    /**
     * @brief Cập nhật hạng khách hàng dựa trên tổng chi tiêu
     */
    void capNhatHang();

    /**
     * @brief Hiển thị thông tin khách hàng (override)
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
     * @param other Đối tượng KhachHang khác
     * @return Tham chiếu đến đối tượng hiện tại
     */
    KhachHang &operator=(const KhachHang &other);

    /**
     * @brief Toán tử so sánh (theo mã khách hàng)
     * @param other Đối tượng KhachHang khác
     * @return true nếu mã khách hàng giống nhau
     */
    bool operator==(const KhachHang &other) const;

    /**
     * @brief Toán tử xuất ra stream
     * @param os Output stream
     * @param kh Đối tượng KhachHang
     * @return Tham chiếu đến output stream
     */
    friend std::ostream &operator<<(std::ostream &os, const KhachHang &kh);
};

#endif // KHACHHANG_H
