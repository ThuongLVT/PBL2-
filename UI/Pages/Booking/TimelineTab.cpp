/**
 * @file TimelineTab.cpp
 * @brief Implementation of TimelineTab
 */

#include "TimelineTab.h"
#include "TimelineGridWidget.h"
#include "../../../UI/Dialogs/AddCustomerDialog.h"
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QDateTime>
#include <QScrollArea>
#include <QDialog>
#include <QListWidget>
#include <QDebug>
#include <QSet>
#include <QTextCharFormat>

TimelineTab::TimelineTab(QWidget *parent)
    : QWidget(parent), mainLayout(nullptr), verticalSplitter(nullptr), topPanel(nullptr), topPanelLayout(nullptr), calendarPanel(nullptr), calendarLayout(nullptr), calendar(nullptr), formPanel(nullptr), formLayout(nullptr), phoneEdit(nullptr), addCustomerBtn(nullptr), nameLabel(nullptr), fieldCombo(nullptr), priceLabel(nullptr), statusCombo(nullptr), typeCombo(nullptr), dateEdit(nullptr), fromTimeEdit(nullptr), toTimeEdit(nullptr), durationLabel(nullptr), duration30Btn(nullptr), duration60Btn(nullptr), duration90Btn(nullptr), duration120Btn(nullptr), noteEdit(nullptr), fieldFilterCombo(nullptr), statusFilterCombo(nullptr), timeFilterCombo(nullptr), saveBtn(nullptr), deleteBtn(nullptr), checkinBtn(nullptr), timelinePanel(nullptr), timelineLayout(nullptr), timelineGrid(nullptr), system(nullptr), currentBooking(nullptr), isEditMode(false)
{
    system = HeThongQuanLy::getInstance();
    selectedDate = QDate::currentDate();

    setupUI();
    setupConnections();
    loadFields();
    clearForm();
}

TimelineTab::~TimelineTab()
{
    // Qt handles cleanup
}

void TimelineTab::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // Vertical splitter (top 40% / bottom 60%)
    verticalSplitter = new QSplitter(Qt::Vertical, this);

    setupCalendarPanel();
    setupFormPanel();

    // Top panel (calendar + form) with 30/70 ratio
    topPanel = new QWidget(this);
    topPanelLayout = new QHBoxLayout(topPanel);
    topPanelLayout->setContentsMargins(0, 0, 0, 0);
    topPanelLayout->setSpacing(10);

    // Set sizes for 30:70 ratio
    topPanelLayout->addWidget(calendarPanel, 30); // 30%
    topPanelLayout->addWidget(formPanel, 70);     // 70%

    setupTimelinePanel();

    // Add to splitter
    verticalSplitter->addWidget(topPanel);
    verticalSplitter->addWidget(timelinePanel);

    // Set initial sizes for 40/60 ratio
    // Calculate based on a reference height (e.g., 1000px)
    QList<int> sizes;
    sizes << 400 << 600; // 40% : 60%
    verticalSplitter->setSizes(sizes);

    // Set stretch factors to maintain 40/60 ratio when resizing
    verticalSplitter->setStretchFactor(0, 40); // Top panel: 40%
    verticalSplitter->setStretchFactor(1, 60); // Timeline panel: 60%

    // Prevent collapsing
    verticalSplitter->setCollapsible(0, false);
    verticalSplitter->setCollapsible(1, false);

    mainLayout->addWidget(verticalSplitter);
}

void TimelineTab::setupCalendarPanel()
{
    calendarPanel = new QFrame(this);
    calendarPanel->setObjectName("calendarPanel");
    // Remove maxWidth to allow calendar to use 30% of available space
    calendarPanel->setStyleSheet(
        "#calendarPanel { "
        "background-color: white; "
        "border: 1px solid #e5e7eb; "
        "border-radius: 8px; "
        "}");

    calendarLayout = new QVBoxLayout(calendarPanel);
    calendarLayout->setContentsMargins(6, 6, 6, 6);

    // Calendar widget
    calendar = new QCalendarWidget(calendarPanel);
    calendar->setObjectName("bookingCalendar");
    calendar->setGridVisible(true);
    calendar->setSelectedDate(selectedDate);
    calendar->setMinimumDate(QDate::currentDate());
    calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    // Remove fixed height to allow calendar to expand
    calendar->setMinimumHeight(200);

    // Calendar styling
    calendar->setStyleSheet(
        "QCalendarWidget QWidget { "
        "background-color: white; "
        "} "
        "QCalendarWidget QAbstractItemView:enabled { "
        "background-color: white; "
        "selection-background-color: #16a34a; "
        "selection-color: white; "
        "} "
        "QCalendarWidget QToolButton { "
        "color: #1f2937; "
        "background-color: white; "
        "border: none; "
        "border-radius: 4px; "
        "} "
        "QCalendarWidget QToolButton:hover { "
        "background-color: #f3f4f6; "
        "} "
        "QCalendarWidget QMenu { "
        "background-color: white; "
        "} "
        "QCalendarWidget QSpinBox { "
        "background-color: white; "
        "} ");

    calendarLayout->addWidget(calendar);
}

