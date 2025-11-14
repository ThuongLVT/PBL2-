/**
 * @file BookingPage.cpp
 * @brief Implementation of BookingPage
 */

#include "BookingPage.h"
#include "TimelineTab.h"
#include "../../../Core/QuanLy/HeThongQuanLy.h"
#include "../../../Core/QuanLy/QuanLyDatSan.h"
#include "../../../Core/QuanLy/QuanLyKhachHang.h"
#include "../../../Core/QuanLy/QuanLySan.h"
#include "../../../Core/Models/DatSan.h"
#include "../../../Core/Models/KhachHang.h"
#include "../../../Core/Models/San.h"
#include <QLabel>
#include <QMessageBox>
#include <QDate>

BookingPage::BookingPage(QWidget *parent)
    : QWidget(parent), mainLayout(nullptr), tabWidget(nullptr), timelineTab(nullptr), tableTab(nullptr)
{
    setupUI();
    setupConnections();
}

BookingPage::~BookingPage()
{
    // Qt handles cleanup via parent-child relationship
}

void BookingPage::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Create tab widget
    tabWidget = new QTabWidget(this);
    tabWidget->setObjectName("bookingTabWidget");

    // Tab 1: Timeline View
    timelineTab = new TimelineTab(this);
    tabWidget->addTab(timelineTab, "📅 Timeline View");

    // Tab 2: Table View
    tableTab = new QWidget(this);
    QVBoxLayout *tableLayout = new QVBoxLayout(tableTab);
    tableLayout->setContentsMargins(20, 20, 20, 20);
    tableLayout->setSpacing(15);
    
    // Filter Panel
    QWidget *filterPanel = new QWidget();
    filterPanel->setStyleSheet("QWidget { background: white; border-radius: 8px; padding: 15px; }");
    QGridLayout *filterLayout = new QGridLayout(filterPanel);
    
    // Date Range
    filterLayout->addWidget(new QLabel("Từ ngày:"), 0, 0);
    startDateEdit = new QDateEdit(QDate::currentDate());
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDisplayFormat("dd/MM/yyyy");
    filterLayout->addWidget(startDateEdit, 0, 1);
    
    filterLayout->addWidget(new QLabel("Đến ngày:"), 0, 2);
    endDateEdit = new QDateEdit(QDate::currentDate().addDays(7));
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDisplayFormat("dd/MM/yyyy");
    filterLayout->addWidget(endDateEdit, 0, 3);
    
    // Field Filter
    filterLayout->addWidget(new QLabel("Sân:"), 1, 0);
    fieldFilterCombo = new QComboBox();
    fieldFilterCombo->addItem("-- Tất cả --", "");
    filterLayout->addWidget(fieldFilterCombo, 1, 1);
    
    // Status Filter
    filterLayout->addWidget(new QLabel("Trạng thái:"), 1, 2);
    statusFilterCombo = new QComboBox();
    statusFilterCombo->addItem("-- Tất cả --", "");
    statusFilterCombo->addItem("Đã xác nhận", "confirmed");
    statusFilterCombo->addItem("Chưa checkin", "not_checkin");
    statusFilterCombo->addItem("Đã checkin", "checkin");
    statusFilterCombo->addItem("Đã hoàn thành", "completed");
    statusFilterCombo->addItem("Đã hủy", "cancelled");
    filterLayout->addWidget(statusFilterCombo, 1, 3);
    
    // Search Box
    filterLayout->addWidget(new QLabel("Tìm kiếm:"), 2, 0);
    searchBox = new QLineEdit();
    searchBox->setPlaceholderText("Tên KH, SĐT, Mã đặt...");
    filterLayout->addWidget(searchBox, 2, 1, 1, 2);
    
    QPushButton *filterBtn = new QPushButton("Lọc");
    filterBtn->setStyleSheet("QPushButton { background: #16a34a; color: white; padding: 8px 24px; border-radius: 6px; } QPushButton:hover { background: #15803d; }");
    connect(filterBtn, &QPushButton::clicked, this, &BookingPage::applyTableFilters);
    filterLayout->addWidget(filterBtn, 2, 3);
    
    tableLayout->addWidget(filterPanel);
    
    // Table Widget
    bookingTable = new QTableWidget();
    bookingTable->setColumnCount(8);
    bookingTable->setHorizontalHeaderLabels({"Mã đặt", "Sân", "Khách hàng", "SĐT", "Thời gian", "Trạng thái", "Tổng tiền", "Hành động"});
    bookingTable->horizontalHeader()->setStretchLastSection(false);
    bookingTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    bookingTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    bookingTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    bookingTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    bookingTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    bookingTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    bookingTable->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    bookingTable->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);
    bookingTable->setColumnWidth(7, 200);
    bookingTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    bookingTable->setSelectionMode(QAbstractItemView::SingleSelection);
    bookingTable->setAlternatingRowColors(true);
    bookingTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bookingTable->verticalHeader()->setVisible(false);
    bookingTable->setStyleSheet(
        "QTableWidget { border: 1px solid #e5e7eb; border-radius: 8px; background: white; gridline-color: #f3f4f6; }"
        "QHeaderView::section { background: #f9fafb; padding: 8px; border: none; font-weight: bold; }"
        "QTableWidget::item { padding: 8px; }"
        "QTableWidget::item:alternate { background: #f9fafb; }"
        "QTableWidget::item:selected { background: #dbeafe; }"
    );
    
    tableLayout->addWidget(bookingTable);
    
    // Connect double-click to view details
    connect(bookingTable, &QTableWidget::cellDoubleClicked, this, &BookingPage::onTableRowDoubleClicked);
    
    tabWidget->addTab(tableTab, "📋 Danh sách");

    mainLayout->addWidget(tabWidget);
}

