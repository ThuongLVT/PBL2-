#include "BookingFieldTab.h"
#include "../../../Core/QuanLy/HeThongQuanLy.h"
#include "../../../Core/Models/San.h"
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

// ========== CONSTRUCTOR ==========

BookingFieldTab::BookingFieldTab(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(nullptr),
      m_scrollArea(nullptr),
      m_contentWidget(nullptr),
      m_contentLayout(nullptr),
      m_datePicker(nullptr),
      m_cardsLayout(nullptr),
      m_totalBookingCard(nullptr),
      m_completedCard(nullptr),
      m_cancelledCard(nullptr),
      m_goldenHourCard(nullptr),
      m_chartsRow(nullptr),
      m_donutChartContainer(nullptr),
      m_barChartContainer(nullptr),
      m_topFieldsTable(nullptr),
      m_heThong(nullptr),
      m_thongKe(nullptr)
{
    // Default date range: this month
    m_toDate = QDate::currentDate();
    m_fromDate = QDate(m_toDate.year(), m_toDate.month(), 1);

    setupUI();
}

BookingFieldTab::~BookingFieldTab()
{
    if (m_thongKe)
    {
        delete m_thongKe;
        m_thongKe = nullptr;
    }
}

// ========== PUBLIC METHODS ==========

void BookingFieldTab::setHeThong(HeThongQuanLy *ht)
{
    m_heThong = ht;
    refreshData();
}

void BookingFieldTab::refreshData()
{
    if (!m_heThong)
        return;

    // Delete old statistics
    if (m_thongKe)
    {
        delete m_thongKe;
        m_thongKe = nullptr;
    }

    // Create new statistics with date range
    NgayThang tuNgay(m_fromDate.day(), m_fromDate.month(), m_fromDate.year());
    NgayThang denNgay(m_toDate.day(), m_toDate.month(), m_toDate.year());

    m_thongKe = new ThongKeBooking(tuNgay, denNgay, m_heThong);

    // Update UI
    updateSummaryCards();
    updateDonutChart();
    updateBarChart();
    updateRankingTable();
}

// ========== SLOTS ==========

void BookingFieldTab::onDateRangeChanged(const QDate &from, const QDate &to)
{
    m_fromDate = from;
    m_toDate = to;
    refreshData();
}

// ========== SETUP UI ==========

void BookingFieldTab::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // Scroll area for content
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setStyleSheet("QScrollArea { background-color: #f8fafc; border: none; }");

    m_contentWidget = new QWidget();
    m_contentLayout = new QVBoxLayout(m_contentWidget);
    m_contentLayout->setContentsMargins(24, 24, 24, 24);
    m_contentLayout->setSpacing(24);

    // Date picker
    m_datePicker = new DateRangePicker(this);
    connect(m_datePicker, &DateRangePicker::dateRangeChanged,
            this, &BookingFieldTab::onDateRangeChanged);
    m_contentLayout->addWidget(m_datePicker);

    // Summary cards
    createSummaryCards();

    // Charts
    createCharts();

    // Ranking table
    createRankingTable();

    m_contentLayout->addStretch();

    m_scrollArea->setWidget(m_contentWidget);
    m_mainLayout->addWidget(m_scrollArea);
}

void BookingFieldTab::createSummaryCards()
{
    m_cardsLayout = new QHBoxLayout();
    m_cardsLayout->setSpacing(16);

    // Total Booking Card
    m_totalBookingCard = new SummaryCard("Tổng Booking", "📅", this);
    m_totalBookingCard->setCardStyle(SummaryCard::Primary);
    m_totalBookingCard->setValue("0");

    // Completed Card
    m_completedCard = new SummaryCard("Hoàn Thành", "✅", this);
    m_completedCard->setCardStyle(SummaryCard::Success);
    m_completedCard->setValue("0");

    // Cancelled Card
    m_cancelledCard = new SummaryCard("Đã Hủy", "❌", this);
    m_cancelledCard->setCardStyle(SummaryCard::Danger);
    m_cancelledCard->setValue("0");

    // Golden Hour Card
    m_goldenHourCard = new SummaryCard("Giờ Vàng (18-21h)", "🌟", this);
    m_goldenHourCard->setCardStyle(SummaryCard::Warning);
    m_goldenHourCard->setValue("0");

    m_cardsLayout->addWidget(m_totalBookingCard);
    m_cardsLayout->addWidget(m_completedCard);
    m_cardsLayout->addWidget(m_cancelledCard);
    m_cardsLayout->addWidget(m_goldenHourCard);

    m_contentLayout->addLayout(m_cardsLayout);
}

void BookingFieldTab::createCharts()
{
    m_chartsRow = new QHBoxLayout();
    m_chartsRow->setSpacing(16);

    m_donutChartContainer = new ChartContainer("Tỷ Lệ Loại Sân", this);
    m_donutChartContainer->setMinimumChartHeight(280);

    m_barChartContainer = new ChartContainer("Booking Theo Loại Sân", this);
    m_barChartContainer->setMinimumChartHeight(280);

    m_chartsRow->addWidget(m_donutChartContainer, 1);
    m_chartsRow->addWidget(m_barChartContainer, 1);

    m_contentLayout->addLayout(m_chartsRow);
}

