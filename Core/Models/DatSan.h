#ifndef DATSAN_H
#define DATSAN_H
#include <string>

#include "KhachHang.h"
#include "San.h"
#include "KhungGio.h"
#include "DichVuDat.h"

#include "../CauTrucDuLieu/MangDong.h"
#include "../CauTrucDuLieu/NgayGio.h"

// Enum trạng thái đặt sân
enum TrangThaiDatSan
{
    CHO_XAC_NHAN,
    DA_XAC_NHAN,
    DANG_SU_DUNG,
    HOAN_THANH,
    DA_HUY
};

// Lớp đại diện cho một đơn đặt sân
class DatSan
{
private:
    std::string maDatSan;               // Mã đơn đặt (tự sinh)
    KhachHang *khachHang;               // Khách hàng đặt
    San *san;                           // Sân được đặt
    NgayGio thoiGianDat;                // Thời gian đặt sân
    KhungGio khungGio;                  // Khung giờ (giờ bắt đầu + kết thúc)
    MangDong<DichVuDat> danhSachDichVu; // Danh sách dịch vụ kèm theo
    double tongTien;                    // Tổng tiền (tiền sân + dịch vụ)
    TrangThaiDatSan trangThai;          // Trạng thái
    NgayGio ngayTao;                    // Ngày tạo đơn
    std::string ghiChu;                 // Ghi chú

public:
    // Constructor & Destructor
    DatSan();
    DatSan(const std::string &ma, KhachHang *kh, San *s, const NgayGio &tgDat, const KhungGio &kg);
    DatSan(const DatSan &other);
    ~DatSan();

    // Operators
    DatSan &operator=(const DatSan &other);
    bool operator==(const DatSan &other) const;

    // Getters
    std::string getMaDatSan() const;
    KhachHang *getKhachHang() const;
    San *getSan() const;
    NgayGio getThoiGianDat() const;
    KhungGio getKhungGio() const;
    const MangDong<DichVuDat> &getDanhSachDichVu() const;
    double getTongTien() const;
    TrangThaiDatSan getTrangThai() const;
    NgayGio getNgayTao() const;
    std::string getGhiChu() const;

    // Setters
    void setMaDatSan(const std::string &ma);
    void setKhachHang(KhachHang *kh);
    void setSan(San *s);
    void setThoiGianDat(const NgayGio &tgDat);
    void setKhungGio(const KhungGio &kg);
    void setTrangThai(TrangThaiDatSan tt);
    void setGhiChu(const std::string &gc);

    // Methods
    void themDichVu(const DichVuDat &dv);
    void xoaDichVu(int index);
    void tinhTongTien(); // Tính tổng tiền = tiền sân + dịch vụ
    void hienThi() const;
    std::string getTrangThaiText() const;

    // File I/O
    void ghiFile(FILE *f) const;
    void docFile(FILE *f);

    // Helper methods for pointer resolution
    std::string getMaKhachHang() const; // Get customer ID
    std::string getMaSan() const;       // Get field ID

    // Static methods
    static std::string taoMaDatSan(); // Tạo mã tự động (DS + timestamp)
};

#endif // DATSAN_H
