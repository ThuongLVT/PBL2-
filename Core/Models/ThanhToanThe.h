#ifndef THANHTOANTHE_H
#define THANHTOANTHE_H
#include <string>

#include "ThanhToan.h"

// Lớp thanh toán bằng thẻ (ATM/Credit Card)
class ThanhToanThe : public ThanhToan
{
private:
    std::string soThe;         // Số thẻ (đã mã hóa hoặc masked)
    std::string loaiThe;       // Loại thẻ (Visa, MasterCard, JCB...)
    std::string maPinGiaoDich; // Mã PIN giao dịch

public:
    // Constructor & Destructor
    ThanhToanThe();
    ThanhToanThe(const std::string &ma, DatSan *ds, double st);
    ThanhToanThe(const ThanhToanThe &other);
    ~ThanhToanThe();

    // Operators
    ThanhToanThe &operator=(const ThanhToanThe &other);

    // Getters
    std::string getSoThe() const;
    std::string getLoaiThe() const;
    std::string getMaPinGiaoDich() const;

    // Setters
    void setSoThe(const std::string &st);
    void setLoaiThe(const std::string &lt);
    void setMaPinGiaoDich(const std::string &mp);

    // Override abstract methods
    void xacNhanThanhToan() override;
    void hienThi() const override;

    // Methods
    std::string maskSoThe() const; // Mask số thẻ (**** **** **** 1234)

    // File I/O
    void ghiFile(FILE *f) const override;
    void docFile(FILE *f) override;
};

#endif // THANHTOANTHE_H
