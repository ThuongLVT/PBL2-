# 📋 LOGIC ĐẶT SÂN ĐƠN GIẢN HÓA

## 🎯 MỤC TIÊU

- Đơn giản hóa quy trình đặt sân
- Dễ sử dụng cho nhân viên
- Ít bước, ít lỗi

---

## 🔄 QUY TRÌNH MỚI (3 BƯỚC ĐƠN GIẢN)

### **BƯỚC 1: ĐẶT SÂN + ĐẶT CỌC 30% (Tab 1 - Timeline)**

```
User: Chọn ô trống → Nhập SĐT KH → Nhấn "Lưu"
System:
  ✓ Tạo booking với trạng thái "ĐÃ ĐẶT"
  ✓ Tự động tính tiền cọc 30%
  ✓ Đánh dấu đã đặt cọc
  ✓ Hiện màu + SĐT ở ô ngay lập tức
  ✓ Sync sang Tab 2

Lưu ý: Mặc định mọi booking đều đã cọc 30% khi tạo
```

### **BƯỚC 2: THÊM DỊCH VỤ (Tuỳ chọn - Tab 2 - Dialog)**

```
User: Double-click booking → Nhấn "Thêm dịch vụ"
System:
  ✓ Chọn dịch vụ (Nước, khăn, áo...)
  ✓ Tự động cộng vào tổng tiền
  ✓ Cập nhật số tiền còn lại

Lưu ý: Có thể thêm dịch vụ bất cứ lúc nào trước khi thanh toán
```

### **BƯỚC 3: THANH TOÁN ĐỦ (Khi khách đá xong)**

```
User: Double-click booking → Nhấn "Thanh toán đủ"
System:
  ✓ Tính tổng: (Tiền sân + Dịch vụ) - Tiền cọc 30%
  ✓ Thu số tiền còn lại (70% + dịch vụ)
  ✓ Chuyển trạng thái → "HOÀN THÀNH"
  ✓ Tính vào doanh thu
```

---

## ❌ HỦY LỊCH - ĐỂ NHÂN VIÊN QUYẾT ĐỊNH

### **Quy trình hủy đơn giản:**

```
User: Double-click booking → Nhấn "Hủy đặt sân"
System:
  ✓ Hiện dialog xác nhận hủy
  ✓ Cho phép chọn:
      [ ] Hoàn cọc 100% (khách hủy sớm, lý do hợp lý)
      [ ] Mất cọc 100% (khách hủy gấp, bom sân)
  ✓ Bắt buộc ghi "Lý do hủy": [_______________]
  ✓ Nhấn [Xác nhận hủy]
  ✓ Giải phóng slot trên timeline
  ✓ Cập nhật trạng thái:
      - Nếu hoàn cọc → "ĐÃ HỦY (HOÀN CỌC)"
      - Nếu mất cọc → "ĐÃ HỦY (MẤT CỌC)"
```

### **Ưu điểm:**

- ✅ **Linh hoạt:** Nhân viên tự xét từng trường hợp
- ✅ **Rõ ràng:** Bắt buộc ghi lý do → dễ tra cứu sau
- ✅ **Đơn giản:** Không cần tính toán 3h, không cần code logic phức tạp
- ✅ **Thực tế:** Phù hợp với cách quản lý thủ công của sân bóng mini

---

## 🎨 UI DIALOG - 3 BUTTONS ĐƠN GIẢN

### **Button hiển thị theo trạng thái:**

| Trạng thái              | Buttons hiển thị                                             |
| ----------------------- | ------------------------------------------------------------ |
| **ĐÃ ĐẶT** (đã cọc 30%) | [➕ Thêm dịch vụ] [❌ Hủy lịch] [💰 Thanh toán đủ] [🚪 Đóng] |
| **HOÀN THÀNH**          | [📄 In hóa đơn] [🚪 Đóng]                                    |
| **ĐÃ HỦY**              | [📄 Xem lý do hủy] [🚪 Đóng]                                 |

### **Dialog Hủy lịch:**

```
┌────────────────────────────────────┐
│  ❌ HỦY ĐẶT SÂN                    │
├────────────────────────────────────┤
│  Mã: DS_20251118_001               │
│  Khách: Nguyễn Văn A               │
│  Sân: Sân A5-1                     │
│  Ngày: 20/11/2025  14:00-15:00    │
│  Tiền cọc: 60.000 VND              │
│                                    │
│  Chọn xử lý tiền cọc:              │
│  ( ) Hoàn cọc 100%                 │
│      (Khách hủy sớm/lý do hợp lý)  │
│  ( ) Mất cọc 100%                  │
│      (Khách hủy gấp/bom sân)       │
│                                    │
│  Lý do hủy (bắt buộc):             │
│  [_____________________________]  │
│  [_____________________________]  │
│                                    │
│  [HỦY BỎ]        [XÁC NHẬN HỦY]   │
└────────────────────────────────────┘
```

