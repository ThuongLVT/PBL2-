#ifndef BOOKINGFIELDTAB_H
#define BOOKINGFIELDTAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QtCharts/QChartView>

#include "../../Components/SummaryCard.h"
#include "../../Components/RankingTable.h"
#include "../../Components/ChartContainer.h"
#include "../../Components/MonthYearFilter.h"
#include "../../../Core/ThongKe/ThongKeBooking.h"

class HeThongQuanLy;

/**
 * @class BookingFieldTab
 * @brief Tab thống kê Đặt Sân
 *
 * Bộ lọc: Tháng / Năm (dùng MonthYearFilter component)
 * Hiển thị:
 * - Summary cards: Tổng booking, Hoàn thành, Đã hủy
 * - Donut chart: Tỷ lệ loại sân (5 người vs 7 người)
 * - Bar chart: Booking theo ngày trong tuần (T2-CN)
 * - Peak hours chart: Phân bổ theo khung giờ
 * - Top sân table
 */
class BookingFieldTab : public QWidget
{
    Q_OBJECT

public:
    explicit BookingFieldTab(QWidget *parent = nullptr);
    ~BookingFieldTab();

    void setHeThong(HeThongQuanLy *ht);
    void refreshData();

private slots:
    void onFilterChanged(int month, int year, MonthYearFilter::FilterMode mode);
    void onExportPdf();

private:
    void setupUI();
    void createSummaryCards();
    void createCharts();
    void createRankingTable();

    void updateSummaryCards();
    void updateDonutChart();
    void updateWeekdayChart();
    void updatePeakHoursChart();
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
    SummaryCard *m_totalBookingCard;
    SummaryCard *m_completedCard;
    SummaryCard *m_cancelledCard;

    // Charts
    QHBoxLayout *m_chartsRow;
    QHBoxLayout *m_chartsRow2;
    ChartContainer *m_donutChartContainer;
    ChartContainer *m_weekdayChartContainer;
    ChartContainer *m_peakHoursChartContainer;

    // Ranking table
    RankingTable *m_topFieldsTable;

    // Data
    HeThongQuanLy *m_heThong;
    ThongKeBooking *m_thongKe;
};

#endif // BOOKINGFIELDTAB_H
