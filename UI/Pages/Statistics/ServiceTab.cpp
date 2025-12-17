#include "ServiceTab.h"
#include "../../Components/MonthYearFilter.h"
#include "../../../Core/QuanLy/HeThongQuanLy.h"
#include "../../../Core/Models/DichVu.h"
#include <QDate>
#include <QFileDialog>
#include <QMessageBox>
#include <QPdfWriter>
#include <QPainter>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QDateTime>

// ========== CONSTRUCTOR ==========

ServiceTab::ServiceTab(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(nullptr),
      m_scrollArea(nullptr),
      m_contentWidget(nullptr),
      m_contentLayout(nullptr),
      m_filter(nullptr),
      m_cardsLayout(nullptr),
      m_totalSalesCard(nullptr),
      m_drinkCard(nullptr),
      m_foodCard(nullptr),
      m_equipmentCard(nullptr),
      m_topServicesTable(nullptr),
      m_heThong(nullptr),
      m_thongKe(nullptr)
{
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

    // Get date range from filter component
    QDate fromDate = m_filter->getFromDate();
    QDate toDate = m_filter->getToDate();

    // Create new statistics with date range
    NgayThang tuNgay(fromDate.day(), fromDate.month(), fromDate.year());
    NgayThang denNgay(toDate.day(), toDate.month(), toDate.year());

    m_thongKe = new ThongKeDichVu(tuNgay, denNgay, m_heThong);

    // Update UI
    updateSummaryCards();
    updateRankingTable();
}

// ========== SLOTS ==========

void ServiceTab::onFilterChanged(int month, int year, MonthYearFilter::FilterMode mode)
{
    Q_UNUSED(month);
    Q_UNUSED(year);
    Q_UNUSED(mode);
    refreshData();
}

void ServiceTab::onExportPdf()
{
    if (!m_thongKe)
    {
        QMessageBox::warning(this, "Lỗi", "Không có dữ liệu để xuất!");
        return;
    }

    // Get save path
    QString defaultName = QString("ThongKe_DichVu_%1.pdf")
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

    // Fonts
    QFont titleFont("Arial", 16, QFont::Bold);
    QFont headerFont("Arial", 11, QFont::Bold);
    QFont normalFont("Arial", 9);

    int y = 100;
    int lineHeight = 120;

    // Title
    painter.setFont(titleFont);
    painter.drawText(0, y, pageWidth, 200, Qt::AlignCenter, "BÁO CÁO THỐNG KÊ DỊCH VỤ");
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
    painter.drawText(100, y, "TỔNG KẾT DOANH THU");
    y += lineHeight + 50;

    painter.setFont(normalFont);
    auto formatMoney = [](double val) -> QString
    {
        return QString("%L1 VND").arg(val, 0, 'f', 0);
    };

    QStringList summaryLabels = {"Tổng doanh thu dịch vụ:", "Đồ uống:", "Đồ ăn:", "Thiết bị:"};
    QStringList summaryValues = {
        formatMoney(m_thongKe->getTongDoanhThuDichVu()),
        formatMoney(m_thongKe->getDoanhThuDoUong()),
        formatMoney(m_thongKe->getDoanhThuDoAn()),
        formatMoney(m_thongKe->getDoanhThuThietBi())};

    for (int i = 0; i < summaryLabels.size(); i++)
    {
        painter.drawText(100, y, 800, lineHeight, Qt::AlignLeft, summaryLabels[i]);
        painter.drawText(900, y, 600, lineHeight, Qt::AlignLeft, summaryValues[i]);
        y += lineHeight;
    }
    y += 100;

    // Top services section
    painter.setFont(headerFont);
    painter.drawText(100, y, "TOP DỊCH VỤ BÁN CHẠY NHẤT");
    y += lineHeight + 50;

    // Table header
    painter.setFont(QFont("Arial", 9, QFont::Bold));
    int col1 = 100, col2 = 300, col3 = 900, col4 = 1200, col5 = 1500;
    painter.drawText(col1, y, 200, lineHeight, Qt::AlignLeft, "STT");
    painter.drawText(col2, y, 600, lineHeight, Qt::AlignLeft, "Tên dịch vụ");
    painter.drawText(col3, y, 300, lineHeight, Qt::AlignLeft, "Loại");
    painter.drawText(col4, y, 300, lineHeight, Qt::AlignCenter, "SL bán");
    painter.drawText(col5, y, 500, lineHeight, Qt::AlignRight, "Doanh thu");
    y += lineHeight;

    // Draw line
    painter.drawLine(100, y, pageWidth - 100, y);
    y += 30;

    // Table data
    painter.setFont(normalFont);
    QString loaiNames[] = {"Đồ uống", "Đồ ăn", "Thiết bị"};
    const MangDong<ThongTinDichVuTop> &topDV = m_thongKe->getTopDichVu();

    for (int i = 0; i < topDV.size() && i < 10; i++)
    {
        const ThongTinDichVuTop &item = topDV[i];
        if (item.dichVu)
        {
            int loaiIdx = static_cast<int>(item.dichVu->layLoaiDichVu());
            painter.drawText(col1, y, 200, lineHeight, Qt::AlignLeft, QString::number(i + 1));
            painter.drawText(col2, y, 600, lineHeight, Qt::AlignLeft,
                             QString::fromStdString(item.dichVu->layTenDichVu()));
            painter.drawText(col3, y, 300, lineHeight, Qt::AlignLeft, loaiNames[loaiIdx]);
            painter.drawText(col4, y, 300, lineHeight, Qt::AlignCenter, QString::number(item.soLuongBan));
            painter.drawText(col5, y, 500, lineHeight, Qt::AlignRight, formatMoney(item.doanhThu));
            y += lineHeight;
        }
    }

    painter.end();

    QMessageBox::information(this, "Thành công",
                             QString("Đã xuất báo cáo:\n%1").arg(filePath));

    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
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

    // Filter bar using reusable component
    m_filter = new MonthYearFilter(this, true); // true = show export button
    connect(m_filter, &MonthYearFilter::filterChanged,
            this, &ServiceTab::onFilterChanged);
    connect(m_filter, &MonthYearFilter::exportPdfClicked,
            this, &ServiceTab::onExportPdf);
    m_contentLayout->addWidget(m_filter);

    // Summary cards
    createSummaryCards();

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
    m_totalSalesCard->showTrend(false);

    // Drink Card
    m_drinkCard = new SummaryCard("Đồ Uống", "🥤", this);
    m_drinkCard->setCardStyle(SummaryCard::Success);
    m_drinkCard->setValue("0 VND");
    m_drinkCard->showTrend(false);

    // Food Card
    m_foodCard = new SummaryCard("Đồ Ăn", "🍔", this);
    m_foodCard->setCardStyle(SummaryCard::Warning);
    m_foodCard->setValue("0 VND");
    m_foodCard->showTrend(false);

    // Equipment Card
    m_equipmentCard = new SummaryCard("Thiết Bị", "👕", this);
    m_equipmentCard->setCardStyle(SummaryCard::Purple);
    m_equipmentCard->setValue("0 VND");
    m_equipmentCard->showTrend(false);

    m_cardsLayout->addWidget(m_totalSalesCard);
    m_cardsLayout->addWidget(m_drinkCard);
    m_cardsLayout->addWidget(m_foodCard);
    m_cardsLayout->addWidget(m_equipmentCard);

    m_contentLayout->addLayout(m_cardsLayout);
}

void ServiceTab::createRankingTable()
{
    m_topServicesTable = new RankingTable("🏆 Top Dịch Vụ Bán Chạy Nhất", this);

    QList<RankingTable::ColumnConfig> columns;
    columns.append(RankingTable::ColumnConfig("Tên Dịch Vụ", RankingTable::Text, 400));
    columns.append(RankingTable::ColumnConfig("Loại", RankingTable::Text, 200));
    columns.append(RankingTable::ColumnConfig("SL Bán", RankingTable::Number, 150, true, Qt::AlignCenter));
    columns.append(RankingTable::ColumnConfig("Doanh Thu", RankingTable::Currency, -1));

    m_topServicesTable->setColumns(columns);
    m_topServicesTable->setMaxRows(5);
    m_topServicesTable->setScrollable(false);
    m_topServicesTable->setFixedHeight(380);

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