void TimelineTab::setupFormPanel()
{
    formPanel = new QFrame(this);
    formPanel->setObjectName("formPanel");
    formPanel->setStyleSheet(
        "#formPanel { "
        "background-color: white; "
        "border: 1px solid #e5e7eb; "
        "border-radius: 8px; "
        "}");

    formLayout = new QVBoxLayout(formPanel);
    formLayout->setContentsMargins(10, 10, 10, 10);
    formLayout->setSpacing(6);

    // Title
    QLabel *titleLabel = new QLabel("Thông tin đặt sân", formPanel);
    titleLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #1f2937;");
    formLayout->addWidget(titleLabel);

    // LEFT COLUMN
    QVBoxLayout *leftColumnLayout = new QVBoxLayout();
    leftColumnLayout->setSpacing(4);

    // Customer dropdown with quick add button
    QLabel *phoneLabel = new QLabel("Khách hàng:", formPanel);
    phoneLabel->setStyleSheet("font-size: 11px; font-weight: bold; color: #1f2937;");
    leftColumnLayout->addWidget(phoneLabel);

    QHBoxLayout *phoneLayout = new QHBoxLayout();
    phoneLayout->setSpacing(4);
    
    // Replace QLineEdit with QComboBox
    phoneEdit = new QLineEdit(formPanel); // Keep for compatibility but hide
    phoneEdit->setVisible(false);
    
    QComboBox *customerCombo = new QComboBox(formPanel);
    customerCombo->setObjectName("customerCombo");
    customerCombo->setEditable(true);
    customerCombo->setPlaceholderText("Chọn khách hàng...");
    customerCombo->setStyleSheet(
        "QComboBox { "
        "padding: 6px; "
        "border: 1px solid #d1d5db; "
        "border-radius: 4px; "
        "background-color: white; "
        "font-size: 12px; "
        "}");
    
    // Store combo reference (add to header as member variable)
    customerCombo->setProperty("phoneEdit", QVariant::fromValue((QObject*)phoneEdit));
    
    addCustomerBtn = new QPushButton("+ KH", formPanel);
    addCustomerBtn->setMaximumWidth(50);
    addCustomerBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #3b82f6; "
        "color: white; "
        "border: none; "
        "border-radius: 4px; "
        "padding: 6px 8px; "
        "font-weight: bold; "
        "font-size: 10px; "
        "} "
        "QPushButton:hover { "
        "background-color: #2563eb; "
        "}");
    
    phoneLayout->addWidget(customerCombo, 1);
    phoneLayout->addWidget(addCustomerBtn);
    leftColumnLayout->addLayout(phoneLayout);
    
    // Connect customer selection
    connect(customerCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this, customerCombo](int index) {
        if (index <= 0) {
            phoneEdit->clear();
            nameLabel->setText("-");
            return;
        }
        
        QString phone = customerCombo->currentData().toString();
        if (phone.isEmpty()) {
            phoneEdit->clear();
            nameLabel->setText("-");
            return;
        }
        
        // CRITICAL: Update phoneEdit (used in onSaveClicked validation)
        phoneEdit->setText(phone);
        phoneEdit->setProperty("hasCustomer", true);
        
        // Update name label
        QuanLyKhachHang *qlkh = system->layQuanLyKhachHang();
        if (qlkh) {
            KhachHang *customer = qlkh->timKhachHangTheoSDT(phone.toStdString());
            if (customer) {
                nameLabel->setText(QString::fromStdString(customer->layHoTen()));
            } else {
                nameLabel->setText("(Không tìm thấy)");
            }
        }
    });

    // Name (read-only)
    QLabel *nameFieldLabel = new QLabel("Họ tên:", formPanel);
    nameFieldLabel->setStyleSheet("font-size: 11px; font-weight: bold; color: #1f2937; margin-top: 4px;");
    leftColumnLayout->addWidget(nameFieldLabel);

    nameLabel = new QLabel("-", formPanel);
    nameLabel->setStyleSheet(
        "color: #6b7280; "
        "padding: 6px; "
        "border: 1px solid #e5e7eb; "
        "border-radius: 4px; "
        "background-color: #f9fafb; "
        "font-size: 11px;");
    leftColumnLayout->addWidget(nameLabel);

    // Price (read-only)
    QLabel *priceFieldLabel = new QLabel("Giá sân:", formPanel);
    priceFieldLabel->setStyleSheet("font-size: 11px; font-weight: bold; color: #1f2937; margin-top: 4px;");
    leftColumnLayout->addWidget(priceFieldLabel);

    priceLabel = new QLabel("100,000 VND", formPanel);
    priceLabel->setStyleSheet(
        "color: #16a34a; "
        "font-weight: bold; "
        "padding: 6px; "
        "border: 1px solid #d1fae5; "
        "border-radius: 4px; "
        "background-color: #f0fdf4; "
        "font-size: 11px;");
    leftColumnLayout->addWidget(priceLabel);

    // Status (read-only)
    QLabel *statusLabel = new QLabel("Trạng thái:", formPanel);
    statusLabel->setStyleSheet("font-size: 11px; font-weight: bold; color: #1f2937; margin-top: 4px;");
    leftColumnLayout->addWidget(statusLabel);

    statusCombo = new QComboBox(formPanel);
    statusCombo->addItem("Chờ xác nhận");
    statusCombo->addItem("Đã xác nhận");
    statusCombo->setEnabled(false);
    statusCombo->setStyleSheet(
        "QComboBox { "
        "padding: 6px; "
        "border: 1px solid #e5e7eb; "
        "border-radius: 4px; "
        "background-color: #f9fafb; "
        "color: #6b7280; "
        "font-size: 11px; "
        "}");
    leftColumnLayout->addWidget(statusCombo);

    leftColumnLayout->addStretch();

    // RIGHT COLUMN
    QVBoxLayout *rightColumnLayout = new QVBoxLayout();
    rightColumnLayout->setSpacing(4);

    // Field selection (read-only)
    QLabel *fieldLabel = new QLabel("Sân:", formPanel);
    fieldLabel->setStyleSheet("font-size: 11px; font-weight: bold; color: #1f2937;");
    rightColumnLayout->addWidget(fieldLabel);

    fieldCombo = new QComboBox(formPanel);
    fieldCombo->setEnabled(false);
    fieldCombo->setStyleSheet(
        "QComboBox { "
        "padding: 6px; "
        "border: 1px solid #e5e7eb; "
        "border-radius: 4px; "
        "background-color: #f9fafb; "
        "color: #6b7280; "
        "font-size: 11px; "
        "}");
    rightColumnLayout->addWidget(fieldCombo);

    // Type (read-only)
    QLabel *typeLabel = new QLabel("Loại sân:", formPanel);
    typeLabel->setStyleSheet("font-size: 11px; font-weight: bold; color: #1f2937; margin-top: 4px;");
    rightColumnLayout->addWidget(typeLabel);

    typeCombo = new QComboBox(formPanel);
    typeCombo->addItem("Sân 5 người");
    typeCombo->addItem("Sân 7 người");
    typeCombo->setEnabled(false);
    typeCombo->setStyleSheet(
        "QComboBox { "
        "padding: 6px; "
        "border: 1px solid #e5e7eb; "
        "border-radius: 4px; "
        "background-color: #f9fafb; "
        "color: #6b7280; "
        "font-size: 11px; "
        "}");
    rightColumnLayout->addWidget(typeCombo);

    // Date (hidden - used for data binding only)
    dateEdit = new QDateEdit(formPanel);
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setVisible(false); // Hidden, just for data binding

    // Time display (read-only)
    QLabel *timeLabel = new QLabel("Giờ đá:", formPanel);
    timeLabel->setStyleSheet("font-size: 11px; font-weight: bold; color: #1f2937; margin-top: 4px;");
    rightColumnLayout->addWidget(timeLabel);

    QHBoxLayout *timeDisplayLayout = new QHBoxLayout();
    timeDisplayLayout->setSpacing(4);

    fromTimeEdit = new QTimeEdit(formPanel);
    fromTimeEdit->setDisplayFormat("HH:mm");
    fromTimeEdit->setTime(QTime(7, 0));
    fromTimeEdit->setReadOnly(true);
    fromTimeEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    fromTimeEdit->setMaximumWidth(70);
    fromTimeEdit->setStyleSheet(
        "QTimeEdit { "
        "padding: 4px; "
        "border: 1px solid #e5e7eb; "
        "border-radius: 4px; "
        "background-color: #f9fafb; "
        "color: #6b7280; "
        "font-size: 11px; "
        "}");

    QLabel *toLabel = new QLabel("→", formPanel);
    toLabel->setStyleSheet("color: #6b7280; font-size: 12px;");

    toTimeEdit = new QTimeEdit(formPanel);
    toTimeEdit->setDisplayFormat("HH:mm");
    toTimeEdit->setTime(QTime(7, 30));
    toTimeEdit->setReadOnly(true);
    toTimeEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    toTimeEdit->setMaximumWidth(70);
    toTimeEdit->setStyleSheet(fromTimeEdit->styleSheet());

    durationLabel = new QLabel("(0h30m)", formPanel);
    durationLabel->setStyleSheet("color: #f59e0b; font-weight: bold; font-size: 10px;");

    timeDisplayLayout->addWidget(fromTimeEdit);
    timeDisplayLayout->addWidget(toLabel);
    timeDisplayLayout->addWidget(toTimeEdit);
    timeDisplayLayout->addWidget(durationLabel);
    timeDisplayLayout->addStretch();

    rightColumnLayout->addLayout(timeDisplayLayout);
    
    // Duration quick select buttons
    QLabel *quickDurationLabel = new QLabel("Chọn nhanh:", formPanel);
    quickDurationLabel->setStyleSheet("font-size: 10px; color: #6b7280; margin-top: 2px;");
    rightColumnLayout->addWidget(quickDurationLabel);
    
    QHBoxLayout *durationBtnsLayout = new QHBoxLayout();
    durationBtnsLayout->setSpacing(4);
    
    QString durationBtnStyle = 
        "QPushButton { "
        "background-color: #f3f4f6; "
        "color: #1f2937; "
        "border: 1px solid #d1d5db; "
        "border-radius: 3px; "
        "padding: 4px 8px; "
        "font-size: 10px; "
        "} "
        "QPushButton:hover { "
        "background-color: #e5e7eb; "
        "border: 1px solid #16a34a; "
        "}";
    
    duration30Btn = new QPushButton("30m", formPanel);
    duration30Btn->setStyleSheet(durationBtnStyle);
    duration30Btn->setMaximumWidth(45);
    
    duration60Btn = new QPushButton("1h", formPanel);
    duration60Btn->setStyleSheet(durationBtnStyle);
    duration60Btn->setMaximumWidth(45);
    
    duration90Btn = new QPushButton("1.5h", formPanel);
    duration90Btn->setStyleSheet(durationBtnStyle);
    duration90Btn->setMaximumWidth(45);
    
    duration120Btn = new QPushButton("2h", formPanel);
    duration120Btn->setStyleSheet(durationBtnStyle);
    duration120Btn->setMaximumWidth(45);
    
    durationBtnsLayout->addWidget(duration30Btn);
    durationBtnsLayout->addWidget(duration60Btn);
    durationBtnsLayout->addWidget(duration90Btn);
    durationBtnsLayout->addWidget(duration120Btn);
    durationBtnsLayout->addStretch();
    
    rightColumnLayout->addLayout(durationBtnsLayout);

    // Note (editable)
    QLabel *noteLabel = new QLabel("Ghi chú:", formPanel);
    noteLabel->setStyleSheet("font-size: 11px; font-weight: bold; color: #1f2937; margin-top: 4px;");
    rightColumnLayout->addWidget(noteLabel);

    noteEdit = new QTextEdit(formPanel);
    noteEdit->setPlaceholderText("Ghi chú...");
    noteEdit->setMaximumHeight(50);
    noteEdit->setStyleSheet(
        "QTextEdit { "
        "padding: 6px; "
        "border: 1px solid #d1d5db; "
        "border-radius: 4px; "
        "background-color: white; "
        "font-size: 11px; "
        "} "
        "QTextEdit:focus { "
        "border: 1px solid #16a34a; "
        "}");
    rightColumnLayout->addWidget(noteEdit);

    rightColumnLayout->addStretch();

    // Add columns to form
    QHBoxLayout *formColumnsLayout = new QHBoxLayout();
    formColumnsLayout->addLayout(leftColumnLayout, 1);
    formColumnsLayout->addLayout(rightColumnLayout, 1);
    formLayout->addLayout(formColumnsLayout);

    // Action buttons (centered)
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(4);
    buttonLayout->addStretch();

    QString btnStyle = "QPushButton { background-color: %1; color: white; border: none; border-radius: 4px; padding: 6px 12px; font-weight: bold; font-size: 11px; min-width: 70px; max-height: 32px; } QPushButton:hover { background-color: %2; } QPushButton:disabled { background-color: #9ca3af; }";
    
    saveBtn = new QPushButton("💾 Lưu", formPanel);
    saveBtn->setFixedHeight(32);
    saveBtn->setStyleSheet(btnStyle.arg("#16a34a").arg("#15803d"));

    QPushButton *cancelSelectionBtn = new QPushButton("Hủy chọn", formPanel);
    cancelSelectionBtn->setFixedHeight(32);
    cancelSelectionBtn->setStyleSheet(btnStyle.arg("#6b7280").arg("#4b5563"));
    connect(cancelSelectionBtn, &QPushButton::clicked, this, &TimelineTab::onCancelSelectionClicked);

    deleteBtn = new QPushButton("🗑️ Hủy", formPanel);
    deleteBtn->setFixedHeight(32);
    deleteBtn->setStyleSheet(btnStyle.arg("#ef4444").arg("#dc2626"));

    checkinBtn = new QPushButton("✓ Check-in", formPanel);
    checkinBtn->setFixedHeight(32);
    checkinBtn->setStyleSheet(btnStyle.arg("#3b82f6").arg("#2563eb"));

    buttonLayout->addWidget(saveBtn);
    buttonLayout->addWidget(cancelSelectionBtn);
    buttonLayout->addWidget(deleteBtn);
    buttonLayout->addWidget(checkinBtn);
    buttonLayout->addStretch();

    formLayout->addLayout(buttonLayout);
}

