#ifndef SERVICETAB_H
#define SERVICETAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QtCharts/QChartView>

#include "../../Components/SummaryCard.h"
#include "../../Components/RankingTable.h"
#include "../../Components/DateRangePicker.h"
#include "../../Components/ChartContainer.h"
#include "../../../Core/ThongKe/ThongKeDichVu.h"

class HeThongQuanLy;

/**
 * @class ServiceTab
 * @brief Tab thống kê Dịch vụ (Service Analytics)
 *
 * Hiển thị:
 * - Summary cards: Tổng đơn DV, Doanh thu theo loại
 * - Pie chart: Phân bổ theo loại dịch vụ
 * - Top dịch vụ bán chạy nhất
 * - Thống kê tồn kho
 */
class ServiceTab : public QWidget
{
    Q_OBJECT

public:
    explicit ServiceTab(QWidget *parent = nullptr);
    ~ServiceTab();

    void setHeThong(HeThongQuanLy *ht);
    void refreshData();

private slots:
    void onDateRangeChanged(const QDate &from, const QDate &to);

private:
    void setupUI();
    void createSummaryCards();
    void createCharts();
    void createRankingTable();
    void updateSummaryCards();
    void updatePieChart();
    void updateRankingTable();

    // UI Components
    QVBoxLayout *m_mainLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_contentWidget;
    QVBoxLayout *m_contentLayout;

    // Date picker
    DateRangePicker *m_datePicker;

    // Summary cards
    QHBoxLayout *m_cardsLayout;
    SummaryCard *m_totalSalesCard;
    SummaryCard *m_drinkCard;
    SummaryCard *m_foodCard;
    SummaryCard *m_equipmentCard;

    // Charts
    QHBoxLayout *m_chartsRow;
    ChartContainer *m_pieChartContainer;

    // Tables
    RankingTable *m_topServicesTable;

    // Data
    HeThongQuanLy *m_heThong;
    ThongKeDichVu *m_thongKe;
    QDate m_fromDate;
    QDate m_toDate;
};

#endif // SERVICETAB_H
