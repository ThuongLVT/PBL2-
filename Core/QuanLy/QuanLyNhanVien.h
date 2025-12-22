// Lop quan ly nhan vien - CRUD (admin only), tim kiem, xac thuc dang nhap, quan ly CSV

#ifndef QUANLYNHANVIEN_H
#define QUANLYNHANVIEN_H

#include "../Models/NhanVien.h"
#include "../Models/QuanTriVien.h"
#include "../CauTrucDuLieu/MangDong.h"
#include <string>

class QuanLyNhanVien
{
private:
    MangDong<NguoiDung *> danhSachNhanVien;      // Danh sach tat ca nhan vien (NhanVien only)
    MangDong<QuanTriVien *> danhSachQuanTriVien; // Danh sach quan tri vien
    int maxEmployeeId;                           // Ma NV cao nhat
    bool isLoadingFromCSV;                       // Flag ngan auto-save khi load CSV
    NguoiDung *currentUser;                      // User hien tai (kiem tra quyen admin)

public:
    // ========== CONSTRUCTORS ==========
    QuanLyNhanVien();
    ~QuanLyNhanVien();

    // ========== ACCESS CONTROL ==========
    void setCurrentUser(NguoiDung *user);
    bool isAdmin() const;

    // ========== CRUD OPERATIONS ==========
    bool themNhanVien(NguoiDung *user);                                     // Them NV (admin only)
    bool xoaNhanVien(const std::string &maNV);                              // Xoa NV (admin only)
    bool capNhatNhanVien(const std::string &maNV, const NhanVien &nvMoi);   // Cap nhat NV (admin only)
    NhanVien *timNhanVien(const std::string &maNV);
    const MangDong<NguoiDung *> &layDanhSachNhanVien() const;
    const MangDong<QuanTriVien *> &layDanhSachQuanTriVien() const;

    // ========== SEARCH ==========
    NhanVien *timNhanVienTheoUsername(const std::string &username);
    NhanVien *timNhanVienTheoSDT(const std::string &sdt);
    MangDong<NhanVien *> timNhanVienTheoTen(const std::string &ten);

    // ========== AUTHENTICATION ==========
    NhanVien *xacThucDangNhap(const std::string &username, const std::string &password);

    // ========== STATISTICS ==========
    int tongSoNhanVien() const;
    int demSoAdmin() const;
    int demSoStaff() const;

    // ========== ID GENERATION ==========
    std::string taoMaNhanVienMoi();
    bool kiemTraUsernameTonTai(const std::string &username, const std::string &excludeMaNV = "") const;
    bool kiemTraSDTTonTai(const std::string &sdt, const std::string &excludeMaNV = "") const;

    // ========== CSV I/O (ADMIN ONLY) ==========
    bool luuCSV(const std::string &filename = "nhanvien.csv") const;
    bool docCSV(const std::string &filename = "nhanvien.csv");

    // ========== ADMIN CSV I/O ==========
    bool luuAdminCSV(const std::string &filename = "admin.csv") const;
    bool docAdminCSV(const std::string &filename = "admin.csv");
    QuanTriVien *timQuanTriVienTheoUsername(const std::string &username) const;
    void xoaTatCa();
};

#endif // QUANLYNHANVIEN_H
