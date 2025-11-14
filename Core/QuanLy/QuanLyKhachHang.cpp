/**
 * @file QuanLyKhachHang.cpp
 * @brief Triển khai các phương thức của lớp QuanLyKhachHang
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#include "QuanLyKhachHang.h"
#include "../Utils/CSVHandler.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

QuanLyKhachHang::QuanLyKhachHang() : maxCustomerId(0) {}

QuanLyKhachHang::~QuanLyKhachHang()
{
    xoaTatCa();
}

bool QuanLyKhachHang::themKhachHang(KhachHang *kh)
{
    cout << "\n=== themKhachHang DEBUG ===" << endl;

    if (kh == nullptr)
    {
        cout << "ERROR: Null customer pointer" << endl;
        return false;
    }

    cout << "Attempting to add customer:" << endl;
    cout << "  ID: " << kh->getMaNguoiDung() << endl;
    cout << "  Name: " << kh->getHoTen() << endl;
    cout << "  Phone: " << kh->getSoDienThoai() << endl;
    cout << "  Current customer count: " << danhSachKhachHang.size() << endl;
    cout << "  Current maxCustomerId: " << maxCustomerId << endl;

    // Check for duplicate ID
    KhachHang *existing = timKhachHang(kh->getMaNguoiDung());
    if (existing != nullptr)
    {
        cout << "ERROR: Customer ID already exists: " << kh->getMaNguoiDung() << endl;
        cout << "  Existing customer: " << existing->getHoTen() << endl;
        return false;
    }

    // Check for duplicate phone
    KhachHang *existingPhone = timKhachHangTheoSDT(kh->getSoDienThoai());
    if (existingPhone != nullptr)
    {
        cout << "ERROR: Phone number already exists: " << kh->getSoDienThoai() << endl;
        cout << "  Owner: " << existingPhone->getHoTen() << " (" << existingPhone->getMaNguoiDung() << ")" << endl;
        return false;
    }

    danhSachKhachHang.push_back(kh);
    cout << "SUCCESS: Added customer " << kh->getMaNguoiDung() << " - " << kh->getHoTen() << endl;
    cout << "  New customer count: " << danhSachKhachHang.size() << endl;
    cout << "========================\n"
         << endl;
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
    ss << "KH" << setfill('0') << setw(3) << maxCustomerId;
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

    // First row: maxCustomerId metadata
    rows.push_back({to_string(maxCustomerId), "", "", "", "", ""});

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

    bool success = CSVHandler::writeCSV(filename, headers, rows);
    if (success)
    {
        cout << "Saved " << danhSachKhachHang.size() << " customers to CSV: " << filename << endl;
    }
    return success;
}

bool QuanLyKhachHang::docCSV(const string &filename)
{
    vector<vector<string>> rows = CSVHandler::readCSV(filename);

    if (rows.empty())
    {
        cout << "No customer data found in CSV: " << filename << endl;
        maxCustomerId = 0;
        return true; // Not an error, just empty file
    }

    xoaTatCa();

    // First row contains maxCustomerId
    if (!rows[0].empty() && !rows[0][0].empty())
    {
        maxCustomerId = stoi(rows[0][0]);
        cout << "Loaded maxCustomerId: " << maxCustomerId << endl;
    }

    // Read customer data (skip first metadata row)
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
        }
        catch (const exception &e)
        {
            cerr << "Error parsing CSV row " << (i + 2) << ": " << e.what() << endl;
            continue;
        }
    }

    cout << "Loaded " << danhSachKhachHang.size() << " customers from CSV" << endl;
    return true;
}
