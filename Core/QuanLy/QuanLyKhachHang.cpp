/**
 * @file QuanLyKhachHang.cpp
 * @brief Triển khai các phương thức của lớp QuanLyKhachHang
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#include "QuanLyKhachHang.h"
#include "../Utils/CSVHelper.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

QuanLyKhachHang::QuanLyKhachHang() : maxCustomerId(0), isLoadingFromCSV(false)
{
    // Khởi tạo Hash Table với kích thước 1009 (số nguyên tố)
    hashTableKhachHang = new HashTable<std::string, KhachHang *>(1009);
}

QuanLyKhachHang::~QuanLyKhachHang()
{
    xoaTatCa();
    delete hashTableKhachHang;
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
    
    // Thêm vào Hash Table để tìm kiếm nhanh
    hashTableKhachHang->insert(kh->getMaNguoiDung(), kh);

    // Auto-save to CSV after add (skip if loading)
    if (!isLoadingFromCSV)
    {
        luuCSV("khachhang.csv");
    }

    return true;
}

bool QuanLyKhachHang::xoaKhachHang(const string &maKH)
{
    // Xóa khỏi Hash Table
    hashTableKhachHang->remove(maKH);

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
    // Sử dụng Hash Table để tìm kiếm O(1)
    KhachHang *kh = nullptr;
    if (hashTableKhachHang->search(maKH, kh))
    {
        return kh;
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
    // Auto-save is handled by the caller (usually UI or System) calling luuHeThong()
    // But to be safe, we could save just the ID config, but we are moving to single file storage.
    
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
        hashTableKhachHang->insert(kh->getMaNguoiDung(), kh);
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

    // Reset Hash Table
    if (hashTableKhachHang != nullptr)
    {
        delete hashTableKhachHang;
        hashTableKhachHang = new HashTable<std::string, KhachHang *>(1009);
    }
}

// ========== CSV I/O ==========
bool QuanLyKhachHang::luuCSV(const string &filename) const
{
    vector<string> headers = {
        "MaKH", "HoTen", "SoDienThoai", "BacHoiVien",
        "TongChiTieu", "NgayDangKy"};

    // We will write manually to include the MAX_ID metadata
    string fullPath = CSVHelper::getDataPath() + filename;
    ofstream file(fullPath);
    
    if (!file.is_open()) {
        cerr << "Cannot create CSV file: " << fullPath << endl;
        return false;
    }

    // Write Metadata
    file << "#MAX_ID:" << maxCustomerId << "\n";

    // Write Headers
    for (size_t i = 0; i < headers.size(); i++)
    {
        file << CSVHelper::escapeField(headers[i]);
        if (i < headers.size() - 1)
            file << ",";
    }
    file << "\n";

    // Write Rows
    for (int i = 0; i < danhSachKhachHang.size(); i++)
    {
        KhachHang *kh = danhSachKhachHang[i];
        
        file << CSVHelper::escapeField(kh->layMaKhachHang()) << ",";
        file << CSVHelper::escapeField(kh->getHoTen()) << ",";
        file << CSVHelper::escapeField(kh->getSoDienThoai()) << ",";

        // BacHoiVien
        HangKhachHang hang = kh->layHang();
        string bacHoiVien = "THUONG";
        switch (hang)
        {
        case HangKhachHang::THUONG: bacHoiVien = "THUONG"; break;
        case HangKhachHang::DONG: bacHoiVien = "DONG"; break;
        case HangKhachHang::BAC: bacHoiVien = "BAC"; break;
        case HangKhachHang::VANG: bacHoiVien = "VANG"; break;
        case HangKhachHang::KIM_CUONG: bacHoiVien = "KIM_CUONG"; break;
        }
        file << CSVHelper::escapeField(bacHoiVien) << ",";

        // TongChiTieu - pure number, no VNĐ suffix
        long long chiTieu = static_cast<long long>(kh->layTongChiTieu());
        file << CSVHelper::escapeField(to_string(chiTieu)) << ",";

        // NgayDangKy: DD/MM/YYYY
        NgayThang ngayDK = kh->layNgayDangKy();
        stringstream ss;
        ss << setfill('0') << setw(2) << ngayDK.getNgay() << "/"
           << setfill('0') << setw(2) << ngayDK.getThang() << "/"
           << ngayDK.getNam();
        file << CSVHelper::escapeField(ss.str());
        
        file << "\n";
    }

    file.close();
    cout << "Saved " << danhSachKhachHang.size() << " customers to CSV: " << filename << " (MaxID: " << maxCustomerId << ")" << endl;
    return true;
}

bool QuanLyKhachHang::docCSV(const string &filename)
{
    vector<vector<string>> rows = CSVHelper::readCSV(filename, false);

    if (rows.empty())
    {
        cout << "No customer data found in CSV: " << filename << endl;
        maxCustomerId = 0;
        return true; // Not an error, just empty file
    }

    xoaTatCa();
    isLoadingFromCSV = true; // Prevent auto-save during load

    maxCustomerId = 0;
    size_t startRow = 1; // Default: skip header at row 0

    // Check for Metadata at row 0
    if (!rows.empty() && !rows[0].empty() && rows[0][0].find("#MAX_ID:") == 0) {
        try {
            string val = rows[0][0].substr(8); // Length of "#MAX_ID:"
            maxCustomerId = stoi(val);
            cout << "Loaded persistent MaxID: " << maxCustomerId << endl;
        } catch (...) {
            cout << "Failed to parse MaxID from header" << endl;
        }
        startRow = 2; // Skip metadata (0) and header (1)
    }

    int calculatedMax = 0;

    for (size_t i = startRow; i < rows.size(); i++)
    {
        const auto &row = rows[i];

        // Format: MaKH, HoTen, SoDienThoai, BacHoiVien, TongChiTieu, NgayDangKy
        if (row.size() < 6)
        {
            // If it's the header row (in case metadata was missing but we started at 1), skip it
            if (i == 0 && row.size() > 0 && row[0] == "MaKH") continue;
            
            cerr << "Invalid CSV row at line " << (i + 1) << ": insufficient columns" << endl;
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

            hashTableKhachHang->insert(kh->getMaNguoiDung(), kh);
            danhSachKhachHang.push_back(kh);

            // Update maxCustomerId from KH### format
            if (maKH.length() > 2 && maKH.substr(0, 2) == "KH")
            {
                try
                {
                    int id = stoi(maKH.substr(2));
                    if (id > calculatedMax)
                        calculatedMax = id;
                }
                catch (...)
                {
                    // Ignore invalid ID format
                }
            }
        }
        catch (const exception &e)
        {
            cerr << "Error parsing CSV row " << (i + 1) << ": " << e.what() << endl;
            continue;
        }
    }

    isLoadingFromCSV = false; // Re-enable auto-save

    // Fallback: if calculated max is greater than stored max (e.g. manual edit), update it
    if (calculatedMax > maxCustomerId) {
        maxCustomerId = calculatedMax;
    }

    cout << "Loaded " << danhSachKhachHang.size() << " customers from CSV" << endl;
    return true;
}



