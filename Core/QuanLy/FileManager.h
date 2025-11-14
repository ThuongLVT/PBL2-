#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <string>

#include "../CauTrucDuLieu/MangDong.h"

// Forward declarations
class HeThongQuanLy;

// Lớp quản lý file I/O cho toàn bộ hệ thống
class FileManager
{
private:
    std::string duongDanData;   // Đường dẫn thư mục Data/
    std::string duongDanBackup; // Đường dẫn thư mục backup/

public:
    // Constructor & Destructor
    FileManager();
    FileManager(const std::string &pathData, const std::string &pathBackup);
    ~FileManager();

    // Getters & Setters
    std::string getDuongDanData() const;
    std::string getDuongDanBackup() const;
    void setDuongDanData(const std::string &path);
    void setDuongDanBackup(const std::string &path);

    // ===== LƯU/ĐỌC HỆ THỐNG =====
    bool luuHeThong(HeThongQuanLy *heThong, const std::string &tenFile);
    bool docHeThong(HeThongQuanLy *heThong, const std::string &tenFile);

    // ===== BACKUP & RESTORE =====
    bool taoBackup(const std::string &tenFileGoc);
    bool khoiPhucBackup(const std::string &tenFileBackup);
    MangDong<std::string> layDanhSachBackup() const;
    bool xoaBackup(const std::string &tenFileBackup);
    void xoaTatCaBackup();

    // ===== UTILITY =====
    bool kiemTraFileExists(const std::string &tenFile) const;
    long layKichThuocFile(const std::string &tenFile) const;
    bool xoaFile(const std::string &tenFile) const;
    bool saoChepFile(const std::string &nguon, const std::string &dich) const;

    // ===== XUẤT BÁO CÁO =====
    bool xuatBaoCaoText(const std::string &tenFile, const std::string &noiDung) const;

    // ===== TẠO THƯ MỤC =====
    static bool taoThuMuc(const std::string &duongDan);
};

#endif // FILEMANAGER_H
