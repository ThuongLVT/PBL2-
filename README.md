# ⚽ Hệ Thống Quản Lý Sân Bóng Đá

**Football Field Management System** - Phần mềm quản lý sân bóng đá được xây dựng bằng Qt6 + C++ với kiến trúc OOP thuần túy.

## 🎯 Đặc điểm nổi bật

- ✅ **100% C++ OOP** - Không sử dụng STL
- ✅ **Custom Data Structures** - Tự implement vector, linked list, stack, queue
- ✅ **Core Logic Complete** - Business logic đầy đủ chức năng
- ✅ **Qt6 GUI Ready** - UI trực tiếp gọi Core (No Bridge Layer)
- ✅ **CMake Build System** - Cross-platform, hỗ trợ VS Code và Qt Creator
- ✅ **File-based Storage** - OOP Serialization

## 📦 Yêu cầu hệ thống

- **Qt**: 6.x (chỉ Qt Core)
- **CMake**: 3.16+
- **Compiler**: GCC/MinGW (C++17) hoặc MSVC 2019+
- **IDE**: VS Code (CMake Tools) hoặc Qt Creator

## 🚀 Quick Start

### Build với VS Code

```bash
# 1. Mở project
code d:\FootballFieldManager

# 2. Configure & Build
Ctrl+Shift+B → "CMake: Build"

# 3. Run
Ctrl+Shift+P → "Tasks: Run Task" → "Run Application"
```

### Build với Qt Creator

```bash
# 1. Mở project
File → Open File or Project → CMakeLists.txt

# 2. Build & Run
Ctrl+R
```

### Build từ Command Line

```bash
# Configure
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build -j 4

# Run
build\bin\FootballFieldManager.exe
```

## 📂 Cấu trúc dự án

```
FootballFieldManager/
├── CMakeLists.txt          # CMake configuration
├── CMakePresets.json       # Build presets
├── main.cpp                # Entry point
│
├── Core/                   # Logic C++ thuần (không phụ thuộc Qt)
│   ├── CauTrucDuLieu/     # ✅ Data structures
│   ├── Models/            # ✅ Business models
│   ├── QuanLy/            # ✅ Management classes
│   ├── ThongKe/           # ✅ Statistics
│   ├── ThuatToan/         # ✅ Algorithms
│   └── Utils/             # ✅ Utilities
│
├── Tests/                  # Unit tests
├── Data/                   # Runtime data (binary files)
└── Docs/                   # 📚 Documentation
```

## 📚 Tài liệu

- **[README](Docs/README.md)** - Tổng quan chi tiết
- **[BUILD](Docs/BUILD.md)** - Hướng dẫn build đầy đủ
- **[QUICKSTART](Docs/QUICKSTART.md)** - Tham khảo nhanh
- **[STATUS](Docs/STATUS.md)** - Tiến độ hiện tại
- **[WEEK1_SUMMARY](Docs/WEEK1_SUMMARY.md)** - Báo cáo tuần 1

## 📈 Tiến độ

```
Core Data Structures       ████████████████████ 100% ✅
Core Models                ████████████████████ 100% ✅
Core Management            ███████████████████░  95% ✅
Core Statistics            ████████████████████ 100% ✅
Algorithms                 ████████████████████ 100% ✅
File Storage               ████████████░░░░░░░░  65% ⚠️ (Cần refactor)

Overall: Core Complete, UI Development Ready ✅
```

## ✅ Features Complete

**Core Features:**

- ✅ Custom Data Structures (MangDong, DanhSachLienKet, Ngan, HangDoi)
- ✅ Date/Time handling (ThoiGian, NgayThang, NgayGio)
- ✅ Core Models (16 classes: People, Fields, Services, Bookings, Payments)
- ✅ Field Management (San, KhungGio, DatSan)
- ✅ Service Management (DichVu, DoUong, ThietBi, DichVuDat)
- ✅ Payment System (ThanhToan với 3 phương thức: Cash/Card/Transfer)
- ✅ Statistics (ThongKeDoanhThu, ThongKeKhachHang)
- ✅ Algorithms (QuickSort, MergeSort, BinarySearchTree, HashTable)
- ✅ Manager Layer (HeThongQuanLy Singleton + 5 specialized managers)
- ✅ CMake build system

**Known Issues:**

- ⚠️ File I/O cần refactor (Models dùng FILE\*, Managers dùng ofstream)
- ⚠️ Pointer resolution khi load từ file chưa hoàn chỉnh

## 📝 License

Educational Project - OOP Course

---

**Status:** Core Complete ✅ | UI Development Phase 🚀  
**Next:** Xây dựng Qt6 GUI (no Bridge, direct Core access, React-inspired design)
