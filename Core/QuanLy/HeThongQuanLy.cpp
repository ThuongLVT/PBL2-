/**
 * @file HeThongQuanLy.cpp
 * @brief Triển khai HeThongQuanLy - Coordinator pattern
 * @details HeThongQuanLy giờ chỉ delegate cho các Manager classes
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#include "HeThongQuanLy.h"
#include <iostream>
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
    quanLyDichVu = new QuanLyDichVu();
    quanLyDonHangDichVu = new QuanLyDonHangDichVu();
    quanLyDatSan = new QuanLyDatSan();
    quanLyThanhToan = new QuanLyThanhToan();
    backupManager = new BackupManager();

    // Tải dữ liệu dịch vụ từ CSV (path tuyệt đối)
    quanLyDichVu->taiDuLieuTuCSV("D:/QT_PBL2/Data/dichvu.csv");
}

// Destructor
HeThongQuanLy::~HeThongQuanLy()
{
    xoaTatCaDuLieu();

    // Xóa các Manager
    delete quanLySan;
    delete quanLyKhachHang;
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
    if (nv == nullptr || timNhanVien(nv->layMaNhanVien()) != nullptr)
        return false;
    danhSachNhanVien.push_back(nv);
    return true;
}

bool HeThongQuanLy::xoaNhanVien(const string &ma)
{
    for (int i = 0; i < danhSachNhanVien.size(); i++)
    {
        if (danhSachNhanVien[i]->layMaNhanVien() == ma)
        {
            delete danhSachNhanVien[i];
            danhSachNhanVien.erase(i);
            return true;
        }
    }
    return false;
}

bool HeThongQuanLy::capNhatNhanVien(const string &ma, const NhanVien &nvMoi)
{
    NhanVien *nv = timNhanVien(ma);
    if (nv == nullptr)
        return false;
    *nv = nvMoi;
    return true;
}

NhanVien *HeThongQuanLy::timNhanVien(const string &ma)
{
    for (int i = 0; i < danhSachNhanVien.size(); i++)
    {
        if (danhSachNhanVien[i]->layMaNhanVien() == ma)
            return danhSachNhanVien[i];
    }
    return nullptr;
}

const MangDong<NhanVien *> &HeThongQuanLy::layDanhSachNhanVien() const
{
    return danhSachNhanVien;
}

void HeThongQuanLy::hienThiDanhSachNhanVien() const
{
    cout << "\n===== DANH SACH NHAN VIEN =====" << endl;
    cout << "Tong so: " << danhSachNhanVien.size() << endl;
    for (int i = 0; i < danhSachNhanVien.size(); i++)
    {
        danhSachNhanVien[i]->hienThiThongTin();
        cout << "---" << endl;
    }
}

// ===== QUẢN TRỊ VIÊN (Quản lý trực tiếp) =====
bool HeThongQuanLy::themQuanTriVien(QuanTriVien *qtv)
{
    if (qtv == nullptr || timQuanTriVien(qtv->layTenDangNhap()) != nullptr)
        return false;
    danhSachQuanTriVien.push_back(qtv);
    return true;
}

QuanTriVien *HeThongQuanLy::timQuanTriVien(const string &ma)
{
    for (int i = 0; i < danhSachQuanTriVien.size(); i++)
    {
        if (danhSachQuanTriVien[i]->layTenDangNhap() == ma)
            return danhSachQuanTriVien[i];
    }
    return nullptr;
}

const MangDong<QuanTriVien *> &HeThongQuanLy::layDanhSachQuanTriVien() const
{
    return danhSachQuanTriVien;
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
    ofstream file(tenFile, ios::binary);
    if (!file.is_open())
    {
        cout << "Loi: Khong the mo file de ghi: " << tenFile << endl;
        return false;
    }

    // Ghi từng Manager
    if (!quanLyKhachHang->ghiFile(file))
    {
        cout << "Loi: Khong the ghi QuanLyKhachHang" << endl;
        return false;
    }

    if (!quanLySan->ghiFile(file))
    {
        cout << "Loi: Khong the ghi QuanLySan" << endl;
        return false;
    }

    if (!quanLyDichVu->ghiFile(file))
    {
        cout << "Loi: Khong the ghi QuanLyDichVu" << endl;
        return false;
    }

    if (!quanLyDatSan->ghiFile(file))
    {
        cout << "Loi: Khong the ghi QuanLyDatSan" << endl;
        return false;
    }

    // TODO Week 1 Day 1: Convert QuanLyThanhToan to std::ofstream or write payments directly
    // Temporary: Write payments count and skip detailed data
    int soThanhToan = quanLyThanhToan->tongSoThanhToan();
    file.write(reinterpret_cast<const char *>(&soThanhToan), sizeof(int));
    // if (!quanLyThanhToan->ghiFile(file))
    // {
    //     cout << "Loi: Khong the ghi QuanLyThanhToan" << endl;
    //     return false;
    // }

    // Ghi Nhân viên & Quản trị viên
    int soNV = danhSachNhanVien.size();
    file.write(reinterpret_cast<const char *>(&soNV), sizeof(soNV));
    for (int i = 0; i < soNV; i++)
    {
        if (!danhSachNhanVien[i]->ghiFile(file))
            return false;
    }

    int soQTV = danhSachQuanTriVien.size();
    file.write(reinterpret_cast<const char *>(&soQTV), sizeof(soQTV));
    for (int i = 0; i < soQTV; i++)
    {
        if (!danhSachQuanTriVien[i]->ghiFile(file))
            return false;
    }

    file.close();
    cout << "Luu he thong thanh cong: " << tenFile << endl;
    return true;
}

bool HeThongQuanLy::docHeThong(const string &tenFile)
{
    ifstream file(tenFile, ios::binary);
    if (!file.is_open())
    {
        cout << "Loi: Khong the mo file de doc: " << tenFile << endl;
        return false;
    }

    // Xóa dữ liệu cũ
    xoaTatCaDuLieu();

    // Đọc từng Manager
    if (!quanLyKhachHang->docFile(file))
    {
        cout << "Loi: Khong the doc QuanLyKhachHang" << endl;
        return false;
    }

    if (!quanLySan->docFile(file))
    {
        cout << "Loi: Khong the doc QuanLySan" << endl;
        return false;
    }

    if (!quanLyDichVu->docFile(file))
    {
        cout << "Loi: Khong the doc QuanLyDichVu" << endl;
        return false;
    }

    if (!quanLyDatSan->docFile(file))
    {
        cout << "Loi: Khong the doc QuanLyDatSan" << endl;
        return false;
    }

    // TODO Week 1 Day 1: Read payments properly after conversion
    // Temporary: Skip reading payment details
    int soThanhToan;
    file.read(reinterpret_cast<char *>(&soThanhToan), sizeof(int));
    // if (!quanLyThanhToan->docFile(file))
    // {
    //     cout << "Loi: Khong the doc QuanLyThanhToan" << endl;
    //     return false;
    // }

    // Đọc Nhân viên
    int soNV;
    file.read(reinterpret_cast<char *>(&soNV), sizeof(soNV));
    for (int i = 0; i < soNV; i++)
    {
        NhanVien *nv = new NhanVien();
        if (!nv->docFile(file))
        {
            delete nv;
            return false;
        }
        danhSachNhanVien.push_back(nv);
    }

    // Đọc Quản trị viên
    int soQTV;
    file.read(reinterpret_cast<char *>(&soQTV), sizeof(soQTV));
    for (int i = 0; i < soQTV; i++)
    {
        QuanTriVien *qtv = new QuanTriVien();
        if (!qtv->docFile(file))
        {
            delete qtv;
            return false;
        }
        danhSachQuanTriVien.push_back(qtv);
    }

    file.close();
    cout << "Doc he thong thanh cong: " << tenFile << endl;
    return true;
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
    for (int i = 0; i < danhSachNhanVien.size(); i++)
    {
        delete danhSachNhanVien[i];
    }
    danhSachNhanVien = MangDong<NhanVien *>();

    // Xóa quản trị viên
    for (int i = 0; i < danhSachQuanTriVien.size(); i++)
    {
        delete danhSachQuanTriVien[i];
    }
    danhSachQuanTriVien = MangDong<QuanTriVien *>();
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
    return danhSachNhanVien.size();
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

// ===== CSV I/O =====
bool HeThongQuanLy::luuCSV(const string &dataDir)
{
    cout << "\n=== SAVING DATA TO CSV ===" << endl;
    cout << "Data directory: " << dataDir << endl;

    bool success = true;

    // Save customers
    string khachhangFile = dataDir + "/khachhang.csv";
    if (!quanLyKhachHang->luuCSV(khachhangFile))
    {
        cerr << "Failed to save customers to CSV" << endl;
        success = false;
    }

    // Save services
    string dichvuFile = dataDir + "/dichvu.csv";
    if (!quanLyDichVu->luuDuLieuRaCSV(dichvuFile))
    {
        cerr << "Failed to save services to CSV" << endl;
        success = false;
    }

    // TODO: Add other entities (San, DatSan, ThanhToan, NhanVien, QuanTriVien)

    if (success)
    {
        cout << "✓ All data saved to CSV successfully!" << endl;
    }

    return success;
}

bool HeThongQuanLy::docCSV(const string &dataDir)
{
    cout << "\n=== LOADING DATA FROM CSV ===" << endl;
    cout << "Data directory: " << dataDir << endl;

    bool success = true;

    // Load customers
    string khachhangFile = dataDir + "/khachhang.csv";
    if (!quanLyKhachHang->docCSV(khachhangFile))
    {
        cerr << "Failed to load customers from CSV" << endl;
        success = false;
    }

    // Load fields (san)
    string sanFile = dataDir + "/san.csv";
    if (!quanLySan->loadFromCSV(sanFile))
    {
        cerr << "Failed to load fields from CSV" << endl;
        success = false;
    }

    // TODO: Add other entities (DichVu, DatSan, ThanhToan, NhanVien, QuanTriVien)

    if (success)
    {
        cout << "✓ All data loaded from CSV successfully!" << endl;
    }

    return success;
}
