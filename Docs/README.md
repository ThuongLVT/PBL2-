# ⚽ HỆ THỐNG QUẢN LÝ SÂN BÓNG ĐÁ - QT C++ OOP

## 📋 TỔNG QUAN DỰ ÁN

Phần mềm quản lý sân bóng đá với:

- **Core Logic**: C++ thuần túy với OOP (không dùng STL)
- **UI Framework**: Qt Widgets
- **Kiến trúc**: Model-View-ViewModel (MVVM)
- **Database**: File-based serialization

---

## 🗂️ CẤU TRÚC DỰ ÁN

```
FootballFieldManager/
├── Core/                      # Logic nghiệp vụ (C++ thuần)
│   ├── CauTrucDuLieu/        # Tự implement cấu trúc dữ liệu
│   ├── Models/               # Các lớp đối tượng nghiệp vụ
│   ├── QuanLy/               # Các lớp quản lý (CRUD)
│   ├── ThongKe/              # Thống kê báo cáo
│   └── ThuatToan/            # Thuật toán (sort, search, BST...)
├── Bridge/                    # Cầu nối Core ↔ UI
│   ├── Adapters/             # Chuyển đổi kiểu dữ liệu
│   └── ViewModels/           # Logic điều khiển UI
├── UI/                       # Giao diện Qt Widgets
│   ├── MainWindow/           # Cửa sổ chính
│   ├── Pages/                # Các trang chức năng
│   ├── Components/           # Components tái sử dụng
│   └── Resources/            # Icons, images, styles
├── Utils/                    # Tiện ích chung
├── Tests/                    # Unit tests
├── Data/                     # Dữ liệu runtime (binary files)
└── Docs/                     # Tài liệu

```

---

## 🚀 HƯỚNG DẪN BUILD & RUN

### **1. Yêu cầu hệ thống**

- **Qt**: 6.x hoặc 5.15+
- **CMake**: 3.16 trở lên
- **Compiler**: GCC/MinGW (C++17 trở lên) hoặc MSVC
- **IDE**: VS Code (với CMake Tools) hoặc Qt Creator

### **2. Build với CMake (VS Code)**

```bash
# Cấu hình CMake
cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug

# Build project
cmake --build build --config Debug

# Run
build\bin\FootballFieldManager.exe
```

**Hoặc sử dụng VS Code Tasks:**

- `Ctrl+Shift+B` → Chọn "CMake: Build"
- `Ctrl+Shift+P` → "Tasks: Run Task" → "Run Application"

### **3. Build với Qt Creator**

```bash
# Mở Qt Creator
File → Open File or Project → chọn CMakeLists.txt

# Cấu hình kit (MinGW hoặc MSVC)
Configure Project

# Build & Run
Ctrl + R (hoặc nút Run màu xanh)
```

### **4. Build với qmake (legacy)**

```bash
# Mở Qt Creator
File → Open File or Project → chọn FootballFieldManager.pro

# Build & Run
Ctrl + R
```

---

## 📅 TIẾN ĐỘ PHÁT TRIỂN

### ✅ **TUẦN 1: Cấu trúc dữ liệu cơ bản** (HOÀN THÀNH)

- [x] ChuoiKyTu (replace std::string)
- [x] MangDong<T> (replace std::vector)
- [x] DanhSachLienKet<T> (Linked List)
- [x] Ngan<T> (Stack) và HangDoi<T> (Queue)
- [x] ThoiGian, NgayThang, NgayGio
- [x] Test console thành công

### ✅ **TUẦN 2: Models cơ bản** (HOÀN THÀNH)

- [x] ConNguoi, NguoiDung (lớp cơ sở)
- [x] QuanTriVien, NhanVien, KhachHang
- [x] San, KhungGio
- [x] DichVu (lớp cơ sở)
- [x] Test kế thừa & đa hình thành công

### ✅ **TUẦN 3: Hệ thống hoàn chỉnh** (HOÀN THÀNH)

