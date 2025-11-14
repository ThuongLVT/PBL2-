/**
 * @file QuanLySan.cpp
 * @brief Triển khai các phương thức của lớp QuanLySan
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#include "QuanLySan.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <codecvt>
#include <locale>

using namespace std;

// ========== HELPER METHODS ==========

LoaiSan QuanLySan::parseLoaiSan(const string &str) const
{
    if (str == "5" || str == "SAN_5")
        return LoaiSan::SAN_5;
    else if (str == "7" || str == "SAN_7")
        return LoaiSan::SAN_7;
    return LoaiSan::SAN_5; // Default
}

KhuVuc QuanLySan::parseKhuVuc(const string &str) const
{
    if (str == "A" || str == "a")
        return KhuVuc::A;
    else if (str == "B" || str == "b")
        return KhuVuc::B;
    else if (str == "C" || str == "c")
        return KhuVuc::C;
    else if (str == "D" || str == "d")
        return KhuVuc::D;
    return KhuVuc::A; // Default
}

TrangThaiSan QuanLySan::parseTrangThai(const string &str) const
{
    if (str == "HOAT_DONG" || str == "Hoat dong" || str == "Hoạt động")
        return TrangThaiSan::HOAT_DONG;
    else if (str == "NGUNG_HOAT_DONG" || str == "Ngung hoat dong" || str == "Ngừng hoạt động")
        return TrangThaiSan::NGUNG_HOAT_DONG;
    else if (str == "BAO_TRI" || str == "Bao tri" || str == "Bảo trì")
        return TrangThaiSan::BAO_TRI;
    return TrangThaiSan::HOAT_DONG; // Default
}

// ========== CONSTRUCTORS ==========

QuanLySan::QuanLySan() : csvFilePath("Data/san.csv")
{
}

QuanLySan::~QuanLySan()
{
    xoaTatCa();
}

// ========== CRUD OPERATIONS ==========

bool QuanLySan::themSan(San *san)
{
    if (san == nullptr)
    {
        return false;
    }

    // Kiểm tra trùng mã
    if (timSan(san->layMaSan()) != nullptr)
    {
        return false;
    }

    danhSachSan.push_back(san);
    return true;
}

bool QuanLySan::xoaSan(const string &maSan)
{
    for (int i = 0; i < danhSachSan.size(); i++)
    {
        if (danhSachSan[i]->layMaSan() == maSan)
        {
            delete danhSachSan[i];
            danhSachSan.erase(i);
            return true;
        }
    }
    return false;
}

bool QuanLySan::capNhatSan(const string &maSan, const San &sanMoi)
{
    San *san = timSan(maSan);
    if (san == nullptr)
    {
        return false;
    }

    *san = sanMoi;
    return true;
}

San *QuanLySan::timSan(const string &maSan)
{
    for (int i = 0; i < danhSachSan.size(); i++)
    {
        if (danhSachSan[i]->layMaSan() == maSan)
        {
            return danhSachSan[i];
        }
    }
    return nullptr;
}

const MangDong<San *> &QuanLySan::layDanhSachSan() const
{
    return danhSachSan;
}

// ========== SEARCH & FILTER ==========

MangDong<San *> QuanLySan::timSanTheoLoai(const string &loai)
{
    MangDong<San *> ketQua;

    // Convert string to LoaiSan enum for comparison
    LoaiSan loaiCanTim;
    if (loai == "SAN_5" || loai == "5")
        loaiCanTim = LoaiSan::SAN_5;
    else if (loai == "SAN_7" || loai == "7")
        loaiCanTim = LoaiSan::SAN_7;
    else
        return ketQua; // Empty result if invalid type

    for (int i = 0; i < danhSachSan.size(); i++)
    {
        if (danhSachSan[i]->layLoaiSan() == loaiCanTim)
        {
            ketQua.push_back(danhSachSan[i]);
        }
    }

    return ketQua;
}

MangDong<San *> QuanLySan::timSanTheoTen(const string &ten)
{
    MangDong<San *> ketQua;

    for (int i = 0; i < danhSachSan.size(); i++)
    {
        // Tìm gần đúng (contains)
        if (danhSachSan[i]->layTenSan().find(ten) != string::npos)
        {
            ketQua.push_back(danhSachSan[i]);
        }
    }

    return ketQua;
}

MangDong<San *> QuanLySan::laySanDangHoatDong()
{
    MangDong<San *> ketQua;

    for (int i = 0; i < danhSachSan.size(); i++)
    {
        if (danhSachSan[i]->dangHoatDong())
        {
            ketQua.push_back(danhSachSan[i]);
        }
    }

    return ketQua;
}

MangDong<San *> QuanLySan::laySanBaoTri()
{
    MangDong<San *> ketQua;

    for (int i = 0; i < danhSachSan.size(); i++)
    {
        if (danhSachSan[i]->dangBaoTri())
        {
            ketQua.push_back(danhSachSan[i]);
        }
    }

    return ketQua;
}

MangDong<San *> QuanLySan::laySanNgungHoatDong()
{
    MangDong<San *> ketQua;

    for (int i = 0; i < danhSachSan.size(); i++)
    {
        if (danhSachSan[i]->ngungHoatDong())
        {
            ketQua.push_back(danhSachSan[i]);
        }
    }

    return ketQua;
}

// ========== AVAILABILITY CHECK ==========

bool QuanLySan::kiemTraSanTrong(San *san, const NgayGio & /*thoiGian*/, const KhungGio & /*khungGio*/)
{
    if (san == nullptr)
    {
        return false;
    }

    // Kiểm tra trạng thái sân - sân phải đang hoạt động
    if (!san->dangHoatDong())
    {
        return false;
    }

    // TODO: Cần tích hợp với QuanLyDatSan để kiểm tra lịch đặt
    // Hiện tại chỉ kiểm tra trạng thái sân

    return true;
}

