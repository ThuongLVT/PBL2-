/**
 * @file DonHangDichVu.h
 * @brief Lớp đại diện cho đơn hàng dịch vụ riêng
 * @details Đơn hàng dịch vụ độc lập - không liên quan đến đặt sân
 *          Khách hàng chỉ mua đồ uống/thiết bị mà không đặt sân
 *
 * @author khninh22 - Service Module
 * @date 2025-11-14
 */

#ifndef DONHANGDICHVU_H
#define DONHANGDICHVU_H

#include "KhachHang.h"
#include "DichVuDat.h"
#include "../CauTrucDuLieu/MangDong.h"
#include "../CauTrucDuLieu/NgayGio.h"
#include <string>

/**
 * @enum TrangThaiDonHang
 * @brief Trạng thái đơn hàng dịch vụ
 */
enum class TrangThaiDonHang
{
    CHO_XU_LY,      ///< Đang chờ xử lý
    DANG_CHUAN_BI,  ///< Đang chuẩn bị
    HOAN_THANH,     ///< Hoàn thành
    DA_HUY          ///< Đã hủy
};

/**
 * @class DonHangDichVu
 * @brief Đơn hàng dịch vụ riêng (không liên quan đến đặt sân)
 * 
 * Đại diện cho đơn đặt dịch vụ độc lập - khách hàng chỉ mua
 * đồ uống/thiết bị mà không đặt sân
 */
class DonHangDichVu
{
private:
    std::string maDonHang;               ///< Mã đơn (tự sinh: DH + timestamp)
    KhachHang *khachHang;                ///< Khách hàng (có thể NULL - khách vãng lai)
    MangDong<DichVuDat> danhSachDichVu; ///< Danh sách dịch vụ đặt
    double tongTien;                     ///< Tổng tiền (chưa giảm giá)
    double giamGia;                      ///< Giảm giá (theo membership)
    double thanhTien;                    ///< Thành tiền = tongTien - giamGia
    TrangThaiDonHang trangThai;          ///< Trạng thái đơn hàng
    NgayGio ngayTao;                     ///< Ngày tạo đơn
    std::string ghiChu;                  ///< Ghi chú

public:
    // ========== CONSTRUCTORS ==========
    
    /**
     * @brief Constructor mặc định
     */
    DonHangDichVu();
    
    /**
     * @brief Constructor với tham số
     * @param ma Mã đơn hàng
     * @param kh Khách hàng (có thể NULL)
     */
    DonHangDichVu(const std::string &ma, KhachHang *kh);
    
    /**
     * @brief Copy constructor
     * @param other Đối tượng DonHangDichVu khác
     */
    DonHangDichVu(const DonHangDichVu &other);
    
    /**
     * @brief Destructor
     */
    ~DonHangDichVu();

    // ========== OPERATORS ==========
    
    /**
     * @brief Toán tử gán
     * @param other Đối tượng DonHangDichVu khác
     * @return Tham chiếu đến đối tượng hiện tại
     */
    DonHangDichVu &operator=(const DonHangDichVu &other);
    
    /**
     * @brief Toán tử so sánh
     * @param other Đối tượng DonHangDichVu khác
     * @return true nếu mã đơn hàng giống nhau
     */
    bool operator==(const DonHangDichVu &other) const;

    // ========== GETTERS ==========
    
    /**
     * @brief Lấy mã đơn hàng
     * @return Mã đơn hàng
     */
    std::string getMaDonHang() const;
    
    /**
     * @brief Lấy khách hàng
     * @return Pointer tới khách hàng
     */
    KhachHang *getKhachHang() const;
    
    /**
     * @brief Lấy danh sách dịch vụ
     * @return Danh sách dịch vụ đã đặt
     */
    const MangDong<DichVuDat> &getDanhSachDichVu() const;
    
    /**
     * @brief Lấy tổng tiền
     * @return Tổng tiền (chưa giảm giá)
     */
    double getTongTien() const;
    
    /**
     * @brief Lấy giảm giá
     * @return Số tiền giảm giá
     */
    double getGiamGia() const;
    
    /**
     * @brief Lấy thành tiền
     * @return Thành tiền (sau giảm giá)
     */
    double getThanhTien() const;
    
    /**
     * @brief Lấy trạng thái
     * @return Trạng thái đơn hàng
     */
    TrangThaiDonHang getTrangThai() const;
    
    /**
     * @brief Lấy ngày tạo
     * @return Ngày giờ tạo đơn
     */
    NgayGio getNgayTao() const;
    
    /**
     * @brief Lấy ghi chú
     * @return Ghi chú
     */
    std::string getGhiChu() const;

    // ========== SETTERS ==========
    
    /**
     * @brief Đặt mã đơn hàng
     * @param ma Mã đơn hàng mới
     */
    void setMaDonHang(const std::string &ma);
    
    /**
     * @brief Đặt khách hàng
     * @param kh Pointer tới khách hàng
     */
    void setKhachHang(KhachHang *kh);
    
    /**
     * @brief Đặt trạng thái
     * @param tt Trạng thái mới
     */
    void setTrangThai(TrangThaiDonHang tt);
    
    /**
     * @brief Đặt ghi chú
     * @param gc Ghi chú mới
     */
    void setGhiChu(const std::string &gc);

    // ========== METHODS ==========
    
    /**
     * @brief Thêm dịch vụ vào đơn hàng
     * @param dv Dịch vụ đã đặt
     */
    void themDichVu(const DichVuDat &dv);
    
    /**
     * @brief Xóa dịch vụ khỏi đơn hàng
     * @param index Vị trí dịch vụ cần xóa
     */
    void xoaDichVu(int index);
    
    /**
     * @brief Tính tổng tiền dịch vụ
     */
    void tinhTongTien();
    
    /**
     * @brief Tính giảm giá theo membership
     */
    void tinhGiamGia();
    
    /**
     * @brief Tính thành tiền = tổng - giảm giá
     */
    void tinhThanhTien();
    
    /**
     * @brief Hiển thị thông tin đơn hàng
     */
    void hienThi() const;
    
    /**
     * @brief Lấy tên trạng thái dạng chuỗi
     * @return Tên trạng thái
     */
    std::string getTrangThaiText() const;

    // ========== FILE I/O ==========
    
    /**
     * @brief Ghi thông tin ra file
     * @param f File pointer
     */
    void ghiFile(FILE *f) const;
    
    /**
     * @brief Đọc thông tin từ file
     * @param f File pointer
     */
    void docFile(FILE *f);

    // ========== HELPER ==========
    
    /**
     * @brief Lấy mã khách hàng (hoặc "GUEST")
     * @return Mã khách hàng hoặc "GUEST"
     */
    std::string getMaKhachHang() const;

    // ========== STATIC ==========
    
    /**
     * @brief Tạo mã đơn hàng tự động
     * @return Mã đơn hàng dạng DH + timestamp
     */
    static std::string taoMaDonHang();
};

#endif // DONHANGDICHVU_H
