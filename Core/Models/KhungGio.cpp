/**
 * @file KhungGio.cpp
 * @brief Implementation của lớp KhungGio
 */

#include "KhungGio.h"

#include "../Utils/FileHelper.h"
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

bool KhungGio::ghiFile(std::ofstream &file) const
{
    if (!file.is_open())
        return false;

    gioBatDau.ghiFile(file);
    gioKetThuc.ghiFile(file);

    return file.good();
}

bool KhungGio::docFile(std::ifstream &file)
{
    if (!file.is_open())
        return false;

    gioBatDau.docFile(file);
    gioKetThuc.docFile(file);

    return file.good();
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

bool KhungGio::ghiFile(FILE *f) const
{
    if (!f)
        return false;

    int gioBD = gioBatDau.getGio();
    int phutBD = gioBatDau.getPhut();
    int giayBD = gioBatDau.getGiay();
    int gioKT = gioKetThuc.getGio();
    int phutKT = gioKetThuc.getPhut();
    int giayKT = gioKetThuc.getGiay();

    fwrite(&gioBD, sizeof(int), 1, f);
    fwrite(&phutBD, sizeof(int), 1, f);
    fwrite(&giayBD, sizeof(int), 1, f);
    fwrite(&gioKT, sizeof(int), 1, f);
    fwrite(&phutKT, sizeof(int), 1, f);
    fwrite(&giayKT, sizeof(int), 1, f);

    return true;
}

bool KhungGio::docFile(FILE *f)
{
    if (!f)
        return false;

    int gioBD, phutBD, giayBD, gioKT, phutKT, giayKT;

    fread(&gioBD, sizeof(int), 1, f);
    fread(&phutBD, sizeof(int), 1, f);
    fread(&giayBD, sizeof(int), 1, f);
    fread(&gioKT, sizeof(int), 1, f);
    fread(&phutKT, sizeof(int), 1, f);
    fread(&giayKT, sizeof(int), 1, f);

    gioBatDau = ThoiGian(gioBD, phutBD, giayBD);
    gioKetThuc = ThoiGian(gioKT, phutKT, giayKT);

    return true;
}
