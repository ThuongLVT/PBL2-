/**
 * @file BookingTableTab.cpp
 * @brief Implementation of BookingTableTab - Full Table View
 */

#include "BookingTableTab.h"
#include "BookingDetailDialog.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QDate>
#include <QTime>
#include <iomanip>
#include <sstream>

BookingTableTab::BookingTableTab(QWidget *parent)
    : QWidget(parent),
      system(nullptr)
{
    system = HeThongQuanLy::getInstance();
    fields = system->layDanhSachSan();
    setupUI();
    setupConnections();
    loadTableData();
}

BookingTableTab::~BookingTableTab()
{
    // Qt handles cleanup via parent-child relationship
}

void BookingTableTab::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    setupSearchAndFilters();
    setupStatsCards();
    setupTable();

    mainLayout->addWidget(bookingTable, 1);

    setLayout(mainLayout);
}

void BookingTableTab::setupSearchAndFilters()
{
    // ===== SEARCH & FILTERS ROW =====
    QHBoxLayout *searchFilterLayout = new QHBoxLayout();
    searchFilterLayout->setSpacing(10);

    // Search box
    searchBox = new QLineEdit();
    searchBox->setPlaceholderText("🔍 Tìm theo tên khách hàng, mã đặt sân, hoặc sân...");
    searchBox->setObjectName("searchEdit");
    searchBox->setMinimumHeight(42);
    searchBox->setStyleSheet(
        "QLineEdit { "
        "padding: 8px 15px; "
        "border: 2px solid #e5e7eb; "
        "border-radius: 6px; "
        "font-size: 14px; "
        "background-color: white; "
        "} "
        "QLineEdit:focus { "
        "border: 2px solid #16a34a; "
        "}");
    searchFilterLayout->addWidget(searchBox, 4);

    // Field filter
    fieldFilterCombo = new QComboBox();
    fieldFilterCombo->addItem("🏟️ Tất cả sân", "ALL");
    for (int i = 0; i < fields.size(); i++)
    {
        San *field = fields[i];
        if (field)
        {
            QString label = QString("Sân %1").arg(QString::fromStdString(field->getTenSan()));
            fieldFilterCombo->addItem(label, QString::fromStdString(field->getMaSan()));
        }
    }
    fieldFilterCombo->setMinimumHeight(42);
    fieldFilterCombo->setStyleSheet(
        "QComboBox { "
        "padding: 8px 15px; "
        "border: 2px solid #e5e7eb; "
        "border-radius: 6px; "
        "font-size: 14px; "
        "background-color: white; "
        "} "
        "QComboBox:focus { "
        "border: 2px solid #16a34a; "
        "}");
    searchFilterLayout->addWidget(fieldFilterCombo, 1);

    // Status filter
    statusFilterCombo = new QComboBox();
    statusFilterCombo->addItem("📊 Tất cả trạng thái", -1);
    statusFilterCombo->addItem("🟢 Đã đặt", static_cast<int>(TrangThaiDatSan::DA_DAT));
    statusFilterCombo->addItem("✓ Hoàn thành", static_cast<int>(TrangThaiDatSan::HOAN_THANH));
    statusFilterCombo->addItem("❌ Đã hủy", static_cast<int>(TrangThaiDatSan::DA_HUY));
    statusFilterCombo->setMinimumHeight(42);
    statusFilterCombo->setStyleSheet(
        "QComboBox { "
        "padding: 8px 15px; "
        "border: 2px solid #e5e7eb; "
        "border-radius: 6px; "
        "font-size: 14px; "
        "background-color: white; "
        "} "
        "QComboBox:focus { "
        "border: 2px solid #16a34a; "
        "}");
    searchFilterLayout->addWidget(statusFilterCombo, 1);

    // Date range filters
    QLabel *fromLabel = new QLabel("📅 Từ:");
    fromLabel->setStyleSheet("color: #374151; font-size: 14px; font-weight: 600;");
    searchFilterLayout->addWidget(fromLabel);

    fromDateEdit = new QDateEdit();
    fromDateEdit->setCalendarPopup(true);
    fromDateEdit->setDate(QDate::currentDate().addYears(-1)); // Default to 1 year back
    fromDateEdit->setDisplayFormat("dd/MM/yyyy");
    fromDateEdit->setMinimumHeight(42);
    fromDateEdit->setStyleSheet(
        "QDateEdit { "
        "padding: 8px 12px; "
        "border: 2px solid #e5e7eb; "
        "border-radius: 6px; "
        "font-size: 14px; "
        "background-color: white; "
        "}");
    searchFilterLayout->addWidget(fromDateEdit, 1);

    QLabel *toLabel = new QLabel("Đến:");
    toLabel->setStyleSheet("color: #374151; font-size: 14px; font-weight: 600;");
    searchFilterLayout->addWidget(toLabel);

    toDateEdit = new QDateEdit();
    toDateEdit->setCalendarPopup(true);
    toDateEdit->setDate(QDate::currentDate().addYears(1)); // Default to 1 year forward
    toDateEdit->setDisplayFormat("dd/MM/yyyy");
    toDateEdit->setMinimumHeight(42);
    toDateEdit->setStyleSheet(
        "QDateEdit { "
        "padding: 8px 12px; "
        "border: 2px solid #e5e7eb; "
        "border-radius: 6px; "
        "font-size: 14px; "
        "background-color: white; "
        "}");
    searchFilterLayout->addWidget(toDateEdit, 1);

    mainLayout->addLayout(searchFilterLayout);
}

