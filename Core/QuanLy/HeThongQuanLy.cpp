/**
 * @file HeThongQuanLy.cpp
 * @brief Triển khai HeThongQuanLy - Coordinator pattern
 * @details HeThongQuanLy giờ chỉ delegate cho các Manager classes
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#include "HeThongQuanLy.h"
#include "../Utils/CSVHelper.h"
#include "../Utils/CSVManager.h" // Add this include
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

// Initialize static instance
HeThongQuanLy *HeThongQuanLy::instance = nullptr;

// Constructor
HeThongQuanLy::HeThongQuanLy()
{
    // Khởi tạo các Manager
    quanLySan = new QuanLySan();
    quanLyKhachHang = new QuanLyKhachHang();
    quanLyNhanVien = new QuanLyNhanVien();
    quanLyDichVu = new QuanLyDichVu();
    quanLyDonHangDichVu = new QuanLyDonHangDichVu();
    quanLyDatSan = new QuanLyDatSan();
    quanLyThanhToan = new QuanLyThanhToan();
    backupManager = new BackupManager();

    // NOTE: Do NOT auto-load CSV here
    // CSV will be loaded by main.cpp via docHeThong() to avoid duplicate loading
}

// Destructor
HeThongQuanLy::~HeThongQuanLy()
{
    xoaTatCaDuLieu();

    // Xóa các Manager
    delete quanLySan;
    delete quanLyKhachHang;
    delete quanLyNhanVien;
    delete quanLyDichVu;
    delete quanLyDonHangDichVu;
    delete quanLyDatSan;
    delete quanLyThanhToan;
    delete backupManager;
}

// Get singleton instance
HeThongQuanLy *HeThongQuanLy::getInstance()
{
    if (instance == nullptr)
    {
        instance = new HeThongQuanLy();
    }
    return instance;
}

// ===== KHÁCH HÀNG (Delegate to QuanLyKhachHang) =====
bool HeThongQuanLy::themKhachHang(KhachHang *kh)
{
    return quanLyKhachHang->themKhachHang(kh);
}

bool HeThongQuanLy::xoaKhachHang(const string &ma)
{
    return quanLyKhachHang->xoaKhachHang(ma);
}

bool HeThongQuanLy::capNhatKhachHang(const string &ma, const KhachHang &khMoi)
{
    return quanLyKhachHang->capNhatKhachHang(ma, khMoi);
}

KhachHang *HeThongQuanLy::timKhachHang(const string &ma)
{
    return quanLyKhachHang->timKhachHang(ma);
}

KhachHang *HeThongQuanLy::timKhachHangTheoSDT(const string &sdt)
{
    return quanLyKhachHang->timKhachHangTheoSDT(sdt);
}

const MangDong<KhachHang *> &HeThongQuanLy::layDanhSachKhachHang() const
{
    return quanLyKhachHang->layDanhSachKhachHang();
}

void HeThongQuanLy::hienThiDanhSachKhachHang() const
{
    quanLyKhachHang->hienThiDanhSachKhachHang();
}

// ===== NHÂN VIÊN (Quản lý trực tiếp) =====
bool HeThongQuanLy::themNhanVien(NhanVien *nv)
{
    return quanLyNhanVien->themNhanVien(nv);
}

bool HeThongQuanLy::xoaNhanVien(const string &ma)
{
    return quanLyNhanVien->xoaNhanVien(ma);
}

bool HeThongQuanLy::capNhatNhanVien(const string &ma, const NhanVien &nvMoi)
{
    return quanLyNhanVien->capNhatNhanVien(ma, nvMoi);
}

NhanVien *HeThongQuanLy::timNhanVien(const string &ma)
{
    return quanLyNhanVien->timNhanVien(ma);
}

const MangDong<NhanVien *> &HeThongQuanLy::layDanhSachNhanVien() const
{
    // Note: This returns MangDong<NguoiDung*>, we might need to cast or change return type
    // For now, we can't easily convert MangDong<NguoiDung*> to MangDong<NhanVien*> reference
    // So we might need to keep a local cache or change the interface.
    // However, since we are refactoring, let's see if we can change the return type in header.
    // But MangDong is a template.

    // Temporary workaround: Cast the list (unsafe but works if memory layout matches)
    // Better: Change HeThongQuanLy to return MangDong<NguoiDung*> or fix the architecture.
    // Given the constraints, I will assume the caller handles NguoiDung* or I construct a new list.
    // Constructing a new list is safer but slower.

    static MangDong<NhanVien *> cache;
    cache.clear();
    const MangDong<NguoiDung *> &users = quanLyNhanVien->layDanhSachNhanVien();
    for (int i = 0; i < users.size(); i++)
    {
        NhanVien *nv = dynamic_cast<NhanVien *>(users[i]);
        if (nv)
            cache.push_back(nv);
    }
    return cache;
}

void HeThongQuanLy::hienThiDanhSachNhanVien() const
{
    const MangDong<NguoiDung *> &users = quanLyNhanVien->layDanhSachNhanVien();
    cout << "\n===== DANH SACH NHAN VIEN =====" << endl;
    cout << "Tong so: " << users.size() << endl;
    for (int i = 0; i < users.size(); i++)
    {
        users[i]->hienThiThongTin();
        cout << "---" << endl;
    }
}

// ===== QUẢN TRỊ VIÊN (Quản lý trực tiếp) =====
bool HeThongQuanLy::themQuanTriVien(QuanTriVien *qtv)
{
    return quanLyNhanVien->themNhanVien(qtv);
}

QuanTriVien *HeThongQuanLy::timQuanTriVien(const string &ma)
{
    return quanLyNhanVien->timQuanTriVienTheoUsername(ma);
}

const MangDong<QuanTriVien *> &HeThongQuanLy::layDanhSachQuanTriVien() const
{
    return quanLyNhanVien->layDanhSachQuanTriVien();
}

// ===== SÂN (Delegate to QuanLySan) =====
bool HeThongQuanLy::themSan(San *san)
{
    return quanLySan->themSan(san);
}

bool HeThongQuanLy::xoaSan(const string &ma)
{
    return quanLySan->xoaSan(ma);
}

bool HeThongQuanLy::capNhatSan(const string &ma, const San &sanMoi)
{
    return quanLySan->capNhatSan(ma, sanMoi);
}

San *HeThongQuanLy::timSan(const string &ma)
{
    return quanLySan->timSan(ma);
}

const MangDong<San *> &HeThongQuanLy::layDanhSachSan() const
{
    return quanLySan->layDanhSachSan();
}

void HeThongQuanLy::hienThiDanhSachSan() const
{
    quanLySan->hienThiDanhSachSan();
}

MangDong<San *> HeThongQuanLy::timSanTheoLoai(const string &loai)
{
    return quanLySan->timSanTheoLoai(loai);
}

MangDong<San *> HeThongQuanLy::timSanTrong(const NgayGio &thoiGian, const KhungGio &khung)
{
    return quanLySan->timSanTrong(thoiGian, khung);
}

// ===== DỊCH VỤ (Delegate to QuanLyDichVu) =====
bool HeThongQuanLy::themDichVu(DichVu *dv)
{
    return quanLyDichVu->themDichVu(dv);
}

bool HeThongQuanLy::xoaDichVu(const string &ma)
{
    return quanLyDichVu->xoaDichVu(ma);
}

bool HeThongQuanLy::capNhatDichVu(const string &ma, const DichVu &dvMoi)
{
    return quanLyDichVu->capNhatDichVu(ma, dvMoi);
}

DichVu *HeThongQuanLy::timDichVu(const string &ma)
{
    return quanLyDichVu->timDichVu(ma);
}

const MangDong<DichVu *> &HeThongQuanLy::layDanhSachDichVu() const
{
    return quanLyDichVu->layDanhSachDichVu();
}

void HeThongQuanLy::hienThiDanhSachDichVu() const
{
    quanLyDichVu->hienThiDanhSachDichVu();
}

// ===== ĐƠN HÀNG DỊCH VỤ (Delegate to QuanLyDonHangDichVu) =====
DonHangDichVu *HeThongQuanLy::taoDonHangDichVu(KhachHang *kh)
{
    return quanLyDonHangDichVu->taoDonHang(kh);
}

bool HeThongQuanLy::huyDonHangDichVu(const string &maDH)
{
    return quanLyDonHangDichVu->huyDonHang(maDH);
}

bool HeThongQuanLy::capNhatTrangThaiDonHang(const string &maDH, TrangThaiDonHang trangThai)
{
    return quanLyDonHangDichVu->capNhatTrangThai(maDH, trangThai);
}

DonHangDichVu *HeThongQuanLy::timDonHangDichVu(const string &maDH)
{
    return quanLyDonHangDichVu->timDonHang(maDH);
}

const MangDong<DonHangDichVu *> &HeThongQuanLy::layDanhSachDonHangDichVu() const
{
    return quanLyDonHangDichVu->layDanhSachDonHang();
}

void HeThongQuanLy::hienThiDanhSachDonHangDichVu() const
{
    quanLyDonHangDichVu->hienThiDanhSachDonHang();
}

double HeThongQuanLy::tongDoanhThuDichVu() const
{
    return quanLyDonHangDichVu->tongDoanhThu();
}

// ===== ĐẶT SÂN (Delegate to QuanLyDatSan) =====
DatSan *HeThongQuanLy::taoDatSan(KhachHang *kh, San *san, const NgayGio &thoiGian, const KhungGio &khung)
{
    return quanLyDatSan->taoDatSan(kh, san, thoiGian, khung);
}

bool HeThongQuanLy::huyDatSan(const string &maDatSan)
{
    return quanLyDatSan->huyDatSan(maDatSan);
}

bool HeThongQuanLy::capNhatTrangThaiDatSan(const string &maDatSan, TrangThaiDatSan trangThai)
{
    return quanLyDatSan->capNhatTrangThaiDatSan(maDatSan, trangThai);
}

DatSan *HeThongQuanLy::timDatSan(const string &ma)
{
    return quanLyDatSan->timDatSan(ma);
}

const MangDong<DatSan *> &HeThongQuanLy::layDanhSachDatSan() const
{
    return quanLyDatSan->layDanhSachDatSan();
}

void HeThongQuanLy::hienThiDanhSachDatSan() const
{
    quanLyDatSan->hienThiDanhSachDatSan();
}

MangDong<DatSan *> HeThongQuanLy::timDatSanTheoKhachHang(const string &maKH)
{
    return quanLyDatSan->timDatSanTheoKhachHang(maKH);
}

MangDong<DatSan *> HeThongQuanLy::timDatSanTheoSan(const string &maSan)
{
    return quanLyDatSan->timDatSanTheoSan(maSan);
}

MangDong<DatSan *> HeThongQuanLy::timDatSanTheoNgay(const NgayThang &ngay)
{
    return quanLyDatSan->timDatSanTheoNgay(ngay);
}

bool HeThongQuanLy::kiemTraSanTrong(San *san, const NgayGio &thoiGian, const KhungGio &khung)
{
    return quanLyDatSan->kiemTraTrungLich(san, thoiGian, khung) == false;
}

// ===== THANH TOÁN (Delegate to QuanLyThanhToan) =====
ThanhToan *HeThongQuanLy::taoThanhToan(DatSan *datSan, PhuongThucThanhToan phuongThuc)
{
    return quanLyThanhToan->taoThanhToan(datSan, phuongThuc);
}

bool HeThongQuanLy::xoaThanhToan(const string &ma)
{
    return quanLyThanhToan->xoaThanhToan(ma);
}

ThanhToan *HeThongQuanLy::timThanhToan(const string &ma)
{
    return quanLyThanhToan->timThanhToan(ma);
}

ThanhToan *HeThongQuanLy::timThanhToanTheoDatSan(const string &maDatSan)
{
    return quanLyThanhToan->timThanhToanTheoDatSan(maDatSan);
}

const MangDong<ThanhToan *> &HeThongQuanLy::layDanhSachThanhToan() const
{
    return quanLyThanhToan->layDanhSachThanhToan();
}

void HeThongQuanLy::hienThiDanhSachThanhToan() const
{
    quanLyThanhToan->hienThiDanhSachThanhToan();
}

// ===== FILE I/O =====
bool HeThongQuanLy::luuHeThong(const string &tenFile)
{
    // Save each manager to separate CSV files
    bool success = true;

    if (!luuKhachHangCSV("khachhang.csv")) { cout << "Failed to save khachhang.csv" << endl; success = false; }
    if (!luuSanCSV("san.csv")) { cout << "Failed to save san.csv" << endl; success = false; }
    if (!luuDichVuCSV("dichvu.csv")) { cout << "Failed to save dichvu.csv" << endl; success = false; }
    if (!luuDatSanCSV("datsan.csv")) { cout << "Failed to save datsan.csv" << endl; success = false; }
    
    // Use CSV for DonHangDichVu
    if (!quanLyDonHangDichVu->luuCSV("donhangdichvu.csv")) { cout << "Failed to save donhangdichvu.csv" << endl; success = false; }

    if (!luuQuanTriVienCSV("admin.csv")) { cout << "Failed to save admin.csv" << endl; success = false; }

    if (success)
    {
        cout << "Luu he thong thanh cong (CSV format)" << endl;
    }
    else
    {
        cout << "Loi: Khong the luu mot so file CSV" << endl;
    }

    return success;
}

bool HeThongQuanLy::docHeThong(const string &tenFile)
{
    cout << "\n[DEBUG] docHeThong() called" << endl;
    // Load each manager from separate CSV files
    bool success = true;

    success &= docKhachHangCSV("khachhang.csv");
    success &= docSanCSV("san.csv");
    success &= docDichVuCSV("dichvu.csv");
    success &= docNhanVienCSV("nhanvien.csv");
    success &= docDatSanCSV("datsan.csv");
    
    // Use CSV for DonHangDichVu
    success &= quanLyDonHangDichVu->docCSV("donhangdichvu.csv");

    success &= docQuanTriVienCSV("admin.csv");

    if (success)
    {
        cout << "Doc he thong thanh cong (CSV format)" << endl;
    }
    else
    {
        cout << "Loi: Khong the doc mot so file CSV" << endl;
    }

    return success;
}

void HeThongQuanLy::xoaTatCaDuLieu()
{
    // Xóa dữ liệu trong các Manager
    if (quanLyKhachHang != nullptr)
        quanLyKhachHang->xoaTatCa();

    if (quanLySan != nullptr)
        quanLySan->xoaTatCa();

    if (quanLyDichVu != nullptr)
        quanLyDichVu->xoaTatCa();

    if (quanLyDonHangDichVu != nullptr)
        quanLyDonHangDichVu->xoaTatCa();

    if (quanLyDatSan != nullptr)
        quanLyDatSan->xoaTatCa();

    if (quanLyThanhToan != nullptr)
        quanLyThanhToan->xoaTatCa();

    // Xóa nhân viên
    // Handled by quanLyNhanVien->xoaTatCa()

    // Xóa quản trị viên
    // Handled by quanLyNhanVien->xoaTatCa()
}

// ===== BACKUP/RESTORE =====
bool HeThongQuanLy::saoLuuHeThong(const string &fileGoc)
{
    return backupManager->saoLuu(fileGoc);
}

bool HeThongQuanLy::khoiPhucHeThong(const string &fileBackup, const string &fileDich)
{
    return backupManager->khoiPhuc(fileBackup, fileDich);
}

// ===== UTILITY =====
int HeThongQuanLy::tongSoKhachHang() const
{
    return quanLyKhachHang->tongSoKhachHang();
}

int HeThongQuanLy::tongSoNhanVien() const
{
    return quanLyNhanVien->tongSoNhanVien();
}

int HeThongQuanLy::tongSoSan() const
{
    return quanLySan->tongSoSan();
}

int HeThongQuanLy::tongSoDichVu() const
{
    return quanLyDichVu->tongSoDichVu();
}

int HeThongQuanLy::tongSoDonHangDichVu() const
{
    return quanLyDonHangDichVu->tongSoDonHang();
}

int HeThongQuanLy::tongSoDatSan() const
{
    return quanLyDatSan->tongSoDatSan();
}

int HeThongQuanLy::tongSoThanhToan() const
{
    return quanLyThanhToan->tongSoThanhToan();
}

// ===== SAMPLE DATA INITIALIZATION =====
void HeThongQuanLy::khoiTaoDuLieuMau()
{
    cout << "\n=== SAMPLE DATA GENERATION DISABLED ===" << endl;
    // Data generation code removed as per request
}
