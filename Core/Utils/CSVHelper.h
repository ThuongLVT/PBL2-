// CSVHelper.h - Centralized CSV Helper for all data persistence
// Handles all CSV I/O operations with auto-save functionality
// Uses MangDong instead of std::vector for custom DSA implementation

#ifndef CSVHELPER_H
#define CSVHELPER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../CauTrucDuLieu/MangDong.h"

// Centralized helper for CSV file operations
// Features: Read/Write CSV files, Automatic path handling, Safe file operations
class CSVHelper
{
public:
    // Lay duong dan thu muc Data
    static std::string getDataPath();

    // Doc file CSV va tra ve tat ca cac dong (filename: ten file, skipHeader: bo qua dong dau)
    static MangDong<MangDong<std::string>> readCSV(const std::string &filename, bool skipHeader = true);

    // Ghi du lieu ra file CSV (filename: ten file, headers: tieu de cot, rows: du lieu)
    static bool writeCSV(const std::string &filename,
                         const MangDong<std::string> &headers,
                         const MangDong<MangDong<std::string>> &rows);

    // Escape ky tu dac biet trong truong CSV (field: chuoi can escape)
    static std::string escapeField(const std::string &field);

    // Parse mot dong CSV thanh cac truong (line: dong CSV)
    static MangDong<std::string> parseLine(const std::string &line);

    // Kiem tra file ton tai (filename: ten file can kiem tra)
    static bool fileExists(const std::string &filename);

    // Tao file CSV rong voi header (filename: ten file, headers: tieu de cot)
    static bool createEmptyCSV(const std::string &filename,
                               const MangDong<std::string> &headers);

    // Dam bao thu muc Data ton tai
    static bool ensureDataDirectory();

private:
    CSVHelper() {} // Static class, no instantiation
};

#endif // CSVHELPER_H
