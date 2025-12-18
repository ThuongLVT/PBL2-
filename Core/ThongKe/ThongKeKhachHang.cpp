#include "ThongKeKhachHang.h"
#include "../QuanLy/HeThongQuanLy.h"
#include "../Models/KhachHang.h"
#include "../Models/DatSan.h"
#include "../ThuatToan/QuickSort.h"
#include <iostream>
#include <cstdio>

using namespace std;

// Constructor mặc định
ThongKeKhachHang::ThongKeKhachHang()
    : ThongKe(), tongSoKhachHang(0), soKhachHangMoi(0), soKhachHangQuayLai(0),
      soKhachHangHoatDong(0), chiTieuTrungBinh(0.0), soKhachHangMoiKyTruoc(0),
      phanTramTangKhachMoi(0.0), customerLifetimeValue(0.0), tanSuatDatSanTB(0.0),
      soKhachHangRoiBo(0), heThong(nullptr)
{
    tieuDe = std::string("Bao cao thong ke khach hang");
}

// Constructor với tham số
ThongKeKhachHang::ThongKeKhachHang(const NgayThang &tu, const NgayThang &den, HeThongQuanLy *ht)
    : ThongKe(std::string("Bao cao thong ke khach hang"), tu, den),
      tongSoKhachHang(0), soKhachHangMoi(0), soKhachHangQuayLai(0),
      soKhachHangHoatDong(0), chiTieuTrungBinh(0.0), soKhachHangMoiKyTruoc(0),
      phanTramTangKhachMoi(0.0), customerLifetimeValue(0.0), tanSuatDatSanTB(0.0),
      soKhachHangRoiBo(0), heThong(ht)
{
    tinhToan();
}

// Copy constructor
ThongKeKhachHang::ThongKeKhachHang(const ThongKeKhachHang &other) : ThongKe(other)
{
    tongSoKhachHang = other.tongSoKhachHang;
    soKhachHangMoi = other.soKhachHangMoi;
    soKhachHangQuayLai = other.soKhachHangQuayLai;
    soKhachHangHoatDong = other.soKhachHangHoatDong;
    chiTieuTrungBinh = other.chiTieuTrungBinh;
    soKhachHangMoiKyTruoc = other.soKhachHangMoiKyTruoc;
    phanTramTangKhachMoi = other.phanTramTangKhachMoi;
    topKhachHang = other.topKhachHang;
    thongKeTheoHang = other.thongKeTheoHang;
    thongKeTheoThang = other.thongKeTheoThang;
    customerLifetimeValue = other.customerLifetimeValue;
    tanSuatDatSanTB = other.tanSuatDatSanTB;
    soKhachHangRoiBo = other.soKhachHangRoiBo;
    heThong = other.heThong;
}

// Destructor
ThongKeKhachHang::~ThongKeKhachHang() {}

// Assignment operator
ThongKeKhachHang &ThongKeKhachHang::operator=(const ThongKeKhachHang &other)
{
    if (this != &other)
    {
        ThongKe::operator=(other);
        tongSoKhachHang = other.tongSoKhachHang;
        soKhachHangMoi = other.soKhachHangMoi;
        soKhachHangQuayLai = other.soKhachHangQuayLai;
        soKhachHangHoatDong = other.soKhachHangHoatDong;
        chiTieuTrungBinh = other.chiTieuTrungBinh;
        soKhachHangMoiKyTruoc = other.soKhachHangMoiKyTruoc;
        phanTramTangKhachMoi = other.phanTramTangKhachMoi;
        topKhachHang = other.topKhachHang;
        thongKeTheoHang = other.thongKeTheoHang;
        thongKeTheoThang = other.thongKeTheoThang;
        customerLifetimeValue = other.customerLifetimeValue;
        tanSuatDatSanTB = other.tanSuatDatSanTB;
        soKhachHangRoiBo = other.soKhachHangRoiBo;
        heThong = other.heThong;
    }
    return *this;
}

// ===== GETTERS - CƠ BẢN =====
int ThongKeKhachHang::getTongSoKhachHang() const { return tongSoKhachHang; }
int ThongKeKhachHang::getSoKhachHangMoi() const { return soKhachHangMoi; }
int ThongKeKhachHang::getSoKhachHangQuayLai() const { return soKhachHangQuayLai; }
int ThongKeKhachHang::getSoKhachHangHoatDong() const { return soKhachHangHoatDong; }
double ThongKeKhachHang::getChiTieuTrungBinh() const { return chiTieuTrungBinh; }