---

## 🔧 CẦN THAY ĐỔI TRONG CODE

### **1. Update Enum TrangThaiDatSan (DatSan.h):**

```cpp
// BỎ 2 trạng thái: CHO_XAC_NHAN, DANG_SU_DUNG
enum TrangThaiDatSan
{
    DA_DAT,        // Đã đặt (đã cọc 30%, chờ thanh toán đủ)
    HOAN_THANH,    // Hoàn thành (đã thanh toán 100%)
    DA_HUY         // Đã hủy (có 2 loại: hoàn/mất cọc)
};
```

### **2. Thêm field "Lý do hủy" (DatSan.h):**

```cpp
class DatSan
{
private:
    std::string lyDoHuy;      // Lý do hủy (bắt buộc khi hủy)
    bool hoanCoc;             // true = hoàn cọc, false = mất cọc

public:
    std::string getLyDoHuy() const;
    void setLyDoHuy(const std::string& lyDo);
    bool isHoanCoc() const;
    void setHoanCoc(bool hoan);
};
```

### **3. Cập nhật Dialog Hủy (BookingDetailDialog.cpp):**

```cpp
void BookingDetailDialog::onCancelBookingClicked()
{
    // Hiện dialog:
    // - Radio button: ( ) Hoàn cọc  ( ) Mất cọc
    // - TextEdit: Lý do hủy (bắt buộc)

    if (lyDoHuy.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng ghi lý do hủy!");
        return;
    }

    booking->setTrangThai(DA_HUY);
    booking->setHoanCoc(hoanCocRadio->isChecked());
    booking->setLyDoHuy(lyDoHuy);

    if (booking->isHoanCoc()) {
        booking->setTrangThaiCoc(HOAN_COC);
        // Không tính vào doanh thu
    } else {
        booking->setTrangThaiCoc(MAT_COC);
        // Tính 30% vào doanh thu
    }
}
```

### **4. Đơn giản hóa Timeline Tab (TimelineTab.cpp):**

```cpp
// Khi tạo booking mới:
void TimelineTab::onSaveBookingClicked()
{
    // Mặc định:
    booking->setTrangThai(DA_DAT);
    booking->setTrangThaiCoc(DA_COC);  // Đã cọc 30%
    booking->tinhTienCoc();            // Auto tính 30%

    system->save();

    // Refresh
    timelineGrid->clearPendingSelection();
    timelineGrid->setDate(selectedDate);
    timelineGrid->loadBookings();
    emit bookingDataChanged();
}
```

---

## 📊 TRẠNG THÁI & MÀU SẮC (3 TRẠNG THÁI)

```
ĐÃ ĐẶT (đã cọc 30%)   🟢 Green    #16a34a  (Chờ thanh toán đủ)
HOÀN THÀNH            🔵 Blue     #3b82f6  (Đã thanh toán 100%)
ĐÃ HỦY                🔴 Red      #dc2626  (Đã hủy - ghi lý do)
```

### **Sub-state cho ĐÃ HỦY:**

```
ĐÃ HỦY (HOÀN CỌC)  🟠 Orange  #f97316  (Đã hoàn 30%)
ĐÃ HỦY (MẤT CỌC)   🔴 Red     #dc2626  (Giữ 30%)
```

---

## 💡 LỢI ÍCH

✅ **Cho nhân viên:**

- Ít bước hơn
- Rõ ràng hơn
- Ít lỗi hơn

✅ **Cho hệ thống:**

- Logic đơn giản
- Dễ maintain
- Ít bug

✅ **Cho khách hàng:**

- Quy trình rõ ràng
- Biết trước chi phí
- Chính sách cọc minh bạch

---

## 🚀 TRIỂN KHAI

### **Priority 1: Fix hiển thị (URGENT)**

- [ ] Fix Timeline refresh sau khi đặt
- [ ] Fix sync data sang Tab 2
- [ ] Test flow cơ bản

### **Priority 2: Đơn giản hóa UI**

- [ ] Redesign Dialog buttons
- [ ] Show/hide theo trạng thái
- [ ] Update thông báo messages

### **Priority 3: Polish**

- [ ] Update màu sắc timeline
- [ ] Improve UX messages
- [ ] Add tooltips/hints
