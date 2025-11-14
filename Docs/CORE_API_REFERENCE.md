# 📚 CORE API REFERENCE - QT_FOOTBALL

**For UI Developers**  
**Updated:** 8 Tháng 11, 2025

---

## 🎯 OVERVIEW

Document này hướng dẫn UI developers cách sử dụng Core API **trực tiếp** (không qua Bridge Layer).

**Import Header:**

```cpp
#include "Core/QuanLy/HeThongQuanLy.h"
```

**Access Singleton:**

```cpp
HeThongQuanLy *sys = HeThongQuanLy::getInstance();
```

---

## 🔌 HeThongQuanLy (SINGLETON)

### **Instance Access**

```cpp
HeThongQuanLy *sys = HeThongQuanLy::getInstance();
// OR
HeThongQuanLy *sys = HeThongQuanLy::getInstancePtr();
```

### **Get Managers**

```cpp
QuanLySan *fieldMgr = sys->layQuanLySan();
QuanLyKhachHang *customerMgr = sys->layQuanLyKhachHang();
QuanLyDichVu *serviceMgr = sys->layQuanLyDichVu();
QuanLyDatSan *bookingMgr = sys->layQuanLyDatSan();
QuanLyThanhToan *paymentMgr = sys->layQuanLyThanhToan();
BackupManager *backupMgr = sys->layBackupManager();
```

---

## ⚽ FIELD MANAGEMENT (Quản Lý Sân)

### **Headers:**

```cpp
#include "Core/Models/San.h"
#include "Core/Models/KhungGio.h"
```

### **Create Field**

```cpp
San *field = new San(
    "S01",              // maSan
    "Sân A1",           // tenSan
    LoaiSan::SAN_5,     // loai (SAN_5, SAN_7)
    300000              // giaThueGio (VND)
);

bool success = sys->themSan(field);
```

### **List All Fields**

```cpp
const MangDong<San*>& fields = sys->layDanhSachSan();

for (int i = 0; i < fields.size(); i++) {
    San *field = fields[i];

    std::string ma = field->getMaSan();
    std::string ten = field->getTenSan();
    LoaiSan loai = field->getLoaiSan();
    double gia = field->getGiaThueGio();
    TrangThaiSan trangThai = field->getTrangThai();
}
```

### **Find Field**

```cpp
San *field = sys->timSan("S01");

if (field != nullptr) {
    // Use field
}
```

### **Update Field**

```cpp
San *field = sys->timSan("S01");
if (field) {
    field->datTenSan("Sân A1 - VIP");
    field->datGiaThueGio(350000);

    San updatedField = *field;  // Copy
    bool success = sys->capNhatSan("S01", updatedField);
}
```

### **Delete Field**

```cpp
bool success = sys->xoaSan("S01");
```

### **Find Available Fields**

```cpp
NgayGio thoiGian(14, 0, 0, 8, 11, 2025);  // 14:00:00 08/11/2025
KhungGio khung(ThoiGian(14,0,0), ThoiGian(16,0,0));  // 14:00-16:00

MangDong<San*> availableFields = sys->timSanTrong(thoiGian, khung);
```

### **Field Types**

```cpp
enum LoaiSan {
    SAN_5,   // Sân 5 người
    SAN_7    // Sân 7 người
};
```

### **Field Status**

```cpp
enum TrangThaiSan {
    SAN_TRONG,      // Available
    DANG_SU_DUNG,   // In use
    BAO_TRI,        // Maintenance
    TAM_DONG        // Closed
};
```

---

## 👥 CUSTOMER MANAGEMENT (Quản Lý Khách Hàng)

### **Headers:**

```cpp
#include "Core/Models/KhachHang.h"
```

### **Create Customer**

```cpp
KhachHang *customer = new KhachHang(
    "Nguyen Van A",     // ten
    "0912345678",       // soDienThoai
    "Ha Noi",           // diaChi
    "KH001"             // maKH
);

// Set loại thành viên (optional)
customer->setLoaiThanhVien(LoaiThanhVien::THUONG);

bool success = sys->themKhachHang(customer);
```

### **List All Customers**

```cpp
const MangDong<KhachHang*>& customers = sys->layDanhSachKhachHang();

for (int i = 0; i < customers.size(); i++) {
    KhachHang *cust = customers[i];

    std::string ma = cust->getMaKH();
    std::string ten = cust->getTen();
    std::string sdt = cust->getSoDienThoai();
    std::string diachi = cust->getDiaChi();
    LoaiThanhVien loai = cust->getLoaiThanhVien();
}
```

