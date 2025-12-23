/**
 * @file BookingTableTab.cpp
 * @brief Implementation of BookingTableTab - Full Table View
 */

#include "BookingTableTab.h"
#include "BookingDetailDialog.h"
#include "../../Dialogs/InvoiceDialog.h"
#include "../../../Core/Utils/InvoiceGenerator.h"
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
    searchFilterLayout->addWidget(searchBox, 3);

    // Today button (Active by default)
    todayButton = new QPushButton("Hôm nay");
    todayButton->setMinimumHeight(42);
    todayButton->setObjectName("todayButton");
    todayButton->setStyleSheet(
        "QPushButton { "
        "padding: 8px 20px; "
        "border: 2px solid #16a34a; "
        "border-radius: 6px; "
        "font-size: 14px; "
        "font-weight: 600; "
        "background-color: #16a34a; "
        "color: white; "
        "} "
        "QPushButton:hover { "
        "background-color: #15803d; "
        "border-color: #15803d; "
        "}");
    searchFilterLayout->addWidget(todayButton);

    // Tomorrow button
    tomorrowButton = new QPushButton("Ngày mai");
    tomorrowButton->setMinimumHeight(42);
    tomorrowButton->setStyleSheet(
        "QPushButton { "
        "padding: 8px 20px; "
        "border: 2px solid #e5e7eb; "
        "border-radius: 6px; "
        "font-size: 14px; "
        "font-weight: 600; "
        "background-color: white; "
        "color: #374151; "
        "} "
        "QPushButton:hover { "
        "background-color: #f9fafb; "
        "border-color: #d1d5db; "
        "}");
    searchFilterLayout->addWidget(tomorrowButton);

    // Date picker
    datePickerEdit = new QDateEdit();
    datePickerEdit->setCalendarPopup(true);
    datePickerEdit->setDate(QDate::currentDate());
    datePickerEdit->setDisplayFormat("📅 dd/MM/yyyy");
    datePickerEdit->setMinimumHeight(42);
    datePickerEdit->setStyleSheet(
        "QDateEdit { "
        "padding: 8px 15px; "
        "border: 2px solid #e5e7eb; "
        "border-radius: 6px; "
        "font-size: 14px; "
        "background-color: white; "
        "} "
        "QDateEdit:focus { "
        "border: 2px solid #16a34a; "
        "}");
    searchFilterLayout->addWidget(datePickerEdit, 1);

    // Status filter
    statusFilterCombo = new QComboBox();
    statusFilterCombo->addItem("Trạng thái: Đang phục vụ", static_cast<int>(TrangThaiDatSan::DA_DAT));
    statusFilterCombo->addItem("Trạng thái: Tất cả", -1);
    statusFilterCombo->addItem("Trạng thái: Đã hủy (Hoàn cọc)", static_cast<int>(TrangThaiDatSan::DA_HUY));
    // Removed Completed and Cancelled options as they are now archived to Invoice Page
    statusFilterCombo->setCurrentIndex(0); // Default: Đang phục vụ
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
        "} "
        "QComboBox::drop-down { "
        "border: none; "
        "width: 30px; "
        "} "
        "QComboBox::down-arrow { "
        "image: none; "
        "border-left: 5px solid transparent; "
        "border-right: 5px solid transparent; "
        "border-top: 6px solid #6b7280; "
        "width: 0; "
        "height: 0; "
        "margin-right: 8px; "
        "}");
    searchFilterLayout->addWidget(statusFilterCombo, 1);

    // Field filter
    fieldFilterCombo = new QComboBox();
    fieldFilterCombo->addItem("Sân: Tất cả", "ALL");
    for (int i = 0; i < fields.size(); i++)
    {
        San *field = fields[i];
        if (field)
        {
            QString label = QString("Sân: %1").arg(QString::fromStdString(field->getTenSan()));
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
        "} "
        "QComboBox::drop-down { "
        "border: none; "
        "width: 30px; "
        "} "
        "QComboBox::down-arrow { "
        "image: none; "
        "border-left: 5px solid transparent; "
        "border-right: 5px solid transparent; "
        "border-top: 6px solid #6b7280; "
        "width: 0; "
        "height: 0; "
        "margin-right: 8px; "
        "}");
    searchFilterLayout->addWidget(fieldFilterCombo, 1);

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

    // Card 1: TẤT CẢ (All bookings - Màu Tím/Trắng)
    totalCard = new QFrame();
    totalCard->setStyleSheet(
        "QFrame { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #8b5cf6, stop:1 #7c3aed); "
        "border-radius: 12px; "
        "padding: 15px; "
        "}");
    QVBoxLayout *card1Layout = new QVBoxLayout(totalCard);
    card1Layout->setSpacing(8);
    QLabel *card1Title = new QLabel("📋 TẤT CẢ");
    card1Title->setStyleSheet("color: white; font-size: 14px; font-weight: 600;");
    totalLabel = new QLabel("0");
    totalLabel->setStyleSheet("color: white; font-size: 38px; font-weight: bold;");
    card1Layout->addWidget(card1Title);
    card1Layout->addWidget(totalLabel);
    card1Layout->addStretch();
    statsLayout->addWidget(totalCard);

    // Card 2: ĐANG PHỤC VỤ (DA_DAT - Màu Xanh Dương)
    confirmedCard = new QFrame();
    confirmedCard->setStyleSheet(
        "QFrame { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #3b82f6, stop:1 #2563eb); "
        "border-radius: 12px; "
        "padding: 15px; "
        "}");
    QVBoxLayout *card2Layout = new QVBoxLayout(confirmedCard);
    card2Layout->setSpacing(8);
    QLabel *card2Title = new QLabel("⚽ ĐANG PHỤC VỤ");
    card2Title->setStyleSheet("color: white; font-size: 14px; font-weight: 600;");
    confirmedLabel = new QLabel("0");
    confirmedLabel->setStyleSheet("color: white; font-size: 38px; font-weight: bold;");
    card2Layout->addWidget(card2Title);
    card2Layout->addWidget(confirmedLabel);
    card2Layout->addStretch();
    statsLayout->addWidget(confirmedCard);

    // Card 3: HOÀN THÀNH (HOAN_THANH - Màu Xanh Lá)
    pendingCard = new QFrame();
    pendingCard->setStyleSheet(
        "QFrame { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #16a34a, stop:1 #15803d); "
        "border-radius: 12px; "
        "padding: 15px; "
        "}");
    QVBoxLayout *card3Layout = new QVBoxLayout(pendingCard);
    card3Layout->setSpacing(8);
    QLabel *card3Title = new QLabel("✅ HOÀN THÀNH");
    card3Title->setStyleSheet("color: white; font-size: 14px; font-weight: 600;");
    pendingLabel = new QLabel("0");
    pendingLabel->setStyleSheet("color: white; font-size: 38px; font-weight: bold;");
    card3Layout->addWidget(card3Title);
    card3Layout->addWidget(pendingLabel);
    card3Layout->addStretch();
    statsLayout->addWidget(pendingCard);

    // Card 4: ĐÃ HỦY (DA_HUY - Màu Đỏ)
    revenueCard = new QFrame();
    revenueCard->setStyleSheet(
        "QFrame { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #ef4444, stop:1 #dc2626); "
        "border-radius: 12px; "
        "padding: 15px; "
        "}");
    QVBoxLayout *card4Layout = new QVBoxLayout(revenueCard);
    card4Layout->setSpacing(8);
    QLabel *card4Title = new QLabel("❌ ĐÃ HỦY");
    card4Title->setStyleSheet("color: white; font-size: 14px; font-weight: 600;");
    revenueLabel = new QLabel("0");
    revenueLabel->setStyleSheet("color: white; font-size: 38px; font-weight: bold;");
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
                                             "Ngày đặt", "Giờ", "Thành tiền", "Tiền cọc", "Trạng thái", "Ghi chú", "Chi tiết"});
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
    bookingTable->setColumnWidth(6, 100); // Tổng tiền
    bookingTable->setColumnWidth(7, 100); // Tiền cọc
    bookingTable->setColumnWidth(8, 160); // Trạng thái (tăng để hiện "Đã hủy (Hoàn cọc)")
    bookingTable->setColumnWidth(9, 180); // Ghi chú
    bookingTable->setColumnWidth(10, 80); // Chi tiết

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
    connect(todayButton, &QPushButton::clicked,
            this, &BookingTableTab::onTodayButtonClicked);
    connect(tomorrowButton, &QPushButton::clicked,
            this, &BookingTableTab::onTomorrowButtonClicked);
    connect(datePickerEdit, &QDateEdit::dateChanged,
            this, &BookingTableTab::onDatePickerChanged);
    connect(statusFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BookingTableTab::onFilterChanged);
    connect(fieldFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
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
    QDate selectedDate = datePickerEdit->date();

    int totalCount = 0;
    int servingCount = 0;   // ĐANG PHỤC VỤ (DA_DAT)
    int completedCount = 0; // HOÀN THÀNH (HOAN_THANH)
    int cancelledCount = 0; // ĐÃ HỦY (DA_HUY)

    for (int i = 0; i < allBookings.size(); i++)
    {
        DatSan *booking = allBookings[i];
        if (!booking)
            continue;

        // Apply filters
        NgayGio ngayGio = booking->getThoiGianDat();
        QDate bookingDate(ngayGio.getNam(), ngayGio.getThang(), ngayGio.getNgay());

        // Date filter (only show bookings on selected date)
        if (bookingDate != selectedDate)
            continue;

        // Update stats (Count based on Date only, ignore other filters)
        TrangThaiDatSan currentStatus = booking->getTrangThai();
        totalCount++;
        if (currentStatus == TrangThaiDatSan::DA_DAT)
            servingCount++;
        else if (currentStatus == TrangThaiDatSan::HOAN_THANH)
            completedCount++;
        else if (currentStatus == TrangThaiDatSan::DA_HUY)
            cancelledCount++;

        // Field filter
        if (fieldFilter != "ALL")
        {
            San *san = booking->getSan();
            if (!san || QString::fromStdString(san->getMaSan()) != fieldFilter)
                continue;
        }

        // Global Archive Filter: Hide Completed and Cancelled (Lost Deposit)
        // These are considered "Archived" and moved to Invoice Page
        TrangThaiDatSan status = booking->getTrangThai();
        TrangThaiCoc depositStatus = booking->getTrangThaiCoc();

        if (status == TrangThaiDatSan::HOAN_THANH)
            continue; // Hide completed

        if (status == TrangThaiDatSan::DA_HUY && depositStatus == TrangThaiCoc::MAT_COC)
            continue; // Hide cancelled (lost deposit)

        // Status filter (Dropdown)
        if (statusFilter != -1)
        {
            // Keep the booking visible if it matches the filter OR if it was the last modified booking
            bool isLastModified = (lastModifiedBooking != nullptr && booking == lastModifiedBooking);

            if (static_cast<int>(status) != statusFilter && !isLastModified)
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

        // Column 6: Thành tiền (sau giảm giá)
        double tongTien = booking->getTongTien();
        double thanhTien = tongTien;
        KhachHang *khachHang = booking->getKhachHang();
        if (khachHang)
        {
            int phanTramGiam = khachHang->layPhanTramGiamGia();
            double giamGia = tongTien * phanTramGiam / 100.0;
            thanhTien = tongTien - giamGia;
        }
        QTableWidgetItem *priceItem = new QTableWidgetItem(formatCurrency(thanhTien));
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

        // Column 8: Trạng thái đặt sân (kết hợp với thông tin hoàn cọc)
        QString statusText;
        QColor statusColor;

        if (status == TrangThaiDatSan::DA_HUY)
        {
            // Đã hủy - hiển thị kèm thông tin hoàn cọc
            statusText = "❌ Đã hủy (Hoàn cọc)";
            statusColor = QColor(220, 38, 38); // Red
        }
        else // DA_DAT
        {
            statusText = "🟢 Đã đặt";
            statusColor = QColor(22, 163, 74); // Green
        }

        QTableWidgetItem *statusItem = new QTableWidgetItem(statusText);
        statusItem->setTextAlignment(Qt::AlignCenter);
        statusItem->setForeground(QBrush(statusColor));
        statusItem->setFont(QFont("Segoe UI", 10, QFont::Bold));
        bookingTable->setItem(row, 8, statusItem);

        // Column 9: Ghi chú
        QString note = QString::fromStdString(booking->getGhiChu());
        QTableWidgetItem *noteItem = new QTableWidgetItem(note);
        noteItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        noteItem->setForeground(QBrush(QColor(107, 114, 128))); // Gray
        bookingTable->setItem(row, 9, noteItem);

        // Column 10: Chi tiết button
        QPushButton *detailBtn = new QPushButton();
        detailBtn->setIcon(QIcon("d:/PBL2-/UI/Resources/icons/eye.svg"));
        detailBtn->setIconSize(QSize(20, 20));
        detailBtn->setFixedSize(24, 24);
        detailBtn->setCursor(Qt::PointingHandCursor);
        detailBtn->setStyleSheet(
            "QPushButton { "
            "background: none; "
            "border: none; "
            "padding: 0; "
            "} "
            "QPushButton:hover { "
            "background: none; "
            "opacity: 0.6; "
            "}");

        // Connect to open detail dialog
        connect(detailBtn, &QPushButton::clicked, this, [this, row]()
                { onTableRowDoubleClicked(row, 0); });

        // Center the button in cell (both horizontal and vertical)
        QWidget *widget = new QWidget();
        widget->setStyleSheet("background: transparent;");
        QVBoxLayout *vLayout = new QVBoxLayout(widget);
        QHBoxLayout *hLayout = new QHBoxLayout();
        hLayout->addWidget(detailBtn);
        hLayout->setAlignment(Qt::AlignCenter);
        vLayout->addLayout(hLayout);
        vLayout->setAlignment(Qt::AlignCenter);
        vLayout->setContentsMargins(0, 0, 0, 0);
        vLayout->setSpacing(0);
        widget->setLayout(vLayout);
        bookingTable->setCellWidget(row, 10, widget);
    }

    // Update stats cards
    totalLabel->setText(QString::number(totalCount));
    confirmedLabel->setText(QString::number(servingCount));
    pendingLabel->setText(QString::number(completedCount));
    revenueLabel->setText(QString::number(cancelledCount));
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

    // Check if we should show Invoice Dialog instead of Booking Detail
    // Case 1: Completed Booking -> Show Invoice
    // Case 2: Cancelled with Penalty (Lost Deposit) -> Show Penalty Invoice
    bool showInvoice = false;
    std::string invoiceContent = "";

    if (booking->getTrangThai() == TrangThaiDatSan::HOAN_THANH)
    {
        showInvoice = true;
        invoiceContent = InvoiceGenerator::generateBookingInvoice(*booking);
    }
    else if (booking->getTrangThai() == TrangThaiDatSan::DA_HUY &&
             booking->getTrangThaiCoc() == TrangThaiCoc::MAT_COC)
    {
        showInvoice = true;
        invoiceContent = InvoiceGenerator::generateCancellationInvoice(*booking, booking->getLyDoHuy());
    }

    if (showInvoice && !invoiceContent.empty())
    {
        InvoiceDialog dialog(invoiceContent, this);
        dialog.setReadOnly(true); // Invoices from history are read-only
        dialog.exec();
        return; // No changes possible, so no need to refresh
    }

    // Open detail dialog (For Active bookings or Refunded cancellations)
    BookingDetailDialog dialog(booking, this);

    // Connect reschedule signal
    connect(&dialog, &BookingDetailDialog::rescheduleRequested, this, [this](DatSan *booking)
            { emit rescheduleRequested(booking); });

    if (dialog.exec() == QDialog::Accepted)
    {
        // Track this booking to keep it visible even if status changed
        lastModifiedBooking = booking;

        // Refresh table after changes
        loadTableData();
        emit bookingDataChanged();
    }
}

void BookingTableTab::onSearchTextChanged()
{
    lastModifiedBooking = nullptr;
    loadTableData();
}

void BookingTableTab::onFilterChanged()
{
    lastModifiedBooking = nullptr;
    loadTableData();
}

void BookingTableTab::onTodayButtonClicked()
{
    lastModifiedBooking = nullptr;
    datePickerEdit->setDate(QDate::currentDate());

    // Auto-select status: today/future = "Đang phục vụ"
    statusFilterCombo->setCurrentIndex(0); // Đang phục vụ

    // Update button styles to show active state
    todayButton->setStyleSheet(
        "QPushButton { "
        "padding: 8px 20px; "
        "border: 2px solid #16a34a; "
        "border-radius: 6px; "
        "font-size: 14px; "
        "font-weight: 600; "
        "background-color: #16a34a; "
        "color: white; "
        "} "
        "QPushButton:hover { "
        "background-color: #15803d; "
        "border-color: #15803d; "
        "}");

    tomorrowButton->setStyleSheet(
        "QPushButton { "
        "padding: 8px 20px; "
        "border: 2px solid #e5e7eb; "
        "border-radius: 6px; "
        "font-size: 14px; "
        "font-weight: 600; "
        "background-color: white; "
        "color: #374151; "
        "} "
        "QPushButton:hover { "
        "background-color: #f9fafb; "
        "border-color: #d1d5db; "
        "}");

    loadTableData();
}

void BookingTableTab::onTomorrowButtonClicked()
{
    lastModifiedBooking = nullptr;
    datePickerEdit->setDate(QDate::currentDate().addDays(1));

    // Auto-select status: today/future = "Đang phục vụ"
    statusFilterCombo->setCurrentIndex(0); // Đang phục vụ

    // Update button styles to show active state
    tomorrowButton->setStyleSheet(
        "QPushButton { "
        "padding: 8px 20px; "
        "border: 2px solid #16a34a; "
        "border-radius: 6px; "
        "font-size: 14px; "
        "font-weight: 600; "
        "background-color: #16a34a; "
        "color: white; "
        "} "
        "QPushButton:hover { "
        "background-color: #15803d; "
        "border-color: #15803d; "
        "}");

    todayButton->setStyleSheet(
        "QPushButton { "
        "padding: 8px 20px; "
        "border: 2px solid #e5e7eb; "
        "border-radius: 6px; "
        "font-size: 14px; "
        "font-weight: 600; "
        "background-color: white; "
        "color: #374151; "
        "} "
        "QPushButton:hover { "
        "background-color: #f9fafb; "
        "border-color: #d1d5db; "
        "}");

    loadTableData();
}

void BookingTableTab::onDatePickerChanged()
{
    lastModifiedBooking = nullptr;
    // Auto-select status based on date: past = "Tất cả", today/future = "Đang phục vụ"
    QDate selectedDate = datePickerEdit->date();
    QDate today = QDate::currentDate();

    if (selectedDate < today)
    {
        // Past date: show all statuses (because all orders in the past are completed)
        statusFilterCombo->setCurrentIndex(1); // Tất cả
    }
    else
    {
        // Today or future: show only "serving" status
        statusFilterCombo->setCurrentIndex(0); // Đang phục vụ
    }

    // Update button visual states
    QDate tomorrow = today.addDays(1);

    if (selectedDate == today)
    {
        todayButton->setStyleSheet(
            "QPushButton { "
            "padding: 8px 20px; "
            "border: 2px solid #16a34a; "
            "border-radius: 6px; "
            "font-size: 14px; "
            "font-weight: 600; "
            "background-color: #16a34a; "
            "color: white; "
            "} "
            "QPushButton:hover { "
            "background-color: #15803d; "
            "border-color: #15803d; "
            "}");
        tomorrowButton->setStyleSheet(
            "QPushButton { "
            "padding: 8px 20px; "
            "border: 2px solid #e5e7eb; "
            "border-radius: 6px; "
            "font-size: 14px; "
            "font-weight: 600; "
            "background-color: white; "
            "color: #374151; "
            "} "
            "QPushButton:hover { "
            "background-color: #f9fafb; "
            "border-color: #d1d5db; "
            "}");
    }
    else if (selectedDate == tomorrow)
    {
        tomorrowButton->setStyleSheet(
            "QPushButton { "
            "padding: 8px 20px; "
            "border: 2px solid #16a34a; "
            "border-radius: 6px; "
            "font-size: 14px; "
            "font-weight: 600; "
            "background-color: #16a34a; "
            "color: white; "
            "} "
            "QPushButton:hover { "
            "background-color: #15803d; "
            "border-color: #15803d; "
            "}");
        todayButton->setStyleSheet(
            "QPushButton { "
            "padding: 8px 20px; "
            "border: 2px solid #e5e7eb; "
            "border-radius: 6px; "
            "font-size: 14px; "
            "font-weight: 600; "
            "background-color: white; "
            "color: #374151; "
            "} "
            "QPushButton:hover { "
            "background-color: #f9fafb; "
            "border-color: #d1d5db; "
            "}");
    }
    else
    {
        // Both inactive for other dates
        todayButton->setStyleSheet(
            "QPushButton { "
            "padding: 8px 20px; "
            "border: 2px solid #e5e7eb; "
            "border-radius: 6px; "
            "font-size: 14px; "
            "font-weight: 600; "
            "background-color: white; "
            "color: #374151; "
            "} "
            "QPushButton:hover { "
            "background-color: #f9fafb; "
            "border-color: #d1d5db; "
            "}");
        tomorrowButton->setStyleSheet(
            "QPushButton { "
            "padding: 8px 20px; "
            "border: 2px solid #e5e7eb; "
            "border-radius: 6px; "
            "font-size: 14px; "
            "font-weight: 600; "
            "background-color: white; "
            "color: #374151; "
            "} "
            "QPushButton:hover { "
            "background-color: #f9fafb; "
            "border-color: #d1d5db; "
            "}");
    }

    loadTableData();
}
