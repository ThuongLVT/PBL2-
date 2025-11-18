/**
 * @file QuanLyNhanVien.cpp
 * @brief Triển khai các phương thức của lớp QuanLyNhanVien
 *
 * @author Football Field Management System
 * @date 2025-11-18
 */

#include "QuanLyNhanVien.h"
#include "../Utils/CSVManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

QuanLyNhanVien::QuanLyNhanVien() : maxEmployeeId(0), isLoadingFromCSV(false), currentUser(nullptr) {}

QuanLyNhanVien::~QuanLyNhanVien()
{
    xoaTatCa();
}

// ========== ACCESS CONTROL ==========
void QuanLyNhanVien::setCurrentUser(NguoiDung *user)
{
    currentUser = user;
}

bool QuanLyNhanVien::isAdmin() const
{
    if (currentUser == nullptr)
        return false;
    return currentUser->layVaiTro() == VaiTro::QUAN_TRI_VIEN;
}

// ========== CRUD OPERATIONS ==========
bool QuanLyNhanVien::themNhanVien(NguoiDung *user)
{
    if (user == nullptr)
    {
        cerr << "Error: Cannot add null employee" << endl;
        return false;
    }

    // Check for duplicate username
    if (kiemTraUsernameTonTai(user->layTenDangNhap()))
    {
        cerr << "Error: Username already exists: " << user->layTenDangNhap() << endl;
        return false;
    }

    // Check for duplicate phone
    if (kiemTraSDTTonTai(user->laySoDienThoai()))
    {
        cerr << "Error: Phone number already exists: " << user->laySoDienThoai() << endl;
        return false;
    }

    danhSachNhanVien.push_back(user);

    // Auto-save to CSV after add (skip if loading)
    if (!isLoadingFromCSV)
    {
        luuCSV();
    }

    NhanVien *nv = dynamic_cast<NhanVien*>(user);
    if (nv) {
        cout << "Added employee: " << user->layHoTen() << " (" << nv->layMaNhanVien() << ")" << endl;
    } else {
        cout << "Added admin: " << user->layHoTen() << " (" << user->layTenDangNhap() << ")" << endl;
    }
    return true;
}

bool QuanLyNhanVien::xoaNhanVien(const string &maNV)
{
    // Admin-only operation
    if (!isAdmin())
    {
        cerr << "Error: Only admin can delete employees" << endl;
        return false;
    }

    for (int i = 0; i < danhSachNhanVien.size(); i++)
    {
        NhanVien *nv = dynamic_cast<NhanVien*>(danhSachNhanVien[i]);
        if (nv && nv->layMaNhanVien() == maNV)
        {
            cout << "Deleting employee: " << danhSachNhanVien[i]->layHoTen() << endl;
            delete danhSachNhanVien[i];
            danhSachNhanVien.erase(i);

            // Auto-save to CSV after delete
            luuCSV();

            return true;
        }
    }

    cerr << "Error: Employee not found: " << maNV << endl;
    return false;
}

bool QuanLyNhanVien::capNhatNhanVien(const string &maNV, const NhanVien &nvMoi)
{
    // Admin-only operation
    if (!isAdmin())
    {
        cerr << "Error: Only admin can update employees" << endl;
        return false;
    }

    NhanVien *nv = timNhanVien(maNV);
    if (nv == nullptr)
    {
        cerr << "Error: Employee not found: " << maNV << endl;
        return false;
    }

    // Check username conflict (exclude current employee)
    if (kiemTraUsernameTonTai(nvMoi.layTenDangNhap(), maNV))
    {
        cerr << "Error: Username already exists: " << nvMoi.layTenDangNhap() << endl;
        return false;
    }

    // Check phone conflict (exclude current employee)
    if (kiemTraSDTTonTai(nvMoi.laySoDienThoai(), maNV))
    {
        cerr << "Error: Phone number already exists: " << nvMoi.laySoDienThoai() << endl;
        return false;
    }

    *nv = nvMoi;

    // Auto-save to CSV after update
    luuCSV();

    cout << "Updated employee: " << nv->layHoTen() << endl;
    return true;
}

NhanVien *QuanLyNhanVien::timNhanVien(const string &maNV)
{
    for (int i = 0; i < danhSachNhanVien.size(); i++)
    {
        NhanVien *nv = dynamic_cast<NhanVien*>(danhSachNhanVien[i]);
        if (nv && nv->layMaNhanVien() == maNV)
        {
            return nv;
        }
    }
    return nullptr;
}

const MangDong<NguoiDung *> &QuanLyNhanVien::layDanhSachNhanVien() const
{
    return danhSachNhanVien;
}