### **Find Customer**

```cpp
// By ID
KhachHang *cust = sys->timKhachHang("KH001");

// By Phone
KhachHang *cust = sys->timKhachHangTheoSDT("0912345678");
```

### **Update Customer**

```cpp
KhachHang *cust = sys->timKhachHang("KH001");
if (cust) {
    cust->setDiaChi("Ho Chi Minh");
    cust->setLoaiThanhVien(LoaiThanhVien::VIP);

    KhachHang updated = *cust;
    bool success = sys->capNhatKhachHang("KH001", updated);
}
```

### **Delete Customer**

```cpp
bool success = sys->xoaKhachHang("KH001");
```

### **Customer Types**

```cpp
enum LoaiThanhVien {
    THUONG,  // Regular
    VIP,     // VIP
    DOANH_NGHIEP  // Corporate
};
```

---

## 📅 BOOKING MANAGEMENT (Quản Lý Đặt Sân)

### **Headers:**

```cpp
#include "Core/Models/DatSan.h"
#include "Core/Models/KhungGio.h"
#include "Core/CauTrucDuLieu/NgayGio.h"
```

### **Create Booking**

```cpp
KhachHang *customer = sys->timKhachHang("KH001");
San *field = sys->timSan("S01");

NgayGio thoiGian(14, 0, 0, 8, 11, 2025);  // 14:00 08/11/2025
KhungGio khung(ThoiGian(14,0,0), ThoiGian(16,0,0));  // 2 hours

// Check availability first
bool available = sys->kiemTraSanTrong(field, thoiGian, khung);

if (available) {
    DatSan *booking = sys->taoDatSan(customer, field, thoiGian, khung);

    if (booking != nullptr) {
        // Add services (optional)
        DichVu *service = sys->timDichVu("DV001");
        DichVuDat dvDat(service, 2);  // 2 units
        booking->themDichVu(dvDat);

        // Calculate total
        booking->tinhTongTien();
    }
}
```

### **List All Bookings**

```cpp
const MangDong<DatSan*>& bookings = sys->layDanhSachDatSan();

for (int i = 0; i < bookings.size(); i++) {
    DatSan *booking = bookings[i];

    std::string ma = booking->getMaDatSan();
    KhachHang *cust = booking->getKhachHang();
    San *field = booking->getSan();
    NgayGio thoiGian = booking->getThoiGianDat();
    TrangThaiDatSan status = booking->getTrangThai();
    double tongTien = booking->getTongTien();
}
```

### **Find Bookings**

```cpp
// By ID
DatSan *booking = sys->timDatSan("DS1234567");

// By Customer
MangDong<DatSan*> bookings = sys->timDatSanTheoKhachHang("KH001");

// By Field
MangDong<DatSan*> bookings = sys->timDatSanTheoSan("S01");

// By Date
NgayThang ngay(8, 11, 2025);
MangDong<DatSan*> bookings = sys->timDatSanTheoNgay(ngay);
```

### **Update Booking Status**

```cpp
bool success = sys->capNhatTrangThaiDatSan(
    "DS1234567",
    TrangThaiDatSan::DA_XAC_NHAN
);
```

### **Cancel Booking**

```cpp
bool success = sys->huyDatSan("DS1234567");
// This sets status to DA_HUY
```

### **Booking Status**

```cpp
enum TrangThaiDatSan {
    CHO_XAC_NHAN,   // Pending confirmation
    DA_XAC_NHAN,    // Confirmed
    DANG_SU_DUNG,   // In progress
    HOAN_THANH,     // Completed
    DA_HUY          // Cancelled
};
```

---

## 💳 PAYMENT MANAGEMENT (Quản Lý Thanh Toán)

### **Headers:**

```cpp
#include "Core/Models/ThanhToan.h"
#include "Core/Models/ThanhToanTienMat.h"
#include "Core/Models/ThanhToanThe.h"
#include "Core/Models/ThanhToanChuyenKhoan.h"
```

### **Create Payment**

