  # ⚽ HỆ THỐNG QUẢN LÝ SÂN BÓNG ĐÁ - QT C++ OOP

## 📋 TỔNG QUAN DỰ ÁN

Dự án xây dựng phần mềm quản lý sân bóng đá sử dụng:

- **Core Logic**: C++ thuần túy với OOP (không dùng STL)
- **UI Framework**: Qt Widgets
- **Kiến trúc**: Model-View-ViewModel (MVVM)
- **Database**: File-based hoặc SQLite

---

## 🗂️ CẤU TRÚC THỨ MỤC HOÀN CHỈNH

```
FootballFieldManager/
│
├── Core/                                    # Logic nghiệp vụ (C++ thuần, không phụ thuộc Qt)
│   │
│   ├── CauTrucDuLieu/                      # Tự implement thay STL
│   │   ├── MangDong.h/cpp                  # Thay std::vector (Template)
│   │   ├── DanhSachLienKet.h/cpp           # Linked List (Template)
│   │   ├── Ngan.h/cpp                      # Stack (Template)
│   │   ├── HangDoi.h/cpp                   # Queue (Template)
│   │   ├── ThoiGian.h/cpp                  # Class thời gian (HH:MM:SS)
│   │   ├── NgayThang.h/cpp                 # Class ngày tháng (DD/MM/YYYY)
│   │   └── NgayGio.h/cpp                   # Class ngày giờ kết hợp
│   │   # NOTE: ChuoiKyTu đã loại bỏ - sử dụng std::string
│   │
│   ├── Models/                             # Các lớp đối tượng nghiệp vụ
│   │   ├── ConNguoi.h/cpp                  # Lớp cơ sở
│   │   ├── NguoiDung.h/cpp                 # Lớp cơ sở người dùng
│   │   ├── QuanTriVien.h/cpp               # Admin
│   │   ├── NhanVien.h/cpp                  # Nhân viên
│   │   ├── KhachHang.h/cpp                 # Khách hàng
│   │   ├── San.h/cpp                       # Sân bóng
│   │   ├── KhungGio.h/cpp                  # Khung giờ đặt sân
│   │   ├── DichVu.h/cpp                    # Lớp cơ sở dịch vụ (abstract)
│   │   ├── DoUong.h/cpp                    # Đồ uống (kế thừa DichVu)
│   │   ├── ThietBi.h/cpp                   # Thiết bị cho thuê (kế thừa DichVu)
│   │   ├── DichVuDat.h/cpp                 # Item dịch vụ trong đặt sân
│   │   ├── DatSan.h/cpp                    # Đơn đặt sân
│   │   ├── ThanhToan.h/cpp                 # Lớp cơ sở thanh toán (abstract)
│   │   ├── ThanhToanTienMat.h/cpp          # Thanh toán tiền mặt
│   │   ├── ThanhToanThe.h/cpp              # Thanh toán thẻ
│   │   └── ThanhToanChuyenKhoan.h/cpp      # Thanh toán chuyển khoản
│   │   # NOTE: 16 Models implemented - BaoHiem removed (not needed)
│   │
│   ├── QuanLy/                             # Các lớp quản lý
│   │   ├── HeThongQuanLy.h/cpp             # Hệ thống chính (CRUD tất cả)
│   │   ├── QuanLySan.h/cpp                 # Quản lý sân
│   │   ├── QuanLyKhachHang.h/cpp           # Quản lý khách hàng
│   │   ├── QuanLyDatSan.h/cpp              # Quản lý đặt sân
│   │   ├── QuanLyDichVu.h/cpp              # Quản lý dịch vụ
│   │   ├── QuanLyThanhToan.h/cpp           # Quản lý thanh toán
│   │   ├── FileManager.h/cpp               # ⭐ Lưu/đọc file (ghiFile/docFile)
│   │   └── BackupManager.h/cpp             # Backup/Restore dữ liệu
│   │
│   ├── ThongKe/                            # Thống kê báo cáo
│   │   ├── ThongKe.h/cpp                   # Lớp cơ sở
│   │   ├── ThongKeDoanhThu.h/cpp           # Thống kê doanh thu
│   │   └── ThongKeKhachHang.h/cpp          # Thống kê khách hàng
│   │
│   └── ThuatToan/                          # ⭐ Thuật toán (merge vào Core)
│       ├── BinarySearchTree.h/cpp          # Cây tìm kiếm nhị phân
│       ├── QuickSort.h/cpp                 # Quick sort
│       ├── MergeSort.h/cpp                 # Merge sort
│       └── HashTable.h/cpp                 # Hash table (tìm nhanh O(1))
│
├── Bridge/                                  # Cầu nối Core ↔ UI (MVVM Pattern)
│   │
│   ├── Adapters/                           # Chuyển đổi kiểu dữ liệu (Header-only)
│   │   ├── BaseAdapter.h                   # Base: std::string↔QString, NgayGio↔QDateTime
│   │   ├── SanAdapter.h                    # San model → QVariantMap
│   │   ├── KhachHangAdapter.h              # KhachHang → QVariantMap
│   │   ├── DatSanAdapter.h                 # DatSan → QVariantMap (Planned)
│   │   └── ThanhToanAdapter.h              # ThanhToan → QVariantMap (Planned)
│   │
│   └── ViewModels/                         # Logic điều khiển UI (QObject + signals/slots)
│       ├── BaseViewModel.h                 # Base: HeThongQuanLy access, error handling
│       ├── MainViewModel.h                 # Main window (dashboard, login, stats)
│       ├── BookingViewModel.h              # Booking CRUD, field availability (Planned)
│       ├── CustomerViewModel.h             # Customer management (Planned)
│       ├── StatisticsViewModel.h           # Analytics & charts (Planned)
│       ├── FieldViewModel.h                # Field management (Future)
│       ├── ServiceViewModel.h              # Service management (Future)
│       └── StaffViewModel.h                # Staff management (Future)
│
├── UI/                                      # Giao diện Qt (tái hiện từ web)
│   │
│   ├── MainWindow/                         # Cửa sổ chính
│   │   ├── MainWindow.h/cpp                # Main window logic
│   │   ├── MainWindow.ui                   # Qt Designer UI
│   │   ├── Header.h/cpp/ui                 # Header bar (search, profile)
│   │   └── Sidebar.h/cpp/ui                # Sidebar menu
│   │
│   ├── Pages/                              # Các trang chính (tương đương pages/ web)
│   │   │
│   │   ├── Dashboard/                      # Trang Dashboard
│   │   │   ├── DashboardWidget.h/cpp/ui    # Widget chính
│   │   │   ├── StatCard.h/cpp              # Card thống kê
│   │   │   ├── RevenueChart.h/cpp          # Biểu đồ doanh thu
│   │   │   └── BookingChart.h/cpp          # Biểu đồ đặt sân
│   │   │
│   │   ├── Booking/                        # Trang Đặt sân
│   │   │   ├── BookingWidget.h/cpp/ui      # Widget chính
│   │   │   ├── TimelineView.h/cpp          # Lịch timeline
│   │   │   ├── BookingDialog.h/cpp/ui      # Dialog đặt sân
│   │   │   └── HistoryDialog.h/cpp/ui      # Dialog lịch sử
│   │   │
│   │   ├── Payment/                        # Trang Thanh toán
│   │   │   ├── PaymentWidget.h/cpp/ui      # Widget chính
│   │   │   ├── PaymentDialog.h/cpp/ui      # Dialog thanh toán
│   │   │   └── InvoiceDialog.h/cpp/ui      # Dialog hóa đơn
│   │   │
│   │   ├── FieldManagement/                # Trang Quản lý sân
│   │   │   ├── FieldWidget.h/cpp/ui        # Widget chính
│   │   │   └── FieldDialog.h/cpp/ui        # Dialog thêm/sửa sân
│   │   │
│   │   ├── CustomerManagement/             # Trang Quản lý khách hàng
│   │   │   ├── CustomerWidget.h/cpp/ui     # Widget chính
│   │   │   ├── CustomerDialog.h/cpp/ui     # Dialog thêm/sửa KH
│   │   │   └── CustomerDetailDialog.h/cpp/ui # Chi tiết KH
│   │   │
│   │   ├── ServiceManagement/              # Trang Quản lý dịch vụ
│   │   │   ├── ServiceWidget.h/cpp/ui      # Widget chính
│   │   │   └── ServiceDialog.h/cpp/ui      # Dialog thêm/sửa DV
│   │   │
│   │   ├── StaffManagement/                # Trang Quản lý nhân viên
│   │   │   ├── StaffWidget.h/cpp/ui        # Widget chính
│   │   │   └── StaffDialog.h/cpp/ui        # Dialog thêm/sửa NV
│   │   │
│   │   ├── Statistics/                     # Trang Thống kê
│   │   │   ├── StatisticsWidget.h/cpp/ui   # Widget chính
│   │   │   ├── RevenueReport.h/cpp         # Báo cáo doanh thu
│   │   │   └── CustomerReport.h/cpp        # Báo cáo khách hàng
│   │   │
│   │   └── Login/                          # Trang Đăng nhập
│   │       └── LoginDialog.h/cpp/ui        # Dialog đăng nhập
│   │
│   ├── Components/                         # Components tái sử dụng
│   │   ├── CustomTable.h/cpp               # Table tùy chỉnh
│   │   ├── SearchBox.h/cpp                 # Ô tìm kiếm
│   │   ├── DateTimePicker.h/cpp            # Chọn ngày giờ
│   │   ├── StatusBadge.h/cpp               # Badge trạng thái
│   │   ├── ConfirmDialog.h/cpp             # Dialog xác nhận
│   │   └── LoadingSpinner.h/cpp            # Loading animation
│   │
│   └── Resources/                          # Tài nguyên UI
│       ├── icons/                          # Icons (SVG, PNG)
│       │   ├── dashboard.svg
│       │   ├── booking.svg
│       │   ├── payment.svg
│       │   └── ...
│       ├── images/                         # Hình ảnh
│       │   ├── logo.png
│       │   └── background.jpg
│       ├── styles/                         # Styles
│       │   ├── app.qss                     # Main stylesheet (CSS-like)
│       │   ├── dark-theme.qss              # Dark theme
│       │   └── light-theme.qss             # Light theme
│       └── resources.qrc                   # Qt Resource file
│
├── Utils/                                  # Tiện ích chung
│   ├── Validator.h/cpp                     # Validate dữ liệu
│   ├── Logger.h/cpp                        # Ghi log
│   ├── Config.h/cpp                        # Cấu hình
│   └── Constants.h                         # Hằng số
│
├── Tests/                                  # Unit tests (optional)
│   ├── CoreTests/
│   └── UITests/
│
├── Data/                                   # ⭐ Dữ liệu runtime (file binary)
│   ├── system.dat                          # File lưu toàn bộ HeThongQuanLy
│   ├── config.ini                          # Cấu hình app
│   └── backup/                             # Folder backup
│       ├── system_backup_20251103.dat      # Backup theo ngày
│       └── system_backup_20251102.dat      # ...
│
│   # GHI CHÚ: Data/ chứa dữ liệu RUNTIME (được tạo khi chạy app)
│   # Không phải source code! Không commit vào Git!
│
├── Docs/                                   # Tài liệu
│   ├── ClassDiagram.png                    # Sơ đồ lớp
│   ├── UseCaseDiagram.png                  # Use case
│   └── UserManual.pdf                      # Hướng dẫn sử dụng
│
├── main.cpp                                # Entry point
├── FootballFieldManager.pro                # Qt Project file
├── CMakeLists.txt                          # CMake (optional)
└── README.md                               # File này

```