// ========== SEARCH ==========
NhanVien *QuanLyNhanVien::timNhanVienTheoUsername(const string &username)
{
    for (int i = 0; i < danhSachNhanVien.size(); i++)
    {
        if (danhSachNhanVien[i]->layTenDangNhap() == username)
        {
            return dynamic_cast<NhanVien*>(danhSachNhanVien[i]);
        }
    }
    return nullptr;
}

NhanVien *QuanLyNhanVien::timNhanVienTheoSDT(const string &sdt)
{
    for (int i = 0; i < danhSachNhanVien.size(); i++)
    {
        if (danhSachNhanVien[i]->laySoDienThoai() == sdt)
        {
            return dynamic_cast<NhanVien*>(danhSachNhanVien[i]);
        }
    }
    return nullptr;
}

MangDong<NhanVien *> QuanLyNhanVien::timNhanVienTheoTen(const string &ten)
{
    MangDong<NhanVien *> ketQua;
    // Cast to NhanVien for search results
    string tenLower = ten;
    transform(tenLower.begin(), tenLower.end(), tenLower.begin(), ::tolower);

    for (int i = 0; i < danhSachNhanVien.size(); i++)
    {
        string hoTen = danhSachNhanVien[i]->layHoTen();
        transform(hoTen.begin(), hoTen.end(), hoTen.begin(), ::tolower);

        if (hoTen.find(tenLower) != string::npos)
        {
            // Try to cast to NhanVien
            NhanVien *nv = dynamic_cast<NhanVien*>(danhSachNhanVien[i]);
            if (nv) {
                ketQua.push_back(nv);
            }
        }
    }
    return ketQua;
}

// ========== AUTHENTICATION ==========
NhanVien *QuanLyNhanVien::xacThucDangNhap(const string &username, const string &password)
{
    NhanVien *nv = timNhanVienTheoUsername(username);
    if (nv == nullptr)
    {
        cout << "Login failed: Username not found: " << username << endl;
        return nullptr;
    }

    if (nv->layMatKhau() != password)
    {
        cout << "Login failed: Incorrect password for: " << username << endl;
        return nullptr;
    }

    // Check if account is active
    // Note: NhanVien doesn't have layHoatDong(), skip this check for now
    // TODO: Add active status field to NhanVien model

    cout << "Login successful: " << nv->layHoTen() << " (" << nv->layTenDangNhap() << ")" << endl;
    return nv;
}

// ========== STATISTICS ==========
int QuanLyNhanVien::tongSoNhanVien() const
{
    return danhSachNhanVien.size();
}

int QuanLyNhanVien::demSoAdmin() const
{
    int count = 0;
    for (int i = 0; i < danhSachNhanVien.size(); i++)
    {
        if (danhSachNhanVien[i]->layVaiTro() == VaiTro::QUAN_TRI_VIEN)
        {
            count++;
        }
    }
    return count;
}

int QuanLyNhanVien::demSoStaff() const
{
    int count = 0;
    for (int i = 0; i < danhSachNhanVien.size(); i++)
    {
        if (danhSachNhanVien[i]->layVaiTro() == VaiTro::NHAN_VIEN)
        {
            count++;
        }
    }
    return count;
}

// ========== ID GENERATION ==========
string QuanLyNhanVien::taoMaNhanVienMoi()
{
    maxEmployeeId++;
    stringstream ss;
    ss << "NV" << setw(3) << setfill('0') << maxEmployeeId;
    return ss.str();
}

bool QuanLyNhanVien::kiemTraUsernameTonTai(const string &username, const string &excludeMaNV) const
{
    for (int i = 0; i < danhSachNhanVien.size(); i++)
    {
        NhanVien *nv = dynamic_cast<NhanVien*>(danhSachNhanVien[i]);
        string maNV = nv ? nv->layMaNhanVien() : danhSachNhanVien[i]->layTenDangNhap();
        
        if (maNV != excludeMaNV && danhSachNhanVien[i]->layTenDangNhap() == username)
        {
            return true;
        }
    }
    return false;
}

bool QuanLyNhanVien::kiemTraSDTTonTai(const string &sdt, const string &excludeMaNV) const
{
    for (int i = 0; i < danhSachNhanVien.size(); i++)
    {
        NhanVien *nv = dynamic_cast<NhanVien*>(danhSachNhanVien[i]);
        string maNV = nv ? nv->layMaNhanVien() : danhSachNhanVien[i]->layTenDangNhap();
        
        if (maNV != excludeMaNV && danhSachNhanVien[i]->laySoDienThoai() == sdt)
        {
            return true;
        }
    }
    return false;
}

