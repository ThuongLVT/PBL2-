/**
 * @file CSVHelper.h
 * @brief Centralized CSV Helper for all data persistence
 * @details Handles all CSV I/O operations with auto-save functionality
 *
 * @author Football Field Management System
 * @date 2025-11-26
 */

#ifndef CSVHELPER_H
#define CSVHELPER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * @class CSVHelper
 * @brief Centralized helper for CSV file operations
 *
 * Features:
 * - Read/Write CSV files
 * - Automatic path handling
 * - Safe file operations
 * - UTF-8 support
 */
class CSVHelper
{
public:
    /**
     * @brief Get the Data directory path
     * @return Path to Data directory
     */
    static std::string getDataPath();

    /**
     * @brief Read CSV file and return all rows
     * @param filename Filename (e.g., "khachhang.csv")
     * @param skipHeader Skip first row (header)
     * @return Vector of rows, each row is a vector of fields
     */
    static std::vector<std::vector<std::string>> readCSV(const std::string &filename, bool skipHeader = true);

    /**
     * @brief Write data to CSV file
     * @param filename Filename (e.g., "khachhang.csv")
     * @param headers Column headers
     * @param rows Data rows
     * @return true if successful
     */
    static bool writeCSV(const std::string &filename,
                         const std::vector<std::string> &headers,
                         const std::vector<std::vector<std::string>> &rows);

    /**
     * @brief Escape special characters in CSV field
     * @param field String to escape
     * @return Escaped string (with quotes if needed)
     */
    static std::string escapeField(const std::string &field);

    /**
     * @brief Parse a CSV line into fields
     * @param line CSV line
     * @return Vector of fields
     */
    static std::vector<std::string> parseLine(const std::string &line);

    /**
     * @brief Check if file exists
     * @param filename Filename to check
     * @return true if file exists
     */
    static bool fileExists(const std::string &filename);

    /**
     * @brief Create empty CSV file with headers
     * @param filename Filename
     * @param headers Column headers
     * @return true if successful
     */
    static bool createEmptyCSV(const std::string &filename,
                               const std::vector<std::string> &headers);

    /**
     * @brief Ensure Data directory exists
     * @return true if directory exists or was created
     */
    static bool ensureDataDirectory();

private:
    CSVHelper() {} // Static class, no instantiation
};

#endif // CSVHELPER_H
