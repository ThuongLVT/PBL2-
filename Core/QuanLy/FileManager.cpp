#include "FileManager.h"
#include "HeThongQuanLy.h"
#include <iostream>
#include <cstdio>
#include <ctime>
#include <direct.h> // For _mkdir on Windows
#include <sys/stat.h>

using namespace std;

// Constructor mặc định
FileManager::FileManager()
    : duongDanData("Data/"), duongDanBackup("Data/backup/")
{
    // Tạo thư mục nếu chưa tồn tại
    taoThuMuc(duongDanData);
    taoThuMuc(duongDanBackup);
}

// Constructor với tham số
FileManager::FileManager(const std::string &pathData, const std::string &pathBackup)
    : duongDanData(pathData), duongDanBackup(pathBackup)
{
    taoThuMuc(duongDanData);
    taoThuMuc(duongDanBackup);
}

// Destructor
FileManager::~FileManager() {}

// Getters & Setters
std::string FileManager::getDuongDanData() const { return duongDanData; }
std::string FileManager::getDuongDanBackup() const { return duongDanBackup; }
void FileManager::setDuongDanData(const std::string &path) { duongDanData = path; }
void FileManager::setDuongDanBackup(const std::string &path) { duongDanBackup = path; }

// ===== LƯU/ĐỌC HỆ THỐNG =====
bool FileManager::luuHeThong(HeThongQuanLy *heThong, const std::string &tenFile)
{
    if (heThong == nullptr)
        return false;

    std::string duongDanDayDu = duongDanData + tenFile;

    std::cout << "Dang luu he thong vao file: " << duongDanDayDu.c_str() << std::endl;

    bool ketQua = heThong->luuHeThong(duongDanDayDu);

    if (ketQua)
    {
        std::cout << "Luu he thong thanh cong!" << std::endl;
    }
    else
    {
        std::cout << "Luu he thong that bai!" << std::endl;
    }

    return ketQua;
}

bool FileManager::docHeThong(HeThongQuanLy *heThong, const std::string &tenFile)
{
    if (heThong == nullptr)
        return false;

    std::string duongDanDayDu = duongDanData + tenFile;

    if (!kiemTraFileExists(duongDanDayDu))
    {
        std::cout << "File khong ton tai: " << duongDanDayDu.c_str() << std::endl;
        return false;
    }

    std::cout << "Dang doc he thong tu file: " << duongDanDayDu.c_str() << std::endl;

    bool ketQua = heThong->docHeThong(duongDanDayDu);

    if (ketQua)
    {
        std::cout << "Doc he thong thanh cong!" << std::endl;
    }
    else
    {
        std::cout << "Doc he thong that bai!" << std::endl;
    }

    return ketQua;
}

// ===== BACKUP & RESTORE =====
bool FileManager::taoBackup(const std::string &tenFileGoc)
{
    std::string nguon = duongDanData + tenFileGoc;

    if (!kiemTraFileExists(nguon))
    {
        std::cout << "File goc khong ton tai!" << std::endl;
        return false;
    }

    // Tạo tên file backup với timestamp
    time_t now = time(0);
    struct tm *timeinfo = localtime(&now);
    char buffer[100];
    sprintf(buffer, "%s_backup_%04d%02d%02d_%02d%02d%02d.dat",
            tenFileGoc.c_str(),
            timeinfo->tm_year + 1900,
            timeinfo->tm_mon + 1,
            timeinfo->tm_mday,
            timeinfo->tm_hour,
            timeinfo->tm_min,
            timeinfo->tm_sec);

    std::string dich = duongDanBackup + std::string(buffer);

    bool ketQua = saoChepFile(nguon, dich);

    if (ketQua)
    {
        std::cout << "Tao backup thanh cong: " << dich.c_str() << std::endl;
    }
    else
    {
        std::cout << "Tao backup that bai!" << std::endl;
    }

    return ketQua;
}

