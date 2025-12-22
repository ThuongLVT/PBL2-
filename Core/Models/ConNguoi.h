// ConNguoi.h - Lop co so dai dien cho mot con nguoi
// Chua cac thuoc tinh va phuong thuc co ban cua con nguoi
// Lop co so cho KhachHang, NguoiDung (Admin, NhanVien)

#ifndef CONNGUOI_H
#define CONNGUOI_H
#include <string>

#include <iostream>

// Lop co so dai dien cho con nguoi trong he thong
// Chua: Ho ten, So dien thoai, Dia chi
class ConNguoi
{
protected:
    std::string hoTen;       // Ho va ten day du
    std::string soDienThoai; // So dien thoai lien lac
    std::string diaChi;      // Dia chi noi o

public:
    // ========== CONSTRUCTORS ==========

    // Constructor mac dinh
    ConNguoi();

    // Constructor co tham so (ht: Ho ten, sdt: So dien thoai, dc: Dia chi)
    ConNguoi(const std::string &ht, const std::string &sdt, const std::string &dc);

    // Copy constructor (other: Doi tuong ConNguoi khac)
    ConNguoi(const ConNguoi &other);

    // Destructor ao (de cho phep ke thua dung)
    virtual ~ConNguoi();

    // ========== GETTERS ==========

    // Lay ho ten
    std::string layHoTen() const;
    std::string getHoTen() const { return hoTen; } // Alias

    // Lay so dien thoai
    std::string laySoDienThoai() const;
    std::string getSoDienThoai() const { return soDienThoai; } // Alias

    // Lay dia chi
    std::string layDiaChi() const;

    // ========== SETTERS ==========

    // Dat ho ten (ht: Ho ten moi)
    void datHoTen(const std::string &ht);

    // Dat so dien thoai (sdt: So dien thoai moi)
    void datSoDienThoai(const std::string &sdt);

    // Dat dia chi (dc: Dia chi moi)
    void datDiaChi(const std::string &dc);

    // ========== METHODS ==========

    // Hien thi thong tin con nguoi (phuong thuc ao de lop con co the override)
    virtual void hienThiThongTin() const;

    // ========== OPERATORS ==========

    // Toan tu gan (other: Doi tuong ConNguoi khac)
    ConNguoi &operator=(const ConNguoi &other);

    // Toan tu so sanh bang - so sanh theo ho ten (other: Doi tuong ConNguoi khac)
    bool operator==(const ConNguoi &other) const;

    // Toan tu xuat ra stream (os: Output stream, cn: Doi tuong ConNguoi)
    friend std::ostream &operator<<(std::ostream &os, const ConNguoi &cn);
};

#endif // CONNGUOI_H
