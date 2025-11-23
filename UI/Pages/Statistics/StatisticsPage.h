#ifndef STATISTICSPAGE_H
#define STATISTICSPAGE_H

#include <QWidget>
#include <QDateEdit>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QFrame>
#include <QScrollArea>
#include <QTabWidget>
#include <QComboBox>
#include <QtCharts>

class StatisticsPage : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsPage(QWidget *parent = nullptr);
    ~StatisticsPage();

    void refreshData();

private slots:
    void onAnalyzeClicked();
    void onTabChanged(int index);

private:
    void setupUI();
    void setupHeader();
    
    // Tab Setup Methods
    void setupOverviewTab();
    void setupRevenueTab();
    void setupFieldTab();
    void setupServiceTab();
    void setupCustomerTab();
    void setupAnalysisTab();

    // Data Update Methods
    void updateOverview();
    void updateRevenue();
    void updateFieldStats();
    void updateServiceStats();
    void updateCustomerStats();
    void updateAnalysisStats();

    // Helper to create a stat card
    QFrame *createStatCard(const QString &title, const QString &value, const QString &color, const QString &icon = "");
    
    // Helper for Charts
    QChartView* createChartContainer(QChart* chart);
    
    // Helper for Table Styling
    void setupTableStyle(QTableWidget* table);

    // UI Elements
    QDateEdit *fromDateEdit;
    QDateEdit *toDateEdit;
    QPushButton *analyzeBtn;
    QPushButton *exportPdfBtn;
    QTabWidget *tabWidget;

    // Overview Widgets
    QLabel *lblTotalRevenue;
    QLabel *lblTotalBookings;
    QLabel *lblTotalCustomers;
    QLabel *lblAvgRevenue;
    QLabel *lblGrowth;
    QChartView *revenueTrendChart; // New

    // Revenue Widgets
    QTableWidget *revenueTable;
    QLabel *lblFieldRevenue;
    QLabel *lblServiceRevenue;
    QChartView *revenuePieChart; // New

    // Field Widgets
    QTableWidget *fieldTable;
    QChartView *fieldBarChart; // New

    // Service Widgets
    QTableWidget *serviceTable;

    // Customer Widgets
    QTableWidget *customerTable;

    // Analysis Widgets
    QTableWidget *peakHoursTable;
    QLabel *lblOccupancyRate;
    QLabel *lblForecastRevenue;
    QChartView *peakHoursChart; // New

    // Helper
    QString formatCurrency(double amount);
    void exportToPdf();
};

#endif // STATISTICSPAGE_H