void TimelineTab::setupTimelinePanel()
{
    timelinePanel = new QFrame(this);
    timelinePanel->setObjectName("timelinePanel");
    timelinePanel->setStyleSheet(
        "#timelinePanel { "
        "background-color: white; "
        "border: 1px solid #e5e7eb; "
        "border-radius: 8px; "
        "}");

    timelineLayout = new QVBoxLayout(timelinePanel);
    timelineLayout->setContentsMargins(10, 10, 10, 10);
    
    // Advanced filters panel
    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->setSpacing(8);
    
    QLabel *filterLabel = new QLabel("Bộ lọc:", timelinePanel);
    filterLabel->setStyleSheet("font-size: 12px; font-weight: bold; color: #1f2937;");
    filterLayout->addWidget(filterLabel);
    
    // Field filter
    fieldFilterCombo = new QComboBox(timelinePanel);
    fieldFilterCombo->addItem("Tất cả sân");
    fieldFilterCombo->setStyleSheet(
        "QComboBox { "
        "padding: 4px 8px; "
        "border: 1px solid #d1d5db; "
        "border-radius: 4px; "
        "background-color: white; "
        "font-size: 11px; "
        "}");
    filterLayout->addWidget(fieldFilterCombo);
    
    // Status filter
    statusFilterCombo = new QComboBox(timelinePanel);
    statusFilterCombo->addItem("Tất cả trạng thái");
    statusFilterCombo->addItem("Trống");
    statusFilterCombo->addItem("Đã đặt");
    statusFilterCombo->setStyleSheet(fieldFilterCombo->styleSheet());
    filterLayout->addWidget(statusFilterCombo);
    
    // Time filter
    timeFilterCombo = new QComboBox(timelinePanel);
    timeFilterCombo->addItem("Cả ngày");
    timeFilterCombo->addItem("Sáng (6h-12h)");
    timeFilterCombo->addItem("Chiều (12h-18h)");
    timeFilterCombo->addItem("Tối (18h-23h)");
    timeFilterCombo->setStyleSheet(fieldFilterCombo->styleSheet());
    filterLayout->addWidget(timeFilterCombo);
    
    filterLayout->addStretch();
    timelineLayout->addLayout(filterLayout);

    // Scroll area for timeline grid
    QScrollArea *scrollArea = new QScrollArea(timelinePanel);
    scrollArea->setWidgetResizable(false);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setStyleSheet("QScrollArea { border: none; background-color: white; }");

    // Timeline grid widget
    timelineGrid = new TimelineGridWidget();
    scrollArea->setWidget(timelineGrid);

    timelineLayout->addWidget(scrollArea);
    
    // Load fields into filter
    setupFilters();
}

