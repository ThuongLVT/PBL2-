/**
 * @file KhachHang.cpp
 * @brief Implementation của lớp KhachHang
 */

#include "KhachHang.h"
#include "../Utils/FileHelper.h"

// ========== CONSTRUCTORS ==========

KhachHang::KhachHang()
    : ConNguoi(), maKhachHang(""), tongChiTieu(0.0),
      hang(HangKhachHang::THUONG), ngayDangKy(), soLanDatSan(0)
{
}

KhachHang::KhachHang(const std::string &ht, const std::string &sdt, const std::string &dc,
                     const std::string &maKH)
    : ConNguoi(ht, sdt, dc), maKhachHang(maKH), tongChiTieu(0.0),
      hang(HangKhachHang::THUONG), ngayDangKy(), soLanDatSan(0)
{
}

KhachHang::KhachHang(const KhachHang &other)
    : ConNguoi(other), maKhachHang(other.maKhachHang),
      tongChiTieu(other.tongChiTieu), hang(other.hang),
      ngayDangKy(other.ngayDangKy), soLanDatSan(other.soLanDatSan)
{
}

KhachHang::~KhachHang()
{
    // Destructor
}

// ========== GETTERS ==========

std::string KhachHang::layMaKhachHang() const
{
    return maKhachHang;
}

std::string KhachHang::getMaNguoiDung() const
{
    return maKhachHang; // Return customer ID as user ID
}

double KhachHang::layTongChiTieu() const
{
    return tongChiTieu;
}

HangKhachHang KhachHang::layHang() const
{
    return hang;
}

NgayThang KhachHang::layNgayDangKy() const
{
    return ngayDangKy;
}

int KhachHang::laySoLanDatSan() const
{
    return soLanDatSan;
}

std::string KhachHang::layTenHang() const
{
    switch (hang)
    {
    case HangKhachHang::THUONG:
        return std::string("Thuong");
    case HangKhachHang::DONG:
        return std::string("Dong");
    case HangKhachHang::BAC:
        return std::string("Bac");
    case HangKhachHang::VANG:
        return std::string("Vang");
    case HangKhachHang::KIM_CUONG:
        return std::string("Kim Cuong");
    default:
        return std::string("Khong xac dinh");
    }
}

int KhachHang::layPhanTramGiamGia() const
{
    switch (hang)
    {
    case HangKhachHang::THUONG:
        return 0;
    case HangKhachHang::DONG:
        return 2;
    case HangKhachHang::BAC:
        return 4;
    case HangKhachHang::VANG:
        return 6;
    case HangKhachHang::KIM_CUONG:
        return 8;
    default:
        return 0;
    }
}

// ========== SETTERS ==========

void KhachHang::datMaKhachHang(const std::string &maKH)
{
    maKhachHang = maKH;
}

void KhachHang::datNgayDangKy(const NgayThang &ngay)
{
    ngayDangKy = ngay;
}

void KhachHang::datTongChiTieu(double tien)
{
    tongChiTieu = tien;
    capNhatHang(); // Update tier based on spending
}

// ========== METHODS ==========

void KhachHang::themChiTieu(double soTien)
{
    if (soTien > 0)
    {
        tongChiTieu += soTien;
        capNhatHang();
    }
}

void KhachHang::tangSoLanDatSan()
{
    soLanDatSan++;
}

void KhachHang::capNhatHang()
{
    if (tongChiTieu >= 20000000) // >= 20,000,000 VND
    {
        hang = HangKhachHang::KIM_CUONG;
    }
    else if (tongChiTieu >= 10000000) // 10,000,000 - 19,999,999 VND
    {
        hang = HangKhachHang::VANG;
    }
    else if (tongChiTieu >= 5000000) // 5,000,000 - 9,999,999 VND
    {
        hang = HangKhachHang::BAC;
    }
    else if (tongChiTieu >= 1000000) // 1,000,000 - 4,999,999 VND
    {
        hang = HangKhachHang::DONG;
    }
    else // < 1,000,000 VND
    {
        hang = HangKhachHang::THUONG;
    }
}

void KhachHang::hienThiThongTin() const
{
    std::cout << "===== KHACH HANG =====" << std::endl;
    ConNguoi::hienThiThongTin();
    std::cout << "Ma khach hang: " << maKhachHang << std::endl;
    std::cout << "Tong chi tieu: " << tongChiTieu << " VND" << std::endl;
    std::cout << "Hang thanh vien: " << layTenHang()
              << " (Giam " << layPhanTramGiamGia() << "%)" << std::endl;
    std::cout << "So lan dat san: " << soLanDatSan << std::endl;
}

bool KhachHang::ghiFile(std::ofstream &file) const
{
    if (!file.is_open())
        return false;

    // Ghi thông tin lớp cha
    if (!ConNguoi::ghiFile(file))
        return false;

    // Ghi thông tin lớp con
    if (!FileHelper::ghiString(file, maKhachHang))
        return false;
    file.write(reinterpret_cast<const char *>(&tongChiTieu), sizeof(tongChiTieu));

    int hangInt = static_cast<int>(hang);
    file.write(reinterpret_cast<const char *>(&hangInt), sizeof(hangInt));

    ngayDangKy.ghiFile(file);
    file.write(reinterpret_cast<const char *>(&soLanDatSan), sizeof(soLanDatSan));

    return file.good();
}

bool KhachHang::docFile(std::ifstream &file)
{
    if (!file.is_open())
        return false;

    // Đọc thông tin lớp cha
    if (!ConNguoi::docFile(file))
        return false;

    // Đọc thông tin lớp con
    if (!FileHelper::docString(file, maKhachHang))
        return false;
    file.read(reinterpret_cast<char *>(&tongChiTieu), sizeof(tongChiTieu));

    int hangInt;
    file.read(reinterpret_cast<char *>(&hangInt), sizeof(hangInt));
    hang = static_cast<HangKhachHang>(hangInt);

    ngayDangKy.docFile(file);
    file.read(reinterpret_cast<char *>(&soLanDatSan), sizeof(soLanDatSan));

    return file.good();
}

// ========== OPERATORS ==========

KhachHang &KhachHang::operator=(const KhachHang &other)
{
    if (this != &other)
    {
        ConNguoi::operator=(other);
        maKhachHang = other.maKhachHang;
        tongChiTieu = other.tongChiTieu;
        hang = other.hang;
        ngayDangKy = other.ngayDangKy;
        soLanDatSan = other.soLanDatSan;
    }
    return *this;
}

bool KhachHang::operator==(const KhachHang &other) const
{
    return maKhachHang == other.maKhachHang;
}

std::ostream &operator<<(std::ostream &os, const KhachHang &kh)
{
    os << static_cast<const ConNguoi &>(kh)
       << ", MaKH: " << kh.maKhachHang
       << ", Chi tieu: " << kh.tongChiTieu << " VND"
       << ", Hang: " << kh.layTenHang();
    return os;
}
