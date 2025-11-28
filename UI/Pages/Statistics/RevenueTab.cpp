#include "RevenueTab.h"
#include "../../../Core/QuanLy/HeThongQuanLy.h"
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QDateTime>

// ========== CONSTRUCTOR ==========

RevenueTab::RevenueTab(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(nullptr),
      m_scrollArea(nullptr),
      m_contentWidget(nullptr),
      m_contentLayout(nullptr),
      m_datePicker(nullptr),
      m_cardsLayout(nullptr),
      m_totalRevenueCard(nullptr),
      m_fieldRevenueCard(nullptr),
      m_serviceRevenueCard(nullptr),
      m_avgRevenueCard(nullptr),
      m_chartsRow1(nullptr),
      m_chartsRow2(nullptr),
      m_lineChartContainer(nullptr),
      m_pieChartContainer(nullptr),
      m_barChartContainer(nullptr),
      m_heThong(nullptr),
      m_thongKe(nullptr)
{
    // Default date range: this month
    m_toDate = QDate::currentDate();
    m_fromDate = QDate(m_toDate.year(), m_toDate.month(), 1);

    setupUI();
}

RevenueTab::~RevenueTab()
{
    if (m_thongKe)
    {
        delete m_thongKe;
        m_thongKe = nullptr;
    }
}

// ========== PUBLIC METHODS ==========

void RevenueTab::setHeThong(HeThongQuanLy *ht)
{
    m_heThong = ht;
    refreshData();
}

void RevenueTab::refreshData()
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

    m_thongKe = new ThongKeDoanhThu(tuNgay, denNgay, m_heThong);

    // Update UI
    updateSummaryCards();
    updateLineChart();
    updatePieChart();
    updateBarChart();
}

// ========== SLOTS ==========

void RevenueTab::onDateRangeChanged(const QDate &from, const QDate &to)
{
    m_fromDate = from;
    m_toDate = to;
    refreshData();
}

// ========== SETUP UI ==========

void RevenueTab::setupUI()
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
            this, &RevenueTab::onDateRangeChanged);
    m_contentLayout->addWidget(m_datePicker);

    // Summary cards
    createSummaryCards();

    // Charts
    createCharts();

    m_contentLayout->addStretch();

    m_scrollArea->setWidget(m_contentWidget);
    m_mainLayout->addWidget(m_scrollArea);
}

void RevenueTab::createSummaryCards()
{
    m_cardsLayout = new QHBoxLayout();
    m_cardsLayout->setSpacing(16);

    // Total Revenue Card
    m_totalRevenueCard = new SummaryCard("Tổng Doanh Thu", "💵", this);
    m_totalRevenueCard->setCardStyle(SummaryCard::Primary);
    m_totalRevenueCard->setValue("0 VND");

    // Field Revenue Card
    m_fieldRevenueCard = new SummaryCard("Doanh Thu Sân", "⚽", this);
    m_fieldRevenueCard->setCardStyle(SummaryCard::Success);
    m_fieldRevenueCard->setValue("0 VND");

    // Service Revenue Card
    m_serviceRevenueCard = new SummaryCard("Doanh Thu Dịch Vụ", "🛒", this);
    m_serviceRevenueCard->setCardStyle(SummaryCard::Purple);
    m_serviceRevenueCard->setValue("0 VND");

    // Average Revenue Card
    m_avgRevenueCard = new SummaryCard("Trung Bình/Ngày", "📊", this);
    m_avgRevenueCard->setCardStyle(SummaryCard::Info);
    m_avgRevenueCard->setValue("0 VND");

    m_cardsLayout->addWidget(m_totalRevenueCard);
    m_cardsLayout->addWidget(m_fieldRevenueCard);
    m_cardsLayout->addWidget(m_serviceRevenueCard);
    m_cardsLayout->addWidget(m_avgRevenueCard);

    m_contentLayout->addLayout(m_cardsLayout);
}

void RevenueTab::createCharts()
{
    // Row 1: Line chart (full width)
    m_chartsRow1 = new QHBoxLayout();
    m_chartsRow1->setSpacing(16);

    m_lineChartContainer = new ChartContainer("Doanh Thu Theo Thời Gian", this);
    m_lineChartContainer->setMinimumChartHeight(300);
    m_lineChartContainer->setMinimumWidth(600);
    m_chartsRow1->addWidget(m_lineChartContainer);

    m_contentLayout->addLayout(m_chartsRow1);

    // Row 2: Pie chart + Bar chart
    m_chartsRow2 = new QHBoxLayout();
    m_chartsRow2->setSpacing(16);

    m_pieChartContainer = new ChartContainer("Phân Bổ Nguồn Thu", this);
    m_pieChartContainer->setMinimumChartHeight(280);

    m_barChartContainer = new ChartContainer("So Sánh Doanh Thu", this);
    m_barChartContainer->setMinimumChartHeight(280);

    m_chartsRow2->addWidget(m_pieChartContainer, 1);
    m_chartsRow2->addWidget(m_barChartContainer, 1);

    m_contentLayout->addLayout(m_chartsRow2);
}

// ========== UPDATE METHODS ==========

