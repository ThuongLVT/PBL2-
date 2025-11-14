# Hướng Dẫn Sử Dụng Tiếng Việt Có Dấu Trong CSV

## Vấn Đề

File CSV mặc định dùng ASCII, không hiển thị được tiếng Việt có dấu.

## Giải Pháp

### 1. Lưu File CSV với UTF-8 BOM

Khi tạo/chỉnh sửa file CSV trong Excel hoặc text editor:

**Excel:**

1. Mở file CSV
2. File → Save As
3. Chọn "CSV UTF-8 (Comma delimited) (\*.csv)"
4. Save

**VS Code / Notepad++:**

1. Mở file CSV
2. Chọn encoding: UTF-8 with BOM
3. Save

### 2. Ví Dụ File CSV Có Dấu

Xem file mẫu: `Data/san_vidu_codau.csv`

```csv
MaSan,TenSan,LoaiSan,KhuVuc,GiaThue,TrangThai,GhiChu
SB01,Sân A5-1,5,A,200000,Hoạt động,
SB02,Sân A5-2,5,A,200000,Hoạt động,
SB03,Sân A5-3,5,A,200000,Ngừng hoạt động,Chờ nâng cấp
SB04,Sân A7-1,7,A,400000,Hoạt động,
SB05,Sân A7-2,7,A,400000,Hoạt động,Sân đẹp nhất
```

### 3. Lưu Ý

- **Trạng thái** phải khớp với code:

  - `Hoạt động` (không phải "Hoat dong")
  - `Ngừng hoạt động` (không phải "Ngung hoat dong")
  - `Bảo trì` (không phải "Bao tri")

- **Định dạng giá**: Số nguyên không dấu phẩy (vd: 200000, không phải 200,000)

- **Encoding**: UTF-8 with BOM để Windows Console và Qt đọc đúng

### 4. Cập Nhật Data Hiện Tại

Để cập nhật file `san.csv` hiện tại:

1. Mở `Data/san.csv` trong VS Code
2. Click góc dưới bên phải nơi hiển thị encoding
3. Chọn "Save with Encoding" → "UTF-8 with BOM"
4. Thay thế tên không dấu bằng tên có dấu:
   - `San A5-1` → `Sân A5-1`
   - `Hoat dong` → `Hoạt động`
   - `Ngung hoat dong` → `Ngừng hoạt động`
   - `Bao tri` → `Bảo trì`
5. Save file

### 5. Tương Tự Cho Khách Hàng

File `khachhang.csv` cũng làm tương tự:

- Họ tên có thể dùng tiếng Việt: Nguyễn Văn A, Trần Thị B
- Địa chỉ có dấu: Hà Nội, Thành phố Hồ Chí Minh
- Lưu với UTF-8 BOM

## Kiểm Tra

Sau khi cập nhật:

1. Build lại: `cmake --build build --config Debug`
2. Chạy app: `build/bin/FootballFieldManager.exe`
3. Kiểm tra hiển thị tiếng Việt có dấu trong table
