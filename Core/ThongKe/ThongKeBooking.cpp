#include "ThongKeBooking.h"
#include "../QuanLy/HeThongQuanLy.h"
#include "../Models/DatSan.h"
#include "../Models/San.h"
#include "../ThuatToan/QuickSort.h"
#include <iostream>
#include <cstdio>

using namespace std;

// Helper function: Tính ngày trong tuần từ NgayThang
// Sử dụng Zeller's congruence algorithm
// Trả về: 0=CN, 1=T2, 2=T3, ..., 6=T7
static int tinhThuTrongTuan(const NgayThang &ngay)
{
    int d = ngay.getNgay();
    int m = ngay.getThang();
    int y = ngay.getNam();

    // Điều chỉnh cho Zeller: tháng 1,2 -> tháng 13,14 của năm trước
    if (m < 3)
    {
        m += 12;
        y--;
    }

    int k = y % 100;
    int j = y / 100;

    // Zeller's formula
    int h = (d + (13 * (m + 1)) / 5 + k + k / 4 + j / 4 - 2 * j) % 7;

    // Chuyển từ Zeller (0=Sat, 1=Sun, ...) sang (0=Sun, 1=Mon, ...)
    int dayOfWeek = ((h + 6) % 7);

    return dayOfWeek;
}

// ========== CONSTRUCTORS ==========

ThongKeBooking::ThongKeBooking()
    : ThongKe(), tongSoBooking(0), soBookingHoanThanh(0), soBookingHuy(0),
      soBookingDangCho(0), tyLeHoanThanh(0.0), tyLeHuy(0.0),
      soBookingSan5(0), soBookingSan7(0), doanhThuSan5(0.0), doanhThuSan7(0.0),
      soBookingGioVang(0), soBookingGioThuong(0), tyLeSuDungTrungBinh(0.0),
      heThong(nullptr)
{
    tieuDe = std::string("Bao cao thong ke Booking & San");
}

ThongKeBooking::ThongKeBooking(const NgayThang &tu, const NgayThang &den, HeThongQuanLy *ht)
    : ThongKe(std::string("Bao cao thong ke Booking & San"), tu, den),
      tongSoBooking(0), soBookingHoanThanh(0), soBookingHuy(0),
      soBookingDangCho(0), tyLeHoanThanh(0.0), tyLeHuy(0.0),
      soBookingSan5(0), soBookingSan7(0), doanhThuSan5(0.0), doanhThuSan7(0.0),
      soBookingGioVang(0), soBookingGioThuong(0), tyLeSuDungTrungBinh(0.0),
      heThong(ht)
{
    tinhToan();
}

ThongKeBooking::ThongKeBooking(const ThongKeBooking &other) : ThongKe(other)
{
    tongSoBooking = other.tongSoBooking;
    soBookingHoanThanh = other.soBookingHoanThanh;
    soBookingHuy = other.soBookingHuy;
    soBookingDangCho = other.soBookingDangCho;
    tyLeHoanThanh = other.tyLeHoanThanh;
    tyLeHuy = other.tyLeHuy;
    soBookingSan5 = other.soBookingSan5;
    soBookingSan7 = other.soBookingSan7;
    doanhThuSan5 = other.doanhThuSan5;
    doanhThuSan7 = other.doanhThuSan7;
    soBookingGioVang = other.soBookingGioVang;
    soBookingGioThuong = other.soBookingGioThuong;
    thongKeKhungGio = other.thongKeKhungGio;
    thongKeTheoNgay = other.thongKeTheoNgay;
    topSan = other.topSan;
    tyLeSuDungTrungBinh = other.tyLeSuDungTrungBinh;
    heThong = other.heThong;
}

ThongKeBooking::~ThongKeBooking() {}

// ========== OPERATORS ==========