void TimelineTab::setupConnections()
{
    // Calendar
    connect(calendar, &QCalendarWidget::clicked, this, &TimelineTab::onDateSelected);

    // Form - auto-search on phone text changed
    connect(phoneEdit, &QLineEdit::textChanged, this, &TimelineTab::onPhoneSearchClicked);
    connect(addCustomerBtn, &QPushButton::clicked, this, &TimelineTab::onAddCustomerClicked);
    connect(fieldCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TimelineTab::onFieldChanged);

    // Buttons
    connect(saveBtn, &QPushButton::clicked, this, &TimelineTab::onSaveClicked);
    connect(deleteBtn, &QPushButton::clicked, this, &TimelineTab::onDeleteClicked);
    connect(checkinBtn, &QPushButton::clicked, this, &TimelineTab::onCheckinClicked);
    
    // Duration quick select
    connect(duration30Btn, &QPushButton::clicked, this, &TimelineTab::onDuration30Clicked);
    connect(duration60Btn, &QPushButton::clicked, this, &TimelineTab::onDuration60Clicked);
    connect(duration90Btn, &QPushButton::clicked, this, &TimelineTab::onDuration90Clicked);
    connect(duration120Btn, &QPushButton::clicked, this, &TimelineTab::onDuration120Clicked);
    
    // Filters
    connect(fieldFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TimelineTab::onFieldFilterChanged);
    connect(statusFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TimelineTab::onStatusFilterChanged);
    connect(timeFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TimelineTab::onTimeFilterChanged);

    // Timeline grid
    connect(timelineGrid, &TimelineGridWidget::slotSelected, this, &TimelineTab::onTimelineSlotSelected);
    connect(timelineGrid, &TimelineGridWidget::bookingClicked, this, &TimelineTab::onBookingBlockClicked);
}

