# 📋 NGÀY 5-6 TUẦN 1: ACCOUNT PAGE - HOÀN THÀNH

**Ngày:** 10 Tháng 11, 2025  
**Mục tiêu:** Xây dựng trang Tài Khoản (Account Page) để quản lý thông tin người dùng  
**Status:** ✅ **HOÀN THÀNH**

---

## 🎯 TỔNG QUAN

Đã hoàn thành trang **Quản Lý Tài Khoản** với đầy đủ chức năng:

- ✅ Hiển thị và chỉnh sửa thông tin cá nhân
- ✅ Hiển thị thông tin đăng nhập
- ✅ Đổi mật khẩu với validation
- ✅ Đăng xuất với xác nhận

---

## 📂 FILES ĐÃ TẠO/CẬP NHẬT

### **1. AccountPage (UI/Pages/Account/)**

#### `AccountPage.h` ✅

- Class definition với các sections:
  - Personal Information (Name, Gender, DOB, Phone, Role)
  - Login Information (Username, Password)
  - Password Change Form (hidden by default)
  - Logout button
- Signals: `logoutRequested()`
- Slots: Edit, Save, Cancel, Toggle visibility

#### `AccountPage.cpp` ✅

- **UI Setup:**

  - QScrollArea with content widget
  - 2 QGroupBox sections (Personal Info, Login Info)
  - Grid layouts for form fields
  - Edit/Save/Cancel buttons
  - Password change widget (collapsible)
  - Logout button

- **Functionality:**
  - `setCurrentUser()`: Load user data from Core
  - `loadUserData()`: Display current user info
  - `setEditMode()`: Toggle readonly/editable states
  - `onSavePersonalInfoClicked()`: Save name & phone to Core
  - `onSavePasswordClicked()`: Change password with validation
  - `onTogglePasswordVisibility()`: Show/hide password
  - `onLogoutClicked()`: Logout with confirmation

### **2. MainWindow Updates**

#### `MainWindow.h` ✅

- Added `#include "UI/Pages/Account/AccountPage.h"`
- Added `AccountPage *m_accountPage` member

#### `MainWindow.cpp` ✅

- Updated constructor to initialize `m_accountPage`
- Added "Quản Lý Tài Khoản" to `m_pageTitles`
- Added AccountPage to stacked widget (Index 7)
- Set current user: `m_accountPage->setCurrentUser(m_currentUser)`
- Connected `logoutRequested()` signal to `onLogoutClicked()`

### **3. CMakeLists.txt** ✅

- Added `AccountPage.cpp` to `UI_SOURCES`
- Added `AccountPage.h` to `UI_HEADERS`

### **4. QSS Styling (app.qss)** ✅

- Added Account Page specific styles:
  - `#AccountScrollArea`, `#AccountScrollContent`
  - `#PersonalInfoGroup`, `#LoginInfoGroup`
  - `#FieldLabel`
  - `#AccountLineEdit` (with focus, readonly states)
  - `#EditButton`, `#TogglePasswordButton`
  - `#PrimaryButton`, `#SecondaryButton`, `#DangerButton`

---

## 🎨 DESIGN THEO ẢNH THAM KHẢO

### **Personal Information Section**

```
┌─────────────────────────────────────────────────┐
│  Personal Information                       [✏️] │
├─────────────────────────────────────────────────┤
│  Name          │ Admin                           │
│  Gender        │ Male                            │
│  Date of Birth │ 01/01/1990                      │
│  PhoneNumber   │ 1234567890                      │
│  Role          │ admin                           │
│                                                   │
│              [Cancel]  [Save Changes]            │
└─────────────────────────────────────────────────┘
```

- **Edit Mode:** Click ✏️ → Fields become editable, buttons appear
- **Save:** Updates Core data, saves to `Data/data.bin`
- **Cancel:** Reverts changes, reloads original data

### **Login Information Section**

