# 🎨 KẾ HOẠCH PHÁT TRIỂN UI - QT_FOOTBALL

**Tạo:** 8 Tháng 11, 2025  
**Cập nhật:** 8 Tháng 11, 2025 - Added React port details  
**Mục tiêu:** Xây dựng Qt6 GUI đẹp như React web version  
**React Reference:** `d:\html_test\src` (Chỉ tham khảo design ideas)  
**Timeline:** 3 tuần (18 ngày)  
**Status:** 📝 Planning Complete - READY TO START

---

## 🔗 REACT PROJECT REFERENCE (Design Inspiration Only)

**Location:** `d:\html_test\src` - Web version, tham khảo ý tưởng thiết kế

**Chỉ lấy ý tưởng:**

- Colors & styling từ `src/index.css` → `UI/Resources/styles/app.qss`
- Layout concepts → Qt C++ implementation
- Timeline booking UI design → Qt custom widgets

**Không port trực tiếp:**

- ❌ React components → Code Qt C++ from scratch
- ❌ Recharts → Không dùng charts
- ❌ React Router → QStackedWidget (Qt native)
- ❌ Dashboard → Bỏ hoàn toàn

---

## 🎯 DESIGN PHILOSOPHY

### **Principles:**

✅ **Qt C++ Native** - Pure Qt6 Widgets, không port từ React  
✅ **Simple & Clean** - Minimal UI, tập trung vào chức năng  
✅ **NO Dashboard** - Không cần stats/charts, chỉ quản lý CRUD  
✅ **Simple Sidebar** - Nút ba gạch + Menu + Admin icon  
✅ **NO Bridge** - UI gọi trực tiếp `HeThongQuanLy::getInstance()`  
✅ **Green Theme** - `#16a34a` (tham khảo React color)  
✅ **Functional Focus** - CRUD operations cho Booking/Field/Customer/Payment  
✅ **Role-Based Access** - Admin vs Staff permissions

### **React Reference (Design Ideas Only):**

🎨 **Color Palette:**

- Lấy ý tưởng màu xanh lá `#16a34a` từ React
- Shadows, spacing, typography concepts

🏗️ **Layout Concepts:**

- Sidebar collapsible idea (but simpler implementation)
- Clean table designs
- Form styling ideas

📅 **Booking Timeline:**

- Timeline view concept với time slots
- Color coding cho trạng thái sân
- Visual feedback khi select

### **Color Palette (từ React index.css):**

```css
/* Primary Green Theme - EXACT từ React */
--primary-green:       #16a34a  /* Green 600 - Main brand */
--primary-green-dark:  #15803d  /* Green 700 - Hover states */
--primary-green-light: #22c55e  /* Green 500 - Accents */
--secondary-green:     #4ade80  /* Light green - Highlights */
--bg-green-light:      #f0fdf4  /* Very light green - Backgrounds */

/* Text Colors */
--text-dark:           #1f2937  /* Gray 800 - Headers */
--text-gray:           #6b7280  /* Gray 500 - Body text */
--text-light:          #9ca3af  /* Gray 400 - Placeholders */

/* Backgrounds */
--bg-gray:             #f9fafb  /* Gray 50 - Light backgrounds */
--surface:             #ffffff  /* White - Cards, dialogs */

/* Borders */
--border-gray:         #e5e7eb  /* Gray 200 - Dividers */

/* Status Colors */
--success-green:       #10b981  /* Emerald 500 - Success states */
--warning-yellow:      #f59e0b  /* Amber 500 - Warnings */
--danger-red:          #ef4444  /* Red 500 - Errors, cancel */
--info-blue:           #3b82f6  /* Blue 500 - Info, links */

/* Shadows (từ React) */
--shadow-sm:  0 1px 2px 0 rgba(0, 0, 0, 0.05);
--shadow-md:  0 4px 6px -1px rgba(0, 0, 0, 0.1);
--shadow-lg:  0 10px 15px -3px rgba(0, 0, 0, 0.1);
--shadow-xl:  0 20px 25px -5px rgba(0, 0, 0, 0.1);

/* Background Gradient (từ React body) */
background: linear-gradient(135deg, #f0fdf4 0%, #e5e7eb 100%);
```

### **QSS Translation:**

```qss
/* QWidget backgrounds */
QWidget#centralWidget {
    background: qlineargradient(
        x1: 0, y1: 0, x2: 1, y2: 1,
        stop: 0 #f0fdf4, stop: 1 #e5e7eb
    );
}

/* QPushButton primary */
QPushButton#primaryButton {
    background-color: #16a34a;
    color: white;
    border: none;
    border-radius: 8px;
    padding: 10px 20px;
    font-weight: 600;
}

QPushButton#primaryButton:hover {
    background-color: #15803d;
}

/* QTableWidget (giống React table) */
QTableWidget {
    background-color: white;
    border: 1px solid #e5e7eb;
    border-radius: 8px;
    gridline-color: #e5e7eb;
}

QTableWidget::item:selected {
    background-color: #f0fdf4;
    color: #16a34a;
}
```