void TimelineTab::loadFields()
{
    fieldCombo->clear();

    QuanLySan *quanLySan = system->layQuanLySan();
    if (quanLySan)
    {
        fields = quanLySan->layDanhSachSan();

        for (int i = 0; i < fields.size(); i++)
        {
            San *san = fields[i];
            QString fieldName = QString::fromStdString(san->layTenSan());
            QString fieldType = (san->layLoaiSan() == LoaiSan::SAN_5) ? "Sân 5" : "Sân 7";
            fieldCombo->addItem(fieldName + " (" + fieldType + ")", i);
        }
    }

    if (fieldCombo->count() > 0)
    {
        fieldCombo->setCurrentIndex(0);
        onFieldChanged(0);
    }
    
    // Load customers into combo
    QComboBox *customerCombo = formPanel->findChild<QComboBox*>("customerCombo");
    if (customerCombo) {
        customerCombo->clear();
        customerCombo->addItem("-- Chọn khách hàng --", "");
        
        QuanLyKhachHang *qlkh = system->layQuanLyKhachHang();
        if (qlkh) {
            const MangDong<KhachHang*>& customers = qlkh->layDanhSachKhachHang();
            for (int i = 0; i < customers.size(); i++) {
                KhachHang *kh = customers[i];
                if (kh) {
                    QString display = QString::fromStdString(kh->layHoTen()) + 
                                     " - " + QString::fromStdString(kh->laySoDienThoai());
                    customerCombo->addItem(display, QString::fromStdString(kh->laySoDienThoai()));
                }
            }
        }
    }
}

void TimelineTab::refreshData()
{
    // Reload bookings for selected date
    timelineGrid->setDate(selectedDate);
    timelineGrid->loadBookings();
    
    // Update calendar to highlight dates with bookings
    updateCalendarDates();
}

void TimelineTab::clearForm()
{
    phoneEdit->clear();
    phoneEdit->setProperty("hasCustomer", false);
    nameLabel->setText("-");
    
    // Reset customer combo to "-- Chọn khách hàng --"
    QComboBox *customerCombo = formPanel->findChild<QComboBox*>("customerCombo");
    if (customerCombo) {
        customerCombo->setCurrentIndex(0);
    }
    
    if (fieldCombo->count() > 0)
    {
        fieldCombo->setCurrentIndex(0);
    }
    statusCombo->setCurrentIndex(0);
    typeCombo->setCurrentIndex(0);
    dateEdit->setDate(selectedDate);
    fromTimeEdit->setTime(QTime(7, 0));
    toTimeEdit->setTime(QTime(7, 30));
    noteEdit->clear();

    currentBooking = nullptr;
    isEditMode = false;

    deleteBtn->setEnabled(false);
    checkinBtn->setEnabled(false);

    updateDuration();
}

void TimelineTab::populateForm(DatSan *booking)
{
    if (!booking)
    {
        clearForm();
        return;
    }

    currentBooking = booking;
    isEditMode = true;

    // Customer info
    KhachHang *customer = booking->getKhachHang();
    if (customer)
    {
        QString phone = QString::fromStdString(customer->laySoDienThoai());
        phoneEdit->setText(phone);
        phoneEdit->setProperty("hasCustomer", true);
        nameLabel->setText(QString::fromStdString(customer->layHoTen()));
        
        // Sync customer combo
        QComboBox *customerCombo = formPanel->findChild<QComboBox*>("customerCombo");
        if (customerCombo) {
            for (int i = 0; i < customerCombo->count(); i++) {
                if (customerCombo->itemData(i).toString() == phone) {
                    customerCombo->setCurrentIndex(i);
                    break;
                }
            }
        }
    }

    // Field
    San *san = booking->getSan();
    if (san)
    {
        for (int i = 0; i < fieldCombo->count(); i++)
        {
            int fieldIndex = fieldCombo->itemData(i).toInt();
            if (fieldIndex >= 0 && fieldIndex < fields.size() && fields[fieldIndex] == san)
            {
                fieldCombo->setCurrentIndex(i);
                break;
            }
        }
    }

    // Date & Time
    NgayGio ngayGio = booking->getThoiGianDat();
    QDate date(ngayGio.getNam(), ngayGio.getThang(), ngayGio.getNgay());
    QTime time(ngayGio.getGio(), ngayGio.getPhut());
    dateEdit->setDate(date);
    fromTimeEdit->setTime(time);

    // Calculate end time from KhungGio
    KhungGio khungGio = booking->getKhungGio();
    ThoiGian gioKT = khungGio.getGioKetThuc();
    QTime endTime(gioKT.getGio(), gioKT.getPhut());
    toTimeEdit->setTime(endTime);

    // Note (if available)
    // noteEdit->setText(QString::fromStdString(booking->layGhiChu()));

    deleteBtn->setEnabled(true);
    checkinBtn->setEnabled(true);

    updateDuration();
}

