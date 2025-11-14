#include "ThanhToanTienMat.h"
#include <iostream>

using namespace std;

// Constructor mặc định
ThanhToanTienMat::ThanhToanTienMat() : ThanhToan(), tienKhachDua(0.0), tienThua(0.0)
{
    phuongThuc = TIEN_MAT;
}

// Constructor với tham số
ThanhToanTienMat::ThanhToanTienMat(const std::string &ma, DatSan *ds, double st)
    : ThanhToan(ma, ds, st, TIEN_MAT), tienKhachDua(0.0), tienThua(0.0) {}

// Copy constructor
ThanhToanTienMat::ThanhToanTienMat(const ThanhToanTienMat &other) : ThanhToan(other)
{
    tienKhachDua = other.tienKhachDua;
    tienThua = other.tienThua;
}

// Destructor
ThanhToanTienMat::~ThanhToanTienMat() {}

// Assignment operator
ThanhToanTienMat &ThanhToanTienMat::operator=(const ThanhToanTienMat &other)
{
    if (this != &other)
    {
        ThanhToan::operator=(other);
        tienKhachDua = other.tienKhachDua;
        tienThua = other.tienThua;
    }
    return *this;
}

// Getters
double ThanhToanTienMat::getTienKhachDua() const
{
    return tienKhachDua;
}

double ThanhToanTienMat::getTienThua() const
{
    return tienThua;
}

// Setters
void ThanhToanTienMat::setTienKhachDua(double tkd)
{
    tienKhachDua = tkd;
    tinhTienThua();
}

// Methods
void ThanhToanTienMat::tinhTienThua()
{
    tienThua = tienKhachDua - soTien;
    if (tienThua < 0)
    {
        tienThua = 0;
    }
}

void ThanhToanTienMat::xacNhanThanhToan()
{
    std::cout << "Xac nhan thanh toan tien mat thanh cong!" << std::endl;
    std::cout << "Tien khach dua: " << tienKhachDua << " VND" << std::endl;
    std::cout << "Tien thua tra lai: " << tienThua << " VND" << std::endl;
}

void ThanhToanTienMat::hienThi() const
{
    std::cout << "\n===== THANH TOAN TIEN MAT =====" << std::endl;
    std::cout << "Ma thanh toan: " << maThanhToan.c_str() << std::endl;
    std::cout << "Ngay thanh toan: " << ngayThanhToan.toString().c_str() << std::endl;
    std::cout << "So tien: " << soTien << " VND" << std::endl;
    std::cout << "Tien khach dua: " << tienKhachDua << " VND" << std::endl;
    std::cout << "Tien thua: " << tienThua << " VND" << std::endl;

    if (nguoiThu.length() > 0)
    {
        std::cout << "Nguoi thu: " << nguoiThu.c_str() << std::endl;
    }
}

// File I/O
void ThanhToanTienMat::ghiFile(FILE *f) const
{
    if (f == nullptr)
        return;

    // Ghi base class
    ThanhToan::ghiFile(f);

    // Ghi tiền khách đưa và tiền thừa
    fwrite(&tienKhachDua, sizeof(double), 1, f);
    fwrite(&tienThua, sizeof(double), 1, f);
}

void ThanhToanTienMat::docFile(FILE *f)
{
    if (f == nullptr)
        return;

    // Đọc base class
    ThanhToan::docFile(f);

    // Đọc tiền khách đưa và tiền thừa
    fread(&tienKhachDua, sizeof(double), 1, f);
    fread(&tienThua, sizeof(double), 1, f);
}
