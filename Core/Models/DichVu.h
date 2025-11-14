/**
 * @file DichVu.h
 * @brief Lớp cơ sở đại diện cho Dịch vụ
 * @details Lớp trừu tượng cho các dịch vụ kèm theo (đồ uống, thiết bị...)
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#ifndef DICHVU_H
#define DICHVU_H
#include <string>

#include <iostream>
#include <fstream>

/**
 * @enum LoaiDichVu
 * @brief Các loại dịch vụ
 */
enum class LoaiDichVu
{
    DO_UONG,  ///< Đồ uống (nước, trà, cà phê...)
    THIET_BI, ///< Thiết bị (áo, giày, bóng...)
    BAO_HIEM, ///< Bảo hiểm tai nạn
    KHAC      ///< Dịch vụ khác
};

/**
 * @class DichVu
 * @brief Lớp cơ sở đại diện cho dịch vụ
 *
 * Dịch vụ bao gồm:
 * - Mã dịch vụ
 * - Tên dịch vụ
 * - Đơn giá
 * - Loại dịch vụ
 * - Mô tả
 */
class DichVu
{
protected:
    std::string maDichVu;    ///< Mã dịch vụ (VD: DV001)
    std::string tenDichVu;   ///< Tên dịch vụ
    double donGia;         ///< Đơn giá
    LoaiDichVu loaiDichVu; ///< Loại dịch vụ
    std::string moTa;        ///< Mô tả chi tiết
    bool conHang;          ///< Trạng thái còn hàng

public:
    // ========== CONSTRUCTORS ==========

    /**
     * @brief Constructor mặc định
     */
    DichVu();

    /**
     * @brief Constructor có tham số
     * @param maDV Mã dịch vụ
     * @param tenDV Tên dịch vụ
     * @param gia Đơn giá
     * @param loai Loại dịch vụ
     */
    DichVu(const std::string &maDV, const std::string &tenDV,
           double gia, LoaiDichVu loai);

    /**
     * @brief Copy constructor
     * @param other Đối tượng DichVu khác
     */
    DichVu(const DichVu &other);

    /**
     * @brief Destructor ảo
     */
    virtual ~DichVu();

    // ========== GETTERS ==========

    /**
     * @brief Lấy mã dịch vụ
     * @return Mã dịch vụ
     */
    std::string layMaDichVu() const;

    /**
     * @brief Lấy tên dịch vụ
     * @return Tên dịch vụ
     */
    std::string layTenDichVu() const;

    /**
     * @brief Lấy đơn giá
     * @return Đơn giá
     */
    double layDonGia() const;

    /**
     * @brief Lấy loại dịch vụ
     * @return Loại dịch vụ
     */
    LoaiDichVu layLoaiDichVu() const;

    /**
     * @brief Lấy mô tả
     * @return Mô tả
     */
    std::string layMoTa() const;

    /**
     * @brief Kiểm tra còn hàng
     * @return true nếu còn hàng
     */
    bool coConHang() const;

    /**
     * @brief Lấy tên loại dịch vụ
     * @return Tên loại dịch vụ dạng chuỗi
     */
    std::string layTenLoaiDichVu() const;

    // ========== SETTERS ==========

    /**
     * @brief Đặt tên dịch vụ
     * @param ten Tên dịch vụ mới
     */
    void datTenDichVu(const std::string &ten);

    /**
     * @brief Đặt đơn giá
     * @param gia Đơn giá mới
     */
    void datDonGia(double gia);

    /**
     * @brief Đặt mô tả
     * @param mt Mô tả mới
     */
    void datMoTa(const std::string &mt);

    /**
     * @brief Đặt trạng thái còn hàng
     * @param ch Trạng thái còn hàng
     */
    void datConHang(bool ch);

    // ========== METHODS ==========

    /**
     * @brief Tính tiền dịch vụ theo số lượng
     * @param soLuong Số lượng
     * @return Tổng tiền
     */
    virtual double tinhTien(int soLuong) const;

    /**
     * @brief Hiển thị thông tin dịch vụ (virtual)
     */
    virtual void hienThiThongTin() const;

    /**
     * @brief Ghi thông tin ra file nhị phân (virtual)
     * @param file File stream để ghi
     * @return true nếu ghi thành công
     */
    virtual bool ghiFile(std::ofstream &file) const;

    /**
     * @brief Đọc thông tin từ file nhị phân (virtual)
     * @param file File stream để đọc
     * @return true nếu đọc thành công
     */
    virtual bool docFile(std::ifstream &file);

    // ========== OPERATORS ==========

    /**
     * @brief Toán tử gán
     * @param other Đối tượng DichVu khác
     * @return Tham chiếu đến đối tượng hiện tại
     */
    DichVu &operator=(const DichVu &other);

    /**
     * @brief Toán tử so sánh (theo mã dịch vụ)
     * @param other Đối tượng DichVu khác
     * @return true nếu mã dịch vụ giống nhau
     */
    bool operator==(const DichVu &other) const;

    /**
     * @brief Toán tử so sánh < (theo đơn giá)
     * @param other Đối tượng DichVu khác
     * @return true nếu đơn giá nhỏ hơn
     */
    bool operator<(const DichVu &other) const;

    /**
     * @brief Toán tử xuất ra stream
     * @param os Output stream
     * @param dv Đối tượng DichVu
     * @return Tham chiếu đến output stream
     */
    friend std::ostream &operator<<(std::ostream &os, const DichVu &dv);
};

#endif // DICHVU_H
