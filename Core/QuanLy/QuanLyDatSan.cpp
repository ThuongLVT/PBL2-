/**
 * @file QuanLyDatSan.cpp
 * @brief Triển khai các phương thức của lớp QuanLyDatSan
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#include "QuanLyDatSan.h"
#include <iostream>

using namespace std;

QuanLyDatSan::QuanLyDatSan() {}

QuanLyDatSan::~QuanLyDatSan()
{
    xoaTatCa();
}

DatSan *QuanLyDatSan::taoDatSan(KhachHang *kh, San *san, const NgayGio &thoiGian, const KhungGio &khung)
{
    if (kh == nullptr || san == nullptr)
    {
        return nullptr;
    }

    // Kiểm tra trùng lịch
    if (kiemTraTrungLich(san, thoiGian, khung))
    {
        cout << "San da co nguoi dat trong khung gio nay!" << endl;
        return nullptr;
    }

    // Tạo mã đặt sân tự động
    static int soThuTu = 1;
    string maDatSan = "DS" + to_string(soThuTu++);

    DatSan *datSan = new DatSan(maDatSan, kh, san, thoiGian, khung);
    danhSachDatSan.push_back(datSan);

    return datSan;
}

bool QuanLyDatSan::huyDatSan(const string &maDatSan)
{
    DatSan *datSan = timDatSan(maDatSan);
    if (datSan == nullptr)
    {
        return false;
    }

    datSan->setTrangThai(TrangThaiDatSan::DA_HUY);
    return true;
}

bool QuanLyDatSan::capNhatTrangThaiDatSan(const string &maDatSan, TrangThaiDatSan trangThai)
{
    DatSan *datSan = timDatSan(maDatSan);
    if (datSan == nullptr)
    {
        return false;
    }

    datSan->setTrangThai(trangThai);
    return true;
}

DatSan *QuanLyDatSan::timDatSan(const string &maDatSan)
{
    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        if (danhSachDatSan[i]->getMaDatSan() == maDatSan)
        {
            return danhSachDatSan[i];
        }
    }
    return nullptr;
}

const MangDong<DatSan *> &QuanLyDatSan::layDanhSachDatSan() const
{
    return danhSachDatSan;
}

MangDong<DatSan *> QuanLyDatSan::timDatSanTheoKhachHang(const string &maKH)
{
    MangDong<DatSan *> ketQua;
    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        if (danhSachDatSan[i]->getKhachHang()->getMaNguoiDung() == maKH)
        {
            ketQua.push_back(danhSachDatSan[i]);
        }
    }
    return ketQua;
}

MangDong<DatSan *> QuanLyDatSan::timDatSanTheoSan(const string &maSan)
{
    MangDong<DatSan *> ketQua;
    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        if (danhSachDatSan[i]->getSan()->layMaSan() == maSan)
        {
            ketQua.push_back(danhSachDatSan[i]);
        }
    }
    return ketQua;
}

MangDong<DatSan *> QuanLyDatSan::timDatSanTheoNgay(const NgayThang &ngay)
{
    MangDong<DatSan *> ketQua;
    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        NgayGio thoiGian = danhSachDatSan[i]->getThoiGianDat();
        if (thoiGian.getNgay() == ngay.getNgay() &&
            thoiGian.getThang() == ngay.getThang() &&
            thoiGian.getNam() == ngay.getNam())
        {
            ketQua.push_back(danhSachDatSan[i]);
        }
    }
    return ketQua;
}

MangDong<DatSan *> QuanLyDatSan::timDatSanTheoTrangThai(TrangThaiDatSan trangThai)
{
    MangDong<DatSan *> ketQua;
    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        if (danhSachDatSan[i]->getTrangThai() == trangThai)
        {
            ketQua.push_back(danhSachDatSan[i]);
        }
    }
    return ketQua;
}

bool QuanLyDatSan::kiemTraTrungLich(San *san, const NgayGio &thoiGian, const KhungGio &khung)
{
    if (san == nullptr)
    {
        return false;
    }

    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        DatSan *ds = danhSachDatSan[i];

        // Chỉ kiểm tra đặt sân cùng sân và chưa hủy/hoàn thành
        if (ds->getSan()->layMaSan() != san->layMaSan())
            continue;

        if (ds->getTrangThai() == TrangThaiDatSan::DA_HUY ||
            ds->getTrangThai() == TrangThaiDatSan::HOAN_THANH)
            continue;

        // Kiểm tra trùng ngày
        NgayGio tgDat = ds->getThoiGianDat();
        if (tgDat.getNgay() != thoiGian.getNgay() ||
            tgDat.getThang() != thoiGian.getThang() ||
            tgDat.getNam() != thoiGian.getNam())
            continue;

        // Kiểm tra trùng khung giờ
        KhungGio kgDat = ds->getKhungGio();
        if (khung.layGioBatDau() == kgDat.layGioBatDau() &&
            khung.layGioKetThuc() == kgDat.layGioKetThuc())
        {
            return true; // Trùng lịch
        }

        // Kiểm tra overlap (khung giờ mới bắt đầu trong khoảng khung giờ đã đặt)
        if (khung.kiemTraTrung(kgDat))
        {
            return true; // Trùng lịch
        }
    }

    return false; // Không trùng
}

void QuanLyDatSan::hienThiDanhSachDatSan() const
{
    cout << "\n===== DANH SACH DAT SAN =====" << endl;
    cout << "Tong so: " << danhSachDatSan.size() << endl;
    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        cout << "\n[" << (i + 1) << "] ";
        danhSachDatSan[i]->hienThi();
    }
}

int QuanLyDatSan::tongSoDatSan() const
{
    return danhSachDatSan.size();
}

bool QuanLyDatSan::ghiFile(ofstream &file) const
{
    if (!file.is_open())
        return false;

    int soLuong = danhSachDatSan.size();
    file.write(reinterpret_cast<const char *>(&soLuong), sizeof(soLuong));

    // Skip FILE* writing for now - DatSan uses FILE* not ofstream
    // TODO: Fix serialization

    return file.good();
}

bool QuanLyDatSan::docFile(ifstream &file)
{
    if (!file.is_open())
        return false;

    xoaTatCa();

    int soLuong;
    file.read(reinterpret_cast<char *>(&soLuong), sizeof(soLuong));

    // Skip FILE* reading for now - DatSan uses FILE* not ifstream
    // TODO: Fix deserialization

    return file.good();
}

void QuanLyDatSan::xoaTatCa()
{
    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        delete danhSachDatSan[i];
    }
    danhSachDatSan = MangDong<DatSan *>();
}
