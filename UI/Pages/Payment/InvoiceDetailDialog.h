#ifndef INVOICEDETAILDIALOG_H
#define INVOICEDETAILDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>
#include "../../../Core/Models/DatSan.h"
#include "../../../Core/Models/DonHangDichVu.h"

class InvoiceDetailDialog : public QDialog
{
    Q_OBJECT

public:
    enum InvoiceType
    {
        BOOKING,
        SERVICE
    };

    explicit InvoiceDetailDialog(const QString &id, InvoiceType type, QWidget *parent = nullptr);
    ~InvoiceDetailDialog();

private slots:
    void onExportPdfClicked();

private:
    void setupUI();
    void loadBookingData(const QString &id);
    void loadServiceData(const QString &id);
    void printInvoice(const QString &filename);
    void applyStyles();
    QFrame *createSeparator();

    QString m_id;
    InvoiceType m_type;
    DatSan *m_booking;
    DonHangDichVu *m_order;

    // Store data for PDF
    QString m_invoiceNumber;
    QString m_invoiceDate;
    QString m_customerName;
    QString m_customerPhone;
    QString m_customerAddress;
    QString m_bookingInfo;
    QString m_paymentMethod;
    QString m_status;

    // UI Components - Header
    QLabel *m_companyNameLabel;

    // Invoice Title
    QLabel *m_titleLabel;
    QLabel *m_invoiceNumberLabel;
    QLabel *m_invoiceDateLabel;

    // Customer Info
    QLabel *m_customerNameLabel;
    QLabel *m_customerPhoneLabel;
    QLabel *m_customerAddressLabel;
    QLabel *m_bookingInfoLabel;

    QTableWidget *m_detailsTable;

    // Summary Labels
    QLabel *m_subtotalLabel;
    QLabel *m_depositLabel;
    QLabel *m_discountLabel;
    QLabel *m_totalLabel;
    QLabel *m_amountInWordsLabel;
    QLabel *m_statusLabel;

    QPushButton *m_exportBtn;
    QPushButton *m_closeBtn;
};

#endif // INVOICEDETAILDIALOG_H
