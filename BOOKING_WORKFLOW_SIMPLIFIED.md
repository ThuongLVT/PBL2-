# 📋 WORKFLOW BOOKING SIÊU ĐỠN GIẢN

> **Phân tích nghiệp vụ (BA) cho hệ thống đặt sân bóng mini**
>
> **Version:** 3.0 - Ultra Simplified  
> **Ngày:** 18/11/2025

---

## 🎯 NGUYÊN TẮC THIẾT KẾ

✅ **Đơn giản nhất có thể**  
✅ **Ít bước nhất**  
✅ **Nhân viên mới dùng được ngay**  
✅ **Không cần training phức tạp**

---

## 🔄 QUY TRÌNH 4 BƯỚC

### **1️⃣ ĐẶT SÂN** (Tab 1 - Timeline)

**Input:**

- Ngày (chọn từ calendar)
- Sân + Giờ (click vào ô trống)
- SĐT khách hàng (nhập)
- Tên khách (tự động fill nếu đã có, nhập nếu mới)
- Ghi chú (tùy chọn)

**Process:**

```
1. User click ô trống trên timeline
2. Panel hiển thị form với thông tin auto-fill:
   - Sân: [auto]
   - Ngày: [auto]
   - Giờ: [auto]
   - Giá sân: [auto calculate]
3. User nhập SĐT
4. System check SĐT:
   - Nếu đã tồn tại → fill Tên khách
   - Nếu mới → cho phép nhập Tên
5. System tự động tính:
   - Tiền cọc = Giá sân × 30%
6. User nhấn [Lưu]
```

**Output:**

```
DatSan {
  trangThai: DA_DAT
  trangThaiCoc: DA_COC
  tienCoc: giaSan * 0.3
  soTienConLai: giaSan * 0.7
  tongTien: giaSan
}
```

**UI Effect:**

- Ô trên timeline hiện màu xanh lá 🟢
- Hiển thị SĐT ở giữa ô
- Booking xuất hiện ở Tab 2

---

### **2️⃣ HỦY LỊCH** (Tab 2 - Nếu cần)

**Input:**

- Lựa chọn: Hoàn cọc / Mất cọc
- Lý do hủy (bắt buộc)

**Process:**

```
1. User double-click booking ở Tab 2
2. User nhấn [Hủy lịch]
3. Dialog hiển thị:
   ┌─────────────────────────────┐
   │ Chọn xử lý tiền cọc:        │
   │ ( ) Hoàn cọc 100%           │
   │     (Khách hủy sớm)         │
   │ ( ) Mất cọc 100%            │
   │     (Khách hủy gấp/bom sân) │
   │                             │
   │ Lý do hủy (bắt buộc):       │
   │ [__________________________]│
   │                             │
   │ [Hủy bỏ]  [Xác nhận hủy]    │
   └─────────────────────────────┘
4. User chọn 1 trong 2 + nhập lý do
5. User nhấn [Xác nhận hủy]
```

**Output:**

```
DatSan {
  trangThai: DA_HUY
  lyDoHuy: "..."
  hoanCoc: true/false

  // Nếu hoàn cọc:
  trangThaiCoc: HOAN_COC
  doanhThu: 0 (không tính)

  // Nếu mất cọc:
  trangThaiCoc: MAT_COC
  doanhThu: tienCoc (30%)
}
```

**UI Effect:**

- Trạng thái: "Đã hủy" (màu đỏ 🔴)
- Slot trên timeline được giải phóng (trở về trống)
- Lý do hủy được lưu → tra cứu sau

---

### **3️⃣ THÊM DỊCH VỤ** (Tab 2 - Tùy chọn)

**Input:**

- Dịch vụ (chọn từ dropdown)
- Số lượng (nhập)

**Process:**

```
1. User double-click booking
2. User nhấn [Thêm dịch vụ]
3. Dialog thêm dịch vụ:
   - Chọn dịch vụ: [Nước ▼]
   - Số lượng: [2]
   - Đơn giá: 10.000đ (auto)
   - Thành tiền: 20.000đ (auto)
4. User nhấn [Lưu]
```

**Output:**

```
DatSan {
  danhSachDichVu: [..., newService]
  tongTien: giaSan + sum(dichVu)
  soTienConLai: tongTien - tienCoc
}
```

**UI Effect:**

- Tổng tiền cập nhật
- Số tiền còn lại cập nhật

---

### **4️⃣ THANH TOÁN** (Tab 2 - Khi khách đá xong)

