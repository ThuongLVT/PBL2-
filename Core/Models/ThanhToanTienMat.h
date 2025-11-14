#ifndef THANHTOANTIENMAT_H
#define THANHTOANTIENMAT_H
#include <string>

#include "ThanhToan.h"

// Lớp thanh toán bằng tiền mặt
class ThanhToanTienMat : public ThanhToan
{
private:
    double tienKhachDua; // Tiền khách đưa
    double tienThua;     // Tiền thừa trả lại

public:
    // Constructor & Destructor
    ThanhToanTienMat();
    ThanhToanTienMat(const std::string &ma, DatSan *ds, double st);
    ThanhToanTienMat(const ThanhToanTienMat &other);
    ~ThanhToanTienMat();

    // Operators
    ThanhToanTienMat &operator=(const ThanhToanTienMat &other);

    // Getters
    double getTienKhachDua() const;
    double getTienThua() const;

    // Setters
    void setTienKhachDua(double tkd);

    // Override abstract methods
    void xacNhanThanhToan() override;
    void hienThi() const override;

    // Methods
    void tinhTienThua(); // Tính tiền thừa

    // File I/O
    void ghiFile(FILE *f) const override;
    void docFile(FILE *f) override;
};

#endif // THANHTOANTIENMAT_H
