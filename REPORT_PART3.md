# 3. THIẾT KẾ CẤU TRÚC DỮ LIỆU VÀ GIẢI THUẬT

## 3.1. Phát biểu bài toán

### a. Mô tả dữ liệu đầu vào (Input)
Hệ thống quản lý sân bóng đá mini xử lý các luồng thông tin phức tạp xoay quanh việc đặt sân và quản lý dịch vụ. Dữ liệu đầu vào được mô hình hóa thành các lớp đối tượng (Class) với các thuộc tính cụ thể như sau:

**1. Đối tượng Khách hàng (Customer):**
*   **Mã khách hàng (ID):** Chuỗi định danh duy nhất (VD: KH001, KH002).
*   **Thông tin cá nhân:** Họ tên, Số điện thoại (Key tìm kiếm chính).
*   **Thông tin thành viên:** Ngày đăng ký, Tổng chi tiêu tích lũy.
*   **Hạng thành viên:** Phân loại dựa trên chi tiêu (Thường, Đồng, Bạc, Vàng, Kim Cương).

**2. Đối tượng Sân bóng (Field):**
*   **Mã sân:** Định danh sân (VD: SAN01, SAN02).
*   **Thuộc tính:** Tên sân, Loại sân (Sân 5 người, Sân 7 người).
*   **Kinh doanh:** Giá thuê theo giờ (VNĐ), Trạng thái hiện tại (Trống/Đang đá/Bảo trì).

**3. Đối tượng Dịch vụ (Service):**
*   **Thông tin:** Mã dịch vụ, Tên dịch vụ (Nước, Giày, Áo...).
*   **Kho vận:** Đơn giá nhập/bán, Số lượng tồn kho, Đơn vị tính.
*   **Thống kê:** Số lượng đã bán.

**4. Đối tượng Đặt sân (Booking):**
*   **Liên kết:** Mã đặt sân, Khách hàng đặt, Sân được chọn.
*   **Thời gian:** Ngày đặt, Giờ bắt đầu, Giờ kết thúc.
*   **Trạng thái:** Đã đặt (Confirmed), Đã hủy (Cancelled), Đã hoàn thành (Completed).

### b. Yêu cầu đầu ra (Output)
Hệ thống cần đảm bảo cung cấp đầy đủ các thông tin đầu ra phục vụ cho việc quản lý, vận hành và ra quyết định kinh doanh:

**1. Đầu ra Nghiệp vụ (Operational Output):**
*   **Danh sách tình trạng sân:** Hiển thị trực quan trạng thái (Trống/Có khách) của tất cả các sân trong khung giờ hiện tại hoặc một ngày cụ thể.
*   **Kết quả kiểm tra lịch (Conflict Check):** Thông báo "Hợp lệ" hoặc "Trùng lịch" ngay lập tức khi nhân viên chọn giờ đặt, kèm theo gợi ý các khung giờ trống lân cận.
*   **Hóa đơn thanh toán (Invoice):** Tự động tính toán và xuất ra chi tiết bao gồm: Tiền thuê sân (theo số giờ đá thực tế) + Tiền dịch vụ (nước, thuê đồ) - Giảm giá (theo hạng thành viên).

**2. Đầu ra Quản lý (Management Output):**
*   **Danh sách quản lý:** Các bảng dữ liệu (Table) cho phép xem, lọc và sắp xếp thông tin Khách hàng, Sân, Dịch vụ, Nhân viên.
*   **Nhật ký hoạt động:** Ghi nhận các thay đổi quan trọng (Ví dụ: Khách hàng A vừa hủy sân, Dịch vụ B vừa nhập thêm kho).
*   **Cảnh báo tồn kho:** Tự động đánh dấu hoặc thông báo các mặt hàng dịch vụ sắp hết hàng (Low Stock).

**3. Đầu ra Thống kê & Báo cáo (Analytical Output):**
*   **Báo cáo doanh thu:** Biểu đồ cột/đường thể hiện tổng doanh thu theo ngày, tháng, năm.
*   **Phân tích hiệu suất sân:** Báo cáo tần suất sử dụng của từng sân (Sân nào đắt khách nhất, khung giờ nào đông nhất).
*   **Phân loại khách hàng:** Danh sách Top khách hàng chi tiêu cao nhất (VIP) để áp dụng chính sách chăm sóc.

---

## 3.2. Hình ảnh mô phỏng dữ liệu đầu vào

Khác với các hệ thống Console truyền thống nhập liệu qua dòng lệnh, hệ thống này sử dụng giao diện đồ họa (GUI) để nhập liệu trực quan và lưu trữ dữ liệu bền vững (Persistence) thông qua các tệp tin cấu trúc CSV (Comma-Separated Values).

