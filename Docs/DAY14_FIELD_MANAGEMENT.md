# Day 14 - Field Management (Quản lý sân bóng)

## Overview

Complete field management system with modern 60/40 split layout, 4 clickable statistics cards, and full CRUD operations with CSV persistence.

## Implementation Date

November 12, 2025

## Features Implemented

### 1. Model Layer Updates

- **TrangThaiSan Enum**: Extended from 2 states to 3 states
  - `HOAT_DONG` - Active/operational
  - `NGUNG_HOAT_DONG` - Suspended/inactive
  - `BAO_TRI` - Under maintenance
- **San.h/cpp**: Added `ngungHoatDong()` method for status checking
- **QuanLySan**:
  - Added `demSanNgungHoatDong()` for counting suspended fields
  - Added `laySanNgungHoatDong()` for retrieving suspended fields list
  - Updated `parseTrangThai()` to handle 3 states

### 2. Data Layer

- **CSV Format Update**: Changed from zone-based codes (A501, B702) to sequential codes (SB01-SB30)
- **Sample Data**: 23 football fields distributed across 4 zones (A, B, C, D) and 2 field types (5, 7 players)
  - 22 fields: `Hoat dong` (Active)
  - 4 fields: `Bao tri` (Maintenance) - SB08, SB18, SB20, SB26, SB29
  - 4 fields: `Ngung hoat dong` (Suspended) - SB03, SB14, SB23, SB30
- **Pricing Structure**:
  - 5-player field: 200,000 VNĐ/hour
  - 7-player field: 400,000 VNĐ/hour
- **File Location**: `D:\\QT_PBL2\Data\san.csv`
- **CSV Loading**: Integrated into `HeThongQuanLy::docCSV()` method

### 3. UI Layer - 60/40 Split Layout

#### Left Panel (60% - Data View)

**Search & Filters:**

- 🔍 Search bar: Real-time text search by field name
- Combo box filter: Field type (5/7 players)
- Combo box filter: Zone (A/B/C/D)

**4 Clickable Statistics Cards:**
Cards follow CustomerManagementPage design with:

- Title on top (13px, gray)
- Large number in center (32px, bold, colored)
- White background with rounded corners
- Hover effect with blue border
- Click to filter table data