---

## 📅 TIMELINE - 3 TUẦN (18 NGÀY)

### **TUẦN 1: FOUNDATION, LOGIN & LAYOUT** (Ngày 1-6)

**Mục tiêu:** Setup project, login, main window với simple sidebar

#### **Ngày 1: Project Setup** ✅ COMPLETED

- [x] Tạo thư mục `UI/` structure (Pages/, Components/, Dialogs/, Resources/)
- [x] Setup `CMakeLists.txt` với Qt6 Widgets (NO Qt Charts)
- [x] Tạo `resources.qrc` (icons từ lucide-react equivalents)
- [x] Tạo `app.qss` stylesheet (port từ React index.css)
- [x] Test build & run empty window

**Deliverables:** ✅

- ✅ `UI/Resources/resources.qrc`
- ✅ `UI/Resources/styles/app.qss` (simple QSS với green theme)
- ✅ CMakeLists.txt updated với Qt6 Widgets (NO Qt Charts)

---

#### **Ngày 2: Login Dialog** ✅ COMPLETED

- [x] `UI/Pages/Login/LoginDialog.h/cpp/ui`
- [x] Form: Username + Password + Đăng nhập button
- [x] Gọi `HeThongQuanLy::getInstance()` để xác thực
- [x] Admin credentials: admin/1
- [x] Staff credentials: staff1/staff123
- [x] Error messages với QMessageBox
- [x] Styling với QSS
- [x] Football field background image (1920x1080)
- [x] Dark semi-transparent dialog (rgba(0,0,0,180))
- [x] Close button to exit application

**Core API sử dụng:**

```cpp
HeThongQuanLy *sys = HeThongQuanLy::getInstance();
QuanTriVien *admin = sys->timQuanTriVien(username.toStdString());
if (admin && admin->layMatKhau() == password.toStdString()) {
    // Login success
}
```

**Deliverables:** ✅

- ✅ LoginDialog working với real authentication
- ✅ QSS styling cho login form
- ✅ Football field background image created (D:\\QT_PBL2\UI\Resources\images\football_field_bg.jpg)
- ✅ Build successful: D:\\QT_PBL2\build\bin\FootballFieldManager.exe
- ✅ Login tested with admin/1 credentials

**Build & Run:**

```bash
# Build
D:\\QT_PBL2\build.bat

# Run
D:\\QT_PBL2\build\bin\FootballFieldManager.exe
```

---

#### **Ngày 3-4: MainWindow với Simple Sidebar** ✅ COMPLETED

- [x] `UI/MainWindow/MainWindow.h/cpp/ui`
- [x] `UI/Components/Sidebar.h/cpp/ui`
  - [x] **Nút ba gạch (☰)** - Toggle collapse/expand
  - [x] **Menu Items** (Icons + Text when expanded):
    1. 📅 Đặt Sân
    2. 💳 Thanh Toán
    3. ⚽ Sân Bóng
    4. 👥 Khách Hàng
    5. 🍺 Dịch Vụ
    6. 👨‍💼 Nhân Viên (Admin only)
    7. 📊 Thống Kê (Admin only)
    8. 👤 Tài Khoản - **NEW!** Chức năng thứ 8, hiển thị trong content area như 7 chức năng khác
  - [x] Collapsed width: 60px (chỉ icons)
  - [x] Expanded width: 250px (icons + text "Sân bóng DUT")
- [x] `UI/Components/Header.h/cpp/ui`
  - [x] Page title (dynamic) - "Quản lý + [Chức năng]"
  - [x] Fixed height: 60px (same as sidebar toggle section)
- [x] Layout: Sidebar (left) + Right Panel (Header top + Content center)
- [x] QStackedWidget cho pages với 8 placeholder pages (thêm Tài Khoản)
- [x] 3 divider lines trong sidebar (sau toggle, sau menu items, trước admin - BỎ vì không còn Admin icon)

**QSS Styling:** ✅

- Sidebar: white background, #e5e7eb borders
- Collapsed: 60px wide, only icons visible
- Expanded: 250px wide, icons + text visible
- Menu hover: #f3f4f6 background
- Active menu: #16a34a left border (3px), #dcfce7 background
- Header: 60px height, matches toggle section
- **Tài Khoản** button: Có icon 👤, hiển thị như menu item bình thường

**Deliverables:** ✅

- [x] MainWindow với collapsible Sidebar working
- [x] Navigation working với QStackedWidget
- [x] **8 menu items** (thêm Tài Khoản)
- [x] Role-based menu (Admin sees all 8, Staff sees 6 items - hides Nhân Viên + Thống Kê)
- [x] Smooth animation với QPropertyAnimation
- [x] SVG icons loaded from resources
- [x] QSS styling applied
- [x] 8 placeholder pages created (thêm AccountPage)
- [x] Build successful: `D:\\QT_PBL2\build\bin\FootballFieldManager.exe`

