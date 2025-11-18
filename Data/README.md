# Hệ Thống Quản Lý Dữ Liệu CSV

## Tổng Quan

Dự án đã được chuyển đổi hoàn toàn sang sử dụng CSV để lưu trữ dữ liệu thay vì file binary. Tất cả dữ liệu được lưu trong thư mục `D:\QT_PBL2\Data\`.

## Các File CSV

### 1. khachhang.csv (Khách Hàng)

**Cột dữ liệu:**

- `MaKH`: Mã khách hàng (KH001, KH002, ...)
- `HoTen`: Họ tên đầy đủ
- `SoDienThoai`: Số điện thoại (10 số)
- `BacHoiVien`: Bậc hội viên (THUONG, DONG, BAC, VANG, KIM_CUONG)
- `TongChiTieu`: Tổng chi tiêu (số nguyên, không có đơn vị)
- `NgayDangKy`: Ngày đăng ký (DD/MM/YYYY)

**Ví dụ:**

```csv
MaKH,HoTen,SoDienThoai,BacHoiVien,TongChiTieu,NgayDangKy
KH001,Nguyễn Văn An,0901234567,THUONG,0,01/01/2024
KH003,Lê Hoàng Cường,0903456789,DONG,2500000,01/02/2024
```

### 2. san.csv (Sân Bóng)

**Cột dữ liệu:**

- `MaSan`: Mã sân (A501, B701, ...)
- `TenSan`: Tên sân
- `LoaiSan`: Loại sân (SAN_5, SAN_7)
- `KhuVuc`: Khu vực (A, B, C, D)
- `GiaThue`: Giá thuê/giờ (số nguyên)
- `TrangThai`: Trạng thái (Hoạt động, Bảo trì, Ngừng hoạt động)
- `GhiChu`: Ghi chú (có thể để trống)

**Ví dụ:**

```csv
MaSan,TenSan,LoaiSan,KhuVuc,GiaThue,TrangThai,GhiChu
A501,Sân A5-01,SAN_5,A,200000,Hoạt động,Sân 5 người khu vực A
B701,Sân B7-01,SAN_7,B,300000,Hoạt động,Sân 7 người khu vực B
C501,Sân C5-01,SAN_5,C,200000,Bảo trì,Đang bảo trì thảm cỏ
```

### 3. dichvu.csv (Dịch Vụ)

**Cột dữ liệu:**

- `MaDichVu`: Mã dịch vụ (DV001, DV002, ...)
- `TenDichVu`: Tên dịch vụ
- `LoaiDichVu`: Loại (DO_UONG, THIET_BI, BAO_HIEM, KHAC)
- `DonGia`: Đơn giá (số nguyên)
- `DonVi`: Đơn vị (Chai, Lon, Quả, Bộ, Đôi, Cái, ...)
- `SoLuongBan`: Số lượng đã bán
- `TrangThai`: Trạng thái (1 = còn hàng, 0 = hết hàng)
- `HinhAnh`: Tên file hình ảnh
- `MoTa`: Mô tả chi tiết

**Ví dụ:**

```csv
MaDichVu,TenDichVu,LoaiDichVu,DonGia,DonVi,SoLuongBan,TrangThai,HinhAnh,MoTa
DV001,Nước suối Aquafina,DO_UONG,5000,Chai,150,1,nuoc_suoi.png,Nước suối tinh khiết 500ml
DV004,Thuê bóng,THIET_BI,30000,Quả,50,1,ball.png,Bóng đá tiêu chuẩn FIFA
```

### 4. datsan.csv (Đặt Sân)

**Cột dữ liệu:**

- `MaDatSan`: Mã đặt sân (DS001, DS002, ...)
- `MaKhachHang`: Mã khách hàng (FK -> khachhang.csv)
- `MaSan`: Mã sân (FK -> san.csv)
- `NgayDat`: Ngày đặt (DD/MM/YYYY HH:MM)
- `GioBatDau`: Giờ bắt đầu (HH:MM)
- `GioKetThuc`: Giờ kết thúc (HH:MM)
- `TongTien`: Tổng tiền (số nguyên)
- `TienCoc`: Tiền cọc (số nguyên, 30% tổng tiền)
- `TrangThai`: Trạng thái (DA_DAT, HOAN_THANH, DA_HUY)
- `TrangThaiCoc`: Trạng thái cọc (DA_COC, HOAN_COC, MAT_COC)
- `GhiChu`: Ghi chú

**Ví dụ:**

```csv
MaDatSan,MaKhachHang,MaSan,NgayDat,GioBatDau,GioKetThuc,TongTien,TienCoc,TrangThai,TrangThaiCoc,GhiChu
DS001,KH001,A501,18/11/2025 08:00,08:00,09:00,200000,60000,DA_DAT,DA_COC,Đặt sân buổi sáng
DS003,KH003,A701,18/11/2025 14:00,14:00,15:00,300000,90000,HOAN_THANH,DA_COC,Đã hoàn thành
```

## Cơ Chế Auto-Save

### Tự Động Lưu

Mỗi khi có thao tác thêm/sửa/xóa trên UI, dữ liệu sẽ **TỰ ĐỘNG** được lưu xuống file CSV tương ứng:

- **Thêm khách hàng** → Auto-save `khachhang.csv`
- **Sửa sân** → Auto-save `san.csv`
- **Xóa dịch vụ** → Auto-save `dichvu.csv`
- **Đặt sân mới** → Auto-save `datsan.csv`

### Tự Động Load

- Khi khởi động ứng dụng, tất cả CSV files sẽ được load tự động
- Không cần thao tác thủ công
- Nếu file CSV không tồn tại, hệ thống sẽ tạo file trống

## An Toàn Dữ Liệu

### Build/Rebuild Project

✅ **KHÔNG BỊ MẤT DỮ LIỆU** khi:

- Clean project
- Rebuild project
- Delete build folder
- Recompile code

Dữ liệu CSV nằm ngoài thư mục build (`D:\QT_PBL2\Data\`), không bị ảnh hưởng bởi build process.

### Backup Thủ Công

Để backup dữ liệu, chỉ cần copy toàn bộ thư mục `Data\`:

```
D:\QT_PBL2\Data\
  ├── khachhang.csv
  ├── san.csv
  ├── dichvu.csv
  └── datsan.csv