void RevenueTab::updateSummaryCards()
{
    if (!m_thongKe)
        return;

    // Total Revenue
    double totalRevenue = m_thongKe->getTongDoanhThu();
    m_totalRevenueCard->setValue(SummaryCard::formatCurrency(totalRevenue));
    m_totalRevenueCard->setTrend(m_thongKe->getPhanTramTangGiam(),
                                 m_thongKe->getPhanTramTangGiam() >= 0 ? SummaryCard::Up : SummaryCard::Down);

    // Field Revenue
    double fieldRevenue = m_thongKe->getDoanhThuTienSan();
    m_fieldRevenueCard->setValue(SummaryCard::formatCurrency(fieldRevenue));
    m_fieldRevenueCard->setTrend(m_thongKe->getPhanTramTangSan(),
                                 m_thongKe->getPhanTramTangSan() >= 0 ? SummaryCard::Up : SummaryCard::Down);

    // Service Revenue
    double serviceRevenue = m_thongKe->getDoanhThuDichVu();
    m_serviceRevenueCard->setValue(SummaryCard::formatCurrency(serviceRevenue));
    m_serviceRevenueCard->setTrend(m_thongKe->getPhanTramTangDV(),
                                   m_thongKe->getPhanTramTangDV() >= 0 ? SummaryCard::Up : SummaryCard::Down);

    // Average Revenue
    double avgRevenue = m_thongKe->getDoanhThuTrungBinh();
    m_avgRevenueCard->setValue(SummaryCard::formatCurrency(avgRevenue));
}

void RevenueTab::updateLineChart()
{
    if (!m_thongKe)
        return;

    QChart *chart = new QChart();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QLineSeries *series = new QLineSeries();
    series->setName("Doanh thu");

    // Get daily revenue data
    const MangDong<DoanhThuTheoNgay> &dailyData = m_thongKe->getDoanhThuTheoNgayList();

    qint64 minTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    qint64 maxTime = 0;
    double maxValue = 0;

    for (int i = 0; i < dailyData.size(); i++)
    {
        const DoanhThuTheoNgay &item = dailyData[i];
        QDateTime dt(QDate(item.ngay.getNam(), item.ngay.getThang(), item.ngay.getNgay()), QTime(0, 0));
        qint64 msecs = dt.toMSecsSinceEpoch();

        series->append(msecs, item.doanhThu);

        if (msecs < minTime)
            minTime = msecs;
        if (msecs > maxTime)
            maxTime = msecs;
        if (item.doanhThu > maxValue)
            maxValue = item.doanhThu;
    }

    // Style the line
    QPen pen(QColor("#3b82f6"));
    pen.setWidth(3);
    series->setPen(pen);

    chart->addSeries(series);

    // X Axis (DateTime)
    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setFormat("dd/MM");
    axisX->setTitleText("Ngày");
    axisX->setLabelsAngle(-45);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Y Axis (Value)
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("VND");
    axisY->setLabelFormat("%.0f");
    axisY->setRange(0, maxValue * 1.1);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(false);
    chart->setBackgroundVisible(false);

    m_lineChartContainer->setChart(chart);
}

void RevenueTab::updatePieChart()
{
    if (!m_thongKe)
        return;

    QChart *chart = new QChart();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QPieSeries *series = new QPieSeries();

    double fieldRevenue = m_thongKe->getDoanhThuTienSan();
    double drinkRevenue = m_thongKe->getDoanhThuDoUong();
    double foodRevenue = m_thongKe->getDoanhThuDoAn();
    double equipRevenue = m_thongKe->getDoanhThuThietBi();

    if (fieldRevenue > 0)
    {
        QPieSlice *slice = series->append("Tiền sân", fieldRevenue);
        slice->setColor(QColor("#3b82f6"));
        slice->setLabelVisible(true);
    }
    if (drinkRevenue > 0)
    {
        QPieSlice *slice = series->append("Đồ uống", drinkRevenue);
        slice->setColor(QColor("#10b981"));
    }
    if (foodRevenue > 0)
    {
        QPieSlice *slice = series->append("Đồ ăn", foodRevenue);
        slice->setColor(QColor("#f59e0b"));
    }
    if (equipRevenue > 0)
    {
        QPieSlice *slice = series->append("Thiết bị", equipRevenue);
        slice->setColor(QColor("#8b5cf6"));
    }

    // If no data, show placeholder
    if (series->count() == 0)
    {
        series->append("Chưa có dữ liệu", 1);
        series->slices().at(0)->setColor(QColor("#e2e8f0"));
    }

    chart->addSeries(series);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setBackgroundVisible(false);

    m_pieChartContainer->setChart(chart);
}

void RevenueTab::updateBarChart()
{
    if (!m_thongKe)
        return;

    QChart *chart = new QChart();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarSet *currentSet = new QBarSet("Kỳ này");
    QBarSet *previousSet = new QBarSet("Kỳ trước");

    currentSet->setColor(QColor("#3b82f6"));
    previousSet->setColor(QColor("#94a3b8"));

    // Current period
    *currentSet << m_thongKe->getDoanhThuTienSan() << m_thongKe->getDoanhThuDichVu();

    // Previous period
    *previousSet << m_thongKe->getDoanhThuSanKyTruoc() << m_thongKe->getDoanhThuDVKyTruoc();

    QBarSeries *series = new QBarSeries();
    series->append(currentSet);
    series->append(previousSet);

    chart->addSeries(series);

    // X Axis (Categories)
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(QStringList() << "Tiền sân" << "Dịch vụ");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Y Axis (Value)
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("VND");
    axisY->setLabelFormat("%.0f");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setBackgroundVisible(false);

    m_barChartContainer->setChart(chart);
}