MangDong<San *> QuanLySan::timSanTrong(const NgayGio &thoiGian, const KhungGio &khungGio)
{
    MangDong<San *> ketQua;

    for (int i = 0; i < danhSachSan.size(); i++)
    {
        if (kiemTraSanTrong(danhSachSan[i], thoiGian, khungGio))
        {
            ketQua.push_back(danhSachSan[i]);
        }
    }

    return ketQua;
}

// ========== STATUS MANAGEMENT ==========

bool QuanLySan::datTrangThaiSan(const string &maSan, TrangThaiSan trangThai)
{
    San *san = timSan(maSan);
    if (san == nullptr)
    {
        return false;
    }

    san->datTrangThai(trangThai);
    return true;
}

// ========== DISPLAY ==========

void QuanLySan::hienThiDanhSachSan() const
{
    cout << "\n========== DANH SACH SAN ==========" << endl;
    cout << "Tong so san: " << danhSachSan.size() << endl;
    cout << "====================================" << endl;

    for (int i = 0; i < danhSachSan.size(); i++)
    {
        cout << "\n[" << (i + 1) << "] ";
        danhSachSan[i]->hienThiThongTin();
    }

    cout << "====================================" << endl;
}

void QuanLySan::hienThiThongTinSan(const string &maSan) const
{
    for (int i = 0; i < danhSachSan.size(); i++)
    {
        if (danhSachSan[i]->layMaSan() == maSan)
        {
            danhSachSan[i]->hienThiThongTin();
            return;
        }
    }

    cout << "Khong tim thay san voi ma: " << maSan << endl;
}

// ========== STATISTICS ==========

int QuanLySan::tongSoSan() const
{
    return danhSachSan.size();
}

int QuanLySan::demSanHoatDong() const
{
    int dem = 0;
    for (int i = 0; i < danhSachSan.size(); i++)
    {
        if (danhSachSan[i]->dangHoatDong())
        {
            dem++;
        }
    }
    return dem;
}

