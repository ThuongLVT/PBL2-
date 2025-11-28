# 📊 Kế Hoạch Phát Triển Chức Năng Thống Kê - Statistics Feature Development Plan

## 📋 Tổng Quan Dự Án

**Hệ Thống Quản Lý Sân Bóng Đá** (Football Field Management System) là ứng dụng desktop được xây dựng bằng **Qt6 + C++** với kiến trúc OOP thuần túy.

### Các Entity Chính:

- **Khách hàng (KhachHang)**: Quản lý thông tin khách hàng, hạng thành viên (Thường, Đồng, Bạc, Vàng, Kim Cương)
- **Sân (San)**: Sân 5 người, sân 7 người với các trạng thái (Hoạt động, Ngừng, Bảo trì)
- **Đặt sân (DatSan)**: Booking sân với khung giờ, tiền cọc, dịch vụ kèm theo
- **Dịch vụ (DichVu)**: Đồ uống, Đồ ăn, Thiết bị cho thuê
- **Đơn hàng dịch vụ (DonHangDichVu)**: Đơn hàng dịch vụ riêng lẻ

---

## 🎯 Mục Tiêu Chức Năng Thống Kê

Xây dựng module thống kê với **4 tab chính** và giao diện **hiện đại, đẹp mắt** như frontend web.

---

## 📑 Chi Tiết 4 Tab Thống Kê

### Tab 1: 💰 DOANH THU (Revenue Analytics)

#### 1.1 Summary Cards (Thẻ tổng quan)

```
┌─────────────────────────────────────────────────────────────────────┐
│  💵 Tổng Doanh Thu    │  📈 Doanh Thu Sân    │  🛒 Doanh Thu DV     │
│  150.000.000 VND      │  120.000.000 VND     │  30.000.000 VND      │
│  ↑ 12% so với tháng   │  ↑ 8% so với tháng   │  ↑ 25% so với tháng  │
└─────────────────────────────────────────────────────────────────────┘
```

#### 1.2 Biểu đồ

| Loại           | Mô tả                                                  |
| -------------- | ------------------------------------------------------ |
| **Line Chart** | Doanh thu theo ngày/tuần/tháng (có thể chọn)           |
| **Bar Chart**  | So sánh doanh thu sân vs dịch vụ                       |
| **Pie Chart**  | Phân bổ nguồn thu (Tiền sân, Đồ uống, Đồ ăn, Thiết bị) |
| **Area Chart** | Xu hướng tăng trưởng doanh thu                         |

#### 1.3 Metrics Chi Tiết

- Doanh thu trung bình/ngày
- Doanh thu theo khung giờ (giờ vàng vs giờ thường)
- So sánh với kỳ trước (% tăng/giảm)
- Doanh thu theo phương thức thanh toán

#### 1.4 Filter Options

- Theo ngày/tuần/tháng/năm
- Theo khoảng thời gian tùy chọn
- Export báo cáo (CSV/PDF)

---

### Tab 2: ⚽ BOOKING & SÂN (Booking & Field Analytics)

#### 2.1 Summary Cards

```
┌─────────────────────────────────────────────────────────────────────┐
│  📅 Tổng Booking      │  ✅ Hoàn Thành        │  ❌ Đã Hủy           │
│  250                  │  220 (88%)            │  30 (12%)            │
│  ↑ 15% so với tháng   │                       │                      │
└─────────────────────────────────────────────────────────────────────┘
```

#### 2.2 Biểu đồ

| Loại               | Mô tả                                               |
| ------------------ | --------------------------------------------------- |
| **Heatmap Chart**  | Booking theo ngày trong tuần + khung giờ (giờ vàng) |
| **Donut Chart**    | Tỷ lệ booking theo loại sân (5 người vs 7 người)    |
| **Stacked Bar**    | Trạng thái booking theo tháng                       |
| **Horizontal Bar** | Top sân được đặt nhiều nhất                         |

#### 2.3 Bảng Xếp Hạng

```
🏆 TOP 5 SÂN ĐƯỢC ĐẶT NHIỀU NHẤT
┌──────┬────────────────┬───────────┬─────────────┬────────────┐
│ Rank │ Tên Sân        │ Loại     │ Lượt Booking│ Doanh Thu  │
├──────┼────────────────┼───────────┼─────────────┼────────────┤
│ 🥇   │ Sân B7-01      │ 7 người  │ 45          │ 18.000.000 │
│ 🥈   │ Sân A5-01      │ 5 người  │ 38          │ 9.500.000  │
│ 🥉   │ Sân B5-01      │ 5 người  │ 35          │ 8.750.000  │
│ 4    │ Sân C7-01      │ 7 người  │ 28          │ 7.000.000  │
│ 5    │ Sân A5-02      │ 5 người  │ 25          │ 6.250.000  │
└──────┴────────────────┴───────────┴─────────────┴────────────┘
```

