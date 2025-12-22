#ifndef HETHONGQUANLY_H
#define HETHONGQUANLY_H
#include <string>

#include "../Models/KhachHang.h"
#include "../Models/NhanVien.h"
#include "../Models/QuanTriVien.h"
#include "../Models/San.h"
#include "../Models/DichVu.h"
#include "../Models/DatSan.h"
#include "../CauTrucDuLieu/MangDong.h"

// Import cac lop quan ly
#include "QuanLySan.h"
#include "QuanLyKhachHang.h"
#include "QuanLyNhanVien.h"
#include "QuanLyDichVu.h"
#include "QuanLyDonHangDichVu.h"
#include "QuanLyDatSan.h"

// Lop he thong quan ly chinh - Singleton pattern
// Dong vai tro Coordinator - dieu phoi cac lop quan ly rieng biet
class HeThongQuanLy
{
private:
    // Singleton instance
    static HeThongQuanLy *instance;

    // ===== CAC LOP QUAN LY (MANAGERS) =====
    QuanLySan *quanLySan;
    QuanLyKhachHang *quanLyKhachHang;
    QuanLyNhanVien *quanLyNhanVien;
    QuanLyDichVu *quanLyDichVu;
    QuanLyDonHangDichVu *quanLyDonHangDichVu;
    QuanLyDatSan *quanLyDatSan;

    // ===== NHAN VIEN & QUAN TRI VIEN =====
    MangDong<NhanVien *> danhSachNhanVien;
    MangDong<QuanTriVien *> danhSachQuanTriVien;

    // Constructor private (Singleton)
    HeThongQuanLy();

public:
    ~HeThongQuanLy();
    static HeThongQuanLy *getInstance();
    static HeThongQuanLy *getInstancePtr() { return getInstance(); }

    // ===== GETTERS CHO CAC MANAGER =====
    QuanLySan *layQuanLySan() const { return quanLySan; }
    QuanLyKhachHang *layQuanLyKhachHang() const { return quanLyKhachHang; }
    QuanLyNhanVien *layQuanLyNhanVien() const { return quanLyNhanVien; }
    QuanLyDichVu *layQuanLyDichVu() const { return quanLyDichVu; }
    QuanLyDonHangDichVu *layQuanLyDonHangDichVu() const { return quanLyDonHangDichVu; }
    QuanLyDatSan *layQuanLyDatSan() const { return quanLyDatSan; }

    // ===== KHACH HANG (Delegate to QuanLyKhachHang) =====
    bool themKhachHang(KhachHang *kh);
    bool xoaKhachHang(const std::string &ma);
    bool capNhatKhachHang(const std::string &ma, const KhachHang &khMoi);
    KhachHang *timKhachHang(const std::string &ma);
    KhachHang *timKhachHangTheoSDT(const std::string &sdt);
    const MangDong<KhachHang *> &layDanhSachKhachHang() const;
    void hienThiDanhSachKhachHang() const;

    // ===== NHAN VIEN =====
    bool themNhanVien(NhanVien *nv);
    bool xoaNhanVien(const std::string &ma);
    bool capNhatNhanVien(const std::string &ma, const NhanVien &nvMoi);
    NhanVien *timNhanVien(const std::string &ma);
    const MangDong<NhanVien *> &layDanhSachNhanVien() const;
    void hienThiDanhSachNhanVien() const;

    // ===== QUAN TRI VIEN =====
    bool themQuanTriVien(QuanTriVien *qtv);
    QuanTriVien *timQuanTriVien(const std::string &ma);
    const MangDong<QuanTriVien *> &layDanhSachQuanTriVien() const;

    // ===== SAN (Delegate to QuanLySan) =====
    bool themSan(San *san);
    bool xoaSan(const std::string &ma);
    bool capNhatSan(const std::string &ma, const San &sanMoi);
    San *timSan(const std::string &ma);
    const MangDong<San *> &layDanhSachSan() const;
    void hienThiDanhSachSan() const;
    MangDong<San *> timSanTheoLoai(const std::string &loai);
    MangDong<San *> timSanTrong(const NgayGio &thoiGian, const KhungGio &khung);

    // ===== DICH VU (Delegate to QuanLyDichVu) =====
    bool themDichVu(DichVu *dv);
    bool xoaDichVu(const std::string &ma);
    bool capNhatDichVu(const std::string &ma, const DichVu &dvMoi);
    DichVu *timDichVu(const std::string &ma);
    const MangDong<DichVu *> &layDanhSachDichVu() const;
    void hienThiDanhSachDichVu() const;

    // ===== DON HANG DICH VU (Delegate to QuanLyDonHangDichVu) =====
    DonHangDichVu *taoDonHangDichVu(KhachHang *kh);
    bool huyDonHangDichVu(const std::string &maDH);
    bool capNhatTrangThaiDonHang(const std::string &maDH, TrangThaiDonHang trangThai);
    DonHangDichVu *timDonHangDichVu(const std::string &maDH);
    const MangDong<DonHangDichVu *> &layDanhSachDonHangDichVu() const;
    void hienThiDanhSachDonHangDichVu() const;
    double tongDoanhThuDichVu() const;

    // ===== DAT SAN (Delegate to QuanLyDatSan) =====
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

    // ===== FILE I/O =====
    bool luuHeThong(const std::string &tenFile);
    bool docHeThong(const std::string &tenFile);
    void xoaTatCaDuLieu();
    void khoiTaoDuLieuMau(); // Khoi tao du lieu mau (10 items moi loai)

    // ===== CSV I/O =====
    bool luuKhachHangCSV(const std::string &filename);
    bool docKhachHangCSV(const std::string &filename);
    bool luuNhanVienCSV(const std::string &filename);
    bool docNhanVienCSV(const std::string &filename);
    bool luuQuanTriVienCSV(const std::string &filename);
    bool docQuanTriVienCSV(const std::string &filename);
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
    void tinhLaiTongChiTieuKhachHang(); // Dong bo lai tong chi tieu
};

#endif // HETHONGQUANLY_H