**1. Cấu trúc lưu trữ vật lý (File CSV):**
Dữ liệu được lưu trữ dưới dạng văn bản thuần, mỗi dòng là một bản ghi, các trường ngăn cách nhau bởi dấu phẩy.

> **[HÌNH ẢNH 3.1]:** *Bạn hãy mở file `Data/khachhang.csv` hoặc `Data/dichvu.csv` bằng Notepad/Excel và chụp lại.*
>
> *Chú thích:* Hình 3.1. Cấu trúc file CSV lưu trữ dữ liệu khách hàng và dịch vụ.

**2. Giao diện nhập liệu (Data Entry GUI):**
Dữ liệu đầu vào từ người dùng được kiểm tra tính hợp lệ (Validation) ngay trên giao diện trước khi đưa vào xử lý.

> **[HÌNH ẢNH 3.2]:** *Bạn chạy chương trình, mở Dialog "Thêm Khách Hàng" hoặc "Thêm Dịch Vụ" và chụp lại.*
>
> *Chú thích:* Hình 3.2. Biểu mẫu nhập liệu thông tin với các ràng buộc dữ liệu.

---

## 3.3. Phân tích và ứng dụng cấu trúc dữ liệu

Để tối ưu hóa hiệu năng và quản lý bộ nhớ hiệu quả, hệ thống sử dụng các cấu trúc dữ liệu tự định nghĩa (Custom Data Structures). Dưới đây là phân tích chi tiết về cách chúng vận hành bên trong hệ thống.

### a. Mảng động (Dynamic Array - MangDong)

**Phân tích lý thuyết:**
Mảng động khắc phục nhược điểm kích thước cố định của mảng tĩnh bằng cách tự động thay đổi dung lượng bộ nhớ khi cần thiết.

**Cài đặt (Implementation):**
Class `MangDong<T>` quản lý một con trỏ `data` trỏ đến vùng nhớ heap và hai biến nguyên để theo dõi kích thước.

> **[HÌNH ẢNH 3.3a]:** *Bạn mở file `Core/CauTrucDuLieu/MangDong.h` và chụp lại phần khai báo class (khoảng dòng 10-20), bao gồm các thuộc tính `data`, `kichThuoc`, `dungLuong`.*
>
> *Chú thích:* Hình 3.3a. Khai báo lớp Mảng động với cơ chế quản lý bộ nhớ tự động.

**Cơ chế hoạt động trong hệ thống:**
Quá trình thêm một phần tử mới (ví dụ: Thêm một Khách hàng mới) diễn ra theo quy trình sau:
1.  **Kiểm tra sức chứa:** Hệ thống so sánh số lượng phần tử hiện tại (`size`) với dung lượng tối đa (`capacity`).
2.  **Mở rộng (Resize):** Nếu mảng đã đầy (`size == capacity`), hệ thống thực hiện:
    *   Tạo một vùng nhớ mới có kích thước gấp đôi (`new_capacity = capacity * 2`).
    *   Sao chép toàn bộ dữ liệu từ vùng nhớ cũ sang vùng nhớ mới.
    *   Giải phóng (delete) vùng nhớ cũ để tránh rò rỉ bộ nhớ.
3.  **Thêm phần tử:** Chèn phần tử mới vào vị trí `index = size` và tăng biến đếm `size` lên 1.

**Ứng dụng thực tiễn:**
Template class `MangDong<T>` là cấu trúc lưu trữ chính cho `DanhSachKhachHang`, `DanhSachSan`, `DanhSachDichVu`.
*   *Tại sao chọn Mảng động?* Giao diện người dùng (UI) sử dụng `QTableWidget` để hiển thị dữ liệu. Widget này truy xuất dữ liệu theo dòng (Row Index). Mảng động cung cấp khả năng truy cập ngẫu nhiên (Random Access) với độ phức tạp **O(1)**, giúp việc render dữ liệu lên bảng cực nhanh, không bị giật lag khi cuộn trang.

### b. Danh sách liên kết (Linked List - DanhSachLienKet)

**Phân tích lý thuyết:**
Danh sách liên kết bao gồm các nút (Node) nằm rải rác trong bộ nhớ, liên kết với nhau qua con trỏ `next`.

**Cài đặt (Implementation):**
Class `DanhSachLienKet<T>` được xây dựng từ các `Node` và quản lý bởi con trỏ đầu (`head`) và cuối (`tail`).

> **[HÌNH ẢNH 3.3b]:** *Bạn mở file `Core/CauTrucDuLieu/DanhSachLienKet.h` và chụp lại phần khai báo `struct Node` và các con trỏ `head`, `tail` (khoảng dòng 15-25).*
>
> *Chú thích:* Hình 3.3b. Cấu trúc Node và các con trỏ quản lý trong Danh sách liên kết đơn.