void BookingPage::setupConnections()
{
    // Connect TimelineTab signal to refresh table
    if (timelineTab) {
        connect(timelineTab, &TimelineTab::bookingDataChanged, this, &BookingPage::loadTableData);
    }
    
    // Connect filter changes
    connect(startDateEdit, &QDateEdit::dateChanged, this, &BookingPage::applyTableFilters);
    connect(endDateEdit, &QDateEdit::dateChanged, this, &BookingPage::applyTableFilters);
    connect(fieldFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BookingPage::applyTableFilters);
    connect(statusFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BookingPage::applyTableFilters);
    connect(searchBox, &QLineEdit::textChanged, this, &BookingPage::applyTableFilters);
}

void BookingPage::refreshData()
{
    // Refresh timeline tab data when page becomes visible
    if (timelineTab)
    {
        timelineTab->refreshData();
    }
    
    // Load fields to filter
    loadFieldsToFilter();
    
    // Load table data
    loadTableData();
}

void BookingPage::loadFieldsToFilter()
{
    if (!fieldFilterCombo) return;
    
    fieldFilterCombo->clear();
    fieldFilterCombo->addItem("-- Tất cả --", "");
    
    HeThongQuanLy *system = HeThongQuanLy::getInstance();
    QuanLySan *qlsan = system->layQuanLySan();
    
    const MangDong<San *> &dsSan = qlsan->layDanhSachSan();
    for (int i = 0; i < dsSan.size(); i++)
    {
        San *san = dsSan[i];
        if (san)
        {
            fieldFilterCombo->addItem(QString::fromStdString(san->layTenSan()), QString::fromStdString(san->layMaSan()));
        }
    }
}

void BookingPage::applyTableFilters()
{
    loadTableData();
}

