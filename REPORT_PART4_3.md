# 4.3. PHÂN TÍCH CÁC TÍNH CHẤT OOP TRONG DỰ ÁN

Lập trình hướng đối tượng (OOP) là xương sống của toàn bộ dự án. Không chỉ dừng lại ở lý thuyết, các nguyên lý này được áp dụng triệt để trong mọi ngóc ngách của phần Core (Models, Managers, Data Structures). Dưới đây là bản phân tích đầy đủ và chi tiết nhất về việc áp dụng 4 tính chất OOP trong dự án.

### 4.3.1. Tính Đóng gói (Encapsulation)

**1. Định nghĩa:**
Tính đóng gói là việc gom dữ liệu (thuộc tính) và phương thức xử lý dữ liệu đó vào trong một đơn vị duy nhất (lớp), đồng thời che giấu chi tiết cài đặt nội bộ và chỉ cung cấp giao diện giao tiếp cần thiết ra bên ngoài.

**2. Áp dụng toàn diện trong dự án:**

*   **Tại các lớp Models (`KhachHang`, `San`, `DatSan`, `NhanVien`, `DichVu`...):**
    *   **Che giấu dữ liệu:** Tất cả thuộc tính như `maKhachHang`, `luongCoBan`, `matKhau` đều là `private` hoặc `protected`. Điều này ngăn chặn việc truy cập tự do từ bên ngoài (ví dụ: không ai có thể tự ý sửa `luongCoBan` của nhân viên từ hàm `main`).
    *   **Kiểm soát truy cập:** Dữ liệu chỉ được sửa đổi thông qua các hàm `setter` có kèm logic kiểm tra hợp lệ (Validation Logic).
    *   **Ví dụ:** Trong `KhachHang.h`, thuộc tính `tongChiTieu` là `private`. Hàm `themChiTieu(double tien)` không chỉ cộng tiền mà còn tự động kích hoạt logic `capNhatHang()` để thăng hạng thành viên.

*   **Tại lớp `HeThongQuanLy` (Singleton Pattern):**
    *   Đây là ví dụ điển hình nhất của đóng gói mức cao. Constructor của `HeThongQuanLy` là `private`, ngăn chặn việc tạo nhiều instance của hệ thống.
    *   Truy cập duy nhất thông qua phương thức tĩnh `getInstance()`.

*   **Tại các cấu trúc dữ liệu (`MangDong`, `DanhSachLienKet`):**
    *   Các lớp này đóng gói hoàn toàn việc quản lý bộ nhớ (cấp phát/giải phóng con trỏ). Người sử dụng chỉ cần gọi `them()`, `xoa()` mà không cần quan tâm đến việc `new` hay `delete` bộ nhớ bên dưới.

> **[HÌNH ẢNH 4.10]:** *Chụp code `HeThongQuanLy.h` phần `private: HeThongQuanLy();` và `static HeThongQuanLy* instance;` để minh họa Singleton và Đóng gói.*
>
> *Chú thích:* Hình 4.10. Áp dụng Singleton Pattern để đóng gói thể hiện duy nhất của hệ thống.

---

### 4.3.2. Tính Kế thừa (Inheritance)

**1. Định nghĩa:**
Cho phép xây dựng các lớp mới dựa trên các lớp đã có, thừa hưởng lại các thuộc tính và phương thức, giúp tái sử dụng mã nguồn và tạo cấu trúc phân cấp rõ ràng.

**2. Hệ thống phân cấp lớp trong dự án:**

Dự án sử dụng 3 nhánh kế thừa chính:

*   **Nhánh 1: Con Người (Person Hierarchy)**
    *   `ConNguoi` (Base Class): Chứa `hoTen`, `sdt`, `diaChi`.
    *   `KhachHang` (Derived from `ConNguoi`): Thêm `maKH`, `hang`, `diemTichLuy`.
    *   `NguoiDung` (Derived from `ConNguoi`): Thêm `taiKhoan`, `matKhau`.
        *   `NhanVien` (Derived from `NguoiDung`): Thêm `maNV`, `luong`, `caLamViec`.
        *   `QuanTriVien` (Derived from `NguoiDung`): Thêm quyền quản trị hệ thống.

*   **Nhánh 2: Dịch Vụ (Service Hierarchy)**
    *   `DichVu` (Base Class): Chứa `maDV`, `tenDV`, `donGia`.
    *   `DoUong` (Derived from `DichVu`): Thêm `dungTich`, `coDa`, `loaiDoUong`.
    *   `ThietBi` (Derived from `DichVu`): Thêm `tinhTrang`, `size`, `loaiThietBi`.

