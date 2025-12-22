// Lop quan ly dat san - Tao/Huy/Cap nhat/Tim kiem dat san, Kiem tra trung lich

#ifndef QUANLYDATSAN_H
#define QUANLYDATSAN_H

#include "../Models/DatSan.h"
#include "../Models/KhachHang.h"
#include "../Models/San.h"
#include "../CauTrucDuLieu/MangDong.h"
#include "../CauTrucDuLieu/NgayGio.h"
#include "../Models/KhungGio.h"
#include <string>

class QuanLyDatSan
{
private:
    MangDong<DatSan *> danhSachDatSan;
    int maxBookingId; // ID lon nhat (tranh trung ID)

public:
    // ========== CONSTRUCTORS ==========
    QuanLyDatSan();
    ~QuanLyDatSan();

    // ========== CRUD OPERATIONS ==========
    DatSan *taoDatSan(KhachHang *kh, San *san, const NgayGio &thoiGian, const KhungGio &khung);
    void themDatSanTrucTiep(DatSan *datSan); // For CSV loading - khong tao ID moi
    bool huyDatSan(const std::string &maDatSan);
    bool capNhatTrangThaiDatSan(const std::string &maDatSan, TrangThaiDatSan trangThai);
    DatSan *timDatSan(const std::string &maDatSan);
    const MangDong<DatSan *> &layDanhSachDatSan() const;

    // ========== SEARCH ==========
    MangDong<DatSan *> timDatSanTheoKhachHang(const std::string &maKH);
    MangDong<DatSan *> timDatSanTheoSan(const std::string &maSan);
    MangDong<DatSan *> timDatSanTheoNgay(const NgayThang &ngay);
    MangDong<DatSan *> timDatSanTheoTrangThai(TrangThaiDatSan trangThai);

    // ========== VALIDATION ==========
    bool kiemTraTrungLich(San *san, const NgayGio &thoiGian, const KhungGio &khung);

    // ========== DISPLAY ==========
    void hienThiDanhSachDatSan() const;

    // ========== STATISTICS ==========
    int tongSoDatSan() const;

    // ========== DATA MANAGEMENT ==========
    void xoaTatCa();

    // ========== CSV I/O ==========
    bool loadFromCSV(const std::string &filename, class QuanLyKhachHang *qlKH, class QuanLySan *qlSan, class QuanLyDichVu *qlDV);
    bool saveToCSV(const std::string &filename);
};

#endif // QUANLYDATSAN_H