void TimelineTab::updateDuration()
{
    QTime from = fromTimeEdit->time();
    QTime to = toTimeEdit->time();

    int minutes = from.secsTo(to) / 60;
    if (minutes < 0)
    {
        minutes = 0;
    }

    int hours = minutes / 60;
    int mins = minutes % 60;

    durationLabel->setText(QString("%1h%2m").arg(hours).arg(mins, 2, 10, QChar('0')));
}

QString TimelineTab::formatCurrency(double amount)
{
    return QString("%L1 VND").arg(amount, 0, 'f', 0);
}

void TimelineTab::onDateSelected(const QDate &date)
{
    selectedDate = date;
    dateEdit->setDate(date);
    refreshData();
}

void TimelineTab::onPhoneSearchClicked()
{
    QString phone = phoneEdit->text().trimmed();
    if (phone.isEmpty())
    {
        nameLabel->setText("-");
        return;
    }

    QuanLyKhachHang *quanLyKH = system->layQuanLyKhachHang();
    if (!quanLyKH)
    {
        return;
    }

    KhachHang *customer = quanLyKH->timKhachHangTheoSDT(phone.toStdString());
    if (customer)
    {
        nameLabel->setText(QString::fromStdString(customer->layHoTen()));
    }
    else
    {
        nameLabel->setText("-");
    }
}

void TimelineTab::onFieldChanged(int index)
{
    if (index < 0 || index >= fieldCombo->count())
    {
        return;
    }

    int fieldIndex = fieldCombo->itemData(index).toInt();
    if (fieldIndex >= 0 && fieldIndex < fields.size())
    {
        San *san = fields[fieldIndex];
        double price = san->layGiaThueGio();
        priceLabel->setText(formatCurrency(price));

        // Update type combo
        if (san->layLoaiSan() == LoaiSan::SAN_5)
        {
            typeCombo->setCurrentIndex(0); // 5 a side
        }
        else
        {
            typeCombo->setCurrentIndex(1); // 7 a side
        }
    }
}

void TimelineTab::onSaveClicked()
{
    qDebug() << "=== SAVE BOOKING DEBUG ===";
    
    // Validate customer selection first
    QString phone = phoneEdit->text().trimmed();
    qDebug() << "Phone from phoneEdit:" << phone;
    qDebug() << "Has customer property:" << phoneEdit->property("hasCustomer").toBool();
    
    if (phone.isEmpty() || !phoneEdit->property("hasCustomer").toBool())
    {
        QMessageBox::warning(this, "❌ Lỗi", "Vui lòng chọn khách hàng từ danh sách!");
        return;
    }
    
    // Clear pending selection after customer validation passes
    if (timelineGrid)
    {
        timelineGrid->clearPendingSelection();
    }

    if (fieldCombo->currentIndex() < 0)
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn sân!");
        return;
    }

    QTime from = fromTimeEdit->time();
    QTime to = toTimeEdit->time();
    if (from >= to)
    {
        QMessageBox::warning(this, "Lỗi", "Giờ kết thúc phải sau giờ bắt đầu!");
        return;
    }

    // Find customer
    QuanLyKhachHang *quanLyKH = system->layQuanLyKhachHang();
    
    // Debug: Check phone format
    qDebug() << "Phone length:" << phone.length();
    qDebug() << "Phone bytes:" << phone.toUtf8().toHex();
    std::string phoneStd = phone.toStdString();
    qDebug() << "Phone std::string length:" << phoneStd.length();
    
    KhachHang *customer = quanLyKH ? quanLyKH->timKhachHangTheoSDT(phoneStd) : nullptr;

    if (!customer)
    {
        // Debug: List all customers
        if (quanLyKH) {
            const MangDong<KhachHang*>& customers = quanLyKH->layDanhSachKhachHang();
            qDebug() << "Total customers:" << customers.size();
            for (int i = 0; i < qMin(5, customers.size()); i++) {
                if (customers[i]) {
                    qDebug() << "Customer" << i << "phone:" << QString::fromStdString(customers[i]->laySoDienThoai());
                }
            }
        }
        QMessageBox::warning(this, "Lỗi", QString("Không tìm thấy khách hàng với SĐT: %1\n\nVui lòng kiểm tra lại!").arg(phone));
        return;
    }

    // Get field
    int fieldIndex = fieldCombo->currentData().toInt();
    if (fieldIndex < 0 || fieldIndex >= fields.size())
    {
        QMessageBox::warning(this, "Lỗi", "Sân không hợp lệ!");
        return;
    }
    San *san = fields[fieldIndex];

    // Create NgayGio
    QDate date = dateEdit->date();
    NgayGio ngayGio(from.hour(), from.minute(), 0, date.day(), date.month(), date.year());

    // Create KhungGio with start and end times
    KhungGio khungGio(from.hour(), from.minute(), to.hour(), to.minute());
    
    // ===== CONFLICT DETECTION =====
    if (checkBookingConflict(san, ngayGio, khungGio))
    {
        QMessageBox::StandardButton reply = QMessageBox::warning(
            this,
            "⚠️ Cảnh báo xung đột",
            "Khung giờ này đã có người đặt!\n\nBạn có muốn tiếp tục đặt (sẽ ghi đè)?",
            QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::No) {
            return;
        }
    }

    try
    {
        QuanLyDatSan *quanLyDS = system->layQuanLyDatSan();
        if (!quanLyDS)
        {
            throw std::runtime_error("QuanLyDatSan not initialized");
        }

        if (isEditMode && currentBooking)
        {
            // Update existing booking
            // Note: Core API may need update method
            QMessageBox::information(this, "Thành công", "Cập nhật đặt sân thành công!");
        }
        else
        {
            // Create new booking
            DatSan *newBooking = quanLyDS->taoDatSan(customer, san, ngayGio, khungGio);
            if (newBooking)
            {
                // ===== SAVE DATA TO FILE =====
                try {
                    system->luuHeThong("D:/QT_PBL2/Data/data.bin");
                } catch (...) {
                    qDebug() << "Warning: Could not save to data.bin";
                }
                
                QMessageBox::information(this, "✅ Thành công", "Đặt sân thành công!");
                
                // Emit signal to refresh table view
                emit bookingDataChanged();
                
                clearForm();
                refreshData();
            }
            else
            {
                throw std::runtime_error("Failed to create booking");
            }
        }
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Lỗi", QString("Không thể lưu đặt sân: %1").arg(e.what()));
    }
}