void BookingPage::loadTableData()
{
    if (!bookingTable) return;
    
    bookingTable->setRowCount(0);
    
    HeThongQuanLy *system = HeThongQuanLy::getInstance();
    QuanLyDatSan *qlds = system->layQuanLyDatSan();
    QuanLyKhachHang *qlkh = system->layQuanLyKhachHang();
    QuanLySan *qlsan = system->layQuanLySan();
    
    // Get filter values
    QDate startDate = startDateEdit->date();
    QDate endDate = endDateEdit->date();
    QString fieldFilter = fieldFilterCombo->currentData().toString();
    QString statusFilter = statusFilterCombo->currentData().toString();
    QString searchText = searchBox->text().toLower();
    
    // Load bookings
    const MangDong<DatSan *> &dsDatSan = qlds->layDanhSachDatSan();
    for (int i = 0; i < dsDatSan.size(); i++)
    {
        DatSan *ds = dsDatSan[i];
        if (!ds) continue;
        
        // Apply date filter
        NgayGio ngayGio = ds->getThoiGianDat();
        QDate bookingDate(ngayGio.getNam(), ngayGio.getThang(), ngayGio.getNgay());
        if (bookingDate < startDate || bookingDate > endDate)
            continue;
        
        // Apply field filter
        if (!fieldFilter.isEmpty() && QString::fromStdString(ds->getMaSan()) != fieldFilter)
            continue;
        
        // Apply status filter
        if (!statusFilter.isEmpty())
        {
            int status = static_cast<int>(ds->getTrangThai());
            if ((statusFilter == "confirmed" && status != 0) ||
                (statusFilter == "not_checkin" && status != 1) ||
                (statusFilter == "checkin" && status != 2) ||
                (statusFilter == "completed" && status != 3) ||
                (statusFilter == "cancelled" && status != 4))
                continue;
        }
        
        // Get related data
        KhachHang *kh = ds->getKhachHang();
        San *san = ds->getSan();
        
        QString khName = kh ? QString::fromStdString(kh->layHoTen()) : "N/A";
        QString sdt = kh ? QString::fromStdString(kh->getSoDienThoai()) : "N/A";
        QString sanName = san ? QString::fromStdString(san->layTenSan()) : "N/A";
        
        // Apply search filter
        if (!searchText.isEmpty())
        {
            QString maDat = QString::fromStdString(ds->getMaDatSan());
            if (!khName.toLower().contains(searchText) && 
                !sdt.contains(searchText) && 
                !maDat.toLower().contains(searchText))
                continue;
        }
        
        // Add row
        int row = bookingTable->rowCount();
        bookingTable->insertRow(row);
        
        // Column 0: Mã đặt
        bookingTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(ds->getMaDatSan())));
        
        // Column 1: Sân
        bookingTable->setItem(row, 1, new QTableWidgetItem(sanName));
        
        // Column 2: Khách hàng
        bookingTable->setItem(row, 2, new QTableWidgetItem(khName));
        
        // Column 3: SĐT
        bookingTable->setItem(row, 3, new QTableWidgetItem(sdt));
        
        // Column 4: Thời gian
        KhungGio khungGio = ds->getKhungGio();
        QString timeStr = QString("%1/%2/%3 %4:%5 - %6h")
            .arg(ngayGio.getNgay(), 2, 10, QChar('0'))
            .arg(ngayGio.getThang(), 2, 10, QChar('0'))
            .arg(ngayGio.getNam())
            .arg(ngayGio.getGio(), 2, 10, QChar('0'))
            .arg(ngayGio.getPhut(), 2, 10, QChar('0'))
            .arg(khungGio.tinhSoGio(), 0, 'f', 1);
        bookingTable->setItem(row, 4, new QTableWidgetItem(timeStr));
        
        // Column 5: Trạng thái (with color badge)
        QTableWidgetItem *statusItem = new QTableWidgetItem(getStatusText(static_cast<int>(ds->getTrangThai())));
        statusItem->setForeground(getStatusColor(static_cast<int>(ds->getTrangThai())));
        QFont font = statusItem->font();
        font.setBold(true);
        statusItem->setFont(font);
        bookingTable->setItem(row, 5, statusItem);
        
        // Column 6: Tổng tiền
        bookingTable->setItem(row, 6, new QTableWidgetItem(QString::number(ds->getTongTien()) + " đ"));
        
        // Column 7: Action buttons
        QWidget *actionWidget = new QWidget();
        QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
        actionLayout->setContentsMargins(2, 2, 2, 2);
        actionLayout->setSpacing(4);
        
        QPushButton *viewBtn = new QPushButton("Xem");
        viewBtn->setStyleSheet("QPushButton { background: #3b82f6; color: white; padding: 4px 8px; border-radius: 4px; } QPushButton:hover { background: #2563eb; }");
        viewBtn->setProperty("row", row);
        connect(viewBtn, &QPushButton::clicked, [this, row]() { onViewBooking(row); });
        actionLayout->addWidget(viewBtn);
        
        QPushButton *payBtn = new QPushButton("💰");
        payBtn->setToolTip("Thanh toán");
        payBtn->setStyleSheet("QPushButton { background: #16a34a; color: white; padding: 4px 8px; border-radius: 4px; } QPushButton:hover { background: #15803d; }");
        connect(payBtn, &QPushButton::clicked, [this, row]() { onPaymentBooking(row); });
        actionLayout->addWidget(payBtn);
        
        QPushButton *cancelBtn = new QPushButton("❌");
        cancelBtn->setToolTip("Hủy");
        cancelBtn->setStyleSheet("QPushButton { background: #ef4444; color: white; padding: 4px 8px; border-radius: 4px; } QPushButton:hover { background: #dc2626; }");
        connect(cancelBtn, &QPushButton::clicked, [this, row]() { onCancelBooking(row); });
        actionLayout->addWidget(cancelBtn);
        
        bookingTable->setCellWidget(row, 7, actionWidget);
    }
}