// ===== GETTERS - SO SÁNH =====
int ThongKeKhachHang::getSoKhachHangMoiKyTruoc() const { return soKhachHangMoiKyTruoc; }
double ThongKeKhachHang::getPhanTramTangKhachMoi() const { return phanTramTangKhachMoi; }

// ===== GETTERS - TOP & PHÂN LOẠI =====
const MangDong<ThongTinKhachHangTop> &ThongKeKhachHang::getTopKhachHang() const { return topKhachHang; }
const MangDong<ThongKeHangKhachHang> &ThongKeKhachHang::getThongKeTheoHang() const { return thongKeTheoHang; }
const MangDong<ThongKeKhachHangTheoThang> &ThongKeKhachHang::getThongKeTheoThang() const { return thongKeTheoThang; }

// ===== GETTERS - INSIGHTS =====
double ThongKeKhachHang::getCustomerLifetimeValue() const { return customerLifetimeValue; }
double ThongKeKhachHang::getTanSuatDatSanTB() const { return tanSuatDatSanTB; }
int ThongKeKhachHang::getSoKhachHangRoiBo() const { return soKhachHangRoiBo; }

// Methods
void ThongKeKhachHang::tinhToan()
{
    if (heThong == nullptr)
        return;

    // Reset
    tongSoKhachHang = 0;
    soKhachHangMoi = 0;
    soKhachHangQuayLai = 0;
    soKhachHangHoatDong = 0;
    chiTieuTrungBinh = 0.0;

    const MangDong<KhachHang *> &danhSachKH = heThong->layDanhSachKhachHang();
    tongSoKhachHang = danhSachKH.size();

    double tongChiTieu = 0.0;

    for (int i = 0; i < danhSachKH.size(); i++)
    {
        KhachHang *kh = danhSachKH[i];

        // Kiểm tra đăng ký trong kỳ (Khách Mới)
        if (laDangKyTrongKy(kh))
        {
            soKhachHangMoi++;
        }
        else
        {
            // Khách đăng ký trước kỳ (Khách Cũ)
            soKhachHangQuayLai++;
        }

        // Kiểm tra hoạt động trong kỳ
        if (laHoatDongTrongKy(kh))
        {
            soKhachHangHoatDong++;
        }

        double chiTieu = tinhTongChiTieuCuaKhachHang(kh);
        tongChiTieu += chiTieu;
    }

    if (tongSoKhachHang > 0)
    {
        chiTieuTrungBinh = tongChiTieu / tongSoKhachHang;
    }

    // Tính các thống kê bổ sung
    tinhTopKhachHang(10);
    tinhThongKeTheoHang();
    tinhThongKeTheoThang();
    tinhCustomerInsights();
    tinhSoSanhKyTruoc();
}

void ThongKeKhachHang::hienThi() const
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "  " << tieuDe.c_str() << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Thoi gian: " << tuNgay.toString().c_str()
              << " - " << denNgay.toString().c_str() << std::endl;
    std::cout << "Ngay tao bao cao: " << ngayTao.toString().c_str() << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Tong so khach hang:    " << tongSoKhachHang << std::endl;
    std::cout << "  - Khach hang moi:    " << soKhachHangMoi << std::endl;
    std::cout << "  - Khach quay lai:    " << soKhachHangQuayLai << std::endl;
    std::cout << "Chi tieu trung binh:   " << chiTieuTrungBinh << " VND" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    if (topKhachHang.size() > 0)
    {
        std::cout << "TOP KHACH HANG:" << std::endl;
        for (int i = 0; i < topKhachHang.size() && i < 10; i++)
        {
            const ThongTinKhachHangTop &info = topKhachHang[i];
            if (info.khachHang != nullptr)
            {
                std::cout << (i + 1) << ". " << info.khachHang->getHoTen().c_str()
                          << " - " << info.soDonDat << " don"
                          << " - " << info.tongChiTieu << " VND" << std::endl;
            }
        }
    }

    std::cout << "========================================\n"
              << std::endl;
}

