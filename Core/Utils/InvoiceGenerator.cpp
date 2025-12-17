#include "InvoiceGenerator.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <vector>

// Helper to count UTF-8 characters for alignment
int utf8Length(const std::string &str)
{
    int len = 0;
    for (size_t i = 0; i < str.length(); ++i)
    {
        if ((str[i] & 0xC0) != 0x80)
        {
            len++;
        }
    }
    return len;
}

std::string InvoiceGenerator::formatCurrency(double amount)
{
    long long val = (long long)amount;
    std::string s = std::to_string(val);
    int n = s.length() - 3;
    while (n > 0)
    {
        s.insert(n, ",");
        n -= 3;
    }
    return s;
}

std::string InvoiceGenerator::centerString(const std::string &str, int width)
{
    int len = utf8Length(str);
    if (len >= width)
        return str;
    int left = (width - len) / 2;
    int right = width - len - left;
    return std::string(left, ' ') + str + std::string(right, ' ');
}

std::string InvoiceGenerator::padRight(const std::string &str, int width)
{
    int len = utf8Length(str);
    if (len >= width)
        return str;
    return str + std::string(width - len, ' ');
}

std::string InvoiceGenerator::padLeft(const std::string &str, int width)
{
    int len = utf8Length(str);
    if (len >= width)
        return str;
    return std::string(width - len, ' ') + str;
}

std::string InvoiceGenerator::getCurrentDateTime()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "%d/%m/%Y %H:%M:%S", ltm);
    return std::string(buffer);
}

std::string InvoiceGenerator::getRow(const std::string &label, const std::string &value)
{
    // Label width approx 16 chars?
    // "Sân:            "
    // The template uses tabs or spaces.
    // Let's use a fixed width for label.
    // "Sân:" is 4 chars.
    // "Giá sân:" is 8 chars.
    // Let's align values at column 16.
    std::string l = label;
    int l_len = utf8Length(l);
    int padding = 16 - l_len;
    if (padding < 1)
        padding = 1;
    return l + std::string(padding, ' ') + value;
}

