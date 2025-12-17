# Cải Thiện Thiết Kế & UX - Quản Lý Đặt Sân

Tài liệu này tổng hợp các quyết định thiết kế giao diện và trải nghiệm người dùng (UX) cho chức năng **Danh sách đặt sân (Tab 2)** và mối liên hệ với các module khác.

## 1. Vai Trò Của Các Tab (Workflow)

- **Tab 1: Đặt Sân (Booking Creation)**

  - **Nhiệm vụ:** Chỉ dùng để **TẠO MỚI** đơn đặt sân.
  - **Mục tiêu:** Nhập liệu nhanh nhất có thể (Chọn giờ -> Chọn sân -> Điền tên -> Xong).

- **Tab 2: Danh sách Đặt Sân (Operations Center)**

  - **Nhiệm vụ:** Là **TRUNG TÂM ĐIỀU KHIỂN VẬN HÀNH**.
  - **Chức năng:** Xử lý mọi biến động của đơn hàng: Check-in, Thêm dịch vụ (nước, giày), Đổi lịch, Hủy sân, Thanh toán.
  - **Dữ liệu:** Chứa **TẤT CẢ** trạng thái (Đã đặt, Hoàn thành, Đã hủy) để theo dõi lịch sử vận hành (Sân nào trống, sân nào có khách, ai bùng kèo).

- **Module Hóa Đơn (Invoice)**

  - **Nhiệm vụ:** Lưu trữ **LỊCH SỬ TÀI CHÍNH**.
  - **Cơ chế:** Được sinh ra **TỰ ĐỘNG** khi nhân viên bấm "Thanh toán" thành công tại Tab 2.
  - **Lưu ý:** Không dùng tab này để tra cứu lịch sử đặt sân (vì thiếu các đơn hủy không mất tiền).

- **Module Thống Kê (Statistics)**
  - **Nhiệm vụ:** Báo cáo tổng hợp.
  - **Nguồn dữ liệu:** Lấy doanh thu từ Hóa đơn và hiệu suất lấp đầy/hủy từ Danh sách đặt sân.

---

## 2. Thiết Kế Chi Tiết Tab 2 (Danh sách Đặt Sân)

### Dòng 1: Thanh Công Cụ & Bộ Lọc (Toolbar)

Tối ưu cho thao tác nhanh của nhân viên lễ tân.

- **Tìm kiếm:** Đặt đầu tiên. Tìm theo Tên, SĐT, Mã đặt sân.
- **Bộ lọc Thời gian:**
  - `[Hôm nay]` (Nút bấm nhanh - **Mặc định & Active**): Reset về ngày hiện tại. Mặc định khi vào tab danh sách là hôm nay cho tiện làm việc. 
  - `[Ngày mai]` (Nút bấm nhanh): Xem lịch chuẩn bị cho ngày mai.
  - `[Chọn ngày]` (Date Picker): Để tra cứu quá khứ hoặc tương lai xa.
- **Bộ lọc Trạng thái:** Mặc định chọn **"Đang phục vụ"** (chỉ hiện đơn chưa xong) để giao diện gọn gàng.
- **Bộ lọc Sân:** Lọc theo Tên sân hoặc Loại sân (5/7).

```text
+---------------------------------------------------------------------------------------------------------------------------------------+
|  [ 🔍 Tìm kiếm (Tên, SĐT, Mã BK...) ] [ Hôm nay (Active) ] [ Ngày mai ]  [ 📅 13/12/2025 ]    [ 📂 Trạng thái: Đang phục vụ v ]  [ 🏟️ Sân: Tất cả v ]  |
+---------------------------------------------------------------------------------------------------------------------------------------+
```

### Dòng 2: Các Thẻ Trạng Thái (Dashboard Mini)

Hoạt động như các tab con để lọc nhanh danh sách bên dưới.

1.  **Thẻ "TẤT CẢ" (Màu Tím/Trắng)**

    - **Số liệu:** Tổng số booking trong ngày.
    - **Ý nghĩa:** Cái nhìn tổng quan về độ bận rộn của sân.

2.  **Thẻ "ĐANG PHỤC VỤ" (Màu Xanh Dương - Quan trọng nhất)**

    - **Số liệu:** Đếm số đơn `DA_DAT`.
    - **Ý nghĩa:** Danh sách việc cần làm (To-Do List). Khách chưa đến hoặc đang đá, chưa thanh toán.
    - **Hành động:** Check-in, Thêm dịch vụ, Thanh toán.

3.  **Thẻ "HOÀN THÀNH" (Màu Xanh Lá)**

    - **Số liệu:** Đếm số đơn `HOAN_THANH`.
    - **Ý nghĩa:** Lịch sử các đơn đã xong trong ngày.
    - **Hành động:** Xem chi tiết, In lại hóa đơn (Read-only).

4.  **Thẻ "ĐÃ HỦY" (Màu Đỏ)**
    - **Số liệu:** Đếm số đơn `DA_HUY`.
    - **Ý nghĩa:** Các slot trống do hủy.
    - **Hành động:** Xem lý do hủy, trạng thái cọc (Hoàn/Mất).

```text
+-----------------------------+   +-----------------------------+   +-----------------------------+   +-----------------------------+
|  📋 TẤT CẢ (All)            |   |  ⚽ ĐANG PHỤC VỤ (To-Do)    |   |  ✅ HOÀN THÀNH (Done)       |   |  ❌ ĐÃ HỦY (Cancelled)      |
|                             |   |                             |   |                             |   |                             |
|            20               |   |            05               |   |            12               |   |            03               |
|                             |   |                             |   |                             |   |                             |
|  (Tổng lịch trong ngày)     |   |  (Khách chưa thanh toán)    |   |  (Đã thu tiền & xong)       |   |  (Trống sân / Sự cố)        |
+-----------------------------+   +-----------------------------+   +-----------------------------+   +-----------------------------+
      (Màu Tím/Trắng)                   (Màu Xanh Dương)                  (Màu Xanh Lá)                     (Màu Đỏ/Xám)
```

### Bảng Dữ Liệu (Data Table)

- **Hiển thị:** Thay đổi visual dựa trên trạng thái.
  - `DA_DAT`: Chữ đậm, nút thao tác sáng rõ.
  - `HOAN_THANH`: Chữ bình thường/xám nhẹ, khóa sửa đổi.
  - `DA_HUY`: Chữ gạch ngang hoặc đỏ nhạt.
- **Cột hiển thị:** Nên gộp cột để thoáng mắt (Ví dụ: Khách hàng + SĐT chung 1 cột; Sân + Loại sân chung 1 cột).

---

## 3. Các Điểm Cần Lưu Ý (Fixes)

1.  **Không ẩn đơn Hoàn thành/Đã hủy khỏi Tab 2:** Phải giữ lại để tra cứu lịch sử vận hành (Ví dụ: Tra cứu đồ thất lạc, xem lại lịch cũ để đặt lại). Dùng bộ lọc để ẩn bớt chứ không xóa.
2.  **Cơ chế xem Quá khứ:** Cho phép chọn ngày cũ, nhưng **Vô hiệu hóa (Disable)** các nút thao tác sửa đổi (Hủy, Đổi lịch) trên dữ liệu quá khứ.
3.  **Tự động hóa:** Việc thanh toán tại Tab 2 phải tự động đẩy dữ liệu sang Hóa đơn. Nhân viên không nhập liệu thủ công vào tab Hóa đơn.

---