**Cơ chế hoạt động trong hệ thống:**
Cấu trúc này được thiết kế đơn giản hóa để phục vụ riêng cho Bảng băm:
1.  **Cấu trúc Node:** Mỗi Node chứa dữ liệu `data` (cặp Key-Value) và con trỏ `pNext`.
2.  **Thêm phần tử:** Luôn thêm vào cuối danh sách (Tail). Hệ thống duy trì con trỏ `pTail` để thao tác thêm diễn ra trong **O(1)** mà không cần duyệt từ đầu.
3.  **Duyệt:** Sử dụng con trỏ `pHead` để duyệt tuần tự khi cần tìm kiếm một phần tử cụ thể trong danh sách.

**Ứng dụng thực tiễn:**
Trong dự án này, Danh sách liên kết đóng vai trò là **cấu trúc nền tảng (Underlying Structure)** cho Bảng băm.
*   *Vai trò:* Nó hoạt động như các "thùng chứa" (buckets) để xử lý va chạm. Khi Bảng băm tính ra cùng một chỉ số cho 2 khóa khác nhau, cả 2 sẽ được lưu chuỗi vào cùng một Danh sách liên kết tại chỉ số đó.

### c. Bảng băm (Hash Table)

**Phân tích lý thuyết:**
Bảng băm ánh xạ khóa (Key) sang chỉ số mảng (Index) để truy xuất dữ liệu gần như tức thời.

