#include "NgayGio.h"
#include <iostream>
#include <string>
#include <ctime>
#include <cstdio>
#include "../Utils/FileHelper.h"

NgayGio::NgayGio() {}

NgayGio::NgayGio(const NgayThang &nt, const ThoiGian &tg)
    : ngayThang(nt), thoiGian(tg) {}

NgayGio::NgayGio(int ngay, int thang, int nam, int gio, int phut, int giay)
    : ngayThang(ngay, thang, nam), thoiGian(gio, phut, giay) {}

void NgayGio::setNgayGio(int ngay, int thang, int nam, int gio, int phut, int giay)
{
    ngayThang.setNgayThang(ngay, thang, nam);
    thoiGian.setThoiGian(gio, phut, giay);
}

bool NgayGio::operator==(const NgayGio &other) const
{
    return ngayThang == other.ngayThang && thoiGian == other.thoiGian;
}

bool NgayGio::operator!=(const NgayGio &other) const
{
    return !(*this == other);
}

bool NgayGio::operator<(const NgayGio &other) const
{
    if (ngayThang != other.ngayThang)
    {
        return ngayThang < other.ngayThang;
    }
    return thoiGian < other.thoiGian;
}

bool NgayGio::operator>(const NgayGio &other) const
{
    return other < *this;
}

bool NgayGio::operator<=(const NgayGio &other) const
{
    return !(other < *this);
}

bool NgayGio::operator>=(const NgayGio &other) const
{
    return !(*this < other);
}

NgayGio NgayGio::addHours(int hours) const
{
    NgayGio result = *this;
    int totalHours = thoiGian.getGio() + hours;
    int extraDays = totalHours / 24;
    int newHours = totalHours % 24;

    if (newHours < 0)
    {
        newHours += 24;
        extraDays -= 1;
    }

    result.thoiGian.setGio(newHours);
    if (extraDays != 0)
    {
        result.ngayThang = result.ngayThang.addDays(extraDays);
    }

    return result;
}

NgayGio NgayGio::addMinutes(int minutes) const
{
    NgayGio result = *this;
    int totalMinutes = thoiGian.getGio() * 60 + thoiGian.getPhut() + minutes;
    int extraDays = totalMinutes / (24 * 60);
    int remainingMinutes = totalMinutes % (24 * 60);

    if (remainingMinutes < 0)
    {
        remainingMinutes += 24 * 60;
        extraDays -= 1;
    }

    int newHours = remainingMinutes / 60;
    int newMinutes = remainingMinutes % 60;

    result.thoiGian.setGio(newHours);
    result.thoiGian.setPhut(newMinutes);

    if (extraDays != 0)
    {
        result.ngayThang = result.ngayThang.addDays(extraDays);
    }

    return result;
}

NgayGio NgayGio::addDays(int days) const
{
    NgayGio result = *this;
    result.ngayThang = ngayThang.addDays(days);
    return result;
}

int NgayGio::diffMinutes(const NgayGio &other) const
{
    int daysDiff = ngayThang.diffDays(other.ngayThang);
    int minutesDiff = thoiGian.getGio() * 60 + thoiGian.getPhut() - (other.thoiGian.getGio() * 60 + other.thoiGian.getPhut());
    return daysDiff * 24 * 60 + minutesDiff;
}

int NgayGio::diffHours(const NgayGio &other) const
{
    return diffMinutes(other) / 60;
}

bool NgayGio::isSameDay(const NgayGio &other) const
{
    return ngayThang == other.ngayThang;
}

bool NgayGio::isValid() const
{
    return ngayThang.isValid() && thoiGian.isValid();
}

void NgayGio::display() const
{
    ngayThang.display();
    std::cout << " ";
    thoiGian.display();
}

std::string NgayGio::toString() const
{
    char buffer[32];
    sprintf(buffer, "%02d/%02d/%04d %02d:%02d:%02d",
            ngayThang.getNgay(), ngayThang.getThang(), ngayThang.getNam(),
            thoiGian.getGio(), thoiGian.getPhut(), thoiGian.getGiay());
    return std::string(buffer);
}

NgayGio NgayGio::layThoiGianHienTai()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return NgayGio(ltm->tm_mday, ltm->tm_mon + 1, 1900 + ltm->tm_year,
                   ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
}

void NgayGio::ghiFile(std::ostream &out) const
{
    ngayThang.ghiFile(out);
    thoiGian.ghiFile(out);
}

void NgayGio::docFile(std::istream &in)
{
    ngayThang.docFile(in);
    thoiGian.docFile(in);
}

void NgayGio::ghiFile(FILE *f) const
{
    int ngay = ngayThang.getNgay();
    int thang = ngayThang.getThang();
    int nam = ngayThang.getNam();
    int gio = thoiGian.getGio();
    int phut = thoiGian.getPhut();
    int giay = thoiGian.getGiay();

    fwrite(&ngay, sizeof(int), 1, f);
    fwrite(&thang, sizeof(int), 1, f);
    fwrite(&nam, sizeof(int), 1, f);
    fwrite(&gio, sizeof(int), 1, f);
    fwrite(&phut, sizeof(int), 1, f);
    fwrite(&giay, sizeof(int), 1, f);
}

void NgayGio::docFile(FILE *f)
{
    int ngay, thang, nam, gio, phut, giay;

    fread(&ngay, sizeof(int), 1, f);
    fread(&thang, sizeof(int), 1, f);
    fread(&nam, sizeof(int), 1, f);
    fread(&gio, sizeof(int), 1, f);
    fread(&phut, sizeof(int), 1, f);
    fread(&giay, sizeof(int), 1, f);

    ngayThang = NgayThang(ngay, thang, nam);
    thoiGian = ThoiGian(gio, phut, giay);
}

std::ostream &operator<<(std::ostream &out, const NgayGio &ng)
{
    out << ng.ngayThang << " " << ng.thoiGian;
    return out;
}

std::istream &operator>>(std::istream &in, NgayGio &ng)
{
    in >> ng.ngayThang >> ng.thoiGian;
    return in;
}