void ThongKeKhachHang::xuatFile(const std::string &tenFile) const
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
    fprintf(f, "Tong so khach hang:    %d\n", tongSoKhachHang);
    fprintf(f, "  - Khach hang moi:    %d\n", soKhachHangMoi);
    fprintf(f, "  - Khach quay lai:    %d\n", soKhachHangQuayLai);
    fprintf(f, "Chi tieu trung binh:   %.2f VND\n", chiTieuTrungBinh);
    fprintf(f, "----------------------------------------\n");

    if (topKhachHang.size() > 0)
    {
        fprintf(f, "TOP KHACH HANG:\n");
        for (int i = 0; i < topKhachHang.size() && i < 10; i++)
        {
            const ThongTinKhachHangTop &info = topKhachHang[i];
            if (info.khachHang != nullptr)
            {
                fprintf(f, "%d. %s - %d don - %.2f VND\n",
                        i + 1, info.khachHang->getHoTen().c_str(),
                        info.soDonDat, info.tongChiTieu);
            }
        }
    }

    fprintf(f, "========================================\n");

    fclose(f);
    std::cout << "Da xuat bao cao ra file: " << tenFile.c_str() << std::endl;
}

void ThongKeKhachHang::tinhTopKhachHang(int soLuong)
{
    if (heThong == nullptr)
        return;

    topKhachHang = MangDong<ThongTinKhachHangTop>();
    const MangDong<KhachHang *> &danhSachKH = heThong->layDanhSachKhachHang();

    // Tạo danh sách thông tin khách hàng
    for (int i = 0; i < danhSachKH.size(); i++)
    {
        KhachHang *kh = danhSachKH[i];
        ThongTinKhachHangTop info;
        info.khachHang = kh;
        info.soDonDat = demSoDonDatCuaKhachHang(kh);
        info.tongChiTieu = tinhTongChiTieuCuaKhachHang(kh);
        topKhachHang.push_back(info);
    }

    // Sắp xếp theo tổng chi tiêu (QuickSort O(n log n))
    QuickSort<ThongTinKhachHangTop>::sort(topKhachHang, [](const ThongTinKhachHangTop &a, const ThongTinKhachHangTop &b) {
        return a.tongChiTieu > b.tongChiTieu; // Giảm dần
    });

    // Giữ lại top N
    if (topKhachHang.size() > soLuong)
    {
        MangDong<ThongTinKhachHangTop> temp;
        for (int i = 0; i < soLuong; i++)
        {
            temp.push_back(topKhachHang[i]);
        }
        topKhachHang = temp;
    }
}

int ThongKeKhachHang::demSoDonDatCuaKhachHang(KhachHang *kh)
{
    if (heThong == nullptr || kh == nullptr)
        return 0;

    int count = 0;
    const MangDong<DatSan *> &danhSach = heThong->layDanhSachDatSan();

    for (int i = 0; i < danhSach.size(); i++)
    {
        DatSan *ds = danhSach[i];
        if (ds->getKhachHang() == kh)
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

double ThongKeKhachHang::tinhTongChiTieuCuaKhachHang(KhachHang *kh)
{
    if (heThong == nullptr || kh == nullptr)
        return 0.0;

    double tongChiTieu = 0.0;
    const MangDong<DatSan *> &danhSach = heThong->layDanhSachDatSan();

    for (int i = 0; i < danhSach.size(); i++)
    {
        DatSan *ds = danhSach[i];
        if (ds->getKhachHang() == kh && ds->getTrangThai() == HOAN_THANH)
        {
            NgayGio tgDat = ds->getThoiGianDat();
            NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());

            if (ngayDat >= tuNgay && ngayDat <= denNgay)
            {
                tongChiTieu += ds->getTongTien();
            }
        }
    }

    return tongChiTieu;
}

// ===== NEW METHODS =====

bool ThongKeKhachHang::laDangKyTrongKy(KhachHang *kh)
{
    if (kh == nullptr)
        return false;

    NgayThang ngayDangKy = kh->layNgayDangKy();
    return (ngayDangKy >= tuNgay && ngayDangKy <= denNgay);
}

bool ThongKeKhachHang::laHoatDongTrongKy(KhachHang *kh)
{
    if (heThong == nullptr || kh == nullptr)
        return false;

    const MangDong<DatSan *> &danhSach = heThong->layDanhSachDatSan();
    for (int i = 0; i < danhSach.size(); i++)
    {
        DatSan *ds = danhSach[i];
        if (ds->getKhachHang() == kh)
        {
            NgayGio tgDat = ds->getThoiGianDat();
            NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());

            if (ngayDat >= tuNgay && ngayDat <= denNgay)
            {
                return true;
            }
        }
    }
    return false;
}

