#ifndef INVOICEPAGE_H
#define INVOICEPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDateEdit>
#include <QHeaderView>

class InvoicePage : public QWidget
{
    Q_OBJECT

public:
    explicit InvoicePage(QWidget *parent = nullptr);
    ~InvoicePage();

    void setUserRole(bool isAdmin);
    void refreshData();

private slots:
    void onSearchTextChanged(const QString &text);
    void onTypeFilterChanged(int index);
    void onDateChanged();
    void onTodayClicked();
    void onYesterdayClicked();
    void onViewDetailsClicked(int row);
    void onSortFilterChanged(int index);

private:
    // UI Components
    QLineEdit *m_searchInput;
    QComboBox *m_typeFilter;
    QComboBox *m_sortFilter;

    // Date Selection
    QPushButton *m_todayBtn;
    QPushButton *m_yesterdayBtn;
    QDateEdit *m_datePicker;

    QLabel *m_totalRevenueLabel;
    QTableWidget *m_invoiceTable;

    // Data State
    QDate m_selectedDate;
    bool m_isAdmin;

    // Setup Functions
    void setupUI();
    void setupToolbar(QVBoxLayout *mainLayout);
    void setupTable(QVBoxLayout *mainLayout);

    // Logic Functions
    void loadData();
    void updateDateRangeFromFilter();
    QString formatCurrency(double amount) const;
    QWidget *createActionWidget(int row);
    QWidget *createStatusBadge(const QString &text, const QString &color);
};

#endif // INVOICEPAGE_H
