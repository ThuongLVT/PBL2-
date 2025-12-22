# 4.4. Kết quả thực thi chương trình

Phần này trình bày chi tiết về giao diện người dùng và các kịch bản sử dụng thực tế của phần mềm **Football Field Manager**. Ứng dụng được xây dựng trên nền tảng **Qt Framework (C++)**, cung cấp giao diện đồ họa (GUI) trực quan, thân thiện và dễ sử dụng.

Quy trình sử dụng phần mềm tuân theo luồng logic: **Đăng nhập -> Giao diện chính -> Các chức năng nghiệp vụ**.

---

### 4.4.1. Giao diện Đăng nhập (Login)

Khi khởi động ứng dụng, màn hình đăng nhập sẽ xuất hiện đầu tiên để xác thực người dùng. Hệ thống hỗ trợ hai loại tài khoản: **Quản trị viên (Admin)** và **Nhân viên (Staff)**.

**Thao tác:**
1. Nhập **Tên đăng nhập** (Username).
2. Nhập **Mật khẩu** (Password).
3. Nhấn nút **Login** để truy cập hệ thống.
4. Nếu thông tin sai, hệ thống sẽ hiển thị thông báo lỗi.

> *[Hình 4.13: Giao diện màn hình Đăng nhập]*

---

### 4.4.2. Giao diện Chính (Main Window)

Sau khi đăng nhập thành công, người dùng sẽ được chuyển đến giao diện chính của chương trình. Giao diện được chia thành 2 phần chính:
1.  **Thanh bên (Sidebar):** Nằm bên trái, chứa menu điều hướng đến các chức năng quản lý.
2.  **Khu vực nội dung (Content Area):** Nằm bên phải, hiển thị thông tin chi tiết của chức năng đang được chọn.
3.  **Thanh tiêu đề (Header):** Hiển thị tên trang hiện tại và thông tin người dùng đang đăng nhập.

> *[Hình 4.14: Giao diện chính của phần mềm với Thanh bên và Khu vực làm việc]*

---

### 4.4.3. Chức năng Quản lý Đặt sân (Booking Management)

Đây là chức năng trung tâm của ứng dụng, hỗ trợ nhân viên lễ tân thực hiện quy trình đặt sân nhanh chóng và chính xác. Quy trình được thiết kế tối ưu hóa thao tác người dùng, đi từ việc chọn sân, xử lý thông tin đến quản lý các dịch vụ đi kèm.

**a. Giao diện đặt sân tổng quan**
Giao diện bao gồm lịch tháng, biểu đồ timeline trạng thái các sân (trục tung là thời gian, trục hoành là các sân) và form nhập liệu bên phải. Nhân viên có thể quan sát trực quan các khung giờ trống trên timeline để chọn giờ đá phù hợp.

> *[Hình 4.15a: Giao diện chính của chức năng Đặt sân với Timeline và Form nhập liệu]*

**b. Kiểm tra tính hợp lệ (Validation)**
Hệ thống tích hợp các quy tắc nghiệp vụ chặt chẽ để đảm bảo dữ liệu chính xác. Ví dụ: Hệ thống sẽ tự động chặn và hiển thị cảnh báo nếu nhân viên cố gắng đặt sân ở một khung giờ đã trôi qua trong quá khứ.

> *[Hình 4.15b: Hệ thống cảnh báo lỗi khi đặt sân ở khung giờ không hợp lệ]*

**c. Tìm kiếm và gợi ý khách hàng thông minh**
Để tăng tốc độ thao tác, ô nhập liệu "Số điện thoại" hoặc "Họ tên" được tích hợp thuật toán tìm kiếm tiền tố (Prefix Search). Khi nhân viên nhập một vài ký tự đầu, hệ thống sẽ gợi ý danh sách khách hàng cũ tương ứng, giúp điền nhanh thông tin mà không cần nhập lại toàn bộ.

> *[Hình 4.15c: Tính năng gợi ý thông tin khách hàng tự động (Auto-complete)]*

