#ifndef REVENUETAB_H
#define REVENUETAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QLabel>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>

#include "../../Components/SummaryCard.h"
#include "../../Components/RankingTable.h"
#include "../../Components/ChartContainer.h"
#include "../../Components/MonthYearFilter.h"
#include "../../../Core/ThongKe/ThongKeDoanhThu.h"

class HeThongQuanLy;

/**
 * @class RevenueTab
 * @brief Tab thống kê doanh thu (Revenue Analytics)
 *
 * Bộ lọc: Tháng / Năm (dùng MonthYearFilter component)
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
    void onFilterChanged(int month, int year, MonthYearFilter::FilterMode mode);
    void onExportPdf();

private:
    void setupUI();
    void createSummaryCards();
    void createCharts();

    void updateSummaryCards();
    void updateRevenueChart();
    void updatePieChart();

    // UI Components
    QVBoxLayout *m_mainLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_contentWidget;
    QVBoxLayout *m_contentLayout;

    // Filter bar (reusable component)
    MonthYearFilter *m_filter;

    // Summary cards
    QHBoxLayout *m_cardsLayout;
    SummaryCard *m_totalRevenueCard;
    SummaryCard *m_fieldRevenueCard;
    SummaryCard *m_serviceRevenueCard;

    // Charts
    QHBoxLayout *m_chartsRow1;
    QHBoxLayout *m_chartsRow2;
    ChartContainer *m_revenueChartContainer;
    ChartContainer *m_pieChartContainer;

    // Data
    HeThongQuanLy *m_heThong;
    ThongKeDoanhThu *m_thongKe;
};

#endif // REVENUETAB_H