---

---

## 🎯 GIAI ĐOẠN PHÁT TRIỂN

#### **Tuần 1: Cấu trúc dữ liệu cơ bản** ✅ HOÀN THÀNH

- [x] Implement `ChuoiKyTu` (replace std::string) ✅
- [x] Implement `MangDong<T>` (replace std::vector) ✅
- [x] Implement `DanhSachLienKet<T>` (Linked List) ✅
- [x] Implement `Ngan<T>` (Stack) và `HangDoi<T>` (Queue) ✅
- [x] Implement `ThoiGian`, `NgayThang`, `NgayGio` ✅
- [x] **Test**: Viết test console cho từng class ✅

#### **Tuần 2: Models cơ bản** ✅ HOÀN THÀNH

- [x] Implement lớp cơ sở: `ConNguoi`, `NguoiDung`
- [x] Implement các lớp con: `QuanTriVien`, `NhanVien`, `KhachHang`
- [x] Implement `San`, `KhungGio`
- [x] Implement `DichVu` và các lớp con: `DoUong`, `ThietBi` (BaoHiem removed)
- [x] **Test**: Tạo objects, test methods

#### **Tuần 3: Models phức tạp & Quản lý + Thuật toán** ✅ HOÀN THÀNH

- [x] Implement `DatSan`, `DichVuDat` ✅
- [x] Implement `ThanhToan` và các lớp con ✅
- [x] Implement `HeThongQuanLy` (CRUD operations) ✅
- [x] Implement `ThongKe`, `ThongKeDoanhThu`, `ThongKeKhachHang` ✅
- [x] Implement `Core/ThuatToan/`: QuickSort, MergeSort, BST, HashTable ✅
- [x] Implement `FileManager` (ghiFile/docFile cho tất cả Models) ✅
- [x] **Test**: Test toàn bộ workflow + lưu/đọc file trong console ✅

