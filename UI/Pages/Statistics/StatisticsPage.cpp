#include "StatisticsPage.h"
#include "Core/QuanLy/HeThongQuanLy.h"
#include "Core/ThongKe/ThongKeDoanhThu.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDate>
#include <QMessageBox>
#include <QDebug>

StatisticsPage::StatisticsPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();

    // Set default date range (current month)
    QDate currentDate = QDate::currentDate();
    fromDateEdit->setDate(QDate(currentDate.year(), currentDate.month(), 1));
    toDateEdit->setDate(currentDate);

    // Initial load
    refreshData();
}

StatisticsPage::~StatisticsPage()
{
}

void StatisticsPage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    // Title
    QLabel *titleLabel = new QLabel("Thống Kê Doanh Thu", this);
    QFont titleFont;
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    setupHeader();
    setupSummaryCards();
    setupDetailsTable();

    mainLayout->addStretch();
}

void StatisticsPage::setupHeader()
{
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(15);

    QLabel *fromLabel = new QLabel("Từ ngày:", this);
    fromDateEdit = new QDateEdit(this);
    fromDateEdit->setCalendarPopup(true);
    fromDateEdit->setDisplayFormat("dd/MM/yyyy");
    fromDateEdit->setFixedWidth(120);

    QLabel *toLabel = new QLabel("Đến ngày:", this);
    toDateEdit = new QDateEdit(this);
    toDateEdit->setCalendarPopup(true);
    toDateEdit->setDisplayFormat("dd/MM/yyyy");
    toDateEdit->setFixedWidth(120);

    analyzeBtn = new QPushButton("Phân Tích", this);
    analyzeBtn->setFixedWidth(120);
    analyzeBtn->setCursor(Qt::PointingHandCursor);
    connect(analyzeBtn, &QPushButton::clicked, this, &StatisticsPage::onAnalyzeClicked);

    headerLayout->addWidget(fromLabel);
    headerLayout->addWidget(fromDateEdit);
    headerLayout->addWidget(toLabel);
    headerLayout->addWidget(toDateEdit);
    headerLayout->addWidget(analyzeBtn);
    headerLayout->addStretch();

    layout()->addItem(headerLayout);
}

void StatisticsPage::setupSummaryCards()
{
    QHBoxLayout *cardsLayout = new QHBoxLayout();
    cardsLayout->setSpacing(20);

    // Card 1: Total Revenue
    QFrame *card1 = createStatCard("Tổng Doanh Thu", "0 VND", ":/icons/money.svg", "#16a34a");
    lblTotalRevenue = card1->findChild<QLabel *>("valueLabel");

    // Card 2: Total Bookings
    QFrame *card2 = createStatCard("Tổng Đơn Đặt", "0", ":/icons/booking.svg", "#3b82f6");
    lblTotalBookings = card2->findChild<QLabel *>("valueLabel");

    // Card 3: Completed
    QFrame *card3 = createStatCard("Hoàn Thành", "0", ":/icons/check.svg", "#10b981");
    lblCompleted = card3->findChild<QLabel *>("valueLabel");

    // Card 4: Cancelled
    QFrame *card4 = createStatCard("Đã Hủy", "0", ":/icons/cancel.svg", "#ef4444");
    lblCancelled = card4->findChild<QLabel *>("valueLabel");

    cardsLayout->addWidget(card1);
    cardsLayout->addWidget(card2);
    cardsLayout->addWidget(card3);
    cardsLayout->addWidget(card4);

    layout()->addItem(cardsLayout);
}