**Testing Results:** ✅

- ✅ Login với admin/1 → MainWindow hiển thị
- ✅ Sidebar mặc định collapsed (60px)
- ✅ Toggle button expand sidebar → 250px với text "Sân bóng DUT"
- ✅ 8 menu items hiển thị với icons
- ✅ Click menu items → Switch pages, header title update
- ✅ Tài Khoản page hiển thị ở content area
- ✅ Logout có confirmation dialog

---

#### **Ngày 5-6: Account Management Page** ✅ COMPLETED

- [x] `UI/Pages/Account/AccountPage.h/cpp/ui`
  - [x] **Personal Information section:**
    - [x] Display: Name, Gender, Date of Birth, Phone Number, Role
    - [x] Editable fields: Name, Gender, Date of Birth, Phone Number
    - [x] Edit button with icon
    - [x] "Save Changes" and "Cancel" buttons (shown when editing)
  - [x] **Login Information section:**
    - [x] Display: Username (readonly)
    - [x] Display: Password (masked as **\***)
    - [x] Toggle password visibility button
    - [x] Edit button to change password
  - [x] **Change Password form (hidden by default):**
    - [x] Old Password field
    - [x] New Password field
    - [x] Confirm Password field
    - [x] Validation: Match new passwords, old password correct
    - [x] "Change Password" and "Cancel" buttons
  - [x] **Logout button:**
    - [x] Confirmation dialog
    - [x] Logout and return to LoginDialog

**Core API:**

```cpp
// Get current user info
HeThongQuanLy *sys = HeThongQuanLy::getInstance();
QuanTriVien *currentUser = sys->layQuanTriVienHienTai();

// Update user info
currentUser->datHoTen(newName);
currentUser->datSoDienThoai(newPhone);
sys->luuHeThong("Data/data.bin"); // Save to file

// Change password
bool success = currentUser->doiMatKhau(oldPassword, newPassword);
if (success) {
    sys->luuHeThong("Data/data.bin"); // Save to file
}
```

**Deliverables:** ✅

- [x] Account page with Personal Information section ✅
- [x] Account page with Login Information section ✅
- [x] Edit personal info working ✅
- [x] Change password working with validation ✅
- [x] Logout working with confirmation ✅
- [x] AccountPage integrated into MainWindow (Index 7) ✅
- [x] QSS styling for AccountPage applied ✅
- [x] Week 1 complete: Login + Layout + Account features ✅

**Build & Test Results:** ✅

```bash
# Build successful
D:\\QT_PBL2\build.bat

# Run
D:\\QT_PBL2\build\bin\FootballFieldManager.exe
```

**Testing Checklist:**

- [x] Login with admin/1 → MainWindow shows
- [x] Click "Tài Khoản" in sidebar → AccountPage displays
- [x] Personal Information section displays correctly
- [x] Click Edit button → Fields become editable
- [x] Edit Name and Phone → Click Save → Data persists
- [x] Click Cancel → Changes revert
- [x] Click Edit Password → Password change form appears
- [x] Enter old password, new password, confirm → Click Change Password
- [x] Validation: Empty fields shows warning
- [x] Validation: Mismatched passwords shows warning
- [x] Validation: Incorrect old password shows error
- [x] Successful password change → Confirmation message
- [x] Toggle password visibility button works
- [x] Click Logout → Confirmation dialog → Returns to Login

---

### **TUẦN 2: BOOKING MANAGEMENT** (Ngày 7-12)

**Mục tiêu:** Timeline booking + CRUD operations

#### **Ngày 7: Booking List View**

- [ ] `UI/Pages/Booking/BookingListPage.h/cpp/ui`
- [ ] QTableWidget hiển thị danh sách đặt sân
- [ ] Columns: ID | Sân | Khách hàng | Thời gian | Trạng thái | Actions
- [ ] Status badges (Confirmed/Pending/Cancelled)
- [ ] Filters: Theo ngày, theo trạng thái, theo sân
- [ ] Search box
- [ ] New Booking button

**Core API:**

```cpp
HeThongQuanLy *sys = HeThongQuanLy::getInstance();
const MangDong<DatSan*>& bookings = sys->layDanhSachDatSan();

for (int i = 0; i < bookings.size(); i++) {
    DatSan *booking = bookings[i];
    // Populate table row
}
```

**Deliverables:**

- Booking list table với data từ Core
- Filters working
- Table styling

---

#### **Ngày 8-9: Booking Creation Dialog (Part 1 & 2)**

- [ ] `UI/Dialogs/BookingDialog.h/cpp/ui`
- [ ] **Step 1: Select Field**
  - [ ] List các sân available (QListWidget)
  - [ ] Hiển thị loại sân, giá thuê
  - [ ] Filter theo loại (5/7 người)
- [ ] **Step 2: Select Date & Time**
  - [ ] QCalendarWidget để chọn ngày
  - [ ] Time slot picker (khung giờ available)
  - [ ] Kiểm tra trùng lịch real-time