---

### **GIAI ĐOẠN 2: BRIDGE LAYER (1 tuần)** ✅ HOÀN THÀNH

#### **Tuần 4: Adapters & ViewModels** ✅ HOÀN THÀNH

- [x] Implement `QtStringAdapter` (ChuoiKyTu ↔ QString) ✅
- [x] Implement `QtDateAdapter` (NgayGio ↔ QDateTime) ✅
- [x] Implement `QtTableAdapter` (MangDong ↔ QTableWidget) ✅
- [x] Implement các Adapters:
  - [x] `BaseAdapter` - Foundation với type conversions ✅
  - [x] `SanAdapter` - Field adapter ✅
  - [x] `KhachHangAdapter` - Customer adapter ✅
  - [x] `DatSanAdapter` - Booking adapter ✅
  - [x] `ThanhToanAdapter` - Payment adapter ✅
- [x] Implement các ViewModels:
  - [x] `BaseViewModel` - MVVM foundation ✅
  - [x] `MainViewModel` - Auth & Dashboard ✅
  - [x] `BookingViewModel` - Booking management ✅
  - [x] `CustomerViewModel` - Customer management ✅
  - [x] `StatisticsViewModel` - Analytics & reports ✅
  - [x] `FieldViewModel` - Field management (basic) ✅
  - [x] `ServiceViewModel` - Service management (placeholder) ✅
  - [x] `StaffViewModel` - Staff management (placeholder) ✅
