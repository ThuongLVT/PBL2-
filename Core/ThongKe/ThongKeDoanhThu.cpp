#include "ThongKeDoanhThu.h"
#include "../QuanLy/HeThongQuanLy.h"
#include "../Models/DatSan.h"
#include "../Models/DonHangDichVu.h"
#include "../Models/DichVuDat.h"
#include <iostream>
#include <cstdio>

using namespace std;

// Constructor mặc định
ThongKeDoanhThu::ThongKeDoanhThu()
    : ThongKe(), tongDoanhThu(0.0), doanhThuTienSan(0.0), doanhThuDichVu(0.0),
      soLuongDonDat(0), soLuongDonHoanThanh(0), soLuongDonHuy(0),
      doanhThuTrungBinh(0.0), doanhThuKyTruoc(0.0), phanTramTangGiam(0.0),
      doanhThuSanKyTruoc(0.0), doanhThuDVKyTruoc(0.0), phanTramTangSan(0.0),
      phanTramTangDV(0.0), doanhThuDoUong(0.0), doanhThuDoAn(0.0),
      doanhThuThietBi(0.0), doanhThuGioVang(0.0), doanhThuGioThuong(0.0),
      heThong(nullptr)
{
    tieuDe = std::string("Bao cao thong ke doanh thu");
}

// Constructor với tham số
ThongKeDoanhThu::ThongKeDoanhThu(const NgayThang &tu, const NgayThang &den, HeThongQuanLy *ht)
    : ThongKe(std::string("Bao cao thong ke doanh thu"), tu, den),
      tongDoanhThu(0.0), doanhThuTienSan(0.0), doanhThuDichVu(0.0),
      soLuongDonDat(0), soLuongDonHoanThanh(0), soLuongDonHuy(0),
      doanhThuTrungBinh(0.0), doanhThuKyTruoc(0.0), phanTramTangGiam(0.0),
      doanhThuSanKyTruoc(0.0), doanhThuDVKyTruoc(0.0), phanTramTangSan(0.0),
      phanTramTangDV(0.0), doanhThuDoUong(0.0), doanhThuDoAn(0.0),
      doanhThuThietBi(0.0), doanhThuGioVang(0.0), doanhThuGioThuong(0.0),
      heThong(ht)
{
    tinhToan();
}

// Copy constructor
ThongKeDoanhThu::ThongKeDoanhThu(const ThongKeDoanhThu &other) : ThongKe(other)
{
    tongDoanhThu = other.tongDoanhThu;
    doanhThuTienSan = other.doanhThuTienSan;
    doanhThuDichVu = other.doanhThuDichVu;
    soLuongDonDat = other.soLuongDonDat;
    soLuongDonHoanThanh = other.soLuongDonHoanThanh;
    soLuongDonHuy = other.soLuongDonHuy;
    doanhThuTrungBinh = other.doanhThuTrungBinh;
    doanhThuKyTruoc = other.doanhThuKyTruoc;
    phanTramTangGiam = other.phanTramTangGiam;
    doanhThuSanKyTruoc = other.doanhThuSanKyTruoc;
    doanhThuDVKyTruoc = other.doanhThuDVKyTruoc;
    phanTramTangSan = other.phanTramTangSan;
    phanTramTangDV = other.phanTramTangDV;
    doanhThuDoUong = other.doanhThuDoUong;
    doanhThuDoAn = other.doanhThuDoAn;
    doanhThuThietBi = other.doanhThuThietBi;
    doanhThuTheoNgay = other.doanhThuTheoNgay;
    doanhThuTheoThang = other.doanhThuTheoThang;
    doanhThuGioVang = other.doanhThuGioVang;
    doanhThuGioThuong = other.doanhThuGioThuong;
    heThong = other.heThong;
}

// Destructor
ThongKeDoanhThu::~ThongKeDoanhThu() {}

