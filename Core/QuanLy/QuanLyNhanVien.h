/**
 * @file QuanLyNhanVien.h
 * @brief Lớp quản lý nhân viên
 * @details Quản lý CRUD nhân viên với quyền truy cập admin-only cho file CSV
 *
 * @author Football Field Management System
 * @date 2025-11-18
 */

#ifndef QUANLYNHANVIEN_H
#define QUANLYNHANVIEN_H

#include "../Models/NhanVien.h"
#include "../Models/QuanTriVien.h"
#include "../CauTrucDuLieu/MangDong.h"
#include <string>

/**
 * @class QuanLyNhanVien
 * @brief Lớp quản lý nhân viên và quản trị viên
 *
 * Chức năng:
 * - CRUD nhân viên (admin only)
 * - Tìm kiếm theo mã, username, tên, SĐT
 * - Xác thực đăng nhập
 * - Quản lý CSV với access control
 */
class QuanLyNhanVien
{
private:
    MangDong<NguoiDung *> danhSachNhanVien;     ///< Danh sách tất cả nhân viên (NhanVien only, admin separate)
    MangDong<QuanTriVien *> danhSachQuanTriVien; ///< Danh sách quản trị viên (separate from staff)
    int maxEmployeeId;                          ///< Mã NV cao nhất đã sử dụng
    bool isLoadingFromCSV;                      ///< Flag để ngăn auto-save trong khi load CSV
    NguoiDung *currentUser;                     ///< User hiện tại (để kiểm tra quyền admin)

public:
    // ========== CONSTRUCTORS ==========
    QuanLyNhanVien();
    ~QuanLyNhanVien();

    // ========== ACCESS CONTROL ==========
    /**
     * @brief Đặt user hiện tại (để kiểm tra quyền admin)
     * @param user Pointer tới NguoiDung hiện tại
     */
    void setCurrentUser(NguoiDung *user);

    /**
     * @brief Kiểm tra user hiện tại có quyền admin không
     * @return true nếu là admin
     */
    bool isAdmin() const;

    // ========== CRUD OPERATIONS ==========
    /**
     * @brief Thêm nhân viên mới (admin only)
     * @param user Pointer tới NguoiDung (NhanVien hoặc QuanTriVien)
     * @return true nếu thành công
     */
    bool themNhanVien(NguoiDung *user);

    /**
     * @brief Xóa nhân viên (admin only)
     * @param maNV Mã nhân viên
     * @return true nếu thành công
     */
    bool xoaNhanVien(const std::string &maNV);

    /**
     * @brief Cập nhật thông tin nhân viên (admin only)
     * @param maNV Mã nhân viên
     * @param nvMoi Thông tin mới
     * @return true nếu thành công
     */
    bool capNhatNhanVien(const std::string &maNV, const NhanVien &nvMoi);

    /**
     * @brief Tìm nhân viên theo mã
     * @param maNV Mã nhân viên
     * @return Pointer tới NhanVien, nullptr nếu không tìm thấy
     */
    NhanVien *timNhanVien(const std::string &maNV);

    /**
     * @brief Lấy toàn bộ danh sách nhân viên (admin only)
     * @return Reference tới MangDong<NguoiDung*>
     */
    const MangDong<NguoiDung *> &layDanhSachNhanVien() const;

    // ========== SEARCH ==========
    /**
     * @brief Tìm nhân viên theo username (dùng cho login)
     * @param username Tên đăng nhập
     * @return Pointer tới NhanVien, nullptr nếu không tìm thấy
     */
    NhanVien *timNhanVienTheoUsername(const std::string &username);

    /**
     * @brief Tìm nhân viên theo số điện thoại
     * @param sdt Số điện thoại
     * @return Pointer tới NhanVien, nullptr nếu không tìm thấy
     */
    NhanVien *timNhanVienTheoSDT(const std::string &sdt);

    /**
     * @brief Tìm nhân viên theo tên (gần đúng)
     * @param ten Tên nhân viên
     * @return MangDong chứa các nhân viên tìm thấy
     */
    MangDong<NhanVien *> timNhanVienTheoTen(const std::string &ten);

    // ========== AUTHENTICATION ==========
    /**
     * @brief Xác thực đăng nhập
     * @param username Tên đăng nhập
     * @param password Mật khẩu
     * @return Pointer tới NhanVien nếu thành công, nullptr nếu thất bại
     */
    NhanVien *xacThucDangNhap(const std::string &username, const std::string &password);

    // ========== STATISTICS ==========
    /**
     * @brief Tổng số nhân viên
     * @return Số lượng nhân viên
     */
    int tongSoNhanVien() const;

    /**
     * @brief Đếm số admin
     * @return Số lượng quản trị viên
     */
    int demSoAdmin() const;

    /**
     * @brief Đếm số staff
     * @return Số lượng nhân viên thường
     */
    int demSoStaff() const;

    // ========== ID GENERATION ==========
    /**
     * @brief Tạo mã nhân viên mới duy nhất (NV001, NV002, ...)
     * @return Mã nhân viên mới
     */
    std::string taoMaNhanVienMoi();

    /**
     * @brief Kiểm tra username đã tồn tại chưa
     * @param username Username cần kiểm tra
     * @param excludeMaNV Loại trừ mã NV này (dùng cho cập nhật)
     * @return true nếu username đã tồn tại
     */
    bool kiemTraUsernameTonTai(const std::string &username, const std::string &excludeMaNV = "") const;

    /**
     * @brief Kiểm tra SĐT đã tồn tại chưa
     * @param sdt Số điện thoại cần kiểm tra
     * @param excludeMaNV Loại trừ mã NV này (dùng cho cập nhật)
     * @return true nếu SĐT đã tồn tại
     */
    bool kiemTraSDTTonTai(const std::string &sdt, const std::string &excludeMaNV = "") const;

    // ========== CSV I/O (ADMIN ONLY) ==========
    /**
     * @brief Lưu danh sách nhân viên ra file CSV (admin only)
     * @param filename Đường dẫn file CSV (mặc định: Data/nhanvien.csv)
     * @return true nếu thành công
     */
    bool luuCSV(const std::string &filename = "Data/nhanvien.csv") const;

    /**
     * @brief Đọc danh sách nhân viên từ file CSV (admin only)
     * @param filename Đường dẫn file CSV (mặc định: Data/nhanvien.csv)
     * @return true nếu thành công
     */
    bool docCSV(const std::string &filename = "Data/nhanvien.csv");

    // ========== ADMIN CSV I/O ==========
    /**
     * @brief Lưu danh sách quản trị viên ra file CSV
     * @param filename Đường dẫn file CSV (mặc định: admin.csv)
     * @return true nếu thành công
     */
    bool luuAdminCSV(const std::string &filename = "admin.csv") const;

    /**
     * @brief Đọc danh sách quản trị viên từ file CSV
     * @param filename Đường dẫn file CSV (mặc định: admin.csv)
     * @return true nếu thành công
     */
    bool docAdminCSV(const std::string &filename = "admin.csv");

    /**
     * @brief Tìm quản trị viên theo username
     * @param username Tên đăng nhập
     * @return Pointer tới QuanTriVien nếu tìm thấy, nullptr nếu không
     */
    QuanTriVien *timQuanTriVienTheoUsername(const std::string &username) const;

    /**
     * @brief Xóa tất cả nhân viên (dùng cho cleanup)
     */
    void xoaTatCa();
};

#endif // QUANLYNHANVIEN_H
