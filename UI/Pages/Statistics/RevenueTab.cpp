#include "RevenueTab.h"
#include "../../../Core/QuanLy/HeThongQuanLy.h"
#include "../../../Core/QuanLy/QuanLyDatSan.h"
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegendMarker>
#include <QDateTime>
#include <QMap>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QPdfWriter>
#include <QPainter>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>

// ========== CONSTRUCTOR ==========

RevenueTab::RevenueTab(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(nullptr),
      m_scrollArea(nullptr),
      m_contentWidget(nullptr),
      m_contentLayout(nullptr),
      m_filter(nullptr),
      m_cardsLayout(nullptr),
      m_totalRevenueCard(nullptr),
      m_fieldRevenueCard(nullptr),
      m_serviceRevenueCard(nullptr),
      m_chartsRow1(nullptr),
      m_chartsRow2(nullptr),
      m_revenueChartContainer(nullptr),
      m_pieChartContainer(nullptr),
      m_heThong(nullptr),
      m_thongKe(nullptr)
{
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

    // Get date range from filter
    QDate fromDate = m_filter->getFromDate();
    QDate toDate = m_filter->getToDate();

    // Create new statistics with date range
    NgayThang tuNgay(fromDate.day(), fromDate.month(), fromDate.year());
    NgayThang denNgay(toDate.day(), toDate.month(), toDate.year());

    m_thongKe = new ThongKeDoanhThu(tuNgay, denNgay, m_heThong);

    // Update UI
    updateSummaryCards();
    updateRevenueChart();
    updatePieChart();
}

// ========== SLOTS ==========

void RevenueTab::onFilterChanged(int month, int year, MonthYearFilter::FilterMode mode)
{
    Q_UNUSED(month);
    Q_UNUSED(year);
    Q_UNUSED(mode);
    refreshData();
}

void RevenueTab::onExportPdf()
{
    if (!m_thongKe)
    {
        QMessageBox::warning(this, "Lỗi", "Không có dữ liệu để xuất!");
        return;
    }

    // Get filter values
    int selectedMonth = m_filter->selectedMonth();
    int selectedYear = m_filter->selectedYear();
    MonthYearFilter::FilterMode filterMode = m_filter->filterMode();

    // Ensure report directory exists
    QDir().mkpath("D:/REPORT/thongke");

    QString defaultName = QString("D:/REPORT/thongke/DoanhThu_%1_%2.pdf")
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
    painter.drawText(0, y, pageWidth, 200, Qt::AlignCenter, "BÁO CÁO DOANH THU");
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
    painter.drawText(100, y, "TỔNG KẾT");
    y += lineHeight + 50;

    painter.setFont(normalFont);
    auto formatMoney = [](double val) -> QString
    {
        return QString("%L1 VND").arg(val, 0, 'f', 0);
    };

    QStringList summaryLabels = {"Tổng doanh thu:", "Doanh thu sân:", "Doanh thu dịch vụ:",
                                 "Booking hoàn thành:", "Booking đã hủy:"};
    QStringList summaryValues = {
        formatMoney(m_thongKe->getTongDoanhThu()),
        formatMoney(m_thongKe->getDoanhThuTienSan()),
        formatMoney(m_thongKe->getDoanhThuDichVu()),
        QString::number(m_thongKe->getSoLuongDonHoanThanh()),
        QString::number(m_thongKe->getSoLuongDonHuy())};

    for (int i = 0; i < summaryLabels.size(); i++)
    {
        painter.drawText(100, y, 1500, lineHeight, Qt::AlignLeft, summaryLabels[i]);
        painter.drawText(1600, y, 2000, lineHeight, Qt::AlignLeft, summaryValues[i]);
        y += lineHeight;
    }

    y += 200;

    // Revenue breakdown
    painter.setFont(headerFont);
    painter.drawText(100, y, "PHÂN BỔ NGUỒN THU");
    y += lineHeight + 50;

    painter.setFont(normalFont);
    QStringList breakdownLabels = {"Đồ uống:", "Đồ ăn:", "Thiết bị:"};
    QStringList breakdownValues = {
        formatMoney(m_thongKe->getDoanhThuDoUong()),
        formatMoney(m_thongKe->getDoanhThuDoAn()),
        formatMoney(m_thongKe->getDoanhThuThietBi())};

    for (int i = 0; i < breakdownLabels.size(); i++)
    {
        painter.drawText(100, y, 1500, lineHeight, Qt::AlignLeft, breakdownLabels[i]);
        painter.drawText(1600, y, 2000, lineHeight, Qt::AlignLeft, breakdownValues[i]);
        y += lineHeight;
    }

    y += 300;

    // ===== DANH SÁCH ĐƠN ĐẶT SÂN HOÀN THÀNH =====
    painter.setFont(headerFont);
    painter.drawText(100, y, "DANH SÁCH ĐƠN HOÀN THÀNH");
    y += lineHeight + 100;

    // Table header
    painter.setFont(QFont("Arial", 9, QFont::Bold));
    int col1 = 100;  // Mã đơn
    int col2 = 600;  // Khách hàng
    int col3 = 1400; // Sân
    int col4 = 1900; // Ngày
    int col5 = 2500; // Khung giờ
    int col6 = 3100; // Tiền sân
    int col7 = 3600; // Dịch vụ
    int col8 = 4100; // Tổng

    painter.drawText(col1, y, 500, lineHeight, Qt::AlignLeft, "Mã đơn");
    painter.drawText(col2, y, 800, lineHeight, Qt::AlignLeft, "Khách hàng");
    painter.drawText(col3, y, 500, lineHeight, Qt::AlignLeft, "Sân");
    painter.drawText(col4, y, 600, lineHeight, Qt::AlignLeft, "Ngày");
    painter.drawText(col5, y, 600, lineHeight, Qt::AlignLeft, "Khung giờ");
    painter.drawText(col6, y, 500, lineHeight, Qt::AlignRight, "Tiền sân");
    painter.drawText(col7, y, 500, lineHeight, Qt::AlignRight, "Dịch vụ");
    painter.drawText(col8, y, 500, lineHeight, Qt::AlignRight, "Tổng");
    y += lineHeight + 20;

    // Draw line under header
    painter.drawLine(col1, y, col8 + 500, y);
    y += 30;

    // Get bookings from system
    painter.setFont(smallFont);
    QuanLyDatSan *qlDatSan = m_heThong->layQuanLyDatSan();
    const MangDong<DatSan *> &allBookings = qlDatSan->layDanhSachDatSan();

    int pageHeight = pdfWriter.height();
    int rowCount = 0;

    for (int i = 0; i < allBookings.size(); i++)
    {
        DatSan *booking = allBookings[i];
        if (!booking || booking->getTrangThai() != HOAN_THANH)
            continue;

        // Check if booking is in the selected period
        NgayGio ngayDat = booking->getThoiGianDat();
        bool inPeriod = false;

        if (filterMode == MonthYearFilter::ByMonth)
        {
            inPeriod = (ngayDat.getThang() == selectedMonth && ngayDat.getNam() == selectedYear);
        }
        else
        {
            inPeriod = (ngayDat.getNam() == selectedYear);
        }

        if (!inPeriod)
            continue;

        // Check if need new page
        if (y > pageHeight - 400)
        {
            pdfWriter.newPage();
            y = 100;

            // Redraw table header on new page
            painter.setFont(QFont("Arial", 9, QFont::Bold));
            painter.drawText(col1, y, 500, lineHeight, Qt::AlignLeft, "Mã đơn");
            painter.drawText(col2, y, 800, lineHeight, Qt::AlignLeft, "Khách hàng");
            painter.drawText(col3, y, 500, lineHeight, Qt::AlignLeft, "Sân");
            painter.drawText(col4, y, 600, lineHeight, Qt::AlignLeft, "Ngày");
            painter.drawText(col5, y, 600, lineHeight, Qt::AlignLeft, "Khung giờ");
            painter.drawText(col6, y, 500, lineHeight, Qt::AlignRight, "Tiền sân");
            painter.drawText(col7, y, 500, lineHeight, Qt::AlignRight, "Dịch vụ");
            painter.drawText(col8, y, 500, lineHeight, Qt::AlignRight, "Tổng");
            y += lineHeight + 20;
            painter.drawLine(col1, y, col8 + 500, y);
            y += 30;
            painter.setFont(smallFont);
        }

        // Draw booking row
        QString maDon = QString::fromStdString(booking->getMaDatSan());
        QString khachHang = booking->getKhachHang()
                                ? QString::fromStdString(booking->getKhachHang()->getHoTen())
                                : "N/A";
        QString tenSan = booking->getSan()
                             ? QString::fromStdString(booking->getSan()->getTenSan())
                             : "N/A";
        QString ngay = QString("%1/%2/%3")
                           .arg(ngayDat.getNgay(), 2, 10, QChar('0'))
                           .arg(ngayDat.getThang(), 2, 10, QChar('0'))
                           .arg(ngayDat.getNam());
        QString khungGio = QString("%1:00-%2:00")
                               .arg(booking->getKhungGio().getGioBatDau().getGio())
                               .arg(booking->getKhungGio().getGioKetThuc().getGio());

        int soGio = booking->getKhungGio().getGioKetThuc().getGio() - booking->getKhungGio().getGioBatDau().getGio();
        if (soGio < 0)
            soGio = 0;
        double tienSan = booking->getSan() ? booking->getSan()->getGiaThue() * soGio : 0;
        double tienDV = booking->getTongTien() - tienSan;
        if (tienDV < 0)
            tienDV = 0;

        painter.drawText(col1, y, 500, lineHeight, Qt::AlignLeft, maDon);
        painter.drawText(col2, y, 800, lineHeight, Qt::AlignLeft, khachHang);
        painter.drawText(col3, y, 500, lineHeight, Qt::AlignLeft, tenSan);
        painter.drawText(col4, y, 600, lineHeight, Qt::AlignLeft, ngay);
        painter.drawText(col5, y, 600, lineHeight, Qt::AlignLeft, khungGio);
        painter.drawText(col6, y, 500, lineHeight, Qt::AlignRight, formatMoney(tienSan).replace(" VND", ""));
        painter.drawText(col7, y, 500, lineHeight, Qt::AlignRight, formatMoney(tienDV).replace(" VND", ""));
        painter.drawText(col8, y, 500, lineHeight, Qt::AlignRight, formatMoney(booking->getTongTien()).replace(" VND", ""));

        y += lineHeight;
        rowCount++;
    }

    // Draw bottom line
    y += 20;
    painter.drawLine(col1, y, col8 + 500, y);
    y += lineHeight;

    // Total row
    painter.setFont(QFont("Arial", 9, QFont::Bold));
    painter.drawText(col1, y, 3000, lineHeight, Qt::AlignLeft,
                     QString("Tổng cộng: %1 đơn").arg(rowCount));
    painter.drawText(col8, y, 500, lineHeight, Qt::AlignRight, formatMoney(m_thongKe->getTongDoanhThu()).replace(" VND", ""));

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

    // Filter bar (using reusable component)
    m_filter = new MonthYearFilter(this, true); // true = show export button
    connect(m_filter, &MonthYearFilter::filterChanged, this, &RevenueTab::onFilterChanged);
    connect(m_filter, &MonthYearFilter::exportPdfClicked, this, &RevenueTab::onExportPdf);
    m_contentLayout->addWidget(m_filter);

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
    m_totalRevenueCard->showTrend(false);

    // Field Revenue Card
    m_fieldRevenueCard = new SummaryCard("Doanh Thu Sân", "⚽", this);
    m_fieldRevenueCard->setCardStyle(SummaryCard::Success);
    m_fieldRevenueCard->setValue("0 VND");
    m_fieldRevenueCard->showTrend(false);

    // Service Revenue Card
    m_serviceRevenueCard = new SummaryCard("Doanh Thu Dịch Vụ", "🛒", this);
    m_serviceRevenueCard->setCardStyle(SummaryCard::Purple);
    m_serviceRevenueCard->setValue("0 VND");
    m_serviceRevenueCard->showTrend(false);

    m_cardsLayout->addWidget(m_totalRevenueCard);
    m_cardsLayout->addWidget(m_fieldRevenueCard);
    m_cardsLayout->addWidget(m_serviceRevenueCard);

    m_contentLayout->addLayout(m_cardsLayout);
}

void RevenueTab::createCharts()
{
    // Row 1: Revenue bar chart (full width)
    m_chartsRow1 = new QHBoxLayout();
    m_chartsRow1->setSpacing(16);

    m_revenueChartContainer = new ChartContainer("Doanh Thu", this);
    m_revenueChartContainer->setMinimumChartHeight(350);
    m_revenueChartContainer->setMinimumWidth(600);
    m_chartsRow1->addWidget(m_revenueChartContainer);

    m_contentLayout->addLayout(m_chartsRow1);

    // Row 2: Pie chart (full width)
    m_chartsRow2 = new QHBoxLayout();
    m_chartsRow2->setSpacing(16);

    m_pieChartContainer = new ChartContainer("Phân Bổ Nguồn Thu", this);
    m_pieChartContainer->setMinimumChartHeight(320);

    m_chartsRow2->addWidget(m_pieChartContainer);

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

    // Field Revenue
    double fieldRevenue = m_thongKe->getDoanhThuTienSan();
    m_fieldRevenueCard->setValue(SummaryCard::formatCurrency(fieldRevenue));

    // Service Revenue
    double serviceRevenue = m_thongKe->getDoanhThuDichVu();
    m_serviceRevenueCard->setValue(SummaryCard::formatCurrency(serviceRevenue));
}

void RevenueTab::updateRevenueChart()
{
    if (!m_thongKe)
        return;

    QChart *chart = new QChart();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Create single bar set for total revenue
    QBarSet *revenueSet = new QBarSet("Doanh thu");
    revenueSet->setColor(QColor("#3b82f6")); // Blue

    QStringList categories;
    double maxValue = 0;
    QString chartTitle;
    QString axisXTitle;

    // Get daily data and create lookup map
    const MangDong<DoanhThuTheoNgay> &dailyData = m_thongKe->getDoanhThuTheoNgayList();
    const MangDong<DoanhThuTheoThang> &monthlyData = m_thongKe->getDoanhThuTheoThangList();

    // Create map for quick lookup by day number
    QMap<int, double> dayRevenueMap;
    for (int i = 0; i < dailyData.size(); i++)
    {
        const DoanhThuTheoNgay &item = dailyData[i];
        dayRevenueMap[item.ngay.getNgay()] = item.doanhThu;
    }

    // Create map for monthly lookup
    QMap<int, double> monthRevenueMap;
    for (int i = 0; i < monthlyData.size(); i++)
    {
        const DoanhThuTheoThang &item = monthlyData[i];
        monthRevenueMap[item.thang] = item.doanhThu;
    }

    MonthYearFilter::FilterMode filterMode = m_filter->filterMode();
    int selectedMonth = m_filter->selectedMonth();
    int selectedYear = m_filter->selectedYear();

    switch (filterMode)
    {
    case MonthYearFilter::ByMonth:
    {
        // Show all days of selected month
        chartTitle = QString("Doanh Thu Tháng %1/%2")
                         .arg(selectedMonth)
                         .arg(selectedYear);
        axisXTitle = "Ngày";

        QDate monthStart(selectedYear, selectedMonth, 1);
        int daysInMonth = monthStart.daysInMonth();

        // Iterate through all days of the month
        for (int day = 1; day <= daysInMonth; day++)
        {
            double total = dayRevenueMap.value(day, 0.0);
            *revenueSet << total;
            categories << QString::number(day);

            if (total > maxValue)
                maxValue = total;
        }
        break;
    }

    case MonthYearFilter::ByYear:
    {
        // Show 12 months
        chartTitle = QString("Doanh Thu Năm %1").arg(selectedYear);
        axisXTitle = "Tháng";

        QString monthNames[] = {"Th1", "Th2", "Th3", "Th4", "Th5", "Th6",
                                "Th7", "Th8", "Th9", "Th10", "Th11", "Th12"};

        // Iterate through all 12 months
        for (int m = 1; m <= 12; m++)
        {
            double total = monthRevenueMap.value(m, 0.0);
            *revenueSet << total;
            categories << monthNames[m - 1];

            if (total > maxValue)
                maxValue = total;
        }
        break;
    }
    }

    // Update chart title
    m_revenueChartContainer->setTitle(chartTitle);

    // Set label color for readability
    revenueSet->setLabelColor(QColor("#1e293b")); // Dark text

    // Determine scale and format values
    QString unitSuffix;
    double scaleFactor = 1.0;

    if (maxValue >= 1000000)
    {
        scaleFactor = 1000000.0;
        unitSuffix = "M";
    }
    else if (maxValue >= 1000)
    {
        scaleFactor = 1000.0;
        unitSuffix = "K";
    }

    // Scale bar values and set 0 values to very small negative (to hide label)
    for (int i = 0; i < revenueSet->count(); i++)
    {
        double val = revenueSet->at(i);
        if (val > 0)
        {
            revenueSet->replace(i, val / scaleFactor);
        }
        // Values of 0 stay as 0, labels will be hidden via format
    }
    maxValue = maxValue / scaleFactor;

    // Create bar series
    QBarSeries *series = new QBarSeries();
    series->append(revenueSet);

    // Show value labels on top of bars
    series->setLabelsVisible(true);
    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);

    // Format: show value with unit, but 0 will still show - we handle via precision
    if (!unitSuffix.isEmpty())
    {
        series->setLabelsFormat(QString("@value %1").arg(unitSuffix));
    }
    else
    {
        series->setLabelsFormat("@value VND");
    }

    chart->addSeries(series);

    // X Axis
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsAngle(0); // Vertical labels
    axisX->setTitleText(axisXTitle);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Y Axis
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText(unitSuffix.isEmpty() ? "VND" : QString("VND (%1)").arg(unitSuffix));
    axisY->setLabelFormat(scaleFactor >= 1000000 ? "%.1f" : "%.0f");
    axisY->setRange(0, maxValue > 0 ? maxValue * 1.15 : 1);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Hide legend
    chart->legend()->setVisible(false);
    chart->setBackgroundVisible(false);

    m_revenueChartContainer->setChart(chart);
}

