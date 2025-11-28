#include "ThongKeDichVu.h"
#include "../QuanLy/HeThongQuanLy.h"
#include "../Models/DatSan.h"
#include "../Models/DichVu.h"
#include "../Models/DichVuDat.h"
#include "../Models/DonHangDichVu.h"
#include <iostream>
#include <cstdio>

using namespace std;

// ========== CONSTRUCTORS ==========

ThongKeDichVu::ThongKeDichVu()
    : ThongKe(), tongSoLuongBan(0), tongDoanhThuDichVu(0.0), tongSoDonDichVu(0),
      soDichVuKemBooking(0), doanhThuDoUong(0.0), doanhThuDoAn(0.0),
      doanhThuThietBi(0.0), soSanPhamSapHet(0), soSanPhamHetHang(0),
      heThong(nullptr)
{
    tieuDe = std::string("Bao cao thong ke Dich vu");
}

ThongKeDichVu::ThongKeDichVu(const NgayThang &tu, const NgayThang &den, HeThongQuanLy *ht)
    : ThongKe(std::string("Bao cao thong ke Dich vu"), tu, den),
      tongSoLuongBan(0), tongDoanhThuDichVu(0.0), tongSoDonDichVu(0),
      soDichVuKemBooking(0), doanhThuDoUong(0.0), doanhThuDoAn(0.0),
      doanhThuThietBi(0.0), soSanPhamSapHet(0), soSanPhamHetHang(0),
      heThong(ht)
{
    tinhToan();
}

ThongKeDichVu::ThongKeDichVu(const ThongKeDichVu &other) : ThongKe(other)
{
    tongSoLuongBan = other.tongSoLuongBan;
    tongDoanhThuDichVu = other.tongDoanhThuDichVu;
    tongSoDonDichVu = other.tongSoDonDichVu;
    soDichVuKemBooking = other.soDichVuKemBooking;
    thongKeTheoLoai = other.thongKeTheoLoai;
    doanhThuDoUong = other.doanhThuDoUong;
    doanhThuDoAn = other.doanhThuDoAn;
    doanhThuThietBi = other.doanhThuThietBi;
    topDichVu = other.topDichVu;
    thongKeTonKho = other.thongKeTonKho;
    soSanPhamSapHet = other.soSanPhamSapHet;
    soSanPhamHetHang = other.soSanPhamHetHang;
    topDichVuKemBooking = other.topDichVuKemBooking;
    heThong = other.heThong;
}

ThongKeDichVu::~ThongKeDichVu() {}

// ========== OPERATORS ==========

ThongKeDichVu &ThongKeDichVu::operator=(const ThongKeDichVu &other)
{
    if (this != &other)
    {
        ThongKe::operator=(other);
        tongSoLuongBan = other.tongSoLuongBan;
        tongDoanhThuDichVu = other.tongDoanhThuDichVu;
        tongSoDonDichVu = other.tongSoDonDichVu;
        soDichVuKemBooking = other.soDichVuKemBooking;
        thongKeTheoLoai = other.thongKeTheoLoai;
        doanhThuDoUong = other.doanhThuDoUong;
        doanhThuDoAn = other.doanhThuDoAn;
        doanhThuThietBi = other.doanhThuThietBi;
        topDichVu = other.topDichVu;
        thongKeTonKho = other.thongKeTonKho;
        soSanPhamSapHet = other.soSanPhamSapHet;
        soSanPhamHetHang = other.soSanPhamHetHang;
        topDichVuKemBooking = other.topDichVuKemBooking;
        heThong = other.heThong;
    }
    return *this;
}

// ========== GETTERS ==========

int ThongKeDichVu::getTongSoLuongBan() const { return tongSoLuongBan; }
double ThongKeDichVu::getTongDoanhThuDichVu() const { return tongDoanhThuDichVu; }
int ThongKeDichVu::getTongSoDonDichVu() const { return tongSoDonDichVu; }
int ThongKeDichVu::getSoDichVuKemBooking() const { return soDichVuKemBooking; }

const MangDong<ThongKeLoaiDichVu> &ThongKeDichVu::getThongKeTheoLoai() const { return thongKeTheoLoai; }
double ThongKeDichVu::getDoanhThuDoUong() const { return doanhThuDoUong; }
double ThongKeDichVu::getDoanhThuDoAn() const { return doanhThuDoAn; }
double ThongKeDichVu::getDoanhThuThietBi() const { return doanhThuThietBi; }