- [x] **Test**: Test chuyển đổi dữ liệu ✅
  - [x] BridgeTest.cpp - 6 test suites passing 100% ✅

---

### **GIAI ĐOẠN 3: THIẾT KẾ UI CƠ BẢN (2 tuần)**

#### **Tuần 5: Main Window & Layout**

- [ ] Tạo Qt Project, cấu hình .pro file
- [ ] Thiết kế `MainWindow.ui` trong Qt Designer
- [ ] Implement `MainWindow.h/cpp`
- [ ] Implement `Header.h/cpp/ui` (search bar, user profile)
- [ ] Implement `Sidebar.h/cpp/ui` (menu navigation)
- [ ] Implement `LoginDialog.h/cpp/ui`
- [ ] **Test**: Chạy app, test navigation

#### **Tuần 6: Dashboard & Components**

- [ ] Thiết kế `DashboardWidget.ui`
- [ ] Implement `StatCard` component
- [ ] Tích hợp QtCharts: Line, Bar, Pie charts
- [ ] Implement `CustomTable` component
- [ ] Implement `SearchBox`, `StatusBadge`
- [ ] Load stylesheet `app.qss`
- [ ] **Test**: Hiển thị dữ liệu mock

---

### **GIAI ĐOẠN 4: CÁC TRANG CHỨC NĂNG (3-4 tuần)**