```

## Lưu Ý Kỹ Thuật

### Encoding

- Tất cả file CSV sử dụng **UTF-8** encoding
- Hỗ trợ tiếng Việt có dấu
- Tự động escape các ký tự đặc biệt (dấu phẩy, dấu ngoặc kép, xuống dòng)

### Format Dữ Liệu

- **Số tiền**: Luôn là số nguyên (VD: 200000 thay vì 200,000 VND)
- **Ngày giờ**:
  - Ngày: `DD/MM/YYYY` (VD: 18/11/2025)
  - Ngày giờ: `DD/MM/YYYY HH:MM` (VD: 18/11/2025 14:30)
  - Giờ: `HH:MM` (VD: 08:00)
- **Boolean**: 1 = true, 0 = false
- **Enum**: Sử dụng tên enum gốc (VD: DO_UONG, SAN_5, DA_DAT)

### Quan Hệ Giữa Các Bảng

- `datsan.csv` → `khachhang.csv` (qua `MaKhachHang`)
- `datsan.csv` → `san.csv` (qua `MaSan`)

Khi load `datsan.csv`, hệ thống tự động resolve các reference này.

## Troubleshooting

### Lỗi "Cannot open CSV file"

- Kiểm tra thư mục `D:\QT_PBL2\Data\` có tồn tại không
- Kiểm tra quyền truy cập thư mục

### Dữ liệu không được lưu

- Kiểm tra console output xem có lỗi ghi file không
- Đảm bảo ứng dụng không bị crash trước khi lưu
- Kiểm tra dung lượng ổ đĩa

### Dữ liệu bị duplicate

- Đừng load CSV thủ công nếu ứng dụng đã auto-load
- Kiểm tra không gọi `docHeThong()` nhiều lần

## Architecture Overview

```
CSVManager (Singleton)
  ↓
QuanLyKhachHang, QuanLySan, QuanLyDichVu, QuanLyDatSan
  ↓
HeThongQuanLy (Coordinator)
  ↓
UI Components (Auto-save on every operation)
```

Mỗi Manager class tự quản lý CSV file của riêng mình, đảm bảo tính độc lập và dễ maintain.
