# Hướng Dẫn Đóng Góp Code - PBL2 Football Field Management

## Setup Ban Đầu

### 1. Clone Repository

```bash
git clone https://github.com/ThuongLVT/PBL2-.git
cd PBL2-
```

### 2. Cài Đặt Dependencies

- Qt 6.5+
- CMake 3.20+
- MinGW hoặc MSVC compiler
- Git

## Workflow Làm Việc Nhóm

### Branch Strategy

```
main (production-ready code)
  ├── develop (integration branch)
  ├── feature/booking (người làm đặt sân)
  ├── feature/service (người làm dịch vụ)
  └── feature/payment (người làm thanh toán)
```

### Tạo Branch Mới

```bash
# Lấy code mới nhất
git checkout main
git pull origin main

# Tạo branch feature của bạn
git checkout -b feature/booking      # Người làm đặt sân
# hoặc
git checkout -b feature/service      # Người làm dịch vụ
```

## Phân Chia Công Việc

### Người 1: Booking Management (Đặt Sân)

**Files được phép sửa:**

- `UI/Pages/Booking/*`
- `Core/QuanLy/QuanLyDatSan.cpp/h`
- `Core/Models/DatSan.cpp/h`
- `Core/Models/KhungGio.cpp/h`

**Không được sửa:**

- `MainWindow.cpp` (trừ khi thảo luận)
- `HeThongQuanLy.cpp/h` (trừ khi thảo luận)
- Files của người khác

### Người 2: Service Management (Dịch Vụ)

**Files được phép sửa:**

- `UI/Pages/Service/*`
- `Core/QuanLy/QuanLyDichVu.cpp/h`
- `Core/Models/DichVu.cpp/h`
- `Core/Models/DichVuDat.cpp/h`

**Không được sửa:**

- `MainWindow.cpp` (trừ khi thảo luận)
- `HeThongQuanLy.cpp/h` (trừ khi thảo luận)
- Files của người khác

## Quy Trình Commit

### 1. Kiểm tra thay đổi

```bash
git status
git diff
```

### 2. Add files

```bash
# Add specific files (khuyên dùng)
git add UI/Pages/Booking/BookingListPage.cpp
git add Core/QuanLy/QuanLyDatSan.cpp

# Hoặc add tất cả (cẩn thận!)
git add .
```

### 3. Commit với message rõ ràng

```bash
git commit -m "feat(booking): add create booking dialog"
git commit -m "fix(service): correct price calculation"
git commit -m "refactor(booking): improve booking list display"
```

**Commit Message Format:**

- `feat(module): description` - Tính năng mới
- `fix(module): description` - Sửa lỗi
- `refactor(module): description` - Cải thiện code
- `docs(module): description` - Cập nhật tài liệu
- `style(module): description` - Format code
- `test(module): description` - Thêm test

### 4. Push lên GitHub

```bash
git push origin feature/booking
# hoặc
git push origin feature/service
```

## Quy Trình Merge Code

### 1. Update Branch của bạn với Main

```bash
git checkout main
git pull origin main
git checkout feature/booking
git merge main
# Giải quyết conflicts nếu có
```

### 2. Tạo Pull Request trên GitHub

1. Vào https://github.com/ThuongLVT/PBL2-/pulls
2. Click "New Pull Request"
3. Base: `main` <- Compare: `feature/booking`
4. Viết mô tả chi tiết về thay đổi
5. Assign reviewer (người trong nhóm)
6. Chờ review và approve

### 3. Merge sau khi được approve

- Có thể merge ngay trên GitHub
- Hoặc merge local:

```bash
git checkout main
git merge feature/booking
git push origin main
```

## Giải Quyết Conflicts

### Khi có conflict:

```bash
# Xem file nào bị conflict
git status

# Mở file và tìm các dòng:
<<<<<<< HEAD
code của main branch
=======
code của bạn
>>>>>>> feature/booking

# Sửa và giữ lại code đúng
# Sau đó:
git add <file-đã-sửa>
git commit -m "resolve: merge conflicts in booking module"
```

## Best Practices

### 1. Commit thường xuyên

- Commit mỗi khi hoàn thành 1 tính năng nhỏ
- Không chờ đến cuối ngày mới commit

### 2. Pull thường xuyên

```bash
# Mỗi sáng trước khi code
git checkout main
git pull origin main
git checkout feature/booking
git merge main
```

### 3. Code Review

- Review code của nhau trên Pull Request
- Comment constructive
- Học hỏi từ code của teammate

### 4. Testing

- Test kỹ trước khi push
- Build thành công mới được push

```bash
# Test build
cmake -B build -S . -G "MinGW Makefiles"
cmake --build build
./build/bin/FootballFieldManager.exe
```

### 5. Communication

- Thảo luận trước khi sửa file chung
- Thông báo khi push code lớn
- Họp daily standup nếu cần

## Troubleshooting

### Lỗi "Repository not found"

```bash
# Check remote
git remote -v
# Nếu sai, sửa lại:
git remote set-url origin https://github.com/ThuongLVT/PBL2-.git
```

### Lỗi Authentication

```bash
# Sử dụng Personal Access Token thay vì password
# Tạo token tại: https://github.com/settings/tokens
```

### Quên pull trước khi code

```bash
# Stash code của bạn
git stash
# Pull code mới
git pull origin main
# Apply code của bạn lại
git stash pop
```

### Muốn undo commit cuối

```bash
# Undo nhưng giữ thay đổi
git reset --soft HEAD~1

# Undo và xóa thay đổi (cẩn thận!)
git reset --hard HEAD~1
```

## Liên Hệ

- GitHub Issues: https://github.com/ThuongLVT/PBL2-/issues
- Team Lead: ThuongLVT

---

**Lưu ý:** Luôn backup code trước khi thực hiện các thao tác nguy hiểm!
