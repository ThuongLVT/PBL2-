#ifndef DICHVUDAT_H
#define DICHVUDAT_H

#include "DichVu.h"
#include <string>

// Lớp đại diện cho một dịch vụ trong đơn đặt sân
class DichVuDat
{
private:
    DichVu *dichVu;   // Pointer tới dịch vụ
    int soLuong;      // Số lượng dịch vụ đặt
    double thanhTien; // Thành tiền = đơn giá * số lượng

public:
    // Constructor & Destructor
    DichVuDat();
    DichVuDat(DichVu *dv, int sl);
    DichVuDat(const DichVuDat &other);
    ~DichVuDat();

    // Operators
    DichVuDat &operator=(const DichVuDat &other);

    // Getters
    DichVu *getDichVu() const;
    int getSoLuong() const;
    double getThanhTien() const;

    // Setters
    void setDichVu(DichVu *dv);
    void setSoLuong(int sl);

    // Methods
    void tinhThanhTien(); // Tính thành tiền = đơn giá * số lượng
    void hienThi() const;

    };

#endif // DICHVUDAT_H
