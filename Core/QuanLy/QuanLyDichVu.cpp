/**
 * @file QuanLyDichVu.cpp
 * @brief Triển khai các phương thức của lớp QuanLyDichVu
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#include "QuanLyDichVu.h"
#include <iostream>

using namespace std;

QuanLyDichVu::QuanLyDichVu() {}

QuanLyDichVu::~QuanLyDichVu()
{
    xoaTatCa();
}

bool QuanLyDichVu::themDichVu(DichVu *dv)
{
    if (dv == nullptr || timDichVu(dv->layMaDichVu()) != nullptr)
    {
        return false;
    }
    danhSachDichVu.push_back(dv);
    return true;
}

bool QuanLyDichVu::xoaDichVu(const string &maDV)
{
    for (int i = 0; i < danhSachDichVu.size(); i++)
    {
        if (danhSachDichVu[i]->layMaDichVu() == maDV)
        {
            delete danhSachDichVu[i];
            danhSachDichVu.erase(i);
            return true;
        }
    }
    return false;
}

bool QuanLyDichVu::capNhatDichVu(const string &maDV, const DichVu &dvMoi)
{
    DichVu *dv = timDichVu(maDV);
    if (dv == nullptr)
        return false;
    *dv = dvMoi;
    return true;
}

DichVu *QuanLyDichVu::timDichVu(const string &maDV)
{
    for (int i = 0; i < danhSachDichVu.size(); i++)
    {
        if (danhSachDichVu[i]->layMaDichVu() == maDV)
        {
            return danhSachDichVu[i];
        }
    }
    return nullptr;
}

const MangDong<DichVu *> &QuanLyDichVu::layDanhSachDichVu() const
{
    return danhSachDichVu;
}

MangDong<DichVu *> QuanLyDichVu::timDichVuTheoLoai(LoaiDichVu loai)
{
    MangDong<DichVu *> ketQua;
    for (int i = 0; i < danhSachDichVu.size(); i++)
    {
        if (danhSachDichVu[i]->layLoaiDichVu() == loai)
        {
            ketQua.push_back(danhSachDichVu[i]);
        }
    }
    return ketQua;
}

MangDong<DichVu *> QuanLyDichVu::timDichVuConHang()
{
    MangDong<DichVu *> ketQua;
    for (int i = 0; i < danhSachDichVu.size(); i++)
    {
        if (danhSachDichVu[i]->coConHang())
        {
            ketQua.push_back(danhSachDichVu[i]);
        }
    }
    return ketQua;
}

void QuanLyDichVu::hienThiDanhSachDichVu() const
{
    cout << "\n===== DANH SACH DICH VU =====" << endl;
    cout << "Tong so: " << danhSachDichVu.size() << endl;
    for (int i = 0; i < danhSachDichVu.size(); i++)
    {
        cout << "\n[" << (i + 1) << "] ";
        danhSachDichVu[i]->hienThiThongTin();
    }
}

int QuanLyDichVu::tongSoDichVu() const
{
    return danhSachDichVu.size();
}

bool QuanLyDichVu::ghiFile(ofstream &file) const
{
    if (!file.is_open())
        return false;

    int soLuong = danhSachDichVu.size();
    file.write(reinterpret_cast<const char *>(&soLuong), sizeof(soLuong));

    for (int i = 0; i < soLuong; i++)
    {
        if (!danhSachDichVu[i]->ghiFile(file))
            return false;
    }
    return file.good();
}

bool QuanLyDichVu::docFile(ifstream &file)
{
    if (!file.is_open())
        return false;

    xoaTatCa();

    int soLuong;
    file.read(reinterpret_cast<char *>(&soLuong), sizeof(soLuong));

    for (int i = 0; i < soLuong; i++)
    {
        DichVu *dv = new DichVu();
        if (!dv->docFile(file))
        {
            delete dv;
            return false;
        }
        danhSachDichVu.push_back(dv);
    }
    return file.good();
}

void QuanLyDichVu::xoaTatCa()
{
    for (int i = 0; i < danhSachDichVu.size(); i++)
    {
        delete danhSachDichVu[i];
    }
    danhSachDichVu = MangDong<DichVu *>();
}
