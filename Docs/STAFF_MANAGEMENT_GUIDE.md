# 📚 HƯỚNG DẪN SỬ DỤNG HỆ THỐNG QUẢN LÝ NHÂN VIÊN
## Staff Management System - Comprehensive Usage Guide

**Date:** November 18, 2025  
**Version:** 1.0.0  
**Author:** Football Field Management System

---

## 📋 MỤC LỤC

1. [Tổng Quan Hệ Thống](#1-tổng-quan-hệ-thống)
2. [Kiến Trúc Access Control](#2-kiến-trúc-access-control)
3. [Quy Trình Admin](#3-quy-trình-admin)
4. [Quy Trình Staff](#4-quy-trình-staff)
5. [Data Synchronization](#5-data-synchronization)
6. [Cấu Trúc File CSV](#6-cấu-trúc-file-csv)
7. [Troubleshooting](#7-troubleshooting)

---

## 1. TỔNG QUAN HỆ THỐNG

### 1.1 Chức Năng Chính

Hệ thống quản lý nhân viên cung cấp:
- ✅ **CRUD Operations**: Thêm, Sửa, Xóa, Tìm kiếm nhân viên
- ✅ **Role Management**: Phân quyền Admin/Staff
- ✅ **Access Control**: Kiểm soát truy cập 3 lớp
- ✅ **CSV Persistence**: Lưu trữ dữ liệu vào `nhanvien.csv`
- ✅ **Account Management**: Quản lý thông tin cá nhân
- ✅ **Data Sync**: Đồng bộ giữa Account và Staff Management

### 1.2 Vai Trò Người Dùng

#### **Admin (Quản Trị Viên)**
- Quyền: **Full Access**
- Có thể:
  - ✅ Xem danh sách tất cả nhân viên
  - ✅ Thêm nhân viên mới (Admin/Staff)
  - ✅ Sửa thông tin nhân viên bất kỳ
  - ✅ Xóa nhân viên
  - ✅ Truy cập trang "Quản Lý Nhân Viên"
  - ✅ Đọc/ghi file `nhanvien.csv`
  - ✅ Sửa thông tin cá nhân trong trang Account

#### **Staff (Nhân Viên)**
- Quyền: **Restricted Access**
- Có thể:
  - ✅ Xem và sửa thông tin cá nhân trong trang Account
  - ❌ KHÔNG thể truy cập trang "Quản Lý Nhân Viên"
  - ❌ KHÔNG thể xem thông tin nhân viên khác
  - ❌ KHÔNG thể đọc/ghi file `nhanvien.csv`

---

## 2. KIẾN TRÚC ACCESS CONTROL

### 2.1 Hybrid 3-Layer Security Model

```
┌─────────────────────────────────────────────────┐
│  LAYER 1: UI Hiding (Frontend)                 │
│  - Sidebar hides "Staff Management" for Staff   │
│  - Menu item only visible to Admin              │
└─────────────────────────────────────────────────┘
                      ↓
┌─────────────────────────────────────────────────┐
│  LAYER 2: Navigation Blocking (Logic)          │
│  - MainWindow checks role before navigation     │
│  - Redirects Staff to Dashboard if accessing    │
└─────────────────────────────────────────────────┘
                      ↓
┌─────────────────────────────────────────────────┐
│  LAYER 3: File-Level Protection (Core)         │
│  - QuanLyNhanVien::luuCSV() checks isAdmin()    │
│  - QuanLyNhanVien::docCSV() checks isAdmin()    │
│  - Returns false if non-admin tries to access   │
└─────────────────────────────────────────────────┘
```

### 2.2 Implementation Details

#### **Layer 1: UI Hiding (Sidebar.cpp)**
```cpp
void Sidebar::setUserRole(VaiTro role) {
    if (role == VaiTro::NHAN_VIEN) {
        // Hide Staff Management menu item
        m_staffManagementItem->setVisible(false);
    } else {
        // Show for Admin
        m_staffManagementItem->setVisible(true);
    }
}
```

#### **Layer 2: Navigation Blocking (MainWindow.cpp)**
```cpp
void MainWindow::onMenuItemClicked(const QString &menuName) {
    if (menuName == "Quản Lý Nhân Viên") {
        if (m_currentUser->layVaiTro() != VaiTro::QUAN_TRI_VIEN) {
            QMessageBox::warning(this, "Từ chối truy cập", 
                "Chỉ Admin mới có quyền truy cập!");
            return;
        }
    }
    // Navigate to page...
}
```

#### **Layer 3: File Protection (QuanLyNhanVien.cpp)**
```cpp
bool QuanLyNhanVien::luuCSV(const string &filename) const {
    // Admin-only operation
    if (!isAdmin() && currentUser != nullptr) {
        cerr << "Error: Only admin can save employee data to CSV" << endl;
        return false;
    }
    // Proceed with save...
}
```

---

## 3. QUY TRÌNH ADMIN

### 3.1 Đăng Nhập

1. **Mở ứng dụng** → Màn hình đăng nhập
2. **Nhập thông tin**:
   - Username: `admin` (từ `nhanvien.csv`)
   - Password: `admin123`
3. **Click "Đăng nhập"**
4. **System loads**:
   - Đọc `nhanvien.csv` → Load tất cả staff
   - Set `currentUser` = Admin object
   - Show MainWindow với full menu

### 3.2 Xem Danh Sách Nhân Viên

1. **Click menu "👨‍💼 Quản Lý Nhân Viên"**
2. **Trang hiển thị**:
   - Table bên trái (70%): Danh sách tất cả nhân viên
   - Form bên phải (30%): Chi tiết nhân viên
3. **Table columns**:
   ```
   | Mã NV | Họ Tên | Giới Tính | Ngày Sinh | SĐT | Username | Password | Vai Trò | Trạng Thái |
   ```
4. **Search bar**: Tìm theo Tên/SĐT/Username

### 3.3 Thêm Nhân Viên Mới

#### **Step 1: Click "➕ Thêm Mới"**
- Form bên phải xóa sạch
- Cursor focus vào "Họ và Tên"

#### **Step 2: Nhập thông tin**
```
┌─────────────────────────────────────┐
│ Họ và Tên:    [Nguyễn Văn B       ] │
│                                      │
│ Giới tính:    ○ Nam  ● Nữ  ○ Khác  │
│                                      │
│ Ngày sinh:    [15/05/1995 ▼]       │
│                                      │
│ Số điện thoại:[0987654322          ] │
│                                      │
│ Tên đăng nhập:[staff2              ] │
│                                      │
│ Mật khẩu:     [********            ] │
│                                      │
│ Vai trò:      [Staff ▼]            │
│               (Admin / Staff)       │
└─────────────────────────────────────┘
```

#### **Step 3: Click "💾 Lưu"**
- **Validation check**:
  - ✅ Họ tên không rỗng
  - ✅ SĐT >= 10 ký tự, chỉ số
  - ✅ Username >= 3 ký tự, chưa tồn tại
  - ✅ Password >= 6 ký tự (bắt buộc cho NV mới)
- **Core operations**:
  1. Generate mã NV mới: `NV004`
  2. Create `NhanVien` hoặc `QuanTriVien` object
  3. Set gender, DOB, active status
  4. Call `staffManager->themNhanVien(nv)`
  5. Auto-save to `nhanvien.csv`
  6. Reload table

#### **Step 4: Thông báo kết quả**
```
✅ Thành công
Đã thêm nhân viên mới!

[OK]
```

### 3.4 Sửa Thông Tin Nhân Viên

#### **Step 1: Click vào dòng trong table**
- Form auto-fill với dữ liệu từ nhân viên đã chọn
- Delete button enabled

#### **Step 2: Chỉnh sửa thông tin**
- Có thể sửa: Tên, Gender, DOB, Phone, Username, Password, Role
- Password để trống = giữ nguyên password cũ
- Password nhập mới = cập nhật password

#### **Step 3: Click "💾 Lưu"**
- **Validation check** (tương tự thêm mới)
- **Core operations**:
  1. Create `NhanVien` object với data mới
  2. Copy mã NV từ selected staff
  3. Call `staffManager->capNhatNhanVien(maNV, nvMoi)`
  4. Auto-save to `nhanvien.csv`
  5. Reload table

#### **Step 4: Thông báo kết quả**
```
✅ Thành công
Đã cập nhật thông tin nhân viên!

[OK]
```

### 3.5 Xóa Nhân Viên

#### **Step 1: Click vào dòng trong table**
#### **Step 2: Click "🗑️ Xóa"**
- **Confirmation dialog**:
```
⚠️ Xác nhận xóa
Bạn có chắc chắn muốn xóa nhân viên 'Nguyễn Văn B' (NV002)?

[Có]  [Không]
```

#### **Step 3: Click "Có"**
- **Core operations**:
  1. Call `staffManager->xoaNhanVien(maNV)`
  2. Delete object from memory
  3. Auto-save to `nhanvien.csv`
  4. Reload table
  5. Clear form

#### **Step 4: Thông báo kết quả**
```
✅ Thành công
Đã xóa nhân viên!

[OK]
```

### 3.6 Quản Lý Thông Tin Cá Nhân (Account Page)

#### **Xem thông tin**
1. Click menu "👤 Tài Khoản"
2. Trang hiển thị:
   - **Thông Tin Cá Nhân**: Họ tên, SĐT, Giới tính, Ngày sinh, Vai trò
   - **Thông Tin Đăng Nhập**: Username (read-only), Password (masked)

#### **Sửa thông tin cá nhân**
1. Click "✏️ Chỉnh Sửa Thông Tin"
2. Dialog hiển thị với data hiện tại
3. Sửa: Tên, Phone, Gender, DOB
4. Click "Lưu"
5. **System sync**:
   - Update `NhanVien` object trong Core
   - Auto-save to `nhanvien.csv`
   - Thay đổi xuất hiện ngay trong Staff Management (nếu Admin xem)

#### **Đổi mật khẩu**
1. Click "🔒 Đổi Mật Khẩu"
2. Dialog hiển thị:
   - Mật khẩu cũ
   - Mật khẩu mới
   - Xác nhận mật khẩu mới
3. Click "Lưu"
4. **System sync**:
   - Validate password cũ
   - Update password trong Core
   - Auto-save to `nhanvien.csv`

---

## 4. QUY TRÌNH STAFF

### 4.1 Đăng Nhập

1. **Mở ứng dụng** → Màn hình đăng nhập
2. **Nhập thông tin**:
   - Username: `staff1` (từ `nhanvien.csv`)
   - Password: `staff123`
3. **Click "Đăng nhập"**
4. **System loads**:
   - Set `currentUser` = NhanVien object
   - Show MainWindow với **restricted menu**
   - **"Quản Lý Nhân Viên" HIDDEN**

### 4.2 Menu Visible cho Staff

```
📊 Đặt Sân          ← Có quyền
💰 Thanh Toán       ← Có quyền
⚽ Quản Lý Sân      ← Có quyền
👥 Khách Hàng       ← Có quyền
🍹 Dịch Vụ          ← Có quyền
👨‍💼 Nhân Viên        ← ẨNẨN
📈 Thống Kê         ← Có quyền (limited)
👤 Tài Khoản        ← Có quyền
```

### 4.3 Quản Lý Thông Tin Cá Nhân

Staff có thể:
- ✅ Xem thông tin cá nhân (giống Admin)
- ✅ Sửa thông tin cá nhân (Dialog)
- ✅ Đổi mật khẩu

**Quy trình giống Admin** (xem Section 3.6)

### 4.4 Giới Hạn Truy Cập

#### **Nếu Staff cố gắng truy cập Staff Management**
```
Scenario 1: Menu item ẩn
→ Staff không thấy menu item
→ Không thể click

Scenario 2: URL manipulation (future-proof)
→ MainWindow kiểm tra VaiTro
→ Show error dialog:
   ⚠️ Từ chối truy cập
   Chỉ Admin mới có quyền truy cập!
→ Redirect về Dashboard

Scenario 3: Direct Core access
→ QuanLyNhanVien::luuCSV() check isAdmin()
→ Return false
→ Console error: "Only admin can save employee data"
```

---

## 5. DATA SYNCHRONIZATION

### 5.1 Architecture Overview

```
┌──────────────────────────────────────────────────────────┐
│                    HeThongQuanLy                         │
│  (Singleton - Central Coordinator)                       │
└──────────────────┬──────────────────────────────────────┘
                   │
         ┌─────────┴─────────┐
         │                   │
         ▼                   ▼
┌─────────────────┐  ┌─────────────────┐
│ QuanLyNhanVien  │  │  NhanVien       │
│  - CRUD ops     │  │  - Model class  │
│  - CSV I/O      │  │  - Gender       │
│  - Admin check  │  │  - DOB          │
└────────┬────────┘  └────────┬────────┘
         │                    │
         │  Load/Save         │  Inherit
         ▼                    ▼
┌─────────────────┐  ┌─────────────────┐
│ nhanvien.csv    │  │ QuanTriVien     │
│  (Data/...)     │  │  (Admin)        │
└─────────────────┘  └─────────────────┘
         ▲
         │  Auto-save after CRUD
         │
┌────────┴─────────────────────────────┐
│  StaffManagementPage                 │
│   - Add/Edit/Delete → Save CSV       │
└──────────────────────────────────────┘

┌──────────────────────────────────────┐
│  AccountPage                         │
│   - Edit personal info → Save CSV    │
└──────────────────────────────────────┘
```

### 5.2 Sync Scenarios

#### **Scenario A: Admin sửa Staff trong Staff Management**
1. Admin click row → Form populate
2. Admin edit name: "Nguyễn Văn B" → "Nguyễn Văn C"
3. Admin click "Lưu"
4. **Backend**:
   - `staffManager->capNhatNhanVien(maNV, nvMoi)`
   - Update `NhanVien` object in memory
   - `staffManager->luuCSV("nhanvien.csv")`
   - Write all staff to CSV
5. **Frontend**:
   - Table reload → Show "Nguyễn Văn C"
6. **If Staff "Nguyễn Văn C" is logged in**:
   - Their AccountPage shows OLD data (in memory)
   - **Solution**: Re-login to reload from CSV

#### **Scenario B: Staff sửa thông tin cá nhân trong Account**
1. Staff click "Chỉnh Sửa Thông Tin"
2. Staff edit phone: "0987654322" → "0987654399"
3. Staff click "Lưu" in dialog
4. **Backend**:
   - `currentUser->datSoDienThoai("0987654399")`
   - Update `NhanVien` object in memory
   - ⚠️ **Issue**: Staff cannot call `luuCSV()` (admin-only)
   - **Solution**: Call via `HeThongQuanLy`
5. **Frontend**:
   - AccountPage reload → Show "0987654399"
6. **If Admin views Staff Management**:
   - Table auto-reload → Show "0987654399"

#### **Scenario C: Multiple Admin instances (future)**
- **Problem**: Admin A edits → Save CSV → Admin B doesn't see changes
- **Solution 1**: File watcher (reload CSV on change)
- **Solution 2**: Database instead of CSV
- **Current**: Single-instance app, no issue

### 5.3 CSV Auto-Save Triggers

| Action | Trigger | Method |
|--------|---------|--------|
| Add Staff | `themNhanVien()` | Auto-save if `!isLoadingFromCSV` |
| Update Staff | `capNhatNhanVien()` | Auto-save after update |
| Delete Staff | `xoaNhanVien()` | Auto-save after delete |
| Edit Account | `AccountPage::onSave()` | Manually call `system->luuNhanVienCSV()` |

### 5.4 CSV Load Timing

```
┌─────────────────────────────────────────────────┐
│  main.cpp                                       │
│  1. HeThongQuanLy::getInstance()                │
│  2. docHeThong(data.bin) - Load binary          │
│  3. docNhanVienCSV("nhanvien.csv") ← HERE       │
│  4. LoginDialog → Admin login                   │
│  5. MainWindow(currentUser)                     │
│  6. StaffManagementPage::setCurrentAdmin()      │
│  7. StaffManagementPage::loadStaff()            │
└─────────────────────────────────────────────────┘
```

---

## 6. CẤU TRÚC FILE CSV

### 6.1 File Location
```
D:/QT_PBL2/Data/nhanvien.csv
```

### 6.2 CSV Format

#### **Header Row**
```csv
MaNV,HoTen,SoDienThoai,GioiTinh,NgaySinh,TenDangNhap,MatKhau,VaiTro,TrangThai
```

#### **Data Rows (Example)**
```csv
NV001,Admin User,0987654321,Nam,01/01/1990,admin,admin123,Admin,Hoat dong
NV002,Staff One,0987654322,Nữ,15/05/1995,staff1,staff123,Staff,Hoat dong
NV003,Staff Two,0987654323,Nam,20/08/1998,staff2,staff123,Staff,Khoa
```

### 6.3 Field Specifications

| Column | Type | Format | Required | Notes |
|--------|------|--------|----------|-------|
| **MaNV** | String | `NV###` | Yes | Auto-generated, 3-digit padded |
| **HoTen** | String | UTF-8 | Yes | Full name |
| **SoDienThoai** | String | Digits | Yes | Phone number, 10+ digits |
| **GioiTinh** | String | Nam/Nữ/Khác | Yes | Gender |
| **NgaySinh** | String | DD/MM/YYYY | Yes | Date of birth |
| **TenDangNhap** | String | Alphanumeric | Yes | Username, unique, 3+ chars |
| **MatKhau** | String | Plain text | Yes | Password, 6+ chars (⚠️ unencrypted) |
| **VaiTro** | String | Admin/Staff | Yes | Role: "Admin" → QUAN_TRI_VIEN, "Staff" → NHAN_VIEN |
| **TrangThai** | String | Hoat dong/Khoa | Yes | Status: Active or Locked |

### 6.4 CSV Parsing Logic

```cpp
// Read CSV
vector<vector<string>> rows = CSVManager::readCSV(filename, false);

// Skip header (row 0), parse from row 1
for (size_t i = 1; i < rows.size(); i++) {
    const auto &row = rows[i];
    if (row.size() < 9) continue; // Invalid row
    
    string maNV = row[0];
    string hoTen = row[1];
    string sdt = row[2];
    string gioiTinh = row[3];
    string ngaySinh = row[4];
    string username = row[5];
    string password = row[6];
    string vaiTroStr = row[7];
    string trangThaiStr = row[8];
    
    // Parse role
    VaiTro vaiTro = (vaiTroStr == "Admin") 
        ? VaiTro::QUAN_TRI_VIEN 
        : VaiTro::NHAN_VIEN;
    
    // Create object
    NhanVien *nv = (vaiTro == VaiTro::QUAN_TRI_VIEN)
        ? new QuanTriVien(hoTen, sdt, username, password, maNV)
        : new NhanVien(hoTen, sdt, username, password, maNV);
    
    nv->datGioiTinh(gioiTinh);
    nv->datNgaySinh(ngaySinh);
    nv->datHoatDong(trangThaiStr == "Hoat dong");
    
    danhSachNhanVien.push_back(nv);
}
```

### 6.5 CSV Writing Logic

```cpp
vector<vector<string>> rows;

// Header
rows.push_back({"MaNV", "HoTen", "SoDienThoai", "GioiTinh", 
                "NgaySinh", "TenDangNhap", "MatKhau", 
                "VaiTro", "TrangThai"});

// Data
for (int i = 0; i < danhSachNhanVien.size(); i++) {
    NhanVien *nv = danhSachNhanVien[i];
    vector<string> row;
    
    row.push_back(nv->layMaNhanVien());
    row.push_back(nv->layHoTen());
    row.push_back(nv->laySoDienThoai());
    row.push_back(nv->layGioiTinh());
    row.push_back(nv->layNgaySinh());
    row.push_back(nv->layTenDangNhap());
    row.push_back(nv->layMatKhau());
    row.push_back(nv->layVaiTro() == VaiTro::QUAN_TRI_VIEN 
        ? "Admin" : "Staff");
    row.push_back(nv->layHoatDong() ? "Hoat dong" : "Khoa");
    
    rows.push_back(row);
}

CSVManager::writeCSV(filename, rows);
```

---

## 7. TROUBLESHOOTING

### 7.1 Common Issues

#### **Issue 1: Staff không thấy menu "Quản Lý Nhân Viên"**
- **Expected**: Đúng, Staff không có quyền
- **Solution**: Đăng nhập bằng Admin account

#### **Issue 2: Admin thêm staff nhưng CSV không cập nhật**
- **Cause**: `isLoadingFromCSV` flag stuck
- **Solution**:
  1. Check `QuanLyNhanVien::themNhanVien()`
  2. Ensure `!isLoadingFromCSV` before `luuCSV()`
  3. Restart app

#### **Issue 3: CSV bị corrupt sau khi sửa thủ công**
- **Symptom**: App crash hoặc load 0 staff
- **Cause**: Invalid CSV format (missing columns, wrong delimiter)
- **Solution**:
  1. Backup CSV: `nhanvien.csv.bak`
  2. Fix format: Ensure 9 columns, comma-separated
  3. Reload app

#### **Issue 4: Password không khớp sau khi đổi**
- **Symptom**: Login failed với password mới
- **Cause**: CSV chưa save hoặc cache issue
- **Solution**:
  1. Check `nhanvien.csv` → Verify password column
  2. Restart app to clear memory cache
  3. Re-login

#### **Issue 5: Table hiển thị sai thông tin**
- **Cause**: Data not synced after edit
- **Solution**:
  1. Click "Refresh" button (nếu có)
  2. Re-enter Staff Management page
  3. Check `loadStaff()` in debug mode

### 7.2 Debug Commands

#### **Check CSV content**
```bash
Get-Content D:\QT_PBL2\Data\nhanvien.csv
```

#### **Check staff count in Core**
```cpp
cout << "Total staff: " << staffManager->tongSoNhanVien() << endl;
cout << "Admins: " << staffManager->demSoAdmin() << endl;
cout << "Staff: " << staffManager->demSoStaff() << endl;
```

#### **Verify current user role**
```cpp
cout << "Current user: " << currentUser->layHoTen() << endl;
cout << "Role: " << (currentUser->layVaiTro() == VaiTro::QUAN_TRI_VIEN 
    ? "Admin" : "Staff") << endl;
```

### 7.3 Reset to Default

#### **Reset CSV**
```csv
MaNV,HoTen,SoDienThoai,GioiTinh,NgaySinh,TenDangNhap,MatKhau,VaiTro,TrangThai
NV001,Admin User,0987654321,Nam,01/01/1990,admin,admin123,Admin,Hoat dong
```

#### **Reset binary data**
```bash
Remove-Item D:\QT_PBL2\Data\data.bin
# App will regenerate with sample data
```

---

## 📞 LIÊN HỆ HỖ TRỢ

**Email**: support@footballmanager.com  
**Phone**: 0987-654-321  
**GitHub**: https://github.com/ThuongLVT/PBL2-

---

**© 2025 Football Field Management System. All Rights Reserved.**
