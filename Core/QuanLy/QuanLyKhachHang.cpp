/**
 * @file QuanLyKhachHang.cpp
 * @brief Triển khai các phương thức của lớp QuanLyKhachHang
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#include "QuanLyKhachHang.h"
#include "../Utils/CSVManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

QuanLyKhachHang::QuanLyKhachHang() : maxCustomerId(0), isLoadingFromCSV(false) {}

QuanLyKhachHang::~QuanLyKhachHang()
{
    xoaTatCa();
}

bool QuanLyKhachHang::themKhachHang(KhachHang *kh)
{
    cout << "\n=== themKhachHang DEBUG ===" << endl;

    if (kh == nullptr)
    {
        return false;
    }

    // Check for duplicate ID
    KhachHang *existing = timKhachHang(kh->getMaNguoiDung());
    if (existing != nullptr)
    {
        return false;
    }

    // Check for duplicate phone
    KhachHang *existingPhone = timKhachHangTheoSDT(kh->getSoDienThoai());
    if (existingPhone != nullptr)
    {
        return false;
    }

    danhSachKhachHang.push_back(kh);

    // Auto-save to CSV after add (skip if loading)
    if (!isLoadingFromCSV)
    {
        luuCSV("khachhang.csv");
    }

    return true;
}

bool QuanLyKhachHang::xoaKhachHang(const string &maKH)
{
    for (int i = 0; i < danhSachKhachHang.size(); i++)
    {
        if (danhSachKhachHang[i]->getMaNguoiDung() == maKH)
        {
            delete danhSachKhachHang[i];
            danhSachKhachHang.erase(i);

            // Auto-save to CSV after delete
            luuCSV("khachhang.csv");

            return true;
        }
    }
    return false;
}

bool QuanLyKhachHang::capNhatKhachHang(const string &maKH, const KhachHang &khMoi)
{
    KhachHang *kh = timKhachHang(maKH);
    if (kh == nullptr)
        return false;
    *kh = khMoi;

    // Auto-save to CSV after update
    luuCSV("khachhang.csv");

    return true;
}

KhachHang *QuanLyKhachHang::timKhachHang(const string &maKH)
{
    for (int i = 0; i < danhSachKhachHang.size(); i++)
    {
        if (danhSachKhachHang[i]->getMaNguoiDung() == maKH)
        {
            return danhSachKhachHang[i];
        }
    }
    return nullptr;
}

const MangDong<KhachHang *> &QuanLyKhachHang::layDanhSachKhachHang() const
{
    return danhSachKhachHang;
}

KhachHang *QuanLyKhachHang::timKhachHangTheoSDT(const string &sdt)
{
    for (int i = 0; i < danhSachKhachHang.size(); i++)
    {
        if (danhSachKhachHang[i]->getSoDienThoai() == sdt)
        {
            return danhSachKhachHang[i];
        }
    }
    return nullptr;
}

MangDong<KhachHang *> QuanLyKhachHang::timKhachHangTheoTen(const string &ten)
{
    MangDong<KhachHang *> ketQua;
    for (int i = 0; i < danhSachKhachHang.size(); i++)
    {
        if (danhSachKhachHang[i]->getHoTen().find(ten) != string::npos)
        {
            ketQua.push_back(danhSachKhachHang[i]);
        }
    }
    return ketQua;
}

void QuanLyKhachHang::hienThiDanhSachKhachHang() const
{
    cout << "\n===== DANH SACH KHACH HANG =====" << endl;
    cout << "Tong so: " << danhSachKhachHang.size() << endl;
    for (int i = 0; i < danhSachKhachHang.size(); i++)
    {
        cout << "\n[" << (i + 1) << "] ";
        danhSachKhachHang[i]->hienThiThongTin();
    }
}

void QuanLyKhachHang::hienThiThongTinKhachHang(const string &maKH) const
{
    for (int i = 0; i < danhSachKhachHang.size(); i++)
    {
        if (danhSachKhachHang[i]->getMaNguoiDung() == maKH)
        {
            danhSachKhachHang[i]->hienThiThongTin();
            return;
        }
    }
    cout << "Khong tim thay khach hang: " << maKH << endl;
}

int QuanLyKhachHang::tongSoKhachHang() const
{
    return danhSachKhachHang.size();
}

// ========== ID GENERATION ==========
string QuanLyKhachHang::taoMaKhachHangMoi()
{
    maxCustomerId++;
    stringstream ss;
    ss << "KH" << setfill('0') << setw(2) << maxCustomerId;
    return ss.str();
}

bool QuanLyKhachHang::kiemTraSDTTonTai(const string &sdt, const string &excludeMaKH) const
{
    for (int i = 0; i < danhSachKhachHang.size(); i++)
    {
        if (danhSachKhachHang[i]->getSoDienThoai() == sdt &&
            danhSachKhachHang[i]->layMaKhachHang() != excludeMaKH)
        {
            return true;
        }
    }
    return false;
}

// ========== FILE I/O ==========
bool QuanLyKhachHang::ghiFile(ofstream &file) const
{
    if (!file.is_open())
        return false;

    // Ghi maxCustomerId
    file.write(reinterpret_cast<const char *>(&maxCustomerId), sizeof(maxCustomerId));

    int soLuong = danhSachKhachHang.size();
    file.write(reinterpret_cast<const char *>(&soLuong), sizeof(soLuong));

    for (int i = 0; i < soLuong; i++)
    {
        if (!danhSachKhachHang[i]->ghiFile(file))
            return false;
    }
    return file.good();
}

bool QuanLyKhachHang::docFile(ifstream &file)
{
    if (!file.is_open())
        return false;

    xoaTatCa();

    // Đọc maxCustomerId
    file.read(reinterpret_cast<char *>(&maxCustomerId), sizeof(maxCustomerId));
    cout << "Loaded maxCustomerId: " << maxCustomerId << endl;

    int soLuong;
    file.read(reinterpret_cast<char *>(&soLuong), sizeof(soLuong));
    cout << "Loading " << soLuong << " customers..." << endl;

    for (int i = 0; i < soLuong; i++)
    {
        KhachHang *kh = new KhachHang();
        if (!kh->docFile(file))
        {
            delete kh;
            return false;
        }
        danhSachKhachHang.push_back(kh);
    }
    return file.good();
}

void QuanLyKhachHang::xoaTatCa()
{
    for (int i = 0; i < danhSachKhachHang.size(); i++)
    {
        delete danhSachKhachHang[i];
    }
    danhSachKhachHang = MangDong<KhachHang *>();
}

// ========== CSV I/O ==========
bool QuanLyKhachHang::luuCSV(const string &filename) const
{
    vector<string> headers = {
        "MaKH", "HoTen", "SoDienThoai", "BacHoiVien",
        "TongChiTieu", "NgayDangKy"};

    vector<vector<string>> rows;

    // Customer data rows
    for (int i = 0; i < danhSachKhachHang.size(); i++)
    {
        KhachHang *kh = danhSachKhachHang[i];
        vector<string> row;

        row.push_back(kh->layMaKhachHang());
        row.push_back(kh->getHoTen());
        row.push_back(kh->getSoDienThoai());

        // BacHoiVien
        HangKhachHang hang = kh->layHang();
        string bacHoiVien = "THUONG";
        switch (hang)
        {
        case HangKhachHang::THUONG:
            bacHoiVien = "THUONG";
            break;
        case HangKhachHang::DONG:
            bacHoiVien = "DONG";
            break;
        case HangKhachHang::BAC:
            bacHoiVien = "BAC";
            break;
        case HangKhachHang::VANG:
            bacHoiVien = "VANG";
            break;
        case HangKhachHang::KIM_CUONG:
            bacHoiVien = "KIM_CUONG";
            break;
        }
        row.push_back(bacHoiVien);

        // TongChiTieu - pure number, no VNĐ suffix
        long long chiTieu = static_cast<long long>(kh->layTongChiTieu());
        row.push_back(to_string(chiTieu));

        // NgayDangKy: DD/MM/YYYY
        NgayThang ngayDK = kh->layNgayDangKy();
        stringstream ss;
        ss << setfill('0') << setw(2) << ngayDK.getNgay() << "/"
           << setfill('0') << setw(2) << ngayDK.getThang() << "/"
           << ngayDK.getNam();
        row.push_back(ss.str());

        rows.push_back(row);
    }

    bool success = CSVManager::writeCSV(filename, headers, rows);
    if (success)
    {
        cout << "Saved " << danhSachKhachHang.size() << " customers to CSV: " << filename << endl;
    }
    return success;
}

bool QuanLyKhachHang::docCSV(const string &filename)
{
    vector<vector<string>> rows = CSVManager::readCSV(filename, false);

    if (rows.empty())
    {
        cout << "No customer data found in CSV: " << filename << endl;
        maxCustomerId = 0;
        return true; // Not an error, just empty file
    }

    xoaTatCa();
    isLoadingFromCSV = true; // Prevent auto-save during load

    // Skip header row, read customer data starting from row 1
    maxCustomerId = 0;

    for (size_t i = 1; i < rows.size(); i++)
    {
        const auto &row = rows[i];

        // Format: MaKH, HoTen, SoDienThoai, BacHoiVien, TongChiTieu, NgayDangKy
        if (row.size() < 6)
        {
            cerr << "Invalid CSV row at line " << (i + 2) << ": insufficient columns" << endl;
            continue;
        }

        try
        {
            string maKH = row[0];
            string hoTen = row[1];
            string sdt = row[2];
            // row[3] is BacHoiVien - ignore, we calculate from TongChiTieu
            long long tongChiTieu = stoll(row[4]);
            string ngayStr = row[5];

            // Parse date DD/MM/YYYY
            int day, month, year;
            char sep;
            stringstream ss(ngayStr);
            ss >> day >> sep >> month >> sep >> year;

            KhachHang *kh = new KhachHang(hoTen, sdt, "N/A", maKH);
            kh->datNgayDangKy(NgayThang(day, month, year));
            kh->datTongChiTieu(tongChiTieu);

            danhSachKhachHang.push_back(kh);

            // Update maxCustomerId from KH### format
            if (maKH.length() > 2 && maKH.substr(0, 2) == "KH")
            {
                try
                {
                    int id = stoi(maKH.substr(2));
                    if (id > maxCustomerId)
                        maxCustomerId = id;
                }
                catch (...)
                {
                    // Ignore invalid ID format
                }
            }
        }
        catch (const exception &e)
        {
            cerr << "Error parsing CSV row " << (i + 2) << ": " << e.what() << endl;
            continue;
        }
    }

    isLoadingFromCSV = false; // Re-enable auto-save
    cout << "Loaded " << danhSachKhachHang.size() << " customers from CSV" << endl;
    return true;
}
