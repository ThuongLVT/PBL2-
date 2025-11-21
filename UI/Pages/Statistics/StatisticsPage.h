#ifndef STATISTICSPAGE_H
#define STATISTICSPAGE_H

#include <QWidget>
#include <QDateEdit>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QFrame>

class StatisticsPage : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsPage(QWidget *parent = nullptr);
    ~StatisticsPage();

    void refreshData();

private slots:
    void onAnalyzeClicked();

private:
    void setupUI();
    void setupHeader();
    void setupSummaryCards();
    void setupDetailsTable();

    // Helper to create a stat card
    QFrame *createStatCard(const QString &title, const QString &value, const QString &iconPath, const QString &color);

    // UI Elements
    QDateEdit *fromDateEdit;
    QDateEdit *toDateEdit;
    QPushButton *analyzeBtn;

    // Summary Labels
    QLabel *lblTotalRevenue;
    QLabel *lblFieldRevenue;
    QLabel *lblServiceRevenue;
    QLabel *lblTotalBookings;
    QLabel *lblCompleted;
    QLabel *lblCancelled;

    // Table
    QTableWidget *detailsTable;

    // Helper
    QString formatCurrency(double amount);
};

#endif // STATISTICSPAGE_H
