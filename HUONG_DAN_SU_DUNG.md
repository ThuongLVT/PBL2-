# 🎯 HƯỚNG DẪN SỬ DỤNG - ĐẶT SÂN BÓNG

## 📝 QUY TRÌNH ĐƠN GIẢN (4 BƯỚC)

### **BƯỚC 1: ĐẶT SÂN**

📍 **Tab 1 - Timeline**

1. **Chọn ngày** (calendar bên trái)
2. **Click vào ô trống** trên lưới (chọn sân + giờ)
3. **Nhập SĐT khách hàng**
   - Nếu SĐT đã có → tự động điền tên
   - Nếu SĐT mới → nhập thêm tên khách
4. **Ghi chú** (tùy chọn)
5. Nhấn **"💾 Lưu"**

➡️ **Kết quả:**

- Ô hiện màu xanh + SĐT
- Đã tự động đặt cọc 30%
- Xuất hiện ở Tab 2

---

### **BƯỚC 2: HỦY LỊCH (nếu cần)**

📍 **Tab 2 - Danh sách đặt sân**

1. **Double-click** vào booking cần hủy
2. Nhấn **"❌ Hủy lịch"**
3. **Chọn 1 trong 2:**
   - **( ) Hoàn cọc** - Khách hủy sớm, lý do hợp lý
   - **( ) Mất cọc** - Khách hủy gấp/bom sân
4. **Ghi lý do hủy** (bắt buộc)
   - VD: "Khách báo trước 1 ngày"
   - VD: "Hủy gần giờ, không kịp cho thuê lại"
   - VD: "Không đến không báo"
5. Nhấn **"Xác nhận"**

➡️ **Kết quả:**

- Trạng thái: **"Đã hủy"** (màu đỏ 🔴)
- Giải phóng slot trên timeline

---

### **BƯỚC 3: THÊM DỊCH VỤ (tùy chọn)**

📍 **Tab 2 - Danh sách đặt sân**

1. **Double-click** vào booking
2. Nhấn **"➕ Thêm dịch vụ"**
3. Chọn dịch vụ (Nước, Khăn, Áo...)
4. Nhập số lượng
5. Nhấn **"Lưu"**

➡️ **Kết quả:**

- Tổng tiền tăng lên
- Số tiền còn lại cập nhật

---

### **BƯỚC 4: THANH TOÁN**

📍 **Tab 2 - Khi khách đá xong**

1. **Double-click** vào booking
2. Xem chi tiết:
   ```
   Tiền sân:       200.000đ
   Dịch vụ:         50.000đ
   ─────────────────────────
   Tổng cộng:      250.000đ
   Đã cọc 30%:    -75.000đ
   ─────────────────────────
   Còn phải thu:   175.000đ
   ```
3. Nhấn **"💰 Thanh toán đủ"**
4. Thu tiền còn lại

➡️ **Kết quả:**

- Trạng thái: **"Hoàn thành"** (màu xanh dương 🔵)
- Hoàn tất booking

---

## 💡 VÍ DỤ THỰC TẾ

### **Tình huống 1: Khách hủy sớm**

```
📞 Khách gọi: "Em có việc đột xuất, hủy giúp em nhé"
👉 Nhân viên:
   - Chọn: [x] Hoàn cọc
   - Lý do: "Khách báo trước, lý do hợp lý"
```

### **Tình huống 2: Khách hủy gấp**

```
📞 Khách gọi: "Tôi không đi được, hủy giúp tôi" (còn 1h nữa đá)
👉 Nhân viên:
   - Chọn: [x] Mất cọc
   - Lý do: "Hủy gấp, không kịp cho thuê lại"
```

### **Tình huống 3: Bom sân**

```
⏰ Đến giờ, khách không đến, không nghe máy
👉 Nhân viên:
   - Chọn: [x] Mất cọc
   - Lý do: "Không đến không báo (bom sân)"
```

---

## 🎨 MÀU SẮC TRẠNG THÁI

| Màu           | Trạng thái     | Ý nghĩa                    |
| ------------- | -------------- | -------------------------- |
| 🟢 Xanh lá    | **Đã đặt**     | Đã cọc 30%, chờ thanh toán |
| 🔵 Xanh dương | **Hoàn thành** | Đã thanh toán đủ 100%      |
| 🔴 Đỏ         | **Đã hủy**     | Đã hủy (có ghi lý do)      |

💡 **Lưu ý:** Khi xem chi tiết booking đã hủy, sẽ thấy:

