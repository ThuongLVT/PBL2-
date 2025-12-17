#include "BookingFieldTab.h"
#include "../../Components/MonthYearFilter.h"
#include "../../../Core/QuanLy/HeThongQuanLy.h"
#include "../../../Core/QuanLy/QuanLyDatSan.h"
#include "../../../Core/Models/San.h"
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegendMarker>
#include <QDate>
#include <QFileDialog>
#include <QMessageBox>
#include <QPdfWriter>
#include <QPainter>
#include <QDesktopServices>
#include <QUrl>
#include <QDateTime>
#include <QDir>

// ========== CONSTRUCTOR ==========

BookingFieldTab::BookingFieldTab(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(nullptr),
      m_scrollArea(nullptr),
      m_contentWidget(nullptr),
      m_contentLayout(nullptr),
      m_filter(nullptr),
      m_cardsLayout(nullptr),
      m_totalBookingCard(nullptr),
      m_completedCard(nullptr),
      m_cancelledCard(nullptr),
      m_chartsRow(nullptr),
      m_chartsRow2(nullptr),
      m_donutChartContainer(nullptr),
      m_weekdayChartContainer(nullptr),
      m_peakHoursChartContainer(nullptr),
      m_topFieldsTable(nullptr),
      m_heThong(nullptr),
      m_thongKe(nullptr)
{
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

    // Get date range from filter
    QDate fromDate = m_filter->getFromDate();
    QDate toDate = m_filter->getToDate();

    // Create new statistics with date range
    NgayThang tuNgay(fromDate.day(), fromDate.month(), fromDate.year());
    NgayThang denNgay(toDate.day(), toDate.month(), toDate.year());

    m_thongKe = new ThongKeBooking(tuNgay, denNgay, m_heThong);

    // Update UI
    updateSummaryCards();
    updateDonutChart();
    updateWeekdayChart();
    updatePeakHoursChart();
    updateRankingTable();
}

// ========== SLOTS ==========

void BookingFieldTab::onFilterChanged(int month, int year, MonthYearFilter::FilterMode mode)
{
    Q_UNUSED(month);
    Q_UNUSED(year);
    Q_UNUSED(mode);
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

    // Filter bar using reusable component
    m_filter = new MonthYearFilter(this, true); // true = show export button
    connect(m_filter, &MonthYearFilter::filterChanged,
            this, &BookingFieldTab::onFilterChanged);
    connect(m_filter, &MonthYearFilter::exportPdfClicked,
            this, &BookingFieldTab::onExportPdf);
    m_contentLayout->addWidget(m_filter);

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
    m_totalBookingCard->showTrend(false);

    // Completed Card
    m_completedCard = new SummaryCard("Hoàn Thành", "✅", this);
    m_completedCard->setCardStyle(SummaryCard::Success);
    m_completedCard->setValue("0");
    m_completedCard->showTrend(false);

    // Cancelled Card
    m_cancelledCard = new SummaryCard("Đã Hủy", "❌", this);
    m_cancelledCard->setCardStyle(SummaryCard::Danger);
    m_cancelledCard->setValue("0");
    m_cancelledCard->showTrend(false);

    m_cardsLayout->addWidget(m_totalBookingCard);
    m_cardsLayout->addWidget(m_completedCard);
    m_cardsLayout->addWidget(m_cancelledCard);

    m_contentLayout->addLayout(m_cardsLayout);
}

void BookingFieldTab::createCharts()
{
    m_chartsRow = new QHBoxLayout();
    m_chartsRow->setSpacing(16);

    m_donutChartContainer = new ChartContainer("Tỷ Lệ Loại Sân", this);
    m_donutChartContainer->setMinimumChartHeight(280);

    m_weekdayChartContainer = new ChartContainer("Booking Theo Ngày Trong Tuần", this);
    m_weekdayChartContainer->setMinimumChartHeight(280);

    m_chartsRow->addWidget(m_donutChartContainer, 1);
    m_chartsRow->addWidget(m_weekdayChartContainer, 1);

    m_contentLayout->addLayout(m_chartsRow);

    // Row 2: Peak Hours Chart
    m_chartsRow2 = new QHBoxLayout();
    m_chartsRow2->setSpacing(16);

    m_peakHoursChartContainer = new ChartContainer("Phân Bổ Booking Theo Khung Giờ", this);
    m_peakHoursChartContainer->setMinimumChartHeight(300);

    m_chartsRow2->addWidget(m_peakHoursChartContainer);
    m_contentLayout->addLayout(m_chartsRow2);
}

void BookingFieldTab::createRankingTable()
{
    m_topFieldsTable = new RankingTable("🏆 Top Sân Được Đặt Nhiều Nhất", this);

    QList<RankingTable::ColumnConfig> columns;
    columns.append(RankingTable::ColumnConfig("Tên Sân", RankingTable::Text, 300));
    columns.append(RankingTable::ColumnConfig("Loại", RankingTable::Text, 150));
    columns.append(RankingTable::ColumnConfig("Lượt Đặt", RankingTable::Number, 150, true, Qt::AlignCenter));
    columns.append(RankingTable::ColumnConfig("Doanh Thu", RankingTable::Currency, -1));

    m_topFieldsTable->setColumns(columns);
    m_topFieldsTable->setMaxRows(5);
    m_topFieldsTable->setScrollable(false);
    m_topFieldsTable->setFixedHeight(380);

    m_contentLayout->addWidget(m_topFieldsTable);
}

// ========== UPDATE METHODS ==========

void BookingFieldTab::updateSummaryCards()
{
    if (!m_thongKe)
        return;

    // Completed
    int completed = m_thongKe->getSoBookingHoanThanh();
    m_completedCard->setValueInt(completed);

    // Cancelled
    int cancelled = m_thongKe->getSoBookingHuy();
    m_cancelledCard->setValueInt(cancelled);

    // Total = Completed + Cancelled (chỉ đếm đơn đã xử lý xong)
    int totalBooking = completed + cancelled;
    m_totalBookingCard->setValueInt(totalBooking);
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
    int total = san5 + san7;

    if (san5 > 0)
    {
        double percent = (san5 * 100.0) / total;
        QPieSlice *slice = series->append(QString("Sân 5"), san5);
        slice->setColor(QColor("#3b82f6"));
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1%").arg(percent, 0, 'f', 1));
    }
    if (san7 > 0)
    {
        double percent = (san7 * 100.0) / total;
        QPieSlice *slice = series->append(QString("Sân 7"), san7);
        slice->setColor(QColor("#10b981"));
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1%").arg(percent, 0, 'f', 1));
    }

    if (series->count() == 0)
    {
        series->append("Chưa có dữ liệu", 1);
        series->slices().at(0)->setColor(QColor("#e2e8f0"));
    }

    chart->addSeries(series);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setBackgroundVisible(false);

    // Update legend labels with name and count
    if (total > 0)
    {
        const auto markers = chart->legend()->markers(series);
        int i = 0;
        if (san5 > 0 && i < markers.size())
        {
            markers[i]->setLabel(QString("Sân 5 người (%1)").arg(san5));
            i++;
        }
        if (san7 > 0 && i < markers.size())
        {
            markers[i]->setLabel(QString("Sân 7 người (%1)").arg(san7));
        }
    }

    m_donutChartContainer->setChart(chart);
}

void BookingFieldTab::updateWeekdayChart()
{
    if (!m_thongKe)
        return;

    QChart *chart = new QChart();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarSet *set = new QBarSet("Lượt đặt");
    set->setColor(QColor("#8b5cf6")); // Purple color

    const MangDong<ThongKeTheoNgay> &theoNgay = m_thongKe->getThongKeTheoNgay();

    // Weekday names: 0=CN, 1=T2, ..., 6=T7
    QStringList categories = {"CN", "T2", "T3", "T4", "T5", "T6", "T7"};
    int maxValue = 0;

    // ThongKeTheoNgay already has 7 items (0-6)
    for (int i = 0; i < 7 && i < theoNgay.size(); i++)
    {
        *set << theoNgay[i].soLuotDat;
        if (theoNgay[i].soLuotDat > maxValue)
            maxValue = theoNgay[i].soLuotDat;
    }

    // Fill remaining if less than 7
    while (set->count() < 7)
    {
        *set << 0;
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);
    series->setLabelsVisible(true);
    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);

    chart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, maxValue > 0 ? maxValue + 2 : 5);
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(false);
    chart->setBackgroundVisible(false);

    m_weekdayChartContainer->setChart(chart);
}