const MangDong<ThongTinDichVuTop> &ThongKeDichVu::getTopDichVu() const { return topDichVu; }

const MangDong<ThongKeTonKho> &ThongKeDichVu::getThongKeTonKho() const { return thongKeTonKho; }
int ThongKeDichVu::getSoSanPhamSapHet() const { return soSanPhamSapHet; }
int ThongKeDichVu::getSoSanPhamHetHang() const { return soSanPhamHetHang; }

const MangDong<ThongTinDichVuTop> &ThongKeDichVu::getTopDichVuKemBooking() const { return topDichVuKemBooking; }

// ========== MAIN CALCULATION ==========

void ThongKeDichVu::tinhToan()
{
    if (heThong == nullptr)
        return;

    // Reset tất cả metrics
    tongSoLuongBan = 0;
    tongDoanhThuDichVu = 0.0;
    tongSoDonDichVu = 0;
    soDichVuKemBooking = 0;
    doanhThuDoUong = 0.0;
    doanhThuDoAn = 0.0;
    doanhThuThietBi = 0.0;

    // Thống kê từ đơn hàng dịch vụ riêng lẻ
    const MangDong<DonHangDichVu *> &danhSachDon = heThong->layDanhSachDonHangDichVu();
    for (int i = 0; i < danhSachDon.size(); i++)
    {
        DonHangDichVu *don = danhSachDon[i];
        NgayGio ngayTaoDon = don->getNgayTao();
        NgayThang ngayDon(ngayTaoDon.getNgay(), ngayTaoDon.getThang(), ngayTaoDon.getNam());

        if (ngayDon >= tuNgay && ngayDon <= denNgay &&
            don->getTrangThai() == TrangThaiDonHang::HOAN_THANH)
        {
            tongSoDonDichVu++;
            tongDoanhThuDichVu += don->getThanhTien();

            const MangDong<DichVuDat> &dsDV = don->getDanhSachDichVu();
            for (int j = 0; j < dsDV.size(); j++)
            {
                tongSoLuongBan += dsDV[j].getSoLuong();

                // Phân loại
                DichVu *dv = dsDV[j].getDichVu();
                if (dv != nullptr)
                {
                    double thanhTien = dsDV[j].getThanhTien();
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
    }

    // Thống kê dịch vụ kèm booking
    const MangDong<DatSan *> &danhSachDatSan = heThong->layDanhSachDatSan();
    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        DatSan *ds = danhSachDatSan[i];
        NgayGio tgDat = ds->getThoiGianDat();
        NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());

        if (ngayDat >= tuNgay && ngayDat <= denNgay &&
            ds->getTrangThai() == HOAN_THANH)
        {
            const MangDong<DichVuDat> &dsDV = ds->getDanhSachDichVu();
            for (int j = 0; j < dsDV.size(); j++)
            {
                soDichVuKemBooking += dsDV[j].getSoLuong();
                tongSoLuongBan += dsDV[j].getSoLuong();

                DichVu *dv = dsDV[j].getDichVu();
                if (dv != nullptr)
                {
                    double thanhTien = dsDV[j].getThanhTien();
                    tongDoanhThuDichVu += thanhTien;

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
    }

    // Tính các thống kê chi tiết
    tinhThongKeTheoLoai();
    tinhTopDichVu(10);
    tinhThongKeTonKho();
    tinhTopDichVuKemBooking(5);
}

// ========== SPECIFIC METHODS ==========

void ThongKeDichVu::tinhThongKeTheoLoai()
{
    thongKeTheoLoai = MangDong<ThongKeLoaiDichVu>();

    // Đồ uống
    ThongKeLoaiDichVu tkDoUong(LoaiDichVu::DO_UONG);
    tkDoUong.doanhThu = doanhThuDoUong;
    if (tongDoanhThuDichVu > 0)
    {
        tkDoUong.tyLe = doanhThuDoUong / tongDoanhThuDichVu * 100;
    }
    thongKeTheoLoai.push_back(tkDoUong);

    // Đồ ăn
    ThongKeLoaiDichVu tkDoAn(LoaiDichVu::DO_AN);
    tkDoAn.doanhThu = doanhThuDoAn;
    if (tongDoanhThuDichVu > 0)
    {
        tkDoAn.tyLe = doanhThuDoAn / tongDoanhThuDichVu * 100;
    }
    thongKeTheoLoai.push_back(tkDoAn);

    // Thiết bị
    ThongKeLoaiDichVu tkThietBi(LoaiDichVu::THIET_BI);
    tkThietBi.doanhThu = doanhThuThietBi;
    if (tongDoanhThuDichVu > 0)
    {
        tkThietBi.tyLe = doanhThuThietBi / tongDoanhThuDichVu * 100;
    }
    thongKeTheoLoai.push_back(tkThietBi);
}

void ThongKeDichVu::tinhTopDichVu(int soLuong)
{
    if (heThong == nullptr)
        return;

    topDichVu = MangDong<ThongTinDichVuTop>();
    const MangDong<DichVu *> &danhSachDV = heThong->layDanhSachDichVu();

    // Tạo thông tin cho từng dịch vụ
    for (int i = 0; i < danhSachDV.size(); i++)
    {
        DichVu *dv = danhSachDV[i];
        ThongTinDichVuTop info;
        info.dichVu = dv;
        info.soLuongBan = laySoLuongBanCuaDichVu(dv);
        info.doanhThu = tinhDoanhThuDichVu(dv);
        topDichVu.push_back(info);
    }

    // Sắp xếp theo số lượng bán (giảm dần)
    for (int i = 0; i < topDichVu.size() - 1; i++)
    {
        for (int j = 0; j < topDichVu.size() - i - 1; j++)
        {
            if (topDichVu[j].soLuongBan < topDichVu[j + 1].soLuongBan)
            {
                ThongTinDichVuTop temp = topDichVu[j];
                topDichVu[j] = topDichVu[j + 1];
                topDichVu[j + 1] = temp;
            }
        }
    }

    // Giữ lại top N
    if (topDichVu.size() > soLuong)
    {
        MangDong<ThongTinDichVuTop> temp;
        for (int i = 0; i < soLuong; i++)
        {
            temp.push_back(topDichVu[i]);
        }
        topDichVu = temp;
    }
}

int ThongKeDichVu::laySoLuongBanCuaDichVu(DichVu *dv)
{
    if (heThong == nullptr || dv == nullptr)
        return 0;

    int soLuong = 0;

    // Từ đơn hàng dịch vụ
    const MangDong<DonHangDichVu *> &danhSachDon = heThong->layDanhSachDonHangDichVu();
    for (int i = 0; i < danhSachDon.size(); i++)
    {
        DonHangDichVu *don = danhSachDon[i];
        NgayGio ngayTaoDon = don->getNgayTao();
        NgayThang ngayDon(ngayTaoDon.getNgay(), ngayTaoDon.getThang(), ngayTaoDon.getNam());

        if (ngayDon >= tuNgay && ngayDon <= denNgay &&
            don->getTrangThai() == TrangThaiDonHang::HOAN_THANH)
        {
            const MangDong<DichVuDat> &dsDV = don->getDanhSachDichVu();
            for (int j = 0; j < dsDV.size(); j++)
            {
                if (dsDV[j].getDichVu() == dv)
                {
                    soLuong += dsDV[j].getSoLuong();
                }
            }
        }
    }

    // Từ booking có dịch vụ kèm
    const MangDong<DatSan *> &danhSachDatSan = heThong->layDanhSachDatSan();
    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        DatSan *ds = danhSachDatSan[i];
        NgayGio tgDat = ds->getThoiGianDat();
        NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());

        if (ngayDat >= tuNgay && ngayDat <= denNgay &&
            ds->getTrangThai() == HOAN_THANH)
        {
            const MangDong<DichVuDat> &dsDV = ds->getDanhSachDichVu();
            for (int j = 0; j < dsDV.size(); j++)
            {
                if (dsDV[j].getDichVu() == dv)
                {
                    soLuong += dsDV[j].getSoLuong();
                }
            }
        }
    }

    return soLuong;
}

double ThongKeDichVu::tinhDoanhThuDichVu(DichVu *dv)
{
    if (heThong == nullptr || dv == nullptr)
        return 0.0;

    double doanhThu = 0.0;

    // Từ đơn hàng dịch vụ
    const MangDong<DonHangDichVu *> &danhSachDon = heThong->layDanhSachDonHangDichVu();
    for (int i = 0; i < danhSachDon.size(); i++)
    {
        DonHangDichVu *don = danhSachDon[i];
        NgayGio ngayTaoDon = don->getNgayTao();
        NgayThang ngayDon(ngayTaoDon.getNgay(), ngayTaoDon.getThang(), ngayTaoDon.getNam());

        if (ngayDon >= tuNgay && ngayDon <= denNgay &&
            don->getTrangThai() == TrangThaiDonHang::HOAN_THANH)
        {
            const MangDong<DichVuDat> &dsDV = don->getDanhSachDichVu();
            for (int j = 0; j < dsDV.size(); j++)
            {
                if (dsDV[j].getDichVu() == dv)
                {
                    doanhThu += dsDV[j].getThanhTien();
                }
            }
        }
    }

    // Từ booking có dịch vụ kèm
    const MangDong<DatSan *> &danhSachDatSan = heThong->layDanhSachDatSan();
    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        DatSan *ds = danhSachDatSan[i];
        NgayGio tgDat = ds->getThoiGianDat();
        NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());

        if (ngayDat >= tuNgay && ngayDat <= denNgay &&
            ds->getTrangThai() == HOAN_THANH)
        {
            const MangDong<DichVuDat> &dsDV = ds->getDanhSachDichVu();
            for (int j = 0; j < dsDV.size(); j++)
            {
                if (dsDV[j].getDichVu() == dv)
                {
                    doanhThu += dsDV[j].getThanhTien();
                }
            }
        }
    }

    return doanhThu;
}

