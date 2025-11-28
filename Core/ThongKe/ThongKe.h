#ifndef THONGKE_H
#define THONGKE_H
#include <string>

#include "../CauTrucDuLieu/NgayGio.h"

// Lớp cơ sở trừu tượng cho các loại thống kê
class ThongKe
{
protected:
    std::string tieuDe; // Tiêu đề báo cáo
    NgayThang tuNgay;   // Từ ngày
    NgayThang denNgay;  // Đến ngày
    NgayThang ngayTao;  // Ngày tạo báo cáo

public:
    // Constructor & Destructor
    ThongKe();
    ThongKe(const std::string &td, const NgayThang &tu, const NgayThang &den);
    ThongKe(const ThongKe &other);
    virtual ~ThongKe();

    // Operators
    ThongKe &operator=(const ThongKe &other);

    // Getters
    std::string getTieuDe() const;
    NgayThang getTuNgay() const;
    NgayThang getDenNgay() const;
    NgayThang getNgayTao() const;

    // Setters
    void setTieuDe(const std::string &td);
    void setTuNgay(const NgayThang &tu);
    void setDenNgay(const NgayThang &den);

    // Abstract methods
    virtual void tinhToan() = 0;                                 // Tính toán thống kê
    virtual void hienThi() const = 0;                            // Hiển thị báo cáo
    virtual void xuatFile(const std::string &tenFile) const = 0; // Xuất ra file
};

#endif // THONGKE_H
