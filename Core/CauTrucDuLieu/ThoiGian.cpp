#include "ThoiGian.h"
#include <iomanip>
#include <cstdio>
#include <string>

ThoiGian::ThoiGian() : gio(0), phut(0), giay(0) {}

ThoiGian::ThoiGian(int g, int p, int gi)
{
    setThoiGian(g, p, gi);
}

void ThoiGian::setGio(int g)
{
    if (g >= 0 && g < 24)
    {
        gio = g;
    }
    else
    {
        gio = 0;
    }
}

void ThoiGian::setPhut(int p)
{
    if (p >= 0 && p < 60)
    {
        phut = p;
    }
    else
    {
        phut = 0;
    }
}

void ThoiGian::setGiay(int gi)
{
    if (gi >= 0 && gi < 60)
    {
        giay = gi;
    }
    else
    {
        giay = 0;
    }
}

void ThoiGian::setThoiGian(int g, int p, int gi)
{
    setGio(g);
    setPhut(p);
    setGiay(gi);
}

bool ThoiGian::operator==(const ThoiGian &other) const
{
    return gio == other.gio && phut == other.phut && giay == other.giay;
}

bool ThoiGian::operator!=(const ThoiGian &other) const
{
    return !(*this == other);
}

bool ThoiGian::operator<(const ThoiGian &other) const
{
    if (gio != other.gio)
        return gio < other.gio;
    if (phut != other.phut)
        return phut < other.phut;
    return giay < other.giay;
}

bool ThoiGian::operator>(const ThoiGian &other) const
{
    return other < *this;
}

bool ThoiGian::operator<=(const ThoiGian &other) const
{
    return !(other < *this);
}

bool ThoiGian::operator>=(const ThoiGian &other) const
{
    return !(*this < other);
}

int ThoiGian::toSeconds() const
{
    return gio * 3600 + phut * 60 + giay;
}

void ThoiGian::fromSeconds(int totalSeconds)
{
    gio = (totalSeconds / 3600) % 24;
    phut = (totalSeconds % 3600) / 60;
    giay = totalSeconds % 60;
}

ThoiGian ThoiGian::add(int hours, int minutes) const
{
    int totalSeconds = toSeconds() + hours * 3600 + minutes * 60;
    ThoiGian result;
    result.fromSeconds(totalSeconds);
    return result;
}

int ThoiGian::diffMinutes(const ThoiGian &other) const
{
    int thisMinutes = gio * 60 + phut;
    int otherMinutes = other.gio * 60 + other.phut;
    return thisMinutes - otherMinutes;
}

bool ThoiGian::isValid() const
{
    return gio >= 0 && gio < 24 && phut >= 0 && phut < 60 && giay >= 0 && giay < 60;
}

void ThoiGian::display() const
{
    std::cout << std::setfill('0') << std::setw(2) << gio << ":"
              << std::setw(2) << phut << ":"
              << std::setw(2) << giay;
}

std::string ThoiGian::toString() const
{
    char buffer[16];
    sprintf(buffer, "%02d:%02d:%02d", gio, phut, giay);
    return std::string(buffer);
}

void ThoiGian::ghiFile(std::ostream &out) const
{
    out.write(reinterpret_cast<const char *>(&gio), sizeof(gio));
    out.write(reinterpret_cast<const char *>(&phut), sizeof(phut));
    out.write(reinterpret_cast<const char *>(&giay), sizeof(giay));
}

void ThoiGian::docFile(std::istream &in)
{
    in.read(reinterpret_cast<char *>(&gio), sizeof(gio));
    in.read(reinterpret_cast<char *>(&phut), sizeof(phut));
    in.read(reinterpret_cast<char *>(&giay), sizeof(giay));
}

std::ostream &operator<<(std::ostream &out, const ThoiGian &t)
{
    out << std::setfill('0') << std::setw(2) << t.gio << ":"
        << std::setw(2) << t.phut << ":"
        << std::setw(2) << t.giay;
    return out;
}

std::istream &operator>>(std::istream &in, ThoiGian &t)
{
    char sep;
    in >> t.gio >> sep >> t.phut >> sep >> t.giay;
    return in;
}
