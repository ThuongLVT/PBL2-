# 4.1. PHÂN TÍCH CÁC LỚP ĐỐI TƯỢNG CHÍNH

Chương trình bao gồm các lớp thực thể quan trọng, đại diện cho các đối tượng thực tế trong bài toán quản lý. Dưới đây là phân tích chi tiết từng lớp:

### a. Lớp Khách Hàng (Class KhachHang)

Lớp `KhachHang` kế thừa từ lớp cơ sở `ConNguoi`, đại diện cho người sử dụng dịch vụ sân bóng.

> **[HÌNH ẢNH 4.1]:** *Bạn mở file `Core/Models/KhachHang.h` và chụp lại toàn bộ phần khai báo class (từ `class KhachHang : public ConNguoi` đến hết phần `private` và một số hàm `public` quan trọng).*
>
> *Chú thích:* Hình 4.1. Thuộc tính và phương thức của lớp Khách hàng.

**Các thuộc tính quan trọng:**
*   `maKhachHang`: Chuỗi định danh duy nhất (VD: KH001), dùng để phân biệt các khách hàng khác nhau trong hệ thống.
*   `tongChiTieu`: Lưu tổng số tiền khách đã thanh toán, dùng để xét hạng thành viên.
*   `hang`: Enum `HangKhachHang` (Thường, Đồng, Bạc, Vàng, Kim Cương) xác định mức ưu đãi giảm giá.
*   `soLanDatSan`: Thống kê tần suất sử dụng dịch vụ để đánh giá độ trung thành.
*   `ngayDangKy`: Lưu thời điểm khách hàng bắt đầu tham gia hệ thống.

**Các phương thức chính:**
*   `layPhanTramGiamGia()`: Trả về % giảm giá (0-10%) dựa trên hạng thành viên hiện tại.
*   `themChiTieu(double soTien)`: Cộng dồn tiền vào tổng chi tiêu và tự động gọi `capNhatHang()` để thăng hạng nếu đủ điều kiện.
*   `hienThiThongTin()`: Override phương thức ảo từ lớp cha để hiển thị thông tin chi tiết kèm theo hạng thành viên.
*   `ghiFile()` / `docFile()`: Hỗ trợ lưu trữ và khôi phục dữ liệu từ file nhị phân (Persistence).

**Hàm bạn (Friend Functions):**
*   `friend std::ostream &operator<<(std::ostream &os, const KhachHang &kh)`: Cho phép in trực tiếp thông tin đối tượng `KhachHang` ra luồng output (như `cout` hoặc file text) một cách tiện lợi.

### b. Lớp Sân Bóng (Class San)

Lớp `San` đại diện cho cơ sở vật chất quan trọng nhất của hệ thống.

> **[HÌNH ẢNH 4.2]:** *Bạn mở file `Core/Models/San.h` và chụp lại phần khai báo class.*
>
> *Chú thích:* Hình 4.2. Cấu trúc dữ liệu của đối tượng Sân bóng.

**Các thuộc tính quan trọng:**
*   `maSan`: Mã định danh sân (VD: A501, B702).
*   `loaiSan`: Enum `LoaiSan` (Sân 5, Sân 7) quyết định sức chứa và giá cơ bản.
*   `khuVuc`: Enum `KhuVuc` (A, B, C, D) giúp quản lý vị trí sân bãi.
*   `trangThai`: Enum `TrangThaiSan` (Hoạt động, Bảo trì, Ngừng hoạt động) giúp ngăn chặn việc đặt sân khi sân đang sửa chữa.
*   `giaThueGio`: Đơn giá thuê cơ bản mỗi giờ.
*   `ghiChu`: Lưu trữ các thông tin phụ (ví dụ: "Sân gần cổng vào").

**Các phương thức chính:**
*   `tinhTien(double soGio)`: Tính toán tiền thuê dựa trên số giờ đá thực tế.
*   `dangHoatDong()` / `dangBaoTri()`: Các hàm kiểm tra nhanh trạng thái của sân.
*   `ghiFile()` / `docFile()`: Lưu trữ và đọc thông tin sân từ file.
*   `operator<`: Nạp chồng toán tử so sánh để hỗ trợ sắp xếp sân theo giá thuê.