void BookingTableTab::setupStatsCards()
{
    // ===== STATS CARDS =====
    QFrame *statsContainer = new QFrame();
    statsContainer->setStyleSheet(
        "QFrame { "
        "background-color: transparent; "
        "border: none; "
        "}");
    QHBoxLayout *statsLayout = new QHBoxLayout(statsContainer);
    statsLayout->setSpacing(15);
    statsLayout->setContentsMargins(0, 0, 0, 0);

    // Card 1: Total bookings
    totalCard = new QFrame();
    totalCard->setStyleSheet(
        "QFrame { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #3b82f6, stop:1 #2563eb); "
        "border-radius: 12px; "
        "padding: 15px; "
        "}");
    QVBoxLayout *card1Layout = new QVBoxLayout(totalCard);
    card1Layout->setSpacing(8);
    QLabel *card1Title = new QLabel("📊 Tổng đơn đặt sân");
    card1Title->setStyleSheet("color: white; font-size: 14px; font-weight: 600;");
    totalLabel = new QLabel("0");
    totalLabel->setStyleSheet("color: white; font-size: 38px; font-weight: bold;");
    card1Layout->addWidget(card1Title);
    card1Layout->addWidget(totalLabel);
    card1Layout->addStretch();
    statsLayout->addWidget(totalCard);

    // Card 2: Confirmed
    confirmedCard = new QFrame();
    confirmedCard->setStyleSheet(
        "QFrame { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #16a34a, stop:1 #15803d); "
        "border-radius: 12px; "
        "padding: 15px; "
        "}");
    QVBoxLayout *card2Layout = new QVBoxLayout(confirmedCard);
    card2Layout->setSpacing(8);
    QLabel *card2Title = new QLabel("✅ Đã xác nhận");
    card2Title->setStyleSheet("color: white; font-size: 14px; font-weight: 600;");
    confirmedLabel = new QLabel("0");
    confirmedLabel->setStyleSheet("color: white; font-size: 38px; font-weight: bold;");
    card2Layout->addWidget(card2Title);
    card2Layout->addWidget(confirmedLabel);
    card2Layout->addStretch();
    statsLayout->addWidget(confirmedCard);

    // Card 3: Pending
    pendingCard = new QFrame();
    pendingCard->setStyleSheet(
        "QFrame { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f59e0b, stop:1 #d97706); "
        "border-radius: 12px; "
        "padding: 15px; "
        "}");
    QVBoxLayout *card3Layout = new QVBoxLayout(pendingCard);
    card3Layout->setSpacing(8);
    QLabel *card3Title = new QLabel("⏰ Chờ xác nhận");
    card3Title->setStyleSheet("color: white; font-size: 14px; font-weight: 600;");
    pendingLabel = new QLabel("0");
    pendingLabel->setStyleSheet("color: white; font-size: 38px; font-weight: bold;");
    card3Layout->addWidget(card3Title);
    card3Layout->addWidget(pendingLabel);
    card3Layout->addStretch();
    statsLayout->addWidget(pendingCard);

    // Card 4: Revenue
    revenueCard = new QFrame();
    revenueCard->setStyleSheet(
        "QFrame { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #8b5cf6, stop:1 #7c3aed); "
        "border-radius: 12px; "
        "padding: 15px; "
        "}");
    QVBoxLayout *card4Layout = new QVBoxLayout(revenueCard);
    card4Layout->setSpacing(8);
    QLabel *card4Title = new QLabel("💰 Tổng doanh thu");
    card4Title->setStyleSheet("color: white; font-size: 14px; font-weight: 600;");
    revenueLabel = new QLabel("0đ");
    revenueLabel->setStyleSheet("color: white; font-size: 32px; font-weight: bold;");
    card4Layout->addWidget(card4Title);
    card4Layout->addWidget(revenueLabel);
    card4Layout->addStretch();
    statsLayout->addWidget(revenueCard);

    mainLayout->addWidget(statsContainer);
}