// Assignment operator
ThongKeDoanhThu &ThongKeDoanhThu::operator=(const ThongKeDoanhThu &other)
{
    if (this != &other)
    {
        ThongKe::operator=(other);
        tongDoanhThu = other.tongDoanhThu;
        doanhThuTienSan = other.doanhThuTienSan;
        doanhThuDichVu = other.doanhThuDichVu;
        soLuongDonDat = other.soLuongDonDat;
        soLuongDonHoanThanh = other.soLuongDonHoanThanh;
        soLuongDonHuy = other.soLuongDonHuy;
        doanhThuTrungBinh = other.doanhThuTrungBinh;
        doanhThuKyTruoc = other.doanhThuKyTruoc;
        phanTramTangGiam = other.phanTramTangGiam;
        doanhThuSanKyTruoc = other.doanhThuSanKyTruoc;
        doanhThuDVKyTruoc = other.doanhThuDVKyTruoc;
        phanTramTangSan = other.phanTramTangSan;
        phanTramTangDV = other.phanTramTangDV;
        doanhThuDoUong = other.doanhThuDoUong;
        doanhThuDoAn = other.doanhThuDoAn;
        doanhThuThietBi = other.doanhThuThietBi;
        doanhThuTheoNgay = other.doanhThuTheoNgay;
        doanhThuTheoThang = other.doanhThuTheoThang;
        doanhThuGioVang = other.doanhThuGioVang;
        doanhThuGioThuong = other.doanhThuGioThuong;
        heThong = other.heThong;
    }
    return *this;
}

// ===== GETTERS - CƠ BẢN =====
double ThongKeDoanhThu::getTongDoanhThu() const { return tongDoanhThu; }
double ThongKeDoanhThu::getDoanhThuTienSan() const { return doanhThuTienSan; }
double ThongKeDoanhThu::getDoanhThuDichVu() const { return doanhThuDichVu; }
int ThongKeDoanhThu::getSoLuongDonDat() const { return soLuongDonDat; }
int ThongKeDoanhThu::getSoLuongDonHoanThanh() const { return soLuongDonHoanThanh; }
int ThongKeDoanhThu::getSoLuongDonHuy() const { return soLuongDonHuy; }
double ThongKeDoanhThu::getDoanhThuTrungBinh() const { return doanhThuTrungBinh; }

// ===== GETTERS - SO SÁNH =====
double ThongKeDoanhThu::getDoanhThuKyTruoc() const { return doanhThuKyTruoc; }
double ThongKeDoanhThu::getPhanTramTangGiam() const { return phanTramTangGiam; }
double ThongKeDoanhThu::getDoanhThuSanKyTruoc() const { return doanhThuSanKyTruoc; }
double ThongKeDoanhThu::getDoanhThuDVKyTruoc() const { return doanhThuDVKyTruoc; }
double ThongKeDoanhThu::getPhanTramTangSan() const { return phanTramTangSan; }
double ThongKeDoanhThu::getPhanTramTangDV() const { return phanTramTangDV; }

// ===== GETTERS - CHI TIẾT LOẠI DV =====
double ThongKeDoanhThu::getDoanhThuDoUong() const { return doanhThuDoUong; }
double ThongKeDoanhThu::getDoanhThuDoAn() const { return doanhThuDoAn; }
double ThongKeDoanhThu::getDoanhThuThietBi() const { return doanhThuThietBi; }

// ===== GETTERS - THEO THỜI GIAN =====
const MangDong<DoanhThuTheoNgay> &ThongKeDoanhThu::getDoanhThuTheoNgayList() const { return doanhThuTheoNgay; }
const MangDong<DoanhThuTheoThang> &ThongKeDoanhThu::getDoanhThuTheoThangList() const { return doanhThuTheoThang; }

// ===== GETTERS - KHUNG GIỜ =====
double ThongKeDoanhThu::getDoanhThuGioVang() const { return doanhThuGioVang; }
double ThongKeDoanhThu::getDoanhThuGioThuong() const { return doanhThuGioThuong; }

