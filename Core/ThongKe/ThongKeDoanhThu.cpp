#include "ThongKeDoanhThu.h"
#include "../QuanLy/HeThongQuanLy.h"
#include "../Models/DatSan.h"
#include <iostream>
#include <cstdio>

using namespace std;

// Constructor mặc định
ThongKeDoanhThu::ThongKeDoanhThu()
    : ThongKe(), tongDoanhThu(0.0), doanhThuTienSan(0.0), doanhThuDichVu(0.0),
      soLuongDonDat(0), soLuongDonHoanThanh(0), soLuongDonHuy(0),
      doanhThuTrungBinh(0.0), heThong(nullptr)
{
    tieuDe = std::string("Bao cao thong ke doanh thu");
}

// Constructor với tham số
ThongKeDoanhThu::ThongKeDoanhThu(const NgayThang &tu, const NgayThang &den, HeThongQuanLy *ht)
    : ThongKe(std::string("Bao cao thong ke doanh thu"), tu, den),
      tongDoanhThu(0.0), doanhThuTienSan(0.0), doanhThuDichVu(0.0),
      soLuongDonDat(0), soLuongDonHoanThanh(0), soLuongDonHuy(0),
      doanhThuTrungBinh(0.0), heThong(ht)
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
        heThong = other.heThong;
    }
    return *this;
}

// Getters
double ThongKeDoanhThu::getTongDoanhThu() const { return tongDoanhThu; }
double ThongKeDoanhThu::getDoanhThuTienSan() const { return doanhThuTienSan; }
double ThongKeDoanhThu::getDoanhThuDichVu() const { return doanhThuDichVu; }
int ThongKeDoanhThu::getSoLuongDonDat() const { return soLuongDonDat; }
int ThongKeDoanhThu::getSoLuongDonHoanThanh() const { return soLuongDonHoanThanh; }
int ThongKeDoanhThu::getSoLuongDonHuy() const { return soLuongDonHuy; }
double ThongKeDoanhThu::getDoanhThuTrungBinh() const { return doanhThuTrungBinh; }

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
                tongDoanhThu += ds->getTongTien();

                // Tính doanh thu tiền sân
                if (ds->getSan() != nullptr)
                {
                    double soGio = ds->getKhungGio().tinhSoGio();
                    doanhThuTienSan += ds->getSan()->getGiaThue() * soGio;
                }

                // Tính doanh thu dịch vụ
                const MangDong<DichVuDat> &dsDichVu = ds->getDanhSachDichVu();
                for (int j = 0; j < dsDichVu.size(); j++)
                {
                    doanhThuDichVu += dsDichVu[j].getThanhTien();
                }
            }
            else if (ds->getTrangThai() == DA_HUY)
            {
                soLuongDonHuy++;
            }
        }
    }

    // Tính doanh thu trung bình
    int soNgay = tuNgay.tinhKhoangCach(denNgay);
    if (soNgay > 0)
    {
        doanhThuTrungBinh = tongDoanhThu / soNgay;
    }
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