void BookingTableTab::setupTable()
{
    bookingTable = new QTableWidget();
    bookingTable->setObjectName("customerTable");
    bookingTable->setColumnCount(11);
    bookingTable->setHorizontalHeaderLabels({"Mã đặt sân", "Khách hàng", "SĐT", "Sân",
                                             "Ngày đặt", "Giờ", "Tổng tiền", "Tiền cọc", "Trạng thái", "Thanh toán", "Ghi chú"});
    QFont headerFont;
    headerFont.setBold(true);
    headerFont.setPointSize(11);

    for (int i = 0; i < bookingTable->columnCount(); i++)
    {
        QTableWidgetItem *headerItem = bookingTable->horizontalHeaderItem(i);
        if (headerItem)
        {
            headerItem->setFont(headerFont);
            headerItem->setForeground(QBrush(QColor(31, 41, 55)));
            headerItem->setBackground(QBrush(QColor(249, 250, 251)));
            headerItem->setTextAlignment(Qt::AlignCenter);
        }
    }

    bookingTable->horizontalHeader()->setStretchLastSection(true);
    bookingTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    bookingTable->setColumnWidth(0, 110); // Mã đặt sân (-10px)
    bookingTable->setColumnWidth(1, 150); // Khách hàng (-30px)
    bookingTable->setColumnWidth(2, 120); // SĐT
    bookingTable->setColumnWidth(3, 100); // Sân
    bookingTable->setColumnWidth(4, 100); // Ngày đặt
    bookingTable->setColumnWidth(5, 140); // Giờ (+40px)
    bookingTable->setColumnWidth(6, 120); // Tổng tiền
    bookingTable->setColumnWidth(7, 120); // Tiền cọc
    bookingTable->setColumnWidth(8, 120); // Trạng thái
    bookingTable->setColumnWidth(9, 130); // Thanh toán
    // Ghi chú sẽ stretch

    bookingTable->verticalHeader()->setVisible(false);
    bookingTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    bookingTable->setSelectionMode(QAbstractItemView::SingleSelection);
    bookingTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bookingTable->setAlternatingRowColors(true);
    bookingTable->setShowGrid(true);
    bookingTable->setGridStyle(Qt::SolidLine);

    // Set row height
    bookingTable->verticalHeader()->setDefaultSectionSize(48);

    // Styling
    bookingTable->setStyleSheet(
        "QTableWidget { "
        "background-color: white; "
        "border: 2px solid #e5e7eb; "
        "border-radius: 8px; "
        "gridline-color: #f3f4f6; "
        "} "
        "QTableWidget::item { "
        "padding: 8px; "
        "border-bottom: 1px solid #f3f4f6; "
        "} "
        "QTableWidget::item:selected { "
        "background-color: #dcfce7; "
        "color: #166534; "
        "} "
        "QTableWidget::item:hover { "
        "background-color: #f0fdf4; "
        "} "
        "QHeaderView::section { "
        "background-color: #f9fafb; "
        "padding: 12px 8px; "
        "border: none; "
        "border-bottom: 2px solid #e5e7eb; "
        "border-right: 1px solid #e5e7eb; "
        "font-weight: bold; "
        "color: #1f2937; "
        "}");
}