ThongKeBooking &ThongKeBooking::operator=(const ThongKeBooking &other)
{
    if (this != &other)
    {
        ThongKe::operator=(other);
        tongSoBooking = other.tongSoBooking;
        soBookingHoanThanh = other.soBookingHoanThanh;
        soBookingHuy = other.soBookingHuy;
        soBookingDangCho = other.soBookingDangCho;
        tyLeHoanThanh = other.tyLeHoanThanh;
        tyLeHuy = other.tyLeHuy;
        soBookingSan5 = other.soBookingSan5;
        soBookingSan7 = other.soBookingSan7;
        doanhThuSan5 = other.doanhThuSan5;
        doanhThuSan7 = other.doanhThuSan7;
        soBookingGioVang = other.soBookingGioVang;
        soBookingGioThuong = other.soBookingGioThuong;
        thongKeKhungGio = other.thongKeKhungGio;
        thongKeTheoNgay = other.thongKeTheoNgay;
        topSan = other.topSan;
        tyLeSuDungTrungBinh = other.tyLeSuDungTrungBinh;
        heThong = other.heThong;
    }
    return *this;
}

// ========== GETTERS ==========

int ThongKeBooking::getTongSoBooking() const { return tongSoBooking; }
int ThongKeBooking::getSoBookingHoanThanh() const { return soBookingHoanThanh; }
int ThongKeBooking::getSoBookingHuy() const { return soBookingHuy; }
int ThongKeBooking::getSoBookingDangCho() const { return soBookingDangCho; }
double ThongKeBooking::getTyLeHoanThanh() const { return tyLeHoanThanh; }
double ThongKeBooking::getTyLeHuy() const { return tyLeHuy; }

int ThongKeBooking::getSoBookingSan5() const { return soBookingSan5; }
int ThongKeBooking::getSoBookingSan7() const { return soBookingSan7; }
double ThongKeBooking::getDoanhThuSan5() const { return doanhThuSan5; }
double ThongKeBooking::getDoanhThuSan7() const { return doanhThuSan7; }

int ThongKeBooking::getSoBookingGioVang() const { return soBookingGioVang; }
int ThongKeBooking::getSoBookingGioThuong() const { return soBookingGioThuong; }
const MangDong<ThongKeKhungGio> &ThongKeBooking::getThongKeKhungGio() const { return thongKeKhungGio; }

const MangDong<ThongKeTheoNgay> &ThongKeBooking::getThongKeTheoNgay() const { return thongKeTheoNgay; }

const MangDong<ThongTinSanTop> &ThongKeBooking::getTopSan() const { return topSan; }

double ThongKeBooking::getTyLeSuDungTrungBinh() const { return tyLeSuDungTrungBinh; }

// ========== MAIN CALCULATION ==========

void ThongKeBooking::tinhToan()
{
    if (heThong == nullptr)
        return;

    // Reset tất cả metrics
    tongSoBooking = 0;
    soBookingHoanThanh = 0;
    soBookingHuy = 0;
    soBookingDangCho = 0;
    soBookingSan5 = 0;
    soBookingSan7 = 0;
    doanhThuSan5 = 0.0;
    doanhThuSan7 = 0.0;
    soBookingGioVang = 0;
    soBookingGioThuong = 0;

    const MangDong<DatSan *> &danhSach = heThong->layDanhSachDatSan();

    for (int i = 0; i < danhSach.size(); i++)
    {
        DatSan *ds = danhSach[i];
        NgayGio tgDat = ds->getThoiGianDat();
        NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());

        // Kiểm tra trong khoảng thời gian
        if (ngayDat >= tuNgay && ngayDat <= denNgay)
        {
            tongSoBooking++;

            // Đếm theo trạng thái
            switch (ds->getTrangThai())
            {
            case HOAN_THANH:
                soBookingHoanThanh++;
                break;
            case DA_HUY:
                soBookingHuy++;
                break;
            case DA_DAT:
                soBookingDangCho++;
                break;
            }

            // Đếm theo loại sân
            San *san = ds->getSan();
            if (san != nullptr)
            {
                if (san->layLoaiSan() == LoaiSan::SAN_5)
                {
                    soBookingSan5++;
                    if (ds->getTrangThai() == HOAN_THANH)
                    {
                        doanhThuSan5 += ds->getTongTien();
                    }
                }
                else if (san->layLoaiSan() == LoaiSan::SAN_7)
                {
                    soBookingSan7++;
                    if (ds->getTrangThai() == HOAN_THANH)
                    {
                        doanhThuSan7 += ds->getTongTien();
                    }
                }
            }

            // Đếm theo giờ vàng
            if (laGioVang(ds))
            {
                soBookingGioVang++;
            }
            else
            {
                soBookingGioThuong++;
            }
        }
    }

    // Tính tỷ lệ
    if (tongSoBooking > 0)
    {
        tyLeHoanThanh = (double)soBookingHoanThanh / tongSoBooking * 100;
        tyLeHuy = (double)soBookingHuy / tongSoBooking * 100;
    }

    // Tính các thống kê chi tiết
    tinhTopSan(5);
    tinhThongKeKhungGio();
    tinhThongKeTheoNgay();
    tinhTyLeSuDung();
}

