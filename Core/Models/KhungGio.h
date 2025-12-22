// KhungGio.h - Lop dai dien cho Khung gio dat san
// Quan ly thoi gian bat dau va ket thuc cua mot booking

#ifndef KHUNGGIO_H
#define KHUNGGIO_H
#include <string>

#include "../CauTrucDuLieu/NgayGio.h"
#include <iostream>

// Lop dai dien cho mot khung gio dat san
// Khung gio bao gom: Gio bat dau, Gio ket thuc, So gio thue
class KhungGio
{
private:
    ThoiGian gioBatDau;  // Gio bat dau
    ThoiGian gioKetThuc; // Gio ket thuc

public:
    // ========== CONSTRUCTORS ==========

    // Constructor mac dinh
    KhungGio();

    // Constructor co tham so (bd: Gio bat dau, kt: Gio ket thuc)
    KhungGio(const ThoiGian &bd, const ThoiGian &kt);

    // Constructor voi so gio, phut (gioBD: Gio bat dau, phutBD: Phut bat dau, gioKT: Gio ket thuc, phutKT: Phut ket thuc)
    KhungGio(int gioBD, int phutBD, int gioKT, int phutKT);

    // Copy constructor (other: Doi tuong KhungGio khac)
    KhungGio(const KhungGio &other);

    // Destructor
    ~KhungGio();

    // ========== GETTERS ==========

    // Lay gio bat dau
    ThoiGian layGioBatDau() const;
    ThoiGian getGioBatDau() const { return gioBatDau; } // Alias

    // Lay gio ket thuc
    ThoiGian layGioKetThuc() const;
    ThoiGian getGioKetThuc() const { return gioKetThuc; } // Alias

    // ========== SETTERS ==========

    // Dat gio bat dau (bd: Gio bat dau moi)
    void datGioBatDau(const ThoiGian &bd);

    // Dat gio ket thuc (kt: Gio ket thuc moi)
    void datGioKetThuc(const ThoiGian &kt);

    // ========== METHODS ==========

    // Tinh so gio thue dang thap phan (VD: 1.5 gio = 1 gio 30 phut)
    double tinhSoGio() const;

    // Kiem tra khung gio co hop le khong (gio bat dau < gio ket thuc)
    bool hopLe() const;

    // Kiem tra trung lap voi khung gio khac (other: Khung gio khac)
    bool kiemTraTrung(const KhungGio &other) const;

    // Hien thi thong tin khung gio
    void hienThiThongTin() const;

    // ========== OPERATORS ==========

    // Toan tu gan (other: Doi tuong KhungGio khac)
    KhungGio &operator=(const KhungGio &other);

    // Toan tu so sanh bang (other: Doi tuong KhungGio khac)
    bool operator==(const KhungGio &other) const;

    // Toan tu so sanh < theo gio bat dau (other: Doi tuong KhungGio khac)
    bool operator<(const KhungGio &other) const;

    // Toan tu xuat ra stream (os: Output stream, kg: Doi tuong KhungGio)
    friend std::ostream &operator<<(std::ostream &os, const KhungGio &kg);

    };

#endif // KHUNGGIO_H
