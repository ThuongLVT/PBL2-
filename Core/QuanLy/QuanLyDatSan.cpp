/**
 * @file QuanLyDatSan.cpp
 * @brief Triển khai các phương thức của lớp QuanLyDatSan
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#include "QuanLyDatSan.h"
#include "QuanLyKhachHang.h"
#include "QuanLySan.h"
#include "QuanLyDichVu.h"
#include "../Models/DichVu.h"
#include "../Models/DichVuDat.h"
#include "../Utils/CSVHelper.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

QuanLyDatSan::QuanLyDatSan() : maxBookingId(0) {}

QuanLyDatSan::~QuanLyDatSan()
{
    xoaTatCa();
}

DatSan *QuanLyDatSan::taoDatSan(KhachHang *kh, San *san, const NgayGio &thoiGian, const KhungGio &khung)
{
    if (kh == nullptr || san == nullptr)
    {
        return nullptr;
    }

    // Kiểm tra trùng lịch
    if (kiemTraTrungLich(san, thoiGian, khung))
    {
        cout << "San da co nguoi dat trong khung gio nay!" << endl;
        return nullptr;
    }

    // Tạo mã đặt sân tự động với format BK001, BK002, etc.
    maxBookingId++;
    char maDatSan[10];
    sprintf(maDatSan, "BK%03d", maxBookingId);

    DatSan *datSan = new DatSan(string(maDatSan), kh, san, thoiGian, khung);
    danhSachDatSan.push_back(datSan);

    return datSan;
}

void QuanLyDatSan::themDatSanTrucTiep(DatSan *datSan)
{
    // Add booking directly without generating new ID (for CSV loading)
    if (datSan != nullptr)
    {
        danhSachDatSan.push_back(datSan);

        // Update maxBookingId to prevent ID reuse
        string maDS = datSan->getMaDatSan();
        if (maDS.length() > 2 && maDS.substr(0, 2) == "BK")
        {
            int id = atoi(maDS.substr(2).c_str());
            if (id > maxBookingId)
            {
                maxBookingId = id;
            }
        }
    }
}

bool QuanLyDatSan::huyDatSan(const string &maDatSan)
{
    DatSan *datSan = timDatSan(maDatSan);
    if (datSan == nullptr)
    {
        return false;
    }

    datSan->setTrangThai(TrangThaiDatSan::DA_HUY);
    return true;
}

bool QuanLyDatSan::capNhatTrangThaiDatSan(const string &maDatSan, TrangThaiDatSan trangThai)
{
    DatSan *datSan = timDatSan(maDatSan);
    if (datSan == nullptr)
    {
        return false;
    }

    datSan->setTrangThai(trangThai);
    return true;
}

DatSan *QuanLyDatSan::timDatSan(const string &maDatSan)
{
    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        if (danhSachDatSan[i]->getMaDatSan() == maDatSan)
        {
            return danhSachDatSan[i];
        }
    }
    return nullptr;
}

const MangDong<DatSan *> &QuanLyDatSan::layDanhSachDatSan() const
{
    return danhSachDatSan;
}

MangDong<DatSan *> QuanLyDatSan::timDatSanTheoKhachHang(const string &maKH)
{
    MangDong<DatSan *> ketQua;
    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        if (danhSachDatSan[i]->getKhachHang()->getMaNguoiDung() == maKH)
        {
            ketQua.push_back(danhSachDatSan[i]);
        }
    }
    return ketQua;
}

MangDong<DatSan *> QuanLyDatSan::timDatSanTheoSan(const string &maSan)
{
    MangDong<DatSan *> ketQua;
    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        if (danhSachDatSan[i]->getSan()->layMaSan() == maSan)
        {
            ketQua.push_back(danhSachDatSan[i]);
        }
    }
    return ketQua;
}

MangDong<DatSan *> QuanLyDatSan::timDatSanTheoNgay(const NgayThang &ngay)
{
    MangDong<DatSan *> ketQua;
    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        NgayGio thoiGian = danhSachDatSan[i]->getThoiGianDat();
        if (thoiGian.getNgay() == ngay.getNgay() &&
            thoiGian.getThang() == ngay.getThang() &&
            thoiGian.getNam() == ngay.getNam())
        {
            ketQua.push_back(danhSachDatSan[i]);
        }
    }
    return ketQua;
}

MangDong<DatSan *> QuanLyDatSan::timDatSanTheoTrangThai(TrangThaiDatSan trangThai)
{
    MangDong<DatSan *> ketQua;
    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        if (danhSachDatSan[i]->getTrangThai() == trangThai)
        {
            ketQua.push_back(danhSachDatSan[i]);
        }
    }
    return ketQua;
}

bool QuanLyDatSan::kiemTraTrungLich(San *san, const NgayGio &thoiGian, const KhungGio &khung)
{
    if (san == nullptr)
    {
        return false;
    }

    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        DatSan *ds = danhSachDatSan[i];

        // Chỉ kiểm tra đặt sân cùng sân và chưa hủy/hoàn thành
        if (ds->getSan()->layMaSan() != san->layMaSan())
            continue;

        if (ds->getTrangThai() == TrangThaiDatSan::DA_HUY ||
            ds->getTrangThai() == TrangThaiDatSan::HOAN_THANH)
            continue;

        // Kiểm tra trùng ngày
        NgayGio tgDat = ds->getThoiGianDat();
        if (tgDat.getNgay() != thoiGian.getNgay() ||
            tgDat.getThang() != thoiGian.getThang() ||
            tgDat.getNam() != thoiGian.getNam())
            continue;

        // Kiểm tra trùng khung giờ
        KhungGio kgDat = ds->getKhungGio();
        if (khung.layGioBatDau() == kgDat.layGioBatDau() &&
            khung.layGioKetThuc() == kgDat.layGioKetThuc())
        {
            return true; // Trùng lịch
        }

        // Kiểm tra overlap (khung giờ mới bắt đầu trong khoảng khung giờ đã đặt)
        if (khung.kiemTraTrung(kgDat))
        {
            return true; // Trùng lịch
        }
    }

    return false; // Không trùng
}

void QuanLyDatSan::hienThiDanhSachDatSan() const
{
    cout << "\n===== DANH SACH DAT SAN =====" << endl;
    cout << "Tong so: " << danhSachDatSan.size() << endl;
    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        cout << "\n[" << (i + 1) << "] ";
        danhSachDatSan[i]->hienThi();
    }
}

int QuanLyDatSan::tongSoDatSan() const
{
    return danhSachDatSan.size();
}

// ========== DATA MANAGEMENT ==========

void QuanLyDatSan::xoaTatCa()
{
    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        delete danhSachDatSan[i];
    }
    danhSachDatSan = MangDong<DatSan *>();
}

// ========== CSV I/O ==========

bool QuanLyDatSan::loadFromCSV(const std::string &filename, QuanLyKhachHang *qlKH, QuanLySan *qlSan, QuanLyDichVu *qlDV)
{
    if (qlKH == nullptr || qlSan == nullptr)
    {
        cerr << "QuanLyKhachHang or QuanLySan is null!" << endl;
        return false;
    }

    MangDong<MangDong<string>> rows = CSVHelper::readCSV(filename, true);

    if (rows.isEmpty())
    {
        cout << "No booking data found in CSV: " << filename << endl;
        return true; // Not an error, just empty file
    }

    for (int i = 0; i < rows.size(); i++)
    {
        const MangDong<string> &row = rows[i];

        // CSV Format: MaDatSan,MaKhachHang,MaSan,NgayDat,GioBatDau,GioKetThuc,TongTien,TienCoc,TrangThai,GhiChu,NgayThanhToan,DichVu:SL
        if (row.size() < 10)
        {
            cerr << "Invalid CSV row at line " << (i + 2) << ": insufficient columns" << endl;
            continue;
        }

        try
        {
            string maDatSan = row[0];
            string maKH = row[1];
            string maSan = row[2];
            string ngayDatStr = row[3];    // DD/MM/YYYY HH:MM
            string gioBatDauStr = row[4];  // HH:MM
            string gioKetThucStr = row[5]; // HH:MM
            // double tongTien = stod(row[6]);
            // double tienCoc = stod(row[7]);
            string trangThaiStr = row[8];
            string ghiChu = row[9];

            // Find customer and field
            KhachHang *kh = qlKH->timKhachHang(maKH);
            San *san = qlSan->timSan(maSan);

            if (kh == nullptr || san == nullptr)
            {
                cerr << "Cannot resolve customer " << maKH << " or field " << maSan << endl;
                continue;
            }

            // Parse date time: DD/MM/YYYY (Time is in KhungGio)
            int day, month, year;
            char sep;
            stringstream ss(ngayDatStr);
            ss >> day >> sep >> month >> sep >> year;
            // Set time to 00:00 for the date object
            NgayGio ngayDat(day, month, year, 0, 0);

            // Parse time: HH:MM
            int gioBatDauH, gioBatDauM, gioKetThucH, gioKetThucM;
            stringstream ss1(gioBatDauStr);
            ss1 >> gioBatDauH >> sep >> gioBatDauM;
            stringstream ss2(gioKetThucStr);
            ss2 >> gioKetThucH >> sep >> gioKetThucM;
            ThoiGian gioBatDau(gioBatDauH, gioBatDauM);
            ThoiGian gioKetThuc(gioKetThucH, gioKetThucM);
            KhungGio khungGio(gioBatDau, gioKetThuc);

            // Create DatSan object
            DatSan *datSan = new DatSan(maDatSan, kh, san, ngayDat, khungGio);
            datSan->setGhiChu(ghiChu);

            // Set trang thai
            if (trangThaiStr == "DA_DAT")
                datSan->setTrangThai(TrangThaiDatSan::DA_DAT);
            else if (trangThaiStr == "HOAN_THANH")
                datSan->setTrangThai(TrangThaiDatSan::HOAN_THANH);
            else if (trangThaiStr == "DA_HUY")
                datSan->setTrangThai(TrangThaiDatSan::DA_HUY);

            // Parse NgayThanhToan if available (Column 10) - Format: DD/MM/YYYY HH:MM
            if (row.size() > 10 && !row[10].empty())
            {
                string ngayTTStr = row[10];
                int ttDay, ttMonth, ttYear, ttHour = 0, ttMin = 0;
                char sep1, sep2;
                stringstream ssTT(ngayTTStr);
                ssTT >> ttDay >> sep1 >> ttMonth >> sep1 >> ttYear;
                // Check if time is included
                if (ssTT.peek() == ' ')
                {
                    ssTT >> ttHour >> sep2 >> ttMin;
                }
                NgayGio ngayThanhToan(ttDay, ttMonth, ttYear, ttHour, ttMin);
                datSan->setNgayThanhToan(ngayThanhToan);
            }

            // Parse DichVu if available (Column 11) - Format: MaDV:SoLuong;MaDV:SoLuong;...
            if (row.size() > 11 && qlDV != nullptr)
            {
                string dichVuStr = row[11];
                if (!dichVuStr.empty())
                {
                    stringstream ssDV(dichVuStr);
                    string item;
                    while (getline(ssDV, item, ';'))
                    {
                        size_t colonPos = item.find(':');
                        if (colonPos != string::npos)
                        {
                            string maDV = item.substr(0, colonPos);
                            int soLuong = stoi(item.substr(colonPos + 1));
                            DichVu *dv = qlDV->timDichVu(maDV);
                            if (dv)
                            {
                                DichVuDat dvd(dv, soLuong);
                                datSan->themDichVu(dvd);
                            }
                        }
                    }
                }
            }

            // Don't auto-save when loading from CSV
            themDatSanTrucTiep(datSan);
        }
        catch (const exception &e)
        {
            cerr << "Error parsing CSV row " << (i + 2) << ": " << e.what() << endl;
            continue;
        }
    }

    cout << "Loaded " << danhSachDatSan.size() << " bookings from CSV: " << filename << endl;

    return true;
}

bool QuanLyDatSan::saveToCSV(const std::string &filename)
{
    MangDong<string> headers;
    headers.push_back("MaDatSan");
    headers.push_back("MaKhachHang");
    headers.push_back("MaSan");
    headers.push_back("NgayDat");
    headers.push_back("GioBatDau");
    headers.push_back("GioKetThuc");
    headers.push_back("TongTien");
    headers.push_back("TienCoc");
    headers.push_back("TrangThai");
    headers.push_back("GhiChu");
    headers.push_back("NgayThanhToan");
    headers.push_back("DichVu:SL");

    MangDong<MangDong<string>> rows;

    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        DatSan *ds = danhSachDatSan[i];
        MangDong<string> row;

        row.push_back(ds->getMaDatSan());
        row.push_back(ds->getMaKhachHang());
        row.push_back(ds->getMaSan());

        // NgayDat: DD/MM/YYYY (Only Date)
        NgayGio ngayDat = ds->getThoiGianDat();
        stringstream ss;
        ss << setfill('0') << setw(2) << ngayDat.getNgay() << "/"
           << setfill('0') << setw(2) << ngayDat.getThang() << "/"
           << ngayDat.getNam();
        row.push_back(ss.str());

        // GioBatDau: HH:MM
        KhungGio khungGio = ds->getKhungGio();
        ThoiGian gioBatDau = khungGio.layGioBatDau();
        stringstream ss1;
        ss1 << setfill('0') << setw(2) << gioBatDau.getGio() << ":"
            << setfill('0') << setw(2) << gioBatDau.getPhut();
        row.push_back(ss1.str());

        // GioKetThuc: HH:MM
        ThoiGian gioKetThuc = khungGio.layGioKetThuc();
        stringstream ss2;
        ss2 << setfill('0') << setw(2) << gioKetThuc.getGio() << ":"
            << setfill('0') << setw(2) << gioKetThuc.getPhut();
        row.push_back(ss2.str());

        // TongTien
        row.push_back(to_string(static_cast<long long>(ds->getTongTien())));

        // TienCoc
        row.push_back(to_string(static_cast<long long>(ds->getTienCoc())));

        // TrangThai
        string trangThai;
        switch (ds->getTrangThai())
        {
        case TrangThaiDatSan::DA_DAT:
            trangThai = "DA_DAT";
            break;
        case TrangThaiDatSan::HOAN_THANH:
            trangThai = "HOAN_THANH";
            break;
        case TrangThaiDatSan::DA_HUY:
            trangThai = "DA_HUY";
            break;
        }
        row.push_back(trangThai);

        // GhiChu
        row.push_back(ds->getGhiChu());

        // NgayThanhToan: DD/MM/YYYY HH:MM (empty if not paid)
        NgayGio ngayTT = ds->getNgayThanhToan();
        if (ngayTT.getNam() > 0) // Valid date
        {
            stringstream ssTT;
            ssTT << setfill('0') << setw(2) << ngayTT.getNgay() << "/"
                 << setfill('0') << setw(2) << ngayTT.getThang() << "/"
                 << ngayTT.getNam() << " "
                 << setfill('0') << setw(2) << ngayTT.getGio() << ":"
                 << setfill('0') << setw(2) << ngayTT.getPhut();
            row.push_back(ssTT.str());
        }
        else
        {
            row.push_back("");
        }

        // DichVu:SL - Format: MaDV:SoLuong;MaDV:SoLuong;...
        const MangDong<DichVuDat> &services = ds->getDanhSachDichVu();
        stringstream ssDV;
        for (int j = 0; j < services.size(); ++j)
        {
            DichVuDat dv = services[j];
            if (dv.getDichVu())
            {
                if (j > 0)
                    ssDV << ";";
                ssDV << dv.getDichVu()->layMaDichVu() << ":" << dv.getSoLuong();
            }
        }
        row.push_back(ssDV.str());

        rows.push_back(row);
    }

    bool success = CSVHelper::writeCSV(filename, headers, rows);
    if (success)
    {
        cout << "Saved " << danhSachDatSan.size() << " bookings to CSV: " << filename << endl;
    }
    return success;
}