void BookingTableTab::setupConnections()
{
    // Search & Filters
    connect(searchBox, &QLineEdit::textChanged,
            this, &BookingTableTab::onSearchTextChanged);
    connect(fieldFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BookingTableTab::onFilterChanged);
    connect(statusFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BookingTableTab::onFilterChanged);
    connect(fromDateEdit, &QDateEdit::dateChanged,
            this, &BookingTableTab::onFilterChanged);
    connect(toDateEdit, &QDateEdit::dateChanged,
            this, &BookingTableTab::onFilterChanged);

    // Table double-click
    connect(bookingTable, &QTableWidget::cellDoubleClicked,
            this, &BookingTableTab::onTableRowDoubleClicked);
}

void BookingTableTab::refreshData()
{
    loadTableData();
}

void BookingTableTab::loadTableData()
{
    if (!system)
        return;

    bookingTable->setRowCount(0);

    const MangDong<DatSan *> &allBookings = system->layDanhSachDatSan();

    // Get filter values
    QString searchText = searchBox->text().toLower();
    QString fieldFilter = fieldFilterCombo->currentData().toString();
    int statusFilter = statusFilterCombo->currentData().toInt();
    QDate fromDate = fromDateEdit->date();
    QDate toDate = toDateEdit->date();

    int totalCount = 0;
    int confirmedCount = 0;
    int pendingCount = 0;
    double totalRevenue = 0.0;

    for (int i = 0; i < allBookings.size(); i++)
    {
        DatSan *booking = allBookings[i];
        if (!booking)
            continue;

        // Apply filters
        NgayGio ngayGio = booking->getThoiGianDat();
        QDate bookingDate(ngayGio.getNam(), ngayGio.getThang(), ngayGio.getNgay());

        // Date range filter
        if (bookingDate < fromDate || bookingDate > toDate)
            continue;

        // Field filter
        if (fieldFilter != "ALL")
        {
            San *san = booking->getSan();
            if (!san || QString::fromStdString(san->getMaSan()) != fieldFilter)
                continue;
        }

        // Status filter
        if (statusFilter != -1)
        {
            if (static_cast<int>(booking->getTrangThai()) != statusFilter)
                continue;
        }

        // Search filter
        if (!searchText.isEmpty())
        {
            QString maDS = QString::fromStdString(booking->getMaDatSan()).toLower();
            QString customerName = "";
            QString phone = "";
            QString fieldName = "";

            KhachHang *kh = booking->getKhachHang();
            if (kh)
            {
                customerName = QString::fromStdString(kh->getHoTen()).toLower();
                phone = QString::fromStdString(kh->getSoDienThoai()).toLower();
            }

            San *san = booking->getSan();
            if (san)
            {
                fieldName = QString::fromStdString(san->getTenSan()).toLower();
            }

            if (!maDS.contains(searchText) &&
                !customerName.contains(searchText) &&
                !phone.contains(searchText) &&
                !fieldName.contains(searchText))
                continue;
        }

        // Add row to table
        int row = bookingTable->rowCount();
        bookingTable->insertRow(row);

        // Column 0: Mã đặt sân
        QTableWidgetItem *maItem = new QTableWidgetItem(QString::fromStdString(booking->getMaDatSan()));
        maItem->setTextAlignment(Qt::AlignCenter);
        maItem->setFont(QFont("Segoe UI", 10, QFont::Bold));
        bookingTable->setItem(row, 0, maItem);

        // Column 1: Khách hàng
        QString customerName = "N/A";
        KhachHang *kh = booking->getKhachHang();
        if (kh)
        {
            customerName = QString::fromStdString(kh->getHoTen());
        }
        QTableWidgetItem *customerItem = new QTableWidgetItem(customerName);
        customerItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        bookingTable->setItem(row, 1, customerItem);

        // Column 2: SĐT
        QString phone = "N/A";
        if (kh)
        {
            phone = QString::fromStdString(kh->getSoDienThoai());
        }
        QTableWidgetItem *phoneItem = new QTableWidgetItem(phone);
        phoneItem->setTextAlignment(Qt::AlignCenter);
        bookingTable->setItem(row, 2, phoneItem);

        // Column 3: Sân
        QString fieldName = "N/A";
        San *san = booking->getSan();
        if (san)
        {
            fieldName = QString::fromStdString(san->getTenSan());
        }
        QTableWidgetItem *fieldItem = new QTableWidgetItem(fieldName);
        fieldItem->setTextAlignment(Qt::AlignCenter);
        bookingTable->setItem(row, 3, fieldItem);

        // Column 4: Ngày đặt
        QTableWidgetItem *dateItem = new QTableWidgetItem(
            bookingDate.toString("dd/MM/yyyy"));
        dateItem->setTextAlignment(Qt::AlignCenter);
        bookingTable->setItem(row, 4, dateItem);

        // Column 5: Giờ
        KhungGio kg = booking->getKhungGio();
        ThoiGian tStart = kg.layGioBatDau();
        ThoiGian tEnd = kg.layGioKetThuc();
        QString timeStr = QString("%1:%2 - %3:%4")
                              .arg(tStart.getGio(), 2, 10, QChar('0'))
                              .arg(tStart.getPhut(), 2, 10, QChar('0'))
                              .arg(tEnd.getGio(), 2, 10, QChar('0'))
                              .arg(tEnd.getPhut(), 2, 10, QChar('0'));
        QTableWidgetItem *timeItem = new QTableWidgetItem(timeStr);
        timeItem->setTextAlignment(Qt::AlignCenter);
        bookingTable->setItem(row, 5, timeItem);

        // Column 6: Tổng tiền
        QTableWidgetItem *priceItem = new QTableWidgetItem(formatCurrency(booking->getTongTien()));
        priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        priceItem->setForeground(QBrush(QColor(22, 163, 74)));
        priceItem->setFont(QFont("Segoe UI", 10, QFont::Bold));
        bookingTable->setItem(row, 6, priceItem);

        // Column 7: Tiền cọc (30%)
        QTableWidgetItem *depositItem = new QTableWidgetItem(formatCurrency(booking->getTienCoc()));
        depositItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        depositItem->setForeground(QBrush(QColor(245, 158, 11)));
        depositItem->setFont(QFont("Segoe UI", 10, QFont::Bold));
        bookingTable->setItem(row, 7, depositItem);

        // Column 8: Trạng thái đặt sân
        TrangThaiDatSan status = booking->getTrangThai();
        QTableWidgetItem *statusItem = new QTableWidgetItem(getStatusText(status));
        statusItem->setTextAlignment(Qt::AlignCenter);
        QColor statusColor = getStatusColor(status);
        statusItem->setForeground(QBrush(statusColor));
        statusItem->setFont(QFont("Segoe UI", 10, QFont::Bold));
        bookingTable->setItem(row, 8, statusItem);

        // Column 9: Thanh toán (deposit status)
        TrangThaiCoc depositStatus = booking->getTrangThaiCoc();
        QString paymentText;
        QColor paymentColor;

        // Map deposit status to payment text
        switch (depositStatus)
        {
        case DA_COC:
            paymentText = "Chỉ cọc";
            paymentColor = QColor(245, 158, 11); // Orange
            break;
        case HOAN_COC:
            paymentText = "Hoàn cọc";
            paymentColor = QColor(59, 130, 246); // Blue
            break;
        case MAT_COC:
            paymentText = "Mất cọc";
            paymentColor = QColor(239, 68, 68); // Red
            break;
        default:
            // If CHUA_COC and status is HOAN_THANH, show "Đã thanh toán"
            if (status == TrangThaiDatSan::HOAN_THANH)
            {
                paymentText = "Đã thanh toán";
                paymentColor = QColor(22, 163, 74); // Green
            }
            else
            {
                paymentText = "Chỉ cọc";
                paymentColor = QColor(245, 158, 11); // Orange
            }
            break;
        }

        QTableWidgetItem *paymentItem = new QTableWidgetItem(paymentText);
        paymentItem->setTextAlignment(Qt::AlignCenter);
        paymentItem->setForeground(QBrush(paymentColor));
        paymentItem->setFont(QFont("Segoe UI", 10, QFont::Bold));
        bookingTable->setItem(row, 9, paymentItem);

        // Column 10: Ghi chú
        QString note = QString::fromStdString(booking->getGhiChu());
        QTableWidgetItem *noteItem = new QTableWidgetItem(note);
        noteItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        noteItem->setForeground(QBrush(QColor(107, 114, 128))); // Gray
        bookingTable->setItem(row, 10, noteItem);

        // Update stats
        totalCount++;
        if (status == TrangThaiDatSan::DA_DAT)
            confirmedCount++;
        if (status == TrangThaiDatSan::DA_DAT)
            pendingCount++;
        if (status == TrangThaiDatSan::HOAN_THANH)
            totalRevenue += booking->getTongTien();
    }

    // Update stats cards
    totalLabel->setText(QString::number(totalCount));
    confirmedLabel->setText(QString::number(confirmedCount));
    pendingLabel->setText(QString::number(pendingCount));
    revenueLabel->setText(formatCurrency(totalRevenue));
}

