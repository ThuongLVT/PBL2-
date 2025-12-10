#ifndef BOOKINGFIELDTAB_H
#define BOOKINGFIELDTAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QtCharts/QChartView>

#include "../../Components/SummaryCard.h"
#include "../../Components/RankingTable.h"
#include "../../Components/DateRangePicker.h"
#include "../../Components/ChartContainer.h"
#include "../../../Core/ThongKe/ThongKeBooking.h"

class HeThongQuanLy;

/**
 * @class BookingFieldTab
 * @brief Tab thống kê Booking & Sân
 *
 * Hiển thị:
 * - Summary cards: Tổng booking, Hoàn thành, Đã hủy
 * - Donut chart: Tỷ lệ loại sân (5 người vs 7 người)
 * - Bar chart: Top sân được đặt nhiều nhất
 * - Thống kê khung giờ vàng
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
    void onDateRangeChanged(const QDate &from, const QDate &to);

private:
    void setupUI();
    void createSummaryCards();
    void createCharts();
    void createRankingTable();
    void updateSummaryCards();
    void updateDonutChart();
    void updateBarChart();
    void updatePeakHoursChart();
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
    SummaryCard *m_totalBookingCard;
    SummaryCard *m_completedCard;
    SummaryCard *m_cancelledCard;
    SummaryCard *m_goldenHourCard;

    // Charts
    QHBoxLayout *m_chartsRow;
    QHBoxLayout *m_chartsRow2;
    ChartContainer *m_donutChartContainer;
    ChartContainer *m_barChartContainer;
    ChartContainer *m_peakHoursChartContainer;

    // Ranking table
    RankingTable *m_topFieldsTable;

    // Data
    HeThongQuanLy *m_heThong;
    ThongKeBooking *m_thongKe;
    QDate m_fromDate;
    QDate m_toDate;
};

#endif // BOOKINGFIELDTAB_H