- [ ] **Step 3: Select Customer**
  - [ ] Search customer by phone/name
  - [ ] Dropdown existing customers
  - [ ] "Add New Customer" button inline

**Core API:**

```cpp
// Check available slots
HeThongQuanLy *sys = HeThongQuanLy::getInstance();
bool available = sys->kiemTraSanTrong(san, ngayGio, khungGio);

// Get available fields
MangDong<San*> fields = sys->timSanTrong(ngayGio, khungGio);
```

**Deliverables:**

- Booking dialog với 3 steps
- Field selection working
- Date/Time picker working
- Customer selection working

---

#### **Ngày 10: Booking Creation Dialog (Part 3)**

- [ ] **Step 4: Add Services (Optional)**
  - [ ] List dịch vụ available (DoUong + ThietBi)
  - [ ] Checkboxes hoặc quantity spinners
  - [ ] Tính tổng tiền real-time
- [ ] **Step 5: Confirmation**
  - [ ] Summary: Sân, Thời gian, Khách hàng, Dịch vụ, Tổng tiền
  - [ ] Ghi chú field
  - [ ] "Xác nhận đặt sân" button

**Core API:**

```cpp
// Create booking
DatSan *booking = sys->taoDatSan(customer, field, ngayGio, khungGio);

// Add services
for (each selected service) {
    DichVuDat dvDat(service, quantity);
    booking->themDichVu(dvDat);
}

booking->tinhTongTien();
```

**Deliverables:**

- Service selection working
- Booking confirmation working
- Data saves to Core

---

#### **Ngày 11-12: Booking Details & Actions**

- [ ] `UI/Dialogs/BookingDetailsDialog.h/cpp/ui`
- [ ] View full booking details (read-only)
- [ ] Actions: Hủy đặt sân, Xác nhận, Hoàn thành
- [ ] Status update với confirmation
- [ ] Refresh booking list sau khi update

**Core API:**

```cpp
// Cancel booking
sys->huyDatSan(bookingId);

// Update status
sys->capNhatTrangThaiDatSan(bookingId, TrangThaiDatSan::DA_XAC_NHAN);
```

**Deliverables:**

- Booking details dialog
- Cancel/Confirm/Complete actions working

---

### **TUẦN 3: PAYMENT, FIELD, CUSTOMER, SERVICE, STAFF, STATISTICS** (Ngày 13-18)

**Mục tiêu:** Complete all CRUD operations + Statistics

#### **Ngày 13: Payment Management**

- [ ] `UI/Pages/Payment/PaymentListPage.h/cpp/ui`
- [ ] QTableWidget: All payments
- [ ] Columns: Mã | Đặt sân | Khách hàng | Số tiền | Phương thức | Ngày
- [ ] Filter by date range
- [ ] Filter by payment method
- [ ] `UI/Dialogs/PaymentDialog.h/cpp/ui`
  - [ ] Select booking to pay
  - [ ] Payment method (Cash/Card/Transfer)
  - [ ] Amount input
  - [ ] Confirmation

**Core API:**

```cpp
// Create payment
ThanhToan *payment = sys->taoThanhToan(booking, phuongThuc);

// List payments
const MangDong<ThanhToan*>& payments = sys->layDanhSachThanhToan();
```

**Deliverables:**

- Payment list + creation
- Payment dialog working

---

#### **Ngày 14: Field Management**

- [ ] `UI/Pages/Field/FieldManagementPage.h/cpp/ui`
- [ ] QTableWidget: List all fields
- [ ] Columns: Mã | Tên | Loại | Giá | Trạng thái | Actions
- [ ] Add Field button → Dialog
- [ ] Edit Field (double-click row)
- [ ] Delete Field với confirmation
- [ ] `UI/Dialogs/FieldDialog.h/cpp/ui`
  - [ ] Form: Mã, Tên, Loại (combo), Giá (spinbox), Ghi chú
  - [ ] Validation
  - [ ] Save/Cancel buttons

**Core API:**

```cpp
// Create
San *field = new San(ma, ten, loai, gia);
sys->themSan(field);

// Update
sys->capNhatSan(ma, fieldMoi);

// Delete
sys->xoaSan(ma);

// List
const MangDong<San*>& fields = sys->layDanhSachSan();
```

**Deliverables:**

- Field list table
- Add/Edit/Delete working
- Field dialog styled

---

#### **Ngày 15: Customer Management** ✅ COMPLETED

- [x] `UI/Pages/Customer/CustomerManagementPage.h/cpp`
- [x] **Left Panel (70%):**
  - [x] Search box (by name or phone)
  - [x] Filter by member tier (Tất cả, Thường, Đồng, Bạc, Vàng, Kim cương)
  - [x] Filter by spending range (< 1M, 1-5M, 5-10M, 10-20M, >= 20M)
  - [x] 3 Stats cards: Total customers, New customers (this month), VIP customers
  - [x] Customer table with columns: Mã KH, Tên KH, SĐT, Hội viên, Tổng chi tiêu, Ngày đăng ký