bool FileManager::khoiPhucBackup(const std::string &tenFileBackup)
{
    std::string nguon = duongDanBackup + tenFileBackup;

    if (!kiemTraFileExists(nguon))
    {
        std::cout << "File backup khong ton tai!" << std::endl;
        return false;
    }

    std::string dich = duongDanData + std::string("system.dat");

    bool ketQua = saoChepFile(nguon, dich);

    if (ketQua)
    {
        std::cout << "Khoi phuc backup thanh cong!" << std::endl;
    }
    else
    {
        std::cout << "Khoi phuc backup that bai!" << std::endl;
    }

    return ketQua;
}

MangDong<std::string> FileManager::layDanhSachBackup() const
{
    MangDong<std::string> danhSach;

    // TODO: Implement directory listing
    // This would require platform-specific code (Windows vs Linux)

    std::cout << "Chuc nang lay danh sach backup chua duoc implement!" << std::endl;

    return danhSach;
}

bool FileManager::xoaBackup(const std::string &tenFileBackup)
{
    std::string duongDan = duongDanBackup + tenFileBackup;
    return xoaFile(duongDan);
}

void FileManager::xoaTatCaBackup()
{
    std::cout << "Chuc nang xoa tat ca backup chua duoc implement!" << std::endl;
}

// ===== UTILITY =====
bool FileManager::kiemTraFileExists(const std::string &tenFile) const
{
    FILE *f = fopen(tenFile.c_str(), "r");
    if (f != nullptr)
    {
        fclose(f);
        return true;
    }
    return false;
}

long FileManager::layKichThuocFile(const std::string &tenFile) const
{
    FILE *f = fopen(tenFile.c_str(), "rb");
    if (f == nullptr)
        return -1;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fclose(f);

    return size;
}

bool FileManager::xoaFile(const std::string &tenFile) const
{
    if (remove(tenFile.c_str()) == 0)
    {
        std::cout << "Xoa file thanh cong: " << tenFile.c_str() << std::endl;
        return true;
    }
    std::cout << "Xoa file that bai: " << tenFile.c_str() << std::endl;
    return false;
}

bool FileManager::saoChepFile(const std::string &nguon, const std::string &dich) const
{
    FILE *fNguon = fopen(nguon.c_str(), "rb");
    if (fNguon == nullptr)
    {
        std::cout << "Khong the mo file nguon: " << nguon.c_str() << std::endl;
        return false;
    }

    FILE *fDich = fopen(dich.c_str(), "wb");
    if (fDich == nullptr)
    {
        fclose(fNguon);
        std::cout << "Khong the tao file dich: " << dich.c_str() << std::endl;
        return false;
    }

    // Sao chép từng byte
    char buffer[4096];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), fNguon)) > 0)
    {
        fwrite(buffer, 1, bytesRead, fDich);
    }

    fclose(fNguon);
    fclose(fDich);

    return true;
}

// ===== XUẤT BÁO CÁO =====
bool FileManager::xuatBaoCaoText(const std::string &tenFile, const std::string &noiDung) const
{
    std::string duongDanDayDu = duongDanData + tenFile;

    FILE *f = fopen(duongDanDayDu.c_str(), "w");
    if (f == nullptr)
    {
        std::cout << "Khong the tao file bao cao: " << duongDanDayDu.c_str() << std::endl;
        return false;
    }

    fprintf(f, "%s", noiDung.c_str());
    fclose(f);

    std::cout << "Xuat bao cao thanh cong: " << duongDanDayDu.c_str() << std::endl;
    return true;
}

// ===== TẠO THƯ MỤC =====
bool FileManager::taoThuMuc(const std::string &duongDan)
{
    struct stat info;

    // Kiểm tra thư mục đã tồn tại chưa
    if (stat(duongDan.c_str(), &info) == 0 && (info.st_mode & S_IFDIR))
    {
        return true; // Thư mục đã tồn tại
    }

// Tạo thư mục (Windows)
#ifdef _WIN32
    if (_mkdir(duongDan.c_str()) == 0)
    {
        std::cout << "Tao thu muc thanh cong: " << duongDan.c_str() << std::endl;
        return true;
    }
#else
    // Linux/Unix
    if (mkdir(duongDan.c_str(), 0755) == 0)
    {
        std::cout << "Tao thu muc thanh cong: " << duongDan.c_str() << std::endl;
        return true;
    }
#endif

    // Có thể thư mục đã tồn tại hoặc lỗi khác
    return false;
}
