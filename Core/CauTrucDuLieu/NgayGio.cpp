#include "NgayGio.h"
#include <iomanip>
#include <sstream>
#include <ctime>
#include <cstdio>
#include <string>

// ==========================================
// IMPLEMENTATION: NgayThang
// ==========================================

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
    int diff = other.toDays() - toDays();
    return diff >= 0 ? diff : -diff;
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

// ==========================================
// IMPLEMENTATION: ThoiGian
// ==========================================

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

// ==========================================
// IMPLEMENTATION: NgayGio
// ==========================================

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