**d. Xác nhận thông tin đặt sân**
Sau khi điền đầy đủ thông tin, một hộp thoại xác nhận sẽ xuất hiện tóm tắt toàn bộ dữ liệu quan trọng (Khách hàng, Sân, Giờ đá, Giá sân, Tiền cọc) để nhân viên đối chiếu lần cuối trước khi lưu vào cơ sở dữ liệu, tránh sai sót.

> *[Hình 4.15d: Hộp thoại xác nhận thông tin trước khi tạo đơn đặt sân]*

**e. Quản lý danh sách đặt sân (Dashboard)**
Tab "Danh sách đặt sân" cung cấp cái nhìn tổng quan về tình hình hoạt động. Các thẻ thống kê phía trên hiển thị số lượng đơn theo trạng thái: **Tất cả**, **Đang phục vụ**, **Hoàn thành**, và **Đã hủy**. Bên dưới là bảng dữ liệu chi tiết cho phép lọc và tìm kiếm.

> *[Hình 4.15e: Dashboard quản lý danh sách các đơn đặt sân theo trạng thái]*

**f. Chi tiết đơn hàng và Thanh toán**
Nhân viên có thể xem chi tiết từng đơn đặt sân bằng cách chọn từ danh sách. Giao diện chi tiết hiển thị đầy đủ các thông tin về tiền sân, danh sách dịch vụ đã sử dụng, các khoản giảm giá/phụ thu và tổng số tiền khách cần thanh toán.

> *[Hình 4.15f: Giao diện chi tiết đơn đặt sân và tính toán chi phí]*

**g. Thêm dịch vụ đi kèm**
Trong quá trình khách sử dụng sân, nhân viên có thể thêm các dịch vụ (nước giải khát, thuê giày, áo...) vào đơn hàng. Giao diện chọn dịch vụ được thiết kế trực quan với hình ảnh minh họa và ô nhập số lượng, giúp thao tác bán hàng diễn ra nhanh chóng.

> *[Hình 4.15g: Giao diện chọn và thêm dịch vụ vào đơn đặt sân]*

---

### 4.4.4. Chức năng Quản lý Sân bóng (Field Management)

Chức năng này dành riêng cho Quản trị viên để quản lý cơ sở vật chất. Giao diện được thiết kế dạng Dashboard giúp nắm bắt nhanh tình trạng hoạt động của toàn bộ hệ thống sân bãi.

**a. Tổng quan và Danh sách sân**
Giao diện chính được chia thành 3 khu vực:
*   **Thanh thống kê (Top):** Hiển thị các chỉ số quan trọng như Tổng số sân, Số sân đang hoạt động, Đang bảo trì, và Ngừng hoạt động.
*   **Bảng danh sách (Center):** Liệt kê chi tiết các sân với mã sân, tên, loại sân (5/7 người), khu vực, giá thuê và trạng thái hiện tại. Màu sắc trạng thái được mã hóa (Xanh/Cam/Đỏ) để dễ nhận diện.
*   **Panel chi tiết (Right):** Cho phép xem và chỉnh sửa thông tin của sân đang được chọn, hoặc thêm sân mới.

> *[Hình 4.16a: Giao diện Dashboard quản lý sân bóng với các chỉ số thống kê]*

**b. Quản lý trạng thái bảo trì**
Khi một sân cần sửa chữa (ví dụ: thay cỏ, sửa đèn), quản trị viên có thể chuyển trạng thái sang "Bảo trì". Hệ thống sẽ yêu cầu nhập lý do bảo trì cụ thể để lưu vết lịch sử. Trong thời gian bảo trì, sân này sẽ bị khóa và không thể nhận lịch đặt mới.

> *[Hình 4.16b: Hộp thoại xác nhận đưa sân vào trạng thái bảo trì]*

---

### 4.4.5. Chức năng Quản lý Khách hàng (Customer Management)

Chức năng này giúp lưu trữ và quản lý hồ sơ khách hàng, hỗ trợ chiến lược chăm sóc khách hàng thân thiết (CRM).

