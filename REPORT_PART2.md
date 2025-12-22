# 2. PHÂN TÍCH VÀ THIẾT KẾ HỆ THỐNG

## 2.1. Yêu cầu chức năng
Hệ thống cần đáp ứng các nhóm chức năng chính sau:

### 2.1.1. Quản lý Sân bóng
*   Hiển thị danh sách sân và trạng thái hiện tại (Trống, Đang đá, Đang bảo trì).
*   Cập nhật thông tin sân (Loại sân 5/7 người, Giá tiền mỗi giờ).

### 2.1.2. Quản lý Đặt sân (Booking)
*   Đặt sân mới cho khách hàng (ghi nhận giờ bắt đầu, giờ kết thúc dự kiến).
*   Check-in (Nhận sân) và Check-out (Thanh toán).
*   Tự động tính tiền sân dựa trên thời gian thực tế sử dụng.

### 2.1.3. Quản lý Dịch vụ
*   Quản lý danh mục đồ uống, thức ăn, dụng cụ cho thuê.
*   Thêm dịch vụ vào hóa đơn đặt sân (Ví dụ: Khách gọi thêm 2 chai nước).
*   Quản lý tồn kho cơ bản.

### 2.1.4. Quản lý Khách hàng
*   Lưu trữ thông tin khách hàng (Tên, SĐT).
*   Phân loại khách hàng (Thường, VIP) dựa trên chi tiêu để áp dụng giảm giá.

### 2.1.5. Thống kê & Báo cáo
*   Thống kê doanh thu theo ngày, tháng.
*   Thống kê tần suất sử dụng của từng sân.

## 2.2. Yêu cầu phi chức năng
*   **Giao diện:** Trực quan, dễ sử dụng, thao tác nhanh (vì nhân viên thu ngân cần tốc độ).
*   **Lưu trữ:** Dữ liệu phải được lưu xuống file (CSV/Binary) để không bị mất khi tắt chương trình.
*   **Hiệu năng:** Tìm kiếm khách hàng và xử lý thanh toán phải phản hồi tức thì (< 1 giây).

## 2.3. Biểu đồ Use Case (Sơ lược)

> **[HÌNH ẢNH 2.1]:** *Bạn hãy vẽ biểu đồ Use Case tổng quát bao gồm các Actor: Admin (Quản trị viên) và Staff (Nhân viên).*
> *   **Admin:** Quản lý nhân viên, Thống kê doanh thu, Cấu hình giá sân.
> *   **Staff:** Đặt sân, Thanh toán, Bán dịch vụ, Đăng ký khách hàng thành viên.
>
> *Chú thích:* Hình 2.1. Biểu đồ Use Case tổng quát của hệ thống.