- "Đã hủy (Hoàn cọc)" - nếu nhân viên chọn hoàn
- "Đã hủy (Mất cọc)" - nếu nhân viên chọn giữ cọc

---

## 🔍 TÌM KIẾM & LỌC (Tab 2)

### **Tìm kiếm nhanh:**

- Gõ **SĐT** khách hàng
- Gõ **Tên** khách hàng
- Gõ **Mã đặt sân**

### **Lọc theo:**

- **Sân:** Tất cả / Sân A5-1 / Sân B7-1...
- **Trạng thái:** Tất cả / Đã đặt / Hoàn thành / Đã hủy
- **Khoảng thời gian:** Từ ngày → Đến ngày

---

## 🆘 CÂU HỎI THƯỜNG GẶP

**❓ Khách muốn đổi giờ?**

- Tạo booking mới cho giờ mới
- Hủy booking cũ (chọn hoàn/mất cọc tùy tình huống)

**❓ Khách không đến?**

- Chờ quá giờ → Hủy lịch
- Chọn "Mất cọc" + Ghi lý do "Không đến không báo"

**❓ Sửa thông tin booking?**

- Double-click booking → Sửa ghi chú
- **Lưu ý:** Không sửa được sân, giờ, khách hàng (phải hủy tạo mới)

**❓ Khách hỏi "Hủy có mất tiền không?"**

- Nhân viên tự xét: hủy sớm → hoàn, hủy gấp → mất

---

## 📊 THỐNG KÊ (Tab 2)

### **4 Cards thống kê:**

- **Tổng đơn:** Tất cả bookings trong khoảng thời gian
- **Đã đặt:** Bookings đang chờ thanh toán
- **Hoàn thành:** Bookings đã thanh toán đủ
- **Tổng doanh thu:** Tổng tiền từ các booking hoàn thành

---

## ✅ CHECKLIST NHÂN VIÊN

### **📌 Khi nhận đặt sân (Tab 1):**

- [ ] Chọn đúng ngày, sân, giờ
- [ ] Nhập đúng SĐT (10 số)
- [ ] Kiểm tra tên khách tự động điền
- [ ] Nhấn "Lưu" → Xác nhận ô hiện màu

### **📌 Khi khách hủy:**

- [ ] Xét tình huống: hủy sớm hay hủy gấp
- [ ] Chọn đúng: Hoàn cọc / Mất cọc
- [ ] Ghi rõ lý do hủy

### **📌 Khi khách đến:**

- [ ] Hỏi có cần dịch vụ gì không
- [ ] Thêm dịch vụ vào booking (nếu có)

### **📌 Khi khách đá xong:**

- [ ] Kiểm tra lại dịch vụ đã dùng
- [ ] Xem tổng tiền (đã trừ cọc 30%)
- [ ] Thu tiền còn lại
- [ ] Nhấn "Thanh toán đủ"

---

---

## 🎯 TÓM TẮT QUY TRÌNH

```
1️⃣ ĐẶT SÂN (Tab 1)
   ↓ Chọn ô → Nhập SĐT → Lưu

2️⃣ HỦY LỊCH (nếu cần - Tab 2)
   ↓ Chọn Hoàn/Mất cọc → Ghi lý do

3️⃣ THÊM DỊCH VỤ (tùy chọn - Tab 2)
   ↓ Nước, Khăn, Áo...

4️⃣ THANH TOÁN (Tab 2)
   ↓ Thu tiền còn lại (đã trừ cọc 30%)
```

**Đơn giản vậy thôi! 🎉**

---

## 📝 LƯU Ý QUAN TRỌNG

✅ **Tất cả booking đều tự động đặt cọc 30% khi tạo**
✅ **Không cần xác nhận, không cần check-in**
✅ **Hủy lịch: Nhân viên tự quyết định hoàn/mất cọc**
✅ **Bắt buộc ghi lý do khi hủy → dễ tra cứu sau**

---

## 🔄 LỊCH SỬ CẬP NHẬT

**Version:** 3.0 - Ultra Simplified
**Ngày:** 18/11/2025
**Thay đổi:**

- ✅ Giảm từ 4 bước xuống 4 bước thực sự đơn giản
- ✅ Bỏ "Xác nhận & Đặt cọc" (tự động cọc khi tạo)
- ✅ Bỏ "Check-in" (không cần thiết)
- ✅ Hủy lịch linh hoạt (nhân viên tự xét)
