# Football Field Manager - Qt6 Widgets UI

## 🎨 Giao Diện Hiện Đại & Dễ Sử Dụng

Dự án này là ứng dụng quản lý sân bóng đá với giao diện Qt6 Widgets hiện đại, đẹp mắt và dễ sử dụng.

![Dashboard](https://via.placeholder.com/800x450/2ecc71/ffffff?text=Dashboard+Preview)

## ✨ Tính Năng Chính

### 🏠 Dashboard (Trang Chủ)

- **Thống kê tổng quan** với 4 thẻ đẹp mắt:
  - 💰 Tổng doanh thu
  - 📅 Đơn đặt sân
  - 👥 Khách hàng
  - ⚽ Sân hoạt động
- **Biểu đồ doanh thu** 7 ngày gần đây (Qt Charts)
- **Bảng đặt sân gần đây** với trạng thái màu sắc

### 📅 Quản Lý Đặt Sân

- Xem tất cả đơn đặt sân
- Tạo đơn mới
- Hủy đơn
- Lọc theo trạng thái, ngày
- Timeline đặt sân theo giờ

### 💳 Quản Lý Thanh Toán

- Danh sách thanh toán
- Phương thức: Tiền mặt, Thẻ, Chuyển khoản
- Trạng thái thanh toán

### ⚽ Quản Lý Sân

- Danh sách sân (5, 7 người)
- Giá thuê
- Trạng thái (Trống, Đã đặt, Bảo trì)
- Thêm/Sửa/Xóa sân

### 👥 Quản Lý Khách Hàng

- Thông tin khách hàng
- Điểm tích lũy & Hạng thành viên
- Lịch sử đặt sân
- Thêm/Sửa/Xóa khách hàng

### 🛍️ Quản Lý Dịch Vụ

- Dịch vụ đồ uống, thiết bị, bảo hiểm
- Tồn kho
- Giá dịch vụ

### 👨‍💼 Quản Lý Nhân Viên

- Thông tin nhân viên
- Lương, ca làm việc
- Thêm/Sửa/Xóa nhân viên

### 📊 Thống Kê & Báo Cáo

- Doanh thu theo thời gian
- Top khách hàng
- Sân được đặt nhiều nhất
- Xuất báo cáo

## 🎨 Thiết Kế UI

### Màu Sắc Chính

- **Primary**: `#2ecc71` (Xanh lá - Fresh & Sport)
- **Secondary**: `#27ae60` (Xanh đậm)
- **Background**: `#f5f6fa` (Xám nhạt)
- **Text**: `#2c3e50` (Xám đậm)
- **Accent**:
  - Success: `#27ae60`
  - Warning: `#f39c12`
  - Danger: `#e74c3c`
  - Info: `#3498db`

### Components

#### 1. **SidebarButton**

```cpp
SidebarButton(":/icons/home.svg", "Trang chủ", this);
```

- Icon + Text
- Hover effect
- Active state (trắng với chữ xanh)

#### 2. **StatsCard**

```cpp
StatsCard("Tổng doanh thu", "12,500,000 đ",
          ":/icons/money.svg", QColor("#3498db"), this);
```

- Icon với background màu
- Title, Value, Trend
- Shadow & hover effect

#### 3. **CustomTable**

```cpp
CustomTable table;
table.setupHeaders({"Mã", "Tên", "Trạng thái"});
table.addDataRow({"001", "Nguyễn Văn A", "Hoạt động"});
```

- Header xanh lá
- Alternating rows
- Sortable
- Hover highlight

## 🏗️ Kiến Trúc

```
┌─────────────────────────────────────┐
│         main.cpp (Entry)            │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│        MainWindow                    │
│  ┌───────────┐  ┌─────────────────┐ │
│  │  Sidebar  │  │  QStackedWidget │ │
│  │  (Green)  │  │                 │ │
│  │           │  │  ┌──────────┐   │ │
│  │ • Home    │  │  │Dashboard │   │ │
│  │ • Booking │  │  │   Page   │   │ │
│  │ • Payment │  │  └──────────┘   │ │
│  │ • Fields  │  │                 │ │
│  │ • ...     │  │  (8 pages)      │ │
│  └───────────┘  └─────────────────┘ │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│      HeThongQuanLy (Core)           │
│  • QuanLySan                        │
│  • QuanLyDatSan                     │
│  • QuanLyKhachHang                  │
│  • QuanLyThanhToan                  │
│  • QuanLyDichVu                     │
│  • FileManager, ThongKe, etc.       │
└─────────────────────────────────────┘
```

## 📂 Cấu Trúc Files

```
UI/
├── MainWindow.h/cpp         # Cửa sổ chính
├── Widgets/                 # Custom widgets
│   ├── SidebarButton.h/.cpp
│   ├── StatsCard.h/.cpp
│   └── CustomTable.h/.cpp
├── Pages/                   # Các trang
│   ├── DashboardPage.h/.cpp
│   ├── BookingPage.h/.cpp
│   ├── PaymentPage.h/.cpp
│   ├── FieldManagementPage.h/.cpp
│   ├── CustomerPage.h/.cpp
│   ├── ServicePage.h/.cpp
│   ├── StaffPage.h/.cpp
│   └── StatisticsPage.h/.cpp
└── Resources/
    ├── icons/               # SVG icons
    ├── styles/              # QSS files
    └── resources.qrc        # Qt Resource file
```

## 🚀 Build & Run

```bash
# Configure
cmake -B build -S . -G "MinGW Makefiles"

# Build
cmake --build build

# Run
build/bin/FootballFieldManager.exe
```

Chi tiết xem: [QT6_GUI_BUILD_GUIDE.md](QT6_GUI_BUILD_GUIDE.md)

## 📸 Screenshots

### Dashboard

- Thẻ thống kê màu sắc
- Biểu đồ line chart
- Bảng đơn gần đây

### Booking Page

- Form đặt sân
- Timeline khung giờ
- Bảng đơn đặt với filter

### Field Management

- Grid view các sân
- Trạng thái sân (màu sắc)
- Form thêm/sửa sân

## 🔌 Tích Hợp Core

UI kết nối trực tiếp với `HeThongQuanLy` singleton:

```cpp
// Trong MainWindow constructor
m_heThong = HeThongQuanLy::getInstance();

// Trong DashboardPage
void DashboardPage::loadStatistics() {
    int totalCustomers = m_heThong->tongSoKhachHang();
    int totalBookings = m_heThong->tongSoDatSan();

    const auto &payments = m_heThong->layDanhSachThanhToan();
    // ... update UI
}
```

Tất cả logic nghiệp vụ đều ở `Core/`, UI chỉ hiển thị!

## 🎯 Mở Rộng

### Thêm Page Mới

1. **Tạo Page Class**:

```cpp
// UI/Pages/MyNewPage.h
class MyNewPage : public QWidget {
    Q_OBJECT
public:
    explicit MyNewPage(HeThongQuanLy *heThong, QWidget *parent = nullptr);
    void refreshData();
private:
    HeThongQuanLy *m_heThong;
    void setupUI();
};
```

2. **Add vào MainWindow**:

```cpp
// MainWindow::setupPages()
m_stackedWidget->addWidget(new MyNewPage(m_heThong, this));
```

3. **Thêm menu button**:

```cpp
// MainWindow::setupSidebar()
SidebarButton *btn = new SidebarButton("", "🎯 My Page", m_sidebar);
m_menuButtons.append(btn);
```

### Custom Widget Mới

```cpp
class MyWidget : public QWidget {
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    void setupUI();
};
```

## 🎨 Styling

Chỉnh sửa `UI/Resources/styles/main.qss`:

```qss
QPushButton {
    background-color: #2ecc71;
    color: white;
    border-radius: 8px;
    padding: 10px 20px;
}

QPushButton:hover {
    background-color: #27ae60;
}
```

## 📊 Data Flow

```
User Action (UI)
      ↓
Button Click Handler
      ↓
Call HeThongQuanLy method
      ↓
Core logic processes
      ↓
Return result
      ↓
Update UI
      ↓
refreshData() / QTableWidget update
```

## 🛠️ Tools & Libraries

- **Qt 6.9.3** - Framework chính
- **Qt Widgets** - UI components
- **Qt Charts** - Biểu đồ
- **CMake** - Build system
- **SVG Icons** - Icons đẹp scalable

## 📝 TODO / Future

- [ ] Login dialog
- [ ] User roles & permissions
- [ ] Real-time updates
- [ ] Print invoice
- [ ] Export Excel/PDF
- [ ] Multi-language support
- [ ] Dark mode theme
- [ ] Notification system

## 🤝 Contributing

Để thêm tính năng mới:

1. Tạo Widget/Page trong `UI/`
2. Kết nối với `Core/` qua `HeThongQuanLy`
3. Test thoroughly
4. Update docs

## 📄 License

MIT License - Football Field Management System

---

**Version**: 1.0.0  
**Date**: 2025-11-06  
**Author**: Football Field Management Team