**Input:**

- Xác nhận thanh toán

**Process:**

```
1. User double-click booking
2. Dialog hiển thị chi tiết:
   ┌──────────────────────────┐
   │ THANH TOÁN               │
   ├──────────────────────────┤
   │ Tiền sân:    200.000đ    │
   │ Dịch vụ:      50.000đ    │
   │ ─────────────────────    │
   │ Tổng cộng:   250.000đ    │
   │ Đã cọc 30%: -75.000đ     │
   │ ─────────────────────    │
   │ CÒN PHẢI THU: 175.000đ   │
   │                          │
   │ [Đóng]  [Thanh toán đủ]  │
   └──────────────────────────┘
3. User nhấn [Thanh toán đủ]
```

**Output:**

```
DatSan {
  trangThai: HOAN_THANH
  soTienConLai: 0
  ngayThanhToan: now()
  doanhThu: tongTien (100%)
}
```

**UI Effect:**

- Trạng thái: "Hoàn thành" (màu xanh dương 🔵)
- Không thể chỉnh sửa nữa

---

## 📊 STATE MACHINE (Sơ đồ chuyển trạng thái)

```
                [TẠO BOOKING]
                      ↓
                ┌──────────┐
                │ ĐÃ ĐẶT   │ (đã cọc 30%)
                │ (DA_DAT) │
                └────┬─────┘
                     │
         ┌───────────┼───────────┐
         │           │           │
    [Hủy lịch]  [Thêm DV]  [Thanh toán]
         │           │           │
         ↓           ↓           ↓
   ┌─────────┐  ┌────────┐  ┌──────────┐
   │ ĐÃ HỦY  │  │ ĐÃ ĐẶT │  │ HOÀN     │
   │ (DA_HUY)│  │+ dịch vụ│  │ THÀNH    │
   └─────────┘  └────┬───┘  └──────────┘
                     │
                     ↓
               [Thanh toán]
                     ↓
               ┌──────────┐
               │ HOÀN     │
               │ THÀNH    │
               └──────────┘
```

### **Quy tắc chuyển trạng thái:**

| Từ trạng thái  | Hành động     | Sang trạng thái        |
| -------------- | ------------- | ---------------------- |
| **ĐÃ ĐẶT**     | Hủy lịch      | **ĐÃ HỦY**             |
| **ĐÃ ĐẶT**     | Thêm dịch vụ  | **ĐÃ ĐẶT** (không đổi) |
| **ĐÃ ĐẶT**     | Thanh toán đủ | **HOÀN THÀNH**         |
| **HOÀN THÀNH** | -             | (terminal state)       |
| **ĐÃ HỦY**     | -             | (terminal state)       |

---

## 🗃️ DATA MODEL

### **DatSan (Core Model)**

```cpp
class DatSan {
private:
    // Core fields
    std::string maDatSan;          // DS_YYYYMMDD_XXX
    KhachHang* khachHang;
    San* san;
    NgayGio thoiGianDat;
    KhungGio khungGio;

    // Money fields
    double tongTien;               // Giá sân + dịch vụ
    double tienCoc;                // 30% × tongTien
    double soTienConLai;           // 70% × tongTien + dịch vụ

    // Status fields
    TrangThaiDatSan trangThai;     // DA_DAT, HOAN_THANH, DA_HUY
    TrangThaiCoc trangThaiCoc;     // DA_COC, HOAN_COC, MAT_COC

    // Cancel fields
    std::string lyDoHuy;           // Lý do hủy (bắt buộc khi hủy)
    bool hoanCoc;                  // true = hoàn, false = mất

    // Services
    MangDong<DichVuDat> danhSachDichVu;

    // Metadata
    NgayGio ngayTao;
    NgayGio ngayDatCoc;
    std::string ghiChu;
};
```

### **Enum Definitions**

```cpp
enum TrangThaiDatSan {
    DA_DAT,        // Đã đặt (đã cọc 30%)
    HOAN_THANH,    // Hoàn thành (đã thanh toán đủ)
    DA_HUY         // Đã hủy (có lý do)
};

enum TrangThaiCoc {
    DA_COC,        // Đã đặt cọc 30% (mặc định khi tạo)
    HOAN_COC,      // Đã hoàn cọc (hủy lịch - chọn hoàn)
    MAT_COC        // Mất cọc (hủy lịch - chọn mất)
};
```

---

## 🎨 UI SPECIFICATIONS