#### **Tuần 7: Quản lý Sân & Khách hàng**

- [ ] Implement `FieldWidget` + `FieldDialog`
  - [ ] CRUD sân bóng
  - [ ] Hiển thị table, filter
- [ ] Implement `CustomerWidget` + `CustomerDialog`
  - [ ] CRUD khách hàng
  - [ ] Tìm kiếm, xem lịch sử
- [ ] **Test**: Test CRUD operations

#### **Tuần 8: Đặt sân**

- [ ] Implement `BookingWidget`
- [ ] Implement `TimelineView` (lịch timeline giống web)
- [ ] Implement `BookingDialog` (form đặt sân + dịch vụ)
- [ ] Implement `HistoryDialog` (lịch sử đặt)
- [ ] Tính năng hủy đặt sân
- [ ] **Test**: Test workflow đặt sân hoàn chỉnh

#### **Tuần 9: Thanh toán & Dịch vụ**

- [ ] Implement `PaymentWidget`
- [ ] Implement `PaymentDialog` (chọn phương thức TT)
- [ ] Implement `InvoiceDialog` (hiển thị + in hóa đơn)
- [ ] Implement `ServiceWidget` + `ServiceDialog`
  - [ ] CRUD dịch vụ
  - [ ] Quản lý tồn kho
- [ ] **Test**: Test thanh toán với các phương thức

#### **Tuần 10: Nhân viên & Thống kê**

- [ ] Implement `StaffWidget` + `StaffDialog`
  - [ ] CRUD nhân viên
  - [ ] Phân quyền
- [ ] Implement `StatisticsWidget`
  - [ ] Biểu đồ doanh thu theo ngày/tuần/tháng
  - [ ] Top khách hàng
  - [ ] Xuất báo cáo
- [ ] **Test**: Test quyền admin/staff

---

### **GIAI ĐOẠN 5: LƯU TRỮ HƯỚNG ĐỐI TƯỢNG (1 tuần)**

#### **Tuần 11: Serialization**

- [ ] Implement `ghiFile()` cho tất cả Models
  - [ ] ConNguoi, KhachHang, NhanVien
  - [ ] San, DichVu
  - [ ] DatSan, ThanhToan
- [ ] Implement `docFile()` cho tất cả Models
- [ ] Implement `HeThongQuanLy::luuHeThong()`
- [ ] Implement `HeThongQuanLy::docHeThong()`
- [ ] Implement auto-save timer trong MainWindow
- [ ] Implement backup/restore (copy file)
- [ ] **Test**: Test lưu/đọc dữ liệu, restart app

---

### **GIAI ĐOẠN 6: HOÀN THIỆN & TỐI ƯU (2 tuần)**

#### **Tuần 12: Polish UI**

- [ ] Hoàn thiện stylesheet (dark/light theme)
- [ ] Thêm animations, transitions
- [ ] Responsive layout
- [ ] Icons, images đẹp
- [ ] Loading spinners
- [ ] Toast notifications
- [ ] **Test**: Test UI/UX

#### **Tuần 13: Testing & Bug Fixes**