void BookingFieldTab::createRankingTable()
{
    m_topFieldsTable = new RankingTable("🏆 Top Sân Được Đặt Nhiều Nhất", this);

    QList<RankingTable::ColumnConfig> columns;
    columns.append(RankingTable::ColumnConfig("Tên Sân", RankingTable::Text, 150));
    columns.append(RankingTable::ColumnConfig("Loại", RankingTable::Text, 100));
    columns.append(RankingTable::ColumnConfig("Lượt Đặt", RankingTable::Number, 100, true, Qt::AlignCenter));
    columns.append(RankingTable::ColumnConfig("Doanh Thu", RankingTable::Currency, -1));

    m_topFieldsTable->setColumns(columns);
    m_topFieldsTable->setMaxRows(5);
    m_topFieldsTable->setMinimumHeight(350);

    m_contentLayout->addWidget(m_topFieldsTable);
}

// ========== UPDATE METHODS ==========

void BookingFieldTab::updateSummaryCards()
{
    if (!m_thongKe)
        return;

    // Total Booking
    int totalBooking = m_thongKe->getTongSoBooking();
    m_totalBookingCard->setValueInt(totalBooking);

    // Completed
    int completed = m_thongKe->getSoBookingHoanThanh();
    double completedRate = m_thongKe->getTyLeHoanThanh();
    m_completedCard->setValue(QString("%1 (%2%)").arg(completed).arg(completedRate, 0, 'f', 1));

    // Cancelled
    int cancelled = m_thongKe->getSoBookingHuy();
    double cancelledRate = m_thongKe->getTyLeHuy();
    m_cancelledCard->setValue(QString("%1 (%2%)").arg(cancelled).arg(cancelledRate, 0, 'f', 1));

    // Golden Hour
    int goldenHour = m_thongKe->getSoBookingGioVang();
    int total = m_thongKe->getTongSoBooking();
    double goldenRate = total > 0 ? (goldenHour * 100.0 / total) : 0;
    m_goldenHourCard->setValue(QString("%1 (%2%)").arg(goldenHour).arg(goldenRate, 0, 'f', 1));
}

void BookingFieldTab::updateDonutChart()
{
    if (!m_thongKe)
        return;

    QChart *chart = new QChart();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.5); // Make it a donut

    int san5 = m_thongKe->getSoBookingSan5();
    int san7 = m_thongKe->getSoBookingSan7();

    if (san5 > 0)
    {
        QPieSlice *slice = series->append(QString("Sân 5 (%1)").arg(san5), san5);
        slice->setColor(QColor("#3b82f6"));
        slice->setLabelVisible(true);
    }
    if (san7 > 0)
    {
        QPieSlice *slice = series->append(QString("Sân 7 (%1)").arg(san7), san7);
        slice->setColor(QColor("#10b981"));
        slice->setLabelVisible(true);
    }

    if (series->count() == 0)
    {
        series->append("Chưa có dữ liệu", 1);
        series->slices().at(0)->setColor(QColor("#e2e8f0"));
    }

    chart->addSeries(series);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setBackgroundVisible(false);

    m_donutChartContainer->setChart(chart);
}

void BookingFieldTab::updateBarChart()
{
    if (!m_thongKe)
        return;

    QChart *chart = new QChart();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarSet *bookingSet = new QBarSet("Số booking");
    QBarSet *revenueSet = new QBarSet("Doanh thu (triệu)");

    bookingSet->setColor(QColor("#3b82f6"));
    revenueSet->setColor(QColor("#10b981"));

    *bookingSet << m_thongKe->getSoBookingSan5() << m_thongKe->getSoBookingSan7();
    *revenueSet << (m_thongKe->getDoanhThuSan5() / 1000000.0)
                << (m_thongKe->getDoanhThuSan7() / 1000000.0);

    QBarSeries *series = new QBarSeries();
    series->append(bookingSet);
    series->append(revenueSet);

    chart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(QStringList() << "Sân 5 người" << "Sân 7 người");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setBackgroundVisible(false);

    m_barChartContainer->setChart(chart);
}

void BookingFieldTab::updateRankingTable()
{
    if (!m_thongKe)
        return;

    m_topFieldsTable->clearData();

    const MangDong<ThongTinSanTop> &topSan = m_thongKe->getTopSan();

    for (int i = 0; i < topSan.size(); i++)
    {
        const ThongTinSanTop &item = topSan[i];
        if (item.san)
        {
            QStringList row;
            row << QString::fromStdString(item.san->getTenSan());
            row << (item.san->layLoaiSan() == LoaiSan::SAN_5 ? "5 người" : "7 người");
            row << QString::number(item.soLuotDat);
            row << QString::number(item.doanhThu);
            m_topFieldsTable->addRow(row);
        }
    }
}
