# CHƯƠNG 5: KẾT LUẬN VÀ HƯỚNG PHÁT TRIỂN

## 5.1. Kết luận và Đánh giá chi tiết

Qua quá trình nghiên cứu, thiết kế và xây dựng phần mềm **Football Field Manager**, nhóm thực hiện đã hoàn thành mục tiêu đề ra của đồ án PBL2. Dưới đây là những đánh giá chi tiết và chuyên sâu về kết quả đạt được:

### 5.1.1. Đánh giá về mặt Kỹ thuật và Công nghệ
*   **Kiến trúc phần mềm:** Hệ thống được xây dựng dựa trên mô hình hướng đối tượng (OOP) kết hợp với kiến trúc phân lớp (Layered Architecture). Việc tách biệt rõ ràng giữa lớp Giao diện (View), lớp Xử lý nghiệp vụ (Controller/Manager) và lớp Dữ liệu (Model) giúp mã nguồn trong sáng, dễ bảo trì và mở rộng.
*   **Cấu trúc dữ liệu:** Nhóm đã vận dụng linh hoạt các cấu trúc dữ liệu phù hợp cho từng bài toán cụ thể:
    *   *Danh sách liên kết (Linked List):* Sử dụng để quản lý danh sách sân bóng và đơn hàng, giúp tối ưu việc thêm/xóa phần tử động.
    *   *Mảng động (Vector/Dynamic Array):* Sử dụng cho danh sách nhân viên và khách hàng để truy xuất ngẫu nhiên nhanh chóng.
*   **Kỹ thuật lập trình Qt:** Khai thác hiệu quả các tính năng mạnh mẽ của Qt Framework như Signal & Slot cơ chế để xử lý sự kiện, Model/View để hiển thị dữ liệu bảng, và QSS để tùy biến giao diện.

### 5.1.2. Đánh giá về mặt Chức năng và Nghiệp vụ
*   **Độ hoàn thiện:** Phần mềm đã bao phủ được toàn bộ quy trình nghiệp vụ của một sân bóng đá mini: từ lúc khách đặt sân, nhận sân, sử dụng dịch vụ đi kèm, đến khi thanh toán và xuất hóa đơn.
*   **Tính chính xác:** Các thuật toán tính tiền giờ, tính tổng hóa đơn và thống kê doanh thu đã được kiểm thử kỹ lưỡng, đảm bảo độ chính xác tuyệt đối, xử lý đúng các trường hợp khung giờ đặc biệt (giờ vàng, ngày lễ).
*   **Hiệu năng:** Tốc độ phản hồi của ứng dụng nhanh, thao tác chuyển đổi giữa các màn hình mượt mà. Việc đọc/ghi file dữ liệu được tối ưu hóa để không gây treo giao diện.

### 5.1.3. Những hạn chế tồn tại
Mặc dù đã đạt được những kết quả tích cực, đồ án vẫn còn một số điểm hạn chế cần nhìn nhận khách quan:
*   **Hệ quản trị cơ sở dữ liệu:** Việc sử dụng File (CSV/Text) để lưu trữ dữ liệu là giải pháp tạm thời cho quy mô nhỏ. Khi dữ liệu lớn (Big Data), phương pháp này sẽ bộc lộ nhược điểm về tốc độ truy vấn và khó khăn trong việc bảo đảm tính toàn vẹn dữ liệu (Data Integrity).
*   **Bảo mật thông tin:** Cơ chế lưu trữ mật khẩu và thông tin cá nhân chưa được mã hóa mạnh (Encryption). Phân quyền người dùng mới chỉ dừng lại ở mức cơ bản (Admin/Staff).
*   **Khả năng mở rộng mạng:** Ứng dụng hiện tại là Desktop App chạy cục bộ (Standalone). Chưa hỗ trợ mô hình Client-Server để nhiều máy tính cùng kết nối và đồng bộ dữ liệu theo thời gian thực.

## 5.2. Hướng phát triển

Dựa trên những phân tích về ưu điểm và hạn chế, nhóm đề xuất lộ trình phát triển tiếp theo để nâng cấp sản phẩm thành một giải pháp thương mại hoàn chỉnh:

1.  **Chuyển đổi sang SQL Database:**
    *   Tích hợp hệ quản trị cơ sở dữ liệu như **SQLite** (cho bản Offline) hoặc **MySQL/PostgreSQL** (cho bản Online).
    *   Sử dụng SQL để thực hiện các truy vấn thống kê phức tạp và báo cáo động.

2.  **Phát triển Hệ sinh thái Đa nền tảng (Cross-platform Ecosystem):**
    *   **Mobile App (Flutter/React Native):** Cho phép khách hàng tìm sân trống, đặt sân và thanh toán online ngay trên điện thoại.
    *   **Web Admin Dashboard:** Giúp chủ sân quản lý từ xa, xem báo cáo doanh thu thời gian thực trên mọi thiết bị có trình duyệt.

3.  **Tích hợp Công nghệ mới:**
    *   **Thanh toán điện tử:** Tích hợp API của Momo, VNPay, ZaloPay để hỗ trợ thanh toán không tiền mặt (Cashless).
    *   **AI/Machine Learning:** Phân tích dữ liệu lịch sử đặt sân để dự đoán nhu cầu, gợi ý giá linh hoạt (Dynamic Pricing) vào giờ cao điểm.

4.  **Nâng cao trải nghiệm người dùng (UX):**
    *   Cải thiện giao diện với các hiệu ứng đồ họa mượt mà hơn.
    *   Thêm chế độ Dark Mode.
    *   Hỗ trợ đa ngôn ngữ (i18n) để mở rộng thị trường.

## 5.3. Lời kết

Đồ án **"Xây dựng phần mềm quản lý sân bóng đá"** không chỉ là một sản phẩm phần mềm, mà còn là kết quả của quá trình học tập, nghiên cứu và làm việc nhóm nghiêm túc. Qua đồ án này, nhóm đã tích lũy được những kinh nghiệm quý báu về quy trình phát triển phần mềm (SDLC), kỹ năng giải quyết vấn đề và tư duy lập trình chuyên nghiệp. Đây sẽ là hành trang vững chắc cho các thành viên trên con đường sự nghiệp sau này.