- [x] **Right Top Panel (60%):**
  - [x] Customer details form: Họ tên, Số điện thoại, Tổng chi tiêu (read-only)
  - [x] Buttons: [+ Thêm khách hàng], [Lưu], [Xoá]
  - [x] Empty by default, populate on table row click
  - [x] Phone validation (10-11 digits, unique)
- [x] **Right Bottom Panel (40%):**
  - [x] Booking history list with scroll
  - [x] Display: Field name, Date/Time, Total amount
- [x] **CRUD Operations:**
  - [x] Add new customer (click +, fill form, save)
  - [x] Edit customer (click row, modify, save)
  - [x] Delete customer (click row, click delete, confirm)
  - [x] Unique customer ID generation (KH001, KH002, ..., non-reusable)
  - [x] Phone uniqueness validation (exclude current customer on edit)
- [x] QSS styling applied (stats cards, table, form, buttons)

**Core API Updates:**

```cpp
// Updated KhachHang model: Spending-based tier system
enum class HangKhachHang {
    THUONG,    // < 1,000,000 VND - 0% discount
    DONG,      // 1,000,000 - 4,999,999 VND - 2% discount
    BAC,       // 5,000,000 - 9,999,999 VND - 4% discount
    VANG,      // 10,000,000 - 19,999,999 VND - 6% discount
    KIM_CUONG  // >= 20,000,000 VND - 8% discount
};

// Unique customer ID generation
string QuanLyKhachHang::taoMaKhachHangMoi();
bool QuanLyKhachHang::kiemTraSDTTonTai(const string &sdt, const string &excludeMaKH);

// Customer spending tracking
double KhachHang::layTongChiTieu() const;
void KhachHang::themChiTieu(double soTien);
void KhachHang::capNhatHang(); // Auto-update tier based on spending
```

**Deliverables:** ✅

- ✅ Customer management page with 3-panel layout (70% left, 30% right split 60/40)
- ✅ Search and filters working
- ✅ Stats cards displaying real-time data
- ✅ Customer table with all columns
- ✅ Add/Edit/Delete customer operations
- ✅ Phone validation (10-11 digits, unique)
- ✅ Unique customer ID system (non-reusable after deletion)
- ✅ Booking history display for selected customer
- ✅ Spending-based tier system (Thường, Đồng, Bạc, Vàng, Kim cương)
- ✅ QSS styling applied
- ✅ Build successful: `D:\\QT_PBL2\build\bin\FootballFieldManager.exe`

**Testing Checklist:**

- [ ] Login với admin/1 → MainWindow hiển thị
- [ ] Click "Khách Hàng" in sidebar → CustomerManagementPage displays
- [ ] Stats cards show correct counts
- [ ] Search by name works
- [ ] Search by phone works
- [ ] Filter by tier works
- [ ] Filter by spending range works
- [ ] Click customer row → Details populate, booking history loads
- [ ] Click "+ Thêm khách hàng" → Form clears, ready for new customer
- [ ] Enter name + phone → Click "Lưu" → Customer added with unique ID
- [ ] Try to add duplicate phone → Shows warning
- [ ] Try to add phone with < 10 or > 11 digits → Shows warning
- [ ] Click customer row → Edit name/phone → Click "Lưu" → Changes saved
- [ ] Click customer row → Click "Xoá" → Confirmation dialog → Customer deleted
- [ ] After delete, add new customer → ID increments (non-reusable)
- [ ] Customer with spending >= 20M shows as "Kim cương" in table

---

#### **Ngày 16: Service Management**

- [ ] `UI/Dialogs/PaymentDialog.h/cpp/ui`
- [ ] Trigger từ Booking details ("Thanh toán")
- [ ] Display: Booking info, Total amount
- [ ] Payment method radio buttons:
  - [ ] Tiền mặt (cash) - input tiền khách đưa
  - [ ] Thẻ (card) - input số thẻ, loại thẻ
  - [ ] Chuyển khoản (transfer) - input STK, ngân hàng
- [ ] Tính tiền thừa cho cash
- [ ] "Xác nhận thanh toán" button

**Core API:**

```cpp
// Create payment
ThanhToan *payment = sys->taoThanhToan(
    booking,
    PhuongThucThanhToan::TIEN_MAT
);

// For cash payment
ThanhToanTienMat *cashPayment = dynamic_cast<ThanhToanTienMat*>(payment);
cashPayment->setTienKhachDua(amount);
cashPayment->tinhTienThua();
cashPayment->xacNhanThanhToan();
```

**Deliverables:**

- Payment dialog với 3 methods
- Payment creation working
- Invoice preview (optional)

---

#### **Ngày 17: Staff Management (Admin Only)**

