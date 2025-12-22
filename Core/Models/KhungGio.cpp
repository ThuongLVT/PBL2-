/**
 * @file KhungGio.cpp
 * @brief Implementation của lớp KhungGio
 */

#include "KhungGio.h"

// ========== CONSTRUCTORS ==========

KhungGio::KhungGio()
    : gioBatDau(), gioKetThuc()
{
}

KhungGio::KhungGio(const ThoiGian &bd, const ThoiGian &kt)
    : gioBatDau(bd), gioKetThuc(kt)
{
}

KhungGio::KhungGio(int gioBD, int phutBD, int gioKT, int phutKT)
    : gioBatDau(gioBD, phutBD, 0), gioKetThuc(gioKT, phutKT, 0)
{
}

KhungGio::KhungGio(const KhungGio &other)
    : gioBatDau(other.gioBatDau), gioKetThuc(other.gioKetThuc)
{
}

KhungGio::~KhungGio()
{
    // Destructor
}

// ========== GETTERS ==========

ThoiGian KhungGio::layGioBatDau() const
{
    return gioBatDau;
}

ThoiGian KhungGio::layGioKetThuc() const
{
    return gioKetThuc;
}

// ========== SETTERS ==========

void KhungGio::datGioBatDau(const ThoiGian &bd)
{
    gioBatDau = bd;
}

void KhungGio::datGioKetThuc(const ThoiGian &kt)
{
    gioKetThuc = kt;
}

// ========== METHODS ==========

double KhungGio::tinhSoGio() const
{
    if (!hopLe())
        return 0.0;

    // Chuyển đổi sang giây rồi tính số giờ
    int giayBD = gioBatDau.getGio() * 3600 + gioBatDau.getPhut() * 60 + gioBatDau.getGiay();
    int giayKT = gioKetThuc.getGio() * 3600 + gioKetThuc.getPhut() * 60 + gioKetThuc.getGiay();

    double soGio = (giayKT - giayBD) / 3600.0;
    return soGio;
}

bool KhungGio::hopLe() const
{
    return gioBatDau < gioKetThuc;
}

bool KhungGio::kiemTraTrung(const KhungGio &other) const
{
    // Hai khung giờ trùng nếu:
    // 1. Giờ bắt đầu này nằm trong khoảng của khung giờ kia
    // 2. Giờ kết thúc này nằm trong khoảng của khung giờ kia
    // 3. Khung giờ này bao trùm khung giờ kia

    bool batDauTrung = (gioBatDau >= other.gioBatDau && gioBatDau < other.gioKetThuc);
    bool ketThucTrung = (gioKetThuc > other.gioBatDau && gioKetThuc <= other.gioKetThuc);
    bool baoTrum = (gioBatDau <= other.gioBatDau && gioKetThuc >= other.gioKetThuc);

    return batDauTrung || ketThucTrung || baoTrum;
}

void KhungGio::hienThiThongTin() const
{
    std::cout << "Khung gio: " << gioBatDau << " - " << gioKetThuc;
    if (hopLe())
    {
        std::cout << " (" << tinhSoGio() << " gio)";
    }
    else
    {
        std::cout << " (KHONG HOP LE)";
    }
    std::cout << std::endl;
}

// ========== OPERATORS ==========

KhungGio &KhungGio::operator=(const KhungGio &other)
{
    if (this != &other)
    {
        gioBatDau = other.gioBatDau;
        gioKetThuc = other.gioKetThuc;
    }
    return *this;
}

bool KhungGio::operator==(const KhungGio &other) const
{
    return (gioBatDau == other.gioBatDau) && (gioKetThuc == other.gioKetThuc);
}

bool KhungGio::operator<(const KhungGio &other) const
{
    return gioBatDau < other.gioBatDau;
}

std::ostream &operator<<(std::ostream &os, const KhungGio &kg)
{
    os << kg.gioBatDau << " - " << kg.gioKetThuc;
    return os;
}

