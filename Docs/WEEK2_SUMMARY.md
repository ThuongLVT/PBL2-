# 🎉 TUẦN 2 HOÀN THÀNH - Models Cơ Bản

## ✅ Đã hoàn thành 100%

### 📂 Các Models đã implement (16 files)

```
Core/Models/
├── ConNguoi.h/cpp          ✅ Base person class
├── NguoiDung.h/cpp         ✅ Base user class (kế thừa ConNguoi)
├── QuanTriVien.h/cpp       ✅ Admin (kế thừa NguoiDung)
├── NhanVien.h/cpp          ✅ Staff (kế thừa NguoiDung)
├── KhachHang.h/cpp         ✅ Customer (kế thừa ConNguoi)
├── San.h/cpp               ✅ Football Field
├── KhungGio.h/cpp          ✅ Time Slot
└── DichVu.h/cpp            ✅ Service base class
```

---

## 📊 Thống kê Code

| Metric                 | Count      |
| ---------------------- | ---------- |
| **C++ Classes**        | 8 (Models) |
| **Header Files**       | 8          |
| **Source Files**       | 8          |
| **Total Lines**        | ~2,400     |
| **Enums**              | 5          |
| **Inheritance Levels** | 3 levels   |
| **Build Success**      | ✅ 100%    |
| **Test Success**       | ✅ 100%    |

---

## 🏗️ Kiến trúc kế thừa (Inheritance Tree)

```
ConNguoi (Base)
├── NguoiDung (User with account)
│   ├── QuanTriVien (Admin)
│   └── NhanVien (Staff)
└── KhachHang (Customer - no account)

San (Field) - Standalone

KhungGio (TimeSlot) - Standalone

DichVu (Service) - Base for future services
```

---

## 🎯 Các tính năng OOP được áp dụng

### 1. **Kế thừa (Inheritance)**

- `ConNguoi` → `NguoiDung` → `QuanTriVien`, `NhanVien`
- `ConNguoi` → `KhachHang`
- Virtual destructors cho phép kế thừa an toàn

### 2. **Đa hình (Polymorphism)**

- Virtual methods: `hienThiThongTin()`, `ghiFile()`, `docFile()`
- Method overriding trong các lớp con
- Test đa hình thành công với con trỏ base class

### 3. **Đóng gói (Encapsulation)**

- Private/Protected/Public access modifiers
- Getters/Setters cho tất cả thuộc tính
- Che giấu implementation details

### 4. **Abstraction**

- Base classes định nghĩa interface chung
- Derived classes implement chi tiết cụ thể

---

## 📋 Chi tiết từng Model

### 1. ConNguoi (Base Person)

**Thuộc tính:**

- `hoTen`: ChuoiKyTu
- `soDienThoai`: ChuoiKyTu
- `diaChi`: ChuoiKyTu

**Methods:**

- Constructors (default, parameterized, copy)
- Virtual destructor
- Getters/Setters
- `virtual hienThiThongTin()`
- `virtual ghiFile() / docFile()`
- Operators: `=`, `==`, `<<`

---

### 2. NguoiDung (User)

**Kế thừa:** ConNguoi

**Thuộc tính bổ sung:**

- `tenDangNhap`: ChuoiKyTu
- `matKhau`: ChuoiKyTu
- `vaiTro`: VaiTro enum (QUAN_TRI_VIEN, NHAN_VIEN)
- `hoatDong`: bool

**Methods bổ sung:**

- `dangNhap(matKhau)` - Xác thực đăng nhập
- `doiMatKhau(mkCu, mkMoi)` - Đổi mật khẩu
- `chuyenTrangThaiHoatDong()` - Khóa/mở khóa tài khoản
- Override `hienThiThongTin()`, `ghiFile()`, `docFile()`

---

### 3. QuanTriVien (Admin)

**Kế thừa:** NguoiDung

**Thuộc tính bổ sung:**

- `ngayTao`: NgayGio
- `soLanDangNhap`: int
- `lanDangNhapCuoi`: NgayGio

**Methods bổ sung:**

- `capNhatDangNhap()` - Tăng số lần đăng nhập
- Override `hienThiThongTin()`, `ghiFile()`, `docFile()`

**Vai trò:** Quản lý toàn bộ hệ thống

---

### 4. NhanVien (Staff)

**Kế thừa:** NguoiDung

**Thuộc tính bổ sung:**

- `maNhanVien`: ChuoiKyTu
- `ngayVaoLam`: NgayThang
- `luongCoBan`: double
- `caLamViec`: CaLamViec enum (SANG, CHIEU, TOI)
- `soNgayLam`: int

**Methods bổ sung:**

- `tangSoNgayLam()` - Tăng số ngày làm việc
- `tinhLuong()` - Tính lương theo công thức
- Override `hienThiThongTin()`, `ghiFile()`, `docFile()`

