/**
 * @file CSVManager.cpp
 * @brief Implementation of CSVManager
 *
 * @author Football Field Management System
 * @date 2025-11-18
 */

#include "CSVManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

std::string CSVManager::getDataPath()
{
    return "D:/QT_PBL2/Data/";
}

bool CSVManager::ensureDataDirectory()
{
    std::string dataPath = getDataPath();
    struct stat info;

    if (stat(dataPath.c_str(), &info) != 0)
    {
        // Directory doesn't exist, create it
        if (MKDIR(dataPath.c_str()) != 0)
        {
            std::cerr << "Failed to create Data directory: " << dataPath << std::endl;
            return false;
        }
        std::cout << "Created Data directory: " << dataPath << std::endl;
    }
    return true;
}

bool CSVManager::fileExists(const std::string &filename)
{
    std::string fullPath = getDataPath() + filename;
    std::ifstream file(fullPath);
    return file.good();
}

std::vector<std::vector<std::string>> CSVManager::readCSV(const std::string &filename, bool skipHeader)
{
    std::vector<std::vector<std::string>> data;
    std::string fullPath = getDataPath() + filename;

    std::ifstream file(fullPath);
    if (!file.is_open())
    {
        std::cerr << "Cannot open CSV file: " << fullPath << std::endl;
        return data;
    }

    std::string line;
    bool firstLine = true;

    while (std::getline(file, line))
    {
        if (firstLine && skipHeader)
        {
            firstLine = false;
            continue;
        }

        if (line.empty())
            continue;

        std::vector<std::string> row = parseLine(line);
        data.push_back(row);
        firstLine = false;
    }

    file.close();
    return data;
}

bool CSVManager::writeCSV(const std::string &filename,
                          const std::vector<std::string> &headers,
                          const std::vector<std::vector<std::string>> &rows)
{
    if (!ensureDataDirectory())
    {
        return false;
    }

    std::string fullPath = getDataPath() + filename;
    std::ofstream file(fullPath);

    if (!file.is_open())
    {
        std::cerr << "Cannot create CSV file: " << fullPath << std::endl;
        return false;
    }

    // Write headers
    for (size_t i = 0; i < headers.size(); i++)
    {
        file << escapeField(headers[i]);
        if (i < headers.size() - 1)
            file << ",";
    }
    file << "\n";

    // Write rows
    for (const auto &row : rows)
    {
        for (size_t i = 0; i < row.size(); i++)
        {
            file << escapeField(row[i]);
            if (i < row.size() - 1)
                file << ",";
        }
        file << "\n";
    }

    file.close();
    std::cout << "Saved CSV file: " << fullPath << std::endl;
    return true;
}

std::string CSVManager::escapeField(const std::string &field)
{
    // If field contains comma, newline, or quote, wrap in quotes
    if (field.find(',') != std::string::npos ||
        field.find('\n') != std::string::npos ||
        field.find('"') != std::string::npos)
    {
        std::string escaped = "\"";
        for (char c : field)
        {
            if (c == '"')
                escaped += "\"\""; // Escape quote with double quote
            else
                escaped += c;
        }
        escaped += "\"";
        return escaped;
    }
    return field;
}

std::vector<std::string> CSVManager::parseLine(const std::string &line)
{
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;

    for (size_t i = 0; i < line.size(); i++)
    {
        char c = line[i];

        if (c == '"')
        {
            if (inQuotes && i + 1 < line.size() && line[i + 1] == '"')
            {
                // Double quote = escaped quote
                field += '"';
                i++; // Skip next quote
            }
            else
            {
                // Toggle quote mode
                inQuotes = !inQuotes;
            }
        }
        else if (c == ',' && !inQuotes)
        {
            // End of field
            fields.push_back(field);
            field.clear();
        }
        else
        {
            field += c;
        }
    }

    // Add last field
    fields.push_back(field);

    return fields;
}

bool CSVManager::createEmptyCSV(const std::string &filename,
                                const std::vector<std::string> &headers)
{
    std::vector<std::vector<std::string>> emptyRows;
    return writeCSV(filename, headers, emptyRows);
}
