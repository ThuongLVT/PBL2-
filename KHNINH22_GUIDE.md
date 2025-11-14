# Hướng Dẫn Cho khninh22 - Module Dịch Vụ

## ⚡ Quick Start

### 1. Tạo Branch của Bạn

```bash
cd D:\QT_PBL2
git checkout -b feature/service
```

### 2. Kiểm Tra

```bash
git branch
# Phải thấy: * feature/service
```

---

## 📁 Files Bạn Sẽ Làm Việc

### Chính:

- `UI/Pages/Service/ServiceManagementPage.cpp`
- `UI/Pages/Service/ServiceManagementPage.h`
- `Core/QuanLy/QuanLyDichVu.cpp`
- `Core/QuanLy/QuanLyDichVu.h`
- `Core/Models/DichVu.cpp`
- `Core/Models/DichVu.h`
- `Core/Models/DichVuDat.cpp`
- `Core/Models/DichVuDat.h`

### ❌ KHÔNG được sửa:

- `UI/Pages/Booking/*` (ThuongLVT đang làm)
- `Core/QuanLy/QuanLyDatSan.*`
- `Core/Models/DatSan.*`

---

## 💻 Workflow Hàng Ngày

### Sáng (Trước khi code):

```bash
git checkout main
git pull origin main
git checkout feature/service
git merge main
```

### Sau khi code xong 1 tính năng:

```bash
git add UI/Pages/Service/ServiceManagementPage.cpp
git commit -m "feat(service): implement service list UI"
git push origin feature/service
```

### Tối:

```bash
git add .
git commit -m "feat(service): complete service CRUD"
git push origin feature/service
```

---

## 🎯 Nhiệm Vụ Của Bạn

### Phase 1: Service List UI ✨

1. Sửa `ServiceManagementPage.cpp` để hiển thị danh sách dịch vụ
2. Thêm bảng (QTableWidget) với các cột:
   - Mã DV
   - Tên dịch vụ
   - Loại (Đồ uống/Thiết bị)
   - Giá
   - Trạng thái

### Phase 2: Service Management 🔧

1. Nút "Thêm dịch vụ"
2. Nút "Sửa dịch vụ"
3. Nút "Xóa dịch vụ"
4. Dialog để nhập thông tin dịch vụ

### Phase 3: Core Logic 💡

1. Implement các method trong `QuanLyDichVu.cpp`:
   - `themDichVu()`
   - `xoaDichVu()`
   - `suaDichVu()`
   - `timDichVu()`
   - `layDanhSachDichVu()`

### Phase 4: Integration 🔗

1. Kết nối UI với Core
2. Test thêm/sửa/xóa dịch vụ
3. Test load dữ liệu từ CSV/binary

---

## 📝 Commit Message Examples

```bash
git commit -m "feat(service): add service list table"
git commit -m "feat(service): implement add service dialog"
git commit -m "feat(service): add delete service confirmation"
git commit -m "fix(service): correct price validation"
git commit -m "style(service): improve service card design"
```

---

## 🔍 Code Reference

### Tham khảo từ Customer Page:

`UI/Pages/Customer/CustomerManagementPage.cpp` - đã implement tương tự

### Tham khảo QuanLy:

`Core/QuanLy/QuanLyKhachHang.cpp` - pattern tương tự

---

## 🆘 Nếu Cần Giúp

1. **Xem hướng dẫn chi tiết:** `WORKFLOW_GUIDE.md`
2. **Chat với ThuongLVT** trước khi sửa file chung
3. **Tạo Issue** trên GitHub nếu gặp bug

---

## ✅ Checklist Trước Khi Push

```bash
# Build test
cmake -B build -S . -G "MinGW Makefiles"
cmake --build build

# Run test
./build/bin/FootballFieldManager.exe

# Nếu OK:
git add .
git commit -m "feat(service): your message"
git push origin feature/service
```

---

## 📚 Useful Commands

```bash
# Xem branch
git branch

# Xem thay đổi
git status
git diff

# Hủy thay đổi 1 file
git checkout -- filename

# Xem log
git log --oneline

# Pull code mới
git pull origin main
```

---

**Let's code! 🚀**

Repository: https://github.com/ThuongLVT/PBL2-
