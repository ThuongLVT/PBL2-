#ifndef REVENUETAB_H
#define REVENUETAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>

#include "../../Components/SummaryCard.h"
#include "../../Components/RankingTable.h"
#include "../../Components/DateRangePicker.h"
#include "../../Components/ChartContainer.h"
#include "../../../Core/ThongKe/ThongKeDoanhThu.h"

class HeThongQuanLy;

/**
 * @class RevenueTab
 * @brief Tab thống kê doanh thu (Revenue Analytics)
 *
 * Hiển thị:
 * - Summary cards: Tổng doanh thu, Doanh thu sân, Doanh thu dịch vụ
 * - Line chart: Doanh thu theo thời gian
 * - Pie chart: Phân bổ nguồn thu
 * - So sánh với kỳ trước
 */
class RevenueTab : public QWidget
{
    Q_OBJECT

public:
    explicit RevenueTab(QWidget *parent = nullptr);
    ~RevenueTab();

    void setHeThong(HeThongQuanLy *ht);
    void refreshData();

private slots:
    void onDateRangeChanged(const QDate &from, const QDate &to);

private:
    void setupUI();
    void createSummaryCards();
    void createCharts();
    void updateSummaryCards();
    void updateLineChart();
    void updatePieChart();
    void updateBarChart();

    // UI Components
    QVBoxLayout *m_mainLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_contentWidget;
    QVBoxLayout *m_contentLayout;

    // Date picker
    DateRangePicker *m_datePicker;

    // Summary cards
    QHBoxLayout *m_cardsLayout;
    SummaryCard *m_totalRevenueCard;
    SummaryCard *m_fieldRevenueCard;
    SummaryCard *m_serviceRevenueCard;
    SummaryCard *m_avgRevenueCard;

    // Charts
    QHBoxLayout *m_chartsRow1;
    QHBoxLayout *m_chartsRow2;
    ChartContainer *m_lineChartContainer;
    ChartContainer *m_pieChartContainer;
    ChartContainer *m_barChartContainer;

    // Data
    HeThongQuanLy *m_heThong;
    ThongKeDoanhThu *m_thongKe;
    QDate m_fromDate;
    QDate m_toDate;
};

#endif // REVENUETAB_H