// ========== SPECIFIC METHODS ==========

bool ThongKeBooking::laGioVang(DatSan *ds)
{
    if (ds == nullptr)
        return false;

    KhungGio kg = ds->getKhungGio();
    int gioBatDau = kg.getGioBatDau().getGio();

    // Giờ vàng: 18:00 - 21:00
    return (gioBatDau >= 18 && gioBatDau < 21);
}

void ThongKeBooking::tinhTopSan(int soLuong)
{
    if (heThong == nullptr)
        return;

    topSan = MangDong<ThongTinSanTop>();
    const MangDong<San *> &danhSachSan = heThong->layDanhSachSan();

    // Tạo thông tin cho từng sân
    for (int i = 0; i < danhSachSan.size(); i++)
    {
        San *san = danhSachSan[i];
        ThongTinSanTop info;
        info.san = san;
        info.soLuotDat = demSoBookingCuaSan(san);
        info.doanhThu = tinhDoanhThuSan(san);
        topSan.push_back(info);
    }

    // Sắp xếp theo số lượt đặt (QuickSort O(n log n))
    QuickSort<ThongTinSanTop>::sort(topSan, [](const ThongTinSanTop &a, const ThongTinSanTop &b) {
        return a.soLuotDat > b.soLuotDat; // Giảm dần
    });

    // Giữ lại top N
    if (topSan.size() > soLuong)
    {
        MangDong<ThongTinSanTop> temp;
        for (int i = 0; i < soLuong; i++)
        {
            temp.push_back(topSan[i]);
        }
        topSan = temp;
    }
}

int ThongKeBooking::demSoBookingCuaSan(San *san)
{
    if (heThong == nullptr || san == nullptr)
        return 0;

    int count = 0;
    const MangDong<DatSan *> &danhSach = heThong->layDanhSachDatSan();

    for (int i = 0; i < danhSach.size(); i++)
    {
        DatSan *ds = danhSach[i];
        if (ds->getSan() == san)
        {
            NgayGio tgDat = ds->getThoiGianDat();
            NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());

            if (ngayDat >= tuNgay && ngayDat <= denNgay)
            {
                count++;
            }
        }
    }

    return count;
}

double ThongKeBooking::tinhDoanhThuSan(San *san)
{
    if (heThong == nullptr || san == nullptr)
        return 0.0;

    double doanhThu = 0.0;
    const MangDong<DatSan *> &danhSach = heThong->layDanhSachDatSan();

    for (int i = 0; i < danhSach.size(); i++)
    {
        DatSan *ds = danhSach[i];
        if (ds->getSan() == san && ds->getTrangThai() == HOAN_THANH)
        {
            NgayGio tgDat = ds->getThoiGianDat();
            NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());

            if (ngayDat >= tuNgay && ngayDat <= denNgay)
            {
                doanhThu += ds->getTongTien();
            }
        }
    }

    return doanhThu;
}

