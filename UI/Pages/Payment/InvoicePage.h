#ifndef INVOICEPAGE_H
#define INVOICEPAGE_H

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHeaderView>

class InvoicePage : public QWidget
{
    Q_OBJECT

public:
    explicit InvoicePage(QWidget *parent = nullptr);
    ~InvoicePage();

    void refreshData(); // To reload data when switching to this page

private:
    QTabWidget *m_tabWidget;
    QWidget *m_bookingInvoiceTab;
    QWidget *m_serviceInvoiceTab;

    QTableWidget *m_bookingTable;
    QTableWidget *m_serviceTable;

    QLineEdit *m_searchBookingInput;
    QLineEdit *m_searchServiceInput;

    void setupUI();
    void setupBookingTab();
    void setupServiceTab();

    void loadBookingData();
    void loadServiceData();

    void filterBookingData(const QString &text);
    void filterServiceData(const QString &text);
};

#endif // INVOICEPAGE_H
