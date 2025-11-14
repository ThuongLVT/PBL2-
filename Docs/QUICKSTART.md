# ⚡ QUICK START - Build & Run

## 🚀 Chạy nhanh nhất (VS Code)

```bash
# 1. Mở folder trong VS Code
code d:\\QT_PBL2

# 2. Build & Run (một lệnh)
Ctrl+Shift+B → "CMake: Build"
Ctrl+Shift+P → "Tasks: Run Task" → "Run Application"
```

## 🎯 Chạy nhanh (Qt Creator)

```bash
# 1. Mở project
File → Open File or Project → chọn CMakeLists.txt

# 2. Build & Run
Ctrl+R
```

## 💻 Chạy nhanh (Command Line)

```bash
# Windows
cd d:\\QT_PBL2
cmake -B build -S . -G "MinGW Makefiles"
cmake --build build -j 4
build\bin\FootballFieldManager.exe

# Linux/macOS
cd /path/to/FootballFieldManager
cmake -B build -S .
cmake --build build -j $(nproc)
./build/bin/FootballFieldManager
```

---

## 📂 Cấu trúc Project

```
FootballFieldManager/
├── CMakeLists.txt          ← CMake config (build với VS Code/Qt Creator)
├── FootballFieldManager.pro ← qmake config (legacy, chỉ Qt Creator)
├── main.cpp                ← Entry point
├── BUILD.md                ← Hướng dẫn build chi tiết
├── README.md               ← Tổng quan dự án
│
├── Core/                   ← Logic C++ thuần (không Qt)
│   ├── CauTrucDuLieu/     ← ✅ TUẦN 1 HOÀN THÀNH
│   ├── Models/            ← 🔜 Tuần 2
│   ├── QuanLy/            ← 🔜 Tuần 3
│   ├── ThongKe/           ← 🔜 Tuần 3
│   └── ThuatToan/         ← 🔜 Tuần 3
│
├── Bridge/                 ← 🔜 Tuần 4 (Core ↔ UI)
├── UI/                     ← 🔜 Tuần 5-10 (Qt Widgets)
├── Utils/                  ← Tiện ích
├── Tests/                  ← Unit tests
├── Data/                   ← Runtime data (binary files)
└── Docs/                   ← Tài liệu
```

---

## ✅ Tiến độ hiện tại

### TUẦN 1: Cấu trúc dữ liệu cơ bản ✅ (100%)

- ✅ ChuoiKyTu (replace std::string)
- ✅ MangDong<T> (replace std::vector)
- ✅ DanhSachLienKet<T> (Linked List)
- ✅ Ngan<T> (Stack) và HangDoi<T> (Queue)
- ✅ ThoiGian, NgayThang, NgayGio
- ✅ CMake build system (VS Code + Qt Creator)
- ✅ Test console thành công

### TUẦN 2: Models cơ bản 🔜 (Next)

- [ ] ConNguoi (lớp cơ sở)
- [ ] NguoiDung (lớp cơ sở người dùng)
- [ ] QuanTriVien, NhanVien, KhachHang
- [ ] San, KhungGio
- [ ] DichVu (DoUong, ThietBi, BaoHiem)

---

## 🛠️ Build Systems

### ✅ CMake (Khuyến nghị)

- ✅ VS Code support
- ✅ Qt Creator support
- ✅ Cross-platform
- ✅ Modern C++ tooling

### ✅ qmake (Legacy)

- ✅ Qt Creator only
- ⚠️ Không dùng cho VS Code

---

## 📖 Tài liệu

- **Kế hoạch chi tiết**: `QT_PROJECT_STRUCTURE.md` (ở parent folder)
- **Hướng dẫn build**: `BUILD.md`
- **Tổng quan**: `README.md`

---

## 🎯 Next Steps

```bash
# 1. Test lại toàn bộ Tuần 1
build\bin\FootballFieldManager.exe

# 2. Bắt đầu Tuần 2 - Models cơ bản
# Tạo các file:
# - Core/Models/ConNguoi.h/cpp
# - Core/Models/NguoiDung.h/cpp
# - Core/Models/QuanTriVien.h/cpp
# - Core/Models/NhanVien.h/cpp
# - Core/Models/KhachHang.h/cpp
```

---

**Happy Coding! ⚽🚀**