void ThongKeBooking::tinhThongKeKhungGio()
{
    if (heThong == nullptr)
        return;

    // Khởi tạo các khung giờ (mỗi khung 2 tiếng)
    thongKeKhungGio = MangDong<ThongKeKhungGio>();
    for (int i = 6; i < 22; i += 2) // 6:00 - 22:00
    {
        thongKeKhungGio.push_back(ThongKeKhungGio(i, i + 2));
    }

    const MangDong<DatSan *> &danhSach = heThong->layDanhSachDatSan();

    for (int i = 0; i < danhSach.size(); i++)
    {
        DatSan *ds = danhSach[i];
        NgayGio tgDat = ds->getThoiGianDat();
        NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());

        if (ngayDat >= tuNgay && ngayDat <= denNgay)
        {
            KhungGio kg = ds->getKhungGio();
            int gioBatDau = kg.getGioBatDau().getGio();

            // Tìm khung giờ phù hợp
            for (int j = 0; j < thongKeKhungGio.size(); j++)
            {
                if (gioBatDau >= thongKeKhungGio[j].gioBatDau &&
                    gioBatDau < thongKeKhungGio[j].gioKetThuc)
                {
                    thongKeKhungGio[j].soLuotDat++;
                    break;
                }
            }
        }
    }

    // Tính tỷ lệ
    for (int i = 0; i < thongKeKhungGio.size(); i++)
    {
        if (tongSoBooking > 0)
        {
            thongKeKhungGio[i].tyLe = (double)thongKeKhungGio[i].soLuotDat / tongSoBooking * 100;
        }
    }
}

void ThongKeBooking::tinhThongKeTheoNgay()
{
    if (heThong == nullptr)
        return;

    // Khởi tạo 7 ngày trong tuần
    thongKeTheoNgay = MangDong<ThongKeTheoNgay>();
    for (int i = 0; i < 7; i++)
    {
        ThongKeTheoNgay tk;
        tk.ngayTrongTuan = i;
        thongKeTheoNgay.push_back(tk);
    }

    const MangDong<DatSan *> &danhSach = heThong->layDanhSachDatSan();

    for (int i = 0; i < danhSach.size(); i++)
    {
        DatSan *ds = danhSach[i];
        NgayGio tgDat = ds->getThoiGianDat();
        NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());

        if (ngayDat >= tuNgay && ngayDat <= denNgay)
        {
            int ngayTrongTuan = tinhThuTrongTuan(ngayDat); // 0=CN, 1=T2...

            if (ngayTrongTuan >= 0 && ngayTrongTuan < 7)
            {
                thongKeTheoNgay[ngayTrongTuan].soLuotDat++;

                if (ds->getTrangThai() == HOAN_THANH)
                {
                    thongKeTheoNgay[ngayTrongTuan].doanhThu += ds->getTongTien();
                }
            }
        }
    }
}

