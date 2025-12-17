#ifndef INVOICEDIALOG_H
#define INVOICEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <string>

class InvoiceDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InvoiceDialog(const std::string &invoiceText, QWidget *parent = nullptr);
    ~InvoiceDialog();

    void setReadOnly(bool isReadOnly);

private:
    QScrollArea *scrollArea;
    QLabel *invoiceLabel;
    QPushButton *btnPay;
    QPushButton *btnPayAndExport;
    QPushButton *btnCancel;

private slots:
    void onPayClicked();
    void onPayAndExportClicked();
    void onCancelClicked();
};

#endif // INVOICEDIALOG_H