QString BookingTableTab::getStatusText(TrangThaiDatSan status) const
{
    switch (status)
    {
    case TrangThaiDatSan::DA_DAT:
        return "🟢 Đã đặt";
    case TrangThaiDatSan::HOAN_THANH:
        return "✓ Hoàn thành";
    case TrangThaiDatSan::DA_HUY:
        return "❌ Đã hủy";
    default:
        return "N/A";
    }
}

QColor BookingTableTab::getStatusColor(TrangThaiDatSan status) const
{
    switch (status)
    {
    case TrangThaiDatSan::DA_DAT:
        return QColor(22, 163, 74); // Green
    case TrangThaiDatSan::HOAN_THANH:
        return QColor(59, 130, 246); // Blue
    case TrangThaiDatSan::DA_HUY:
        return QColor(220, 38, 38); // Red
    default:
        return QColor(107, 114, 128); // Gray
    }
}

QString BookingTableTab::getPaymentStatusText(bool paid) const
{
    return paid ? "✓ Đã thanh toán" : "⏳ Chưa thanh toán";
}

QColor BookingTableTab::getPaymentStatusColor(bool paid) const
{
    return paid ? QColor(22, 163, 74) : QColor(245, 158, 11);
}

DatSan *BookingTableTab::getBookingAtRow(int row)
{
    if (row < 0 || row >= bookingTable->rowCount())
        return nullptr;

    QString maDS = bookingTable->item(row, 0)->text();
    return system->timDatSan(maDS.toStdString());
}

QString BookingTableTab::formatCurrency(double amount) const
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(0) << amount;
    QString result = QString::fromStdString(ss.str());

    // Add thousand separators
    int pos = result.length() - 3;
    while (pos > 0)
    {
        result.insert(pos, '.');
        pos -= 3;
    }

    return result + "đ";
}

// ===== SLOTS =====

void BookingTableTab::onTableRowDoubleClicked(int row, int column)
{
    Q_UNUSED(column);

    if (row < 0 || row >= bookingTable->rowCount())
        return;

    DatSan *booking = getBookingAtRow(row);
    if (!booking)
        return;

    // Open detail dialog
    BookingDetailDialog dialog(booking, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        // Refresh table after changes
        loadTableData();
        emit bookingDataChanged();
    }
}

void BookingTableTab::onSearchTextChanged()
{
    loadTableData();
}

void BookingTableTab::onFilterChanged()
{
    loadTableData();
}
