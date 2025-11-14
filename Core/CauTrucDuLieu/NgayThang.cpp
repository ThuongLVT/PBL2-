#include "NgayThang.h"
#include <iomanip>
#include <sstream>
#include <ctime>

NgayThang::NgayThang() : ngay(1), thang(1), nam(2025) {}

NgayThang::NgayThang(int d, int m, int y)
{
    setNgayThang(d, m, y);
}

bool NgayThang::isLeapYear(int y) const
{
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int NgayThang::daysInMonth(int m, int y) const
{
    const int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2 && isLeapYear(y))
    {
        return 29;
    }
    return days[m];
}

void NgayThang::setNgay(int d)
{
    if (d >= 1 && d <= daysInMonth(thang, nam))
    {
        ngay = d;
    }
    else
    {
        ngay = 1;
    }
}

void NgayThang::setThang(int m)
{
    if (m >= 1 && m <= 12)
    {
        thang = m;
    }
    else
    {
        thang = 1;
    }
}

void NgayThang::setNam(int y)
{
    if (y >= 1900 && y <= 2100)
    {
        nam = y;
    }
    else
    {
        nam = 2025;
    }
}

void NgayThang::setNgayThang(int d, int m, int y)
{
    setNam(y);
    setThang(m);
    setNgay(d);
}

bool NgayThang::operator==(const NgayThang &other) const
{
    return ngay == other.ngay && thang == other.thang && nam == other.nam;
}

bool NgayThang::operator!=(const NgayThang &other) const
{
    return !(*this == other);
}

bool NgayThang::operator<(const NgayThang &other) const
{
    if (nam != other.nam)
        return nam < other.nam;
    if (thang != other.thang)
        return thang < other.thang;
    return ngay < other.ngay;
}

bool NgayThang::operator>(const NgayThang &other) const
{
    return other < *this;
}

bool NgayThang::operator<=(const NgayThang &other) const
{
    return !(other < *this);
}

bool NgayThang::operator>=(const NgayThang &other) const
{
    return !(*this < other);
}

int NgayThang::toDays() const
{
    // Tính số ngày từ 1/1/1900
    int days = 0;

    // Cộng số ngày của các năm trước
    for (int y = 1900; y < nam; y++)
    {
        days += isLeapYear(y) ? 366 : 365;
    }

    // Cộng số ngày của các tháng trong năm hiện tại
    for (int m = 1; m < thang; m++)
    {
        days += daysInMonth(m, nam);
    }

    // Cộng số ngày trong tháng hiện tại
    days += ngay;

    return days;
}

void NgayThang::fromDays(int totalDays)
{
    int y = 1900;
    while (totalDays > (isLeapYear(y) ? 366 : 365))
    {
        totalDays -= isLeapYear(y) ? 366 : 365;
        y++;
    }
    nam = y;

    int m = 1;
    while (totalDays > daysInMonth(m, nam))
    {
        totalDays -= daysInMonth(m, nam);
        m++;
    }
    thang = m;

    ngay = totalDays;
}

NgayThang NgayThang::addDays(int days) const
{
    int totalDays = toDays() + days;
    NgayThang result;
    result.fromDays(totalDays);
    return result;
}

int NgayThang::diffDays(const NgayThang &other) const
{
    return toDays() - other.toDays();
}

int NgayThang::tinhKhoangCach(const NgayThang &other) const
{
    return diffDays(other);
}

std::string NgayThang::toString() const
{
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << ngay << "/"
        << std::setw(2) << thang << "/" << nam;
    return oss.str();
}

NgayThang NgayThang::layNgayHienTai()
{
    std::time_t now = std::time(nullptr);
    std::tm *localTime = std::localtime(&now);
    return NgayThang(localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900);
}

bool NgayThang::isValid() const
{
    if (nam < 1900 || nam > 2100)
        return false;
    if (thang < 1 || thang > 12)
        return false;
    if (ngay < 1 || ngay > daysInMonth(thang, nam))
        return false;
    return true;
}

void NgayThang::display() const
{
    std::cout << std::setfill('0') << std::setw(2) << ngay << "/"
              << std::setw(2) << thang << "/" << nam;
}

void NgayThang::ghiFile(std::ostream &out) const
{
    out.write(reinterpret_cast<const char *>(&ngay), sizeof(ngay));
    out.write(reinterpret_cast<const char *>(&thang), sizeof(thang));
    out.write(reinterpret_cast<const char *>(&nam), sizeof(nam));
}

void NgayThang::docFile(std::istream &in)
{
    in.read(reinterpret_cast<char *>(&ngay), sizeof(ngay));
    in.read(reinterpret_cast<char *>(&thang), sizeof(thang));
    in.read(reinterpret_cast<char *>(&nam), sizeof(nam));
}

std::ostream &operator<<(std::ostream &out, const NgayThang &nt)
{
    out << std::setfill('0') << std::setw(2) << nt.ngay << "/"
        << std::setw(2) << nt.thang << "/" << nt.nam;
    return out;
}

std::istream &operator>>(std::istream &in, NgayThang &nt)
{
    char sep;
    in >> nt.ngay >> sep >> nt.thang >> sep >> nt.nam;
    return in;
}
