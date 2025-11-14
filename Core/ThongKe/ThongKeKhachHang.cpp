#include "ThongKeKhachHang.h"
#include "../QuanLy/HeThongQuanLy.h"
#include "../Models/KhachHang.h"
#include "../Models/DatSan.h"
#include <iostream>
#include <cstdio>

using namespace std;

// Constructor mặc định
ThongKeKhachHang::ThongKeKhachHang()
    : ThongKe(), tongSoKhachHang(0), soKhachHangMoi(0), soKhachHangQuayLai(0),
      chiTieuTrungBinh(0.0), heThong(nullptr)
{
    tieuDe = std::string("Bao cao thong ke khach hang");
}

// Constructor với tham số
ThongKeKhachHang::ThongKeKhachHang(const NgayThang &tu, const NgayThang &den, HeThongQuanLy *ht)
    : ThongKe(std::string("Bao cao thong ke khach hang"), tu, den),
      tongSoKhachHang(0), soKhachHangMoi(0), soKhachHangQuayLai(0),
      chiTieuTrungBinh(0.0), heThong(ht)
{
    tinhToan();
}

// Copy constructor
ThongKeKhachHang::ThongKeKhachHang(const ThongKeKhachHang &other) : ThongKe(other)
{
    tongSoKhachHang = other.tongSoKhachHang;
    soKhachHangMoi = other.soKhachHangMoi;
    soKhachHangQuayLai = other.soKhachHangQuayLai;
    chiTieuTrungBinh = other.chiTieuTrungBinh;
    topKhachHang = other.topKhachHang;
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
        chiTieuTrungBinh = other.chiTieuTrungBinh;
        topKhachHang = other.topKhachHang;
        heThong = other.heThong;
    }
    return *this;
}

// Getters
int ThongKeKhachHang::getTongSoKhachHang() const { return tongSoKhachHang; }
int ThongKeKhachHang::getSoKhachHangMoi() const { return soKhachHangMoi; }
int ThongKeKhachHang::getSoKhachHangQuayLai() const { return soKhachHangQuayLai; }
double ThongKeKhachHang::getChiTieuTrungBinh() const { return chiTieuTrungBinh; }
const MangDong<ThongTinKhachHangTop> &ThongKeKhachHang::getTopKhachHang() const { return topKhachHang; }

// Methods
void ThongKeKhachHang::tinhToan()
{
    if (heThong == nullptr)
        return;

    // Reset
    tongSoKhachHang = 0;
    soKhachHangMoi = 0;
    soKhachHangQuayLai = 0;
    chiTieuTrungBinh = 0.0;

    const MangDong<KhachHang *> &danhSachKH = heThong->layDanhSachKhachHang();
    tongSoKhachHang = danhSachKH.size();

    double tongChiTieu = 0.0;

    for (int i = 0; i < danhSachKH.size(); i++)
    {
        KhachHang *kh = danhSachKH[i];
        int soDonDat = demSoDonDatCuaKhachHang(kh);

        if (soDonDat == 1)
        {
            soKhachHangMoi++;
        }
        else if (soDonDat > 1)
        {
            soKhachHangQuayLai++;
        }

        double chiTieu = tinhTongChiTieuCuaKhachHang(kh);
        tongChiTieu += chiTieu;
    }

    if (tongSoKhachHang > 0)
    {
        chiTieuTrungBinh = tongChiTieu / tongSoKhachHang;
    }

    // Tính top khách hàng
    tinhTopKhachHang(10);
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

    // Sắp xếp theo tổng chi tiêu (Bubble sort đơn giản)
    for (int i = 0; i < topKhachHang.size() - 1; i++)
    {
        for (int j = 0; j < topKhachHang.size() - i - 1; j++)
        {
            if (topKhachHang[j].tongChiTieu < topKhachHang[j + 1].tongChiTieu)
            {
                ThongTinKhachHangTop temp = topKhachHang[j];
                topKhachHang[j] = topKhachHang[j + 1];
                topKhachHang[j + 1] = temp;
            }
        }
    }

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