**Cài đặt (Implementation):**
Class `HashTable` kết hợp giữa `MangDong` (làm bảng chứa) và `DanhSachLienKet` (làm bucket chứa dữ liệu).
```cpp
template <typename K, typename V>
class HashTable {
private:
    // Mảng động chứa các con trỏ đến Danh sách liên kết
    MangDong<DanhSachLienKet<KeyValuePair<K, V>> *> table;
    int capacity; // Kích thước bảng băm
    
    int hash(const K &key); // Hàm băm chuyển Key -> Index
public:
    void insert(const K &key, const V &value); // Thêm mới (O(1))

> **[HÌNH ẢNH 3.3c]:** *Bạn mở file `Core/ThuatToan/HashTable.h` và chụp lại dòng khai báo `MangDong<DanhSachLienKet<KeyValuePair<K, V>> *> table` (khoảng dòng 25).*
>
> *Chú thích:* Hình 3.3c. Sự kết hợp giữa Mảng động và Danh sách liên kết để tạo nên Bảng băm. *Bài toán:* Khi nhân viên nhập SĐT khách hàng để đặt sân, hệ thống cần biết ngay khách này đã từng đến chưa để tự động điền tên và tính điểm tích lũy.
*   *Hiệu quả:* Với hàng nghìn khách hàng, việc tìm kiếm tuần tự mất **O(n)**. Bảng băm giúp trả về kết quả gần như ngay lập tức **O(1)**, tăng tốc độ thao tác của thu ngân.

> **[HÌNH ẢNH 3.3]:** *Bạn chụp đoạn code trong file `HashTable.h` chỗ khai báo `MangDong<DanhSachLienKet...>` để chứng minh việc sử dụng kết hợp này.*
>
> *Chú thích:* Hình 3.3. Danh sách liên kết được sử dụng làm cấu trúc nền tảng cho Bảng băm.

---

## 3.4. Phân tích và ứng dụng giải thuật

Hệ thống áp dụng các thuật toán tìm kiếm và sắp xếp kinh điển để xử lý dữ liệu nghiệp vụ. Dưới đây là phân tích chi tiết cách các thuật toán này vận hành trong các chức năng cụ thể.

### a. Thuật toán Tìm kiếm Tuần tự (Linear Search)

**Chức năng áp dụng:** **Thanh tìm kiếm (Search Bar)** trên các màn hình Quản lý Dịch vụ và Khách hàng.

**Cơ chế hoạt động trong hệ thống:**
1.  Người dùng nhập một từ khóa vào ô tìm kiếm (Ví dụ: "Nuoc").
2.  Hệ thống kích hoạt sự kiện `textChanged`.
3.  Thuật toán bắt đầu duyệt vòng lặp `for` từ phần tử đầu tiên đến phần tử cuối cùng của `MangDong<DichVu*>`.
4.  Tại mỗi phần tử, hệ thống lấy tên dịch vụ (`layTenDichVu()`), chuyển về chữ thường và kiểm tra xem từ khóa "nuoc" có xuất hiện trong tên hay không (sử dụng hàm `string::find`).
5.  Nếu tìm thấy (Match), phần tử đó được thêm vào một danh sách tạm thời (`displayedServices`) để hiển thị lên giao diện.

**Tại sao sử dụng:**
Vì nhu cầu tìm kiếm ở đây là **tìm kiếm gần đúng (Pattern Matching)** trên trường Tên (Name), không phải tìm chính xác theo Khóa (Key). Do đó, không thể sử dụng Hash Table hay Binary Search, mà bắt buộc phải duyệt toàn bộ danh sách để không bỏ sót kết quả.

> **[HÌNH ẢNH 3.4]:** *Vào màn hình Quản lý Dịch vụ, gõ từ khóa vào ô tìm kiếm để lọc ra kết quả. Chụp lại màn hình.*
>
> *Chú thích:* Hình 3.4. Kết quả tìm kiếm dịch vụ sử dụng thuật toán Linear Search.

### b. Thuật toán Sắp xếp Nhanh (Quick Sort)

**Chức năng áp dụng:** **Bảng xếp hạng Khách hàng thân thiết (Top Spending)** trong màn hình Thống kê.

**Cơ chế hoạt động trong hệ thống:**
1.  Khi người dùng chọn xem "Top Khách Hàng", hệ thống sao chép danh sách khách hàng từ `MangDong` gốc sang một mảng tạm.
2.  Hàm `QuickSort::sort` được gọi với tiêu chí so sánh là thuộc tính `TongChiTieu`.
3.  **Bước Phân hoạch (Partition):** Thuật toán chọn một khách hàng làm "chốt" (pivot).
4.  Nó quét mảng và hoán đổi vị trí sao cho tất cả khách hàng có chi tiêu **lớn hơn** pivot nằm về phía trước, và chi tiêu **nhỏ hơn** nằm về phía sau (Sắp xếp giảm dần).
5.  Quá trình này lặp lại đệ quy cho hai nửa danh sách con cho đến khi toàn bộ danh sách được sắp xếp.
6.  Kết quả cuối cùng được đưa vào biểu đồ hoặc bảng xếp hạng.

**Tại sao sử dụng:**
Dữ liệu thống kê thường có số lượng lớn. Quick Sort có độ phức tạp trung bình **O(n log n)** và thực hiện việc hoán đổi tại chỗ (in-place), giúp xử lý nhanh và tiết kiệm bộ nhớ hơn so với các thuật toán đơn giản như Bubble Sort (O(n²)).

> **[HÌNH ẢNH 3.5]:** *Vào màn hình Thống kê -> Khách hàng, chụp lại bảng xếp hạng đã sắp xếp.*
>
> *Chú thích:* Hình 3.5. Bảng xếp hạng khách hàng được sắp xếp bằng Quick Sort.

### c. Thuật toán Sắp xếp Trộn (Merge Sort)

**Chức năng áp dụng:** **Bộ lọc sắp xếp đa tiêu chí** trong màn hình Quản lý Dịch vụ (Sắp xếp theo Giá, Tồn kho).

**Cơ chế hoạt động trong hệ thống:**
1.  Người dùng chọn tiêu chí "Giá" và thứ tự "Tăng dần" trên giao diện.
2.  Hệ thống gọi hàm `MergeSort::sort` trên danh sách dịch vụ hiện tại.
3.  **Bước Chia (Divide):** Danh sách dịch vụ liên tục được chia đôi cho đến khi mỗi phần chỉ còn 1 dịch vụ.
4.  **Bước Trộn (Merge):** Hệ thống bắt đầu gộp các phần nhỏ lại. Tại bước này, nó so sánh `DonGia` của hai dịch vụ.
    *   Nếu giá bằng nhau, thuật toán giữ nguyên thứ tự trước sau của chúng (Tính ổn định).
    *   Nếu giá khác nhau, dịch vụ giá thấp hơn được đưa vào trước.
5.  Danh sách sau khi trộn được cập nhật lại lên bảng hiển thị (`QTableWidget`).

**Tại sao sử dụng:**
Merge Sort là thuật toán **Sắp xếp ổn định (Stable Sort)**. Trong quản lý dịch vụ, điều này rất quan trọng để đảm bảo trải nghiệm người dùng (UX). Ví dụ: Nếu danh sách đang được sắp xếp theo Tên (A-Z), khi người dùng chọn sắp xếp lại theo Giá, các món có cùng giá sẽ vẫn giữ nguyên thứ tự A-Z, không bị xáo trộn ngẫu nhiên như Quick Sort.

> **[HÌNH ẢNH 3.6]:** *Vào màn hình Quản lý Dịch vụ, chọn Sắp xếp theo Giá -> Tăng dần. Chụp lại màn hình.*
>
> *Chú thích:* Hình 3.6. Danh sách dịch vụ được sắp xếp ổn định bằng Merge Sort.
