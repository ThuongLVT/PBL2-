#ifndef INVOICEGENERATOR_H
#define INVOICEGENERATOR_H

#include <string>
#include "../Models/DatSan.h"
#include "../Models/DonHangDichVu.h"

class InvoiceGenerator
{
public:
    static std::string generateBookingInvoice(const DatSan &datSan);
    static std::string generateServiceInvoice(const DonHangDichVu &donHang);
    static std::string generateCancellationInvoice(const DatSan &datSan, const std::string &lyDo);

private:
    static std::string formatCurrency(double amount);
    static std::string centerString(const std::string &str, int width);
    static std::string padRight(const std::string &str, int width);
    static std::string padLeft(const std::string &str, int width);
    static std::string getCurrentDateTime();
    static std::string getRow(const std::string &label, const std::string &value);
};

#endif // INVOICEGENERATOR_H
