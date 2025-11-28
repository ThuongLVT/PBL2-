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
#include "../../Components/DateRangePicker.h"

class InvoicePage : public QWidget
{
    Q_OBJECT

public:
    explicit InvoicePage(QWidget *parent = nullptr);
    ~InvoicePage();

    void setUserRole(bool isAdmin);
    void refreshData(); // To reload data when switching to this page

private slots:
    void onDateRangeChanged(const QDate &from, const QDate &to);
    void onTableItemDoubleClicked(QTableWidgetItem *item);

private:
    QTabWidget *m_tabWidget;
    QWidget *m_bookingInvoiceTab;
    QWidget *m_serviceInvoiceTab;

    QTableWidget *m_bookingTable;
    QTableWidget *m_serviceTable;

    QLineEdit *m_searchBookingInput;
    QLineEdit *m_searchServiceInput;

    DateRangePicker *m_datePicker;
    QDate m_fromDate;
    QDate m_toDate;
    bool m_isAdmin;

    void setupUI();
    void setupBookingTab();
    void setupServiceTab();

    void loadBookingData();
    void loadServiceData();

    void filterBookingData(const QString &text);
    void filterServiceData(const QString &text);
};

#endif // INVOICEPAGE_H