void TimelineTab::onCancelSelectionClicked()
{
    // Clear pending selection in timeline grid
    if (timelineGrid)
    {
        timelineGrid->clearPendingSelection();
    }
    
    // Clear form
    clearForm();
    
    QMessageBox::information(this, "Đã hủy", "Đã xóa vùng chọn");
}

void TimelineTab::onDeleteClicked()
{
    if (!currentBooking)
    {
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Xác nhận",
        "Bạn có chắc chắn muốn hủy đặt sân này?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        try
        {
            QuanLyDatSan *quanLyDS = system->layQuanLyDatSan();
            if (quanLyDS)
            {
                bool success = quanLyDS->huyDatSan(currentBooking->getMaDatSan());
                if (success)
                {
                    // ===== SAVE DATA TO FILE =====
                    try {
                        system->luuHeThong("D:/QT_PBL2/Data/data.bin");
                    } catch (...) {
                        qDebug() << "Warning: Could not save to data.bin";
                    }
                    
                    QMessageBox::information(this, "✅ Thành công", "Đã hủy đặt sân!");
                    
                    // Emit signal to refresh table view
                    emit bookingDataChanged();
                    
                    clearForm();
                    refreshData();
                }
            }
        }
        catch (const std::exception &e)
        {
            QMessageBox::critical(this, "Lỗi", QString("Không thể hủy đặt sân: %1").arg(e.what()));
        }
    }
}

void TimelineTab::onCheckinClicked()
{
    if (!currentBooking)
    {
        return;
    }

    QMessageBox::information(this, "Checkin", "Chức năng checkin/match sẽ được triển khai sau!");
}

void TimelineTab::onTimelineSlotSelected(int fieldIndex, int startHour, int startMinute, int durationMinutes)
{
    // IMPORTANT: Don't call clearForm() - keep customer selection!
    // Just reset time and field, keep customer info
    
    // Reset booking mode
    currentBooking = nullptr;
    isEditMode = false;
    deleteBtn->setEnabled(false);
    checkinBtn->setEnabled(false);

    // Set field (enable temporarily to set value, then disable)
    fieldCombo->setEnabled(true);
    if (fieldIndex >= 0 && fieldIndex < fieldCombo->count())
    {
        fieldCombo->setCurrentIndex(fieldIndex);
    }
    fieldCombo->setEnabled(false);

    // Set time
    QTime startTime(startHour, startMinute);
    QTime endTime = startTime.addSecs(durationMinutes * 60);
    fromTimeEdit->setTime(startTime);
    toTimeEdit->setTime(endTime);
    updateDuration();
    
    // Clear note
    noteEdit->clear();

    // If no customer selected, focus phone field
    if (phoneEdit->text().trimmed().isEmpty()) {
        phoneEdit->setFocus();
    }
}

void TimelineTab::onBookingBlockClicked(DatSan *booking)
{
    populateForm(booking);
}

// ===== NEW FEATURE IMPLEMENTATIONS =====

void TimelineTab::onAddCustomerClicked()
{
    // Quick add customer dialog
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Thêm khách hàng nhanh");
    dialog->setMinimumWidth(350);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    QLabel *nameLabel = new QLabel("Họ tên:", dialog);
    QLineEdit *nameEdit = new QLineEdit(dialog);
    nameEdit->setPlaceholderText("Nhập họ tên");
    
    QLabel *phoneLabel = new QLabel("Số điện thoại:", dialog);
    QLineEdit *phoneNewEdit = new QLineEdit(dialog);
    phoneNewEdit->setPlaceholderText("Nhập số điện thoại");
    phoneNewEdit->setText(phoneEdit->text()); // Pre-fill from search box
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *saveBtn = new QPushButton("Lưu", dialog);
    QPushButton *cancelBtn = new QPushButton("Hủy", dialog);
    
    saveBtn->setStyleSheet("background-color: #16a34a; color: white; padding: 8px 16px; border-radius: 4px; font-weight: bold;");
    cancelBtn->setStyleSheet("background-color: #6b7280; color: white; padding: 8px 16px; border-radius: 4px;");
    
    btnLayout->addStretch();
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(cancelBtn);
    
    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(phoneLabel);
    layout->addWidget(phoneNewEdit);
    layout->addSpacing(10);
    layout->addLayout(btnLayout);
    
    connect(saveBtn, &QPushButton::clicked, [=]() {
        QString name = nameEdit->text().trimmed();
        QString phone = phoneNewEdit->text().trimmed();
        
        if (name.isEmpty() || phone.isEmpty()) {
            QMessageBox::warning(dialog, "Lỗi", "Vui lòng nhập đầy đủ thông tin!");
            return;
        }
        
        if (phone.length() < 10 || phone.length() > 11) {
            QMessageBox::warning(dialog, "Lỗi", "Số điện thoại phải từ 10-11 số!");
            return;
        }
        
        QuanLyKhachHang *qlkh = system->layQuanLyKhachHang();
        if (qlkh) {
            try {
                // Create new customer
                KhachHang *newCustomer = new KhachHang(
                    name.toStdString(),
                    phone.toStdString(),
                    "", // empty address
                    qlkh->taoMaKhachHangMoi()
                );
                
                qlkh->themKhachHang(newCustomer);
                
                // Save to CSV
                qlkh->luuCSV("D:/QT_PBL2/Data/khachhang.csv");
                
                QMessageBox::information(dialog, "Thành công", "Đã thêm khách hàng!");
                
                // Update form
                phoneEdit->setText(phone);
                nameLabel->setText(name);
                
                dialog->accept();
            } catch (const std::exception &e) {
                QMessageBox::critical(dialog, "Lỗi", QString("Không thể thêm khách hàng: %1").arg(e.what()));
            }
        }
    });
    
    connect(cancelBtn, &QPushButton::clicked, dialog, &QDialog::reject);
    
    dialog->exec();
    delete dialog;
}