// Methods
void ThongKeDoanhThu::tinhToan()
{
    if (heThong == nullptr)
        return;

    // Reset
    tongDoanhThu = 0.0;
    doanhThuTienSan = 0.0;
    doanhThuDichVu = 0.0;
    soLuongDonDat = 0;
    soLuongDonHoanThanh = 0;
    soLuongDonHuy = 0;
    doanhThuDoUong = 0.0;
    doanhThuDoAn = 0.0;
    doanhThuThietBi = 0.0;
    doanhThuGioVang = 0.0;
    doanhThuGioThuong = 0.0;

    const MangDong<DatSan *> &danhSach = heThong->layDanhSachDatSan();

    for (int i = 0; i < danhSach.size(); i++)
    {
        DatSan *ds = danhSach[i];
        NgayGio tgDat = ds->getThoiGianDat();

        // Kiểm tra trong khoảng thời gian
        NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());
        if (ngayDat >= tuNgay && ngayDat <= denNgay)
        {
            soLuongDonDat++;

            // Chỉ tính doanh thu cho đơn hoàn thành
            if (ds->getTrangThai() == HOAN_THANH)
            {
                soLuongDonHoanThanh++;
                double tongTienBooking = ds->getTongTien();
                tongDoanhThu += tongTienBooking;

                // Tính doanh thu tiền sân
                double tienSan = 0.0;
                if (ds->getSan() != nullptr)
                {
                    double soGio = ds->getKhungGio().tinhSoGio();
                    tienSan = ds->getSan()->getGiaThue() * soGio;
                    doanhThuTienSan += tienSan;
                }

                // Phân loại theo khung giờ (giờ vàng: 18-21h)
                int gioBatDau = ds->getKhungGio().getGioBatDau().getGio();
                if (gioBatDau >= 18 && gioBatDau < 21)
                {
                    doanhThuGioVang += tongTienBooking;
                }
                else
                {
                    doanhThuGioThuong += tongTienBooking;
                }

                // Tính doanh thu dịch vụ và phân loại
                const MangDong<DichVuDat> &dsDichVu = ds->getDanhSachDichVu();
                for (int j = 0; j < dsDichVu.size(); j++)
                {
                    double thanhTien = dsDichVu[j].getThanhTien();
                    doanhThuDichVu += thanhTien;

                    // Phân loại theo loại dịch vụ
                    DichVu *dv = dsDichVu[j].getDichVu();
                    if (dv != nullptr)
                    {
                        switch (dv->layLoaiDichVu())
                        {
                        case LoaiDichVu::DO_UONG:
                            doanhThuDoUong += thanhTien;
                            break;
                        case LoaiDichVu::DO_AN:
                            doanhThuDoAn += thanhTien;
                            break;
                        case LoaiDichVu::THIET_BI:
                            doanhThuThietBi += thanhTien;
                            break;
                        }
                    }
                }
            }
            else if (ds->getTrangThai() == DA_HUY)
            {
                soLuongDonHuy++;
            }
        }
    }

    // Thêm doanh thu từ đơn hàng dịch vụ riêng lẻ
    const MangDong<DonHangDichVu *> &danhSachDon = heThong->layDanhSachDonHangDichVu();
    for (int i = 0; i < danhSachDon.size(); i++)
    {
        DonHangDichVu *don = danhSachDon[i];
        NgayGio ngayTaoDon = don->getNgayTao();
        NgayThang ngayDon(ngayTaoDon.getNgay(), ngayTaoDon.getThang(), ngayTaoDon.getNam());

        if (ngayDon >= tuNgay && ngayDon <= denNgay &&
            don->getTrangThai() == TrangThaiDonHang::HOAN_THANH)
        {
            double thanhTien = don->getThanhTien();
            tongDoanhThu += thanhTien;
            doanhThuDichVu += thanhTien;

            // Phân loại chi tiết
            const MangDong<DichVuDat> &dsDV = don->getDanhSachDichVu();
            for (int j = 0; j < dsDV.size(); j++)
            {
                double tienDV = dsDV[j].getThanhTien();
                DichVu *dv = dsDV[j].getDichVu();
                if (dv != nullptr)
                {
                    switch (dv->layLoaiDichVu())
                    {
                    case LoaiDichVu::DO_UONG:
                        doanhThuDoUong += tienDV;
                        break;
                    case LoaiDichVu::DO_AN:
                        doanhThuDoAn += tienDV;
                        break;
                    case LoaiDichVu::THIET_BI:
                        doanhThuThietBi += tienDV;
                        break;
                    }
                }
            }
        }
    }

    // Tính doanh thu trung bình
    int soNgay = tuNgay.tinhKhoangCach(denNgay);
    if (soNgay > 0)
    {
        doanhThuTrungBinh = tongDoanhThu / soNgay;
    }

    // Tính các thống kê bổ sung
    tinhSoSanhKyTruoc();
    tinhDoanhThuTheoTungNgay();
    tinhDoanhThuTheoTungThang();
}

