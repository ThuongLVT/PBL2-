/**
 * @file San.cpp
 * @brief Implementation của lớp San
 */

#include "San.h"
#include "../Utils/FileHelper.h"

using namespace std;

// ========== CONSTRUCTORS ==========

San::San()
    : maSan(""), tenSan(""), loaiSan(LoaiSan::SAN_5), khuVuc(KhuVuc::A),
      giaThueGio(0.0), trangThai(TrangThaiSan::HOAT_DONG), ghiChu("")
{
}

San::San(const std::string &maSan, const std::string &tenSan,
         LoaiSan loaiSan, KhuVuc khuVuc, double giaThue)
    : maSan(maSan), tenSan(tenSan), loaiSan(loaiSan), khuVuc(khuVuc),
      giaThueGio(giaThue), trangThai(TrangThaiSan::HOAT_DONG), ghiChu("")
{
}

San::San(const San &other)
    : maSan(other.maSan), tenSan(other.tenSan), loaiSan(other.loaiSan),
      khuVuc(other.khuVuc), giaThueGio(other.giaThueGio),
      trangThai(other.trangThai), ghiChu(other.ghiChu)
{
}

San::~San()
{
    // Destructor
}

// ========== GETTERS ==========

std::string San::layMaSan() const
{
    return maSan;
}

std::string San::layTenSan() const
{
    return tenSan;
}

LoaiSan San::layLoaiSan() const
{
    return loaiSan;
}

KhuVuc San::layKhuVuc() const
{
    return khuVuc;
}

double San::layGiaThueGio() const
{
    return giaThueGio;
}

TrangThaiSan San::layTrangThai() const
{
    return trangThai;
}

std::string San::layGhiChu() const
{
    return ghiChu;
}

std::string San::layTenLoaiSan() const
{
    switch (loaiSan)
    {
    case LoaiSan::SAN_5:
        return "5";
    case LoaiSan::SAN_7:
        return "7";
    default:
        return "Khong xac dinh";
    }
}

std::string San::layTenTrangThai() const
{
    switch (trangThai)
    {
    case TrangThaiSan::HOAT_DONG:
        return "Hoạt động";
    case TrangThaiSan::NGUNG_HOAT_DONG:
        return "Ngừng hoạt động";
    case TrangThaiSan::BAO_TRI:
        return "Bảo trì";
    default:
        return "Không xác định";
    }
}

std::string San::layTenKhuVuc() const
{
    switch (khuVuc)
    {
    case KhuVuc::A:
        return "A";
    case KhuVuc::B:
        return "B";
    case KhuVuc::C:
        return "C";
    case KhuVuc::D:
        return "D";
    default:
        return "";
    }
}

// ========== SETTERS ==========

void San::datTenSan(const std::string &ten)
{
    tenSan = ten;
}

void San::datGiaThueGio(double gia)
{
    if (gia >= 0)
    {
        giaThueGio = gia;
    }
}

void San::datTrangThai(TrangThaiSan tt)
{
    trangThai = tt;
}

void San::datGhiChu(const std::string &gc)
{
    ghiChu = gc;
}

void San::datLoaiSan(LoaiSan loai)
{
    loaiSan = loai;
}

void San::datKhuVuc(KhuVuc kv)
{
    khuVuc = kv;
}

// ========== METHODS ==========

bool San::dangHoatDong() const
{
    return trangThai == TrangThaiSan::HOAT_DONG;
}

bool San::ngungHoatDong() const
{
    return trangThai == TrangThaiSan::NGUNG_HOAT_DONG;
}

bool San::dangBaoTri() const
{
    return trangThai == TrangThaiSan::BAO_TRI;
}

double San::tinhTien(double soGio) const
{
    if (soGio <= 0)
        return 0.0;
    return giaThueGio * soGio;
}

void San::hienThiThongTin() const
{
    std::cout << "===== THONG TIN SAN =====" << std::endl;
    std::cout << "Ma san: " << maSan << std::endl;
    std::cout << "Ten san: " << tenSan << std::endl;
    std::cout << "Loai san: " << layTenLoaiSan() << " nguoi" << std::endl;
    std::cout << "Khu vuc: " << layTenKhuVuc() << std::endl;
    std::cout << "Gia thue: " << giaThueGio << " VND/60 phut" << std::endl;
    std::cout << "Trang thai: " << layTenTrangThai() << std::endl;
    if (ghiChu.length() > 0)
    {
        std::cout << "Ghi chu: " << ghiChu << std::endl;
    }
}

bool San::ghiFile(std::ofstream &file) const
{
    if (!file.is_open())
        return false;

    if (!FileHelper::ghiString(file, maSan))
        return false;
    if (!FileHelper::ghiString(file, tenSan))
        return false;

    int loaiSanInt = static_cast<int>(loaiSan);
    file.write(reinterpret_cast<const char *>(&loaiSanInt), sizeof(loaiSanInt));

    int khuVucInt = static_cast<int>(khuVuc);
    file.write(reinterpret_cast<const char *>(&khuVucInt), sizeof(khuVucInt));

    file.write(reinterpret_cast<const char *>(&giaThueGio), sizeof(giaThueGio));

    int trangThaiInt = static_cast<int>(trangThai);
    file.write(reinterpret_cast<const char *>(&trangThaiInt), sizeof(trangThaiInt));

    if (!FileHelper::ghiString(file, ghiChu))
        return false;

    return file.good();
}

bool San::docFile(std::ifstream &file)
{
    if (!file.is_open())
        return false;

    if (!FileHelper::docString(file, maSan))
        return false;
    if (!FileHelper::docString(file, tenSan))
        return false;

    int loaiSanInt;
    file.read(reinterpret_cast<char *>(&loaiSanInt), sizeof(loaiSanInt));
    loaiSan = static_cast<LoaiSan>(loaiSanInt);

    int khuVucInt;
    file.read(reinterpret_cast<char *>(&khuVucInt), sizeof(khuVucInt));
    khuVuc = static_cast<KhuVuc>(khuVucInt);

    file.read(reinterpret_cast<char *>(&giaThueGio), sizeof(giaThueGio));

    int trangThaiInt;
    file.read(reinterpret_cast<char *>(&trangThaiInt), sizeof(trangThaiInt));
    trangThai = static_cast<TrangThaiSan>(trangThaiInt);

    if (!FileHelper::docString(file, ghiChu))
        return false;

    return file.good();
}

// ========== OPERATORS ==========

San &San::operator=(const San &other)
{
    if (this != &other)
    {
        maSan = other.maSan;
        tenSan = other.tenSan;
        loaiSan = other.loaiSan;
        khuVuc = other.khuVuc;
        giaThueGio = other.giaThueGio;
        trangThai = other.trangThai;
        ghiChu = other.ghiChu;
    }
    return *this;
}

bool San::operator==(const San &other) const
{
    return maSan == other.maSan;
}

bool San::operator<(const San &other) const
{
    return giaThueGio < other.giaThueGio;
}

std::ostream &operator<<(std::ostream &os, const San &s)
{
    os << "San " << s.maSan << ": " << s.tenSan
       << " (Loai " << s.layTenLoaiSan() << ", Khu " << s.layTenKhuVuc() << ") - "
       << s.giaThueGio << " VND/60p - " << s.layTenTrangThai();
    return os;
}
