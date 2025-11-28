#include "CustomerTab.h"
#include "../../../Core/QuanLy/HeThongQuanLy.h"
#include "../../../Core/Models/KhachHang.h"
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>

// ========== CONSTRUCTOR ==========

CustomerTab::CustomerTab(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(nullptr),
      m_scrollArea(nullptr),
      m_contentWidget(nullptr),
      m_contentLayout(nullptr),
      m_datePicker(nullptr),
      m_cardsLayout(nullptr),
      m_totalCustomerCard(nullptr),
      m_newCustomerCard(nullptr),
      m_returningCard(nullptr),
      m_avgSpendCard(nullptr),
      m_chartsRow(nullptr),
      m_donutChartContainer(nullptr),
      m_topCustomersTable(nullptr),
      m_membershipTable(nullptr),
      m_heThong(nullptr),
      m_thongKe(nullptr)
{
    // Default date range: this month
    m_toDate = QDate::currentDate();
    m_fromDate = QDate(m_toDate.year(), m_toDate.month(), 1);

    setupUI();
}

CustomerTab::~CustomerTab()
{
    if (m_thongKe)
    {
        delete m_thongKe;
        m_thongKe = nullptr;
    }
}

// ========== PUBLIC METHODS ==========

void CustomerTab::setHeThong(HeThongQuanLy *ht)
{
    m_heThong = ht;
    refreshData();
}

void CustomerTab::refreshData()
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

    m_thongKe = new ThongKeKhachHang(tuNgay, denNgay, m_heThong);

    // Update UI
    updateSummaryCards();
    updateDonutChart();
    updateRankingTable();
    updateMembershipTable();
}

// ========== SLOTS ==========

void CustomerTab::onDateRangeChanged(const QDate &from, const QDate &to)
{
    m_fromDate = from;
    m_toDate = to;
    refreshData();
}

// ========== SETUP UI ==========

void CustomerTab::setupUI()
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
            this, &CustomerTab::onDateRangeChanged);
    m_contentLayout->addWidget(m_datePicker);

    // Summary cards
    createSummaryCards();

    // Charts and tables row
    createCharts();

    // Ranking table
    createRankingTable();

    // Membership table
    createMembershipTable();

    m_contentLayout->addStretch();

    m_scrollArea->setWidget(m_contentWidget);
    m_mainLayout->addWidget(m_scrollArea);
}

void CustomerTab::createSummaryCards()
{
    m_cardsLayout = new QHBoxLayout();
    m_cardsLayout->setSpacing(16);

    // Total Customer Card
    m_totalCustomerCard = new SummaryCard("Tổng Khách Hàng", "👥", this);
    m_totalCustomerCard->setCardStyle(SummaryCard::Primary);
    m_totalCustomerCard->setValue("0");

    // New Customer Card
    m_newCustomerCard = new SummaryCard("Khách Mới", "🆕", this);
    m_newCustomerCard->setCardStyle(SummaryCard::Success);
    m_newCustomerCard->setValue("0");

    // Returning Customer Card
    m_returningCard = new SummaryCard("Khách Quay Lại", "🔄", this);
    m_returningCard->setCardStyle(SummaryCard::Info);
    m_returningCard->setValue("0");

    // Average Spend Card
    m_avgSpendCard = new SummaryCard("Chi Tiêu TB", "💰", this);
    m_avgSpendCard->setCardStyle(SummaryCard::Purple);
    m_avgSpendCard->setValue("0 VND");

    m_cardsLayout->addWidget(m_totalCustomerCard);
    m_cardsLayout->addWidget(m_newCustomerCard);
    m_cardsLayout->addWidget(m_returningCard);
    m_cardsLayout->addWidget(m_avgSpendCard);

    m_contentLayout->addLayout(m_cardsLayout);
}

void CustomerTab::createCharts()
{
    m_chartsRow = new QHBoxLayout();
    m_chartsRow->setSpacing(16);

    m_donutChartContainer = new ChartContainer("Phân Bổ Theo Hạng Thành Viên", this);
    m_donutChartContainer->setMinimumChartHeight(300);

    m_chartsRow->addWidget(m_donutChartContainer);

    m_contentLayout->addLayout(m_chartsRow);
}

void CustomerTab::createRankingTable()
{
    m_topCustomersTable = new RankingTable("🏆 Top Khách Hàng Chi Tiêu Cao Nhất", this);

    QList<RankingTable::ColumnConfig> columns;
    columns.append(RankingTable::ColumnConfig("Tên Khách Hàng", RankingTable::Text, 180));
    columns.append(RankingTable::ColumnConfig("Hạng", RankingTable::Text, 100));
    columns.append(RankingTable::ColumnConfig("Số Lần Đặt", RankingTable::Number, 100, true, Qt::AlignCenter));
    columns.append(RankingTable::ColumnConfig("Tổng Chi Tiêu", RankingTable::Currency, -1));

    m_topCustomersTable->setColumns(columns);
    m_topCustomersTable->setMaxRows(10);
    m_topCustomersTable->setMinimumHeight(400);

    m_contentLayout->addWidget(m_topCustomersTable);
}

