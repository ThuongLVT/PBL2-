// San.h - Lop dai dien cho San bong da
// Chua thong tin ve san, gia thue, trang thai

#ifndef SAN_H
#define SAN_H
#include <string>

#include <iostream>

// Loai san bong da
enum class LoaiSan
{
    SAN_5, // San 5 nguoi
    SAN_7  // San 7 nguoi
};

// Trang thai hien tai cua san
enum class TrangThaiSan
{
    HOAT_DONG,       // San dang hoat dong (co the dat)
    NGUNG_HOAT_DONG, // San ngung hoat dong
    BAO_TRI          // Dang bao tri
};

// Khu vuc cua san
enum class KhuVuc
{
    A, // Khu vuc A
    B, // Khu vuc B
    C, // Khu vuc C
    D  // Khu vuc D
};

// Lop dai dien cho mot san bong da
// Thong tin san bao gom: Ma san, ten san, Loai san (5, 7 nguoi),
// Gia thue theo gio, Trang thai hien tai, Ghi chu
class San
{
private:
    std::string maSan;      // Ma san (VD: A501, B702)
    std::string tenSan;     // Ten san
    LoaiSan loaiSan;        // Loai san
    KhuVuc khuVuc;          // Khu vuc san
    double giaThueGio;      // Gia thue moi gio (VND)
    TrangThaiSan trangThai; // Trang thai hien tai
    std::string ghiChu;     // Ghi chu ve san

public:
    // ========== CONSTRUCTORS ==========

    // Constructor mac dinh
    San();

    // Constructor co tham so (maSan: Ma san, tenSan: Ten san, loaiSan: Loai san, khuVuc: Khu vuc san, giaThue: Gia thue moi gio)
    San(const std::string &maSan, const std::string &tenSan,
        LoaiSan loaiSan, KhuVuc khuVuc, double giaThue);

    // Copy constructor (other: Doi tuong San khac)
    San(const San &other);

    // Destructor
    ~San();

    // ========== GETTERS ==========

    // Lay ma san
    std::string layMaSan() const;
    std::string getMaSan() const { return maSan; } // Alias

    // Lay ten san
    std::string layTenSan() const;
    std::string getTenSan() const { return tenSan; } // Alias

    // Lay loai san
    LoaiSan layLoaiSan() const;

    // Lay khu vuc san
    KhuVuc layKhuVuc() const;

    // Lay gia thue/gio
    double layGiaThueGio() const;
    double getGiaThue() const { return giaThueGio; } // Alias

    // Lay trang thai san
    TrangThaiSan layTrangThai() const;

    // Lay ghi chu
    std::string layGhiChu() const;

    // Lay ten loai san (dang chuoi)
    std::string layTenLoaiSan() const;

    // Lay ten trang thai san (dang chuoi)
    std::string layTenTrangThai() const;

    // Lay ten khu vuc san (A, B, C, D)
    std::string layTenKhuVuc() const;

    // ========== SETTERS ==========

    // Dat ten san (ten: Ten san moi)
    void datTenSan(const std::string &ten);

    // Dat gia thue (gia: Gia thue moi)
    void datGiaThueGio(double gia);

    // Dat trang thai san (tt: Trang thai moi)
    void datTrangThai(TrangThaiSan tt);

    // Dat ghi chu (gc: Ghi chu moi)
    void datGhiChu(const std::string &gc);

    // Dat loai san (loai: Loai san moi)
    void datLoaiSan(LoaiSan loai);

    // Dat khu vuc (kv: Khu vuc moi)
    void datKhuVuc(KhuVuc kv);

    // ========== METHODS ==========

    // Kiem tra san co dang hoat dong khong
    bool dangHoatDong() const;

    // Kiem tra san co ngung hoat dong khong
    bool ngungHoatDong() const;

    // Kiem tra san co dang bao tri khong
    bool dangBaoTri() const;

    // Tinh tien thue san theo so gio (soGio: So gio thue)
    double tinhTien(double soGio) const;

    // Hien thi thong tin san
    void hienThiThongTin() const;

    // ========== OPERATORS ==========

    // Toan tu gan (other: Doi tuong San khac)
    San &operator=(const San &other);

    // Toan tu so sanh theo ma san (other: Doi tuong San khac)
    bool operator==(const San &other) const;

    // Toan tu so sanh < theo gia thue (other: Doi tuong San khac)
    bool operator<(const San &other) const;

    // Toan tu xuat ra stream (os: Output stream, s: Doi tuong San)
    friend std::ostream &operator<<(std::ostream &os, const San &s);
};

#endif // SAN_H