void ThongKeBooking::tinhTyLeSuDung()
{
    if (heThong == nullptr)
        return;

    const MangDong<San *> &danhSachSan = heThong->layDanhSachSan();

    if (danhSachSan.size() == 0)
    {
        tyLeSuDungTrungBinh = 0.0;
        return;
    }

    // Tính số ngày trong khoảng
    int soNgay = tuNgay.tinhKhoangCach(denNgay);
    if (soNgay <= 0)
        soNgay = 1;

    // Giả sử mỗi sân có thể hoạt động 16 giờ/ngày (6:00-22:00)
    int tongGioKhaDung = danhSachSan.size() * soNgay * 16;

    // Tính tổng giờ đã sử dụng
    double tongGioSuDung = 0.0;
    const MangDong<DatSan *> &danhSach = heThong->layDanhSachDatSan();

    for (int i = 0; i < danhSach.size(); i++)
    {
        DatSan *ds = danhSach[i];
        NgayGio tgDat = ds->getThoiGianDat();
        NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());

        if (ngayDat >= tuNgay && ngayDat <= denNgay &&
            ds->getTrangThai() != DA_HUY)
        {
            tongGioSuDung += ds->getKhungGio().tinhSoGio();
        }
    }

    if (tongGioKhaDung > 0)
    {
        tyLeSuDungTrungBinh = tongGioSuDung / tongGioKhaDung * 100;
    }

    // Cập nhật tỷ lệ sử dụng cho từng sân trong topSan
    for (int i = 0; i < topSan.size(); i++)
    {
        if (topSan[i].san != nullptr)
        {
            int gioKhaDungSan = soNgay * 16;
            double gioSuDungSan = 0.0;

            for (int j = 0; j < danhSach.size(); j++)
            {
                DatSan *ds = danhSach[j];
                if (ds->getSan() == topSan[i].san)
                {
                    NgayGio tgDat = ds->getThoiGianDat();
                    NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());

                    if (ngayDat >= tuNgay && ngayDat <= denNgay &&
                        ds->getTrangThai() != DA_HUY)
                    {
                        gioSuDungSan += ds->getKhungGio().tinhSoGio();
                    }
                }
            }

            if (gioKhaDungSan > 0)
            {
                topSan[i].tyLeSuDung = gioSuDungSan / gioKhaDungSan * 100;
            }
        }
    }
}

MangDong<MangDong<int>> ThongKeBooking::getHeatmapData()
{
    // Tạo mảng 2D: 7 ngày x 24 giờ
    MangDong<MangDong<int>> heatmap;

    for (int day = 0; day < 7; day++)
    {
        MangDong<int> row;
        for (int hour = 0; hour < 24; hour++)
        {
            row.push_back(0);
        }
        heatmap.push_back(row);
    }

    if (heThong == nullptr)
        return heatmap;

    const MangDong<DatSan *> &danhSach = heThong->layDanhSachDatSan();

    for (int i = 0; i < danhSach.size(); i++)
    {
        DatSan *ds = danhSach[i];
        NgayGio tgDat = ds->getThoiGianDat();
        NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());

        if (ngayDat >= tuNgay && ngayDat <= denNgay)
        {
            int ngayTrongTuan = tinhThuTrongTuan(ngayDat);
            int gioBatDau = ds->getKhungGio().getGioBatDau().getGio();

            if (ngayTrongTuan >= 0 && ngayTrongTuan < 7 &&
                gioBatDau >= 0 && gioBatDau < 24)
            {
                heatmap[ngayTrongTuan][gioBatDau]++;
            }
        }
    }

    return heatmap;
}

// ========== DISPLAY METHODS ==========