#### 2.4 Thống kê Khung Giờ

- **Giờ vàng** (18:00 - 21:00): Số booking, % tổng
- **Giờ thường**: Số booking, % tổng
- **Khung giờ hot nhất**: Hiển thị top 3

#### 2.5 Utilization Rate (Tỷ lệ sử dụng sân)

- Tỷ lệ sử dụng từng sân (progress bar)
- Trung bình tỷ lệ sử dụng toàn hệ thống
- Sân hiệu quả nhất / kém hiệu quả nhất

---

### Tab 3: 👥 KHÁCH HÀNG (Customer Analytics)

#### 3.1 Summary Cards

```
┌─────────────────────────────────────────────────────────────────────┐
│  👥 Tổng Khách Hàng   │  🆕 Khách Mới        │  🔄 Khách Quay Lại   │
│  500                  │  45 (tháng này)      │  180 (36%)           │
│  ↑ 10% so với tháng   │                       │                      │
└─────────────────────────────────────────────────────────────────────┘
```

#### 3.2 Biểu đồ

| Loại             | Mô tả                                                  |
| ---------------- | ------------------------------------------------------ |
| **Donut Chart**  | Phân bổ theo hạng (Thường, Đồng, Bạc, Vàng, Kim Cương) |
| **Line Chart**   | Xu hướng khách hàng mới theo tháng                     |
| **Bar Chart**    | So sánh khách mới vs khách quay lại                    |
| **Funnel Chart** | Chuyển đổi hạng khách hàng                             |

#### 3.3 Bảng Xếp Hạng - TOP KHÁCH HÀNG TIỀM NĂNG

```
🏆 TOP 10 KHÁCH HÀNG CHI TIÊU CAO NHẤT
┌──────┬─────────────────┬─────────────┬───────────┬─────────────────┐
│ Rank │ Tên Khách Hàng  │ Hạng        │ Số Lần Đặt│ Tổng Chi Tiêu   │
├──────┼─────────────────┼─────────────┼───────────┼─────────────────┤
│ 🥇   │ Lê Viết C       │ 💎 Đồng    │ 12        │ 1.115.000       │
│ 🥈   │ Đinh Khang D    │ ⭐ Thường  │ 8         │ 700.000         │
│ 🥉   │ Nguyễn Văn A    │ ⭐ Thường  │ 5         │ 430.000         │
└──────┴─────────────────┴─────────────┴───────────┴─────────────────┘
```

#### 3.4 Customer Insights

- **Lifetime Value (CLV)** trung bình
- **Tần suất đặt sân** trung bình
- **Thời gian giữa các lần đặt** trung bình
- **Khách hàng rời bỏ** (không đặt > 3 tháng)

#### 3.5 Phân tích Hạng Thành Viên

```
🎖️ PHÂN BỔ HẠNG KHÁCH HÀNG
┌─────────────┬────────┬──────────────┬─────────────────┐
│ Hạng        │ Số KH  │ % Tổng       │ Avg Chi Tiêu    │
├─────────────┼────────┼──────────────┼─────────────────┤
│ ⭐ Thường   │ 350    │ 70%          │ 200.000         │
│ 🥉 Đồng     │ 100    │ 20%          │ 2.500.000       │
│ 🥈 Bạc      │ 35     │ 7%           │ 7.000.000       │
│ 🥇 Vàng     │ 12     │ 2.4%         │ 15.000.000      │
│ 💎 Kim Cương│ 3      │ 0.6%         │ 25.000.000      │
└─────────────┴────────┴──────────────┴─────────────────┘
```

---

### Tab 4: 🛒 DỊCH VỤ (Service Analytics)

#### 4.1 Summary Cards

```
┌─────────────────────────────────────────────────────────────────────┐
│  🛒 Tổng Đơn DV       │  🥤 Đồ Uống          │  🍔 Đồ Ăn            │
│  320                  │  65%                 │  25%                 │
│  ↑ 20% so với tháng   │                       │  👕 Thiết bị: 10%   │
└─────────────────────────────────────────────────────────────────────┘
```

#### 4.2 Biểu đồ

