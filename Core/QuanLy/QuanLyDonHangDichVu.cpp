/**
 * @file QuanLyDonHangDichVu.cpp
 * @brief Implementation của lớp QuanLyDonHangDichVu
 */

#include "QuanLyDonHangDichVu.h"
#include <iostream>

using namespace std;

// ========== CONSTRUCTORS ==========

QuanLyDonHangDichVu::QuanLyDonHangDichVu()
{
}

QuanLyDonHangDichVu::~QuanLyDonHangDichVu()
{
    xoaTatCa();
}

// ========== CRUD OPERATIONS ==========

DonHangDichVu *QuanLyDonHangDichVu::taoDonHang(KhachHang *kh)
{
    // Tạo mã đơn hàng tự động
    string maDH = DonHangDichVu::taoMaDonHang();

    // Tạo đơn hàng mới
    DonHangDichVu *donHang = new DonHangDichVu(maDH, kh);

    // Thêm vào danh sách
    danhSachDonHang.push_back(donHang);

    cout << "Da tao don hang: " << maDH << endl;
    return donHang;
}

bool QuanLyDonHangDichVu::huyDonHang(const string &maDH)
{
    DonHangDichVu *donHang = timDonHang(maDH);
    if (donHang != nullptr)
    {
        donHang->setTrangThai(TrangThaiDonHang::DA_HUY);
        cout << "Da huy don hang: " << maDH << endl;
        return true;
    }

    cout << "Khong tim thay don hang: " << maDH << endl;
    return false;
}

bool QuanLyDonHangDichVu::capNhatTrangThai(const string &maDH, TrangThaiDonHang trangThai)
{
    DonHangDichVu *donHang = timDonHang(maDH);
    if (donHang != nullptr)
    {
        donHang->setTrangThai(trangThai);
        cout << "Da cap nhat trang thai don hang: " << maDH << endl;
        return true;
    }

    cout << "Khong tim thay don hang: " << maDH << endl;
    return false;
}

DonHangDichVu *QuanLyDonHangDichVu::timDonHang(const string &maDH)
{
    for (int i = 0; i < danhSachDonHang.size(); i++)
    {
        if (danhSachDonHang[i]->getMaDonHang() == maDH)
        {
            return danhSachDonHang[i];
        }
    }
    return nullptr;
}

const MangDong<DonHangDichVu *> &QuanLyDonHangDichVu::layDanhSachDonHang() const
{
    return danhSachDonHang;
}

// ========== SEARCH ==========

MangDong<DonHangDichVu *> QuanLyDonHangDichVu::timDonHangTheoKhachHang(const string &maKH)
{
    MangDong<DonHangDichVu *> ketQua;

    for (int i = 0; i < danhSachDonHang.size(); i++)
    {
        if (danhSachDonHang[i]->getMaKhachHang() == maKH)
        {
            ketQua.push_back(danhSachDonHang[i]);
        }
    }

    return ketQua;
}

MangDong<DonHangDichVu *> QuanLyDonHangDichVu::timDonHangTheoNgay(const NgayThang &ngay)
{
    MangDong<DonHangDichVu *> ketQua;

    for (int i = 0; i < danhSachDonHang.size(); i++)
    {
        NgayGio ngayTao = danhSachDonHang[i]->getNgayTao();
        NgayThang ngayDonHang = ngayTao.getNgayThang();

        if (ngayDonHang == ngay)
        {
            ketQua.push_back(danhSachDonHang[i]);
        }
    }

    return ketQua;
}

MangDong<DonHangDichVu *> QuanLyDonHangDichVu::timDonHangTheoTrangThai(TrangThaiDonHang trangThai)
{
    MangDong<DonHangDichVu *> ketQua;

    for (int i = 0; i < danhSachDonHang.size(); i++)
    {
        if (danhSachDonHang[i]->getTrangThai() == trangThai)
        {
            ketQua.push_back(danhSachDonHang[i]);
        }
    }

    return ketQua;
}

// ========== DISPLAY ==========

void QuanLyDonHangDichVu::hienThiDanhSachDonHang() const
{
    cout << "\n===== DANH SACH DON HANG DICH VU =====" << endl;
    cout << "Tong so: " << danhSachDonHang.size() << endl;

    for (int i = 0; i < danhSachDonHang.size(); i++)
    {
        cout << "\n[" << (i + 1) << "] ";
        danhSachDonHang[i]->hienThi();
        cout << "---" << endl;
    }
}

// ========== STATISTICS ==========

int QuanLyDonHangDichVu::tongSoDonHang() const
{
    return danhSachDonHang.size();
}

double QuanLyDonHangDichVu::tongDoanhThu() const
{
    double tongDT = 0;

    for (int i = 0; i < danhSachDonHang.size(); i++)
    {
        // Chỉ tính đơn hàng hoàn thành
        if (danhSachDonHang[i]->getTrangThai() == TrangThaiDonHang::HOAN_THANH)
        {
            tongDT += danhSachDonHang[i]->getThanhTien();
        }
    }

    return tongDT;
}

double QuanLyDonHangDichVu::doanhThuTheoNgay(const NgayThang &ngay) const
{
    double doanhThu = 0;

    for (int i = 0; i < danhSachDonHang.size(); i++)
    {
        NgayGio ngayTao = danhSachDonHang[i]->getNgayTao();
        NgayThang ngayDonHang = ngayTao.getNgayThang();

        if (ngayDonHang == ngay &&
            danhSachDonHang[i]->getTrangThai() == TrangThaiDonHang::HOAN_THANH)
        {
            doanhThu += danhSachDonHang[i]->getThanhTien();
        }
    }

    return doanhThu;
}

// ========== FILE I/O ==========

bool QuanLyDonHangDichVu::ghiFile(std::ofstream &file) const
{
    if (!file.is_open())
    {
        cout << "Loi: Khong the mo file de ghi!" << endl;
        return false;
    }

    // Ghi số lượng đơn hàng
    int soLuong = danhSachDonHang.size();
    file.write(reinterpret_cast<const char *>(&soLuong), sizeof(int));

    // Ghi từng đơn hàng
    // TODO: Implement proper binary file I/O for DonHangDichVu
    for (int i = 0; i < soLuong; i++)
    {
        // Temporary: Skip writing individual orders until file I/O is properly implemented
        // danhSachDonHang[i]->ghiFile(f);
    }

    cout << "Da ghi " << soLuong << " don hang vao file (chi so luong)." << endl;
    return true;
}

bool QuanLyDonHangDichVu::docFile(std::ifstream &file)
{
    if (!file.is_open())
    {
        cout << "Loi: Khong the mo file de doc!" << endl;
        return false;
    }

    // Xóa dữ liệu cũ
    xoaTatCa();

    // Đọc số lượng đơn hàng
    int soLuong;
    file.read(reinterpret_cast<char *>(&soLuong), sizeof(int));

    // Đọc từng đơn hàng
    // TODO: Implement proper binary file I/O for DonHangDichVu
    for (int i = 0; i < soLuong; i++)
    {
        // Temporary: Skip reading individual orders until file I/O is properly implemented
        // DonHangDichVu *donHang = new DonHangDichVu();
        // donHang->docFile(f);
        // danhSachDonHang.push_back(donHang);
    }

    cout << "Da doc thong tin don hang tu file (chi so luong: " << soLuong << ")." << endl;
    return true;
}

void QuanLyDonHangDichVu::xoaTatCa()
{
    for (int i = 0; i < danhSachDonHang.size(); i++)
    {
        delete danhSachDonHang[i];
    }
    danhSachDonHang.clear();
}
