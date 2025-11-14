/**
 * @file DoUong.h
 * @brief Lớp đại diện cho dịch vụ Đồ uống
 * @details Kế thừa từ DichVu, bổ sung thông tin về đồ uống
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#ifndef DOUONG_H
#define DOUONG_H

#include "DichVu.h"
#include <string>

/**
 * @enum LoaiDoUong
 * @brief Các loại đồ uống
 */
enum class LoaiDoUong
{
    NUOC_NGOT,     ///< Nước ngọt (Coca, Pepsi, 7Up...)
    BIA,           ///< Bia (Tiger, Heineken, Saigon...)
    NUOC_SUOI,     ///< Nước suối
    NUOC_TANG_LUC, ///< Nước tăng lực (RedBull, Sting...)
    TRA_SUA,       ///< Trà sữa
    CA_PHE,        ///< Cà phê
    NUOC_EP,       ///< Nước ép trái cây
    KHAC           ///< Loại khác
};

/**
 * @class DoUong
 * @brief Lớp đại diện cho dịch vụ đồ uống
 *
 * Đồ uống bao gồm:
 * - Các thuộc tính của DichVu
 * - Loại đồ uống cụ thể
 * - Dung tích (ml)
 * - Có đá hay không
 * - Số lượng tồn kho
 */
class DoUong : public DichVu
{
private:
    LoaiDoUong loaiDoUong; ///< Loại đồ uống cụ thể
    int dungTich;          ///< Dung tích (ml)
    bool coDa;             ///< Có đá hay không
    int soLuongTon;        ///< Số lượng tồn kho

public:
    // ========== CONSTRUCTORS ==========

    /**
     * @brief Constructor mặc định
     */
    DoUong();

    /**
     * @brief Constructor có tham số
     * @param maDV Mã dịch vụ
     * @param tenDV Tên đồ uống
     * @param gia Đơn giá
     * @param loai Loại đồ uống
     * @param dungTich Dung tích (ml)
     * @param coDa Có đá không
     * @param soLuong Số lượng tồn
     */
    DoUong(const std::string &maDV, const std::string &tenDV,
           double gia, LoaiDoUong loai, int dungTich,
           bool coDa = false, int soLuong = 0);

    /**
     * @brief Copy constructor
     * @param other Đối tượng DoUong khác
     */
    DoUong(const DoUong &other);

    /**
     * @brief Destructor
     */
    ~DoUong() override;

    // ========== GETTERS ==========

    /**
     * @brief Lấy loại đồ uống
     * @return Loại đồ uống
     */
    LoaiDoUong layLoaiDoUong() const;

    /**
     * @brief Lấy dung tích
     * @return Dung tích (ml)
     */
    int layDungTich() const;

    /**
     * @brief Kiểm tra có đá không
     * @return true nếu có đá
     */
    bool laCoDa() const;

    /**
     * @brief Lấy số lượng tồn
     * @return Số lượng tồn kho
     */
    int laySoLuongTon() const;

    /**
     * @brief Lấy tên loại đồ uống
     * @return Tên loại đồ uống dạng chuỗi
     */
    std::string layTenLoaiDoUong() const;

    // ========== SETTERS ==========

    /**
     * @brief Đặt loại đồ uống
     * @param loai Loại đồ uống mới
     */
    void datLoaiDoUong(LoaiDoUong loai);

    /**
     * @brief Đặt dung tích
     * @param dt Dung tích mới
     */
    void datDungTich(int dt);

    /**
     * @brief Đặt có đá
     * @param cd Có đá hay không
     */
    void datCoDa(bool cd);

    /**
     * @brief Đặt số lượng tồn
     * @param sl Số lượng tồn mới
     */
    void datSoLuongTon(int sl);

    // ========== METHODS ==========

    /**
     * @brief Thêm số lượng tồn kho
     * @param soLuong Số lượng thêm vào
     */
    void nhapKho(int soLuong);

    /**
     * @brief Giảm số lượng tồn kho (khi bán)
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
     * @brief Tính tiền đồ uống (override)
     * @param soLuong Số lượng
     * @return Tổng tiền
     */
    double tinhTien(int soLuong) const override;

    /**
     * @brief Hiển thị thông tin đồ uống (override)
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
     * @param other Đối tượng DoUong khác
     * @return Tham chiếu đến đối tượng hiện tại
     */
    DoUong &operator=(const DoUong &other);

    /**
     * @brief Toán tử xuất ra stream
     * @param os Output stream
     * @param du Đối tượng DoUong
     * @return Tham chiếu đến output stream
     */
    friend std::ostream &operator<<(std::ostream &os, const DoUong &du);
};

#endif // DOUONG_H
