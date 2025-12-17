#include "CustomerTab.h"
#include "../../Components/MonthYearFilter.h"
#include "../../../Core/QuanLy/HeThongQuanLy.h"
#include "../../../Core/Models/KhachHang.h"
#include <QDate>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QPdfWriter>
#include <QPainter>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>

// ========== CONSTRUCTOR ==========

CustomerTab::CustomerTab(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(nullptr),
      m_scrollArea(nullptr),
      m_contentWidget(nullptr),
      m_contentLayout(nullptr),
      m_filter(nullptr),
      m_cardsLayout(nullptr),
      m_totalCustomerCard(nullptr),
      m_newCustomerCard(nullptr),
      m_returningCard(nullptr),
      m_tablesRow(nullptr),
      m_topCustomersTable(nullptr),
      m_membershipTable(nullptr),
      m_heThong(nullptr),
      m_thongKe(nullptr)
{
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

    // Get date range from filter component
    QDate fromDate = m_filter->getFromDate();
    QDate toDate = m_filter->getToDate();

    // Create new statistics with date range
    NgayThang tuNgay(fromDate.day(), fromDate.month(), fromDate.year());
    NgayThang denNgay(toDate.day(), toDate.month(), toDate.year());

    m_thongKe = new ThongKeKhachHang(tuNgay, denNgay, m_heThong);

    // Update UI
    updateSummaryCards();
    updateRankingTable();
    updateMembershipTable();
}

// ========== SLOTS ==========

void CustomerTab::onFilterChanged(int month, int year, MonthYearFilter::FilterMode mode)
{
    Q_UNUSED(month);
    Q_UNUSED(year);
    Q_UNUSED(mode);
    refreshData();
}

void CustomerTab::onExportPdf()
{
    if (!m_thongKe)
    {
        QMessageBox::warning(this, "Lỗi", "Không có dữ liệu để xuất!");
        return;
    }

    // Get save path
    QString defaultName = QString("ThongKe_KhachHang_%1.pdf")
                              .arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));

    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Xuất báo cáo PDF",
        QDir::homePath() + "/" + defaultName,
        "PDF Files (*.pdf)");

    if (filePath.isEmpty())
        return;

    // Create PDF
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setPageMargins(QMarginsF(20, 20, 20, 20));
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);
    int pageWidth = pdfWriter.width();
    int pageHeight = pdfWriter.height();

    // Fonts
    QFont titleFont("Arial", 16, QFont::Bold);
    QFont headerFont("Arial", 11, QFont::Bold);
    QFont normalFont("Arial", 9);

    int y = 100;
    int lineHeight = 120;

    // Title
    painter.setFont(titleFont);
    painter.drawText(0, y, pageWidth, 200, Qt::AlignCenter, "BÁO CÁO THỐNG KÊ KHÁCH HÀNG");
    y += 250;

    // Period
    int selectedMonth = m_filter->selectedMonth();
    int selectedYear = m_filter->selectedYear();
    bool isMonthMode = (m_filter->filterMode() == MonthYearFilter::ByMonth);

    painter.setFont(normalFont);
    QString period = isMonthMode
                         ? QString("Tháng %1/%2").arg(selectedMonth).arg(selectedYear)
                         : QString("Năm %1").arg(selectedYear);
    painter.drawText(0, y, pageWidth, 150, Qt::AlignCenter, period);
    y += 300;

    // Summary section
    painter.setFont(headerFont);
    painter.drawText(100, y, "TỔNG KẾT");
    y += lineHeight + 50;

    painter.setFont(normalFont);
    auto formatMoney = [](double val) -> QString
    {
        return QString("%L1 VND").arg(val, 0, 'f', 0);
    };

    QStringList summaryLabels = {"Tổng khách hàng:", "Khách mới:", "Khách cũ:", "Chi tiêu trung bình:"};
    QStringList summaryValues = {
        QString::number(m_thongKe->getTongSoKhachHang()),
        QString::number(m_thongKe->getSoKhachHangMoi()),
        QString::number(m_thongKe->getSoKhachHangQuayLai()),
        formatMoney(m_thongKe->getChiTieuTrungBinh())};

    for (int i = 0; i < summaryLabels.size(); i++)
    {
        painter.drawText(100, y, 800, lineHeight, Qt::AlignLeft, summaryLabels[i]);
        painter.drawText(900, y, 600, lineHeight, Qt::AlignLeft, summaryValues[i]);
        y += lineHeight;
    }
    y += 100;

    // Top customers section
    painter.setFont(headerFont);
    painter.drawText(100, y, "TOP KHÁCH HÀNG CHI TIÊU CAO NHẤT");
    y += lineHeight + 50;

    // Table header
    painter.setFont(QFont("Arial", 9, QFont::Bold));
    int col1 = 100, col2 = 600, col3 = 1100, col4 = 1400, col5 = 1700;
    painter.drawText(col1, y, 500, lineHeight, Qt::AlignLeft, "STT");
    painter.drawText(col2, y, 500, lineHeight, Qt::AlignLeft, "Tên khách hàng");
    painter.drawText(col3, y, 300, lineHeight, Qt::AlignLeft, "Hạng");
    painter.drawText(col4, y, 300, lineHeight, Qt::AlignCenter, "Số đơn");
    painter.drawText(col5, y, 500, lineHeight, Qt::AlignRight, "Tổng chi tiêu");
    y += lineHeight;

    // Draw line
    painter.drawLine(100, y, pageWidth - 100, y);
    y += 30;

    // Table data
    painter.setFont(normalFont);
    QString hangNames[] = {"Thường", "Đồng", "Bạc", "Vàng", "Kim Cương"};
    const MangDong<ThongTinKhachHangTop> &topKH = m_thongKe->getTopKhachHang();

    for (int i = 0; i < topKH.size() && i < 10; i++)
    {
        const ThongTinKhachHangTop &item = topKH[i];
        if (item.khachHang)
        {
            int hangIdx = static_cast<int>(item.khachHang->layHang());
            painter.drawText(col1, y, 500, lineHeight, Qt::AlignLeft, QString::number(i + 1));
            painter.drawText(col2, y, 500, lineHeight, Qt::AlignLeft,
                             QString::fromStdString(item.khachHang->getHoTen()));
            painter.drawText(col3, y, 300, lineHeight, Qt::AlignLeft, hangNames[hangIdx]);
            painter.drawText(col4, y, 300, lineHeight, Qt::AlignCenter, QString::number(item.soDonDat));
            painter.drawText(col5, y, 500, lineHeight, Qt::AlignRight, formatMoney(item.tongChiTieu));
            y += lineHeight;
        }
    }

    painter.end();

    QMessageBox::information(this, "Thành công",
                             QString("Đã xuất báo cáo:\n%1").arg(filePath));

    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
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

    // Filter bar using reusable component
    m_filter = new MonthYearFilter(this, true); // true = show export button
    connect(m_filter, &MonthYearFilter::filterChanged,
            this, &CustomerTab::onFilterChanged);
    connect(m_filter, &MonthYearFilter::exportPdfClicked,
            this, &CustomerTab::onExportPdf);
    m_contentLayout->addWidget(m_filter);

    // Summary cards
    createSummaryCards();

    // Tables row (side by side)
    createTablesRow();

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
    m_totalCustomerCard->showTrend(false);

    // New Customer Card
    m_newCustomerCard = new SummaryCard("Khách Mới", "🆕", this);
    m_newCustomerCard->setCardStyle(SummaryCard::Success);
    m_newCustomerCard->setValue("0");
    m_newCustomerCard->showTrend(false);

    // Returning Customer Card
    m_returningCard = new SummaryCard("Khách Cũ", "🔄", this);
    m_returningCard->setCardStyle(SummaryCard::Purple);
    m_returningCard->setValue("0");
    m_returningCard->showTrend(false);

    m_cardsLayout->addWidget(m_totalCustomerCard);
    m_cardsLayout->addWidget(m_newCustomerCard);
    m_cardsLayout->addWidget(m_returningCard);

    m_contentLayout->addLayout(m_cardsLayout);
}