*   **Nhánh 3: Exception (Nếu có)**
    *   Các lớp ngoại lệ tự định nghĩa thường kế thừa từ `std::exception`.

> **[HÌNH ẢNH 4.11]:** *Chụp sơ đồ code của 2 file `DoUong.h` (`class DoUong : public DichVu`) và `NhanVien.h` (`class NhanVien : public NguoiDung`).*
>
> *Chú thích:* Hình 4.11. Cấu trúc kế thừa đa tầng trong hệ thống.

---

### 4.3.3. Tính Đa hình (Polymorphism)

**1. Định nghĩa:**
Cho phép các đối tượng thuộc các lớp khác nhau phản hồi lại cùng một thông điệp theo những cách riêng biệt. Trong C++, điều này thực hiện qua `virtual function` và `dynamic binding`.

**2. Áp dụng thực tế:**

*   **Quản lý danh sách hỗn hợp:**
    *   Hệ thống có thể quản lý một danh sách `MangDong<DichVu*>`. Danh sách này chứa lẫn lộn cả `DoUong` và `ThietBi`.
    *   Khi duyệt danh sách và gọi `hienThiThongTin()`, chương trình sẽ tự động gọi đúng hàm của đối tượng thực sự (nếu là Bia thì in dung tích, nếu là Giày thì in size) nhờ cơ chế **Dynamic Dispatch**.

*   **Phương thức `ghiFile()` và `docFile()`:**
    *   Đây là ứng dụng quan trọng nhất. Lớp cha `ConNguoi` định nghĩa `virtual bool ghiFile()`.
    *   Khi lưu danh sách nhân viên, hàm `NhanVien::ghiFile()` được gọi để lưu thêm lương và chức vụ.
    *   Khi lưu danh sách khách hàng, hàm `KhachHang::ghiFile()` được gọi để lưu thêm hạng thành viên.

*   **Destructor ảo (`virtual ~Destructor`):**
    *   Trong `ConNguoi.h` và `DichVu.h`, destructor luôn được khai báo `virtual`. Điều này đảm bảo khi `delete` một con trỏ lớp cha, destructor của lớp con sẽ được gọi, tránh rò rỉ bộ nhớ (Memory Leak).

> **[HÌNH ẢNH 4.12]:** *Chụp code hàm `virtual void hienThiThongTin()` trong `DichVu.h` và phần override trong `DoUong.h`.*
>
> *Chú thích:* Hình 4.12. Đa hình giúp xử lý danh sách dịch vụ hỗn hợp một cách linh hoạt.

---

### 4.3.4. Tính Trừu tượng (Abstraction)

**1. Định nghĩa:**
Tập trung vào các đặc điểm cốt lõi của đối tượng và bỏ qua các chi tiết không cần thiết. Trong C++, thường thể hiện qua Lớp trừu tượng (Abstract Class) chứa hàm thuần ảo (Pure Virtual Function).

**2. Áp dụng:**

*   **Lớp `DichVu`:**
    *   Đóng vai trò là một lớp trừu tượng (về mặt thiết kế). Trong thực tế, chúng ta không bao giờ tạo một đối tượng `DichVu` chung chung. Chúng ta chỉ tạo `DoUong` hoặc `ThietBi`.
    *   `DichVu` cung cấp một giao diện chung (`layDonGia()`, `layTen()`) để các lớp quản lý (`QuanLyDichVu`) có thể tương tác mà không cần biết đó là loại dịch vụ gì.

*   **Giao diện `IFileIO` (Implicit):**
    *   Mặc dù không khai báo interface `IFileIO` tường minh, nhưng việc tất cả các Model đều cài đặt cặp hàm `ghiFile/docFile` tạo nên một lớp trừu tượng ngầm định, giúp module `FileManager` có thể làm việc với bất kỳ đối tượng nào có khả năng lưu trữ.

---

### TỔNG KẾT ÁP DỤNG OOP

| Tính chất | Áp dụng chính | Mục đích |
| :--- | :--- | :--- |
| **Đóng gói** | Tất cả Models, Singleton Manager | Bảo mật dữ liệu, kiểm soát truy cập. |
| **Kế thừa** | `ConNguoi` -> `NhanVien`, `DichVu` -> `DoUong` | Tái sử dụng code, tổ chức phân cấp. |
| **Đa hình** | `hienThiThongTin`, `ghiFile`, `Destructor` | Xử lý danh sách hỗn hợp, mở rộng linh hoạt. |
| **Trừu tượng** | Class `DichVu`, `ConNguoi` | Tạo giao diện chung, ẩn chi tiết cài đặt. |