```cpp
DatSan *booking = sys->timDatSan("DS1234567");

// Choose payment method
PhuongThucThanhToan method = PhuongThucThanhToan::TIEN_MAT;

ThanhToan *payment = sys->taoThanhToan(booking, method);

if (payment != nullptr) {
    // Cast to specific type for additional setup

    if (method == PhuongThucThanhToan::TIEN_MAT) {
        ThanhToanTienMat *cash = dynamic_cast<ThanhToanTienMat*>(payment);
        cash->setTienKhachDua(500000);
        cash->tinhTienThua();
        cash->xacNhanThanhToan();
    }
    else if (method == PhuongThucThanhToan::THE) {
        ThanhToanThe *card = dynamic_cast<ThanhToanThe*>(payment);
        card->setSoThe("1234567890123456");
        card->setLoaiThe("Visa");
        card->setMaPinGiaoDich("1234");
        card->xacNhanThanhToan();
    }
    else if (method == PhuongThucThanhToan::CHUYEN_KHOAN) {
        ThanhToanChuyenKhoan *transfer = dynamic_cast<ThanhToanChuyenKhoan*>(payment);
        transfer->setSoTaiKhoan("0123456789");
        transfer->setTenNganHang("Vietcombank");
        transfer->setNoiDungCK("Thanh toan dat san");
        transfer->xacNhanThanhToan();
    }
}
```

### **List All Payments**

```cpp
const MangDong<ThanhToan*>& payments = sys->layDanhSachThanhToan();

for (int i = 0; i < payments.size(); i++) {
    ThanhToan *payment = payments[i];

    std::string ma = payment->getMaThanhToan();
    DatSan *booking = payment->getDonDatSan();
    double soTien = payment->getSoTien();
    PhuongThucThanhToan method = payment->getPhuongThuc();
    NgayGio ngayTT = payment->getNgayThanhToan();
}
```

### **Find Payment**

```cpp
// By ID
ThanhToan *payment = sys->timThanhToan("TT001");

// By Booking
ThanhToan *payment = sys->timThanhToanTheoDatSan("DS1234567");
```

### **Payment Methods**

```cpp
enum PhuongThucThanhToan {
    TIEN_MAT,       // Cash
    THE,            // Card
    CHUYEN_KHOAN    // Bank transfer
};
```

---

## 🛒 SERVICE MANAGEMENT (Quản Lý Dịch Vụ)

### **Headers:**

```cpp
#include "Core/Models/DichVu.h"
#include "Core/Models/DoUong.h"
#include "Core/Models/ThietBi.h"
```

### **Create Service**

```cpp
// Base service (for simple items)
DichVu *service = new DichVu(
    "DV001",                    // maDichVu
    "Nuoc suoi",                // tenDichVu
    10000,                      // giaDichVu
    LoaiDichVu::DO_UONG        // loaiDichVu
);

bool success = sys->themDichVu(service);

// For specific types, use subclasses:

// DoUong (Beverage)
DoUong *drink = new DoUong(
    "DV001", "Coca Cola", 15000,
    LoaiDoUong::NUOC_NGOT,      // Type
    "Lon",                      // Size
    NhietDoDoUong::LANH        // Temperature
);

// ThietBi (Equipment)
ThietBi *equip = new ThietBi(
    "DV002", "Ao dau", 50000,
    LoaiThietBi::AO_DAU,        // Type
    "M",                        // Size
    TinhTrangThietBi::TOT,      // Condition
    10,                         // Ton kho
    "Nike"                      // Brand
);
```

### **List All Services**

```cpp
const MangDong<DichVu*>& services = sys->layDanhSachDichVu();

for (int i = 0; i < services.size(); i++) {
    DichVu *service = services[i];

    std::string ma = service->getMaDichVu();
    std::string ten = service->getTenDichVu();
    double gia = service->getGiaDichVu();
    LoaiDichVu loai = service->getLoaiDichVu();
    bool available = service->isSanCo();
}
```

### **Find Service**

```cpp
DichVu *service = sys->timDichVu("DV001");
```

### **Update/Delete Service**

```cpp
// Update
DichVu updated = *service;
updated.setGiaDichVu(12000);
sys->capNhatDichVu("DV001", updated);

// Delete
sys->xoaDichVu("DV001");
```

### **Service Types**

```cpp
enum LoaiDichVu {
    DO_UONG,    // Beverages
    THIET_BI,   // Equipment
    BAO_HIEM    // Insurance
};

enum LoaiDoUong {
    NUOC_NGOT,  // Soft drinks
    NUOC_SUOI,  // Water
    TRA_SUA,    // Milk tea
    CAFE        // Coffee
};

enum LoaiThietBi {
    AO_DAU,     // Jersey
    GIAY,       // Shoes
    BONG        // Ball
};
```

---

