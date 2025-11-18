/**
 * @file NguoiDung.h
 * @brief Lớp đại diện cho người dùng hệ thống (có tài khoản đăng nhập)
 * @details Kế thừa từ ConNguoi, thêm thông tin đăng nhập và vai trò
 *          Lớp cơ sở cho QuanTriVien và NhanVien
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#ifndef NGUOIDUNG_H
#define NGUOIDUNG_H
#include <string>

#include "ConNguoi.h"

/**
 * @enum VaiTro
 * @brief Các vai trò người dùng trong hệ thống
 */
enum class VaiTro
{
    QUAN_TRI_VIEN, ///< Quản trị viên (Admin)
    NHAN_VIEN      ///< Nhân viên (Staff)
};

/**
 * @class NguoiDung
 * @brief Lớp đại diện cho người dùng có tài khoản trong hệ thống
 *
 * Lớp này kế thừa từ ConNguoi và thêm:
 * - Tên đăng nhập
 * - Mật khẩu
 * - Vai trò (Admin/Staff)
 * - Trạng thái hoạt động
 */
class NguoiDung : public ConNguoi
{
protected:
    std::string tenDangNhap; ///< Tên đăng nhập duy nhất
    std::string matKhau;     ///< Mật khẩu (nên mã hóa trong thực tế)
    VaiTro vaiTro;           ///< Vai trò trong hệ thống
    bool hoatDong;           ///< Trạng thái tài khoản (true = hoạt động)
    std::string gioiTinh;    ///< Giới tính ("Nam", "Nữ", "Khác")
    std::string ngaySinh;    ///< Ngày sinh (format: DD/MM/YYYY)

public:
    // ========== CONSTRUCTORS ==========

    /**
     * @brief Constructor mặc định
     */
    NguoiDung();

    /**
     * @brief Constructor có tham số
     * @param ht Họ tên
     * @param sdt Số điện thoại
     * @param dc Địa chỉ
     * @param tdn Tên đăng nhập
     * @param mk Mật khẩu
     * @param vt Vai trò
     */
    NguoiDung(const std::string &ht, const std::string &sdt, const std::string &dc,
              const std::string &tdn, const std::string &mk, VaiTro vt);

    /**
     * @brief Copy constructor
     * @param other Đối tượng NguoiDung khác
     */
    NguoiDung(const NguoiDung &other);

    /**
     * @brief Destructor ảo
     */
    virtual ~NguoiDung();

    // ========== GETTERS ==========

    /**
     * @brief Lấy tên đăng nhập
     * @return Tên đăng nhập
     */
    std::string layTenDangNhap() const;

    /**
     * @brief Lấy vai trò
     * @return Vai trò người dùng
     */
    VaiTro layVaiTro() const;

    /**
     * @brief Kiểm tra tài khoản có hoạt động không
     * @return true nếu tài khoản đang hoạt động
     */
    bool coHoatDong() const;

    /**
     * @brief Lấy chuỗi mô tả vai trò
     * @return Tên vai trò dạng chuỗi
     */
    std::string layTenVaiTro() const;

    /**
     * @brief Lấy mật khẩu (chỉ dùng để hiển thị trong UI)
     * @return Mật khẩu
     * @warning Chỉ dùng cho mục đích hiển thị, không khuyến khích trong production
     */
    std::string layMatKhau() const;

    /**
     * @brief Lấy giới tính
     * @return Giới tính
     */
    std::string layGioiTinh() const;

    /**
     * @brief Lấy ngày sinh
     * @return Ngày sinh (DD/MM/YYYY)
     */
    std::string layNgaySinh() const;

    // ========== SETTERS ==========

    /**
     * @brief Đặt tên đăng nhập
     * @param tdn Tên đăng nhập mới
     */
    void datTenDangNhap(const std::string &tdn);

    /**
     * @brief Đặt vai trò
     * @param vt Vai trò mới
     */
    void datVaiTro(VaiTro vt);

    /**
     * @brief Đặt trạng thái hoạt động
     * @param hd Trạng thái hoạt động
     */
    void datHoatDong(bool hd);

    /**
     * @brief Đặt giới tính
     * @param gt Giới tính
     */
    void datGioiTinh(const std::string &gt);

    /**
     * @brief Đặt ngày sinh
     * @param ns Ngày sinh (DD/MM/YYYY)
     */
    void datNgaySinh(const std::string &ns);

    // ========== METHODS ==========

    /**
     * @brief Đăng nhập vào hệ thống
     * @param mk Mật khẩu nhập vào
     * @return true nếu đăng nhập thành công
     */
    bool dangNhap(const std::string &mk) const;

    /**
     * @brief Đổi mật khẩu
     * @param mkCu Mật khẩu cũ
     * @param mkMoi Mật khẩu mới
     * @return true nếu đổi thành công
     */
    bool doiMatKhau(const std::string &mkCu, const std::string &mkMoi);

    /**
     * @brief Khóa/mở khóa tài khoản
     */
    void chuyenTrangThaiHoatDong();

    /**
     * @brief Hiển thị thông tin người dùng (override)
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
     * @param other Đối tượng NguoiDung khác
     * @return Tham chiếu đến đối tượng hiện tại
     */
    NguoiDung &operator=(const NguoiDung &other);

    /**
     * @brief Toán tử xuất ra stream
     * @param os Output stream
     * @param nd Đối tượng NguoiDung
     * @return Tham chiếu đến output stream
     */
    friend std::ostream &operator<<(std::ostream &os, const NguoiDung &nd);
};

#endif // NGUOIDUNG_H
