/**
 * @file ConNguoi.h
 * @brief Lớp cơ sở đại diện cho một con người
 * @details Chứa các thuộc tính và phương thức cơ bản của con người
 *          Lớp cơ sở cho KhachHang, NguoiDung (Admin, NhanVien)
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#ifndef CONNGUOI_H
#define CONNGUOI_H
#include <string>

#include <iostream>
#include <fstream>

/**
 * @class ConNguoi
 * @brief Lớp cơ sở đại diện cho con người trong hệ thống
 *
 * Lớp này chứa các thông tin cơ bản của một con người như:
 * - Họ tên
 * - Số điện thoại
 * - Địa chỉ
 */
class ConNguoi
{
protected:
    std::string hoTen;       ///< Họ và tên đầy đủ
    std::string soDienThoai; ///< Số điện thoại liên lạc
    std::string diaChi;      ///< Địa chỉ nơi ở

public:
    // ========== CONSTRUCTORS ==========

    /**
     * @brief Constructor mặc định
     */
    ConNguoi();

    /**
     * @brief Constructor có tham số
     * @param ht Họ tên
     * @param sdt Số điện thoại
     * @param dc Địa chỉ
     */
    ConNguoi(const std::string &ht, const std::string &sdt, const std::string &dc);

    /**
     * @brief Copy constructor
     * @param other Đối tượng ConNguoi khác
     */
    ConNguoi(const ConNguoi &other);

    /**
     * @brief Destructor ảo (để cho phép kế thừa đúng)
     */
    virtual ~ConNguoi();

    // ========== GETTERS ==========

    /**
     * @brief Lấy họ tên
     * @return Họ tên
     */
    std::string layHoTen() const;
    std::string getHoTen() const { return hoTen; } // Alias

    /**
     * @brief Lấy số điện thoại
     * @return Số điện thoại
     */
    std::string laySoDienThoai() const;
    std::string getSoDienThoai() const { return soDienThoai; } // Alias

    /**
     * @brief Lấy địa chỉ
     * @return Địa chỉ
     */
    std::string layDiaChi() const;

    // ========== SETTERS ==========

    /**
     * @brief Đặt họ tên
     * @param ht Họ tên mới
     */
    void datHoTen(const std::string &ht);

    /**
     * @brief Đặt số điện thoại
     * @param sdt Số điện thoại mới
     */
    void datSoDienThoai(const std::string &sdt);

    /**
     * @brief Đặt địa chỉ
     * @param dc Địa chỉ mới
     */
    void datDiaChi(const std::string &dc);

    // ========== METHODS ==========

    /**
     * @brief Hiển thị thông tin con người
     * @details Phương thức ảo để lớp con có thể override
     */
    virtual void hienThiThongTin() const;

    /**
     * @brief Ghi thông tin ra file nhị phân
     * @param file File stream để ghi
     * @return true nếu ghi thành công, false nếu thất bại
     */
    virtual bool ghiFile(std::ofstream &file) const;

    /**
     * @brief Đọc thông tin từ file nhị phân
     * @param file File stream để đọc
     * @return true nếu đọc thành công, false nếu thất bại
     */
    virtual bool docFile(std::ifstream &file);

    // ========== OPERATORS ==========

    /**
     * @brief Toán tử gán
     * @param other Đối tượng ConNguoi khác
     * @return Tham chiếu đến đối tượng hiện tại
     */
    ConNguoi &operator=(const ConNguoi &other);

    /**
     * @brief Toán tử so sánh bằng (so sánh theo họ tên)
     * @param other Đối tượng ConNguoi khác
     * @return true nếu họ tên giống nhau
     */
    bool operator==(const ConNguoi &other) const;

    /**
     * @brief Toán tử xuất ra stream
     * @param os Output stream
     * @param cn Đối tượng ConNguoi
     * @return Tham chiếu đến output stream
     */
    friend std::ostream &operator<<(std::ostream &os, const ConNguoi &cn);
};

#endif // CONNGUOI_H