## 📊 STATISTICS (Thống Kê)

### **Headers:**

```cpp
#include "Core/ThongKe/ThongKeDoanhThu.h"
#include "Core/ThongKe/ThongKeKhachHang.h"
```

### **Revenue Statistics**

```cpp
NgayThang tuNgay(1, 11, 2025);
NgayThang denNgay(8, 11, 2025);

ThongKeDoanhThu *stats = new ThongKeDoanhThu(
    sys,
    tuNgay,
    denNgay,
    "Doanh thu tuan 1"
);

stats->tinhToan();

// Get results
double totalRevenue = stats->getDoanhThu();
int totalBookings = stats->getSoLuotDat();

// Display (console)
stats->hienThi();

// Export to file
stats->xuatFile("revenue_report.txt");

delete stats;
```

### **Customer Statistics**

```cpp
NgayThang tuNgay(1, 11, 2025);
NgayThang denNgay(8, 11, 2025);

ThongKeKhachHang *stats = new ThongKeKhachHang(
    sys,
    tuNgay,
    denNgay,
    "Khach hang tuan 1"
);

stats->tinhToan();
stats->tinhTopKhachHang(10);  // Top 10 customers

// Display
stats->hienThi();

// Export
stats->xuatFile("customer_report.txt");

delete stats;
```

### **Quick Stats**

```cpp
// Counts
int totalFields = sys->tongSoSan();
int totalCustomers = sys->tongSoKhachHang();
int totalBookings = sys->tongSoDatSan();
int totalPayments = sys->tongSoThanhToan();

// Revenue
QuanLyThanhToan *paymentMgr = sys->layQuanLyThanhToan();
double totalRevenue = paymentMgr->tongDoanhThu();
```

---

## 📁 FILE I/O (Lưu/Tải Dữ Liệu)

### **Save System**

```cpp
bool success = sys->luuHeThong("Data/system.dat");
```

### **Load System**

```cpp
bool success = sys->docHeThong("Data/system.dat");
```

### **Backup**

```cpp
BackupManager *backupMgr = sys->layBackupManager();

bool success = backupMgr->taoBackup("Data/system.dat");
// Creates backup with timestamp in Data/backup/
```

### **Restore**

```cpp
bool success = backupMgr->khoiPhuc(
    "Data/backup/system_20251108_140530.dat",
    "Data/system.dat"
);
```

### **Clear All Data**

```cpp
sys->xoaTatCaDuLieu();
// WARNING: Deletes all data in memory (not files)
```

---

## 🔄 TYPE CONVERSIONS (Qt ↔ Core)

### **QString ↔ std::string**

```cpp
// Qt → Core
QString qstr = "Hello";
std::string stdstr = qstr.toStdString();

// Core → Qt
std::string stdstr = "World";
QString qstr = QString::fromStdString(stdstr);
```

### **QDateTime ↔ NgayGio**

```cpp
// Qt → Core
QDateTime qdt = QDateTime::currentDateTime();
NgayGio ngayGio(
    qdt.time().hour(),
    qdt.time().minute(),
    qdt.time().second(),
    qdt.date().day(),
    qdt.date().month(),
    qdt.date().year()
);

// Core → Qt
NgayGio ngayGio = booking->getThoiGianDat();
QDateTime qdt(
    QDate(ngayGio.getNam(), ngayGio.getThang(), ngayGio.getNgay()),
    QTime(ngayGio.getGio(), ngayGio.getPhut(), ngayGio.getGiay())
);
```

### **QDate ↔ NgayThang**

```cpp
// Qt → Core
QDate qdate = QDate::currentDate();
NgayThang ngayThang(qdate.day(), qdate.month(), qdate.year());

// Core → Qt
NgayThang ngayThang = booking->getNgayTao().layNgayThang();
QDate qdate(ngayThang.getNam(), ngayThang.getThang(), ngayThang.getNgay());
```

### **QTime ↔ ThoiGian**

```cpp
// Qt → Core
QTime qtime = QTime::currentTime();
ThoiGian thoiGian(qtime.hour(), qtime.minute(), qtime.second());

// Core → Qt
ThoiGian thoiGian = khungGio.getGioBatDau();
QTime qtime(thoiGian.getGio(), thoiGian.getPhut(), thoiGian.getGiay());
```

### **Helper Function (Create in UI/Utils/TypeHelpers.h)**