- [ ] Unit tests cho Core
- [ ] Integration tests
- [ ] Stress test (nhiều dữ liệu)
- [ ] Fix bugs
- [ ] Optimize performance
- [ ] **Test**: Full regression test

---

### **GIAI ĐOẠN 7: TÀI LIỆU & TRIỂN KHAI (1 tuần)**

#### **Tuần 14: Documentation & Deployment**

- [ ] Viết tài liệu code (comments, Doxygen)
- [ ] Tạo User Manual
- [ ] Vẽ Class Diagram, Use Case Diagram
- [ ] Build release version
- [ ] Tạo installer (Qt Installer Framework)
- [ ] **Demo**: Chuẩn bị presentation

---

## 🏗️ KIẾN TRÚC PHÂN TẦNG

```
┌─────────────────────────────────────────────────────────┐
│                   UI LAYER (Qt)                         │
│         MainWindow, Dialogs, Widgets                    │
│  - Hiển thị dữ liệu (QTableWidget, QChart, QLabel)     │
│  - Nhận input từ user (QLineEdit, QComboBox, QPushButton)│
│  - Chỉ gọi ViewModel, KHÔNG gọi Core trực tiếp         │
└─────────────────────┬───────────────────────────────────┘
                      │ Qt Signals/Slots
                      │ connect(viewModel, signal, this, slot)
┌─────────────────────▼───────────────────────────────────┐
│               BRIDGE LAYER                              │
│                                                         │
│  ┌─────────────────────────────────────────────────┐   │
│  │  ADAPTERS (Chuyển đổi kiểu dữ liệu)            │   │
│  │  - QtStringAdapter:  ChuoiKyTu ↔ QString       │   │
│  │  - QtDateAdapter:    NgayGio ↔ QDateTime       │   │
│  │  - QtTableAdapter:   MangDong ↔ QTableWidget   │   │
│  └─────────────────────────────────────────────────┘   │
│                          │                              │
│  ┌─────────────────────▼─────────────────────────┐     │
│  │  VIEWMODELS (Logic điều khiển UI)            │     │
│  │  - Nhận input từ UI (QString, QDateTime)     │     │
│  │  - Dùng Adapter chuyển đổi                   │     │
│  │  - Gọi Core xử lý logic                      │     │
│  │  - Emit signals về UI (thành công/thất bại)  │     │
│  └───────────────────────────────────────────────┘     │
└─────────────────────┬───────────────────────────────────┘
                      │ Pure C++ function calls
                      │ heThong->taoDatSan(kh, san, gio)
┌─────────────────────▼───────────────────────────────────┐
│              CORE LAYER (C++ thuần OOP)                 │
│         Models + Managers + Algorithms                  │
│                                                         │
│  - KhachHang, San, DatSan, ThanhToan...                │
│  - HeThongQuanLy (CRUD, tìm kiếm, xử lý)               │
│  - ChuoiKyTu, MangDong, NgayGio (tự implement)         │
│  - Logic nghiệp vụ 100% C++ thuần                      │
│  - KHÔNG biết gì về Qt (QString, QWidget...)           │
└─────────────────────┬───────────────────────────────────┘
                      │ File I/O (Binary)
                      │ ghiFile() / docFile()
┌─────────────────────▼───────────────────────────────────┐
│              FILE STORAGE (OOP Serialization)           │
│                                                         │
│  Data/system.dat  (Binary file)                        │
│  - Mỗi object tự serialize/deserialize                 │
│  - HeThongQuanLy lưu tất cả objects                    │
│  - Auto-save mỗi 5 phút                                │
│  - Backup: copy file sang backup/                     │
└─────────────────────────────────────────────────────────┘
```

### **Luồng dữ liệu:**