void ThongKeDoanhThu::hienThi() const
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "  " << tieuDe.c_str() << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Thoi gian: " << tuNgay.toString().c_str()
              << " - " << denNgay.toString().c_str() << std::endl;
    std::cout << "Ngay tao bao cao: " << ngayTao.toString().c_str() << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Tong doanh thu:        " << tongDoanhThu << " VND" << std::endl;
    std::cout << "  - Tu tien san:       " << doanhThuTienSan << " VND" << std::endl;
    std::cout << "  - Tu dich vu:        " << doanhThuDichVu << " VND" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "So luong don dat:      " << soLuongDonDat << std::endl;
    std::cout << "  - Hoan thanh:        " << soLuongDonHoanThanh << std::endl;
    std::cout << "  - Da huy:            " << soLuongDonHuy << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Doanh thu TB/ngay:     " << doanhThuTrungBinh << " VND" << std::endl;
    std::cout << "========================================\n"
              << std::endl;
}

void ThongKeDoanhThu::xuatFile(const std::string &tenFile) const
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
    fprintf(f, "Tong doanh thu:        %.2f VND\n", tongDoanhThu);
    fprintf(f, "  - Tu tien san:       %.2f VND\n", doanhThuTienSan);
    fprintf(f, "  - Tu dich vu:        %.2f VND\n", doanhThuDichVu);
    fprintf(f, "----------------------------------------\n");
    fprintf(f, "So luong don dat:      %d\n", soLuongDonDat);
    fprintf(f, "  - Hoan thanh:        %d\n", soLuongDonHoanThanh);
    fprintf(f, "  - Da huy:            %d\n", soLuongDonHuy);
    fprintf(f, "----------------------------------------\n");
    fprintf(f, "Doanh thu TB/ngay:     %.2f VND\n", doanhThuTrungBinh);
    fprintf(f, "========================================\n");

    fclose(f);
    std::cout << "Da xuat bao cao ra file: " << tenFile.c_str() << std::endl;
}

double ThongKeDoanhThu::tinhDoanhThuTheoNgay(const NgayThang &ngay)
{
    if (heThong == nullptr)
        return 0.0;

    double doanhThu = 0.0;
    const MangDong<DatSan *> &danhSach = heThong->layDanhSachDatSan();

    for (int i = 0; i < danhSach.size(); i++)
    {
        DatSan *ds = danhSach[i];
        NgayGio tgDat = ds->getThoiGianDat();

        if (tgDat.getNgay() == ngay.getNgay() &&
            tgDat.getThang() == ngay.getThang() &&
            tgDat.getNam() == ngay.getNam() &&
            ds->getTrangThai() == HOAN_THANH)
        {
            doanhThu += ds->getTongTien();
        }
    }

    return doanhThu;
}

double ThongKeDoanhThu::tinhDoanhThuTheoThang(int thang, int nam)
{
    if (heThong == nullptr)
        return 0.0;

    double doanhThu = 0.0;
    const MangDong<DatSan *> &danhSach = heThong->layDanhSachDatSan();

    for (int i = 0; i < danhSach.size(); i++)
    {
        DatSan *ds = danhSach[i];
        NgayGio tgDat = ds->getThoiGianDat();

        if (tgDat.getThang() == thang &&
            tgDat.getNam() == nam &&
            ds->getTrangThai() == HOAN_THANH)
        {
            doanhThu += ds->getTongTien();
        }
    }

    return doanhThu;
}