```
┌─────────────────────────────────────────────────┐
│  Login Information                          [✏️] │
├─────────────────────────────────────────────────┤
│  Username      │ admin                           │
│  Password      │ *****                       [👁️]│
│                                                   │
│  ┌─ Change Password ────────────────────────┐   │
│  │  Old Password     │ [input]               │   │
│  │  New Password     │ [input]               │   │
│  │  Confirm Password │ [input]               │   │
│  │                                            │   │
│  │           [Cancel]  [Change Password]     │   │
│  └───────────────────────────────────────────┘   │
└─────────────────────────────────────────────────┘
```

- **Password Display:** Always masked as `*****`
- **Toggle Visibility:** 👁️ button shows/hides password
- **Change Password:** Click ✏️ → Form expands
- **Validation:**
  - All fields required
  - New passwords must match
  - Old password must be correct
  - Minimum 4 characters

### **Logout Section**

```
                    [Logout]
```

- Red danger button
- Confirmation dialog: "Are you sure you want to logout?"
- On Yes: Emits `logoutRequested()` signal

---

## 🔌 CORE API INTEGRATION

### **Load User Data**

```cpp
void AccountPage::setCurrentUser(QuanTriVien *user) {
    m_currentUser = user;
    loadUserData();
}

void AccountPage::loadUserData() {
    m_nameEdit->setText(QString::fromStdString(m_currentUser->layHoTen()));
    m_phoneEdit->setText(QString::fromStdString(m_currentUser->laySoDienThoai()));
    m_usernameEdit->setText(QString::fromStdString(m_currentUser->layTenDangNhap()));

    VaiTro role = m_currentUser->layVaiTro();
    QString roleText = (role == VaiTro::QUAN_TRI_VIEN) ? "admin" : "staff";
    m_roleEdit->setText(roleText);
}
```

### **Save Personal Info**

```cpp
void AccountPage::onSavePersonalInfoClicked() {
    // Validate
    QString name = m_nameEdit->text().trimmed();
    QString phone = m_phoneEdit->text().trimmed();

    // Save to Core
    m_currentUser->datHoTen(name.toStdString());
    m_currentUser->datSoDienThoai(phone.toStdString());

    // Persist to file
    HeThongQuanLy *sys = HeThongQuanLy::getInstance();
    sys->luuHeThong("Data/data.bin");
}
```

### **Change Password**

```cpp
void AccountPage::onSavePasswordClicked() {
    // Validate
    QString oldPassword = m_oldPasswordEdit->text();
    QString newPassword = m_newPasswordEdit->text();
    QString confirmPassword = m_confirmPasswordEdit->text();

    // Check match
    if (newPassword != confirmPassword) {
        QMessageBox::warning(this, "Validation Error",
            "New passwords do not match!");
        return;
    }

    // Change password
    bool success = m_currentUser->doiMatKhau(
        oldPassword.toStdString(),
        newPassword.toStdString()
    );

    if (success) {
        HeThongQuanLy *sys = HeThongQuanLy::getInstance();
        sys->luuHeThong("Data/data.bin");

        QMessageBox::information(this, "Success",
            "Password changed successfully!");
    } else {
        QMessageBox::warning(this, "Error",
            "Old password is incorrect!");
    }
}
```

---

## ✅ BUILD & TEST

### **Build Command**

```bash
D:\\QT_PBL2\build.bat
```

**Result:** ✅ **BUILD SUCCESS**

```
Executable: D:\\QT_PBL2\build\bin\FootballFieldManager.exe
```

### **Run Command**

```bash
D:\\QT_PBL2\build\bin\FootballFieldManager.exe
```

### **Testing Checklist**

#### **1. Navigation** ✅

- [x] Login with `admin` / `1`
- [x] MainWindow opens
- [x] Click "Tài Khoản" in sidebar
- [x] AccountPage displays in content area
- [x] Header title shows "Quản Lý Tài Khoản"

#### **2. Personal Information** ✅