```
USER clicks button
    ↓
UI Layer: on_btnXacNhan_clicked()
    ↓ (gọi ViewModel)
Bridge: viewModel->taoDatSan(QString, QDateTime)
    ↓ (Adapter convert)
Bridge: ChuoiKyTu, NgayGio
    ↓ (gọi Core)
Core: heThong->taoDatSan(KhachHang*, San*, NgayGio)
    ↓ (xử lý logic)
Core: return DatSan*
    ↓ (emit signal)
Bridge: emit datSanThanhCong(QString maDatSan)
    ↓ (UI nhận signal)
UI Layer: onDatSanThanhCong(QString maDatSan)
    ↓
UI hiển thị: QMessageBox::information("Thành công!")
```

---

## 🎨 MAPPING WEB → QT

| **Web Component (React)** | **Qt Widget**                | **Ghi chú**  |
| ------------------------- | ---------------------------- | ------------ |
| `<div>`                   | `QWidget`, `QFrame`          | Container    |
| `<button>`                | `QPushButton`                | Button       |
| `<input>`                 | `QLineEdit`                  | Text input   |
| `<select>`                | `QComboBox`                  | Dropdown     |
| `<table>`                 | `QTableWidget`               | Table        |
| `<form>`                  | `QDialog`                    | Form dialog  |
| CSS Grid/Flex             | `QGridLayout`, `QHBoxLayout` | Layout       |
| Recharts                  | `QtCharts`                   | Charts       |
| Modal                     | `QDialog`                    | Popup        |
| Sidebar                   | `QListWidget`                | Menu         |
| Header                    | Custom `QWidget`             | Top bar      |
| React State               | Class members                | Data storage |
| useEffect()               | Slots connected to signals   | Side effects |
| onClick                   | `clicked()` signal           | Event        |
| CSS                       | QSS (Qt StyleSheet)          | Styling      |

---

## 📝 FILE CẤU HÌNH

### **FootballFieldManager.pro**

```qmake
QT       += core gui widgets charts sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = FootballFieldManager
TEMPLATE = app

# Core
INCLUDEPATH += Core
SOURCES += Core/CauTrucDuLieu/*.cpp \
           Core/Models/*.cpp \
           Core/QuanLy/*.cpp \
           Core/ThongKe/*.cpp \
           Core/ThuatToan/*.cpp

HEADERS += Core/CauTrucDuLieu/*.h \
           Core/Models/*.h \
           Core/QuanLy/*.h \
           Core/ThongKe/*.h \
           Core/ThuatToan/*.h

# Bridge
INCLUDEPATH += Bridge
SOURCES += Bridge/Adapters/*.cpp \
           Bridge/ViewModels/*.cpp

HEADERS += Bridge/Adapters/*.h \
           Bridge/ViewModels/*.h

# UI
INCLUDEPATH += UI
SOURCES += UI/MainWindow/*.cpp \
           UI/Pages/*/*.cpp \
           UI/Components/*.cpp

HEADERS += UI/MainWindow/*.h \
           UI/Pages/*/*.h \
           UI/Components/*.h

FORMS += UI/MainWindow/*.ui \
         UI/Pages/*/*.ui

RESOURCES += UI/Resources/resources.qrc

# Main
SOURCES += main.cpp

# Utils
SOURCES += Utils/*.cpp
HEADERS += Utils/*.h
```

---

## 🚀 HƯỚNG DẪN CHẠY DỰ ÁN

### **1. Cài đặt môi trường**

```bash
# Cài Qt (tải từ qt.io)
# Khuyến nghị: Qt 6.x + Qt Creator

# Windows: Qt Online Installer
# Chọn components: Qt 6.x, Qt Charts, Qt Creator, MinGW/MSVC
```

### **2. Clone/Tạo project**

```bash
# Tạo project mới trong Qt Creator
File → New Project → Application → Qt Widgets Application

# Hoặc dùng qmake
qmake FootballFieldManager.pro
make
```

### **3. Build & Run**

```bash
# Trong Qt Creator: Ctrl + R (Run)

# Hoặc command line:
qmake
make
./FootballFieldManager
```

---

## 📚 TÀI LIỆU THAM KHẢO