std::string InvoiceGenerator::generateBookingInvoice(const DatSan &datSan)
{
    std::stringstream ss;
    const int WIDTH = 64;
    std::string separator = std::string(WIDTH, '=');
    std::string dash = std::string(WIDTH, '-');

    ss << separator << "\n";
    ss << centerString("HÓA ĐƠN ĐẶT SÂN", WIDTH) << "\n";
    ss << centerString("SÂN BÓNG ĐÁ MINI ĐÀ NẴNG", WIDTH) << "\n";
    ss << centerString("Địa chỉ: 54 Nguyễn Lương Bằng, Đà Nẵng", WIDTH) << "\n";
    ss << centerString("Hotline: 0905.123.456", WIDTH) << "\n";
    ss << separator << "\n";

    ss << getRow("Mã đơn:", datSan.getMaDatSan()) << "\n";
    ss << getRow("Ngày lập:", getCurrentDateTime()) << "\n\n";

    ss << "THÔNG TIN KHÁCH HÀNG\n";
    ss << dash << "\n";
    if (datSan.getKhachHang())
    {
        ss << getRow("Tên khách hàng:", datSan.getKhachHang()->getHoTen()) << "\n";
        ss << getRow("Số điện thoại:", datSan.getKhachHang()->getSoDienThoai()) << "\n";

        std::string rankStr;
        switch (datSan.getKhachHang()->layHang())
        {
        case HangKhachHang::THUONG:
            rankStr = "Thường";
            break;
        case HangKhachHang::DONG:
            rankStr = "Đồng";
            break;
        case HangKhachHang::BAC:
            rankStr = "Bạc";
            break;
        case HangKhachHang::VANG:
            rankStr = "Vàng";
            break;
        case HangKhachHang::KIM_CUONG:
            rankStr = "Kim Cương";
            break;
        default:
            rankStr = "Thường";
            break;
        }
        ss << getRow("Hạng thành viên:", rankStr) << "\n";
    }
    else
    {
        ss << getRow("Tên khách hàng:", "Khách vãng lai") << "\n";
    }
    ss << "\n";

    ss << "THÔNG TIN ĐẶT SÂN\n";
    ss << dash << "\n";
    if (datSan.getSan())
    {
        std::string loaiSan = (datSan.getSan()->layLoaiSan() == LoaiSan::SAN_5) ? "Sân 5" : "Sân 7";
        ss << getRow("Sân:", datSan.getSan()->getTenSan() + " (" + loaiSan + ")") << "\n";
    }
    // Only show date, remove time
    ss << getRow("Ngày đá:", datSan.getThoiGianDat().toString().substr(0, 10)) << "\n";

    std::stringstream kgSS;
    kgSS << datSan.getKhungGio();
    ss << getRow("Khung giờ:", kgSS.str()) << "\n";

    double tienDichVu = 0;
    for (int i = 0; i < datSan.getDanhSachDichVu().size(); ++i)
    {
        tienDichVu += datSan.getDanhSachDichVu()[i].getThanhTien();
    }
    double tienSan = datSan.getTongTien() - tienDichVu;
    std::string giaSanStr = formatCurrency(tienSan) + " đ";

    ss << getRow("Giá sân:", giaSanStr) << "\n\n";

    ss << "DỊCH VỤ KÈM THEO\n";
    ss << dash << "\n";
    ss << centerString("STT", 5) << "  " << padRight("Tên dịch vụ", 24) << centerString("SL", 4) << "   " << padRight("Đơn giá", 15) << padLeft("Thành tiền", 11) << "\n";
    ss << dash << "\n";

    for (int i = 0; i < datSan.getDanhSachDichVu().size(); ++i)
    {
        DichVuDat dv = datSan.getDanhSachDichVu()[i];
        ss << centerString(std::to_string(i + 1), 5) << "  ";
        ss << padRight(dv.getDichVu()->layTenDichVu(), 24);
        ss << centerString(std::to_string(dv.getSoLuong()), 4) << "   ";
        ss << padRight(formatCurrency(dv.getDichVu()->layDonGia()) + " đ", 15);
        ss << padLeft(formatCurrency(dv.getThanhTien()) + " đ", 11) << "\n";
    }
    ss << dash << "\n";

    std::string tongDVStr = formatCurrency(tienDichVu) + " đ";
    std::string labelTongDV = "Tổng tiền dịch vụ:";
    ss << labelTongDV << padLeft(tongDVStr, WIDTH - utf8Length(labelTongDV)) << "\n\n";

    ss << "THANH TOÁN\n";
    ss << dash << "\n";

    // Calculate discount based on customer rank
    double discountPercent = 0;
    if (datSan.getKhachHang())
    {
        switch (datSan.getKhachHang()->layHang())
        {
        case HangKhachHang::DONG:
            discountPercent = 0.02;
            break;
        case HangKhachHang::BAC:
            discountPercent = 0.04;
            break;
        case HangKhachHang::VANG:
            discountPercent = 0.06;
            break;
        case HangKhachHang::KIM_CUONG:
            discountPercent = 0.10;
            break;
        default:
            discountPercent = 0;
            break;
        }
    }

    double tongCongTruocGiam = datSan.getTongTien();
    double discountAmount = tongCongTruocGiam * discountPercent;
    double finalTotal = tongCongTruocGiam - discountAmount;

    std::string tongCongStr = formatCurrency(tongCongTruocGiam) + " đ";
    std::string labelTongCong = "Tổng cộng:";
    ss << labelTongCong << padLeft(tongCongStr, WIDTH - utf8Length(labelTongCong)) << "\n";

    if (discountPercent > 0)
    {
        std::string discountLabel = "Giảm giá (" + std::to_string((int)(discountPercent * 100)) + "%):";
        std::string discountStr = "-" + formatCurrency(discountAmount) + " đ";
        ss << discountLabel << padLeft(discountStr, WIDTH - utf8Length(discountLabel)) << "\n";

        std::string finalLabel = "Thành tiền:";
        std::string finalStr = formatCurrency(finalTotal) + " đ";
        ss << finalLabel << padLeft(finalStr, WIDTH - utf8Length(finalLabel)) << "\n";
    }

    std::string cocStr = formatCurrency(datSan.getTienCoc()) + " đ";
    std::string labelCoc = "Đã đặt cọc:";
    ss << labelCoc << padLeft(cocStr, WIDTH - utf8Length(labelCoc)) << "\n";

    // Recalculate remaining based on final total
    double remaining = (discountPercent > 0 ? finalTotal : tongCongTruocGiam) - datSan.getTienCoc();
    std::string conLaiStr = formatCurrency(remaining) + " đ";
    std::string labelConLai = "Còn lại phải thu:";
    ss << labelConLai << padLeft(conLaiStr, WIDTH - utf8Length(labelConLai)) << "\n";

    ss << dash << "\n";
    ss << "Trạng thái: ĐÃ THANH TOÁN\n\n";

    ss << centerString("CẢM ƠN QUÝ KHÁCH VÀ HẸN GẶP LẠI!", WIDTH) << "\n";
    ss << separator << "\n";

    return ss.str();
}