void CustomerTab::createTablesRow()
{
    m_tablesRow = new QHBoxLayout();
    m_tablesRow->setSpacing(16);

    // Left table: Top Customers
    m_topCustomersTable = new RankingTable("🏆 Top Khách Hàng Chi Tiêu Cao Nhất", this);

    QList<RankingTable::ColumnConfig> topColumns;
    topColumns.append(RankingTable::ColumnConfig("Tên KH", RankingTable::Text, 220));
    topColumns.append(RankingTable::ColumnConfig("Hạng", RankingTable::Text, 130));
    topColumns.append(RankingTable::ColumnConfig("Số Lần Đặt", RankingTable::Number, 100, true, Qt::AlignCenter));
    topColumns.append(RankingTable::ColumnConfig("Tổng Chi Tiêu", RankingTable::Currency, -1, true, Qt::AlignRight | Qt::AlignVCenter));

    m_topCustomersTable->setColumns(topColumns);
    m_topCustomersTable->setMaxRows(5);
    m_topCustomersTable->setScrollable(false);
    m_topCustomersTable->setFixedHeight(380);

    // Right table: Membership Distribution
    m_membershipTable = new RankingTable("🎖️ Phân Bổ Hạng Khách Hàng", this);
    m_membershipTable->setShowRankColumn(false);
    m_membershipTable->setShowMedals(false);
    m_membershipTable->setScrollable(false);

    QList<RankingTable::ColumnConfig> memberColumns;
    memberColumns.append(RankingTable::ColumnConfig("Hạng", RankingTable::Text, 200));
    memberColumns.append(RankingTable::ColumnConfig("Số KH", RankingTable::Number, 80, true, Qt::AlignCenter));
    memberColumns.append(RankingTable::ColumnConfig("Tỷ Lệ", RankingTable::Percentage, 100, true, Qt::AlignCenter));
    memberColumns.append(RankingTable::ColumnConfig("Chi Tiêu TB", RankingTable::Currency, -1, true, Qt::AlignRight | Qt::AlignVCenter));

    m_membershipTable->setColumns(memberColumns);
    m_membershipTable->setScrollable(false);
    m_membershipTable->setFixedHeight(380);

    m_tablesRow->addWidget(m_topCustomersTable, 1);
    m_tablesRow->addWidget(m_membershipTable, 1);

    m_contentLayout->addLayout(m_tablesRow);
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

    // Returning Customers
    int returning = m_thongKe->getSoKhachHangQuayLai();
    m_returningCard->setValueInt(returning);
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
