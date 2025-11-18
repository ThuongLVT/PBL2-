#ifndef HETHONGQUANLY_H
#define HETHONGQUANLY_H
#include <string>

#include "../Models/KhachHang.h"
#include "../Models/NhanVien.h"
#include "../Models/QuanTriVien.h"
#include "../Models/San.h"
#include "../Models/DichVu.h"
#include "../Models/DatSan.h"
#include "../Models/ThanhToan.h"
#include "../CauTrucDuLieu/MangDong.h"

// Import các lớp quản lý
#include "QuanLySan.h"
#include "QuanLyKhachHang.h"
#include "QuanLyNhanVien.h"
#include "QuanLyDichVu.h"
#include "QuanLyDonHangDichVu.h"
#include "QuanLyDatSan.h"
#include "QuanLyThanhToan.h"
#include "BackupManager.h"

/**
 * @class HeThongQuanLy
 * @brief Lớp hệ thống quản lý chính - Singleton pattern
 * @details Đóng vai trò Coordinator - điều phối các lớp quản lý riêng biệt
 *          Không còn CRUD trực tiếp, mà delegate cho các Manager classes
 */
class HeThongQuanLy
{
private:
    // Singleton instance
    static HeThongQuanLy *instance;

    // ===== CÁC LỚP QUẢN LÝ (MANAGERS) =====
    QuanLySan *quanLySan;
    QuanLyKhachHang *quanLyKhachHang;
    QuanLyNhanVien *quanLyNhanVien;
    QuanLyDichVu *quanLyDichVu;
    QuanLyDonHangDichVu *quanLyDonHangDichVu;
    QuanLyDatSan *quanLyDatSan;
    QuanLyThanhToan *quanLyThanhToan;
    BackupManager *backupManager;

    // ===== NHÂN VIÊN & QUẢN TRỊ VIÊN (Quản lý trực tiếp) =====
    MangDong<NhanVien *> danhSachNhanVien;
    MangDong<QuanTriVien *> danhSachQuanTriVien;

    // Constructor private (Singleton)
    HeThongQuanLy();

public:
    // Destructor
    ~HeThongQuanLy();

    // Get singleton instance
    static HeThongQuanLy *getInstance();

    // Get singleton instance as pointer (for Qt compatibility)
    static HeThongQuanLy *getInstancePtr() { return getInstance(); }

    // ===== GETTERS CHO CÁC MANAGER =====
    QuanLySan *layQuanLySan() const { return quanLySan; }
    QuanLyKhachHang *layQuanLyKhachHang() const { return quanLyKhachHang; }
    QuanLyNhanVien *layQuanLyNhanVien() const { return quanLyNhanVien; }
    QuanLyDichVu *layQuanLyDichVu() const { return quanLyDichVu; }
    QuanLyDonHangDichVu *layQuanLyDonHangDichVu() const { return quanLyDonHangDichVu; }
    QuanLyDatSan *layQuanLyDatSan() const { return quanLyDatSan; }
    QuanLyThanhToan *layQuanLyThanhToan() const { return quanLyThanhToan; }
    BackupManager *layBackupManager() const { return backupManager; }

    // ===== KHÁCH HÀNG (Delegate to QuanLyKhachHang) =====
    bool themKhachHang(KhachHang *kh);
    bool xoaKhachHang(const std::string &ma);
    bool capNhatKhachHang(const std::string &ma, const KhachHang &khMoi);
    KhachHang *timKhachHang(const std::string &ma);
    KhachHang *timKhachHangTheoSDT(const std::string &sdt);
    const MangDong<KhachHang *> &layDanhSachKhachHang() const;
    void hienThiDanhSachKhachHang() const;

    // ===== NHÂN VIÊN =====
    bool themNhanVien(NhanVien *nv);
    bool xoaNhanVien(const std::string &ma);
    bool capNhatNhanVien(const std::string &ma, const NhanVien &nvMoi);
    NhanVien *timNhanVien(const std::string &ma);
    const MangDong<NhanVien *> &layDanhSachNhanVien() const;
    void hienThiDanhSachNhanVien() const;

    // ===== QUẢN TRỊ VIÊN =====
    bool themQuanTriVien(QuanTriVien *qtv);
    QuanTriVien *timQuanTriVien(const std::string &ma);
    const MangDong<QuanTriVien *> &layDanhSachQuanTriVien() const;