QFrame *StatisticsPage::createStatCard(const QString &title, const QString &value, const QString &iconPath, const QString &color)
{
    QFrame *card = new QFrame(this);
    card->setObjectName("statCard");
    card->setStyleSheet(QString(
                            "#statCard { background-color: white; border: 1px solid #e5e7eb; border-radius: 12px; }"
                            "#statCard:hover { border: 1px solid %1; box-shadow: 0 4px 6px -1px rgba(0, 0, 0, 0.1); }")
                            .arg(color));
    card->setMinimumHeight(100);

    QHBoxLayout *mainLayout = new QHBoxLayout(card);

    // Icon
    QLabel *iconLabel = new QLabel(card);
    iconLabel->setFixedSize(48, 48);
    iconLabel->setStyleSheet(QString("background-color: %1; border-radius: 24px; padding: 10px;").arg(color + "20")); // 20 for transparency
    // Load icon (assuming SVG or PNG)
    QPixmap pixmap(iconPath);
    if (!pixmap.isNull())
    {
        // Scale pixmap to fit
        iconLabel->setPixmap(pixmap.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        iconLabel->setAlignment(Qt::AlignCenter);
    }

    QVBoxLayout *textLayout = new QVBoxLayout();

    QLabel *titleLbl = new QLabel(title, card);
    titleLbl->setStyleSheet("color: #6b7280; font-size: 14px; font-weight: 500;");

    QLabel *valueLbl = new QLabel(value, card);
    valueLbl->setObjectName("valueLabel");
    valueLbl->setStyleSheet(QString("color: %1; font-size: 24px; font-weight: 700; margin-top: 5px;").arg(color));

    textLayout->addWidget(titleLbl);
    textLayout->addWidget(valueLbl);
    textLayout->addStretch();

    mainLayout->addWidget(iconLabel);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(textLayout);
    mainLayout->addStretch();

    return card;
}

void StatisticsPage::setupDetailsTable()
{
    QLabel *tableTitle = new QLabel("Chi Tiết Theo Ngày", this);
    tableTitle->setStyleSheet("font-size: 18px; font-weight: bold; margin-top: 20px; margin-bottom: 10px;");
    layout()->addWidget(tableTitle);

    detailsTable = new QTableWidget(this);
    detailsTable->setColumnCount(4);
    detailsTable->setHorizontalHeaderLabels({"Ngày", "Doanh Thu", "Số Đơn", "Trạng Thái"});
    detailsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    detailsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    detailsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    detailsTable->setStyleSheet("QTableWidget { border: 1px solid #e5e7eb; border-radius: 8px; background-color: white; } QHeaderView::section { background-color: #f9fafb; padding: 10px; border: none; font-weight: bold; }");

    layout()->addWidget(detailsTable);
}

void StatisticsPage::onAnalyzeClicked()
{
    refreshData();
}

void StatisticsPage::refreshData()
{
    QDate fromDate = fromDateEdit->date();
    QDate toDate = toDateEdit->date();

    if (fromDate > toDate)
    {
        QMessageBox::warning(this, "Lỗi", "Ngày bắt đầu phải nhỏ hơn hoặc bằng ngày kết thúc!");
        return;
    }

    // Convert to Core types
    NgayThang tuNgay(fromDate.day(), fromDate.month(), fromDate.year());
    NgayThang denNgay(toDate.day(), toDate.month(), toDate.year());

    // Get System
    HeThongQuanLy *system = HeThongQuanLy::getInstance();
    if (!system)
        return;

    // Create Statistics Object
    ThongKeDoanhThu stats(tuNgay, denNgay, system);

    // Update Summary Cards
    lblTotalRevenue->setText(formatCurrency(stats.getTongDoanhThu()));
    lblTotalBookings->setText(QString::number(stats.getSoLuongDonDat()));
    lblCompleted->setText(QString::number(stats.getSoLuongDonHoanThanh()));
    lblCancelled->setText(QString::number(stats.getSoLuongDonHuy()));

    // Update Table (Daily Breakdown)
    detailsTable->setRowCount(0);

    // Iterate through each day in range
    QDate current = fromDate;
    int row = 0;
    while (current <= toDate)
    {
        NgayThang nt(current.day(), current.month(), current.year());
        double dailyRevenue = stats.tinhDoanhThuTheoNgay(nt);

        // Only show days with revenue > 0 or if range is small (< 10 days)
        if (dailyRevenue > 0 || fromDate.daysTo(toDate) < 10)
        {
            detailsTable->insertRow(row);

            detailsTable->setItem(row, 0, new QTableWidgetItem(current.toString("dd/MM/yyyy")));
            detailsTable->setItem(row, 1, new QTableWidgetItem(formatCurrency(dailyRevenue)));

            // Count bookings for this day (simple check)
            // Note: ThongKeDoanhThu doesn't expose daily count directly,
            // but we can infer or add method. For now, leave empty or simple text
            detailsTable->setItem(row, 2, new QTableWidgetItem("-"));

            QString status = dailyRevenue > 0 ? "Có doanh thu" : "Không có doanh thu";
            detailsTable->setItem(row, 3, new QTableWidgetItem(status));

            row++;
        }

        current = current.addDays(1);
    }
}

QString StatisticsPage::formatCurrency(double amount)
{
    return QString::number(amount, 'f', 0) + " VND";
}