void BookingFieldTab::updatePeakHoursChart()
{
    if (!m_thongKe)
        return;

    QChart *chart = new QChart();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarSet *set = new QBarSet("Số lượt đặt");
    set->setColor(QColor("#f59e0b")); // Orange color

    const MangDong<ThongKeKhungGio> &khungGio = m_thongKe->getThongKeKhungGio();
    QStringList categories;

    // Find max value for Y axis
    int maxValue = 0;

    for (int i = 0; i < khungGio.size(); i++)
    {
        const ThongKeKhungGio &item = khungGio[i];
        *set << item.soLuotDat;
        categories << QString("%1h-%2h").arg(item.gioBatDau).arg(item.gioKetThuc);

        if (item.soLuotDat > maxValue)
            maxValue = item.soLuotDat;
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);
    series->setLabelsVisible(true);
    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);

    chart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, maxValue > 0 ? maxValue + 2 : 5);
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(false);
    chart->setBackgroundVisible(false);

    m_peakHoursChartContainer->setChart(chart);
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

void BookingFieldTab::onExportPdf()
{
    if (!m_thongKe)
    {
        QMessageBox::warning(this, "Lỗi", "Không có dữ liệu để xuất!");
        return;
    }

    // Ensure report directory exists
    QDir().mkpath("D:/REPORT/thongke");

    int selectedMonth = m_filter->selectedMonth();
    int selectedYear = m_filter->selectedYear();
    MonthYearFilter::FilterMode filterMode = m_filter->filterMode();

    QString defaultName = QString("D:/REPORT/thongke/ThongKeDatSan_%1_%2.pdf")
                              .arg(filterMode == MonthYearFilter::ByMonth ? QString("Thang%1").arg(selectedMonth) : "Nam")
                              .arg(selectedYear);

    QString filePath = QFileDialog::getSaveFileName(
        this, "Xuất báo cáo PDF", defaultName, "PDF Files (*.pdf)");

    if (filePath.isEmpty())
        return;

    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setPageMargins(QMarginsF(20, 20, 20, 20), QPageLayout::Millimeter);
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);

    // Fonts
    QFont titleFont("Arial", 18, QFont::Bold);
    QFont headerFont("Arial", 12, QFont::Bold);
    QFont normalFont("Arial", 10);
    QFont smallFont("Arial", 9);

    int y = 100;
    int lineHeight = 150;
    int pageWidth = pdfWriter.width();

    // Title
    painter.setFont(titleFont);
    painter.drawText(0, y, pageWidth, 200, Qt::AlignCenter, "BÁO CÁO THỐNG KÊ ĐẶT SÂN");
    y += 250;

    // Period
    painter.setFont(headerFont);
    QString period = filterMode == MonthYearFilter::ByMonth
                         ? QString("Tháng %1/%2").arg(selectedMonth).arg(selectedYear)
                         : QString("Năm %1").arg(selectedYear);
    painter.drawText(0, y, pageWidth, 150, Qt::AlignCenter, period);
    y += 300;

    // Summary section
    painter.setFont(headerFont);
    painter.drawText(100, y, "TỔNG KẾT BOOKING");
    y += lineHeight + 50;

    painter.setFont(normalFont);
    int completed = m_thongKe->getSoBookingHoanThanh();
    int cancelled = m_thongKe->getSoBookingHuy();
    int total = completed + cancelled;

    QStringList summaryLabels = {"Tổng số booking:", "Hoàn thành:", "Đã hủy:"};
    QStringList summaryValues = {
        QString::number(total),
        QString::number(completed),
        QString::number(cancelled)};

    for (int i = 0; i < summaryLabels.size(); i++)
    {
        painter.drawText(100, y, 1500, lineHeight, Qt::AlignLeft, summaryLabels[i]);
        painter.drawText(1600, y, 2000, lineHeight, Qt::AlignLeft, summaryValues[i]);
        y += lineHeight;
    }

    y += 200;

    // Field type breakdown
    painter.setFont(headerFont);
    painter.drawText(100, y, "THỐNG KÊ THEO LOẠI SÂN");
    y += lineHeight + 50;

    painter.setFont(normalFont);
    QStringList fieldLabels = {"Sân 5 người:", "Sân 7 người:"};
    QStringList fieldValues = {
        QString("%1 lượt").arg(m_thongKe->getSoBookingSan5()),
        QString("%1 lượt").arg(m_thongKe->getSoBookingSan7())};

    for (int i = 0; i < fieldLabels.size(); i++)
    {
        painter.drawText(100, y, 1500, lineHeight, Qt::AlignLeft, fieldLabels[i]);
        painter.drawText(1600, y, 2000, lineHeight, Qt::AlignLeft, fieldValues[i]);
        y += lineHeight;
    }

    y += 200;

    // Top fields
    painter.setFont(headerFont);
    painter.drawText(100, y, "TOP SÂN ĐƯỢC ĐẶT NHIỀU NHẤT");
    y += lineHeight + 50;

    const MangDong<ThongTinSanTop> &topSan = m_thongKe->getTopSan();
    painter.setFont(normalFont);
    for (int i = 0; i < topSan.size() && i < 5; i++)
    {
        const ThongTinSanTop &item = topSan[i];
        if (item.san)
        {
            QString line = QString("%1. %2 - %3 lượt - %L4 VND")
                               .arg(i + 1)
                               .arg(QString::fromStdString(item.san->getTenSan()))
                               .arg(item.soLuotDat)
                               .arg(item.doanhThu, 0, 'f', 0);
            painter.drawText(100, y, 3500, lineHeight, Qt::AlignLeft, line);
            y += lineHeight;
        }
    }

    // Footer
    painter.setFont(smallFont);
    QString footer = QString("Xuất ngày: %1").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"));
    painter.drawText(0, pdfWriter.height() - 200, pageWidth, 150, Qt::AlignCenter, footer);

    painter.end();

    QMessageBox::information(this, "Thành công",
                             QString("Đã xuất báo cáo PDF:\n%1").arg(filePath));

    // Open PDF file
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}
