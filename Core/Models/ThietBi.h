/**
 * @file ThietBi.h
 * @brief Lớp đại diện cho dịch vụ Thiết bị
 * @details Kế thừa từ DichVu, bổ sung thông tin về thiết bị cho thuê
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#ifndef THIETBI_H
#define THIETBI_H

#include "DichVu.h"
#include <string>

/**
 * @enum LoaiThietBi
 * @brief Các loại thiết bị
 */
enum class LoaiThietBi
{
    BONG,     ///< Bóng đá
    GIAY,     ///< Giày đá bóng
    AO,       ///< Áo thi đấu
    QUAN,     ///< Quần thi đấu
    BAO_TAY,  ///< Bảo tay thủ môn
    BANG_SAU, ///< Băng sâu
    GANG_TAY, ///< Găng tay thủ môn
    KHAC      ///< Loại khác
};

/**
 * @enum TinhTrangThietBi
 * @brief Tình trạng thiết bị
 */
enum class TinhTrangThietBi
{
    MOI,        ///< Mới 100%
    TOT,        ///< Tốt (>80%)
    TRUNG_BINH, ///< Trung bình (50-80%)
    KEM,        ///< Kém (<50%)
    HONG        ///< Hỏng, không dùng được
};

/**
 * @class ThietBi
 * @brief Lớp đại diện cho dịch vụ thiết bị
 *
 * Thiết bị bao gồm:
 * - Các thuộc tính của DichVu
 * - Loại thiết bị
 * - Size (S, M, L, XL, 38, 39, 40...)
 * - Tình trạng
 * - Số lượng tồn kho
 */
class ThietBi : public DichVu
{
private:
    LoaiThietBi loaiThietBi;    ///< Loại thiết bị
    std::string size;           ///< Size (S/M/L/XL hoặc số)
    TinhTrangThietBi tinhTrang; ///< Tình trạng thiết bị
    int soLuongTon;             ///< Số lượng tồn kho
    std::string hangSanXuat;    ///< Hãng sản xuất (Nike, Adidas...)

public:
    // ========== CONSTRUCTORS ==========

    /**
     * @brief Constructor mặc định
     */
    ThietBi();

    /**
     * @brief Constructor có tham số
     * @param maDV Mã dịch vụ
     * @param tenDV Tên thiết bị
     * @param gia Đơn giá
     * @param loai Loại thiết bị
     * @param size Size
     * @param tinhTrang Tình trạng
     * @param soLuong Số lượng tồn
     * @param hang Hãng sản xuất
     */
    ThietBi(const std::string &maDV, const std::string &tenDV,
            double gia, LoaiThietBi loai, const std::string &size,
            TinhTrangThietBi tinhTrang = TinhTrangThietBi::TOT,
            int soLuong = 0, const std::string &hang = "");

    /**
     * @brief Copy constructor
     * @param other Đối tượng ThietBi khác
     */
    ThietBi(const ThietBi &other);

    /**
     * @brief Destructor
     */
    ~ThietBi() override;

    // ========== GETTERS ==========

    /**
     * @brief Lấy loại thiết bị
     * @return Loại thiết bị
     */
    LoaiThietBi layLoaiThietBi() const;

    /**
     * @brief Lấy size
     * @return Size
     */
    std::string laySize() const;

    /**
     * @brief Lấy tình trạng
     * @return Tình trạng thiết bị
     */
    TinhTrangThietBi layTinhTrang() const;

    /**
     * @brief Lấy số lượng tồn
     * @return Số lượng tồn kho
     */
    int laySoLuongTon() const;

    /**
     * @brief Lấy hãng sản xuất
     * @return Hãng sản xuất
     */
    std::string layHangSanXuat() const;

    /**
     * @brief Lấy tên loại thiết bị
     * @return Tên loại thiết bị dạng chuỗi
     */
    std::string layTenLoaiThietBi() const;

    /**
     * @brief Lấy tên tình trạng
     * @return Tên tình trạng dạng chuỗi
     */
    std::string layTenTinhTrang() const;

    // ========== SETTERS ==========

    /**
     * @brief Đặt loại thiết bị
     * @param loai Loại thiết bị mới
     */
    void datLoaiThietBi(LoaiThietBi loai);

    /**
     * @brief Đặt size
     * @param sz Size mới
     */
    void datSize(const std::string &sz);

    /**
     * @brief Đặt tình trạng
     * @param tt Tình trạng mới
     */
    void datTinhTrang(TinhTrangThietBi tt);

    /**
     * @brief Đặt số lượng tồn
     * @param sl Số lượng tồn mới
     */
    void datSoLuongTon(int sl);

    /**
     * @brief Đặt hãng sản xuất
     * @param hang Hãng sản xuất mới
     */
    void datHangSanXuat(const std::string &hang);

    // ========== METHODS ==========

    /**
     * @brief Thêm số lượng tồn kho
     * @param soLuong Số lượng thêm vào
     */
    void nhapKho(int soLuong);

    /**
     * @brief Giảm số lượng tồn kho (khi cho thuê)
     * @param soLuong Số lượng xuất ra
     * @return true nếu xuất thành công
     */
    bool xuatKho(int soLuong);

    /**
     * @brief Kiểm tra còn đủ hàng không
     * @param soLuong Số lượng cần kiểm tra
     * @return true nếu đủ hàng
     */
    bool kiemTraDuHang(int soLuong) const;

    /**
     * @brief Tính tiền thuê thiết bị (override)
     * Có thể điều chỉnh giá theo tình trạng
     * @param soLuong Số lượng
     * @return Tổng tiền
     */
    double tinhTien(int soLuong) const override;

    /**
     * @brief Hiển thị thông tin thiết bị (override)
     */
    void hienThiThongTin() const override;

    /**
     * @brief Ghi thông tin ra file nhị phân (override)
     * @param file File stream để ghi
     * @return true nếu ghi thành công
     */
    bool ghiFile(std::ofstream &file) const override;

    /**
     * @brief Đọc thông tin từ file nhị phân (override)
     * @param file File stream để đọc
     * @return true nếu đọc thành công
     */
    bool docFile(std::ifstream &file) override;

    // ========== OPERATORS ==========

    /**
     * @brief Toán tử gán
     * @param other Đối tượng ThietBi khác
     * @return Tham chiếu đến đối tượng hiện tại
     */
    ThietBi &operator=(const ThietBi &other);

    /**
     * @brief Toán tử xuất ra stream
     * @param os Output stream
     * @param tb Đối tượng ThietBi
     * @return Tham chiếu đến output stream
     */
    friend std::ostream &operator<<(std::ostream &os, const ThietBi &tb);
};

#endif // THIETBI_H