void ThongKeKhachHang::tinhThongKeTheoHang()
{
    if (heThong == nullptr)
        return;

    thongKeTheoHang = MangDong<ThongKeHangKhachHang>();

    // Tạo entry cho 5 hạng
    thongKeTheoHang.push_back(ThongKeHangKhachHang(HangKhachHang::THUONG));
    thongKeTheoHang.push_back(ThongKeHangKhachHang(HangKhachHang::DONG));
    thongKeTheoHang.push_back(ThongKeHangKhachHang(HangKhachHang::BAC));
    thongKeTheoHang.push_back(ThongKeHangKhachHang(HangKhachHang::VANG));
    thongKeTheoHang.push_back(ThongKeHangKhachHang(HangKhachHang::KIM_CUONG));

    const MangDong<KhachHang *> &danhSachKH = heThong->layDanhSachKhachHang();
    MangDong<double> tongChiTieuTheoHang;
    for (int i = 0; i < 5; i++)
        tongChiTieuTheoHang.push_back(0.0);

    for (int i = 0; i < danhSachKH.size(); i++)
    {
        KhachHang *kh = danhSachKH[i];
        int idx = (int)kh->layHang();
        if (idx >= 0 && idx < 5)
        {
            thongKeTheoHang[idx].soLuong++;
            tongChiTieuTheoHang[idx] += kh->layTongChiTieu();
        }
    }

    // Tính tỷ lệ và chi tiêu TB
    for (int i = 0; i < 5; i++)
    {
        if (tongSoKhachHang > 0)
        {
            thongKeTheoHang[i].tyLe = (double)thongKeTheoHang[i].soLuong / tongSoKhachHang * 100;
        }
        if (thongKeTheoHang[i].soLuong > 0)
        {
            thongKeTheoHang[i].chiTieuTrungBinh = tongChiTieuTheoHang[i] / thongKeTheoHang[i].soLuong;
        }
    }
}

void ThongKeKhachHang::tinhThongKeTheoThang()
{
    if (heThong == nullptr)
        return;

    thongKeTheoThang = MangDong<ThongKeKhachHangTheoThang>();

    // Xác định các tháng trong khoảng
    int thangBatDau = tuNgay.getThang();
    int namBatDau = tuNgay.getNam();
    int thangKetThuc = denNgay.getThang();
    int namKetThuc = denNgay.getNam();

    int thang = thangBatDau;
    int nam = namBatDau;
    while (nam < namKetThuc || (nam == namKetThuc && thang <= thangKetThuc))
    {
        ThongKeKhachHangTheoThang tk;
        tk.thang = thang;
        tk.nam = nam;
        tk.khachMoi = 0;
        tk.khachQuayLai = 0;
        tk.doanhThu = 0.0;
        thongKeTheoThang.push_back(tk);

        thang++;
        if (thang > 12)
        {
            thang = 1;
            nam++;
        }
    }

    // Đếm khách mới theo tháng đăng ký
    const MangDong<KhachHang *> &danhSachKH = heThong->layDanhSachKhachHang();
    for (int i = 0; i < danhSachKH.size(); i++)
    {
        KhachHang *kh = danhSachKH[i];
        NgayThang ngayDK = kh->layNgayDangKy();

        for (int j = 0; j < thongKeTheoThang.size(); j++)
        {
            if (ngayDK.getThang() == thongKeTheoThang[j].thang &&
                ngayDK.getNam() == thongKeTheoThang[j].nam)
            {
                thongKeTheoThang[j].khachMoi++;
                break;
            }
        }
    }

    // Đếm booking và doanh thu theo tháng
    const MangDong<DatSan *> &danhSach = heThong->layDanhSachDatSan();
    for (int i = 0; i < danhSach.size(); i++)
    {
        DatSan *ds = danhSach[i];
        NgayGio tgDat = ds->getThoiGianDat();

        for (int j = 0; j < thongKeTheoThang.size(); j++)
        {
            if (tgDat.getThang() == thongKeTheoThang[j].thang &&
                tgDat.getNam() == thongKeTheoThang[j].nam)
            {
                if (ds->getTrangThai() == HOAN_THANH)
                {
                    thongKeTheoThang[j].doanhThu += ds->getTongTien();
                }
                break;
            }
        }
    }
}