// ========== CSV I/O (ADMIN ONLY) ==========
bool QuanLyNhanVien::luuCSV(const string &filename) const
{
    // Admin-only operation
    if (!isAdmin() && currentUser != nullptr)
    {
        cerr << "Error: Only admin can save employee data to CSV" << endl;
        return false;
    }

    vector<vector<string>> rows;

    // Header row
    rows.push_back({"MaNV", "HoTen", "SoDienThoai", "GioiTinh", "NgaySinh",
                    "TenDangNhap", "MatKhau", "Luong", "CaLam"});

    // Data rows - only save NhanVien (skip QuanTriVien)
    for (int i = 0; i < danhSachNhanVien.size(); i++)
    {
        NguoiDung *user = danhSachNhanVien[i];
        
        // Skip admin (QuanTriVien) - only save staff
        if (user->layVaiTro() == VaiTro::QUAN_TRI_VIEN) {
            continue;
        }
        
        NhanVien *nv = dynamic_cast<NhanVien*>(user);
        if (!nv) continue; // Safety check
        
        vector<string> row;
        row.push_back(nv->layMaNhanVien());
        row.push_back(nv->layHoTen());
        row.push_back(nv->laySoDienThoai());
        row.push_back(nv->layGioiTinh());
        row.push_back(nv->layNgaySinh());
        row.push_back(nv->layTenDangNhap());
        row.push_back(nv->layMatKhau());
        
        // Luong
        row.push_back(to_string(nv->layLuongCoBan()));
        
        // Ca lam
        CaLamViec ca = nv->layCaLamViec();
        string caStr = "SANG";
        if (ca == CaLamViec::CHIEU) caStr = "CHIEU";
        else if (ca == CaLamViec::TOI) caStr = "TOI";
        row.push_back(caStr);

        rows.push_back(row);
    }

    // Extract header
    vector<string> header = rows[0];
    rows.erase(rows.begin());
    
    bool success = CSVManager::writeCSV(filename, header, rows);
    if (success)
    {
        cout << "Saved " << danhSachNhanVien.size() << " employees to CSV: " << filename << endl;
    }
    else
    {
        cerr << "Error: Failed to save employees to CSV: " << filename << endl;
    }
    return success;
}