std::string InvoiceGenerator::generateServiceInvoice(const DonHangDichVu &donHang)
{
    std::stringstream ss;
    const int WIDTH = 64;
    std::string separator = std::string(WIDTH, '=');
    std::string dash = std::string(WIDTH, '-');

    ss << separator << "\n";
    ss << centerString("HÓA ĐƠN DỊCH VỤ", WIDTH) << "\n";
    ss << centerString("SÂN BÓNG ĐÁ MINI ĐÀ NẴNG", WIDTH) << "\n";
    ss << centerString("Địa chỉ: 54 Nguyễn Lương Bằng, Đà Nẵng", WIDTH) << "\n";
    ss << centerString("Hotline: 0905.123.456", WIDTH) << "\n";
    ss << separator << "\n";

    ss << getRow("Mã đơn:", donHang.getMaDonHang()) << "\n";
    ss << getRow("Ngày lập:", getCurrentDateTime()) << "\n\n";

    ss << "THÔNG TIN KHÁCH HÀNG\n";
    ss << dash << "\n";
    if (donHang.getKhachHang())
    {
        ss << getRow("Tên khách hàng:", donHang.getKhachHang()->getHoTen()) << "\n";
        ss << getRow("Số điện thoại:", donHang.getKhachHang()->getSoDienThoai()) << "\n";

        std::string rankStr;
        switch (donHang.getKhachHang()->layHang())
        {
        case HangKhachHang::THUONG:
            rankStr = "Thường";
            break;
        case HangKhachHang::DONG:
            rankStr = "Đồng";
            break;
        case HangKhachHang::BAC:
            rankStr = "Bạc";
            break;
        case HangKhachHang::VANG:
            rankStr = "Vàng";
            break;
        case HangKhachHang::KIM_CUONG:
            rankStr = "Kim Cương";
            break;
        default:
            rankStr = "Thường";
            break;
        }
        ss << getRow("Hạng thành viên:", rankStr) << "\n";
    }
    else
    {
        ss << getRow("Tên khách hàng:", "Khách lẻ") << "\n";
    }
    ss << "\n";

    ss << "DANH SÁCH DỊCH VỤ\n";
    ss << dash << "\n";
    ss << centerString("STT", 5) << "  " << padRight("Tên dịch vụ", 24) << centerString("SL", 4) << "   " << padRight("Đơn giá", 15) << padLeft("Thành tiền", 11) << "\n";
    ss << dash << "\n";

    for (int i = 0; i < donHang.getDanhSachDichVu().size(); ++i)
    {
        DichVuDat dv = donHang.getDanhSachDichVu()[i];
        ss << centerString(std::to_string(i + 1), 5) << "  ";
        ss << padRight(dv.getDichVu()->layTenDichVu(), 24);
        ss << centerString(std::to_string(dv.getSoLuong()), 4) << "   ";
        ss << padRight(formatCurrency(dv.getDichVu()->layDonGia()) + " đ", 15);
        ss << padLeft(formatCurrency(dv.getThanhTien()) + " đ", 11) << "\n";
    }
    ss << dash << "\n\n";

    ss << "THANH TOÁN\n";
    ss << dash << "\n";

    std::string tongTienStr = formatCurrency(donHang.getTongTien()) + " đ";
    std::string labelTongTien = "Tổng tiền:";
    ss << labelTongTien << padLeft(tongTienStr, WIDTH - utf8Length(labelTongTien)) << "\n";

    if (donHang.getGiamGia() > 0)
    {
        std::string giamGiaStr = formatCurrency(donHang.getGiamGia()) + " đ";
        std::string labelGiamGia = "Giảm giá:";
        ss << labelGiamGia << padLeft(giamGiaStr, WIDTH - utf8Length(labelGiamGia)) << " (Nếu có)\n";
    }

    std::string thanhTienStr = formatCurrency(donHang.getThanhTien()) + " đ";
    std::string labelThanhTien = "Thành tiền:";
    ss << labelThanhTien << padLeft(thanhTienStr, WIDTH - utf8Length(labelThanhTien)) << "\n";

    ss << dash << "\n";
    ss << "Trạng thái: ĐÃ THANH TOÁN\n\n";

    ss << centerString("CẢM ƠN QUÝ KHÁCH VÀ HẸN GẶP LẠI!", WIDTH) << "\n";
    ss << separator << "\n";

    return ss.str();
}