### **Tab 1 - Timeline**

**Panel đặt sân (bên phải):**

```
┌────────────────────────────────────┐
│  📝 THÔNG TIN ĐẶT SÂN              │
├────────────────────────────────────┤
│  📞 SĐT: [___________] *           │
│  👤 Tên: [Auto-fill/Nhập]          │
│                                    │
│  📅 Ngày: 20/11/2025 (auto)        │
│  ⏰ Giờ:  14:00 - 15:00 (auto)     │
│  ⚽ Sân:  Sân A5-1 (auto)           │
│                                    │
│  💰 Giá sân:    200.000đ (auto)    │
│  💵 Cọc 30%:     60.000đ (auto)    │
│                                    │
│  📝 Ghi chú: [______________]      │
│                                    │
│       [HỦY]          [💾 LƯU]      │
└────────────────────────────────────┘
```

**Lưu ý:**

- CHỈ 2 trường bắt buộc: SĐT + Tên (nếu KH mới)
- Tất cả thông tin khác AUTO
- Ghi chú tùy chọn

---

### **Tab 2 - Danh sách**

**Table columns (10 cột):**

| #   | Cột            | Ý nghĩa                      |
| --- | -------------- | ---------------------------- |
| 1   | Mã đặt sân     | DS_YYYYMMDD_XXX              |
| 2   | Khách hàng     | Tên                          |
| 3   | SĐT            | 10 số                        |
| 4   | Sân            | Tên sân                      |
| 5   | Ngày đặt       | DD/MM/YYYY                   |
| 6   | Giờ            | HH:mm - HH:mm                |
| 7   | Tổng tiền      | Tiền sân + dịch vụ           |
| 8   | Tiền cọc       | 30% × Giá sân                |
| 9   | Trạng thái cọc | Đã cọc / Hoàn cọc / Mất cọc  |
| 10  | Trạng thái     | Đã đặt / Hoàn thành / Đã hủy |

---

### **Dialog Chi tiết Booking**

**Buttons hiển thị theo trạng thái:**

```cpp
if (trangThai == DA_DAT) {
    showButtons([
        "➕ Thêm dịch vụ",
        "❌ Hủy lịch",
        "💰 Thanh toán đủ",
        "🚪 Đóng"
    ]);
}
else if (trangThai == HOAN_THANH) {
    showButtons([
        "📄 In hóa đơn",
        "🚪 Đóng"
    ]);
}
else if (trangThai == DA_HUY) {
    showButtons([
        "📄 Xem lý do hủy",
        "🚪 Đóng"
    ]);
}
```

---

### **Dialog Hủy lịch**

```
┌──────────────────────────────────────┐
│  ❌ HỦY ĐẶT SÂN                      │
├──────────────────────────────────────┤
│  Mã: DS_20251118_001                 │
│  Khách: Nguyễn Văn A                 │
│  Sân: Sân A5-1                       │
│  Ngày: 20/11/2025  14:00-15:00       │
│  Tiền cọc: 60.000đ                   │
│                                      │
│  ⚙️ Xử lý tiền cọc:                  │
│  ( ) Hoàn cọc 100%                   │
│      ↳ Khách hủy sớm, lý do hợp lý   │
│  ( ) Mất cọc 100%                    │
│      ↳ Khách hủy gấp/bom sân         │
│                                      │
│  📝 Lý do hủy (bắt buộc):            │
│  ┌────────────────────────────────┐  │
│  │                                │  │
│  │                                │  │
│  └────────────────────────────────┘  │
│                                      │
│  [HỦY BỎ]           [XÁC NHẬN HỦY]   │
└──────────────────────────────────────┘
```

---

## 💾 BUSINESS LOGIC

### **Function: tạoBooking()**

```cpp
DatSan* taoBooking(San* san, NgayGio thoiGian, KhachHang* kh) {
    DatSan* booking = new DatSan();

    // Auto-generate
    booking->setMaDatSan(DatSan::taoMaDatSan());
    booking->setSan(san);
    booking->setThoiGianDat(thoiGian);
    booking->setKhachHang(kh);

    // Auto-calculate
    double giaSan = san->tinhGiaSan(thoiGian);
    booking->setTongTien(giaSan);
    booking->setTienCoc(giaSan * 0.3);
    booking->setSoTienConLai(giaSan * 0.7);

    // Auto-set status
    booking->setTrangThai(DA_DAT);
    booking->setTrangThaiCoc(DA_COC);
    booking->setNgayDatCoc(NgayGio::now());

    return booking;
}
```

