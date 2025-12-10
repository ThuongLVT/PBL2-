#include "ServiceTab.h"
#include "../../../Core/QuanLy/HeThongQuanLy.h"
#include "../../../Core/Models/DichVu.h"
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>

// ========== CONSTRUCTOR ==========

ServiceTab::ServiceTab(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(nullptr),
      m_scrollArea(nullptr),
      m_contentWidget(nullptr),
      m_contentLayout(nullptr),
      m_datePicker(nullptr),
      m_cardsLayout(nullptr),
      m_totalSalesCard(nullptr),
      m_drinkCard(nullptr),
      m_foodCard(nullptr),
      m_equipmentCard(nullptr),
      m_chartsRow(nullptr),
      m_pieChartContainer(nullptr),
      m_topServicesTable(nullptr),
      m_heThong(nullptr),
      m_thongKe(nullptr)
{
    // Default date range: this month
    m_toDate = QDate::currentDate();
    m_fromDate = QDate(m_toDate.year(), m_toDate.month(), 1);

    setupUI();
}

ServiceTab::~ServiceTab()
{
    if (m_thongKe)
    {
        delete m_thongKe;
        m_thongKe = nullptr;
    }
}

// ========== PUBLIC METHODS ==========

void ServiceTab::setHeThong(HeThongQuanLy *ht)
{
    m_heThong = ht;
    refreshData();
}

void ServiceTab::refreshData()
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

    m_thongKe = new ThongKeDichVu(tuNgay, denNgay, m_heThong);

    // Update UI
    updateSummaryCards();
    updatePieChart();
    updateRankingTable();
}

// ========== SLOTS ==========

void ServiceTab::onDateRangeChanged(const QDate &from, const QDate &to)
{
    m_fromDate = from;
    m_toDate = to;
    refreshData();
}

// ========== SETUP UI ==========

void ServiceTab::setupUI()
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
            this, &ServiceTab::onDateRangeChanged);
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

void ServiceTab::createSummaryCards()
{
    m_cardsLayout = new QHBoxLayout();
    m_cardsLayout->setSpacing(16);

    // Total Sales Card
    m_totalSalesCard = new SummaryCard("Tổng Doanh Thu DV", "🛒", this);
    m_totalSalesCard->setCardStyle(SummaryCard::Primary);
    m_totalSalesCard->setValue("0 VND");

    // Drink Card
    m_drinkCard = new SummaryCard("Đồ Uống", "🥤", this);
    m_drinkCard->setCardStyle(SummaryCard::Info);
    m_drinkCard->setValue("0 VND");

    // Food Card
    m_foodCard = new SummaryCard("Đồ Ăn", "🍔", this);
    m_foodCard->setCardStyle(SummaryCard::Warning);
    m_foodCard->setValue("0 VND");

    // Equipment Card
    m_equipmentCard = new SummaryCard("Thiết Bị", "👕", this);
    m_equipmentCard->setCardStyle(SummaryCard::Purple);
    m_equipmentCard->setValue("0 VND");

    m_cardsLayout->addWidget(m_totalSalesCard);
    m_cardsLayout->addWidget(m_drinkCard);
    m_cardsLayout->addWidget(m_foodCard);
    m_cardsLayout->addWidget(m_equipmentCard);

    m_contentLayout->addLayout(m_cardsLayout);
}

void ServiceTab::createCharts()
{
    m_chartsRow = new QHBoxLayout();
    m_chartsRow->setSpacing(16);

    m_pieChartContainer = new ChartContainer("Phân Bổ Doanh Thu Theo Loại", this);
    m_pieChartContainer->setMinimumChartHeight(300);

    m_chartsRow->addWidget(m_pieChartContainer);

    m_contentLayout->addLayout(m_chartsRow);
}

void ServiceTab::createRankingTable()
{
    m_topServicesTable = new RankingTable("🏆 Top Dịch Vụ Bán Chạy Nhất", this);

    QList<RankingTable::ColumnConfig> columns;
    columns.append(RankingTable::ColumnConfig("Tên Dịch Vụ", RankingTable::Text, 200));
    columns.append(RankingTable::ColumnConfig("Loại", RankingTable::Text, 100));
    columns.append(RankingTable::ColumnConfig("SL Bán", RankingTable::Number, 100, true, Qt::AlignCenter));
    columns.append(RankingTable::ColumnConfig("Doanh Thu", RankingTable::Currency, -1));

    m_topServicesTable->setColumns(columns);
    m_topServicesTable->setMaxRows(10);
    m_topServicesTable->setMinimumHeight(400);

    m_contentLayout->addWidget(m_topServicesTable);
}

// ========== UPDATE METHODS ==========

void ServiceTab::updateSummaryCards()
{
    if (!m_thongKe)
        return;

    // Total Revenue
    double total = m_thongKe->getTongDoanhThuDichVu();
    m_totalSalesCard->setValue(SummaryCard::formatCurrency(total));

    // Drink Revenue
    double drink = m_thongKe->getDoanhThuDoUong();
    m_drinkCard->setValue(SummaryCard::formatCurrency(drink));

    // Food Revenue
    double food = m_thongKe->getDoanhThuDoAn();
    m_foodCard->setValue(SummaryCard::formatCurrency(food));

    // Equipment Revenue
    double equip = m_thongKe->getDoanhThuThietBi();
    m_equipmentCard->setValue(SummaryCard::formatCurrency(equip));
}

void ServiceTab::updatePieChart()
{
    if (!m_thongKe)
        return;

    QChart *chart = new QChart();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QPieSeries *series = new QPieSeries();

    double drink = m_thongKe->getDoanhThuDoUong();
    double food = m_thongKe->getDoanhThuDoAn();
    double equip = m_thongKe->getDoanhThuThietBi();

    if (drink > 0)
    {
        QPieSlice *slice = series->append("🥤 Đồ uống", drink);
        slice->setColor(QColor("#06b6d4"));
        slice->setLabelVisible(true);
    }
    if (food > 0)
    {
        QPieSlice *slice = series->append("🍔 Đồ ăn", food);
        slice->setColor(QColor("#f59e0b"));
        slice->setLabelVisible(true);
    }
    if (equip > 0)
    {
        QPieSlice *slice = series->append("👕 Thiết bị", equip);
        slice->setColor(QColor("#8b5cf6"));
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

    m_pieChartContainer->setChart(chart);
}

void ServiceTab::updateRankingTable()
{
    if (!m_thongKe)
        return;

    m_topServicesTable->clearData();

    const MangDong<ThongTinDichVuTop> &topDV = m_thongKe->getTopDichVu();

    QString loaiNames[] = {"🥤 Đồ uống", "🍔 Đồ ăn", "👕 Thiết bị"};

    for (int i = 0; i < topDV.size(); i++)
    {
        const ThongTinDichVuTop &item = topDV[i];
        if (item.dichVu)
        {
            int loaiIdx = static_cast<int>(item.dichVu->layLoaiDichVu());
            QStringList row;
            row << QString::fromStdString(item.dichVu->layTenDichVu());
            row << loaiNames[loaiIdx];
            row << QString::number(item.soLuongBan);
            row << QString::number(item.doanhThu);
            m_topServicesTable->addRow(row);
        }
    }
}