```cpp
#ifndef TYPEHELPERS_H
#define TYPEHELPERS_H

#include <QString>
#include <QDateTime>
#include "Core/CauTrucDuLieu/NgayGio.h"

namespace TypeHelpers {

inline QString toQString(const std::string& str) {
    return QString::fromStdString(str);
}

inline std::string toStdString(const QString& qstr) {
    return qstr.toStdString();
}

inline QDateTime toQDateTime(const NgayGio& ngayGio) {
    return QDateTime(
        QDate(ngayGio.getNam(), ngayGio.getThang(), ngayGio.getNgay()),
        QTime(ngayGio.getGio(), ngayGio.getPhut(), ngayGio.getGiay())
    );
}

inline NgayGio toNgayGio(const QDateTime& qdt) {
    return NgayGio(
        qdt.time().hour(), qdt.time().minute(), qdt.time().second(),
        qdt.date().day(), qdt.date().month(), qdt.date().year()
    );
}

} // namespace TypeHelpers

#endif
```

---

## ⚠️ COMMON PITFALLS

### **1. Pointer Ownership**

```cpp
// BAD: Don't delete objects you get from managers
San *field = sys->timSan("S01");
delete field;  // ❌ WRONG! Manager owns this

// GOOD: Manager handles deletion
San *field = sys->timSan("S01");
// Use field, don't delete
```

### **2. Null Checks**

```cpp
// ALWAYS check for nullptr
KhachHang *cust = sys->timKhachHang("KH001");
if (cust == nullptr) {
    // Handle not found
    return;
}
// Use cust
```

### **3. Copy vs Reference**

```cpp
// Get by reference (read-only)
const MangDong<San*>& fields = sys->layDanhSachSan();

// Iterate safely
for (int i = 0; i < fields.size(); i++) {
    San *field = fields[i];  // Pointer, don't copy object
}
```

### **4. Update Pattern**

```cpp
// WRONG: Modifying pointer doesn't update storage
San *field = sys->timSan("S01");
field->datGiaThueGio(350000);  // ❌ Won't persist

// CORRECT: Use capNhat method
San *field = sys->timSan("S01");
if (field) {
    San updated = *field;  // Copy
    updated.datGiaThueGio(350000);
    sys->capNhatSan("S01", updated);  // ✅ Update storage
}
```

---

## 📝 QUICK REFERENCE CHEAT SHEET

```cpp
// ===== SETUP =====
#include "Core/QuanLy/HeThongQuanLy.h"
HeThongQuanLy *sys = HeThongQuanLy::getInstance();

// ===== FIELD =====
sys->themSan(field);
sys->layDanhSachSan();
sys->timSan(ma);
sys->capNhatSan(ma, updated);
sys->xoaSan(ma);
sys->timSanTrong(ngayGio, khung);

// ===== CUSTOMER =====
sys->themKhachHang(customer);
sys->layDanhSachKhachHang();
sys->timKhachHang(ma);
sys->timKhachHangTheoSDT(sdt);
sys->capNhatKhachHang(ma, updated);
sys->xoaKhachHang(ma);

// ===== BOOKING =====
sys->taoDatSan(customer, field, thoiGian, khung);
sys->layDanhSachDatSan();
sys->timDatSan(ma);
sys->timDatSanTheoKhachHang(maKH);
sys->timDatSanTheoSan(maSan);
sys->timDatSanTheoNgay(ngay);
sys->capNhatTrangThaiDatSan(ma, status);
sys->huyDatSan(ma);
sys->kiemTraSanTrong(san, thoiGian, khung);

// ===== PAYMENT =====
sys->taoThanhToan(booking, method);
sys->layDanhSachThanhToan();
sys->timThanhToan(ma);
sys->timThanhToanTheoDatSan(maDatSan);
sys->xoaThanhToan(ma);

// ===== SERVICE =====
sys->themDichVu(service);
sys->layDanhSachDichVu();
sys->timDichVu(ma);
sys->capNhatDichVu(ma, updated);
sys->xoaDichVu(ma);

// ===== STATS =====
sys->tongSoSan();
sys->tongSoKhachHang();
sys->tongSoDatSan();
sys->tongSoThanhToan();

// ===== FILE I/O =====
sys->luuHeThong(file);
sys->docHeThong(file);
sys->xoaTatCaDuLieu();
```

---

**Status:** ✅ **COMPLETE - READY FOR UI DEVELOPMENT**

---

_Created: 8 Tháng 11, 2025_  
_Version: 1.0.0_  
_Author: Development Team_
