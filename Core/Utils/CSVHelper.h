#ifndef CSVHELPER_H
#define CSVHELPER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class CSVHelper
{
public:
    /**
     * @brief Write CSV header and rows to file
     */
    static bool writeCSV(const std::string &filename,
                         const std::vector<std::string> &headers,
                         const std::vector<std::vector<std::string>> &rows);

    /**
     * @brief Read CSV file and return rows (excluding header)
     */
    static bool readCSV(const std::string &filename,
                        std::vector<std::vector<std::string>> &rows);

    /**
     * @brief Escape CSV value (handle commas and quotes)
     */
    static std::string escapeCSV(const std::string &value);

    /**
     * @brief Parse CSV line into fields
     */
    static std::vector<std::string> parseCSVLine(const std::string &line);
};

#endif // CSVHELPER_H