---

### **Function: huyBooking()**

```cpp
void huyBooking(DatSan* booking, bool hoanCoc, string lyDo) {
    // Validate
    if (lyDo.empty()) {
        throw Exception("Bắt buộc ghi lý do hủy!");
    }

    // Update status
    booking->setTrangThai(DA_HUY);
    booking->setLyDoHuy(lyDo);
    booking->setHoanCoc(hoanCoc);

    if (hoanCoc) {
        // Hoàn cọc
        booking->setTrangThaiCoc(HOAN_COC);
        booking->setSoTienConLai(0);
        // KHÔNG tính vào doanh thu
    } else {
        // Mất cọc
        booking->setTrangThaiCoc(MAT_COC);
        booking->setSoTienConLai(0);
        // TÍNH 30% vào doanh thu
        congDoanhThu(booking->getTienCoc());
    }

    // Giải phóng slot
    giaiPhongSlot(booking->getSan(), booking->getThoiGianDat());
}
```

---

### **Function: themDichVu()**

```cpp
void themDichVu(DatSan* booking, DichVu* dv, int soLuong) {
    // Validate
    if (booking->getTrangThai() != DA_DAT) {
        throw Exception("Chỉ thêm dịch vụ cho booking đang đặt!");
    }

    // Create service record
    DichVuDat* dvDat = new DichVuDat();
    dvDat->setDichVu(dv);
    dvDat->setSoLuong(soLuong);
    dvDat->setThanhTien(dv->getDonGia() * soLuong);

    // Add to booking
    booking->themDichVu(*dvDat);

    // Recalculate
    booking->tinhTongTien();  // tongTien = giaSan + sum(dichVu)
    booking->setSoTienConLai(booking->getTongTien() - booking->getTienCoc());
}
```

---

### **Function: thanhToanDu()**

```cpp
void thanhToanDu(DatSan* booking) {
    // Validate
    if (booking->getTrangThai() != DA_DAT) {
        throw Exception("Chỉ thanh toán được booking đang đặt!");
    }

    // Update status
    booking->setTrangThai(HOAN_THANH);
    booking->setSoTienConLai(0);
    booking->setNgayThanhToan(NgayGio::now());

    // Tính vào doanh thu (100%)
    congDoanhThu(booking->getTongTien());

    // Log
    log("Thanh toán: " + booking->getMaDatSan() +
        " - " + formatMoney(booking->getTongTien()));
}
```

---

## ✅ VALIDATION RULES

### **Khi tạo booking:**

- [x] SĐT bắt buộc (10 số)
- [x] Tên khách bắt buộc
- [x] Sân phải trống tại thời điểm đặt
- [x] Giờ bắt đầu < Giờ kết thúc
- [x] Không được đặt quá khứ

### **Khi hủy booking:**

- [x] Phải chọn hoàn/mất cọc
- [x] Lý do hủy bắt buộc (min 10 ký tự)
- [x] Chỉ hủy được booking "Đã đặt"

### **Khi thêm dịch vụ:**

- [x] Số lượng > 0
- [x] Chỉ thêm được cho booking "Đã đặt"

### **Khi thanh toán:**

- [x] Chỉ thanh toán được booking "Đã đặt"
- [x] Phải xác nhận số tiền

---

## 📈 DOANH THU CALCULATION

```cpp
double tinhDoanhThu(NgayThang tuNgay, NgayThang denNgay) {
    double total = 0;

    for (DatSan* booking : getAllBookings(tuNgay, denNgay)) {
        if (booking->getTrangThai() == HOAN_THANH) {
            // Booking hoàn thành: tính 100%
            total += booking->getTongTien();
        }
        else if (booking->getTrangThai() == DA_HUY &&
                 !booking->isHoanCoc()) {
            // Booking hủy mất cọc: tính 30%
            total += booking->getTienCoc();
        }
    }

    return total;
}
```

---

## 🧪 TEST CASES

### **TC-01: Tạo booking thành công**

```
Given: Sân A5-1 trống lúc 14:00 ngày 20/11
When: User đặt sân với SĐT "0909123456"
Then:
  - Booking được tạo với trạng thái "Đã đặt"
  - Tiền cọc = 30% giá sân
  - Ô timeline hiện màu xanh + SĐT
  - Booking xuất hiện ở Tab 2
```