QString BookingPage::getStatusText(int status) const
{
    switch (status)
    {
        case 0: return "✓ Đã xác nhận";
        case 1: return "⏰ Chưa checkin";
        case 2: return "✓ Đã checkin";
        case 3: return "✓ Hoàn thành";
        case 4: return "❌ Đã hủy";
        default: return "N/A";
    }
}

QColor BookingPage::getStatusColor(int status) const
{
    switch (status)
    {
        case 0: return QColor(22, 163, 74);    // Green - confirmed
        case 1: return QColor(245, 158, 11);   // Yellow - pending
        case 2: return QColor(59, 130, 246);   // Blue - checkin
        case 3: return QColor(107, 114, 128);  // Gray - completed
        case 4: return QColor(239, 68, 68);    // Red - cancelled
        default: return QColor(0, 0, 0);
    }
}

void BookingPage::onViewBooking(int row)
{
    QString maDat = bookingTable->item(row, 0)->text();
    QMessageBox::information(this, "Chi tiết đặt sân", 
        QString("Mã đặt: %1\n\nChức năng xem chi tiết đang được phát triển...").arg(maDat));
}

void BookingPage::onEditBooking(int row)
{
    QString maDat = bookingTable->item(row, 0)->text();
    QMessageBox::information(this, "Chỉnh sửa", 
        QString("Mã đặt: %1\n\nChức năng chỉnh sửa đang được phát triển...").arg(maDat));
}

void BookingPage::onCancelBooking(int row)
{
    QString maDat = bookingTable->item(row, 0)->text();
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Xác nhận hủy",
        QString("Bạn có chắc muốn hủy lịch đặt '%1'?").arg(maDat),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes)
    {
        HeThongQuanLy *system = HeThongQuanLy::getInstance();
        QuanLyDatSan *qlds = system->layQuanLyDatSan();
        
        DatSan *ds = qlds->timDatSan(maDat.toStdString());
        if (ds)
        {
            ds->setTrangThai(static_cast<TrangThaiDatSan>(4)); // Cancelled
            system->luuHeThong("D:/QT_PBL2/Data/data.bin");
            loadTableData(); // Refresh
            QMessageBox::information(this, "Thành công", "Đã hủy lịch đặt!");
        }
    }
}

void BookingPage::onPaymentBooking(int row)
{
    QString maDat = bookingTable->item(row, 0)->text();
    
    HeThongQuanLy *system = HeThongQuanLy::getInstance();
    QuanLyDatSan *qlds = system->layQuanLyDatSan();
    
    DatSan *ds = qlds->timDatSan(maDat.toStdString());
    if (ds)
    {
        double tongTien = ds->getTongTien();
        
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Xác nhận thanh toán",
            QString("Tổng tiền: %1 đ\n\nXác nhận thanh toán?").arg(tongTien),
            QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::Yes)
        {
            ds->setTrangThai(static_cast<TrangThaiDatSan>(3)); // Completed/Paid
            system->luuHeThong("D:/QT_PBL2/Data/data.bin");
            loadTableData(); // Refresh
            QMessageBox::information(this, "Thành công", "Đã thanh toán!");
        }
    }
}

void BookingPage::onAddServices(int row)
{
    QString maDat = bookingTable->item(row, 0)->text();
    QMessageBox::information(this, "Thêm dịch vụ", 
        QString("Mã đặt: %1\n\nChức năng thêm dịch vụ đang được phát triển...").arg(maDat));
}

void BookingPage::onTableRowDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    onViewBooking(row);
}
