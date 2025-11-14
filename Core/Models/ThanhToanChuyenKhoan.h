#ifndef THANHTOANCHUYENKHOAN_H
#define THANHTOANCHUYENKHOAN_H
#include <string>

#include "ThanhToan.h"

// Lớp thanh toán bằng chuyển khoản
class ThanhToanChuyenKhoan : public ThanhToan
{
private:
    std::string soTaiKhoan;  // Số tài khoản người chuyển
    std::string tenNganHang; // Tên ngân hàng
    std::string maDinhDanh;  // Mã định danh giao dịch (Transaction ID)
    std::string noiDungCK;   // Nội dung chuyển khoản

public:
    // Constructor & Destructor
    ThanhToanChuyenKhoan();
    ThanhToanChuyenKhoan(const std::string &ma, DatSan *ds, double st);
    ThanhToanChuyenKhoan(const ThanhToanChuyenKhoan &other);
    ~ThanhToanChuyenKhoan();

    // Operators
    ThanhToanChuyenKhoan &operator=(const ThanhToanChuyenKhoan &other);

    // Getters
    std::string getSoTaiKhoan() const;
    std::string getTenNganHang() const;
    std::string getMaDinhDanh() const;
    std::string getNoiDungCK() const;

    // Setters
    void setSoTaiKhoan(const std::string &stk);
    void setTenNganHang(const std::string &tnh);
    void setMaDinhDanh(const std::string &mdd);
    void setNoiDungCK(const std::string &nd);

    // Override abstract methods
    void xacNhanThanhToan() override;
    void hienThi() const override;

    // File I/O
    void ghiFile(FILE *f) const override;
    void docFile(FILE *f) override;
};

#endif // THANHTOANCHUYENKHOAN_H
