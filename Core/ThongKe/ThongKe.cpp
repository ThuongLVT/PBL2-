#include "ThongKe.h"

using namespace std;

// Constructor mặc định
ThongKe::ThongKe() : tieuDe(""), tuNgay(), denNgay(), ngayTao()
{
    ngayTao = NgayThang::layNgayHienTai();
}

// Constructor với tham số
ThongKe::ThongKe(const std::string &td, const NgayThang &tu, const NgayThang &den)
    : tieuDe(td), tuNgay(tu), denNgay(den)
{
    ngayTao = NgayThang::layNgayHienTai();
}

// Copy constructor
ThongKe::ThongKe(const ThongKe &other)
{
    tieuDe = other.tieuDe;
    tuNgay = other.tuNgay;
    denNgay = other.denNgay;
    ngayTao = other.ngayTao;
}

// Destructor
ThongKe::~ThongKe() {}

// Assignment operator
ThongKe &ThongKe::operator=(const ThongKe &other)
{
    if (this != &other)
    {
        tieuDe = other.tieuDe;
        tuNgay = other.tuNgay;
        denNgay = other.denNgay;
        ngayTao = other.ngayTao;
    }
    return *this;
}

// Getters
std::string ThongKe::getTieuDe() const { return tieuDe; }
NgayThang ThongKe::getTuNgay() const { return tuNgay; }
NgayThang ThongKe::getDenNgay() const { return denNgay; }
NgayThang ThongKe::getNgayTao() const { return ngayTao; }

// Setters
void ThongKe::setTieuDe(const std::string &td) { tieuDe = td; }
void ThongKe::setTuNgay(const NgayThang &tu) { tuNgay = tu; }
void ThongKe::setDenNgay(const NgayThang &den) { denNgay = den; }