void ThongKeDoanhThu::hienThiBieuDoDoanhThu() const
{
    std::cout << "\n===== BIEU DO DOANH THU =====" << std::endl;
    if (tongDoanhThu > 0)
    {
        std::cout << "Doanh thu tien san: ";
        int barSan = (int)((doanhThuTienSan / tongDoanhThu) * 50);
        for (int i = 0; i < barSan; i++)
            std::cout << "█";
        std::cout << " " << doanhThuTienSan << " VND" << std::endl;

        std::cout << "Doanh thu dich vu:  ";
        int barDV = (int)((doanhThuDichVu / tongDoanhThu) * 50);
        for (int i = 0; i < barDV; i++)
            std::cout << "█";
        std::cout << " " << doanhThuDichVu << " VND" << std::endl;
    }
}

// ===== NEW METHODS =====

void ThongKeDoanhThu::tinhSoSanhKyTruoc()
{
    if (heThong == nullptr)
        return;

    // Tính số ngày trong khoảng hiện tại
    int soNgay = tuNgay.tinhKhoangCach(denNgay);
    if (soNgay <= 0)
        soNgay = 1;

    // Tính khoảng thời gian kỳ trước (cùng số ngày)
    NgayThang tuNgayKyTruoc = tuNgay.addDays(-soNgay);
    NgayThang denNgayKyTruoc = tuNgay.addDays(-1);

    // Reset
    doanhThuKyTruoc = 0.0;
    doanhThuSanKyTruoc = 0.0;
    doanhThuDVKyTruoc = 0.0;

    const MangDong<DatSan *> &danhSach = heThong->layDanhSachDatSan();

    for (int i = 0; i < danhSach.size(); i++)
    {
        DatSan *ds = danhSach[i];
        NgayGio tgDat = ds->getThoiGianDat();
        NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());

        if (ngayDat >= tuNgayKyTruoc && ngayDat <= denNgayKyTruoc)
        {
            if (ds->getTrangThai() == HOAN_THANH)
            {
                doanhThuKyTruoc += ds->getTongTien();

                if (ds->getSan() != nullptr)
                {
                    double soGio = ds->getKhungGio().tinhSoGio();
                    doanhThuSanKyTruoc += ds->getSan()->getGiaThue() * soGio;
                }

                const MangDong<DichVuDat> &dsDichVu = ds->getDanhSachDichVu();
                for (int j = 0; j < dsDichVu.size(); j++)
                {
                    doanhThuDVKyTruoc += dsDichVu[j].getThanhTien();
                }
            }
        }
    }

    // Tính % tăng/giảm
    if (doanhThuKyTruoc > 0)
    {
        phanTramTangGiam = ((tongDoanhThu - doanhThuKyTruoc) / doanhThuKyTruoc) * 100;
    }
    else if (tongDoanhThu > 0)
    {
        phanTramTangGiam = 100.0; // Tăng 100% nếu kỳ trước = 0
    }

    if (doanhThuSanKyTruoc > 0)
    {
        phanTramTangSan = ((doanhThuTienSan - doanhThuSanKyTruoc) / doanhThuSanKyTruoc) * 100;
    }
    else if (doanhThuTienSan > 0)
    {
        phanTramTangSan = 100.0;
    }

    if (doanhThuDVKyTruoc > 0)
    {
        phanTramTangDV = ((doanhThuDichVu - doanhThuDVKyTruoc) / doanhThuDVKyTruoc) * 100;
    }
    else if (doanhThuDichVu > 0)
    {
        phanTramTangDV = 100.0;
    }
}