| Loại           | Mô tả                                                |
| -------------- | ---------------------------------------------------- |
| **Pie Chart**  | Phân bổ theo loại dịch vụ (Đồ uống, Đồ ăn, Thiết bị) |
| **Bar Chart**  | Top dịch vụ bán chạy nhất                            |
| **Line Chart** | Xu hướng bán hàng theo thời gian                     |
| **Treemap**    | Doanh thu theo từng dịch vụ                          |

#### 4.3 Bảng Xếp Hạng - TOP DỊCH VỤ BÁN CHẠY

```
🏆 TOP 10 DỊCH VỤ BÁN CHẠY NHẤT
┌──────┬─────────────────┬───────────┬───────────┬─────────────────┐
│ Rank │ Tên Dịch Vụ     │ Loại      │ SL Bán    │ Doanh Thu       │
├──────┼─────────────────┼───────────┼───────────┼─────────────────┤
│ 🥇   │ Áo Bib          │ Thiết bị  │ 50        │ 250.000         │
│ 🥈   │ Sting dâu       │ Đồ uống  │ 40        │ 720.000         │
│ 🥉   │ Redbull         │ Đồ uống  │ 35        │ 700.000         │
│ 4    │ Nước suối       │ Đồ uống  │ 35        │ 350.000         │
│ 5    │ Xúc xích nướng  │ Đồ ăn    │ 30        │ 450.000         │
└──────┴─────────────────┴───────────┴───────────┴─────────────────┘
```

#### 4.4 Inventory Analysis (Phân tích tồn kho)

- Sản phẩm sắp hết (< 10)
- Sản phẩm bán chậm (không bán 30 ngày)
- Tỷ lệ quay vòng tồn kho

#### 4.5 Cross-selling Analysis

- Dịch vụ hay đi kèm booking nhất
- Combo phổ biến nhất
- Gợi ý combo mới

---

## 🎨 UI/UX Design Guidelines

### Design System

#### Color Palette (Modern & Professional)

```css
/* Primary Colors */
--primary: #3b82f6; /* Blue - Main accent */
--primary-dark: #1d4ed8;
--primary-light: #93c5fd;

/* Secondary Colors */
--success: #10b981; /* Green - Positive */
--warning: #f59e0b; /* Orange - Warning */
--danger: #ef4444; /* Red - Negative */
--info: #06b6d4; /* Cyan - Info */

/* Neutral Colors */
--background: #f8fafc; /* Light gray background */
--card-bg: #ffffff;
--text-primary: #1e293b;
--text-secondary: #64748b;
--border: #e2e8f0;

/* Gradient */
--gradient-primary: linear-gradient(135deg, #3b82f6 0%, #8b5cf6 100%);
--gradient-success: linear-gradient(135deg, #10b981 0%, #059669 100%);
```

#### Typography

```css
/* Font Family */
font-family: 'Inter', 'Segoe UI', sans-serif;

/* Sizes */
--h1: 28px (Bold)
--h2: 24px (SemiBold)
--h3: 20px (SemiBold)
--body: 14px (Regular)
--small: 12px (Regular)
```

### Component Design

#### Summary Card (Thẻ tổng quan)

```
┌──────────────────────────────────┐
│  icon    Title                   │
│          Value (bold, large)     │
│          ↑12% vs last month      │
│  ▓▓▓▓▓▓▓▓▓░░░ Progress          │
└──────────────────────────────────┘
```

- Shadow: `0 4px 6px rgba(0,0,0,0.1)`
- Border-radius: 12px
- Hover: scale(1.02) + shadow increase

#### Tab Navigation

```
┌────────────────────────────────────────────────────────────────┐
│  [💰 Doanh Thu]  [⚽ Booking & Sân]  [👥 Khách Hàng]  [🛒 Dịch Vụ] │
└────────────────────────────────────────────────────────────────┘
```

- Active tab: gradient underline
- Hover: background lighten

#### Chart Container

- Title on top-left
- Filter/Period selector on top-right
- Legend at bottom
- Responsive sizing

#### Ranking Table

- Sticky header
- Alternating row colors
- Medal icons for top 3
- Hover highlight effect
- Sortable columns

### Animations

- Page transition: fade-in 200ms
- Card hover: scale + shadow
- Chart render: sequential animation
- Number counting animation

---

## 📐 Qt Implementation Guide

### Recommended Libraries

| Library         | Purpose                          |
| --------------- | -------------------------------- |
| **Qt Charts**   | Biểu đồ (Line, Bar, Pie, Donut)  |
| **QCustomPlot** | Biểu đồ nâng cao (Heatmap, Area) |
| **Qt SVG**      | Icons và illustrations           |

### File Structure