**Hàm bạn (Friend Functions):**
*   `friend std::ostream &operator<<(std::ostream &os, const San &s)`: Hỗ trợ xuất thông tin sân ra màn hình hoặc file log nhanh chóng.

### c. Lớp Đặt Sân (Class DatSan)

Lớp `DatSan` là thực thể trung tâm, kết nối giữa Khách hàng, Sân bóng và Dịch vụ.

> **[HÌNH ẢNH 4.3]:** *Bạn mở file `Core/Models/DatSan.h` và chụp lại phần khai báo class.*
>
> *Chú thích:* Hình 4.3. Lớp Đặt sân quản lý thông tin giao dịch.

**Các thuộc tính quan trọng:**
*   `maDatSan`: Mã giao dịch duy nhất.
*   `khachHang`: Con trỏ trỏ đến đối tượng `KhachHang` thực hiện đặt sân.
*   `san`: Con trỏ trỏ đến đối tượng `San` được chọn.
*   `danhSachDichVu`: Mảng động `MangDong<DichVuDat>` chứa các dịch vụ đi kèm.
*   `trangThai`: Enum `TrangThaiDatSan` (Đã đặt, Hoàn thành, Đã hủy).
*   `tienCoc`: Số tiền khách đã đặt cọc trước (thường là 30%).
*   `trangThaiCoc`: Theo dõi trạng thái cọc (Đã cọc, Hoàn cọc, Mất cọc).
*   `lyDoHuy`: Lưu lý do nếu đơn đặt sân bị hủy.

**Các phương thức chính:**
*   `getTongTien()`: Lấy tổng tiền thanh toán (đã bao gồm tiền sân và dịch vụ).
*   `themDichVu(DichVuDat dv)`: Thêm một dịch vụ vào danh sách dịch vụ của đơn đặt sân.
*   `setTrangThai(TrangThaiDatSan tt)`: Cập nhật trạng thái đơn hàng (Đã đặt -> Hoàn thành / Đã hủy).
*   `setLyDoHuy(string lyDo)`: Ghi nhận lý do hủy sân để phục vụ thống kê.
*   `getKhachHang()` / `getSan()`: Truy xuất đối tượng Khách hàng và Sân liên quan đến đơn đặt.

**Lưu ý:** Lớp `DatSan` tập trung vào xử lý nghiệp vụ liên kết giữa các đối tượng nên không sử dụng hàm bạn `operator<<` để in trực tiếp, mà thường được hiển thị thông qua các phương thức của lớp quản lý (`QuanLyDatSan`).

### d. Lớp Dịch Vụ (Class DichVu)

Lớp `DichVu` quản lý các mặt hàng kinh doanh thêm như đồ uống, dụng cụ thể thao.

> **[HÌNH ẢNH 4.4]:** *Bạn mở file `Core/Models/DichVu.h` và chụp lại phần khai báo class.*
>
> *Chú thích:* Hình 4.4. Lớp Dịch vụ quản lý kho hàng và giá bán.

**Các thuộc tính quan trọng:**
*   `maDichVu`, `tenDichVu`: Định danh dịch vụ.
*   `donGia`: Giá bán đơn vị.
*   `soLuongTon`: Quản lý số lượng tồn kho thực tế để cảnh báo khi sắp hết hàng.
*   `loaiDichVu`: Phân loại (Đồ ăn, Đồ uống, Thiết bị) giúp việc lọc và tìm kiếm dễ dàng hơn.
*   `donVi`: Đơn vị tính (Chai, Lon, Cái, Đôi).
*   `hinhAnh`: Đường dẫn đến file ảnh minh họa cho dịch vụ.

**Các phương thức chính:**
*   `layTenDichVu()`: Lấy tên hiển thị của dịch vụ.
*   `layDonGia()`: Lấy giá bán hiện tại.
*   `conHang()`: Kiểm tra nhanh xem dịch vụ còn trong kho hay không (dựa trên `soLuongTon` > 0).
*   `virtual ~DichVu()`: Destructor ảo đảm bảo hủy đúng đối tượng khi sử dụng tính đa hình (nếu sau này có các lớp con kế thừa chi tiết hơn).

**Lưu ý:** Lớp `DichVu` đóng vai trò là lớp cơ sở (Base Class) chứa dữ liệu chung. Các thao tác nhập xuất phức tạp thường được thực hiện ở các lớp quản lý hoặc lớp con cụ thể.
