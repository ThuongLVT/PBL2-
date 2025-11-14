/**
 * @file CSVHandler.h
 * @brief CSV file I/O utilities for data persistence
 *
 * @author Football Field Management System
 * @date 2025-11-11
 */

#ifndef CSVHANDLER_H
#define CSVHANDLER_H

#include <string>
#include <vector>
#include <fstream>

/**
 * @brief Utility class for CSV file operations
 */
class CSVHandler
{
public:
    /**
     * @brief Read a CSV file and return rows as vector of string vectors
     * @param filename Path to CSV file
     * @return Vector of rows, each row is a vector of fields
     */
    static std::vector<std::vector<std::string>> readCSV(const std::string &filename);

    /**
     * @brief Write data to CSV file
     * @param filename Path to CSV file
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
     * @return Escaped string
     */
    static std::string escapeField(const std::string &field);

    /**
     * @brief Split a CSV line into fields
     * @param line CSV line
     * @return Vector of fields
     */
    static std::vector<std::string> parseLine(const std::string &line);

private:
    CSVHandler() {} // Static class, no instantiation
};

#endif // CSVHANDLER_H