int QuanLySan::demSanNgungHoatDong() const
{
    int dem = 0;
    for (int i = 0; i < danhSachSan.size(); i++)
    {
        if (danhSachSan[i]->ngungHoatDong())
        {
            dem++;
        }
    }
    return dem;
}

int QuanLySan::demSanBaoTri() const
{
    int dem = 0;
    for (int i = 0; i < danhSachSan.size(); i++)
    {
        if (danhSachSan[i]->dangBaoTri())
        {
            dem++;
        }
    }
    return dem;
}

// ========== FILE I/O ==========

bool QuanLySan::ghiFile(ofstream &file) const
{
    if (!file.is_open())
    {
        return false;
    }

    // Ghi số lượng sân
    int soLuong = danhSachSan.size();
    file.write(reinterpret_cast<const char *>(&soLuong), sizeof(soLuong));

    // Ghi từng sân
    for (int i = 0; i < soLuong; i++)
    {
        if (!danhSachSan[i]->ghiFile(file))
        {
            return false;
        }
    }

    return file.good();
}

bool QuanLySan::docFile(ifstream &file)
{
    if (!file.is_open())
    {
        return false;
    }

    // Xóa dữ liệu cũ
    xoaTatCa();

    // Đọc số lượng sân
    int soLuong;
    file.read(reinterpret_cast<char *>(&soLuong), sizeof(soLuong));

    // Đọc từng sân
    for (int i = 0; i < soLuong; i++)
    {
        San *san = new San();
        if (!san->docFile(file))
        {
            delete san;
            return false;
        }
        danhSachSan.push_back(san);
    }

    return file.good();
}

void QuanLySan::xoaTatCa()
{
    for (int i = 0; i < danhSachSan.size(); i++)
    {
        delete danhSachSan[i];
    }
    danhSachSan = MangDong<San *>();
}

// ========== CSV I/O ==========

bool QuanLySan::loadFromCSV(const string &filePath)
{
    if (!filePath.empty())
    {
        csvFilePath = filePath;
    }

    // Open file with UTF-8 encoding support
    ifstream file(csvFilePath, ios::binary);
    if (!file.is_open())
    {
        cerr << "Khong the mo file: " << csvFilePath << endl;
        return false;
    }

    // Xóa dữ liệu cũ
    xoaTatCa();

    string line;
    // Skip BOM if present (UTF-8 BOM: EF BB BF)
    char bom[3] = {0};
    file.read(bom, 3);
    if (!(bom[0] == (char)0xEF && bom[1] == (char)0xBB && bom[2] == (char)0xBF))
    {
        // No BOM, rewind to start
        file.seekg(0);
    }

    // Bỏ qua header
    getline(file, line);

    int lineNumber = 1;
    while (getline(file, line))
    {
        lineNumber++;
        if (line.empty())
            continue;

        stringstream ss(line);
        string maSan, tenSan, loaiSanStr, khuVucStr, giaThueStr, trangThaiStr, ghiChu;

        // Parse CSV: MaSan,TenSan,LoaiSan,KhuVuc,GiaThue,TrangThai,GhiChu
        getline(ss, maSan, ',');
        getline(ss, tenSan, ',');
        getline(ss, loaiSanStr, ',');
        getline(ss, khuVucStr, ',');
        getline(ss, giaThueStr, ',');
        getline(ss, trangThaiStr, ',');
        getline(ss, ghiChu, ',');

        // Trim whitespace
        maSan.erase(0, maSan.find_first_not_of(" \t\r\n"));
        maSan.erase(maSan.find_last_not_of(" \t\r\n") + 1);

        try
        {
            LoaiSan loaiSan = parseLoaiSan(loaiSanStr);
            KhuVuc khuVuc = parseKhuVuc(khuVucStr);
            double giaThue = stod(giaThueStr);
            TrangThaiSan trangThai = parseTrangThai(trangThaiStr);

            San *san = new San(maSan, tenSan, loaiSan, khuVuc, giaThue);
            san->datTrangThai(trangThai);
            if (!ghiChu.empty())
            {
                san->datGhiChu(ghiChu);
            }

            danhSachSan.push_back(san);
        }
        catch (const exception &e)
        {
            cerr << "Loi doc dong " << lineNumber << ": " << e.what() << endl;
        }
    }

    file.close();
    cout << "Load thanh cong " << danhSachSan.size() << " san tu " << csvFilePath << endl;
    return true;
}

