/**
 * @file QuanLyDichVu.cpp
 * @brief Triển khai các phương thức của lớp QuanLyDichVu
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#include "QuanLyDichVu.h"
#include "../Utils/CSVManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

QuanLyDichVu::QuanLyDichVu() : maxServiceId(0) {}

QuanLyDichVu::~QuanLyDichVu()
{
    xoaTatCa();
}

string QuanLyDichVu::taoMaDichVuMoi()
{
    maxServiceId++;
    ostringstream oss;
    oss << "DV" << setfill('0') << setw(3) << maxServiceId;
    return oss.str();
}

bool QuanLyDichVu::themDichVu(DichVu *dv)
{
    if (dv == nullptr || timDichVu(dv->layMaDichVu()) != nullptr)
    {
        return false;
    }
    danhSachDichVu.push_back(dv);
    
    // Update maxServiceId if needed
    string maDV = dv->layMaDichVu();
    if (maDV.length() >= 3 && maDV.substr(0, 2) == "DV") {
        try {
            int id = stoi(maDV.substr(2));
            if (id > maxServiceId) {
                maxServiceId = id;
            }
        } catch (...) {
            // Ignore invalid ID format
        }
    }

    // Auto-save to CSV
    luuDuLieuRaCSV("dichvu.csv");

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

            // Auto-save to CSV
            luuDuLieuRaCSV("dichvu.csv");

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

    // Auto-save to CSV
    luuDuLieuRaCSV("dichvu.csv");

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

    // Save maxServiceId first
    file.write(reinterpret_cast<const char *>(&maxServiceId), sizeof(maxServiceId));
    
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

    // Load maxServiceId first
    file.read(reinterpret_cast<char *>(&maxServiceId), sizeof(maxServiceId));
    
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
    maxServiceId = 0;
}

bool QuanLyDichVu::taiDuLieuTuCSV(const std::string &filePath)
{
    vector<vector<string>> rows = CSVManager::readCSV(filePath, true);

    if (rows.empty())
    {
        cout << "No service data found in CSV: " << filePath << endl;
        return true; // Not an error, just empty file
    }

    int count = 0;
    for (size_t i = 0; i < rows.size(); i++)
    {
        const auto &row = rows[i];

        // Parse CSV line: MaDichVu,TenDichVu,LoaiDichVu,DonGia,DonVi,SoLuongBan,TrangThai,HinhAnh,MoTa
        if (row.size() < 9)
        {
            cerr << "Invalid CSV row at line " << (i + 2) << ": insufficient columns" << endl;
            continue;
        }

        try
        {
            string maDV = row[0];
            string tenDV = row[1];
            string loaiStr = row[2];
            double donGia = stod(row[3]);
            string donVi = row[4];
            int soLuongBan = stoi(row[5]);
            bool trangThai = (row[6] == "1");
            string hinhAnh = row[7];
            string moTa = row[8];

            // Convert loaiStr to LoaiDichVu
            LoaiDichVu loai = LoaiDichVu::KHAC;
            if (loaiStr == "DO_UONG")
                loai = LoaiDichVu::DO_UONG;
            else if (loaiStr == "THIET_BI")
                loai = LoaiDichVu::THIET_BI;
            else if (loaiStr == "BAO_HIEM")
                loai = LoaiDichVu::BAO_HIEM;

            // Create DichVu object
            DichVu *dv = new DichVu(maDV, tenDV, donGia, loai);
            dv->datDonVi(donVi);
            dv->datSoLuongBan(soLuongBan);
            dv->datConHang(trangThai);
            dv->datHinhAnh(hinhAnh);
            dv->datMoTa(moTa);

            // Don't auto-save when loading from CSV (avoid infinite loop)
            danhSachDichVu.push_back(dv);
            count++;
        }
        catch (const exception &e)
        {
            cerr << "Error parsing CSV row " << (i + 2) << ": " << e.what() << endl;
            continue;
        }
    }

    cout << "Loaded " << count << " services from CSV: " << filePath << endl;
    return count > 0;
}

bool QuanLyDichVu::luuDuLieuRaCSV(const std::string &filePath) const
{
    vector<string> headers = {"MaDichVu", "TenDichVu", "LoaiDichVu", "DonGia", "DonVi", "SoLuongBan", "TrangThai", "HinhAnh", "MoTa"};
    vector<vector<string>> rows;

    for (int i = 0; i < danhSachDichVu.size(); i++)
    {
        DichVu *dv = danhSachDichVu[i];
        vector<string> row;

        // Convert LoaiDichVu to string
        string loaiStr;
        switch (dv->layLoaiDichVu())
        {
        case LoaiDichVu::DO_UONG:
            loaiStr = "DO_UONG";
            break;
        case LoaiDichVu::THIET_BI:
            loaiStr = "THIET_BI";
            break;
        case LoaiDichVu::BAO_HIEM:
            loaiStr = "BAO_HIEM";
            break;
        default:
            loaiStr = "KHAC";
            break;
        }

        row.push_back(dv->layMaDichVu());
        row.push_back(dv->layTenDichVu());
        row.push_back(loaiStr);
        row.push_back(to_string(static_cast<long long>(dv->layDonGia())));
        row.push_back(dv->layDonVi());
        row.push_back(to_string(dv->laySoLuongBan()));
        row.push_back(dv->coConHang() ? "1" : "0");
        row.push_back(dv->layHinhAnh());
        row.push_back(dv->layMoTa());

        rows.push_back(row);
    }

    bool success = CSVManager::writeCSV(filePath, headers, rows);
    if (success)
    {
        cout << "Saved " << danhSachDichVu.size() << " services to CSV: " << filePath << endl;
    }
    return success;
}