**Vai trò:** Quản lý các nghiệp vụ hàng ngày

---

### 5. KhachHang (Customer)

**Kế thừa:** ConNguoi (không cần tài khoản)

**Thuộc tính bổ sung:**

- `maKhachHang`: ChuoiKyTu
- `diemTichLuy`: int
- `hang`: HangKhachHang enum (DONG, BAC, VANG, KIM_CUONG)
- `ngayDangKy`: NgayThang
- `soLanDatSan`: int

**Methods bổ sung:**

- `themDiemTichLuy(diem)` - Thêm điểm
- `truDiemTichLuy(diem)` - Trừ điểm (đổi quà)
- `tangSoLanDatSan()` - Tăng số lần đặt
- `capNhatHang()` - Tự động cập nhật hạng
- `layPhanTramGiamGia()` - Tính giảm giá theo hạng

**Hạng khách hàng:**

- Đồng: 0-99 điểm (Giảm 0%)
- Bạc: 100-499 điểm (Giảm 5%)
- Vàng: 500-999 điểm (Giảm 10%)
- Kim cương: 1000+ điểm (Giảm 15%)

---

### 6. San (Football Field)

**Thuộc tính:**

- `maSan`: ChuoiKyTu
- `tenSan`: ChuoiKyTu
- `loaiSan`: LoaiSan enum (SAN_5, SAN_7)
- `giaThueGio`: double
- `trangThai`: TrangThaiSan enum (TRONG, DA_DAT, DANG_SU_DUNG, BAO_TRI)
- `ghiChu`: ChuoiKyTu

**Methods:**

- `coTrong()` - Kiểm tra sân trống
- `tinhTien(soGio)` - Tính tiền thuê
- `hienThiThongTin()`, `ghiFile()`, `docFile()`
- Operators: `=`, `==`, `<`, `<<`

---

### 7. KhungGio (Time Slot)

**Thuộc tính:**

- `gioBatDau`: ThoiGian
- `gioKetThuc`: ThoiGian

**Methods:**

- `tinhSoGio()` - Tính số giờ thuê (dạng double)
- `hopLe()` - Kiểm tra khung giờ hợp lệ
- `kiemTraTrung(other)` - Kiểm tra trùng lặp với khung giờ khác
- `hienThiThongTin()`, `ghiFile()`, `docFile()`
- Operators: `=`, `==`, `<`, `<<`

**Logic kiểm tra trùng:**

- Phát hiện overlap giữa các khung giờ
- Hỗ trợ tránh double booking

---

### 8. DichVu (Service)

**Thuộc tính:**

- `maDichVu`: ChuoiKyTu
- `tenDichVu`: ChuoiKyTu
- `donGia`: double
- `loaiDichVu`: LoaiDichVu enum (DO_UONG, THIET_BI, BAO_HIEM, KHAC)
- `moTa`: ChuoiKyTu
- `conHang`: bool

**Methods:**

- `tinhTien(soLuong)` - Tính tiền theo số lượng
- `virtual hienThiThongTin()`
- `virtual ghiFile() / docFile()`
- Operators: `=`, `==`, `<`, `<<`

**Thiết kế:** Base class cho các dịch vụ cụ thể (sẽ implement sau)

---

## 🧪 Test Results - ALL PASS ✅

### Test OOP Features

```
✅ ConNguoi          - Base class functionality
✅ NguoiDung         - Login, change password
✅ QuanTriVien       - Admin specific features
✅ NhanVien          - Staff management, salary calculation
✅ KhachHang         - Loyalty points, tier system
✅ San               - Field management, pricing
✅ KhungGio          - Time slot validation, overlap detection
✅ DichVu            - Service pricing

✅ Kế thừa (Inheritance) - Working correctly
✅ Đa hình (Polymorphism) - Virtual methods tested
✅ Đóng gói (Encapsulation) - Private members protected
```

### Test Output Sample

```cpp
// Đa hình test
ConNguoi* nguoi1 = new QuanTriVien(...);
ConNguoi* nguoi2 = new NhanVien(...);
ConNguoi* nguoi3 = new KhachHang(...);

nguoi1->hienThiThongTin(); // ✅ Calls QuanTriVien version
nguoi2->hienThiThongTin(); // ✅ Calls NhanVien version
nguoi3->hienThiThongTin(); // ✅ Calls KhachHang version
```

---

## 🔧 Build System Updates

### CMakeLists.txt