void ThongKeKhachHang::tinhCustomerInsights()
{
    if (heThong == nullptr)
        return;

    const MangDong<KhachHang *> &danhSachKH = heThong->layDanhSachKhachHang();

    // Tính CLV
    double tongCLV = 0.0;
    for (int i = 0; i < danhSachKH.size(); i++)
    {
        tongCLV += danhSachKH[i]->layTongChiTieu();
    }
    if (tongSoKhachHang > 0)
    {
        customerLifetimeValue = tongCLV / tongSoKhachHang;
    }

    // Tính tần suất đặt sân TB
    int tongSoDon = 0;
    for (int i = 0; i < danhSachKH.size(); i++)
    {
        tongSoDon += danhSachKH[i]->laySoLanDatSan();
    }

    int soThang = tuNgay.tinhKhoangCach(denNgay) / 30;
    if (soThang <= 0)
        soThang = 1;

    if (tongSoKhachHang > 0)
    {
        tanSuatDatSanTB = (double)tongSoDon / tongSoKhachHang / soThang;
    }

    // Đếm khách hàng rời bỏ (không đặt > 90 ngày)
    soKhachHangRoiBo = 0;
    NgayThang homNay = ngayTao;
    NgayThang nguong90Ngay = homNay.addDays(-90);

    const MangDong<DatSan *> &danhSach = heThong->layDanhSachDatSan();
    for (int i = 0; i < danhSachKH.size(); i++)
    {
        KhachHang *kh = danhSachKH[i];
        bool coDatGanDay = false;

        for (int j = 0; j < danhSach.size(); j++)
        {
            if (danhSach[j]->getKhachHang() == kh)
            {
                NgayGio tgDat = danhSach[j]->getThoiGianDat();
                NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());

                if (ngayDat >= nguong90Ngay)
                {
                    coDatGanDay = true;
                    break;
                }
            }
        }

        if (!coDatGanDay && kh->laySoLanDatSan() > 0)
        {
            soKhachHangRoiBo++;
        }
    }
}

void ThongKeKhachHang::tinhSoSanhKyTruoc()
{
    if (heThong == nullptr)
        return;

    // Tính số ngày
    int soNgay = tuNgay.tinhKhoangCach(denNgay);
    if (soNgay <= 0)
        soNgay = 1;

    // Kỳ trước
    NgayThang tuNgayKT = tuNgay.addDays(-soNgay);
    NgayThang denNgayKT = tuNgay.addDays(-1);

    soKhachHangMoiKyTruoc = 0;

    const MangDong<KhachHang *> &danhSachKH = heThong->layDanhSachKhachHang();
    for (int i = 0; i < danhSachKH.size(); i++)
    {
        KhachHang *kh = danhSachKH[i];
        NgayThang ngayDK = kh->layNgayDangKy();

        if (ngayDK >= tuNgayKT && ngayDK <= denNgayKT)
        {
            soKhachHangMoiKyTruoc++;
        }
    }

    // Tính % tăng
    if (soKhachHangMoiKyTruoc > 0)
    {
        phanTramTangKhachMoi = ((double)(soKhachHangMoi - soKhachHangMoiKyTruoc) / soKhachHangMoiKyTruoc) * 100;
    }
    else if (soKhachHangMoi > 0)
    {
        phanTramTangKhachMoi = 100.0;
    }
}

std::string ThongKeKhachHang::layTenHang(HangKhachHang hang) const
{
    switch (hang)
    {
    case HangKhachHang::THUONG:
        return "Thuong";
    case HangKhachHang::DONG:
        return "Dong";
    case HangKhachHang::BAC:
        return "Bac";
    case HangKhachHang::VANG:
        return "Vang";
    case HangKhachHang::KIM_CUONG:
        return "Kim Cuong";
    default:
        return "N/A";
    }
}
