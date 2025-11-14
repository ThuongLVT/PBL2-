# QT FOOTBALL - HƯỚNG DẪN TÔ MÀU UI

## 📋 Tổng Quan

Dự án đã được **thống nhất hoàn toàn** về cách tô màu UI để tránh xung đột và dễ bảo trì.

## ✅ Quy Tắc Tô Màu

### 🎨 **QUY TẮC VÀNG: Tất cả màu sắc được định nghĩa trong `app.qss`**

```
📁 QT_FOOTBALL/UI/Resources/styles/
   └── app.qss  <-- Tất cả màu sắc ở đây
```

### ❌ KHÔNG được dùng `setStyleSheet()` trong file .cpp

**KHÔNG:**

```cpp
// ❌ SAI - Không tô màu trong file .cpp
button->setStyleSheet("background-color: #16a34a; color: white;");
widget->setStyleSheet("border: 1px solid red;");
```

**CÓ:**

```cpp
// ✅ ĐÚNG - Chỉ set objectName, màu sẽ lấy từ app.qss
button->setObjectName("primaryButton");
widget->setObjectName("mainContainer");
```

## 📂 Cấu Trúc Style

### 1. File `app.qss` - Nguồn màu duy nhất

- Tất cả màu sắc của app
- Sử dụng color palette từ React design
- Dễ thay đổi theme

### 2. Files `.cpp` - Chỉ logic

- Chỉ tạo widget và set `objectName`
- KHÔNG có `setStyleSheet()`
- Màu sắc tự động lấy từ QSS

## 🎨 Color Palette (từ app.qss)

```css
/* Primary Colors */
--primary-green:       #16a34a  /* Màu chủ đạo */
--primary-green-dark:  #15803d  /* Hover states */
--primary-green-light: #22c55e  /* Accents */

/* Text Colors */
--text-dark:  #1f2937  /* Headers */
--text-gray:  #6b7280  /* Body text */
--text-light: #9ca3af  /* Placeholders */

/* Backgrounds */
--bg-gray:       #f9fafb
--bg-green-light: #f0fdf4
--border-gray:   #e5e7eb
```

## 📝 Cách Thêm Style Mới

### Bước 1: Thêm objectName trong .cpp

```cpp
// File: MyWidget.cpp
QPushButton *button = new QPushButton("Click me", this);
button->setObjectName("mySpecialButton");  // Đặt tên
```

### Bước 2: Thêm style trong app.qss

```css
/* File: app.qss */
QPushButton#mySpecialButton {
  background-color: #16a34a;
  color: white;
  border-radius: 8px;
  padding: 10px 20px;
}

QPushButton#mySpecialButton:hover {
  background-color: #15803d;
}
```

## ✨ Ví Dụ Thực Tế

### Sidebar Buttons

```cpp
// Sidebar.cpp - ĐÚNG ✅
button->setObjectName("MenuButton");
// Màu được định nghĩa trong app.qss
```

```css
/* app.qss */
#MenuButton {
  background-color: transparent;
  color: white;
  border-left: 3px solid transparent;
}

#MenuButton:hover {
  background-color: rgba(255, 255, 255, 0.1);
}

#MenuButton:checked {
  background-color: rgba(255, 255, 255, 0.2);
  border-left: 3px solid white;
}
```

### Coming Soon Labels

```cpp
// Page.cpp - ĐÚNG ✅
label->setObjectName("comingSoonLabel");
// Màu được định nghĩa trong app.qss
```

```css
/* app.qss */
QLabel#comingSoonLabel {
  font-size: 20px;
  font-weight: bold;
  color: #6b7280;
}
```

## 🚫 Ngoại Lệ

### LoginDialog - Duy nhất được phép dùng inline style

```cpp
// LoginDialog.cpp - Ngoại lệ duy nhất
void LoginDialog::applyStyles() {
    setStyleSheet(styles);  // OK vì login dialog có background đặc biệt
}
```

**Lý do:**

- LoginDialog có custom paintEvent để vẽ background image
- Style rất khác biệt so với main app
- Là dialog độc lập, không chia sẻ style với app chính

## 📊 Tóm Tắt Thay Đổi

### ✅ Đã Làm

1. ✅ Chuyển tất cả styles từ `.cpp` sang `app.qss`
2. ✅ Xóa `applyStyles()` trong Sidebar.cpp
3. ✅ Xóa `applyStyles()` trong Header.cpp
4. ✅ Xóa `setStyleSheet()` trong MainWindow.cpp
5. ✅ Xóa `setStyleSheet()` trong tất cả Page files
6. ✅ Thêm `comingSoonLabel` style vào app.qss
7. ✅ Giữ nguyên LoginDialog (ngoại lệ)

### 🎯 Kết Quả

- **1 nguồn màu duy nhất**: `app.qss`
- **Không xung đột**: Không có style đè lên nhau
- **Dễ maintain**: Thay đổi màu ở 1 nơi
- **Rõ ràng**: Logic trong .cpp, style trong .qss

## 🔧 Maintenance

### Khi cần thay đổi màu

1. Mở `app.qss`
2. Tìm selector tương ứng
3. Thay đổi màu
4. Save → Rebuild app

### Khi thêm widget mới

1. Tạo widget trong .cpp
2. Set `objectName` cho widget
3. Thêm style vào `app.qss`
4. KHÔNG dùng `setStyleSheet()` trong .cpp

## 📚 Tài Liệu Liên Quan

- [QT_UI_GRAPHICS_GUIDE.md](../QT_UI_GRAPHICS_GUIDE.md) - Hướng dẫn UI tổng quan
- [WEEK4_SUMMARY.md](WEEK4_SUMMARY.md) - Tổng kết tuần 4

---

**Cập nhật:** 9/11/2025  
**Người thực hiện:** GitHub Copilot  
**Trạng thái:** ✅ Hoàn thành - Đã thống nhất 100%
