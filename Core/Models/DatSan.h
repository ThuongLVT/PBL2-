#ifndef DATSAN_H
#define DATSAN_H
#include <string>

#include "KhachHang.h"
#include "San.h"
#include "KhungGio.h"
#include "DichVuDat.h"

#include "../CauTrucDuLieu/MangDong.h"
#include "../CauTrucDuLieu/NgayGio.h"

// Enum trạng thái đặt sân (SIMPLIFIED - 3 states only)
enum TrangThaiDatSan
{
    DA_DAT,     // Đã đặt (đã cọc 30%, chờ thanh toán đủ)
    HOAN_THANH, // Hoàn thành (đã thanh toán 100%)
    DA_HUY      // Đã hủy (có ghi lý do)
};

// Enum trạng thái cọc
enum TrangThaiCoc
{
    DA_COC,   // Đã đặt cọc 30% (mặc định khi tạo booking)
    HOAN_COC, // Đã hoàn cọc (nhân viên chọn khi hủy)
    MAT_COC   // Mất cọc (nhân viên chọn khi hủy)
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

    // Deposit fields (Tiền cọc)
    double tienCoc;            // Tiền cọc (30% tổng tiền)
    TrangThaiCoc trangThaiCoc; // Trạng thái cọc
    NgayGio ngayDatCoc;        // Ngày đặt cọc
    NgayGio ngayThanhToan;     // Ngày thanh toán (khi hoàn tất)

    // Cancel fields (Hủy lịch)
    std::string lyDoHuy; // Lý do hủy (bắt buộc khi hủy)
    bool hoanCoc;        // true = hoàn cọc, false = mất cọc

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
    double getTienCoc() const;
    TrangThaiCoc getTrangThaiCoc() const;
    NgayGio getNgayDatCoc() const;
    NgayGio getNgayThanhToan() const;
    std::string getLyDoHuy() const;
    bool isHoanCoc() const;

    // Setters
    void setMaDatSan(const std::string &ma);
    void setKhachHang(KhachHang *kh);
    void setSan(San *s);
    void setThoiGianDat(const NgayGio &tgDat);
    void setKhungGio(const KhungGio &kg);
    void setTrangThai(TrangThaiDatSan tt);
    void setGhiChu(const std::string &gc);
    void setTienCoc(double tc);
    void setTrangThaiCoc(TrangThaiCoc ttc);
    void setNgayDatCoc(const NgayGio &ndc);
    void setNgayThanhToan(const NgayGio &ntt);
    void setLyDoHuy(const std::string &lyDo);
    void setHoanCoc(bool hoan);

    // Methods
    void themDichVu(const DichVuDat &dv);
    void xoaDichVu(int index);
    void tinhTongTien(); // Tính tổng tiền = tiền sân + dịch vụ
    void hienThi() const;
    std::string getTrangThaiText() const;
    std::string getTrangThaiCocText() const;
    void tinhTienCoc();                                     // Tính tiền cọc (30% tổng tiền)
    void huyBooking(bool hoanCoc, const std::string &lyDo); // Hủy booking với lý do

    // Helper methods for pointer resolution
    std::string getMaKhachHang() const; // Get customer ID
    std::string getMaSan() const;       // Get field ID

    // Static methods
    static std::string taoMaDatSan(); // Tạo mã tự động (DS + timestamp)
};

#endif // DATSAN_H