- [x] DatSan, DichVuDat (Booking entities)
- [x] ThanhToan (Abstract), ThanhToanTienMat, ThanhToanThe, ThanhToanChuyenKhoan
- [x] HeThongQuanLy (Singleton), FileManager, BackupManager
- [x] QuanLySan, QuanLyKhachHang, QuanLyDichVu, QuanLyDatSan, QuanLyThanhToan
- [x] ThongKeDoanhThu, ThongKeKhachHang
- [x] QuickSort, BinarySearchTree, HashTable
- [x] Test toàn bộ hệ thống (Week 1-3) thành công ✅

### 🔜 **TUẦN 4: Bridge Layer** (ĐANG CHUẨN BỊ)

### 📋 **Các tuần tiếp theo**

- **Tuần 4**: Bridge Layer (Adapters, ViewModels)
- **Tuần 5-6**: UI cơ bản (MainWindow, Dashboard)
- **Tuần 7-10**: Các trang chức năng (Booking, Payment...)
- **Tuần 11**: File Storage (OOP Serialization - Refactor)
- **Tuần 12-14**: Hoàn thiện, test, tài liệu

**Progress: 37.5% (3/8 major phases complete)**

---

## 💻 TEST HIỆN TẠI

Chạy `main.cpp` để test hệ thống Week 1-3:

```bash
# Build & Run
cd FootballFieldManager/build
cmake --build . -j 4
./bin/FootballFieldManager.exe

# Output mẫu:
========================================
   HE THONG QUAN LY SAN BONG DA
   TEST TUAN 1, 2 & 3
========================================

######################################
   TEST TUAN 1 - CAU TRUC DU LIEU
######################################
[All data structure tests pass...]

######################################
   TUAN 2 - MODELS CO BAN
######################################
[All model tests pass...]

######################################
   TEST TUAN 3 - HE THONG HOAN CHINH
######################################
[Booking, Payment, Statistics, File I/O tests pass...]

========================================
   TAT CA TEST HOAN THANH!
   Tuan 1, 2 & 3: SUCCESS!
========================================
```

---

## 📚 TÀI LIỆU THAM KHẢO

- **Kế hoạch chi tiết**: `QT_PROJECT_STRUCTURE.md`
- **Build instructions**: `BUILD.md`
- **Quick start**: `QUICKSTART.md`
- **Week 1 Summary**: `WEEK1_SUMMARY.md`
- **Week 2 Summary**: `WEEK2_SUMMARY.md`
- **Week 3 Summary**: `WEEK3_SUMMARY.md` ✨ NEW!
- **Project Status**: `STATUS.md` (Updated!)
- **Qt Documentation**: https://doc.qt.io/

---

## 👨‍💻 DEVELOPER NOTES

### **Nguyên tắc phát triển:**

1. **Core trước, UI sau**: Hoàn thiện logic C++ trước khi làm UI ✅
2. **Không dùng STL**: Tự implement tất cả cấu trúc dữ liệu ✅
3. **OOP chuẩn**: Kế thừa, đa hình, đóng gói đầy đủ ✅
4. **Design Patterns**: Singleton, Factory, Manager patterns ✅
5. **MVVM**: UI không gọi trực tiếp Core, phải qua ViewModel (Coming Week 4)
6. **File I/O**: Mỗi class có `ghiFile()` và `docFile()` ✅

### **Convention:**

- **Tên class**: PascalCase (VD: `ChuoiKyTu`, `KhachHang`)
- **Tên biến**: camelCase (VD: `doDai`, `kichThuoc`)
- **Tên file**: giống tên class (VD: `ChuoiKyTu.h`, `ChuoiKyTu.cpp`)
- **Comments**: Tiếng Việt (cho dễ hiểu), Doxygen format

### **Achievements so far:**

- ✅ 30+ Classes implemented
- ✅ ~6,000+ lines of code
- ✅ Custom data structures (No STL)
- ✅ Complete Core system
- ✅ Manager layer
- ✅ Statistics & Algorithms
- ✅ File I/O & Backup
- ✅ 33/33 tests passing

---

## 📞 SUPPORT

**Project**: Football Field Management System  
**Architecture**: Qt + C++ + OOP + MVVM  
**Build Location**: `FootballFieldManager/build/bin/FootballFieldManager.exe`  
**Status**: Week 1-3 Complete ✅ | Week 4 Coming Soon 🔜

---

**Chúc bạn code thành công! ⚽🚀**