    // ===== SÂN (Delegate to QuanLySan) =====
    bool themSan(San *san);
    bool xoaSan(const std::string &ma);
    bool capNhatSan(const std::string &ma, const San &sanMoi);
    San *timSan(const std::string &ma);
    const MangDong<San *> &layDanhSachSan() const;
    void hienThiDanhSachSan() const;
    MangDong<San *> timSanTheoLoai(const std::string &loai);
    MangDong<San *> timSanTrong(const NgayGio &thoiGian, const KhungGio &khung);

    // ===== DỊCH VỤ (Delegate to QuanLyDichVu) =====
    bool themDichVu(DichVu *dv);
    bool xoaDichVu(const std::string &ma);
    bool capNhatDichVu(const std::string &ma, const DichVu &dvMoi);
    DichVu *timDichVu(const std::string &ma);
    const MangDong<DichVu *> &layDanhSachDichVu() const;
    void hienThiDanhSachDichVu() const;

    // ===== ĐƠN HÀNG DỊCH VỤ (Delegate to QuanLyDonHangDichVu) =====
    DonHangDichVu *taoDonHangDichVu(KhachHang *kh);
    bool huyDonHangDichVu(const std::string &maDH);
    bool capNhatTrangThaiDonHang(const std::string &maDH, TrangThaiDonHang trangThai);
    DonHangDichVu *timDonHangDichVu(const std::string &maDH);
    const MangDong<DonHangDichVu *> &layDanhSachDonHangDichVu() const;
    void hienThiDanhSachDonHangDichVu() const;
    double tongDoanhThuDichVu() const;

    // ===== ĐẶT SÂN (Delegate to QuanLyDatSan) =====
    DatSan *taoDatSan(KhachHang *kh, San *san, const NgayGio &thoiGian, const KhungGio &khung);
    bool huyDatSan(const std::string &maDatSan);
    bool capNhatTrangThaiDatSan(const std::string &maDatSan, TrangThaiDatSan trangThai);
    DatSan *timDatSan(const std::string &ma);
    const MangDong<DatSan *> &layDanhSachDatSan() const;
    void hienThiDanhSachDatSan() const;
    MangDong<DatSan *> timDatSanTheoKhachHang(const std::string &maKH);
    MangDong<DatSan *> timDatSanTheoSan(const std::string &maSan);
    MangDong<DatSan *> timDatSanTheoNgay(const NgayThang &ngay);
    bool kiemTraSanTrong(San *san, const NgayGio &thoiGian, const KhungGio &khung);

    // ===== THANH TOÁN (Delegate to QuanLyThanhToan) =====
    ThanhToan *taoThanhToan(DatSan *datSan, PhuongThucThanhToan phuongThuc);
    bool xoaThanhToan(const std::string &ma);
    ThanhToan *timThanhToan(const std::string &ma);
    ThanhToan *timThanhToanTheoDatSan(const std::string &maDatSan);
    const MangDong<ThanhToan *> &layDanhSachThanhToan() const;
    void hienThiDanhSachThanhToan() const;

    // ===== FILE I/O (BINARY ONLY) =====
    bool luuHeThong(const std::string &tenFile);
    bool docHeThong(const std::string &tenFile);
    void xoaTatCaDuLieu();

    /**
     * @brief Khởi tạo dữ liệu mẫu (10 items mỗi loại)
     * @details Dùng khi chạy lần đầu hoặc không có data.bin
     */
    void khoiTaoDuLieuMau();

    // ===== BACKUP/RESTORE =====
    bool saoLuuHeThong(const std::string &fileGoc);
    bool khoiPhucHeThong(const std::string &fileBackup, const std::string &fileDich);

    // ===== CSV I/O (NEW) =====
    bool luuKhachHangCSV(const std::string &filename);
    bool docKhachHangCSV(const std::string &filename);
    bool luuNhanVienCSV(const std::string &filename);
    bool docNhanVienCSV(const std::string &filename);
    bool luuSanCSV(const std::string &filename);
    bool docSanCSV(const std::string &filename);
    bool luuDichVuCSV(const std::string &filename);
    bool docDichVuCSV(const std::string &filename);
    bool luuDatSanCSV(const std::string &filename);
    bool docDatSanCSV(const std::string &filename);

    // ===== UTILITY =====
    int tongSoKhachHang() const;
    int tongSoNhanVien() const;
    int tongSoSan() const;
    int tongSoDichVu() const;
    int tongSoDonHangDichVu() const;
    int tongSoDatSan() const;
    int tongSoThanhToan() const;
};

#endif // HETHONGQUANLY_H