void ThongKeDoanhThu::tinhDoanhThuTheoTungNgay()
{
    if (heThong == nullptr)
        return;

    doanhThuTheoNgay = MangDong<DoanhThuTheoNgay>();

    // Lấy số ngày trong khoảng
    int soNgay = tuNgay.tinhKhoangCach(denNgay);
    if (soNgay <= 0)
        soNgay = 1;

    // Tạo entry cho từng ngày
    NgayThang ngayHienTai = tuNgay;
    for (int d = 0; d <= soNgay; d++)
    {
        DoanhThuTheoNgay dt;
        dt.ngay = ngayHienTai;
        dt.doanhThu = 0.0;
        dt.doanhThuSan = 0.0;
        dt.doanhThuDichVu = 0.0;
        dt.soBooking = 0;
        doanhThuTheoNgay.push_back(dt);
        ngayHienTai = ngayHienTai.addDays(1);
    }

    // Tính doanh thu từng ngày từ booking
    const MangDong<DatSan *> &danhSach = heThong->layDanhSachDatSan();
    for (int i = 0; i < danhSach.size(); i++)
    {
        DatSan *ds = danhSach[i];
        if (ds->getTrangThai() != HOAN_THANH)
            continue;

        NgayGio tgDat = ds->getThoiGianDat();
        NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());

        if (ngayDat >= tuNgay && ngayDat <= denNgay)
        {
            // Tìm index của ngày
            int idx = tuNgay.tinhKhoangCach(ngayDat);
            if (idx >= 0 && idx < doanhThuTheoNgay.size())
            {
                doanhThuTheoNgay[idx].doanhThu += ds->getTongTien();
                doanhThuTheoNgay[idx].soBooking++;

                if (ds->getSan() != nullptr)
                {
                    double soGio = ds->getKhungGio().tinhSoGio();
                    doanhThuTheoNgay[idx].doanhThuSan += ds->getSan()->getGiaThue() * soGio;
                }

                const MangDong<DichVuDat> &dsDV = ds->getDanhSachDichVu();
                for (int j = 0; j < dsDV.size(); j++)
                {
                    doanhThuTheoNgay[idx].doanhThuDichVu += dsDV[j].getThanhTien();
                }
            }
        }
    }
}

void ThongKeDoanhThu::tinhDoanhThuTheoTungThang()
{
    if (heThong == nullptr)
        return;

    doanhThuTheoThang = MangDong<DoanhThuTheoThang>();

    // Xác định các tháng trong khoảng
    int thangBatDau = tuNgay.getThang();
    int namBatDau = tuNgay.getNam();
    int thangKetThuc = denNgay.getThang();
    int namKetThuc = denNgay.getNam();

    // Tạo entry cho từng tháng
    int thang = thangBatDau;
    int nam = namBatDau;
    while (nam < namKetThuc || (nam == namKetThuc && thang <= thangKetThuc))
    {
        DoanhThuTheoThang dt;
        dt.thang = thang;
        dt.nam = nam;
        dt.doanhThu = 0.0;
        dt.doanhThuSan = 0.0;
        dt.doanhThuDichVu = 0.0;
        dt.soBooking = 0;
        doanhThuTheoThang.push_back(dt);

        thang++;
        if (thang > 12)
        {
            thang = 1;
            nam++;
        }
    }

    // Tính doanh thu từng tháng
    const MangDong<DatSan *> &danhSach = heThong->layDanhSachDatSan();
    for (int i = 0; i < danhSach.size(); i++)
    {
        DatSan *ds = danhSach[i];
        if (ds->getTrangThai() != HOAN_THANH)
            continue;

        NgayGio tgDat = ds->getThoiGianDat();
        NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());

        if (ngayDat >= tuNgay && ngayDat <= denNgay)
        {
            // Tìm index của tháng
            for (int j = 0; j < doanhThuTheoThang.size(); j++)
            {
                if (doanhThuTheoThang[j].thang == tgDat.getThang() &&
                    doanhThuTheoThang[j].nam == tgDat.getNam())
                {
                    doanhThuTheoThang[j].doanhThu += ds->getTongTien();
                    doanhThuTheoThang[j].soBooking++;

                    if (ds->getSan() != nullptr)
                    {
                        double soGio = ds->getKhungGio().tinhSoGio();
                        doanhThuTheoThang[j].doanhThuSan += ds->getSan()->getGiaThue() * soGio;
                    }

                    const MangDong<DichVuDat> &dsDV = ds->getDanhSachDichVu();
                    for (int k = 0; k < dsDV.size(); k++)
                    {
                        doanhThuTheoThang[j].doanhThuDichVu += dsDV[k].getThanhTien();
                    }
                    break;
                }
            }
        }
    }
}

void ThongKeDoanhThu::tinhDoanhThuTheoKhungGio()
{
    // Đã tính trong tinhToan()
}

void ThongKeDoanhThu::tinhDoanhThuChiTietDichVu()
{
    // Đã tính trong tinhToan()
}