### **TC-02: Hủy booking - Hoàn cọc**

```
Given: Booking "DS_20251120_001" đang ở trạng thái "Đã đặt"
When: User hủy, chọn "Hoàn cọc", ghi lý do "Khách báo trước 1 ngày"
Then:
  - Trạng thái: "Đã hủy (Hoàn cọc)"
  - Slot timeline được giải phóng
  - Không tính vào doanh thu
```

### **TC-03: Hủy booking - Mất cọc**

```
Given: Booking "DS_20251120_002" đang ở trạng thái "Đã đặt"
When: User hủy, chọn "Mất cọc", ghi lý do "Không đến không báo"
Then:
  - Trạng thái: "Đã hủy (Mất cọc)"
  - Slot timeline được giải phóng
  - Tính 30% vào doanh thu
```

### **TC-04: Thêm dịch vụ**

```
Given: Booking "DS_20251120_003" trạng thái "Đã đặt"
When: User thêm "Nước suối" × 2 (10.000đ/chai)
Then:
  - Tổng tiền tăng 20.000đ
  - Số tiền còn lại tăng 20.000đ
  - Trạng thái vẫn là "Đã đặt"
```

### **TC-05: Thanh toán đủ**

```
Given: Booking "DS_20251120_004" trạng thái "Đã đặt"
      Tổng tiền: 250.000đ, Đã cọc: 75.000đ
When: User nhấn "Thanh toán đủ"
Then:
  - Thu tiền: 175.000đ
  - Trạng thái: "Hoàn thành"
  - Tính 250.000đ vào doanh thu
```

---

## 📊 REPORTING

### **Báo cáo cần có:**

1. **Doanh thu theo ngày/tuần/tháng**

   - Tổng booking hoàn thành
   - Tổng tiền cọc bị mất
   - Tổng doanh thu

2. **Thống kê hủy lịch**

   - Số booking hủy hoàn cọc
   - Số booking hủy mất cọc
   - Top lý do hủy

3. **Thống kê khách hàng**

   - Khách hàng trung thành (nhiều booking)
   - Khách hàng bom sân (mất cọc)

4. **Thống kê dịch vụ**
   - Dịch vụ được dùng nhiều nhất
   - Doanh thu từ dịch vụ

---

## 🚀 IMPLEMENTATION PRIORITY

### **Phase 1: Core Functions (URGENT)**

- [x] Tạo booking
- [x] Hiển thị timeline
- [x] Sync data Tab 1 ↔ Tab 2
- [ ] **Hủy booking với lý do** 👈 CẦN LÀM
- [ ] **Bỏ enum CHO_XAC_NHAN, DANG_SU_DUNG** 👈 CẦN LÀM

### **Phase 2: Advanced Features**

- [ ] Thêm dịch vụ (đã có UI)
- [ ] Thanh toán đủ (đã có UI)
- [ ] Dialog hủy lịch mới
- [ ] Update màu sắc (3 màu)

### **Phase 3: Polish**

- [ ] Báo cáo doanh thu
- [ ] Thống kê hủy lịch
- [ ] In hóa đơn
- [ ] Export Excel

---

## ✅ CHECKLIST DEVELOPER

- [ ] Update `TrangThaiDatSan` enum (chỉ còn 3: DA_DAT, HOAN_THANH, DA_HUY)
- [ ] Thêm field `lyDoHuy` vào `DatSan`
- [ ] Thêm field `hoanCoc` (bool) vào `DatSan`
- [ ] Update `BookingDetailDialog`:
  - Bỏ button "Xác nhận & Đặt cọc"
  - Bỏ button "Check-in"
  - Thêm dialog hủy lịch mới
- [ ] Update màu sắc timeline (3 màu: xanh lá, xanh dương, đỏ)
- [ ] Update logic tạo booking (auto cọc 30%)
- [ ] Test toàn bộ flow

---

## 📝 KẾT LUẬN

Workflow này được thiết kế để:

✅ **Đơn giản nhất:** Chỉ 4 bước thực sự cần thiết  
✅ **Linh hoạt:** Nhân viên tự quyết hoàn/mất cọc  
✅ **Minh bạch:** Bắt buộc ghi lý do → dễ tra cứu  
✅ **Thực tế:** Phù hợp với cách vận hành sân bóng mini thực tế

Không có logic phức tạp (3h), không có trạng thái thừa (chờ xác nhận, check-in), không cần training nhiều → Nhân viên mới dùng được ngay!
