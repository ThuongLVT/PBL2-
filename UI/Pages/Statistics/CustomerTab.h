#ifndef CUSTOMERTAB_H
#define CUSTOMERTAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QtCharts/QChartView>

#include "../../Components/SummaryCard.h"
#include "../../Components/RankingTable.h"
#include "../../Components/ChartContainer.h"
#include "../../Components/MonthYearFilter.h"
#include "../../../Core/ThongKe/ThongKeKhachHang.h"

class HeThongQuanLy;

/**
 * @class CustomerTab
 * @brief Tab thống kê Khách hàng (Customer Analytics)
 *
 * Bộ lọc: Tháng / Năm (sử dụng MonthYearFilter component)
 * Hiển thị:
 * - Summary cards: Tổng KH đặt sân, Khách mới, Khách quay lại, Chi tiêu TB
 * - Donut chart: Phân bổ theo hạng
 * - Top khách hàng chi tiêu cao nhất
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
    void onFilterChanged(int month, int year, MonthYearFilter::FilterMode mode);
    void onExportPdf();

private:
    void setupUI();
    void createSummaryCards();
    void createTablesRow();

    void updateSummaryCards();
    void updateRankingTable();
    void updateMembershipTable();

    // UI Components
    QVBoxLayout *m_mainLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_contentWidget;
    QVBoxLayout *m_contentLayout;

    // Filter bar (reusable component)
    MonthYearFilter *m_filter;

    // Summary cards
    QHBoxLayout *m_cardsLayout;
    SummaryCard *m_totalCustomerCard;
    SummaryCard *m_newCustomerCard;
    SummaryCard *m_returningCard;

    // Tables (side by side)
    QHBoxLayout *m_tablesRow;
    RankingTable *m_topCustomersTable;
    RankingTable *m_membershipTable;

    // Data
    HeThongQuanLy *m_heThong;
    ThongKeKhachHang *m_thongKe;
};

#endif // CUSTOMERTAB_H
