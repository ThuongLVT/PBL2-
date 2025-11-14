# Football Field Manager - Qt6 GUI Build Guide

## Hướng dẫn Build và Chạy Ứng Dụng Qt6 Widgets

### Yêu Cầu Hệ Thống

- **Qt 6.9.3** (hoặc cao hơn)
- **CMake 3.16+**
- **MinGW** hoặc **MSVC** compiler
- **Git** (optional)

### Cấu Trúc Dự Án

```
FootballFieldManager/
├── Core/               # Business Logic Layer
│   ├── CauTrucDuLieu/  # Data Structures
│   ├── Models/         # Domain Models
│   ├── QuanLy/         # Managers
│   ├── ThongKe/        # Statistics
│   └── ThuatToan/      # Algorithms
├── UI/                 # Presentation Layer (Qt6 Widgets)
│   ├── MainWindow.h/.cpp
│   ├── Widgets/        # Custom Widgets
│   │   ├── SidebarButton
│   │   ├── StatsCard
│   │   └── CustomTable
│   ├── Pages/          # Application Pages
│   │   ├── DashboardPage
│   │   ├── BookingPage
│   │   ├── PaymentPage
│   │   ├── FieldManagementPage
│   │   ├── CustomerPage
│   │   ├── ServicePage
│   │   ├── StaffPage
│   │   └── StatisticsPage
│   └── Resources/      # Resources (icons, styles)
│       ├── icons/      # SVG icons
│       ├── styles/     # QSS stylesheets
│       └── resources.qrc
├── Data/               # Application Data
├── CMakeLists.txt
└── main.cpp
```

### Bước 1: Cài Đặt Qt6

#### Windows (Qt Installer):

1. Tải Qt Online Installer từ: https://www.qt.io/download-qt-installer
2. Chọn cài đặt:
   - Qt 6.9.3
   - MinGW 11.2.0 64-bit (hoặc MSVC 2022)
   - Qt Charts
   - CMake
   - Ninja

#### Linux (Ubuntu/Debian):

```bash
sudo apt install qt6-base-dev qt6-charts-dev cmake build-essential
```

### Bước 2: Thiết Lập Môi Trường

#### Windows:

Thêm Qt vào PATH hoặc sử dụng Qt Command Prompt.

```cmd
set PATH=C:\Qt\6.9.3\mingw_64\bin;%PATH%
set CMAKE_PREFIX_PATH=C:\Qt\6.9.3\mingw_64
```

#### Linux:

```bash
export Qt6_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt6
```

### Bước 3: Build Dự Án

#### Sử Dụng VS Code Tasks (Khuyến nghị):

```bash
# Mở VS Code
code .

# Chạy task build
Ctrl+Shift+B
# Chọn: "CMake: Build"
```

#### Hoặc Sử Dụng Command Line:

```cmd
# Windows (MinGW)
cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release -- -j4

# Windows (MSVC)
cmake -B build -S . -G "Visual Studio 17 2022"
cmake --build build --config Release

# Linux
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

### Bước 4: Chạy Ứng Dụng

#### Từ VS Code:

```bash
# Chạy task
Ctrl+Shift+P → "Tasks: Run Task" → "Run Application"
```

#### Hoặc Terminal:

```cmd
# Windows
build\bin\FootballFieldManager.exe

# Linux
./build/bin/FootballFieldManager
```

### Giao Diện Ứng Dụng

Khi chạy, bạn sẽ thấy:

1. **Sidebar xanh lá** bên trái với các menu:

   - 🏠 Trang chủ
   - 📅 Đặt sân
   - 💳 Thanh toán
   - ⚽ Quản lý sân
   - 👥 Khách hàng
   - 🛍️ Dịch vụ
   - 👨‍💼 Nhân viên
   - 📊 Thống kê

2. **Dashboard (Trang chủ)**:

   - 4 thẻ thống kê (Doanh thu, Đơn đặt, Khách hàng, Sân)
   - Biểu đồ doanh thu 7 ngày
   - Bảng đơn đặt sân gần đây

3. **Các trang quản lý**:
   - Hiển thị dữ liệu trong bảng đẹp mắt
   - Có chức năng tìm kiếm, lọc
   - Nút thêm/sửa/xóa

### Tính Năng

- ✅ **Tích hợp hoàn chỉnh** với Core Layer
- ✅ **Giao diện hiện đại** với Material Design
- ✅ **Responsive** - tự động điều chỉnh kích thước
- ✅ **Dữ liệu mẫu** tự động tạo khi chạy lần đầu
- ✅ **Charts** cho thống kê doanh thu
- ✅ **Custom Widgets** tái sử dụng được

### Mở Rộng

#### Thêm Trang Mới:

1. Tạo file trong `UI/Pages/`:

   ```cpp
   // MyNewPage.h
   class MyNewPage : public QWidget {
       Q_OBJECT
   public:
       explicit MyNewPage(HeThongQuanLy *heThong, QWidget *parent = nullptr);
   private:
       void setupUI();
   };
   ```

2. Thêm vào `MainWindow::setupPages()`:

   ```cpp
   m_stackedWidget->addWidget(new MyNewPage(m_heThong, this));
   ```

3. Thêm nút trong `MainWindow::setupSidebar()`

#### Tùy Chỉnh Theme:

Chỉnh sửa `UI/Resources/styles/main.qss` để thay đổi màu sắc, font chữ.

### Troubleshooting

#### Lỗi "Qt6 not found":

```cmd
set CMAKE_PREFIX_PATH=C:\Qt\6.9.3\mingw_64
```

#### Lỗi "Charts not found":

Kiểm tra Qt Charts đã được cài đặt:

```bash
qmake -query QT_INSTALL_LIBS
# Phải có Qt6Charts.dll/so
```

#### Lỗi Build:

```bash
# Clean và rebuild
rm -rf build
cmake -B build -S .
cmake --build build
```

### Các Task VS Code Có Sẵn

- **CMake: Configure** - Cấu hình CMake
- **CMake: Build** - Build toàn bộ dự án
- **CMake: Clean** - Xóa build artifacts
- **CMake: Rebuild** - Clean + Build
- **Run Application** - Chạy ứng dụng

### Performance Tips

- Build mode Release để tối ưu hiệu năng
- Sử dụng `-j` flag cho parallel build
- Cache CMake configuration

### Hỗ Trợ

Nếu gặp vấn đề, kiểm tra:

1. Qt version đúng (6.9.3)
2. CMAKE_PREFIX_PATH đã được set
3. Compiler compatibility (MinGW 11.2+)

---

**Tác giả**: Football Field Management System  
**Ngày**: 2025-11-06  
**Version**: 1.0.0