std::string InvoiceGenerator::generateCancellationInvoice(const DatSan &datSan, const std::string &lyDo)
{
    std::stringstream ss;
    const int WIDTH = 64;
    std::string separator = std::string(WIDTH, '=');
    std::string dash = std::string(WIDTH, '-');

    ss << separator << "\n";
    ss << centerString("XÁC NHẬN HỦY ĐẶT SÂN (MẤT CỌC)", WIDTH) << "\n";
    ss << centerString("SÂN BÓNG ĐÁ MINI ĐÀ NẴNG", WIDTH) << "\n";
    ss << centerString("Địa chỉ: 54 Nguyễn Lương Bằng, Đà Nẵng", WIDTH) << "\n";
    ss << centerString("Hotline: 0905.123.456", WIDTH) << "\n";
    ss << separator << "\n";

    ss << getRow("Mã đơn:", datSan.getMaDatSan()) << "\n";
    ss << getRow("Ngày lập:", getCurrentDateTime()) << "\n\n";

    ss << "THÔNG TIN KHÁCH HÀNG\n";
    ss << dash << "\n";
    if (datSan.getKhachHang())
    {
        ss << getRow("Tên khách hàng:", datSan.getKhachHang()->getHoTen()) << "\n";
        ss << getRow("Số điện thoại:", datSan.getKhachHang()->getSoDienThoai()) << "\n";
    }
    ss << "\n";

    ss << "THÔNG TIN ĐẶT SÂN ĐÃ HỦY\n";
    ss << dash << "\n";
    if (datSan.getSan())
    {
        std::string loaiSan = (datSan.getSan()->layLoaiSan() == LoaiSan::SAN_5) ? "Sân 5" : "Sân 7";
        ss << getRow("Sân:", datSan.getSan()->getTenSan() + " (" + loaiSan + ")") << "\n";
    }
    // Only show date, remove time
    ss << getRow("Ngày đá:", datSan.getThoiGianDat().toString().substr(0, 10)) << "\n";

    std::stringstream kgSS;
    kgSS << datSan.getKhungGio();
    ss << getRow("Khung giờ:", kgSS.str()) << "\n\n";

    ss << "CHI TIẾT HỦY\n";
    ss << dash << "\n";
    ss << getRow("Lý do hủy:", lyDo) << "\n";
    ss << getRow("Thời gian hủy:", getCurrentDateTime()) << "\n\n";

    ss << "THÔNG TIN THANH TOÁN\n";
    ss << dash << "\n";

    std::string cocStr = formatCurrency(datSan.getTienCoc()) + " đ";
    std::string labelCoc = "Tiền cọc đã đóng:";
    ss << labelCoc << padLeft(cocStr, WIDTH - utf8Length(labelCoc)) << "\n";

    ss << getRow("Trạng thái cọc:", "KHÔNG HOÀN LẠI (MẤT CỌC)") << "\n";

    std::string hoanLaiStr = "0 đ";
    std::string labelHoanLai = "Số tiền hoàn lại:";
    ss << labelHoanLai << padLeft(hoanLaiStr, WIDTH - utf8Length(labelHoanLai)) << "\n";

    ss << dash << "\n";
    ss << "Trạng thái: ĐÃ HỦY\n\n";

    ss << centerString("XÁC NHẬN CỦA HỆ THỐNG", WIDTH) << "\n";
    ss << separator << "\n";

    return ss.str();
}
