/**
 * @file HeThongQuanLyCSV.cpp
 * @brief CSV I/O methods for HeThongQuanLy
 */

#include "HeThongQuanLy.h"
#include "../Utils/CSVHelper.h"
#include <sstream>

using namespace std;

// ========== CUSTOMERS CSV ==========
bool HeThongQuanLy::luuKhachHangCSV(const string &filename)
{
    return quanLyKhachHang->luuCSV(filename);
}

bool HeThongQuanLy::docKhachHangCSV(const string &filename)
{
    return quanLyKhachHang->docCSV(filename);
}

// ========== FIELDS CSV ==========
bool HeThongQuanLy::luuSanCSV(const string &filename)
{
    return quanLySan->saveToCSV();
}

bool HeThongQuanLy::docSanCSV(const string &filename)
{
    return quanLySan->loadFromCSV(filename);
}

// ========== SERVICES CSV ==========
bool HeThongQuanLy::luuDichVuCSV(const string &filename)
{
    return quanLyDichVu->luuDuLieuRaCSV(filename);
}

bool HeThongQuanLy::docDichVuCSV(const string &filename)
{
    return quanLyDichVu->taiDuLieuTuCSV(filename);
}

// ========== BOOKINGS CSV ==========
bool HeThongQuanLy::luuDatSanCSV(const string &filename)
{
    return quanLyDatSan->saveToCSV(filename);
}

bool HeThongQuanLy::docDatSanCSV(const string &filename)
{
    return quanLyDatSan->loadFromCSV(filename, quanLyKhachHang, quanLySan);
}
