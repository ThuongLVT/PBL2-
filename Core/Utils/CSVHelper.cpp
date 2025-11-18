#include "CSVHelper.h"
#include <iostream>

using namespace std;

bool CSVHelper::writeCSV(const string &filename,
                         const vector<string> &headers,
                         const vector<vector<string>> &rows)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Cannot open file for writing: " << filename << endl;
        return false;
    }

    // Write header
    for (size_t i = 0; i < headers.size(); i++)
    {
        file << escapeCSV(headers[i]);
        if (i < headers.size() - 1)
            file << ",";
    }
    file << "\n";

    // Write rows
    for (const auto &row : rows)
    {
        for (size_t i = 0; i < row.size(); i++)
        {
            file << escapeCSV(row[i]);
            if (i < row.size() - 1)
                file << ",";
        }
        file << "\n";
    }

    file.close();
    cout << "✓ Saved CSV: " << filename << " (" << rows.size() << " rows)" << endl;
    return true;
}

bool CSVHelper::readCSV(const string &filename, vector<vector<string>> &rows)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Cannot open file for reading: " << filename << endl;
        return false;
    }

    rows.clear();
    string line;

    // Skip header
    getline(file, line);

    // Read data rows
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        vector<string> fields = parseCSVLine(line);
        rows.push_back(fields);
    }

    file.close();
    cout << "✓ Loaded CSV: " << filename << " (" << rows.size() << " rows)" << endl;
    return true;
}

string CSVHelper::escapeCSV(const string &value)
{
    // If value contains comma or quote, wrap in quotes and escape internal quotes
    if (value.find(',') != string::npos || value.find('"') != string::npos)
    {
        string escaped = "\"";
        for (char c : value)
        {
            if (c == '"')
                escaped += "\"\""; // Double the quote
            else
                escaped += c;
        }
        escaped += "\"";
        return escaped;
    }
    return value;
}

vector<string> CSVHelper::parseCSVLine(const string &line)
{
    vector<string> fields;
    string field;
    bool inQuotes = false;

    for (size_t i = 0; i < line.length(); i++)
    {
        char c = line[i];

        if (c == '"')
        {
            // Check for escaped quote ("")
            if (inQuotes && i + 1 < line.length() && line[i + 1] == '"')
            {
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
