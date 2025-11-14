/**
 * @file QuanLyDichVu.cpp
 * @brief Triển khai các phương thức của lớp QuanLyDichVu
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#include "QuanLyDichVu.h"
#include <iostream>
#include <fstream>
#include <sstream>

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

bool QuanLyDichVu::taiDuLieuTuCSV(const std::string &filePath)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        cerr << "Khong the mo file: " << filePath << endl;
        return false;
    }

    string line;
    // Skip header line
    getline(file, line);

    int count = 0;
    while (getline(file, line))
    {
        if (line.empty()) continue;

        // Parse CSV line: MaDichVu,TenDichVu,LoaiDichVu,DonGia,DonVi,SoLuongBan,TrangThai,HinhAnh,MoTa
        stringstream ss(line);
        string maDV, tenDV, loaiStr, donGiaStr, donVi, soLuongBanStr, trangThaiStr, hinhAnh, moTa;

        getline(ss, maDV, ',');
        getline(ss, tenDV, ',');
        getline(ss, loaiStr, ',');
        getline(ss, donGiaStr, ',');
        getline(ss, donVi, ',');
        getline(ss, soLuongBanStr, ',');
        getline(ss, trangThaiStr, ',');
        getline(ss, hinhAnh, ',');
        getline(ss, moTa);

        // Convert loaiStr to LoaiDichVu
        LoaiDichVu loai = LoaiDichVu::KHAC;
        if (loaiStr == "DO_UONG") loai = LoaiDichVu::DO_UONG;
        else if (loaiStr == "THIET_BI") loai = LoaiDichVu::THIET_BI;
        else if (loaiStr == "BAO_HIEM") loai = LoaiDichVu::BAO_HIEM;

        // Convert donGia
        double donGia = stod(donGiaStr);

        // Create DichVu object
        DichVu* dv = new DichVu(maDV, tenDV, donGia, loai);
        dv->datDonVi(donVi);
        dv->datSoLuongBan(stoi(soLuongBanStr));
        dv->datConHang(trangThaiStr == "1");
        dv->datHinhAnh(hinhAnh);
        dv->datMoTa(moTa);

        if (themDichVu(dv))
        {
            count++;
        }
        else
        {
            delete dv;
        }
    }

    file.close();
    cout << "Tai thanh cong " << count << " dich vu tu CSV" << endl;
    return count > 0;
}

bool QuanLyDichVu::luuDuLieuRaCSV(const std::string &filePath) const
{
    ofstream file(filePath);
    if (!file.is_open())
    {
        cerr << "Khong the mo file de ghi: " << filePath << endl;
        return false;
    }

    // Write header
    file << "MaDichVu,TenDichVu,LoaiDichVu,DonGia,DonVi,SoLuongBan,TrangThai,HinhAnh,MoTa" << endl;

    // Write each service
    for (int i = 0; i < danhSachDichVu.size(); i++)
    {
        DichVu* dv = danhSachDichVu[i];
        
        // Convert LoaiDichVu to string
        string loaiStr;
        switch (dv->layLoaiDichVu())
        {
            case LoaiDichVu::DO_UONG: loaiStr = "DO_UONG"; break;
            case LoaiDichVu::THIET_BI: loaiStr = "THIET_BI"; break;
            case LoaiDichVu::BAO_HIEM: loaiStr = "BAO_HIEM"; break;
            default: loaiStr = "KHAC"; break;
        }

        // Write CSV line: MaDichVu,TenDichVu,LoaiDichVu,DonGia,DonVi,SoLuongBan,TrangThai,HinhAnh,MoTa
        file << dv->layMaDichVu() << ","
             << dv->layTenDichVu() << ","
             << loaiStr << ","
             << dv->layDonGia() << ","
             << dv->layDonVi() << ","
             << dv->laySoLuongBan() << ","
             << (dv->coConHang() ? "1" : "0") << ","
             << dv->layHinhAnh() << ","
             << dv->layMoTa() << endl;
    }

    file.close();
    cout << "Luu thanh cong " << danhSachDichVu.size() << " dich vu ra CSV: " << filePath << endl;
    return true;
}
