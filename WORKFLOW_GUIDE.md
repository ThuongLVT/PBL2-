# Hướng Dẫn Workflow Cho Team

## Setup Đã Hoàn Thành ✅

### Bạn (ThuongLVT) - Làm Đặt Sân
- ✅ Đã tạo branch `feature/booking`
- ✅ Đang ở branch `feature/booking`

### khninh22 - Làm Dịch Vụ
Chạy lệnh sau trong terminal:
```bash
cd D:\QT_PBL2  # hoặc đường dẫn nơi clone về
git checkout -b feature/service
```

---

## 📋 Phân Công Chi Tiết

### 🎯 Bạn (ThuongLVT) - Module Đặt Sân

**Files BẠN ĐƯỢC phép sửa:**
```
UI/Pages/Booking/
├── BookingListPage.cpp
├── BookingListPage.h
└── (các file mới bạn tạo trong Booking/)

Core/QuanLy/
├── QuanLyDatSan.cpp
├── QuanLyDatSan.h

Core/Models/
├── DatSan.cpp
├── DatSan.h
├── KhungGio.cpp
└── KhungGio.h
```

**Files KHÔNG được sửa:**
- ❌ `UI/Pages/Service/*` (của khninh22)
- ❌ `Core/QuanLy/QuanLyDichVu.*` (của khninh22)
- ❌ `Core/Models/DichVu.*` (của khninh22)

**Files CẦN THẢO LUẬN trước khi sửa:**
- ⚠️ `UI/MainWindow/MainWindow.cpp` (chỉ thêm kết nối UI)
- ⚠️ `Core/QuanLy/HeThongQuanLy.cpp/h` (nếu cần thêm method)
- ⚠️ `CMakeLists.txt` (khi thêm file mới)

---

### 🎯 khninh22 - Module Dịch Vụ

**Files khninh22 ĐƯỢC phép sửa:**
```
UI/Pages/Service/
├── ServiceManagementPage.cpp
├── ServiceManagementPage.h
└── (các file mới khninh22 tạo trong Service/)

Core/QuanLy/
├── QuanLyDichVu.cpp
├── QuanLyDichVu.h

Core/Models/
├── DichVu.cpp
├── DichVu.h
├── DichVuDat.cpp
└── DichVuDat.h
```

**Files khninh22 KHÔNG được sửa:**
- ❌ `UI/Pages/Booking/*` (của ThuongLVT)
- ❌ `Core/QuanLy/QuanLyDatSan.*` (của ThuongLVT)
- ❌ `Core/Models/DatSan.*` (của ThuongLVT)

---

## 🔄 Quy Trình Làm Việc Hàng Ngày

### Sáng (Trước Khi Code)
```bash
# 1. Kiểm tra branch hiện tại
git branch

# 2. Lấy code mới nhất từ main
git checkout main
git pull origin main

# 3. Quay về branch của mình và merge main vào
git checkout feature/booking      # Bạn
# hoặc
git checkout feature/service      # khninh22

# 4. Merge code mới từ main
git merge main
```

### Trong Ngày (Sau Mỗi Tính Năng Nhỏ)
```bash
# 1. Xem thay đổi
git status
git diff

# 2. Add file đã sửa
git add UI/Pages/Booking/BookingListPage.cpp
git add Core/QuanLy/QuanLyDatSan.cpp

# 3. Commit với message rõ ràng
git commit -m "feat(booking): add create booking form"
# hoặc
git commit -m "fix(booking): correct date validation"

# 4. Push lên GitHub
git push origin feature/booking      # Bạn
# hoặc
git push origin feature/service      # khninh22
```

### Tối (Trước Khi Kết Thúc)
```bash
# Push tất cả thay đổi lên GitHub
git status
git add .
git commit -m "feat(booking): complete booking list UI"
git push origin feature/booking
```

---

## 🔥 Commit Message Format

### Template:
```
<type>(<module>): <description>

[optional body]
```

### Types:
- `feat` - Tính năng mới
- `fix` - Sửa lỗi
- `refactor` - Cải thiện code
- `style` - Format, UI styling
- `docs` - Cập nhật tài liệu
- `test` - Thêm test

### Ví dụ:
```bash
git commit -m "feat(booking): add booking creation dialog"
git commit -m "fix(service): correct price calculation in service form"
git commit -m "refactor(booking): improve booking list performance"
git commit -m "style(booking): update booking card UI design"
```

---

## 🎯 Ví Dụ Workflow Thực Tế