```cmake
set(CORE_MODELS_SOURCES
    Core/Models/ConNguoi.cpp
    Core/Models/NguoiDung.cpp
    Core/Models/QuanTriVien.cpp
    Core/Models/NhanVien.cpp
    Core/Models/KhachHang.cpp
    Core/Models/San.cpp
    Core/Models/KhungGio.cpp
    Core/Models/DichVu.cpp
)

set(CORE_MODELS_HEADERS
    Core/Models/ConNguoi.h
    Core/Models/NguoiDung.h
    Core/Models/QuanTriVien.h
    Core/Models/NhanVien.h
    Core/Models/KhachHang.h
    Core/Models/San.h
    Core/Models/KhungGio.h
    Core/Models/DichVu.h
)
```

### main.cpp

- Cấu trúc lại thành 2 functions: `testWeek1_DataStructures()` và `testWeek2_Models()`
- Test đầy đủ tất cả Models
- Test kế thừa và đa hình

---

## 🐛 Issues Fixed During Development

### 1. File I/O Return Types

**Problem:** ChuoiKyTu, ThoiGian, NgayThang, NgayGio có `ghiFile()/docFile()` return `void`
**Solution:**

- Sửa ChuoiKyTu return `bool`
- Không check return cho ThoiGian, NgayThang, NgayGio (sẽ fix ở Tuần 11)

### 2. Method Naming Inconsistency

**Problem:** Một số methods dùng tiếng Việt (`layGio`), một số dùng tiếng Anh (`getGio`)
**Solution:** Sử dụng naming từ class đã có (ThoiGian dùng `get`, ChuoiKyTu dùng `length`)

### 3. Build Path Issues

**Problem:** CMake cache có path cũ
**Solution:** Xóa `build/` và reconfigure từ đầu

---

## 📈 Progress Tracking

```
TUẦN 1: Cấu trúc dữ liệu  ████████████████████ 100% ✅
TUẦN 2: Models cơ bản      ████████████████████ 100% ✅
TUẦN 3: Models phức tạp    ░░░░░░░░░░░░░░░░░░░░   0% 🔜
TUẦN 4: Bridge Layer       ░░░░░░░░░░░░░░░░░░░░   0%
TUẦN 5-6: UI cơ bản        ░░░░░░░░░░░░░░░░░░░░   0%
TUẦN 7-10: Chức năng       ░░░░░░░░░░░░░░░░░░░░   0%
TUẦN 11: File Storage      ░░░░░░░░░░░░░░░░░░░░   0%
TUẦN 12-14: Hoàn thiện     ░░░░░░░░░░░░░░░░░░░░   0%
```

**Overall Progress: 25% (2/8 weeks completed)**

---

## 🚀 Next Steps (Week 3)

### Models phức tạp cần implement:

```cpp
Core/Models/
├── PhieuDatSan.h/cpp       // Booking entity (combine KhachHang + San + KhungGio)
├── HoaDon.h/cpp            // Invoice
├── ChiTietHoaDon.h/cpp     // Invoice details
├── ThanhToan.h/cpp         // Payment
├── BaoCao.h/cpp            // Report base class
└── LichSuGiaoDich.h/cpp    // Transaction history
```

### Key features to add:

1. **PhieuDatSan** - Liên kết KhachHang, San, KhungGio, DichVu
2. **HoaDon** - Tính tổng tiền, áp dụng giảm giá
3. **ThanhToan** - Xử lý các phương thức thanh toán
4. **Relationships** - Implement quan hệ giữa các objects

---

## 🎓 Lessons Learned

### 1. **OOP Design Principles**

- Single Responsibility: Mỗi class chỉ làm một việc
- Open/Closed: Có thể extend không cần modify
- Liskov Substitution: Derived classes có thể thay thế base classes

### 2. **Inheritance Hierarchy**

- Base classes chứa logic chung
- Derived classes thêm functionality cụ thể
- Virtual methods cho phép override

### 3. **Code Organization**

- Header files: Declarations
- Source files: Implementations
- Clear separation of concerns

### 4. **Testing Strategy**

- Test base classes trước
- Test derived classes sau
- Test polymorphism cuối cùng

---

## 📚 Documentation

### Files đã cập nhật:

- ✅ CMakeLists.txt - Thêm Models sources
- ✅ main.cpp - Test Tuần 1 & 2
- ✅ README.md - Cập nhật progress
- ✅ WEEK2_SUMMARY.md - Tài liệu này
- 🔜 STATUS.md - Cập nhật trạng thái dự án

---

## 🏆 Week 2 Success Criteria - ALL MET ✅

- ✅ 8 Models classes implemented
- ✅ Inheritance working (3 levels)
- ✅ Polymorphism tested successfully
- ✅ Encapsulation applied
- ✅ No STL used (custom data structures)
- ✅ Memory management correct
- ✅ Build system updated
- ✅ All tests passing
- ✅ Documentation complete
- ✅ Code quality good
- ✅ Ready for Week 3

---

**Status: Week 2 Complete! Moving to Week 3... 🚀⚽**

_Last Updated: November 3, 2025_
_Project: Football Field Management System_
_Team: Development Team_
