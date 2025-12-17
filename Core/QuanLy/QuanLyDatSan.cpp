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

bool QuanLyDatSan::ghiFile(ofstream &file) const
{
    if (!file.is_open())
        return false;

    int soLuong = danhSachDatSan.size();
    file.write(reinterpret_cast<const char *>(&soLuong), sizeof(soLuong));

    // Skip FILE* writing for now - DatSan uses FILE* not ofstream
    // TODO: Fix serialization

    return file.good();
}

bool QuanLyDatSan::docFile(ifstream &file)
{
    if (!file.is_open())
        return false;

    xoaTatCa();

    int soLuong;
    file.read(reinterpret_cast<char *>(&soLuong), sizeof(soLuong));

    // Skip FILE* reading for now - DatSan uses FILE* not ifstream
    // TODO: Fix deserialization

    return file.good();
}

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

    vector<vector<string>> rows = CSVHelper::readCSV(filename, true);

    if (rows.empty())
    {
        cout << "No booking data found in CSV: " << filename << endl;
        return true; // Not an error, just empty file
    }

    for (size_t i = 0; i < rows.size(); i++)
    {
        const auto &row = rows[i];

        // CSV Format: MaDatSan,MaKhachHang,MaSan,NgayDat,GioBatDau,GioKetThuc,TongTien,TienCoc,TrangThai,TrangThaiCoc,GhiChu
        if (row.size() < 11)
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
            string trangThaiCocStr = row[9];
            string ghiChu = row[10];

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

            // Set trang thai coc
            if (trangThaiCocStr == "DA_COC")
                datSan->setTrangThaiCoc(TrangThaiCoc::DA_COC);
            else if (trangThaiCocStr == "HOAN_COC")
                datSan->setTrangThaiCoc(TrangThaiCoc::HOAN_COC);
            else if (trangThaiCocStr == "MAT_COC")
                datSan->setTrangThaiCoc(TrangThaiCoc::MAT_COC);

            // Parse NgayThanhToan if available (Column 11)
            if (row.size() > 11)
            {
                string ngayThanhToanStr = row[11];
                if (!ngayThanhToanStr.empty())
                {
                    // Format: DD/MM/YYYY HH:MM:SS
                    int d, m, y, h = 0, min = 0, sec = 0;
                    char s;
                    stringstream ssNTT(ngayThanhToanStr);
                    ssNTT >> d >> s >> m >> s >> y;

                    // Try to read time if available
                    if (ssNTT >> h >> s >> min >> s >> sec)
                    {
                        // Successfully read time
                    }
                    else
                    {
                        // Reset stream or just use default 00:00:00
                    }

                    datSan->setNgayThanhToan(NgayGio(d, m, y, h, min, sec));
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

    // Load services for bookings
    if (qlDV != nullptr)
    {
        string serviceFilename = filename;
        size_t pos = serviceFilename.find("datsan.csv");
        if (pos != string::npos)
        {
            serviceFilename.replace(pos, 10, "datsan_dichvu.csv");
        }
        else
        {
            // Try to append _dichvu if not standard name
            if (serviceFilename.length() > 4 && serviceFilename.substr(serviceFilename.length() - 4) == ".csv")
                serviceFilename.insert(serviceFilename.length() - 4, "_dichvu");
            else
                serviceFilename += "_dichvu.csv";
        }

        vector<vector<string>> serviceRows = CSVHelper::readCSV(serviceFilename);
        if (!serviceRows.empty())
        {
            int count = 0;
            for (const auto &row : serviceRows)
            {
                if (row.size() < 3)
                    continue;
                string maDatSan = row[0];
                string maDichVu = row[1];
                int soLuong = 0;
                try
                {
                    soLuong = stoi(row[2]);
                }
                catch (...)
                {
                    continue;
                }

                DatSan *ds = timDatSan(maDatSan);
                DichVu *dv = qlDV->timDichVu(maDichVu);

                if (ds && dv)
                {
                    DichVuDat dvd(dv, soLuong);
                    ds->themDichVu(dvd);
                    count++;
                }
            }
            cout << "Loaded " << count << " service items for bookings from " << serviceFilename << endl;
        }
    }

    return true;
}

bool QuanLyDatSan::saveToCSV(const std::string &filename)
{
    vector<string> headers = {"MaDatSan", "MaKhachHang", "MaSan", "NgayDat", "GioBatDau", "GioKetThuc", "TongTien", "TienCoc", "TrangThai", "TrangThaiCoc", "GhiChu", "NgayThanhToan"};
    vector<vector<string>> rows;

    for (int i = 0; i < danhSachDatSan.size(); i++)
    {
        DatSan *ds = danhSachDatSan[i];
        vector<string> row;

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

        // TrangThaiCoc
        string trangThaiCoc;
        switch (ds->getTrangThaiCoc())
        {
        case TrangThaiCoc::DA_COC:
            trangThaiCoc = "DA_COC";
            break;
        case TrangThaiCoc::HOAN_COC:
            trangThaiCoc = "HOAN_COC";
            break;
        case TrangThaiCoc::MAT_COC:
            trangThaiCoc = "MAT_COC";
            break;
        }
        row.push_back(trangThaiCoc);

        // GhiChu
        row.push_back(ds->getGhiChu());

        // NgayThanhToan
        NgayGio ntt = ds->getNgayThanhToan();
        if (ntt.getNam() > 0)
        { // Valid date
            // Use toString() which returns "DD/MM/YYYY HH:MM:SS"
            row.push_back(ntt.toString());
        }
        else
        {
            row.push_back("");
        }

        rows.push_back(row);
    }

    bool success = CSVHelper::writeCSV(filename, headers, rows);
    if (success)
    {
        cout << "Saved " << danhSachDatSan.size() << " bookings to CSV: " << filename << endl;

        // Save services
        string serviceFilename = filename;
        size_t pos = serviceFilename.find("datsan.csv");
        if (pos != string::npos)
        {
            serviceFilename.replace(pos, 10, "datsan_dichvu.csv");
        }
        else
        {
            if (serviceFilename.length() > 4 && serviceFilename.substr(serviceFilename.length() - 4) == ".csv")
                serviceFilename.insert(serviceFilename.length() - 4, "_dichvu");
            else
                serviceFilename += "_dichvu.csv";
        }

        vector<string> serviceHeaders = {"MaDatSan", "MaDichVu", "SoLuong", "DonGia", "ThanhTien"};
        vector<vector<string>> serviceRows;

        for (int i = 0; i < danhSachDatSan.size(); i++)
        {
            DatSan *ds = danhSachDatSan[i];
            const MangDong<DichVuDat> &services = ds->getDanhSachDichVu();
            for (int j = 0; j < services.size(); ++j)
            {
                DichVuDat dv = services[j];
                if (dv.getDichVu())
                {
                    vector<string> row;
                    row.push_back(ds->getMaDatSan());
                    row.push_back(dv.getDichVu()->layMaDichVu());
                    row.push_back(to_string(dv.getSoLuong()));
                    row.push_back(to_string(static_cast<long long>(dv.getDichVu()->layDonGia())));
                    row.push_back(to_string(static_cast<long long>(dv.getThanhTien())));
                    serviceRows.push_back(row);
                }
            }
        }
        CSVHelper::writeCSV(serviceFilename, serviceHeaders, serviceRows);
    }
    return success;
}