bool QuanLySan::saveToCSV()
{
    ofstream file(csvFilePath);
    if (!file.is_open())
    {
        cerr << "Khong the mo file de ghi: " << csvFilePath << endl;
        return false;
    }

    // Write header
    file << "MaSan,TenSan,LoaiSan,KhuVuc,GiaThue,TrangThai,GhiChu\n";

    // Write data
    for (int i = 0; i < danhSachSan.size(); i++)
    {
        San *san = danhSachSan[i];
        file << san->layMaSan() << ","
             << san->layTenSan() << ","
             << san->layTenLoaiSan() << ","
             << san->layTenKhuVuc() << ","
             << san->layGiaThueGio() << ","
             << san->layTenTrangThai() << ","
             << san->layGhiChu() << "\n";
    }

    file.close();
    cout << "Luu thanh cong " << danhSachSan.size() << " san vao " << csvFilePath << endl;
    return true;
}

// ========== AUTO GENERATION ==========

string QuanLySan::taoMaSanTuDong(KhuVuc khuVuc, LoaiSan loaiSan)
{
    // Format: <Khu><Loai><STT> (VD: A501, B702, C1103)
    string khuStr;
    switch (khuVuc)
    {
    case KhuVuc::A:
        khuStr = "A";
        break;
    case KhuVuc::B:
        khuStr = "B";
        break;
    case KhuVuc::C:
        khuStr = "C";
        break;
    case KhuVuc::D:
        khuStr = "D";
        break;
    }

    string loaiStr;
    switch (loaiSan)
    {
    case LoaiSan::SAN_5:
        loaiStr = "5";
        break;
    case LoaiSan::SAN_7:
        loaiStr = "7";
        break;
    }

    // Find max STT for this khu + loai combination
    int maxSTT = 0;
    string prefix = khuStr + loaiStr;

    for (int i = 0; i < danhSachSan.size(); i++)
    {
        string maSan = danhSachSan[i]->layMaSan();
        if (maSan.substr(0, prefix.length()) == prefix)
        {
            // Extract STT
            string sttStr = maSan.substr(prefix.length());
            try
            {
                int stt = stoi(sttStr);
                if (stt > maxSTT)
                {
                    maxSTT = stt;
                }
            }
            catch (...)
            {
                // Ignore invalid format
            }
        }
    }

    // Generate new code
    int newSTT = maxSTT + 1;
    stringstream ss;
    ss << khuStr << loaiStr;
    if (newSTT < 10)
        ss << "0";
    ss << newSTT;

    return ss.str();
}

// ========== STATUS CHANGE ==========

bool QuanLySan::chuyenSangBaoTri(const string &maSan)
{
    San *san = timSan(maSan);
    if (san == nullptr)
    {
        return false;
    }

    // Check if field has future bookings
    if (kiemTraSanCoLichDat(maSan))
    {
        return false; // Cannot maintain if has bookings
    }

    san->datTrangThai(TrangThaiSan::BAO_TRI);
    saveToCSV(); // Persist change
    return true;
}

bool QuanLySan::chuyenVeHoatDong(const string &maSan)
{
    San *san = timSan(maSan);
    if (san == nullptr)
    {
        return false;
    }

    san->datTrangThai(TrangThaiSan::HOAT_DONG);
    saveToCSV(); // Persist change
    return true;
}

// ========== VALIDATION ==========

bool QuanLySan::kiemTraSanCoLichDat(const string & /*maSan*/) const
{
    // TODO: Integrate with QuanLyDatSan to check future bookings
    // For now, return false (no bookings)
    return false;
}