**Giao diện quản lý tập trung**
Màn hình quản lý khách hàng cung cấp cái nhìn toàn diện về cơ sở dữ liệu người dùng:
*   **Thống kê tổng quan:** Hiển thị tổng số lượng khách hàng, số khách mới trong tháng và số lượng khách VIP.
*   **Danh sách khách hàng:** Bảng dữ liệu chi tiết bao gồm Mã KH, Họ tên, Số điện thoại, Hạng thành viên (Đồng, Bạc, Vàng...), Tổng chi tiêu tích lũy và Ngày đăng ký.
*   **Panel thông tin chi tiết:** Khi chọn một khách hàng từ danh sách, panel bên phải sẽ hiển thị form chỉnh sửa thông tin cá nhân và đặc biệt là **Lịch sử chi tiêu**. Tính năng này liệt kê toàn bộ các lần đặt sân và thanh toán trong quá khứ, giúp nhân viên nắm bắt thói quen của khách hàng.

> *[Hình 4.17: Giao diện Quản lý hồ sơ Khách hàng và Lịch sử giao dịch]*

---

### 4.4.6. Chức năng Quản lý Dịch vụ (Service Management)

Chức năng này bao gồm hai mảng nghiệp vụ chính: Bán hàng (POS) và Quản lý kho (Inventory).

**a. Quản lý danh mục và Kho hàng**
Giao diện quản lý kho cho phép Admin theo dõi toàn bộ danh mục sản phẩm.
*   **Danh sách sản phẩm:** Hiển thị hình ảnh minh họa, tên, giá bán, đơn vị tính và số lượng tồn kho hiện tại.
*   **Cập nhật thông tin:** Panel bên phải cho phép chỉnh sửa giá bán, nhập thêm hàng vào kho hoặc tải lên hình ảnh mới cho sản phẩm.

> *[Hình 4.18a: Giao diện Quản lý danh mục Dịch vụ và Tồn kho]*

**b. Bán hàng và Giỏ hàng dịch vụ**
Nhân viên có thể tạo đơn hàng bán lẻ cho khách vãng lai (không đặt sân) thông qua tab "Đặt Dịch Vụ".
*   **Chọn sản phẩm:** Giao diện hiển thị lưới sản phẩm trực quan. Khi click vào một món, hộp thoại chọn số lượng sẽ hiện ra, hiển thị rõ giá tiền và số lượng tồn kho còn lại để nhân viên biết.
*   **Giỏ hàng:** Các món đã chọn sẽ được đưa vào giỏ hàng bên trái, tự động tính tổng tiền tạm tính.

> *[Hình 4.18b: Giao diện chọn món và thêm vào giỏ hàng]*

**c. Xử lý thanh toán khách vãng lai**
Đối với khách mua mang về hoặc khách vãng lai, hệ thống hỗ trợ thanh toán nhanh mà không cần gắn với lịch đặt sân. Nếu nhân viên quên chọn khách hàng, hệ thống sẽ hiển thị cảnh báo nhắc nhở để đảm bảo tích điểm chính xác, hoặc cho phép tiếp tục thanh toán với tư cách "Khách vãng lai".

> *[Hình 4.18c: Cảnh báo xác nhận khi thanh toán cho khách vãng lai]*

**d. Xuất hóa đơn bán lẻ**
Sau khi hoàn tất thanh toán, hệ thống sẽ xuất hóa đơn bán lẻ chi tiết, bao gồm thông tin đơn vị, danh sách mặt hàng, đơn giá, số lượng và tổng tiền thanh toán.

> *[Hình 4.18d: Mẫu hóa đơn bán lẻ dịch vụ được in ra từ hệ thống]*

---

### 4.4.7. Chức năng Quản lý Nhân viên (Staff Management)

*Lưu ý: Chức năng này được bảo mật và chỉ dành riêng cho tài khoản có quyền Quản trị viên (Admin).*

**Quản lý hồ sơ nhân sự**
Giao diện cung cấp công cụ toàn diện để quản lý đội ngũ nhân viên:
*   **Danh sách nhân viên:** Hiển thị bảng thông tin tóm tắt gồm Mã NV, Họ tên, Giới tính, Ngày sinh, SĐT, Tài khoản đăng nhập, Mức lương cơ bản và Ca làm việc.
*   **Form nhập liệu chi tiết:** Panel bên phải cho phép Admin thêm mới hoặc cập nhật thông tin nhân viên. Các trường dữ liệu quan trọng như Tên đăng nhập, Mật khẩu, Lương và Ca làm việc được quản lý chặt chẽ.
*   **Phân quyền:** Hệ thống cho phép thiết lập vai trò (Role) cho từng tài khoản, đảm bảo nhân viên chỉ truy cập được các chức năng được cho phép.

