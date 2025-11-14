/**
 * @file San.h
 * @brief Lớp đại diện cho Sân bóng đá
 * @details Chứa thông tin về sân, giá thuê, trạng thái
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#ifndef SAN_H
#define SAN_H
#include <string>

#include <iostream>
#include <fstream>

/**
 * @enum LoaiSan
 * @brief Loại sân bóng đá
 */
enum class LoaiSan
{
    SAN_5, ///< Sân 5 người
    SAN_7  ///< Sân 7 người
};

/**
 * @enum TrangThaiSan
 * @brief Trạng thái hiện tại của sân
 */
enum class TrangThaiSan
{
    HOAT_DONG,       ///< Sân đang hoạt động (có thể đặt)
    NGUNG_HOAT_DONG, ///< Sân ngừng hoạt động
    BAO_TRI          ///< Đang bảo trì
};

/**
 * @enum KhuVuc
 * @brief Khu vực của sân
 */
enum class KhuVuc
{
    A, ///< Khu vực A
    B, ///< Khu vực B
    C, ///< Khu vực C
    D  ///< Khu vực D
};

/**
 * @class San
 * @brief Lớp đại diện cho một sân bóng đá
 *
 * Thông tin sân bao gồm:
 * - Mã sân, tên sân
 * - Loại sân (5, 7 người)
 * - Giá thuê theo giờ
 * - Trạng thái hiện tại
 * - Ghi chú
 */
class San
{
private:
    std::string maSan;      ///< Mã sân (VD: A501, B702)
    std::string tenSan;     ///< Tên sân
    LoaiSan loaiSan;        ///< Loại sân
    KhuVuc khuVuc;          ///< Khu vực sân
    double giaThueGio;      ///< Giá thuê mỗi giờ (VND)
    TrangThaiSan trangThai; ///< Trạng thái hiện tại
    std::string ghiChu;     ///< Ghi chú về sân

public:
    // ========== CONSTRUCTORS ==========

    /**
     * @brief Constructor mặc định
     */
    San();

    /**
     * @brief Constructor có tham số
     * @param maSan Mã sân
     * @param tenSan Tên sân
     * @param loaiSan Loại sân
     * @param khuVuc Khu vực sân
     * @param giaThue Giá thuê mỗi giờ
     */
    San(const std::string &maSan, const std::string &tenSan,
        LoaiSan loaiSan, KhuVuc khuVuc, double giaThue);

    /**
     * @brief Copy constructor
     * @param other Đối tượng San khác
     */
    San(const San &other);

    /**
     * @brief Destructor
     */
    ~San();

    // ========== GETTERS ==========

    /**
     * @brief Lấy mã sân
     * @return Mã sân
     */
    std::string layMaSan() const;
    std::string getMaSan() const { return maSan; } // Alias

    /**
     * @brief Lấy tên sân
     * @return Tên sân
     */
    std::string layTenSan() const;
    std::string getTenSan() const { return tenSan; } // Alias

    /**
     * @brief Lấy loại sân
     * @return Loại sân
     */
    LoaiSan layLoaiSan() const;

    /**
     * @brief Lấy khu vực sân
     * @return Khu vực sân
     */
    KhuVuc layKhuVuc() const;

    /**
     * @brief Lấy giá thuê
     * @return Giá thuê/giờ
     */
    double layGiaThueGio() const;
    double getGiaThue() const { return giaThueGio; } // Alias

    /**
     * @brief Lấy trạng thái sân
     * @return Trạng thái hiện tại
     */
    TrangThaiSan layTrangThai() const;

    /**
     * @brief Lấy ghi chú
     * @return Ghi chú
     */
    std::string layGhiChu() const;

    /**
     * @brief Lấy tên loại sân
     * @return Tên loại sân dạng chuỗi
     */
    std::string layTenLoaiSan() const;

    /**
     * @brief Lấy tên trạng thái sân
     * @return Tên trạng thái dạng chuỗi
     */
    std::string layTenTrangThai() const;

    /**
     * @brief Lấy tên khu vực sân
     * @return Tên khu vực dạng chuỗi (A, B, C, D)
     */
    std::string layTenKhuVuc() const;

    // ========== SETTERS ==========

    /**
     * @brief Đặt tên sân
     * @param ten Tên sân mới
     */
    void datTenSan(const std::string &ten);

    /**
     * @brief Đặt giá thuê
     * @param gia Giá thuê mới
     */
    void datGiaThueGio(double gia);

    /**
     * @brief Đặt trạng thái sân
     * @param tt Trạng thái mới
     */
    void datTrangThai(TrangThaiSan tt);

    /**
     * @brief Đặt ghi chú
     * @param gc Ghi chú mới
     */
    void datGhiChu(const std::string &gc);

    /**
     * @brief Đặt loại sân
     * @param loai Loại sân mới
     */
    void datLoaiSan(LoaiSan loai);

    /**
     * @brief Đặt khu vực
     * @param kv Khu vực mới
     */
    void datKhuVuc(KhuVuc kv);

    // ========== METHODS ==========

    /**
     * @brief Kiểm tra sân có đang hoạt động không
     * @return true nếu sân đang hoạt động
     */
    bool dangHoatDong() const;

    /**
     * @brief Kiểm tra sân có ngừng hoạt động không
     * @return true nếu sân ngừng hoạt động
     */
    bool ngungHoatDong() const;

    /**
     * @brief Kiểm tra sân có đang bảo trì không
     * @return true nếu sân đang bảo trì
     */
    bool dangBaoTri() const;

    /**
     * @brief Tính tiền thuê sân theo số giờ
     * @param soGio Số giờ thuê
     * @return Tổng tiền
     */
    double tinhTien(double soGio) const;

    /**
     * @brief Hiển thị thông tin sân
     */
    void hienThiThongTin() const;

    /**
     * @brief Ghi thông tin ra file nhị phân
     * @param file File stream để ghi
     * @return true nếu ghi thành công
     */
    bool ghiFile(std::ofstream &file) const;

    /**
     * @brief Đọc thông tin từ file nhị phân
     * @param file File stream để đọc
     * @return true nếu đọc thành công
     */
    bool docFile(std::ifstream &file);

    // ========== OPERATORS ==========

    /**
     * @brief Toán tử gán
     * @param other Đối tượng San khác
     * @return Tham chiếu đến đối tượng hiện tại
     */
    San &operator=(const San &other);

    /**
     * @brief Toán tử so sánh (theo mã sân)
     * @param other Đối tượng San khác
     * @return true nếu mã sân giống nhau
     */
    bool operator==(const San &other) const;

    /**
     * @brief Toán tử so sánh < (theo giá thuê)
     * @param other Đối tượng San khác
     * @return true nếu giá thuê nhỏ hơn
     */
    bool operator<(const San &other) const;

    /**
     * @brief Toán tử xuất ra stream
     * @param os Output stream
     * @param s Đối tượng San
     * @return Tham chiếu đến output stream
     */
    friend std::ostream &operator<<(std::ostream &os, const San &s);
};

#endif // SAN_H