### Ngày 1 - Bạn (Booking)
```bash
# Sáng
git checkout main
git pull origin main
git checkout feature/booking
git merge main

# Code BookingListPage UI
# ... code ...

git add UI/Pages/Booking/BookingListPage.cpp
git commit -m "feat(booking): implement booking list layout"
git push origin feature/booking

# Chiều - tiếp tục
# ... code thêm ...
git add UI/Pages/Booking/BookingListPage.cpp
git commit -m "feat(booking): add booking filters"
git push origin feature/booking
```

### Ngày 1 - khninh22 (Service)
```bash
# Sáng
git checkout main
git pull origin main
git checkout feature/service
git merge main

# Code ServiceManagementPage UI
# ... code ...

git add UI/Pages/Service/ServiceManagementPage.cpp
git commit -m "feat(service): implement service list UI"
git push origin feature/service

# Chiều - tiếp tục
# ... code thêm ...
git add UI/Pages/Service/ServiceManagementPage.cpp
git commit -m "feat(service): add service CRUD operations"
git push origin feature/service
```

---

## 🔗 Kết Hợp Code (Sau Khi Hoàn Thành)

### Cách 1: Pull Request trên GitHub (Khuyên Dùng)

#### Bạn:
1. Vào https://github.com/ThuongLVT/PBL2-/pulls
2. Click "New Pull Request"
3. Chọn: `base: main` ← `compare: feature/booking`
4. Viết mô tả chi tiết
5. Assign khninh22 làm reviewer
6. Chờ review và approve

#### khninh22:
1. Review code của bạn trên GitHub
2. Comment nếu có vấn đề
3. Approve nếu OK
4. Sau đó tạo PR của khninh22: `main` ← `feature/service`

### Cách 2: Merge Trực Tiếp (Sau Khi Thảo Luận)

```bash
# Cả 2 người push code lên trước
git push origin feature/booking    # Bạn
git push origin feature/service    # khninh22

# Sau đó 1 người (thường là team lead) merge:
git checkout main
git pull origin main

# Merge booking
git merge feature/booking
# Test xem có lỗi không

# Merge service
git merge feature/service
# Test xem có conflict không

# Nếu có conflict, giải quyết rồi:
git add .
git commit -m "merge: integrate booking and service modules"
git push origin main
```

---

## 🚨 Xử Lý Conflicts

### Khi có conflict:
```bash
# Git sẽ báo file nào bị conflict
git status

# Mở file đó, tìm:
<<<<<<< HEAD
code từ main/branch khác
=======
code của bạn
>>>>>>> feature/booking

# Sửa thành code đúng, xóa các dấu <<<<, ====, >>>>
# Sau đó:
git add <file-đã-sửa>
git commit -m "resolve: merge conflicts in booking module"
git push origin feature/booking
```

---

## 📞 Communication

### Trước khi code file chung:
💬 **Nhắn tin group trước:**
> "Mình cần sửa MainWindow.cpp để thêm button Đặt Sân, OK không?"

### Mỗi ngày:
📅 **Daily standup (có thể chat):**
1. Hôm qua làm gì?
2. Hôm nay làm gì?
3. Có vấn đề gì không?

### Khi push code lớn:
📢 **Thông báo:**
> "Mình vừa push booking module lên feature/booking, mọi người pull về nhé!"

---

## ✅ Checklist Trước Khi Push

- [ ] Code build thành công
- [ ] Đã test chức năng
- [ ] Không sửa file của người khác
- [ ] Commit message rõ ràng
- [ ] Đã pull code mới nhất từ main

```bash
# Test build
cmake -B build -S . -G "MinGW Makefiles"
cmake --build build
./build/bin/FootballFieldManager.exe
```

---

## 🆘 Commands Hữu Ích

```bash
# Xem branch hiện tại
git branch

# Chuyển branch
git checkout feature/booking

# Xem thay đổi
git status
git diff

# Xem lịch sử commit
git log --oneline

# Hủy thay đổi file (cẩn thận!)
git checkout -- <file>

# Xem remote
git remote -v

# Pull code mới
git pull origin main

# Push code
git push origin feature/booking

# Xem ai sửa file này
git log <file>
git blame <file>
```

---

## 📚 Resources

- Repository: https://github.com/ThuongLVT/PBL2-
- Issues: https://github.com/ThuongLVT/PBL2-/issues
- Pull Requests: https://github.com/ThuongLVT/PBL2-/pulls

---

**Good luck coding! 🚀**