- [ ] `UI/Pages/Staff/StaffManagementPage.h/cpp/ui`
- [ ] QTableWidget: List staff (NhanVien)
- [ ] Columns: Mã | Tên | SĐT | Email | Trạng thái | Actions
- [ ] Add/Edit/Delete staff
- [ ] `UI/Dialogs/StaffDialog.h/cpp/ui`
  - [ ] Form: Mã, Tên, SĐT, Email, Username, Password
  - [ ] Validation

**Core API:**

```cpp
// Staff CRUD
NhanVien *staff = new NhanVien(ma, ten, sdt, email, username, password);
sys->themNhanVien(staff);
const MangDong<NhanVien*>& staffList = sys->layDanhSachNhanVien();
```

**Deliverables:**

- Staff management page (Admin only)
- Add/Edit/Delete staff working

---

#### **Ngày 18: Statistics (Admin Only)**

- [ ] `UI/Pages/Statistics/StatisticsPage.h/cpp/ui`
- [ ] **Revenue Stats:**
  - [ ] Date range picker (from/to)
  - [ ] Total revenue display (large number)
  - [ ] Revenue by payment method (table)
- [ ] **Customer Stats:**
  - [ ] Top customers table (by total spending)
  - [ ] Total customers count
  - [ ] New customers this month
- [ ] **Booking Stats:**
  - [ ] Bookings by field type (table)
  - [ ] Bookings by status (table)
  - [ ] Peak hours analysis
- [ ] **Field Stats:**
  - [ ] Field usage percentage
  - [ ] Most popular fields

**Core API:**

```cpp
ThongKeDoanhThu *revStats = new ThongKeDoanhThu(sys, tuNgay, denNgay);
revStats->tinhToan();
double revenue = revStats->getDoanhThu();

ThongKeKhachHang *custStats = new ThongKeKhachHang(sys, tuNgay, denNgay);
custStats->tinhToan();
custStats->tinhTopKhachHang(10);

ThongKeDatSan *bookStats = new ThongKeDatSan(sys, tuNgay, denNgay);
bookStats->tinhToan();
```

**Deliverables:**

- Statistics page với tables
- Revenue + Customer + Booking + Field stats
- Date range filtering working
- Export to file (optional)

---

---

## ✅ SUCCESS CRITERIA

### **Week 1: Foundation (Ngày 1-6)** ✅ COMPLETED

- [x] **Day 1:** Project Setup ✅
- [x] **Day 2:** Login working với real authentication ✅
- [x] **Day 3-4:** MainWindow với collapsible Sidebar ✅
- [x] **Day 3-4:** Sidebar: Nút ba gạch, 8 menu items (thêm Tài Khoản) ✅
- [x] **Day 3-4:** 2 divider lines trong sidebar (sau toggle, sau menu items) ✅
- [x] **Day 3-4:** Header height = toggle section height (60px) ✅
- [x] **Day 3-4:** Dynamic header title "Quản lý + [Chức năng]" ✅
- [x] **Day 5-6:** Account page: Thông tin, Đổi mật khẩu, Đăng xuất ✅
- [x] Navigation working (QStackedWidget) ✅
- [x] Role-based menu (Admin sees 8 items, Staff sees 6) ✅
- [x] QSS styling applied ✅

### **Week 2: Booking Management (Ngày 7-12)**

- [ ] Booking list displays real Core data
- [ ] Create booking workflow complete (5 steps)
- [ ] Timeline view với time slots (tham khảo React design)
- [ ] Service selection working
- [ ] Booking actions (cancel, confirm, complete)
- [ ] Data persists to Core

### **Week 3: All Features Complete (Ngày 13-18)**

- [ ] Payment management complete
- [ ] Field management CRUD complete
- [ ] Customer management CRUD complete
- [ ] Service management CRUD complete
- [ ] Staff management (Admin only)
- [ ] Statistics page (Admin only)
- [ ] All forms validated

### **Overall Quality:**

- [ ] All pages polished và consistent
- [ ] Role-based access control working
- [ ] No critical bugs
- [ ] Application stable
- [ ] Data persistence working
- [ ] Memory management proper (no leaks)

---

## 🚀 NEXT STEPS

1. **Review updated plan** ✅
2. **Setup development environment**
3. **Start Week 1 Day 1:** Project setup
4. **Daily commits** với progress tracking

---

**Timeline:** 3 tuần (18 ngày làm việc)  
**Start Date:** November 8, 2025  
**Current Progress:** Week 1 ✅ FULLY COMPLETED (Days 1-6)  
**Status:** 🎉 **WEEK 1 COMPLETED - READY FOR WEEK 2 (BOOKING MANAGEMENT)**

**Recent Updates (Nov 10, 2025):**

- ✅ **Ngày 3-4 completed:** MainWindow + Sidebar với 8 menu items
- ✅ **Ngày 5-6 completed:** Account page (thay thế Admin popup dialogs)
- ✅ Tài Khoản là chức năng thứ 8, hiển thị ở content area
- ✅ Account page có: Thông tin tài khoản (editable), Đổi mật khẩu, Đăng xuất
- ✅ Bỏ Admin icon ở footer sidebar (đã thay bằng Tài Khoản menu item)
- ✅ Role-based access: Admin sees 8 items, Staff sees 6 (hides Nhân Viên + Thống Kê)

