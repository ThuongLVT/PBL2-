#ifndef THANHTOAN_H
#define THANHTOAN_H
#include <string>
#include <fstream>

#include "../CauTrucDuLieu/NgayGio.h"
#include "DatSan.h"

// Enum phương thức thanh toán
enum PhuongThucThanhToan
{
    TIEN_MAT,
    THE,
    CHUYEN_KHOAN
};

// Lớp cơ sở trừu tượng cho thanh toán
class ThanhToan
{
protected:
    std::string maThanhToan;        // Mã thanh toán (tự sinh)
    DatSan *donDatSan;              // Đơn đặt sân
    double soTien;                  // Số tiền thanh toán
    NgayGio ngayThanhToan;          // Ngày giờ thanh toán
    PhuongThucThanhToan phuongThuc; // Phương thức
    std::string nguoiThu;           // Người thu tiền (nhân viên)
    std::string ghiChu;             // Ghi chú

public:
    // Constructor & Destructor
    ThanhToan();
    ThanhToan(const std::string &ma, DatSan *ds, double st, PhuongThucThanhToan pt);
    ThanhToan(const ThanhToan &other);
    virtual ~ThanhToan();

    // Operators
    ThanhToan &operator=(const ThanhToan &other);

    // Getters
    std::string getMaThanhToan() const;
    DatSan *getDonDatSan() const;
    double getSoTien() const;
    NgayGio getNgayThanhToan() const;
    PhuongThucThanhToan getPhuongThuc() const;
    std::string getNguoiThu() const;
    std::string getGhiChu() const;

    // Setters
    void setMaThanhToan(const std::string &ma);
    void setDonDatSan(DatSan *ds);
    void setSoTien(double st);
    void setNguoiThu(const std::string &nt);
    void setGhiChu(const std::string &gc);

    // Abstract methods - phải override
    virtual void xacNhanThanhToan() = 0; // Xác nhận thanh toán
    virtual void hienThi() const = 0;    // Hiển thị thông tin

    // Common methods
    std::string getPhuongThucText() const;
    void inHoaDon() const; // In hóa đơn

    // File I/O
    virtual void ghiFile(FILE *f) const;
    virtual void docFile(FILE *f);

    // Static method
    static std::string taoMaThanhToan();
};

#endif // THANHTOAN_H