void TimelineTab::onDuration30Clicked() { setDuration(30); }
void TimelineTab::onDuration60Clicked() { setDuration(60); }
void TimelineTab::onDuration90Clicked() { setDuration(90); }
void TimelineTab::onDuration120Clicked() { setDuration(120); }

void TimelineTab::setDuration(int minutes)
{
    QTime start = fromTimeEdit->time();
    QTime end = start.addSecs(minutes * 60);
    toTimeEdit->setTime(end);
    updateDuration();
}

void TimelineTab::onFieldFilterChanged(int index)
{
    applyTimelineFilters();
}

void TimelineTab::onStatusFilterChanged(int index)
{
    applyTimelineFilters();
}

void TimelineTab::onTimeFilterChanged(int index)
{
    applyTimelineFilters();
}

void TimelineTab::applyTimelineFilters()
{
    int fieldIndex = fieldFilterCombo->currentIndex() - 1; // -1 = All
    int statusFilter = statusFilterCombo->currentIndex();
    int timeFilter = timeFilterCombo->currentIndex();
    
    timelineGrid->applyFilters(fieldIndex, statusFilter, timeFilter);
}

void TimelineTab::setupFilters()
{
    // Load fields into filter combo
    fieldFilterCombo->clear();
    fieldFilterCombo->addItem("Tất cả sân");
    
    for (int i = 0; i < fields.size(); i++) {
        San *san = fields[i];
        QString fieldName = QString::fromStdString(san->layTenSan());
        fieldFilterCombo->addItem(fieldName);
    }
}

bool TimelineTab::checkBookingConflict(San *san, const NgayGio &ngayGio, const KhungGio &khungGio)
{
    QuanLyDatSan *quanLyDS = system->layQuanLyDatSan();
    if (!quanLyDS) return false;
    
    // Check if time slot is already booked
    const MangDong<DatSan*>& allBookings = quanLyDS->layDanhSachDatSan();
    
    for (int i = 0; i < allBookings.size(); i++) {
        DatSan *existingBooking = allBookings[i];
        
        // Skip if different field
        if (existingBooking->getSan() != san) continue;
        
        // Skip if different date
        NgayGio existingNgayGio = existingBooking->getThoiGianDat();
        if (existingNgayGio.getNgay() != ngayGio.getNgay() ||
            existingNgayGio.getThang() != ngayGio.getThang() ||
            existingNgayGio.getNam() != ngayGio.getNam()) {
            continue;
        }
        
        // Skip current booking in edit mode
        if (isEditMode && existingBooking == currentBooking) continue;
        
        // Check time overlap
        KhungGio existingKhungGio = existingBooking->getKhungGio();
        
        int newStart = khungGio.getGioBatDau().getGio() * 60 + khungGio.getGioBatDau().getPhut();
        int newEnd = khungGio.getGioKetThuc().getGio() * 60 + khungGio.getGioKetThuc().getPhut();
        int existingStart = existingKhungGio.getGioBatDau().getGio() * 60 + existingKhungGio.getGioBatDau().getPhut();
        int existingEnd = existingKhungGio.getGioKetThuc().getGio() * 60 + existingKhungGio.getGioKetThuc().getPhut();
        
        // Check overlap
        if (!(newEnd <= existingStart || newStart >= existingEnd)) {
            return true; // Conflict found
        }
    }
    
    return false; // No conflict
}

void TimelineTab::updateCalendarDates()
{
    if (!calendar) return;
    
    // Clear all date formats first
    QTextCharFormat defaultFormat;
    defaultFormat.setForeground(Qt::black);
    calendar->setDateTextFormat(QDate(), defaultFormat);
    
    // Get all bookings
    QuanLyDatSan *quanLyDS = system->layQuanLyDatSan();
    if (!quanLyDS) return;
    
    const MangDong<DatSan*>& allBookings = quanLyDS->layDanhSachDatSan();
    
    // Create format for dates with bookings
    QTextCharFormat bookedFormat;
    bookedFormat.setBackground(QColor("#dcfce7")); // Light green
    bookedFormat.setForeground(QColor("#16a34a")); // Dark green
    bookedFormat.setFontWeight(QFont::Bold);
    
    // Highlight dates with bookings
    QSet<QDate> bookedDates;
    for (int i = 0; i < allBookings.size(); i++) {
        DatSan *booking = allBookings[i];
        if (booking) {
            NgayGio ngayGio = booking->getThoiGianDat();
            QDate bookingDate(ngayGio.getNam(), ngayGio.getThang(), ngayGio.getNgay());
            bookedDates.insert(bookingDate);
        }
    }
    
    // Apply format to all booked dates
    for (const QDate &date : bookedDates) {
        calendar->setDateTextFormat(date, bookedFormat);
    }
    
    // Highlight today with different color
    QTextCharFormat todayFormat;
    todayFormat.setBackground(QColor("#dbeafe")); // Light blue
    todayFormat.setForeground(QColor("#1e40af")); // Dark blue
    todayFormat.setFontWeight(QFont::Bold);
    calendar->setDateTextFormat(QDate::currentDate(), todayFormat);
}