1. **Tổng số sân** (Total Fields) - Black text
2. **Sân hoạt động** (Active) - Green (#10b981)
3. **Sân bảo trì** (Maintenance) - Orange (#f59e0b)
4. **Sân ngừng hoạt động** (Suspended) - Red (#ef4444)

**Data Table (7 columns):**
| Column | Alignment | Description |
|--------|-----------|-------------|
| Mã sân | Center | Field code (SB01, SB02...) |
| Tên sân | Left | Field name |
| Loại sân | Center | Field type (5/7 người) |
| Khu vực | Center | Zone (Khu A/B/C/D) |
| Giá thuê | Right | Price in VNĐ |
| Trạng thái | Center | Status with color coding |
| Ghi chú | Left | Notes/remarks |

**Table Features:**

- Single row selection
- Color-coded status (green/orange/red)
- Click row to load details to right panel
- White background with rounded corners
- Light gray gridlines

#### Right Panel (40% - Detail Form)

**Background:** White with 12px rounded corners (distinct from light gray content background)

**Form Fields:**

- Tên sân (Text input)
- Loại sân (Combo box: 5/7 người)
- Khu vực (Combo box: A/B/C/D)
- Trạng thái (Combo box: Hoạt động/Ngừng hoạt động/Bảo trì)
- Giá thuê/giờ (Spin box with VNĐ suffix)
- Ghi chú (Text area, max 80px height)

**Action Buttons:**

- ➕ **Thêm sân mới** (Green #10b981) - Always enabled
- 💾 **Lưu** (Blue #3b82f6) - Enabled when editing
- ⚙️ **Bảo trì** (Orange #f59e0b) - Enabled when field selected
- 🗑️ **Xoá** (Red #ef4444) - Enabled when field selected

**Form Modes:**

1. **Readonly (Default)**: All fields disabled, only "Thêm sân mới" button enabled
2. **New Mode**: All fields enabled when clicking "Thêm sân mới"
3. **Edit Mode**: Fields enabled, can modify existing field data

### 4. MaintenanceDialog

**Purpose:** Confirmation dialog for putting field into maintenance status

**Features:**

- Title: "⚙️ XÁC NHẬN ĐƯA SÂN VÀO BẢO TRÌ"
- Readonly field information display:
  - Mã sân (Field code)
  - Tên sân (Field name)
  - Loại sân (Field type)
  - Khu vực (Zone)
- Text area for maintenance reason (required)
- Placeholder: "Nhập lý do bảo trì (ví dụ: Thay cỏ nhân tạo, Sửa chữa đèn chiếu sáng...)"
- Two action buttons:
  - ✓ Xác nhận bảo trì (Orange)
  - ✗ Huỷ (Gray)

**Behavior:**

- Opens when clicking "⚙️ Bảo trì" button
- Validates that reason is not empty
- Updates field status to `BAO_TRI`
- Updates ghiChu with maintenance reason
- Saves to CSV file
- Refreshes table and form

### 5. CRUD Operations

#### Create (Add New Field)

1. Click "➕ Thêm sân mới" button
2. Form enters edit mode, fields become editable
3. Fill in: Tên sân, Loại sân, Khu vực, Trạng thái, Giá thuê, Ghi chú
4. Click "💾 Lưu" to save
5. Auto-generates field code using `taoMaSanTuDong()`
6. Creates San object and adds to QuanLySan
7. Saves to CSV
8. Refreshes table and stats cards

#### Read (View Details)

1. Click any row in table
2. Field details load to right panel form
3. Form is readonly by default
4. Stats cards show current counts
5. Table updates when clicking stats cards

#### Update (Edit Field)

1. Select field from table
2. Click any editable field in form
3. Modify field values
4. Click "💾 Lưu" to save changes
5. Updates San object in QuanLySan
6. Saves to CSV
7. Refreshes table

#### Delete (Remove Field)

1. Select field from table
2. Click "🗑️ Xoá" button
3. Confirmation dialog appears
4. Click "Yes" to confirm deletion
5. Removes from QuanLySan
6. Saves to CSV
7. Clears form and refreshes table

#### Maintenance (Special Update)

1. Select field from table
2. Click "⚙️ Bảo trì" button
3. MaintenanceDialog opens showing field info
4. Enter maintenance reason (required)
5. Click "✓ Xác nhận bảo trì"
6. Field status changes to `BAO_TRI`
7. Maintenance reason saved to ghiChu
8. Saves to CSV
9. Refreshes table with updated status

### 6. Filtering & Search

**Search Functionality:**

- Real-time text search as user types
- Case-insensitive matching
- Searches in field name only
- Updates table immediately

**Filter Combinations:**
Filters work together (cascade):

1. Stats card filter (Total/Active/Maintenance/Suspended)
2. Field type filter (5/7 players)
3. Zone filter (A/B/C/D)
4. Search text filter

**Stats Card Click Behavior:**

- **Tổng số sân**: Shows all 30 fields
- **Sân hoạt động**: Shows only HOAT_DONG fields (22)
- **Sân bảo trì**: Shows only BAO_TRI fields (4)
- **Sân ngừng hoạt động**: Shows only NGUNG_HOAT_DONG fields (4)

## Technical Implementation

### Files Created/Modified

#### Core Layer

- `Core/Models/San.h` - Added `ngungHoatDong()` method
- `Core/Models/San.cpp` - Implemented status checking methods
- `Core/QuanLy/QuanLySan.h` - Added `demSanNgungHoatDong()`, `laySanNgungHoatDong()`
- `Core/QuanLy/QuanLySan.cpp` - Implemented new methods, updated `parseTrangThai()`
- `Core/QuanLy/HeThongQuanLy.cpp` - Added `quanLySan->loadFromCSV()` to `docCSV()`

#### Data Layer

- `Data/san.csv` - Complete rewrite with 30 fields in new format

#### UI Layer

- `UI/Pages/Field/FieldManagementPage.h` - Complete redesign with new layout
- `UI/Pages/Field/FieldManagementPage.cpp` - Full rewrite (879 lines)
- `UI/Dialogs/MaintenanceDialog.h` - New dialog for maintenance
- `UI/Dialogs/MaintenanceDialog.cpp` - Dialog implementation

#### Build Configuration

- `CMakeLists.txt` - Added MaintenanceDialog sources and headers

### Key Qt Components Used

- `QSplitter` - 60/40 split layout
- `QFrame` - Stats cards with custom styling
- `QTableWidget` - 7-column data table
- `QLineEdit` - Search bar and text inputs
- `QComboBox` - Filters and dropdowns
- `QSpinBox` - Price input with suffix
- `QTextEdit` - Notes and reason input
- `QPushButton` - Action buttons with custom colors
- `QFormLayout` - Right panel form layout
- `QEvent::MouseButtonPress` - Stats card click detection

### Styling Details

**Color Palette:**

- Primary Blue: #3b82f6
- Success Green: #10b981
- Warning Orange: #f59e0b
- Danger Red: #ef4444
- Gray Shades: #374151 (dark), #6b7280 (medium), #d1d5db (light), #e5e7eb (border)
- Background: #f5f5f5 (light gray content area), white (panels/cards)

**Typography:**

- Stats card title: 13px, #666666
- Stats card number: 32px, bold, colored
- Form title: 16px, bold
- Regular text: 13px

**Spacing:**

- Card padding: 20px
- Form spacing: 15px between fields
- Button spacing: 10px between buttons
- Stats cards: 15px gap

## Testing Checklist

✅ **Data Loading:**

- [x] CSV file loads on application start
- [x] 30 fields displayed in table
- [x] Stats cards show correct counts (30, 22, 4, 4)

✅ **Search & Filter:**

- [x] Search by field name works real-time
- [x] Field type filter works (5/7)
- [x] Zone filter works (A/B/C/D)
- [x] Stats cards filter table on click
- [x] Multiple filters work together

✅ **CRUD Operations:**

- [x] Add new field with auto-generated code
- [x] View field details on row click
- [x] Edit existing field
- [x] Delete field with confirmation
- [x] All operations save to CSV

✅ **Maintenance:**

- [x] Maintenance dialog opens
- [x] Field info displays correctly
- [x] Reason field is required
- [x] Status updates to BAO_TRI
- [x] Reason saves to ghiChu
- [x] Changes persist to CSV

✅ **UI/UX:**

- [x] 60/40 split layout
- [x] Stats cards styled like CustomerManagement
- [x] Right panel has white rounded background
- [x] Buttons have proper colors and hover effects
- [x] Table has proper column alignment
- [x] Status colors display correctly
- [x] Form modes work (readonly/edit/new)

## Usage Instructions

### Adding a New Field

1. Navigate to Field Management page
2. Click "➕ Thêm sân mới"
3. Fill in field information
4. Click "💾 Lưu"

### Viewing All Active Fields

1. Click "Sân hoạt động" stats card (green)
2. Table filters to show only active fields

### Putting Field Into Maintenance

1. Click field row in table
2. Click "⚙️ Bảo trì" button
3. Enter maintenance reason
4. Click "✓ Xác nhận bảo trì"

### Searching for a Field

1. Type field name in search bar
2. Table updates in real-time

### Filtering by Zone and Type

1. Select zone from "Khu vực" dropdown
2. Select type from "Loại sân" dropdown
3. Table shows matching fields

## Known Limitations

- Cannot delete or maintain fields with active bookings (validation not yet implemented)
- No history log for maintenance changes
- No bulk operations support

## Future Enhancements

- Add validation to prevent deletion of fields with future bookings
- Add maintenance history log
- Add field availability calendar view
- Add bulk import/export functionality
- Add field performance statistics (usage rate, revenue)

## Dependencies

- Qt 6.8.1/6.9.3 (QtWidgets)
- CMake 3.16+
- Ninja build system
- C++17 standard

## Build Commands

```bash
cd D:\\QT_PBL2\build
ninja
```

## Run Application

```bash
cd D:\\QT_PBL2\build\bin
.\FootballFieldManager.exe
```

## Notes

- CSV file must exist before loading
- Default data directory: `D:/QT_PBL2/Data`
- Field codes auto-generated in format: SB01, SB02, etc.
- All changes persist immediately to CSV file
- Stats cards are clickable for quick filtering