void ThongKeDichVu::tinhThongKeTonKho()
{
    if (heThong == nullptr)
        return;

    thongKeTonKho = MangDong<ThongKeTonKho>();
    soSanPhamSapHet = 0;
    soSanPhamHetHang = 0;

    const MangDong<DichVu *> &danhSachDV = heThong->layDanhSachDichVu();

    for (int i = 0; i < danhSachDV.size(); i++)
    {
        DichVu *dv = danhSachDV[i];
        ThongKeTonKho info;
        info.dichVu = dv;
        info.soLuongTon = dv->laySoLuongTon();
        info.soLuongBan = dv->laySoLuongBan();

        // Tính tỷ lệ quay vòng
        int tongSL = info.soLuongTon + info.soLuongBan;
        if (tongSL > 0)
        {
            info.tyLeQuayVong = (double)info.soLuongBan / tongSL * 100;
        }

        // Kiểm tra sắp hết / hết hàng
        if (info.soLuongTon == 0)
        {
            soSanPhamHetHang++;
        }
        else if (info.soLuongTon < 10)
        {
            info.sapHet = true;
            soSanPhamSapHet++;
        }

        thongKeTonKho.push_back(info);
    }
}

void ThongKeDichVu::tinhTopDichVuKemBooking(int soLuong)
{
    if (heThong == nullptr)
        return;

    topDichVuKemBooking = MangDong<ThongTinDichVuTop>();
    const MangDong<DichVu *> &danhSachDV = heThong->layDanhSachDichVu();

    // Tạo thông tin cho từng dịch vụ
    for (int i = 0; i < danhSachDV.size(); i++)
    {
        DichVu *dv = danhSachDV[i];
        ThongTinDichVuTop info;
        info.dichVu = dv;
        info.soLuongBan = laySoLuongKemBooking(dv);
        topDichVuKemBooking.push_back(info);
    }

    // Sắp xếp theo số lượng bán (giảm dần)
    for (int i = 0; i < topDichVuKemBooking.size() - 1; i++)
    {
        for (int j = 0; j < topDichVuKemBooking.size() - i - 1; j++)
        {
            if (topDichVuKemBooking[j].soLuongBan < topDichVuKemBooking[j + 1].soLuongBan)
            {
                ThongTinDichVuTop temp = topDichVuKemBooking[j];
                topDichVuKemBooking[j] = topDichVuKemBooking[j + 1];
                topDichVuKemBooking[j + 1] = temp;
            }
        }
    }

    // Giữ lại top N
    if (topDichVuKemBooking.size() > soLuong)
    {
        MangDong<ThongTinDichVuTop> temp;
        for (int i = 0; i < soLuong; i++)
        {
            temp.push_back(topDichVuKemBooking[i]);
        }
        topDichVuKemBooking = temp;
    }
}