void ThongKeBooking::hienThi() const
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "  " << tieuDe.c_str() << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Thoi gian: " << tuNgay.toString().c_str()
              << " - " << denNgay.toString().c_str() << std::endl;
    std::cout << "Ngay tao bao cao: " << ngayTao.toString().c_str() << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    std::cout << "TONG QUAN BOOKING:" << std::endl;
    std::cout << "  Tong so booking:     " << tongSoBooking << std::endl;
    std::cout << "  - Hoan thanh:        " << soBookingHoanThanh
              << " (" << tyLeHoanThanh << "%)" << std::endl;
    std::cout << "  - Da huy:            " << soBookingHuy
              << " (" << tyLeHuy << "%)" << std::endl;
    std::cout << "  - Dang cho:          " << soBookingDangCho << std::endl;

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "THEO LOAI SAN:" << std::endl;
    std::cout << "  San 5 nguoi:         " << soBookingSan5
              << " - " << doanhThuSan5 << " VND" << std::endl;
    std::cout << "  San 7 nguoi:         " << soBookingSan7
              << " - " << doanhThuSan7 << " VND" << std::endl;

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "THEO KHUNG GIO:" << std::endl;
    std::cout << "  Gio vang (18-21h):   " << soBookingGioVang << std::endl;
    std::cout << "  Gio thuong:          " << soBookingGioThuong << std::endl;

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "TY LE SU DUNG TRUNG BINH: " << tyLeSuDungTrungBinh << "%" << std::endl;

    if (topSan.size() > 0)
    {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "TOP SAN DUOC DAT NHIEU NHAT:" << std::endl;
        for (int i = 0; i < topSan.size(); i++)
        {
            const ThongTinSanTop &info = topSan[i];
            if (info.san != nullptr)
            {
                std::cout << "  " << (i + 1) << ". " << info.san->getTenSan().c_str()
                          << " - " << info.soLuotDat << " luot"
                          << " - " << info.doanhThu << " VND"
                          << " - " << info.tyLeSuDung << "%" << std::endl;
            }
        }
    }

    std::cout << "========================================\n"
              << std::endl;
}

void ThongKeBooking::xuatFile(const std::string &tenFile) const
{
    FILE *f = fopen(tenFile.c_str(), "w");
    if (f == nullptr)
    {
        std::cout << "Khong the mo file de xuat!" << std::endl;
        return;
    }

    fprintf(f, "========================================\n");
    fprintf(f, "  %s\n", tieuDe.c_str());
    fprintf(f, "========================================\n");
    fprintf(f, "Thoi gian: %s - %s\n", tuNgay.toString().c_str(), denNgay.toString().c_str());
    fprintf(f, "Ngay tao bao cao: %s\n", ngayTao.toString().c_str());
    fprintf(f, "----------------------------------------\n");

    fprintf(f, "TONG QUAN BOOKING:\n");
    fprintf(f, "  Tong so booking:     %d\n", tongSoBooking);
    fprintf(f, "  - Hoan thanh:        %d (%.1f%%)\n", soBookingHoanThanh, tyLeHoanThanh);
    fprintf(f, "  - Da huy:            %d (%.1f%%)\n", soBookingHuy, tyLeHuy);
    fprintf(f, "  - Dang cho:          %d\n", soBookingDangCho);

    fprintf(f, "----------------------------------------\n");
    fprintf(f, "THEO LOAI SAN:\n");
    fprintf(f, "  San 5 nguoi:         %d - %.0f VND\n", soBookingSan5, doanhThuSan5);
    fprintf(f, "  San 7 nguoi:         %d - %.0f VND\n", soBookingSan7, doanhThuSan7);

    fprintf(f, "----------------------------------------\n");
    fprintf(f, "THEO KHUNG GIO:\n");
    fprintf(f, "  Gio vang (18-21h):   %d\n", soBookingGioVang);
    fprintf(f, "  Gio thuong:          %d\n", soBookingGioThuong);

    fprintf(f, "----------------------------------------\n");
    fprintf(f, "TY LE SU DUNG TRUNG BINH: %.1f%%\n", tyLeSuDungTrungBinh);

    if (topSan.size() > 0)
    {
        fprintf(f, "----------------------------------------\n");
        fprintf(f, "TOP SAN DUOC DAT NHIEU NHAT:\n");
        for (int i = 0; i < topSan.size(); i++)
        {
            const ThongTinSanTop &info = topSan[i];
            if (info.san != nullptr)
            {
                fprintf(f, "  %d. %s - %d luot - %.0f VND - %.1f%%\n",
                        i + 1, info.san->getTenSan().c_str(),
                        info.soLuotDat, info.doanhThu, info.tyLeSuDung);
            }
        }
    }

    fprintf(f, "========================================\n");

    fclose(f);
    std::cout << "Da xuat bao cao ra file: " << tenFile.c_str() << std::endl;
}
