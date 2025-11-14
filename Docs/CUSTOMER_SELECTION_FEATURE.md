# Tính năng Chọn Khách Hàng - Customer Selection Feature

## Ngày: 15/11/2025
## Tác giả: khninh22 - Service Module

## Tổng quan

Đã thêm tính năng chọn khách hàng với autocomplete tìm kiếm theo số điện thoại vào module Dịch vụ (Service Order Widget).

## Cấu trúc dữ liệu mới

### 1. Trie (Core/ThuatToan/Trie.h)

Cấu trúc dữ liệu Trie để tìm kiếm prefix (tiền tố) nhanh:

```cpp
template <typename T>
class Trie
{
    // Thêm từ: insert(word, data)
    // Tìm chính xác: search(word)
    // Tìm theo prefix: searchByPrefix(prefix)
    // Kiểm tra prefix: startsWith(prefix)
};
```

**Ứng dụng:**
- Tìm kiếm số điện thoại theo prefix (VD: "09" → tất cả SĐT bắt đầu bằng 09)
- Autocomplete khi người dùng gõ
- Thời gian: O(m) với m là độ dài từ

## Giao diện mới

### 1. CustomerSelectionDialog (UI/Dialogs/CustomerSelectionDialog.h/.cpp)

Dialog chọn khách hàng với các tính năng:

**Tìm kiếm:**
- Autocomplete theo số điện thoại
- Sử dụng Trie để tìm kiếm nhanh
- Hiển thị kết quả ngay khi gõ

**Hiển thị:**
- Danh sách: `SĐT - Tên (Hạng)`
- Màu sắc theo hạng:
  - Kim cương: #0891b2 (Cyan)
  - Vàng: #f59e0b (Amber)
  - Bạc: #6b7280 (Gray)
  - Đồng: #92400e (Brown)
  - Thường: Đen

**Thao tác:**
- Double-click để chọn
- Nút "Chọn" để xác nhận
- Nút "Thêm mới" để thêm khách hàng mới
- Nút "Hủy" để đóng dialog

### 2. AddCustomerDialog (UI/Dialogs/AddCustomerDialog.h/.cpp)

Dialog đơn giản để thêm khách hàng nhanh:

**Thông tin cần thiết:**
- Số điện thoại (bắt buộc, 10-11 số)
- Họ tên (bắt buộc)
- Địa chỉ (tùy chọn)

**Validation:**
- Kiểm tra số điện thoại đúng định dạng
- Kiểm tra SĐT đã tồn tại chưa
- Tự động tạo mã khách hàng (KH029, KH030...)
- Đặt ngày đăng ký là hôm nay

**Tự động:**
- Sau khi thêm thành công, tự động chọn khách hàng mới
- Lưu vào CSV ngay lập tức
- Đóng cả 2 dialog và điền thông tin vào form đặt hàng

## Tích hợp vào ServiceOrderWidget

### Cập nhật UI/Pages/Service/ServiceOrderWidget.cpp

**Phương thức mới:**

```cpp
void ServiceOrderWidget::selectCustomer(KhachHang *customer)
{
    // Lưu khách hàng đã chọn
    // Điền thông tin: SĐT, Tên
    // Cập nhật hiển thị thanh toán
    // Tính lại tổng tiền với giảm giá
}

void ServiceOrderWidget::updateCustomerInfo()
{
    // Cập nhật hiển thị: Tên, SĐT, Hạng thành viên
}
```

**Thay đổi slot:**

```cpp
void ServiceOrderWidget::onSelectCustomerClicked()
{
    // Mở CustomerSelectionDialog
    // Nếu chọn khách hàng → gọi selectCustomer()
}
```

## Cách sử dụng

### Từ góc độ người dùng:

1. **Chọn khách hàng có sẵn:**
   - Click nút "Chọn" bên cạnh ô SĐT
   - Gõ vài số đầu của SĐT (VD: "09")
   - Hệ thống gợi ý tất cả SĐT bắt đầu bằng "09"
   - Double-click hoặc chọn và click "Chọn"
   - Thông tin tự động điền vào form

2. **Thêm khách hàng mới:**
   - Click nút "Chọn"
   - Click "➕ Thêm mới"
   - Nhập SĐT, Tên, Địa chỉ
   - Click "Thêm"
   - Khách hàng mới được lưu và tự động chọn

### Từ góc độ kỹ thuật:

```cpp
// Tạo Trie từ danh sách khách hàng
Trie<KhachHang *> phoneTrie;
for (customer in allCustomers) {
    phoneTrie.insert(customer->laySoDienThoai(), customer);
}

// Tìm kiếm khi người dùng gõ
QString searchText = "09";
MangDong<KhachHang *> results = phoneTrie.searchByPrefix(searchText.toStdString());
// Hiển thị results trong QListWidget
```

## Lợi ích

### Hiệu suất:
- **Tìm kiếm nhanh:** O(m) với Trie, thay vì O(n*m) với tìm kiếm tuyến tính
- **Autocomplete tức thì:** Gợi ý ngay khi gõ, không cần chờ đợi

### Trải nghiệm người dùng:
- **Tiện lợi:** Không cần nhớ chính xác SĐT, chỉ cần vài số đầu
- **Nhanh chóng:** Double-click để chọn, không cần nhiều bước
- **Linh hoạt:** Có thể thêm khách hàng mới ngay trong quá trình đặt hàng

### Quản lý:
- **Dữ liệu nhất quán:** Khách hàng mới được lưu vào CSV ngay lập tức
- **Mã tự động:** Không cần lo việc trùng mã khách hàng
- **Validation:** Kiểm tra SĐT trùng, đảm bảo dữ liệu chính xác

## Files thay đổi

### Files mới:
1. `Core/ThuatToan/Trie.h` - Cấu trúc dữ liệu Trie
2. `UI/Dialogs/CustomerSelectionDialog.h/.cpp` - Dialog chọn khách hàng
3. `UI/Dialogs/AddCustomerDialog.h/.cpp` - Dialog thêm khách hàng nhanh

### Files cập nhật:
1. `UI/Pages/Service/ServiceOrderWidget.cpp` - Tích hợp customer selection
2. `CMakeLists.txt` - Thêm files mới vào build

## Testing

Đã test các tình huống:
- ✅ Tìm kiếm theo prefix (09, 08, 03...)
- ✅ Chọn khách hàng từ danh sách
- ✅ Double-click để chọn nhanh
- ✅ Thêm khách hàng mới với validation SĐT
- ✅ Thông tin tự động điền sau khi chọn
- ✅ Giảm giá theo hạng thành viên
- ✅ Lưu CSV sau khi thêm khách hàng mới

## Khả năng mở rộng

Có thể dễ dàng mở rộng:
1. **Tìm kiếm theo tên:** Thêm Trie cho tên khách hàng
2. **Filter theo hạng:** Lọc chỉ hiển thị khách VIP
3. **Lịch sử mua hàng:** Hiển thị tổng chi tiêu trong dialog
4. **Khách hàng thường xuyên:** Đánh dấu top customers

## Notes

- Dialog sử dụng stylesheet global từ `app.qss`
- Trie là template nên có thể dùng cho các mục đích khác
- Customer selection có thể tái sử dụng cho module đặt sân