```
UI/
├── Pages/
│   └── Statistics/                            # 10 files total (5 .h + 5 .cpp)
│       ├── StatisticsPage.h/.cpp              # Main container với TabWidget
│       ├── RevenueTab.h/.cpp                  # Tab 1: Doanh thu
│       ├── BookingFieldTab.h/.cpp             # Tab 2: Booking & Sân
│       ├── CustomerTab.h/.cpp                 # Tab 3: Khách hàng
│       └── ServiceTab.h/.cpp                  # Tab 4: Dịch vụ
│
├── Components/                                # Shared UI Components
│   ├── SummaryCard.h/.cpp                     # Thẻ summary với icon, value, trend
│   ├── RankingTable.h/.cpp                    # Bảng xếp hạng với medal icons
│   ├── DateRangePicker.h/.cpp                 # Chọn khoảng thời gian
│   └── ChartContainer.h/.cpp                  # Container wrapper cho charts
│
└── Charts/                                    # Chart Components
    ├── LineChart.h/.cpp                       # Line/Area chart
    ├── BarChart.h/.cpp                        # Bar/Stacked bar chart
    ├── PieChart.h/.cpp                        # Pie chart
    ├── DonutChart.h/.cpp                      # Donut chart
    └── HeatmapChart.h/.cpp                    # Heatmap cho khung giờ
```

### Core Statistics Classes Enhancement

```
Core/
└── ThongKe/
    ├── ThongKe.h/.cpp                        # Base class ✅
    ├── ThongKeDoanhThu.h/.cpp               # ✅ Enhanced
    ├── ThongKeKhachHang.h/.cpp              # ✅ Enhanced
    ├── ThongKeBooking.h/.cpp                # ✅ Created
    └── ThongKeDichVu.h/.cpp                 # ✅ Created
```

---

## 📋 Development Phases

### Phase 1: Core Statistics Enhancement (Week 1) ✅ COMPLETED

- [x] Enhance `ThongKeDoanhThu` với methods mới
- [x] Enhance `ThongKeKhachHang` với metrics mới
- [x] Create `ThongKeBooking` class
- [x] Create `ThongKeDichVu` class
- [ ] Unit tests cho statistics

### Phase 2: UI Components (Week 2) ✅ COMPLETED

- [x] Design và implement `SummaryCard`
- [x] Design và implement `RankingTable`
- [x] Design và implement `DateRangePicker`
- [x] Create base `ChartContainer`
- [x] Style với modern CSS

### Phase 3: Tab Implementation (Week 3-4) ✅ COMPLETED

- [x] Implement `RevenueTab`
- [x] Implement `BookingFieldTab`
- [x] Implement `CustomerTab`
- [x] Implement `ServiceTab`

### Phase 4: Charts Integration (Week 5) ✅ COMPLETED

- [x] Integrate Qt Charts (Pie, Donut, Bar, Line charts implemented)
- [x] Implement các loại chart (QPieSeries, QBarSeries, QLineSeries với Qt Charts)
- [x] Line Chart cho doanh thu theo ngày
- [x] Bar Chart so sánh kỳ này vs kỳ trước
- [x] Pie/Donut charts cho phân bổ dữ liệu
- [ ] Heatmap charts (QCustomPlot - optional)

### Phase 5: Polish & Testing (Week 6)

- [x] Bỏ hiệu ứng rung khi hover card
- [x] Cập nhật dữ liệu test để hiển thị đúng
- [ ] Performance optimization
- [ ] Edge cases handling
- [ ] Export functionality (CSV/PDF)
- [ ] Final UI polish

---

## 🎯 Success Metrics

| Metric            | Target                |
| ----------------- | --------------------- |
| Page load time    | < 500ms               |
| Chart render      | < 200ms               |
| Data accuracy     | 100%                  |
| Responsive        | Desktop & Tablet      |
| User satisfaction | UI modern, dễ sử dụng |

---

## 📚 References

### Design Inspiration

- [Tailwind UI Dashboard](https://tailwindui.com/components/application-ui/page-examples/dashboards)
- [Material Dashboard](https://www.creative-tim.com/product/material-dashboard)
- [Ant Design Charts](https://charts.ant.design/)

### Qt Resources

- [Qt Charts Examples](https://doc.qt.io/qt-6/qtcharts-examples.html)
- [Modern Qt Styling](https://doc.qt.io/qt-6/stylesheet-examples.html)

---

## ✍️ Author Notes

Document này là kế hoạch chi tiết cho việc phát triển module thống kê. Cần được review và adjust theo feedback từ team.

**Last Updated**: November 26, 2025
**Version**: 1.3 - Week 4-5 Completed (Charts Integration)