> *[Hình 4.19: Giao diện Quản lý Nhân viên và Phân quyền hệ thống]*

---

### 4.4.8. Chức năng Thống kê & Báo cáo (Statistics)

Hệ thống cung cấp bộ công cụ phân tích dữ liệu mạnh mẽ, giúp chủ sân nắm bắt tình hình kinh doanh theo thời gian thực. Module thống kê được chia thành 4 tab chính:

**a. Thống kê Doanh thu**
Tab này cung cấp cái nhìn tổng quan về dòng tiền:
*   **Tổng quan:** Hiển thị 3 chỉ số KPI chính: Tổng doanh thu, Doanh thu từ tiền sân và Doanh thu từ dịch vụ.
*   **Biểu đồ cột:** Thể hiện biến động doanh thu theo từng ngày trong tháng, giúp nhận diện các ngày cao điểm.
*   **Biểu đồ tròn:** Phân tích cơ cấu nguồn thu (tỷ lệ % giữa tiền sân, đồ uống, đồ ăn, thiết bị...), giúp chủ sân biết mảng nào đang mang lại lợi nhuận chính.

> *[Hình 4.20a: Dashboard thống kê doanh thu và biểu đồ phân tích nguồn thu]*

**b. Thống kê Hiệu suất Đặt sân**
Phân tích hành vi đặt sân để tối ưu hóa vận hành:
*   **Trạng thái đơn:** Thống kê số lượng đơn hoàn thành vs đơn hủy.
*   **Tần suất theo ngày:** Biểu đồ cột cho thấy ngày nào trong tuần (Thứ 2 - CN) đông khách nhất.
*   **Khung giờ vàng:** Biểu đồ phân bố booking theo khung giờ (ví dụ: 18h-20h thường cao điểm nhất).
*   **Top sân:** Bảng xếp hạng các sân được đặt nhiều nhất để có kế hoạch bảo trì phù hợp.

> *[Hình 4.20b: Phân tích hiệu suất đặt sân theo khung giờ và loại sân]*

**c. Thống kê Khách hàng**
Hỗ trợ ra quyết định Marketing và chăm sóc khách hàng:
*   **Tăng trưởng:** Theo dõi số lượng khách hàng mới so với khách hàng cũ quay lại.
*   **Top chi tiêu:** Bảng xếp hạng những khách hàng "VIP" đóng góp nhiều doanh thu nhất.
*   **Phân hạng:** Thống kê tỷ lệ khách hàng theo hạng thành viên (Thường, Đồng, Bạc, Vàng...).

> *[Hình 4.20c: Báo cáo phân tích tệp khách hàng và xếp hạng thành viên]*

**d. Thống kê Dịch vụ**
Giúp tối ưu hóa nhập hàng và quản lý kho:
*   **Doanh số theo nhóm:** Hiển thị doanh thu riêng của từng nhóm hàng (Đồ uống, Đồ ăn, Thiết bị).
*   **Best-sellers:** Bảng xếp hạng các mặt hàng bán chạy nhất (ví dụ: Nước suối, Sting, Mì tôm...) để chủ sân có kế hoạch nhập hàng kịp thời.

> *[Hình 4.20d: Thống kê hiệu quả kinh doanh dịch vụ và các mặt hàng bán chạy]*

---

### 4.4.9. Chức năng Quản lý Hóa đơn (Payment)

Xử lý thanh toán và in hóa đơn cho khách hàng sau khi đá xong.

**Thao tác:**
1.  Chọn lịch đặt sân cần thanh toán.
2.  Hệ thống tự động tính tiền giờ + tiền dịch vụ đã dùng.
3.  Nhấn **"Thanh toán"** để hoàn tất và xuất hóa đơn.

> *[Hình 4.21: Giao diện Thanh toán và Xuất hóa đơn]*
