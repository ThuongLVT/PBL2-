/**
 * @file BackupManager.h
 * @brief Lớp quản lý backup và restore dữ liệu
 * @details Sao lưu và khôi phục dữ liệu hệ thống
 *
 * @author Football Field Management System
 * @date 2025-11-03
 */

#ifndef BACKUPMANAGER_H
#define BACKUPMANAGER_H

#include <string>

/**
 * @class BackupManager
 * @brief Lớp quản lý backup/restore
 *
 * Chức năng:
 * - Sao lưu dữ liệu theo ngày
 * - Khôi phục từ backup
 * - Liệt kê các file backup
 * - Xóa backup cũ
 */
class BackupManager
{
private:
    std::string thuMucBackup; ///< Thư mục chứa backup (mặc định: Data/backup/)

    /**
     * @brief Lấy tên file backup theo ngày hiện tại
     * @return Tên file backup
     */
    std::string layTenFileBackup() const;

public:
    // ========== CONSTRUCTORS ==========

    /**
     * @brief Constructor với thư mục backup
     * @param thuMuc Thư mục backup (mặc định: "Data/backup/")
     */
    BackupManager(const std::string &thuMuc = "Data/backup/");

    /**
     * @brief Destructor
     */
    ~BackupManager();

    // ========== BACKUP OPERATIONS ==========

    /**
     * @brief Sao lưu file dữ liệu
     * @param fileGoc File dữ liệu gốc (VD: system.dat)
     * @return true nếu backup thành công
     */
    bool saoLuu(const std::string &fileGoc);

    /**
     * @brief Khôi phục từ file backup
     * @param fileBackup Tên file backup (VD: system_backup_20251103.dat)
     * @param fileDich File đích để khôi phục (VD: system.dat)
     * @return true nếu restore thành công
     */
    bool khoiPhuc(const std::string &fileBackup, const std::string &fileDich);

    /**
     * @brief Khôi phục từ backup mới nhất
     * @param fileDich File đích để khôi phục
     * @return true nếu restore thành công
     */
    bool khoiPhucMoiNhat(const std::string &fileDich);

    // ========== FILE MANAGEMENT ==========

    /**
     * @brief Kiểm tra thư mục backup có tồn tại không
     * @return true nếu tồn tại
     */
    bool kiemTraThuMucBackup() const;

    /**
     * @brief Tạo thư mục backup nếu chưa có
     * @return true nếu tạo thành công hoặc đã tồn tại
     */
    bool taoThuMucBackup();

    /**
     * @brief Xóa các file backup cũ (giữ lại N file mới nhất)
     * @param soLuongGiuLai Số lượng backup giữ lại
     * @return Số file đã xóa
     */
    int xoaBackupCu(int soLuongGiuLai = 7);

    // ========== DISPLAY ==========

    /**
     * @brief Hiển thị danh sách file backup
     */
    void hienThiDanhSachBackup() const;
};

#endif // BACKUPMANAGER_H
