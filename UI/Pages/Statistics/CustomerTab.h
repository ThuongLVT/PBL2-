#ifndef CUSTOMERTAB_H
#define CUSTOMERTAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QtCharts/QChartView>

#include "../../Components/SummaryCard.h"
#include "../../Components/RankingTable.h"
#include "../../Components/DateRangePicker.h"
#include "../../Components/ChartContainer.h"
#include "../../../Core/ThongKe/ThongKeKhachHang.h"

class HeThongQuanLy;

/**
 * @class CustomerTab
 * @brief Tab thống kê Khách hàng (Customer Analytics)
 *
 * Hiển thị:
 * - Summary cards: Tổng KH, Khách mới, Khách quay lại
 * - Donut chart: Phân bổ theo hạng
 * - Top khách hàng chi tiêu cao nhất
 * - Customer insights
 */
class CustomerTab : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerTab(QWidget *parent = nullptr);
    ~CustomerTab();

    void setHeThong(HeThongQuanLy *ht);
    void refreshData();

private slots:
    void onDateRangeChanged(const QDate &from, const QDate &to);

private:
    void setupUI();
    void createSummaryCards();
    void createCharts();
    void createRankingTable();
    void createMembershipTable();
    void updateSummaryCards();
    void updateDonutChart();
    void updateRankingTable();
    void updateMembershipTable();

    // UI Components
    QVBoxLayout *m_mainLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_contentWidget;
    QVBoxLayout *m_contentLayout;

    // Date picker
    DateRangePicker *m_datePicker;

    // Summary cards
    QHBoxLayout *m_cardsLayout;
    SummaryCard *m_totalCustomerCard;
    SummaryCard *m_newCustomerCard;
    SummaryCard *m_returningCard;
    SummaryCard *m_avgSpendCard;

    // Charts
    QHBoxLayout *m_chartsRow;
    ChartContainer *m_donutChartContainer;

    // Tables
    RankingTable *m_topCustomersTable;
    RankingTable *m_membershipTable;

    // Data
    HeThongQuanLy *m_heThong;
    ThongKeKhachHang *m_thongKe;
    QDate m_fromDate;
    QDate m_toDate;
};

#endif // CUSTOMERTAB_H
