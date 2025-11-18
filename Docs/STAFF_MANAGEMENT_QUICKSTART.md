# 📝 STAFF MANAGEMENT - QUICK START
## Hướng Dẫn Nhanh Quản Lý Nhân Viên

### 🚀 CÁCH SỬ DỤNG

#### **1. ADMIN - Quản lý toàn bộ nhân viên**

**Login:**
```
Username: admin
Password: admin123
```

**Thao tác:**
- Click menu "👨‍💼 Quản Lý Nhân Viên"
- **Xem danh sách**: Table hiển thị tất cả nhân viên
- **Tìm kiếm**: Nhập tên/SĐT/username vào search box
- **Thêm mới**: Click "➕ Thêm Mới" → Nhập form → "💾 Lưu"
- **Sửa**: Click dòng trong table → Edit form → "💾 Lưu"
- **Xóa**: Click dòng → "🗑️ Xóa" → Xác nhận

#### **2. STAFF - Chỉ xem/sửa thông tin cá nhân**

**Login:**
```
Username: staff1
Password: staff123
```

**Thao tác:**
- Menu "Quản Lý Nhân Viên" **BỊ ẨN** ❌
- Click "👤 Tài Khoản" để xem thông tin cá nhân
- Click "✏️ Chỉnh Sửa" để sửa Tên/SĐT/Giới tính/Ngày sinh
- Click "🔒 Đổi Mật Khẩu" để thay password

---

### 📂 FILE CSV

**Location:** `D:/QT_PBL2/Data/nhanvien.csv`

**Format:**
```csv
MaNV,HoTen,SoDienThoai,GioiTinh,NgaySinh,TenDangNhap,MatKhau,VaiTro,TrangThai
NV001,Admin User,0987654321,Nam,01/01/1990,admin,admin123,Admin,Hoat dong
NV002,Staff One,0987654322,Nữ,15/05/1995,staff1,staff123,Staff,Hoat dong
```

**Lưu ý:**
- File tự động cập nhật khi Add/Edit/Delete
- Chỉ Admin mới có quyền đọc/ghi file
- Staff cố gắng truy cập → Error message

---

### 🔒 PHÂN QUYỀN

| Chức Năng | Admin | Staff |
|-----------|-------|-------|
| Xem danh sách nhân viên | ✅ | ❌ |
| Thêm nhân viên | ✅ | ❌ |
| Sửa nhân viên bất kỳ | ✅ | ❌ |
| Xóa nhân viên | ✅ | ❌ |
| Xem thông tin cá nhân | ✅ | ✅ |
| Sửa thông tin cá nhân | ✅ | ✅ |
| Đọc/ghi nhanvien.csv | ✅ | ❌ |

---

### 🛠️ TROUBLESHOOTING

**Q: Staff không thấy menu "Quản Lý Nhân Viên"?**
- A: Đúng, Staff không có quyền. Đăng nhập Admin để quản lý.

**Q: Thêm nhân viên bị lỗi "Username đã tồn tại"?**
- A: Username phải unique. Chọn username khác.

**Q: CSV không cập nhật sau khi sửa?**
- A: Restart app. Nếu vẫn lỗi, check console log.

**Q: Làm sao reset về default?**
- A: Xóa file `nhanvien.csv`, copy file mẫu từ backup.

---

### 📋 VALIDATION RULES

**Thêm/Sửa nhân viên:**
- ✅ Họ tên: Không rỗng
- ✅ SĐT: >= 10 ký tự, chỉ số
- ✅ Username: >= 3 ký tự, unique, không có ký tự đặc biệt
- ✅ Password: >= 6 ký tự (bắt buộc cho NV mới, optional khi sửa)
- ✅ Vai trò: Admin hoặc Staff
- ✅ Giới tính: Nam/Nữ/Khác
- ✅ Ngày sinh: Định dạng DD/MM/YYYY

---

### 📖 CHI TIẾT ĐẦY ĐỦ

Xem file: `Docs/STAFF_MANAGEMENT_GUIDE.md` (300+ dòng)

Nội dung:
- Kiến trúc 3-layer security
- Data synchronization chi tiết
- CSV format specification
- Debug commands
- Advanced troubleshooting

---

**© 2025 Football Field Management System**