int ThongKeDichVu::laySoLuongKemBooking(DichVu *dv)
{
    if (heThong == nullptr || dv == nullptr)
        return 0;

    int soLuong = 0;

    const MangDong<DatSan *> &danhSachDatSan = heThong->layDanhSachDatSan();
    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        DatSan *ds = danhSachDatSan[i];
        NgayGio tgDat = ds->getThoiGianDat();
        NgayThang ngayDat(tgDat.getNgay(), tgDat.getThang(), tgDat.getNam());

        if (ngayDat >= tuNgay && ngayDat <= denNgay &&
            ds->getTrangThai() == HOAN_THANH)
        {
            const MangDong<DichVuDat> &dsDV = ds->getDanhSachDichVu();
            for (int j = 0; j < dsDV.size(); j++)
            {
                if (dsDV[j].getDichVu() == dv)
                {
                    soLuong += dsDV[j].getSoLuong();
                }
            }
        }
    }

    return soLuong;
}

std::string ThongKeDichVu::layTenLoai(LoaiDichVu loai) const
{
    switch (loai)
    {
    case LoaiDichVu::DO_UONG:
        return "Do uong";
    case LoaiDichVu::DO_AN:
        return "Do an";
    case LoaiDichVu::THIET_BI:
        return "Thiet bi";
    default:
        return "Khac";
    }
}