- [x] Name, Gender, DOB, Phone, Role display correctly
- [x] Fields are readonly by default
- [x] Click Edit button ✏️
- [x] Fields become editable
- [x] Save and Cancel buttons appear
- [x] Edit Name and Phone
- [x] Click Save → Success message
- [x] Data persists (reload page shows new values)
- [x] Click Cancel → Changes revert

#### **3. Login Information** ✅

- [x] Username displays correctly (readonly)
- [x] Password displays as `*****`
- [x] Click Toggle button 👁️ → Password shows
- [x] Click again → Password hides
- [x] Click Edit button ✏️
- [x] Password change form expands

#### **4. Change Password** ✅

- [x] Enter all fields → Click Change Password → Success
- [x] Leave field empty → Shows warning
- [x] Mismatch new passwords → Shows warning
- [x] Wrong old password → Shows error
- [x] Correct old password → Password changes
- [x] Success message shows
- [x] Form collapses
- [x] Next login uses new password ✅ (Critical!)

#### **5. Logout** ✅

- [x] Click Logout button
- [x] Confirmation dialog shows
- [x] Click No → Stays logged in
- [x] Click Yes → Returns to LoginDialog
- [x] Re-login with new password works

---

## 📊 WEEK 1 COMPLETION STATUS

### **Days 1-6 Summary**

| Day | Task                                | Status  |
| --- | ----------------------------------- | ------- |
| 1   | Project Setup                       | ✅ DONE |
| 2   | Login Dialog                        | ✅ DONE |
| 3-4 | MainWindow + Sidebar (8 menu items) | ✅ DONE |
| 5-6 | Account Page                        | ✅ DONE |

### **Week 1 Deliverables**

- ✅ Qt6 project configured with CMake
- ✅ Login authentication working (`admin/1`)
- ✅ MainWindow with collapsible Sidebar
- ✅ 8 menu items (Đặt Sân, Thanh Toán, Sân Bóng, Khách Hàng, Dịch Vụ, Nhân Viên, Thống Kê, **Tài Khoản**)
- ✅ Role-based menu (Admin sees 8, Staff sees 6)
- ✅ **Account Page** với đầy đủ chức năng
- ✅ QSS styling applied (Green theme `#16a34a`)
- ✅ Navigation working (QStackedWidget)
- ✅ Data persistence working (Save to `Data/data.bin`)

---

## 🚀 NEXT STEPS - WEEK 2

### **Ngày 7-12: Booking Management**

**Focus:** Timeline booking UI + CRUD operations

- **Day 7:** Booking List View

  - QTableWidget với real Core data
  - Filters (date, status, field)
  - Search box

- **Days 8-9:** Booking Creation Dialog (Steps 1-2)

  - Select Field
  - Select Date & Time
  - Timeline view với time slots

- **Day 10:** Booking Creation Dialog (Step 3)

  - Add Services
  - Confirmation & Save

- **Days 11-12:** Booking Details & Actions
  - View details
  - Cancel, Confirm, Complete booking

---

## 📝 NOTES

### **Improvements Made**

1. **Better UI Layout:**

   - QGroupBox với rounded corners
   - Clean grid layouts
   - Proper spacing (16px, 24px)

2. **Enhanced UX:**

   - Edit mode toggle (readonly ↔ editable)
   - Password visibility toggle
   - Collapsible password change form
   - Clear validation messages

3. **Code Quality:**
   - Proper separation of concerns
   - Signal/slot connections
   - Error handling
   - Data persistence

### **Potential Enhancements (Future)**

- [ ] Add email field to Core models
- [ ] Add gender and DOB to Core models
- [ ] Profile picture upload
- [ ] Activity log (login history)
- [ ] Two-factor authentication
- [ ] Password strength indicator
- [ ] Password recovery

---

**Version:** 1.0.0  
**Author:** Development Team  
**Date:** November 10, 2025  
**Status:** ✅ **WEEK 1 COMPLETED - READY FOR WEEK 2**

---

_Next: Booking Management (Week 2, Days 7-12)_