void RevenueTab::updatePieChart()
{
    if (!m_thongKe)
        return;

    QChart *chart = new QChart();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.35); // Donut style

    double fieldRevenue = m_thongKe->getDoanhThuTienSan();
    double drinkRevenue = m_thongKe->getDoanhThuDoUong();
    double foodRevenue = m_thongKe->getDoanhThuDoAn();
    double equipRevenue = m_thongKe->getDoanhThuThietBi();
    double total = fieldRevenue + drinkRevenue + foodRevenue + equipRevenue;

    // Store names for legend
    QStringList sliceNames;

    if (total > 0)
    {
        // Rotate start angle so small slices (Thiết bị) are positioned for right-side labels
        series->setPieStartAngle(-50);
        series->setPieEndAngle(310);

        // Add slices in specific order to control label positions
        // Tiền sân - largest, label goes to bottom-right naturally
        if (fieldRevenue > 0)
        {
            QPieSlice *slice = series->append("Tiền sân", fieldRevenue);
            slice->setColor(QColor("#3b82f6"));
            slice->setLabelVisible(true);
            slice->setLabel(QString("%1%").arg((fieldRevenue / total) * 100.0, 0, 'f', 1));
            slice->setLabelColor(QColor("#334155"));
            slice->setLabelPosition(QPieSlice::LabelOutside);
            sliceNames.append("Tiền sân");
        }

        // Đồ uống - label arm length adjusted
        if (drinkRevenue > 0)
        {
            QPieSlice *slice = series->append("Đồ uống", drinkRevenue);
            slice->setColor(QColor("#10b981"));
            slice->setLabelVisible(true);
            slice->setLabel(QString("%1%").arg((drinkRevenue / total) * 100.0, 0, 'f', 1));
            slice->setLabelColor(QColor("#334155"));
            slice->setLabelPosition(QPieSlice::LabelOutside);
            slice->setLabelArmLengthFactor(0.25);
            sliceNames.append("Đồ uống");
        }

        // Đồ ăn
        if (foodRevenue > 0)
        {
            QPieSlice *slice = series->append("Đồ ăn", foodRevenue);
            slice->setColor(QColor("#f59e0b"));
            slice->setLabelVisible(true);
            slice->setLabel(QString("%1%").arg((foodRevenue / total) * 100.0, 0, 'f', 1));
            slice->setLabelColor(QColor("#334155"));
            slice->setLabelPosition(QPieSlice::LabelOutside);
            slice->setLabelArmLengthFactor(0.15);
            sliceNames.append("Đồ ăn");
        }

        // Thiết bị - positioned to have label go right
        if (equipRevenue > 0)
        {
            QPieSlice *slice = series->append("Thiết bị", equipRevenue);
            slice->setColor(QColor("#8b5cf6"));
            slice->setLabelVisible(true);
            slice->setLabel(QString("%1%").arg((equipRevenue / total) * 100.0, 0, 'f', 1));
            slice->setLabelColor(QColor("#334155"));
            slice->setLabelPosition(QPieSlice::LabelOutside);
            slice->setLabelArmLengthFactor(0.35);
            sliceNames.append("Thiết bị");
        }
    }

    if (series->count() == 0)
    {
        QPieSlice *emptySlice = series->append("Chưa có dữ liệu", 1);
        emptySlice->setColor(QColor("#e2e8f0"));
        emptySlice->setLabelVisible(true);
    }

    chart->addSeries(series);

    // Set legend to show names (not percentages) - bigger font
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->legend()->setFont(QFont("Arial", 11, QFont::Medium));

    // Update legend markers to show names instead of percentage labels
    QList<QLegendMarker *> markers = chart->legend()->markers(series);
    for (int i = 0; i < markers.size() && i < sliceNames.size(); i++)
    {
        markers[i]->setLabel(sliceNames[i]);
    }

    chart->setBackgroundVisible(false);

    m_pieChartContainer->setChart(chart);
}