void CustomerTab::createMembershipTable()
{
    m_membershipTable = new RankingTable("🎖️ Phân Bổ Hạng Khách Hàng", this);
    m_membershipTable->setShowRankColumn(false);
    m_membershipTable->setShowMedals(false);

    QList<RankingTable::ColumnConfig> columns;
    columns.append(RankingTable::ColumnConfig("Hạng", RankingTable::Text, 150));
    columns.append(RankingTable::ColumnConfig("Số KH", RankingTable::Number, 100, true, Qt::AlignCenter));
    columns.append(RankingTable::ColumnConfig("Tỷ Lệ", RankingTable::Percentage, 100, true, Qt::AlignCenter));
    columns.append(RankingTable::ColumnConfig("Chi Tiêu TB", RankingTable::Currency, -1));

    m_membershipTable->setColumns(columns);
    m_membershipTable->setMinimumHeight(300);

    m_contentLayout->addWidget(m_membershipTable);
}

// ========== UPDATE METHODS ==========

void CustomerTab::updateSummaryCards()
{
    if (!m_thongKe)
        return;

    // Total Customers
    int total = m_thongKe->getTongSoKhachHang();
    m_totalCustomerCard->setValueInt(total);

    // New Customers
    int newCustomers = m_thongKe->getSoKhachHangMoi();
    m_newCustomerCard->setValueInt(newCustomers);
    m_newCustomerCard->setTrend(m_thongKe->getPhanTramTangKhachMoi(),
                                m_thongKe->getPhanTramTangKhachMoi() >= 0 ? SummaryCard::Up : SummaryCard::Down);

    // Returning Customers
    int returning = m_thongKe->getSoKhachHangQuayLai();
    double returningRate = total > 0 ? (returning * 100.0 / total) : 0;
    m_returningCard->setValue(QString("%1 (%2%)").arg(returning).arg(returningRate, 0, 'f', 1));

    // Average Spend
    double avgSpend = m_thongKe->getChiTieuTrungBinh();
    m_avgSpendCard->setValue(SummaryCard::formatCurrency(avgSpend));
}

void CustomerTab::updateDonutChart()
{
    if (!m_thongKe)
        return;

    QChart *chart = new QChart();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.5);

    const MangDong<ThongKeHangKhachHang> &hangData = m_thongKe->getThongKeTheoHang();

    QColor colors[] = {
        QColor("#94a3b8"), // Thường - Gray
        QColor("#cd7f32"), // Đồng - Bronze
        QColor("#c0c0c0"), // Bạc - Silver
        QColor("#ffd700"), // Vàng - Gold
        QColor("#b9f2ff")  // Kim Cương - Diamond
    };

    QString hangNames[] = {"⭐ Thường", "🥉 Đồng", "🥈 Bạc", "🥇 Vàng", "💎 Kim Cương"};

    for (int i = 0; i < hangData.size(); i++)
    {
        const ThongKeHangKhachHang &item = hangData[i];
        if (item.soLuong > 0)
        {
            int idx = static_cast<int>(item.hang);
            QPieSlice *slice = series->append(
                QString("%1 (%2)").arg(hangNames[idx]).arg(item.soLuong),
                item.soLuong);
            slice->setColor(colors[idx]);
            slice->setLabelVisible(true);
        }
    }

    if (series->count() == 0)
    {
        series->append("Chưa có dữ liệu", 1);
        series->slices().at(0)->setColor(QColor("#e2e8f0"));
    }

    chart->addSeries(series);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setBackgroundVisible(false);

    m_donutChartContainer->setChart(chart);
}

void CustomerTab::updateRankingTable()
{
    if (!m_thongKe)
        return;

    m_topCustomersTable->clearData();

    const MangDong<ThongTinKhachHangTop> &topKH = m_thongKe->getTopKhachHang();

    QString hangNames[] = {"⭐ Thường", "🥉 Đồng", "🥈 Bạc", "🥇 Vàng", "💎 Kim Cương"};

    for (int i = 0; i < topKH.size(); i++)
    {
        const ThongTinKhachHangTop &item = topKH[i];
        if (item.khachHang)
        {
            int hangIdx = static_cast<int>(item.khachHang->layHang());
            QStringList row;
            row << QString::fromStdString(item.khachHang->getHoTen());
            row << hangNames[hangIdx];
            row << QString::number(item.soDonDat);
            row << QString::number(item.tongChiTieu);
            m_topCustomersTable->addRow(row);
        }
    }
}

void CustomerTab::updateMembershipTable()
{
    if (!m_thongKe)
        return;

    m_membershipTable->clearData();

    const MangDong<ThongKeHangKhachHang> &hangData = m_thongKe->getThongKeTheoHang();

    QString hangNames[] = {"⭐ Thường", "🥉 Đồng", "🥈 Bạc", "🥇 Vàng", "💎 Kim Cương"};

    for (int i = 0; i < hangData.size(); i++)
    {
        const ThongKeHangKhachHang &item = hangData[i];
        int idx = static_cast<int>(item.hang);

        QStringList row;
        row << hangNames[idx];
        row << QString::number(item.soLuong);
        row << QString::number(item.tyLe);
        row << QString::number(item.chiTieuTrungBinh);
        m_membershipTable->addRow(row);
    }
}
