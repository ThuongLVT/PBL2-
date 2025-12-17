#ifndef SERVICETAB_H
#define SERVICETAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QtCharts/QChartView>

#include "../../Components/SummaryCard.h"
#include "../../Components/RankingTable.h"
#include "../../Components/ChartContainer.h"
#include "../../Components/MonthYearFilter.h"
#include "../../../Core/ThongKe/ThongKeDichVu.h"

class HeThongQuanLy;

/**
 * @class ServiceTab
 * @brief Tab thống kê Dịch vụ (Service Analytics)
 *
 * Bộ lọc: Tháng / Năm (sử dụng MonthYearFilter component)
 * Hiển thị:
 * - Summary cards: Tổng đơn DV, Doanh thu theo loại
 * - Pie chart: Phân bổ theo loại dịch vụ
 * - Top dịch vụ bán chạy nhất
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
    void onFilterChanged(int month, int year, MonthYearFilter::FilterMode mode);
    void onExportPdf();

private:
    void setupUI();
    void createSummaryCards();
    void createRankingTable();

    void updateSummaryCards();
    void updateRankingTable();

    // UI Components
    QVBoxLayout *m_mainLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_contentWidget;
    QVBoxLayout *m_contentLayout;

    // Filter bar (reusable component)
    MonthYearFilter *m_filter;

    // Summary cards
    QHBoxLayout *m_cardsLayout;
    SummaryCard *m_totalSalesCard;
    SummaryCard *m_drinkCard;
    SummaryCard *m_foodCard;
    SummaryCard *m_equipmentCard;

    // Tables
    RankingTable *m_topServicesTable;

    // Data
    HeThongQuanLy *m_heThong;
    ThongKeDichVu *m_thongKe;
};

#endif // SERVICETAB_H
