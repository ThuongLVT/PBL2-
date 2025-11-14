/**
 * @file CSVHandler.cpp
 * @brief Implementation of CSV file I/O utilities
 *
 * @author Football Field Management System
 * @date 2025-11-11
 */

#include "CSVHandler.h"
#include <iostream>
#include <sstream>

using namespace std;

vector<vector<string>> CSVHandler::readCSV(const string &filename)
{
    vector<vector<string>> rows;
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Cannot open CSV file for reading: " << filename << endl;
        return rows;
    }

    string line;
    // Skip header line
    if (getline(file, line))
    {
        // Header read, continue to data
    }

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        rows.push_back(parseLine(line));
    }

    file.close();
    return rows;
}

bool CSVHandler::writeCSV(const string &filename,
                          const vector<string> &headers,
                          const vector<vector<string>> &rows)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Cannot open CSV file for writing: " << filename << endl;
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
    return true;
}

string CSVHandler::escapeField(const string &field)
{
    // If field contains comma, quote, or newline, wrap in quotes and escape internal quotes
    if (field.find(',') != string::npos ||
        field.find('"') != string::npos ||
        field.find('\n') != string::npos)
    {
        string escaped = "\"";
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

vector<string> CSVHandler::parseLine(const string &line)
{
    vector<string> fields;
    string field;
    bool inQuotes = false;

    for (size_t i = 0; i < line.length(); i++)
    {
        char c = line[i];

        if (c == '"')
        {
            if (inQuotes && i + 1 < line.length() && line[i + 1] == '"')
            {
                // Escaped quote
                field += '"';
                i++; // Skip next quote
            }
            else
            {
                inQuotes = !inQuotes;
            }
        }
        else if (c == ',' && !inQuotes)
        {
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