1. **Qt Documentation**: https://doc.qt.io/
2. **Qt Widgets**: https://doc.qt.io/qt-6/qtwidgets-index.html
3. **Qt Charts**: https://doc.qt.io/qt-6/qtcharts-index.html
4. **QSS Reference**: https://doc.qt.io/qt-6/stylesheet-reference.html
5. **Signal & Slot**: https://doc.qt.io/qt-6/signalsandslots.html

---

## ✅ CHECKLIST HOÀN THÀNH

### **Core (C++ thuần)**

- [ ] Cấu trúc dữ liệu tự implement (ChuoiKyTu, MangDong, DanhSachLienKet)
- [ ] Models đầy đủ (KhachHang, San, DatSan, ThanhToan...)
- [ ] Hệ thống quản lý hoạt động (HeThongQuanLy CRUD)
- [ ] Thống kê chính xác (ThongKeDoanhThu, ThongKeKhachHang)
- [ ] Test console thành công (main.cpp test tất cả)

### **Bridge - LỚP QUAN TRỌNG!**

- [ ] Adapters chuyển đổi kiểu dữ liệu (ChuoiKyTu ↔ QString, NgayGio ↔ QDateTime)
- [ ] ViewModels xử lý logic UI (logic không nằm trong UI code)
- [ ] Signal/Slot hoạt động (emit thành công/thất bại)
- [ ] UI không trực tiếp gọi Core (tất cả qua ViewModel)

### **UI (Qt Widgets)**

- [ ] Main window đẹp (Header + Sidebar + StackedWidget)
- [ ] Tất cả pages hoạt động (Dashboard, Booking, Payment...)
- [ ] CRUD đầy đủ (Thêm/Sửa/Xóa/Tìm)
- [ ] Charts hiển thị đúng (QtCharts: Line, Bar, Pie)
- [ ] Responsive, smooth (Animation, QSS styling)

### **File Storage (OOP Serialization)**

- [ ] Mỗi class có ghiFile()/docFile()
- [ ] HeThongQuanLy::luuHeThong() hoạt động
- [ ] HeThongQuanLy::docHeThong() hoạt động
- [ ] Auto-save mỗi 5 phút
- [ ] Backup/restore (copy file system.dat)

### **Hoàn thiện**

- [ ] Không bugs nghiêm trọng
- [ ] Performance tốt
- [ ] UI/UX đẹp, dễ dùng
- [ ] Tài liệu đầy đủ

---

## 🎯 MỤC TIÊU CUỐI CÙNG

✅ **Phần mềm desktop hoàn chỉnh**  
✅ **Giao diện đẹp giống web**  
✅ **Code OOP chuẩn C++**  
✅ **Không dùng STL (tự implement)**  
✅ **Kiến trúc MVVM rõ ràng**  
✅ **Có thể mở rộng, bảo trì dễ**  
✅ **Phù hợp làm đồ án tốt nghiệp**

---

## 👨‍💻 NOTES CHO DEVELOPER

### **Tips phát triển:**

1. **Bắt đầu từ Core**: Test kỹ Core trước khi làm UI
2. **Qt Designer**: Dùng Designer để thiết kế UI nhanh
3. **Signal/Slot**: Hiểu rõ cơ chế này, rất quan trọng
4. **QSS**: Tương tự CSS, dễ học
5. **Debug**: Dùng `qDebug()` thay `cout`
6. **Memory**: Nhớ `delete` objects (hoặc dùng smart pointers)

### **Common Issues:**

- **Qt not found**: Kiểm tra PATH environment
- **MOC errors**: Clean build, run qmake lại
- **Linking errors**: Kiểm tra .pro file
- **Crash**: Check null pointers, array bounds

---

## 📞 SUPPORT

**Email**: [your-email]  
**GitHub**: [your-repo]  
**Documentation**: Xem folder `Docs/`

---

**Chúc bạn code thành công! ⚽🚀**