// ========== DISPLAY METHODS ==========

void ThongKeDichVu::hienThi() const
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "  " << tieuDe.c_str() << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Thoi gian: " << tuNgay.toString().c_str()
              << " - " << denNgay.toString().c_str() << std::endl;
    std::cout << "Ngay tao bao cao: " << ngayTao.toString().c_str() << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    std::cout << "TONG QUAN:" << std::endl;
    std::cout << "  Tong so luong ban:       " << tongSoLuongBan << std::endl;
    std::cout << "  Tong doanh thu:          " << tongDoanhThuDichVu << " VND" << std::endl;
    std::cout << "  So don dich vu rieng:    " << tongSoDonDichVu << std::endl;
    std::cout << "  So DV kem booking:       " << soDichVuKemBooking << std::endl;

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "THEO LOAI DICH VU:" << std::endl;
    std::cout << "  Do uong:    " << doanhThuDoUong << " VND" << std::endl;
    std::cout << "  Do an:      " << doanhThuDoAn << " VND" << std::endl;
    std::cout << "  Thiet bi:   " << doanhThuThietBi << " VND" << std::endl;

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "TON KHO:" << std::endl;
    std::cout << "  San pham sap het (<10):  " << soSanPhamSapHet << std::endl;
    std::cout << "  San pham het hang:       " << soSanPhamHetHang << std::endl;

    if (topDichVu.size() > 0)
    {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "TOP DICH VU BAN CHAY:" << std::endl;
        for (int i = 0; i < topDichVu.size(); i++)
        {
            const ThongTinDichVuTop &info = topDichVu[i];
            if (info.dichVu != nullptr)
            {
                std::cout << "  " << (i + 1) << ". " << info.dichVu->layTenDichVu().c_str()
                          << " - " << info.soLuongBan << " sp"
                          << " - " << info.doanhThu << " VND" << std::endl;
            }
        }
    }

    std::cout << "========================================\n"
              << std::endl;
}

void ThongKeDichVu::xuatFile(const std::string &tenFile) const
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

    fprintf(f, "TONG QUAN:\n");
    fprintf(f, "  Tong so luong ban:       %d\n", tongSoLuongBan);
    fprintf(f, "  Tong doanh thu:          %.0f VND\n", tongDoanhThuDichVu);
    fprintf(f, "  So don dich vu rieng:    %d\n", tongSoDonDichVu);
    fprintf(f, "  So DV kem booking:       %d\n", soDichVuKemBooking);

    fprintf(f, "----------------------------------------\n");
    fprintf(f, "THEO LOAI DICH VU:\n");
    fprintf(f, "  Do uong:    %.0f VND\n", doanhThuDoUong);
    fprintf(f, "  Do an:      %.0f VND\n", doanhThuDoAn);
    fprintf(f, "  Thiet bi:   %.0f VND\n", doanhThuThietBi);

    fprintf(f, "----------------------------------------\n");
    fprintf(f, "TON KHO:\n");
    fprintf(f, "  San pham sap het (<10):  %d\n", soSanPhamSapHet);
    fprintf(f, "  San pham het hang:       %d\n", soSanPhamHetHang);

    if (topDichVu.size() > 0)
    {
        fprintf(f, "----------------------------------------\n");
        fprintf(f, "TOP DICH VU BAN CHAY:\n");
        for (int i = 0; i < topDichVu.size(); i++)
        {
            const ThongTinDichVuTop &info = topDichVu[i];
            if (info.dichVu != nullptr)
            {
                fprintf(f, "  %d. %s - %d sp - %.0f VND\n",
                        i + 1, info.dichVu->layTenDichVu().c_str(),
                        info.soLuongBan, info.doanhThu);
            }
        }
    }

    fprintf(f, "========================================\n");

    fclose(f);
    std::cout << "Da xuat bao cao ra file: " << tenFile.c_str() << std::endl;
}
