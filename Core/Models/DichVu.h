// DichVu.h - Lop co so dai dien cho Dich vu
// Lop truu tuong cho cac dich vu kem theo (do uong, thiet bi...)

#ifndef DICHVU_H
#define DICHVU_H
#include <string>

#include <iostream>
#include <fstream>

// Cac loai dich vu
enum class LoaiDichVu
{
    DO_UONG, // Do uong (nuoc, tra, ca phe...)
    DO_AN,   // Do an (mi, snack...)
    THIET_BI // Thiet bi (ao, giay, bong...)
};

// Lop co so dai dien cho dich vu
// Dich vu bao gom: Ma dich vu, Ten dich vu, Don gia, Loai dich vu, Mo ta
class DichVu
{
protected:
    std::string maDichVu;  // Ma dich vu (VD: DV001)
    std::string tenDichVu; // Ten dich vu
    double donGia;         // Don gia
    LoaiDichVu loaiDichVu; // Loai dich vu
    std::string moTa;      // Mo ta chi tiet
    bool conHang;          // Trang thai con hang
    std::string donVi;     // Don vi (Lon, Chai, Cai, Doi, Bo...)
    int soLuongBan;        // So luong da ban
    int soLuongTon;        // So luong ton kho
    std::string hinhAnh;   // Duong dan file hinh anh

public:
    // ========== CONSTRUCTORS ==========

    // Constructor mac dinh
    DichVu();

    // Constructor co tham so (maDV: Ma dich vu, tenDV: Ten dich vu, gia: Don gia, loai: Loai dich vu)
    DichVu(const std::string &maDV, const std::string &tenDV,
           double gia, LoaiDichVu loai);

    // Copy constructor (other: Doi tuong DichVu khac)
    DichVu(const DichVu &other);

    // Destructor ao
    virtual ~DichVu();

    // ========== GETTERS ==========

    // Lay ma dich vu
    std::string layMaDichVu() const;

    // Lay ten dich vu
    std::string layTenDichVu() const;

    // Lay don gia
    double layDonGia() const;

    // Lay loai dich vu
    LoaiDichVu layLoaiDichVu() const;

    // Lay mo ta
    std::string layMoTa() const;

    // Kiem tra con hang
    bool coConHang() const;

    // Lay ten loai dich vu (dang chuoi)
    std::string layTenLoaiDichVu() const;

    // Lay don vi
    std::string layDonVi() const;

    // Lay so luong da ban
    int laySoLuongBan() const;

    // Lay so luong ton kho
    int laySoLuongTon() const;

    // Lay duong dan hinh anh
    std::string layHinhAnh() const;

    // ========== SETTERS ==========

    // Dat ten dich vu (ten: Ten dich vu moi)
    void datTenDichVu(const std::string &ten);

    // Dat don gia (gia: Don gia moi)
    void datDonGia(double gia);

    // Dat mo ta (mt: Mo ta moi)
    void datMoTa(const std::string &mt);

    // Dat trang thai con hang (ch: Trang thai con hang)
    void datConHang(bool ch);

    // Dat don vi (dv: Don vi moi)
    void datDonVi(const std::string &dv);

    // Dat so luong da ban (sl: So luong da ban)
    void datSoLuongBan(int sl);

    // Dat so luong ton kho (sl: So luong ton kho)
    void datSoLuongTon(int sl);

    // Dat duong dan hinh anh (ha: Duong dan hinh anh)
    void datHinhAnh(const std::string &ha);

    // ========== METHODS ==========

    // Tinh tien dich vu theo so luong (soLuong: So luong)
    virtual double tinhTien(int soLuong) const;

    // Hien thi thong tin dich vu (virtual)
    virtual void hienThiThongTin() const;

    // ========== OPERATORS ==========

    // Toan tu gan (other: Doi tuong DichVu khac)
    DichVu &operator=(const DichVu &other);

    // Toan tu so sanh theo ma dich vu (other: Doi tuong DichVu khac)
    bool operator==(const DichVu &other) const;

    // Toan tu so sanh < theo don gia (other: Doi tuong DichVu khac)
    bool operator<(const DichVu &other) const;

    // Toan tu xuat ra stream (os: Output stream, dv: Doi tuong DichVu)
    friend std::ostream &operator<<(std::ostream &os, const DichVu &dv);
};

#endif // DICHVU_H
