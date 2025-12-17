# THIẾT KẾ MẪU HÓA ĐƠN (TEXT/NOTEPAD)

Tài liệu này mô tả định dạng hóa đơn dạng text (.txt) sẽ được xuất ra khi thanh toán đơn đặt sân hoặc đơn hàng dịch vụ.
**Lưu ý:** Sử dụng tiếng Việt có dấu (UTF-8).

## 1. Hóa Đơn Đặt Sân (Booking Invoice)

Mẫu hóa đơn này được sử dụng khi thanh toán cho một lượt đặt sân, bao gồm tiền sân và các dịch vụ đi kèm (nếu có).

### Mẫu thiết kế:

```text
================================================================
                        HÓA ĐƠN ĐẶT SÂN
                    SÂN BÓNG ĐÁ MINI ĐÀ NẴNG
             Địa chỉ: 54 Nguyễn Lương Bằng, Đà Nẵng
                     Hotline: 0905.123.456
================================================================
Mã đơn:         [MA_DAT_SAN]
Ngày lập:       [NGAY_GIO_HIEN_TAI]

THÔNG TIN KHÁCH HÀNG
----------------------------------------------------------------
Tên khách hàng: [TEN_KHACH_HANG]
Số điện thoại:  [SDT_KHACH_HANG]
Hạng thành viên:[HANG_THANH_VIEN] (Nếu có)

THÔNG TIN ĐẶT SÂN
----------------------------------------------------------------
Sân:            [TEN_SAN] ([LOAI_SAN])
Ngày đá:        [NGAY_DA]
Khung giờ:      [GIO_BAT_DAU] - [GIO_KET_THUC]
Giá sân:        [GIA_SAN] đ

DỊCH VỤ KÈM THEO
----------------------------------------------------------------
STT  Tên dịch vụ          Số lượng    Đơn giá      Thành tiền
----------------------------------------------------------------
1    Nước ngọt            2           10,000       20,000
2    Thuê áo đấu          5           15,000       75,000
...
----------------------------------------------------------------
Tổng tiền dịch vụ:                               [TONG_TIEN_DICH_VU] đ

THANH TOÁN
----------------------------------------------------------------
Tổng cộng:                                         [TONG_TIEN] đ
Đã đặt cọc:                                         [TIEN_COC] đ
Còn lại phải thu:                               [TIEN_CON_LAI] đ
----------------------------------------------------------------
Trạng thái: ĐÃ THANH TOÁN

                CẢM ƠN QUÝ KHÁCH VÀ HẸN GẶP LẠI!
================================================================
```

### Các trường dữ liệu:

- **[MA_DAT_SAN]**: Mã định danh của đơn đặt sân.
- **[NGAY_GIO_HIEN_TAI]**: Thời điểm xuất hóa đơn.
- **[TEN_KHACH_HANG]**, **[SDT_KHACH_HANG]**: Thông tin khách đặt.
- **[TEN_SAN]**, **[LOAI_SAN]**: Tên sân (Sân 1, Sân 2...) và loại (Sân 5, Sân 7).
- **[NGAY_DA]**, **[GIO_BAT_DAU]**, **[GIO_KET_THUC]**: Thời gian sử dụng sân.
- **[GIA_SAN]**: Giá thuê sân cho khung giờ đó.
- **Danh sách dịch vụ**: Liệt kê các dịch vụ đã đặt kèm theo booking.
- **[TONG_TIEN]**: Tổng tiền sân + tiền dịch vụ.
- **[TIEN_COC]**: Số tiền đã đặt cọc trước đó.
- **[TIEN_CON_LAI]**: Số tiền khách cần thanh toán lúc này (= Tổng tiền - Cọc).

---

## 2. Hóa Đơn Dịch Vụ (Service Invoice)

Mẫu hóa đơn này được sử dụng khi khách hàng mua dịch vụ lẻ (nước uống, thuê đồ...) mà không gắn liền với một booking cụ thể, hoặc thanh toán đơn hàng dịch vụ riêng biệt.

### Mẫu thiết kế:

```text

================================================================
                        HÓA ĐƠN DỊCH VỤ
                    SÂN BÓNG ĐÁ MINI ĐÀ NẴNG
             Địa chỉ: 54 Nguyễn Lương Bằng, Đà Nẵng
                     Hotline: 0905.123.456
================================================================
Mã đơn:         [MA_DON_HANG]
Ngày lập:       [NGAY_GIO_HIEN_TAI]

THÔNG TIN KHÁCH HÀNG
----------------------------------------------------------------
Tên khách hàng: [TEN_KHACH_HANG] (Khách lẻ)
Số điện thoại:  [SDT_KHACH_HANG]

DANH SÁCH DỊCH VỤ
----------------------------------------------------------------
STT  Tên dịch vụ          Số lượng    Đơn giá      Thành tiền
----------------------------------------------------------------
1    Nước ngọt            2           10,000       20,000
2    Mỳ tôm trứng         1           25,000       25,000
3    Giải khát            3           12,000       36,000
...
----------------------------------------------------------------

THANH TOÁN
----------------------------------------------------------------
Tổng tiền:                                         [TONG_TIEN] đ
Giảm giá:                                           [GIAM_GIA] đ (Nếu có)
Thành tiền:                                       [THANH_TIEN] đ
----------------------------------------------------------------
Trạng thái: ĐÃ THANH TOÁN

                CẢM ƠN QUÝ KHÁCH VÀ HẸN GẶP LẠI!
================================================================
```

### Các trường dữ liệu:

- **[MA_DON_HANG]**: Mã định danh đơn hàng dịch vụ.
- **[TEN_KHACH_HANG]**: Tên khách hàng (hoặc "Khách lẻ" nếu không có thông tin).
- **Danh sách dịch vụ**: Chi tiết các món đã mua.
- **[TONG_TIEN]**: Tổng giá trị đơn hàng trước giảm giá.
- **[GIAM_GIA]**: Số tiền được giảm (nếu có áp dụng khuyến mãi/thành viên).
- **[THANH_TIEN]**: Số tiền thực tế khách phải trả.

---

## 3. Hóa Đơn Hủy Đặt Sân (Mất Cọc)

Mẫu hóa đơn này được sử dụng khi khách hàng hủy đặt sân và bị mất tiền cọc (do hủy muộn hoặc vi phạm chính sách).

### Mẫu thiết kế:

```text
================================================================
                 XÁC NHẬN HỦY ĐẶT SÂN (MẤT CỌC)
                    SÂN BÓNG ĐÁ MINI ĐÀ NẴNG
             Địa chỉ: 54 Nguyễn Lương Bằng, Đà Nẵng
                     Hotline: 0905.123.456
================================================================
Mã đơn:         [MA_DAT_SAN]
Ngày lập:       [NGAY_GIO_HIEN_TAI]

THÔNG TIN KHÁCH HÀNG
----------------------------------------------------------------
Tên khách hàng: [TEN_KHACH_HANG]
Số điện thoại:  [SDT_KHACH_HANG]

THÔNG TIN ĐẶT SÂN ĐÃ HỦY
----------------------------------------------------------------
Sân:            [TEN_SAN] ([LOAI_SAN])
Ngày đá:        [NGAY_DA]
Khung giờ:      [GIO_BAT_DAU] - [GIO_KET_THUC]

CHI TIẾT HỦY
----------------------------------------------------------------
Lý do hủy:      [LY_DO_HUY]
Thời gian hủy:  [THOI_GIAN_HUY]

THÔNG TIN THANH TOÁN
----------------------------------------------------------------
Tiền cọc đã đóng:                                   [TIEN_COC] đ
Trạng thái cọc:         KHÔNG HOÀN LẠI (MẤT CỌC)
Số tiền hoàn lại:                                            0 đ
----------------------------------------------------------------
Trạng thái: ĐÃ HỦY

                     XÁC NHẬN CỦA HỆ THỐNG
================================================================
```

### Các trường dữ liệu:

- **[LY_DO_HUY]**: Lý do khách hàng hủy sân (VD: Bận việc đột xuất, Đổi lịch...).
- **[THOI_GIAN_HUY]**: Thời điểm thực hiện thao tác hủy.
- **Trạng thái cọc**: Ghi rõ là "KHÔNG HOÀN LẠI" để xác nhận việc mất cọc.

## Ghi chú kỹ thuật

- Độ rộng hóa đơn: Khoảng 64 ký tự.
- Encoding: UTF-8 để hiển thị tiếng Việt có dấu.
- Căn chỉnh: Sử dụng khoảng trắng (space).
- Định dạng tiền: Sử dụng dấu phẩy ngăn cách hàng nghìn (VD: 100,000).