---

_Tạo: 8 Tháng 11, 2025_  
_Cập nhật: 10 Tháng 11, 2025 - Week 1 Completed, Account page replaces Admin dialogs_  
_Version: 4.0.0 - Week 1 Complete Edition_  
_Author: Development Team_  
_React: Chỉ tham khảo design ideas (colors, layouts)_

---

## 📂 UI FOLDER STRUCTURE

```
UI/
├── MainWindow/                     Week 1 Day 3-4
│   ├── MainWindow.h/cpp/ui
│   └── Header.h/cpp/ui
│
├── Components/
│   └── Sidebar.h/cpp/ui           Week 1 Day 3-4 (Nút ba gạch + Menu + Admin icon)
│
├── Pages/
│   ├── Login/                      Week 1 Day 2
│   │   └── LoginDialog.h/cpp/ui
│   │
│   ├── Account/                    Week 1 Day 5-6 ✅ NEW!
│   │   └── AccountPage.h/cpp/ui    (Thông tin, Đổi mật khẩu, Đăng xuất)
│   │
│   ├── Booking/                    Week 2 Days 7-12
│   │   └── BookingListPage.h/cpp/ui
│   │
│   ├── Payment/                    Week 3 Day 13
│   │   └── PaymentListPage.h/cpp/ui
│   │
│   ├── Field/                      Week 3 Day 14
│   │   └── FieldManagementPage.h/cpp/ui
│   │
│   ├── Customer/                   Week 3 Day 15
│   │   └── CustomerManagementPage.h/cpp/ui
│   │
│   ├── Service/                    Week 3 Day 16
│   │   └── ServiceManagementPage.h/cpp/ui
│   │
│   ├── Staff/                      Week 3 Day 17 (Admin only)
│   │   └── StaffManagementPage.h/cpp/ui
│   │
│   └── Statistics/                 Week 3 Day 18 (Admin only)
│       └── StatisticsPage.h/cpp/ui
│
├── Dialogs/                        Week 1-3
│   ├── BookingDialog.h/cpp/ui             (Days 8-10)
│   ├── BookingDetailsDialog.h/cpp/ui      (Day 11-12)
│   ├── FieldDialog.h/cpp/ui               (Day 13)
│   ├── CustomerDialog.h/cpp/ui            (Day 14)
│   ├── ServiceDialog.h/cpp/ui             (Day 15)
│   ├── PaymentDialog.h/cpp/ui             (Day 16)
│   └── StaffDialog.h/cpp/ui               (Day 17)
│
└── Resources/                      Week 1 Day 1
    ├── icons/
    │   ├── menu.svg               (Nút ba gạch)
    │   ├── booking.svg            (Đặt sân)
    │   ├── payment.svg            (Thanh toán)
    │   ├── field.svg              (Sân bóng)
    │   ├── customer.svg           (Khách hàng)
    │   ├── service.svg            (Dịch vụ)
    │   ├── staff.svg              (Nhân viên - Admin)
    │   ├── statistics.svg         (Thống kê - Admin)
    │   ├── account.svg            (Tài khoản) ✅ NEW!
    │   └── logout.svg             (Đăng xuất)
    ├── styles/
    │   └── app.qss                 (Simple QSS)
    └── resources.qrc
```

**Menu Order:**

1. Đặt Sân → BookingListPage
2. Thanh Toán → PaymentListPage
3. Sân Bóng → FieldManagementPage
4. Khách Hàng → CustomerManagementPage
5. Dịch Vụ → ServiceManagementPage
6. Nhân Viên → StaffManagementPage (Admin only)
7. Thống Kê → StatisticsPage (Admin only)
8. **Tài Khoản → AccountPage** ✅ **NEW!** (All users)

**Total Files to Create:** ~43 files (giảm 2 dialogs: AdminInfoDialog, ChangePasswordDialog)

---

## 🔌 CORE API INTEGRATION

### **Singleton Access Pattern:**

```cpp
// In every UI class
#include "Core/QuanLy/HeThongQuanLy.h"

void MyWidget::loadData() {
    HeThongQuanLy *sys = HeThongQuanLy::getInstance();

    // Use sys->layQuanLySan()
    // Use sys->layQuanLyKhachHang()
    // etc.
}
```

### **Type Conversion Helpers:**

```cpp
// std::string ↔ QString
QString qstr = QString::fromStdString(stdstr);
std::string stdstr = qstr.toStdString();

// NgayGio ↔ QDateTime
QDateTime qdt = QDateTime(
    QDate(ngayGio.getNam(), ngayGio.getThang(), ngayGio.getNgay()),
    QTime(ngayGio.getGio(), ngayGio.getPhut(), ngayGio.getGiay())
);

NgayGio ngayGio(
    qdt.time().hour(), qdt.time().minute(), qdt.time().second(),
    qdt.date().day(), qdt.date().month(), qdt.date().year()
);
```

