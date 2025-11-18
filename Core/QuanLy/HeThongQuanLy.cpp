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
    // Save each manager to separate CSV files
    bool success = true;

    success &= luuKhachHangCSV("khachhang.csv");
    success &= luuSanCSV("san.csv");
    success &= luuDichVuCSV("dichvu.csv");
    success &= luuDatSanCSV("datsan.csv");

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
    success &= docDatSanCSV("datsan.csv");

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

// ===== SAMPLE DATA INITIALIZATION =====
void HeThongQuanLy::khoiTaoDuLieuMau()
{
    cout << "\n=== INITIALIZING SAMPLE DATA ===" << endl;

    // 1. Khởi tạo 10 sân
    for (int i = 1; i <= 10; i++)
    {
        string maSan = string("S") + (i < 10 ? "0" : "") + to_string(i);
        string tenSan = "Sân " + to_string(i) + (i % 2 == 0 ? " (Sân 7)" : " (Sân 5)");
        LoaiSan loai = (i % 2 == 0) ? LoaiSan::SAN_7 : LoaiSan::SAN_5;
        KhuVuc khuVuc = static_cast<KhuVuc>((i - 1) % 4);    // A, B, C, D
        double giaThue = (i % 2 == 0) ? 300000.0 : 200000.0; // Sân 7: 300k, Sân 5: 200k

        San *san = new San(maSan, tenSan, loai, khuVuc, giaThue);
        san->datTrangThai(TrangThaiSan::HOAT_DONG); // Sân hoạt động
        quanLySan->themSan(san);
    }
    cout << "✓ Created 10 fields" << endl;

    // 2. Khởi tạo 10 khách hàng
    string hoTenList[] = {"Nguyễn Văn An", "Trần Thị Bình", "Lê Hoàng Cường", "Phạm Thị Dung",
                          "Hoàng Văn Em", "Vũ Thị Phượng", "Đặng Văn Giang", "Bùi Thị Hoa",
                          "Phan Văn Inh", "Ngô Thị Kim"};
    for (int i = 1; i <= 10; i++)
    {
        string maKH = string("KH") + (i < 10 ? "0" : "") + to_string(i);
        string hoTen = hoTenList[i - 1];
        string sdt = "09" + to_string(10000000 + i * 1111111).substr(0, 8);
        string diaChi = "Địa chỉ " + to_string(i);

        KhachHang *kh = new KhachHang(hoTen, sdt, diaChi, maKH);
        quanLyKhachHang->themKhachHang(kh);
    }
    cout << "✓ Created 10 customers" << endl;

    // 3. Khởi tạo 10 dịch vụ
    string dichVuList[] = {"Nước suối", "Nước ngọt", "Thuê bóng", "Thuê áo đấu", "Giày đá bóng",
                           "Băng bảo vệ", "Nước tăng lực", "Khăn lau", "Dép tắm", "Túi đựng đồ"};
    double giaList[] = {5000, 10000, 30000, 50000, 150000, 20000, 15000, 10000, 25000, 30000};
    LoaiDichVu loaiList[] = {LoaiDichVu::DO_UONG, LoaiDichVu::DO_UONG, LoaiDichVu::THIET_BI, LoaiDichVu::THIET_BI,
                             LoaiDichVu::THIET_BI, LoaiDichVu::THIET_BI, LoaiDichVu::DO_UONG, LoaiDichVu::THIET_BI,
                             LoaiDichVu::THIET_BI, LoaiDichVu::THIET_BI};

    for (int i = 1; i <= 10; i++)
    {
        string maDV = string("DV") + (i < 10 ? "0" : "") + to_string(i);
        DichVu *dv = new DichVu(maDV, dichVuList[i - 1], giaList[i - 1], loaiList[i - 1]);
        quanLyDichVu->themDichVu(dv);
    }
    cout << "✓ Created 10 services" << endl;

    // 4. Khởi tạo 5 booking mẫu cho ngày hôm nay
    if (quanLyKhachHang->layDanhSachKhachHang().size() >= 5 && quanLySan->layDanhSachSan().size() >= 5)
    {
        NgayGio ngayHienTai = NgayGio::layThoiGianHienTai();
        int nam = ngayHienTai.getNam();
        int thang = ngayHienTai.getThang();
        int ngay = ngayHienTai.getNgay();

        // Booking 1: 08:00-09:00, Sân 1, KH1
        KhachHang *kh1 = quanLyKhachHang->layDanhSachKhachHang()[0];
        San *san1 = quanLySan->layDanhSachSan()[0];
        NgayGio thoiGian1(8, 0, 0, ngay, thang, nam);
        KhungGio khung1(8, 0, 9, 0);
        DatSan *booking1 = quanLyDatSan->taoDatSan(kh1, san1, thoiGian1, khung1);
        if (booking1)
        {
            booking1->tinhTienCoc();
            cout << "✓ Created booking 1: 08:00-09:00, Field 1" << endl;
        }

        // Booking 2: 10:00-11:00, Sân 2, KH2
        KhachHang *kh2 = quanLyKhachHang->layDanhSachKhachHang()[1];
        San *san2 = quanLySan->layDanhSachSan()[1];
        NgayGio thoiGian2(10, 0, 0, ngay, thang, nam);
        KhungGio khung2(10, 0, 11, 0);
        DatSan *booking2 = quanLyDatSan->taoDatSan(kh2, san2, thoiGian2, khung2);
        if (booking2)
        {
            booking2->tinhTienCoc();
            cout << "✓ Created booking 2: 10:00-11:00, Field 2" << endl;
        }

        // Booking 3: 14:00-15:00, Sân 3, KH3
        KhachHang *kh3 = quanLyKhachHang->layDanhSachKhachHang()[2];
        San *san3 = quanLySan->layDanhSachSan()[2];
        NgayGio thoiGian3(14, 0, 0, ngay, thang, nam);
        KhungGio khung3(14, 0, 15, 0);
        DatSan *booking3 = quanLyDatSan->taoDatSan(kh3, san3, thoiGian3, khung3);
        if (booking3)
        {
            booking3->tinhTienCoc();
            cout << "✓ Created booking 3: 14:00-15:00, Field 3" << endl;
        }

        // Booking 4: 16:00-17:00, Sân 4, KH4
        KhachHang *kh4 = quanLyKhachHang->layDanhSachKhachHang()[3];
        San *san4 = quanLySan->layDanhSachSan()[3];
        NgayGio thoiGian4(16, 0, 0, ngay, thang, nam);
        KhungGio khung4(16, 0, 17, 0);
        DatSan *booking4 = quanLyDatSan->taoDatSan(kh4, san4, thoiGian4, khung4);
        if (booking4)
        {
            booking4->tinhTienCoc();
            cout << "✓ Created booking 4: 16:00-17:00, Field 4" << endl;
        }

        // Booking 5: 18:00-19:00, Sân 5, KH5
        KhachHang *kh5 = quanLyKhachHang->layDanhSachKhachHang()[4];
        San *san5 = quanLySan->layDanhSachSan()[4];
        NgayGio thoiGian5(18, 0, 0, ngay, thang, nam);
        KhungGio khung5(18, 0, 19, 0);
        DatSan *booking5 = quanLyDatSan->taoDatSan(kh5, san5, thoiGian5, khung5);
        if (booking5)
        {
            booking5->tinhTienCoc();
            cout << "✓ Created booking 5: 18:00-19:00, Field 5" << endl;
        }
    }

    cout << "=== SAMPLE DATA INITIALIZED SUCCESSFULLY ===" << endl;
}