bool QuanLyNhanVien::docCSV(const string &filename)
{
    vector<vector<string>> rows = CSVManager::readCSV(filename, false);

    if (rows.empty())
    {
        cout << "No employee data found in CSV: " << filename << endl;
        maxEmployeeId = 0;
        return true; // Not an error, just empty file
    }

    // Clear only staff list, NOT admin list (admin managed separately)
    for (int i = 0; i < danhSachNhanVien.size(); i++)
    {
        delete danhSachNhanVien[i];
    }
    danhSachNhanVien.clear();
    
    isLoadingFromCSV = true; // Prevent auto-save during load

    // Skip header row, read employee data starting from row 1
    maxEmployeeId = 0;

    for (size_t i = 1; i < rows.size(); i++)
    {
        const auto &row = rows[i];

        // Format: MaNV, HoTen, SoDienThoai, GioiTinh, NgaySinh, TenDangNhap, MatKhau, Luong, CaLam
        if (row.size() < 9)
        {
            cerr << "Invalid CSV row at line " << (i + 2) << ": insufficient columns" << endl;
            continue;
        }

        try
        {
            string maNV = row[0];
            string hoTen = row[1];
            string sdt = row[2];
            string gioiTinh = row[3];
            string ngaySinh = row[4];
            string username = row[5];
            string password = row[6];
            string luongStr = row[7];
            string caLamStr = row[8];

            // Parse salary
            double luong = 0;
            try {
                luong = stod(luongStr);
            } catch (...) {
                luong = 0;
            }

            // Parse shift
            CaLamViec caLam = CaLamViec::SANG;
            if (caLamStr == "CHIEU") caLam = CaLamViec::CHIEU;
            else if (caLamStr == "TOI") caLam = CaLamViec::TOI;

            // Create NhanVien only (no admin in CSV)
            NhanVien *nv = new NhanVien(hoTen, sdt, "N/A", username, password, maNV, luong, caLam);
            nv->datGioiTinh(gioiTinh);
            nv->datNgaySinh(ngaySinh);
            NguoiDung *nguoiDung = nv;

            danhSachNhanVien.push_back(nguoiDung);

            // Update maxEmployeeId from NV### format
            if (maNV.length() > 2 && maNV.substr(0, 2) == "NV")
            {
                try
                {
                    int id = stoi(maNV.substr(2));
                    if (id > maxEmployeeId)
                        maxEmployeeId = id;
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
    cout << "Loaded " << danhSachNhanVien.size() << " employees from CSV" << endl;
    cout << "  - Admins: " << demSoAdmin() << endl;
    cout << "  - Staff: " << demSoStaff() << endl;
    return true;
}

void QuanLyNhanVien::xoaTatCa()
{
    // Clear both staff and admin lists (used in destructor)
    for (int i = 0; i < danhSachNhanVien.size(); i++)
    {
        delete danhSachNhanVien[i];
    }
    danhSachNhanVien.clear();
    
    for (int i = 0; i < danhSachQuanTriVien.size(); i++)
    {
        delete danhSachQuanTriVien[i];
    }
    danhSachQuanTriVien.clear();
    
    maxEmployeeId = 0;
}

// ========== ADMIN CSV I/O ==========
bool QuanLyNhanVien::luuAdminCSV(const std::string &filename) const
{
    vector<vector<string>> rows;

    // Header row (same format as nhanvien.csv for consistency)
    rows.push_back({"TenDangNhap", "MatKhau", "HoTen", "SoDienThoai", "GioiTinh", "NgaySinh"});

    // Data rows
    for (int i = 0; i < danhSachQuanTriVien.size(); i++)
    {
        QuanTriVien *admin = danhSachQuanTriVien[i];
        
        vector<string> row;
        row.push_back(admin->layTenDangNhap());
        row.push_back(admin->layMatKhau());
        row.push_back(admin->layHoTen());
        row.push_back(admin->laySoDienThoai());
        row.push_back(admin->layGioiTinh());
        row.push_back(admin->layNgaySinh());
        
        rows.push_back(row);
    }

    // Extract header
    vector<string> header = rows[0];
    rows.erase(rows.begin());
    
    bool success = CSVManager::writeCSV(filename, header, rows);
    if (success)
    {
        cout << "Saved " << danhSachQuanTriVien.size() << " admins to CSV: " << filename << endl;
    }
    else
    {
        cerr << "Error: Failed to save admins to CSV: " << filename << endl;
    }
    
    return success;
}

bool QuanLyNhanVien::docAdminCSV(const std::string &filename)
{
    vector<vector<string>> rows = CSVManager::readCSV(filename, false);

    if (rows.empty())
    {
        cout << "No admin data found in CSV: " << filename << endl;
        
        // Create default admin if file empty
        QuanTriVien *defaultAdmin = new QuanTriVien(
            "Quản Trị Viên",
            "0123456789",
            "",
            "admin",
            "admin123"
        );
        defaultAdmin->datGioiTinh("Nam");
        defaultAdmin->datNgaySinh("01/01/1990");
        defaultAdmin->datHoatDong(true);
        
        danhSachQuanTriVien.push_back(defaultAdmin);
        
        // Save default admin to CSV
        luuAdminCSV(filename);
        
        cout << "Created default admin account: admin/admin123" << endl;
        return true;
    }

    // Clear existing admins
    for (int i = 0; i < danhSachQuanTriVien.size(); i++)
    {
        delete danhSachQuanTriVien[i];
    }
    danhSachQuanTriVien.clear();

    // Skip header row, read admin data starting from row 1
    for (size_t i = 1; i < rows.size(); i++)
    {
        const auto &row = rows[i];

        // Format: TenDangNhap, MatKhau, HoTen, SoDienThoai, GioiTinh, NgaySinh
        if (row.size() < 6)
        {
            cerr << "Invalid admin CSV row at line " << (i + 2) << ": insufficient columns" << endl;
            continue;
        }

        try
        {
            string username = row[0];
            string password = row[1];
            string hoTen = row[2];
            string sdt = row[3];
            string gioiTinh = row[4];
            string ngaySinh = row[5];

            QuanTriVien *admin = new QuanTriVien(
                hoTen,
                sdt,
                "", // address
                username,
                password
            );
            admin->datGioiTinh(gioiTinh);
            admin->datNgaySinh(ngaySinh);
            admin->datHoatDong(true);
            
            danhSachQuanTriVien.push_back(admin);
            
            cout << "Loaded admin: " << hoTen << " (" << username << ")" << endl;
        }
        catch (const exception &e)
        {
            cerr << "Error parsing admin CSV row " << (i + 2) << ": " << e.what() << endl;
            continue;
        }
    }

    cout << "Loaded " << danhSachQuanTriVien.size() << " admins from CSV" << endl;
    return true;
}

QuanTriVien *QuanLyNhanVien::timQuanTriVienTheoUsername(const std::string &username) const
{
    for (int i = 0; i < danhSachQuanTriVien.size(); i++)
    {
        if (danhSachQuanTriVien[i]->layTenDangNhap() == username)
        {
            return danhSachQuanTriVien[i];
        }
    }
    return nullptr;
}