Tạo file `UI/Utils/TypeHelpers.h` với inline functions.

---

## 🎨 QSS STYLING GUIDE

### **app.qss Structure:**

```css
/* ===== GLOBAL ===== */
* {
  font-family: "Segoe UI", sans-serif;
}

/* ===== BUTTONS ===== */
QPushButton {
  background-color: #16a34a;
  color: white;
  border: none;
  border-radius: 6px;
  padding: 8px 16px;
  font-size: 14px;
}

QPushButton:hover {
  background-color: #15803d;
}
QPushButton:pressed {
  background-color: #14532d;
}

/* ===== TABLES ===== */
QTableWidget {
  background-color: white;
  border: 1px solid #e5e7eb;
  border-radius: 8px;
}

QHeaderView::section {
  background-color: #f3f4f6;
  padding: 12px;
  border: none;
  font-weight: bold;
  color: #1f2937;
}

QTableWidget::item {
  padding: 8px;
  border-bottom: 1px solid #e5e7eb;
}

QTableWidget::item:selected {
  background-color: #dcfce7;
  color: #15803d;
}

/* ===== INPUTS ===== */
QLineEdit {
  background-color: white;
  border: 1px solid #d1d5db;
  border-radius: 6px;
  padding: 8px 12px;
  font-size: 14px;
}

QLineEdit:focus {
  border: 1px solid #16a34a;
}

/* ===== COMBO BOX ===== */
QComboBox {
  background-color: white;
  border: 1px solid #d1d5db;
  border-radius: 6px;
  padding: 8px 12px;
}

/* ===== CALENDAR ===== */
QCalendarWidget {
  /* ... */
}

/* ===== SIDEBAR ===== */
#Sidebar {
  background-color: white;
  border-right: 1px solid #e5e7eb;
}

#Sidebar QPushButton {
  background-color: transparent;
  color: #6b7280;
  text-align: left;
  padding: 12px 16px;
  border: none;
  border-radius: 0;
}

#Sidebar QPushButton:hover {
  background-color: #f3f4f6;
}

#Sidebar QPushButton:checked {
  background-color: #dcfce7;
  color: #16a34a;
  border-left: 3px solid #16a34a;
}
```

---

## ✅ SUCCESS CRITERIA

### **Week 1: Foundation & Dashboard**

- [ ] Login working với real authentication (giống React LoginPage)
- [ ] MainWindow với Sidebar + Header layout (giống React Layout)
- [ ] Collapsible sidebar với icons (giống React)
- [ ] **Dashboard complete:**
  - [ ] 4 stats cards với icons và colors (DollarSign, Calendar, MapPin, TrendingUp)
  - [ ] Line chart: Revenue 7 ngày (giống React LineChart)
  - [ ] Bar chart: Bookings theo giờ (giống React BarChart)
  - [ ] Pie chart: Tình trạng sân (giống React PieChart)
  - [ ] Recent bookings table
- [ ] Green theme `#16a34a` applied globally (exact từ React)
- [ ] QSS shadows, gradients, borders (port từ React CSS)

### **Week 2: Booking Management**

- [ ] Booking list displays real Core data
- [ ] Timeline view với mini football fields (port từ React timeline)
- [ ] Create booking workflow complete (5 steps)
- [ ] Service selection với quantity spinbox
- [ ] Booking actions working (cancel, confirm, complete)
- [ ] Data persists to Core với HeThongQuanLy
- [ ] Color coding: Green (Trống), Red (Đã đặt) giống React

### **Week 3: CRUD Operations**

- [ ] Field management CRUD complete
- [ ] Customer management CRUD complete
- [ ] Customer tiers với colors (Bronze/Silver/Gold/Diamond) giống React
- [ ] Payment workflow complete (Cash/Card/Transfer)
- [ ] Service management basic working
- [ ] All forms validated properly

### **Week 4: Statistics & Polish**

- [ ] Revenue statistics page với multiple charts
- [ ] Customer statistics với top customers
- [ ] Field usage statistics
- [ ] Staff management (Admin only)
- [ ] All pages polished và consistent
- [ ] Role-based access control (Admin vs Staff) giống React
- [ ] No critical bugs
- [ ] Application stable & production-ready

### **Overall React Parity:**

- [ ] **Visual Consistency:** UI đẹp như React version (colors, spacing, typography)
- [ ] **Component Reusability:** Reusable widgets giống React components
- [ ] **User Experience:** Navigation, interactions, feedback giống React
- [ ] **Data Visualization:** Charts tương đương Recharts quality
- [ ] **Performance:** Responsive, no lag, smooth animations
- [ ] **Accessibility:** User-friendly, clear error messages

---

## 🚀 NEXT STEPS

1. **Review plan với team** ✅
2. **Setup development environment**
3. **Start Week 1 Day 1:** Project setup
4. **Daily commits** với progress tracking
5. **Weekly demos** để review

---
