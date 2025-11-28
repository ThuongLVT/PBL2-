/**
 * @file KhungGio.h
 * @brief Lớp đại diện cho Khung giờ đặt sân
 * @details Quản lý thời gian bắt đầu và kết thúc của một booking
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#ifndef KHUNGGIO_H
#define KHUNGGIO_H
#include <string>

#include "../CauTrucDuLieu/NgayGio.h"
#include <iostream>
#include <fstream>

/**
 * @class KhungGio
 * @brief Lớp đại diện cho một khung giờ đặt sân
 *
 * Khung giờ bao gồm:
 * - Giờ bắt đầu
 * - Giờ kết thúc
 * - Số giờ thuê
 */
class KhungGio
{
private:
    ThoiGian gioBatDau;  ///< Giờ bắt đầu
    ThoiGian gioKetThuc; ///< Giờ kết thúc

public:
    // ========== CONSTRUCTORS ==========

    /**
     * @brief Constructor mặc định
     */
    KhungGio();

    /**
     * @brief Constructor có tham số
     * @param bd Giờ bắt đầu
     * @param kt Giờ kết thúc
     */
    KhungGio(const ThoiGian &bd, const ThoiGian &kt);

    /**
     * @brief Constructor với số giờ, phút, giây
     * @param gioBD Giờ bắt đầu (giờ)
     * @param phutBD Giờ bắt đầu (phút)
     * @param gioKT Giờ kết thúc (giờ)
     * @param phutKT Giờ kết thúc (phút)
     */
    KhungGio(int gioBD, int phutBD, int gioKT, int phutKT);

    /**
     * @brief Copy constructor
     * @param other Đối tượng KhungGio khác
     */
    KhungGio(const KhungGio &other);

    /**
     * @brief Destructor
     */
    ~KhungGio();

    // ========== GETTERS ==========

    /**
     * @brief Lấy giờ bắt đầu
     * @return Giờ bắt đầu
     */
    ThoiGian layGioBatDau() const;
    ThoiGian getGioBatDau() const { return gioBatDau; } // Alias

    /**
     * @brief Lấy giờ kết thúc
     * @return Giờ kết thúc
     */
    ThoiGian layGioKetThuc() const;
    ThoiGian getGioKetThuc() const { return gioKetThuc; } // Alias

    // ========== SETTERS ==========

    /**
     * @brief Đặt giờ bắt đầu
     * @param bd Giờ bắt đầu mới
     */
    void datGioBatDau(const ThoiGian &bd);

    /**
     * @brief Đặt giờ kết thúc
     * @param kt Giờ kết thúc mới
     */
    void datGioKetThuc(const ThoiGian &kt);

    // ========== METHODS ==========

    /**
     * @brief Tính số giờ thuê (dạng thập phân)
     * @return Số giờ (VD: 1.5 giờ = 1 giờ 30 phút)
     */
    double tinhSoGio() const;

    /**
     * @brief Kiểm tra khung giờ có hợp lệ không
     * @return true nếu giờ bắt đầu < giờ kết thúc
     */
    bool hopLe() const;

    /**
     * @brief Kiểm tra trùng lặp với khung giờ khác
     * @param other Khung giờ khác
     * @return true nếu hai khung giờ trùng nhau
     */
    bool kiemTraTrung(const KhungGio &other) const;

    /**
     * @brief Hiển thị thông tin khung giờ
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
     * @param other Đối tượng KhungGio khác
     * @return Tham chiếu đến đối tượng hiện tại
     */
    KhungGio &operator=(const KhungGio &other);

    /**
     * @brief Toán tử so sánh bằng
     * @param other Đối tượng KhungGio khác
     * @return true nếu giống nhau
     */
    bool operator==(const KhungGio &other) const;

    /**
     * @brief Toán tử so sánh < (theo giờ bắt đầu)
     * @param other Đối tượng KhungGio khác
     * @return true nếu bắt đầu sớm hơn
     */
    bool operator<(const KhungGio &other) const;

    /**
     * @brief Toán tử xuất ra stream
     * @param os Output stream
     * @param kg Đối tượng KhungGio
     * @return Tham chiếu đến output stream
     */
    friend std::ostream &operator<<(std::ostream &os, const KhungGio &kg);

    // File I/O overloads for FILE*
    bool ghiFile(FILE *f) const;
    bool docFile(FILE *f);
};

#endif // KHUNGGIO_H
