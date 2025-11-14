/**
 * @file BackupManager.cpp
 * @brief Triển khai các phương thức của lớp BackupManager
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#include "BackupManager.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#define MKDIR(path) _mkdir(path)
#else
#include <dirent.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

using namespace std;

BackupManager::BackupManager(const string &thuMuc)
    : thuMucBackup(thuMuc)
{
    // Tạo thư mục backup nếu chưa có
    taoThuMucBackup();
}

BackupManager::~BackupManager()
{
}

string BackupManager::layTenFileBackup() const
{
    // Lấy ngày hiện tại
    time_t now = time(0);
    tm *ltm = localtime(&now);

    char buffer[32];
    sprintf(buffer, "system_backup_%04d%02d%02d.dat",
            1900 + ltm->tm_year,
            1 + ltm->tm_mon,
            ltm->tm_mday);

    return thuMucBackup + "/" + string(buffer);
}

bool BackupManager::saoLuu(const string &fileGoc)
{
    // Kiểm tra file gốc tồn tại
    ifstream fIn(fileGoc, ios::binary);
    if (!fIn.is_open())
    {
        cout << "Loi: Khong the mo file goc: " << fileGoc << endl;
        return false;
    }

    // Tạo file backup
    string fileBackup = layTenFileBackup();
    ofstream fOut(fileBackup, ios::binary);
    if (!fOut.is_open())
    {
        fIn.close();
        cout << "Loi: Khong the tao file backup: " << fileBackup << endl;
        return false;
    }

    // Copy dữ liệu
    char buffer[4096];
    while (fIn.read(buffer, sizeof(buffer)) || fIn.gcount() > 0)
    {
        fOut.write(buffer, fIn.gcount());
    }

    fIn.close();
    fOut.close();

    cout << "Sao luu thanh cong: " << fileBackup << endl;
    return true;
}

bool BackupManager::khoiPhuc(const string &fileBackup, const string &fileDich)
{
    // Kiểm tra file backup tồn tại
    string duongDanBackup = thuMucBackup + "/" + fileBackup;
    ifstream fIn(duongDanBackup, ios::binary);
    if (!fIn.is_open())
    {
        cout << "Loi: Khong tim thay file backup: " << duongDanBackup << endl;
        return false;
    }

    // Tạo file đích
    ofstream fOut(fileDich, ios::binary);
    if (!fOut.is_open())
    {
        fIn.close();
        cout << "Loi: Khong the tao file dich: " << fileDich << endl;
        return false;
    }

    // Copy dữ liệu
    char buffer[4096];
    while (fIn.read(buffer, sizeof(buffer)) || fIn.gcount() > 0)
    {
        fOut.write(buffer, fIn.gcount());
    }

    fIn.close();
    fOut.close();

    cout << "Khoi phuc thanh cong tu: " << duongDanBackup << endl;
    return true;
}

bool BackupManager::khoiPhucMoiNhat(const string &fileDich)
{
    // TODO: Tìm file backup mới nhất trong thư mục
    // Tạm thời sử dụng file backup hôm nay
    string fileBackup = layTenFileBackup();

    // Lấy tên file từ đường dẫn đầy đủ
    size_t pos = fileBackup.find_last_of("/\\");
    if (pos != string::npos)
    {
        fileBackup = fileBackup.substr(pos + 1);
    }

    return khoiPhuc(fileBackup, fileDich);
}

bool BackupManager::kiemTraThuMucBackup() const
{
    struct stat info;
    return (stat(thuMucBackup.c_str(), &info) == 0 && (info.st_mode & S_IFDIR));
}

bool BackupManager::taoThuMucBackup()
{
    if (kiemTraThuMucBackup())
    {
        return true; // Đã tồn tại
    }

    // Tạo thư mục Data nếu chưa có
    MKDIR("Data");

    // Tạo thư mục backup
    if (MKDIR(thuMucBackup.c_str()) == 0)
    {
        cout << "Tao thu muc backup thanh cong: " << thuMucBackup << endl;
        return true;
    }

    return false;
}

int BackupManager::xoaBackupCu(int /*soLuongGiuLai*/)
{
    // TODO: Implement xóa file backup cũ
    // Liệt kê các file trong thư mục, sắp xếp theo ngày, xóa file cũ
    cout << "Chuc nang xoa backup cu chua duoc implement" << endl;
    return 0;
}

void BackupManager::hienThiDanhSachBackup() const
{
    cout << "\n===== DANH SACH BACKUP =====" << endl;
    cout << "Thu muc: " << thuMucBackup << endl;

    // TODO: Liệt kê các file backup
    cout << "Chuc nang liet ke backup chua duoc implement" << endl;

    cout << "============================" << endl;
}
