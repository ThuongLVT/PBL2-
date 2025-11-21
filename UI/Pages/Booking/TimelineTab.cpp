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

#include <QRegularExpressionValidator>

TimelineTab::TimelineTab(QWidget *parent)
    : QWidget(parent), mainLayout(nullptr), topPanel(nullptr), topPanelLayout(nullptr), calendarPanel(nullptr), calendarLayout(nullptr), calendar(nullptr), formPanel(nullptr), formLayout(nullptr), phoneEdit(nullptr), nameEdit(nullptr), fieldCombo(nullptr), priceLabel(nullptr), statusCombo(nullptr), typeCombo(nullptr), dateEdit(nullptr), fromTimeEdit(nullptr), toTimeEdit(nullptr), durationLabel(nullptr), duration30Btn(nullptr), duration60Btn(nullptr), duration90Btn(nullptr), duration120Btn(nullptr), noteEdit(nullptr), saveBtn(nullptr), deleteBtn(nullptr), timelinePanel(nullptr), timelineLayout(nullptr), timelineGrid(nullptr), system(nullptr), currentBooking(nullptr), isEditMode(false)
{
    system = HeThongQuanLy::getInstance();
    selectedDate = QDate::currentDate();

    setupUI();
    setupConnections();
    loadFields();
    clearForm();
    refreshData();
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

    setupCalendarPanel();
    setupFormPanel();

    // Top panel (calendar + form) with 20/80 ratio - FIXED 40% of total height
    topPanel = new QWidget(this);
    topPanelLayout = new QHBoxLayout(topPanel);
    topPanelLayout->setContentsMargins(0, 0, 0, 0);
    topPanelLayout->setSpacing(10);

    // Set sizes for 20:80 ratio
    topPanelLayout->addWidget(calendarPanel, 20); // 20%
    topPanelLayout->addWidget(formPanel, 80);     // 80%

    setupTimelinePanel();

    // Set size policies to ensure proper ratio
    topPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    topPanel->setMinimumHeight(200);
    topPanel->setMaximumHeight(400);

    timelinePanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    timelinePanel->setMinimumHeight(300);

    // Add panels with stretch factors to maintain 40/60 ratio
    mainLayout->addWidget(topPanel, 2);      // Top: 2 parts (40%)
    mainLayout->addWidget(timelinePanel, 3); // Bottom: 3 parts (60%)
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
    // calendar->setMinimumDate(QDate::currentDate()); // Allow past dates
    calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    // Remove fixed height to allow calendar to expand
    calendar->setMinimumHeight(200);

    // Highlight current date with custom format
    // Removed green highlight for current date as requested
    // QTextCharFormat todayFormat;
    // todayFormat.setBackground(QBrush(QColor(220, 252, 231))); // Light green background (#dcfce7)
    // todayFormat.setForeground(QBrush(QColor(22,163,74)));   // Green text (#16a34a)
    // todayFormat.setFontWeight(QFont::Bold);
    // calendar->setDateTextFormat(QDate::currentDate(), todayFormat);

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
    formLayout->setContentsMargins(12, 10, 12, 10);
    formLayout->setSpacing(8);

    // Main layout: Form + Right Column (Note + Buttons)
    QHBoxLayout *mainContentLayout = new QHBoxLayout();
    mainContentLayout->setSpacing(130);                // Increased spacing to 130px as requested
    mainContentLayout->setContentsMargins(0, 0, 0, 0); // Remove margins to align top
    mainContentLayout->setAlignment(Qt::AlignTop);     // Align everything to top

    // Grid layout for form fields
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setVerticalSpacing(2); // Changed to 1px as requested
    gridLayout->setHorizontalSpacing(10);
    gridLayout->setContentsMargins(0, 0, 0, 0); // Remove margins
    gridLayout->setAlignment(Qt::AlignTop);     // Align grid to top

    // Cấu trúc 5 cột:
    // Col 0: Label cột 1
    // Col 1: Box cột 1 (20px spacing auto)
    // Col 2: Spacer 10px (giữa 2 nhóm)
    // Col 3: Label cột 2
    // Col 4: Box cột 2 (20px spacing auto)

    gridLayout->setColumnMinimumWidth(1, 200); // Box cột 1: 200px
    gridLayout->setColumnMinimumWidth(2, 10);  // Spacer giữa 2 cột: 10px
    gridLayout->setColumnMinimumWidth(4, 200); // Box cột 2: 200px (bằng cột 1)

    int row = 0;

    QString labelStyle = "font-size: 13px; font-weight: bold; color: #1f2937;";
    QString inputStyle =
        "padding: 6px 10px; "
        "border: 1px solid #d1d5db; "
        "border-radius: 4px; "
        "background-color: white; " // Nền trắng cho input
        "font-size: 13px; "
        "min-height: 36px;";
    QString readOnlyStyle =
        "padding: 6px 10px; "
        "border: 1px solid #e5e7eb; "
        "border-radius: 4px; "
        "background-color: #f9fafb; " // Nền xám cho readonly
        "color: #6b7280; "
        "font-size: 13px; "
        "min-height: 36px;";

    // Style for readonly combobox - completely hide drop-down arrow
    QString readOnlyComboStyle =
        "QComboBox { "
        "   padding: 6px 10px; "
        "   border: 1px solid #e5e7eb; "
        "   border-radius: 4px; "
        "   background-color: #f9fafb; "
        "   color: #6b7280; "
        "   font-size: 13px; "
        "   min-height: 36px; "
        "} "
        "QComboBox::drop-down { "
        "   border: none; "
        "   width: 0px; "
        "} "
        "QComboBox::down-arrow { "
        "   image: none; "
        "   border: none; "
        "}";

    // Style for readonly date/time edit - no arrows, NỀN TRẮNG
    QString readOnlyDateTimeStyle = "padding: 6px 10px; border: 1px solid #e5e7eb; border-radius: 4px; background-color: white; color: #6b7280; font-size: 13px; min-height: 36px; QDateEdit::drop-down { border: none; width: 0px; } QDateEdit::down-arrow { image: none; } QTimeEdit::up-button, QTimeEdit::down-button { width: 0px; border: none; } QTimeEdit::up-arrow, QTimeEdit::down-arrow { image: none; }";

    // === ROW 0: Số điện thoại | Sân ===
    QLabel *phoneLabel = new QLabel("Số điện thoại:", formPanel);
    phoneLabel->setStyleSheet(labelStyle);
    phoneLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    gridLayout->addWidget(phoneLabel, row, 0);

    phoneEdit = new QLineEdit(formPanel);
    phoneEdit->setPlaceholderText("Nhập số điện thoại...");
    phoneEdit->setMinimumWidth(200);
    phoneEdit->setMaximumWidth(200);
    // Validator: Only 10 digits allowed, must start with 0
    QRegularExpression rx("^0[0-9]{9}$");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    phoneEdit->setValidator(validator);
    phoneEdit->setStyleSheet(
        "QLineEdit { " + inputStyle + " } "
                                      "QLineEdit:focus { "
                                      "border: 1px solid #16a34a; "
                                      "}");
    gridLayout->addWidget(phoneEdit, row, 1);

    QLabel *fieldLabel = new QLabel("Sân:", formPanel);
    fieldLabel->setStyleSheet(labelStyle);
    fieldLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    gridLayout->addWidget(fieldLabel, row, 3); // Col 3

    fieldCombo = new QComboBox(formPanel);
    fieldCombo->setEnabled(false);
    fieldCombo->setMinimumWidth(200);
    fieldCombo->setMaximumWidth(200);
    fieldCombo->setStyleSheet(readOnlyComboStyle);
    gridLayout->addWidget(fieldCombo, row, 4); // Col 4
    row++;

    // === ROW 1: Họ tên | Loại sân ===
    QLabel *nameFieldLabel = new QLabel("Họ tên:", formPanel);
    nameFieldLabel->setStyleSheet(labelStyle);
    nameFieldLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    gridLayout->addWidget(nameFieldLabel, row, 0);

    nameEdit = new QLineEdit(formPanel);
    nameEdit->setPlaceholderText("Tên khách hàng");
    nameEdit->setMinimumWidth(200);
    nameEdit->setMaximumWidth(200);
    nameEdit->setReadOnly(true); // Default read-only until phone entered
    nameEdit->setStyleSheet(readOnlyStyle);
    gridLayout->addWidget(nameEdit, row, 1);

    QLabel *typeLabel = new QLabel("Loại sân:", formPanel);
    typeLabel->setStyleSheet(labelStyle);
    typeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    gridLayout->addWidget(typeLabel, row, 3); // Col 3

    typeCombo = new QComboBox(formPanel);
    typeCombo->addItem("Sân 5");
    typeCombo->addItem("Sân 7");
    typeCombo->setEnabled(false);
    typeCombo->setMinimumWidth(200);
    typeCombo->setMaximumWidth(200);
    typeCombo->setStyleSheet(readOnlyComboStyle);
    gridLayout->addWidget(typeCombo, row, 4); // Col 4
    row++;

    // === ROW 2: Giá sân | Khu vực ===
    QLabel *priceFieldLabel = new QLabel("Giá sân:", formPanel);
    priceFieldLabel->setStyleSheet(labelStyle);
    priceFieldLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    gridLayout->addWidget(priceFieldLabel, row, 0);

    priceLabel = new QLabel("0 VND", formPanel);
    priceLabel->setMinimumWidth(200);
    priceLabel->setMaximumWidth(200);
    priceLabel->setStyleSheet(
        "color: #16a34a; "
        "font-weight: bold; "
        "padding: 6px 10px; "
        "border: 1px solid #d1fae5; "
        "border-radius: 4px; "
        "background-color: #f0fdf4; "
        "font-size: 13px; "
        "min-height: 36px;");
    gridLayout->addWidget(priceLabel, row, 1);

    QLabel *areaLabel = new QLabel("Khu vực:", formPanel);
    areaLabel->setStyleSheet(labelStyle);
    areaLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    gridLayout->addWidget(areaLabel, row, 3); // Col 3

    QComboBox *areaCombo = new QComboBox(formPanel);
    areaCombo->setObjectName("areaCombo");
    areaCombo->addItem("Khu A");
    areaCombo->addItem("Khu B");
    areaCombo->addItem("Khu C");
    areaCombo->addItem("Khu D");
    areaCombo->setEnabled(false);
    areaCombo->setMinimumWidth(200);
    areaCombo->setMaximumWidth(200);
    areaCombo->setStyleSheet(readOnlyComboStyle);
    gridLayout->addWidget(areaCombo, row, 4); // Col 4
    row++;

    // === ROW 3: Cọc | Trạng thái (Moved Status here) ===
    QLabel *cocLabel = new QLabel("Cọc:", formPanel);
    cocLabel->setStyleSheet(labelStyle);
    cocLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    gridLayout->addWidget(cocLabel, row, 0); // Col 0

    // Deposit amount label - màu vàng
    depositLabel = new QLabel("0 VND", formPanel);
    depositLabel->setMinimumWidth(200);
    depositLabel->setMaximumWidth(200);
    depositLabel->setStyleSheet(
        "color: #f59e0b; " // Orange/Yellow text
        "font-weight: bold; "
        "padding: 6px 10px; "
        "border: 1px solid #fcd34d; " // Yellow border
        "border-radius: 4px; "
        "background-color: #fef3c7; " // Light yellow bg
        "font-size: 13px; "
        "min-height: 36px;");
    gridLayout->addWidget(depositLabel, row, 1); // Col 1

    // Trạng thái label (bên phải cột 2)
    QLabel *statusLabel = new QLabel("Trạng thái:", formPanel);
    statusLabel->setStyleSheet(labelStyle);
    statusLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    gridLayout->addWidget(statusLabel, row, 3); // Col 3

    statusCombo = new QComboBox(formPanel);
    statusCombo->addItem("Đã đặt");
    statusCombo->addItem("Hoàn thành");
    statusCombo->addItem("Đã hủy");
    statusCombo->setEnabled(false);
    statusCombo->setMinimumWidth(200);
    statusCombo->setMaximumWidth(200);
    statusCombo->setStyleSheet(readOnlyComboStyle);
    gridLayout->addWidget(statusCombo, row, 4); // Col 4
    row++;

    // === ROW 4: Ngày: box từ box đến box (Status removed from here) ===
    QLabel *ngayLabel = new QLabel("Ngày:", formPanel);
    ngayLabel->setStyleSheet(labelStyle);
    ngayLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    gridLayout->addWidget(ngayLabel, row, 0); // Cột 0

    QHBoxLayout *dateTimeLayout = new QHBoxLayout();
    dateTimeLayout->setSpacing(6);                  // Giảm từ 8px xuống 6px
    dateTimeLayout->setContentsMargins(0, 0, 0, 0); // Remove margins to align with column 1
    dateTimeLayout->setAlignment(Qt::AlignVCenter); // Align items vertically center

    // Date box
    dateEdit = new QDateEdit(formPanel);
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    dateEdit->setCalendarPopup(false); // Tắt popup calendar
    dateEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    dateEdit->setReadOnly(true); // Chỉ đọc, không cho chỉnh sửa
    dateEdit->setMinimumWidth(95);
    dateEdit->setMaximumWidth(110);
    dateEdit->setMinimumHeight(26); // Reduced height
    dateEdit->setMaximumHeight(26);
    dateEdit->setStyleSheet("QDateEdit { " + readOnlyDateTimeStyle + " min-height: 26px; max-height: 26px; padding: 2px 8px; }"); // Reduced padding
    dateTimeLayout->addWidget(dateEdit, 0, Qt::AlignVCenter);

    // "từ" label - IN ĐẬM
    QLabel *fromLabel = new QLabel("từ", formPanel);
    fromLabel->setStyleSheet("color: #1f2937; font-size: 13px; font-weight: bold;");
    dateTimeLayout->addWidget(fromLabel, 0, Qt::AlignVCenter);

    // From time box
    fromTimeEdit = new QTimeEdit(formPanel);
    fromTimeEdit->setDisplayFormat("HH:mm");
    fromTimeEdit->setTime(QTime(7, 0));
    fromTimeEdit->setReadOnly(true);
    fromTimeEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    fromTimeEdit->setMinimumWidth(60);
    fromTimeEdit->setMaximumWidth(70);
    fromTimeEdit->setMinimumHeight(26); // Reduced height
    fromTimeEdit->setMaximumHeight(26);
    fromTimeEdit->setStyleSheet("QTimeEdit { " + readOnlyDateTimeStyle + " min-height: 26px; max-height: 26px; padding: 2px 8px; }"); // Reduced padding
    dateTimeLayout->addWidget(fromTimeEdit, 0, Qt::AlignVCenter);

    // "đến" label - IN ĐẬM
    QLabel *toLabel = new QLabel("đến", formPanel);
    toLabel->setStyleSheet("color: #1f2937; font-size: 13px; font-weight: bold;");
    dateTimeLayout->addWidget(toLabel, 0, Qt::AlignVCenter);

    // To time box
    toTimeEdit = new QTimeEdit(formPanel);
    toTimeEdit->setDisplayFormat("HH:mm");
    toTimeEdit->setTime(QTime(7, 0)); // Default 0h duration
    toTimeEdit->setReadOnly(true);
    toTimeEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    toTimeEdit->setMinimumWidth(60);
    toTimeEdit->setMaximumWidth(70);
    toTimeEdit->setMinimumHeight(26); // Reduced height
    toTimeEdit->setMaximumHeight(26);
    toTimeEdit->setStyleSheet("QTimeEdit { " + readOnlyDateTimeStyle + " min-height: 26px; max-height: 26px; padding: 2px 8px; }"); // Reduced padding
    dateTimeLayout->addWidget(toTimeEdit, 0, Qt::AlignVCenter);

    // Duration label - màu xanh dương
    durationLabel = new QLabel("0h", formPanel); // Default 0h
    durationLabel->setMinimumHeight(26);
    durationLabel->setMaximumHeight(26);
    durationLabel->setAlignment(Qt::AlignCenter);
    durationLabel->setStyleSheet(
        "color: #1e40af; " // Dark blue text
        "font-weight: bold; "
        "font-size: 12px; "
        "padding: 0px 8px; "          // Reduced padding for fixed height
        "background-color: #dbeafe; " // Light blue bg
        "border-radius: 4px;");
    dateTimeLayout->addWidget(durationLabel, 0, Qt::AlignVCenter);
    dateTimeLayout->addStretch();

    QWidget *dateTimeWidget = new QWidget(formPanel);
    dateTimeWidget->setLayout(dateTimeLayout);
    // Removed negative margin to fix border visibility
    dateTimeWidget->setStyleSheet("background-color: transparent;");
    gridLayout->addWidget(dateTimeWidget, row, 1, Qt::AlignTop); // Cột 1

    // Hidden duration buttons (still needed for code but not shown)
    duration30Btn = new QPushButton(formPanel);
    duration30Btn->setVisible(false);
    duration60Btn = new QPushButton(formPanel);
    duration60Btn->setVisible(false);
    duration90Btn = new QPushButton(formPanel);
    duration90Btn->setVisible(false);
    duration120Btn = new QPushButton(formPanel);
    duration120Btn->setVisible(false);

    // Connect phone edit to auto-lookup customer
    connect(phoneEdit, &QLineEdit::textChanged, [this](const QString &phone)
            {
        if (phone.length() < 10) {
            nameEdit->clear();
            nameEdit->setReadOnly(true);
            nameEdit->setStyleSheet("padding: 6px 10px; border: 1px solid #e5e7eb; border-radius: 4px; background-color: #f9fafb; color: #6b7280; font-size: 13px; min-height: 36px;");
            phoneEdit->setProperty("hasCustomer", false);
            return;
        }
        
        QuanLyKhachHang *qlkh = system->layQuanLyKhachHang();
        if (qlkh) {
            KhachHang *customer = qlkh->timKhachHangTheoSDT(phone.toStdString());
            if (customer) {
                nameEdit->setText(QString::fromStdString(customer->layHoTen()));
                nameEdit->setReadOnly(true);
                nameEdit->setStyleSheet("padding: 6px 10px; border: 1px solid #e5e7eb; border-radius: 4px; background-color: #f9fafb; color: #6b7280; font-size: 13px; min-height: 36px;");
                phoneEdit->setProperty("hasCustomer", true);
            } else {
                // New customer - allow editing name
                if (nameEdit->text().isEmpty() || nameEdit->isReadOnly()) {
                     nameEdit->clear();
                }
                nameEdit->setPlaceholderText("Nhập tên khách hàng mới...");
                nameEdit->setReadOnly(false);
                nameEdit->setStyleSheet("padding: 6px 10px; border: 1px solid #d1d5db; border-radius: 4px; background-color: white; font-size: 13px; min-height: 36px;");
                phoneEdit->setProperty("hasCustomer", false);
            }
        } });

    // === RIGHT COLUMN: Note + Buttons ===
    QVBoxLayout *rightColumnLayout = new QVBoxLayout();
    rightColumnLayout->setSpacing(10);
    rightColumnLayout->setContentsMargins(0, 0, 0, 0);

    // Note Label
    QLabel *noteLabel = new QLabel("Ghi chú:", formPanel);
    noteLabel->setStyleSheet(labelStyle);
    noteLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    rightColumnLayout->addWidget(noteLabel);

    // Note Edit (Height increased to 2 lines ~ 60-70px)
    noteEdit = new QTextEdit(formPanel);
    noteEdit->setPlaceholderText("Ghi chú...");
    noteEdit->setFixedWidth(200);   // Fixed width 200px
    noteEdit->setMinimumHeight(70); // 2 lines height
    noteEdit->setMaximumHeight(70);
    noteEdit->setStyleSheet(
        "QTextEdit { "
        "padding: 6px 10px; "
        "border: 1px solid #4b5563; " // Darker border
        "border-radius: 4px; "
        "background-color: white; "
        "font-size: 13px; "
        "} "
        "QTextEdit:focus { "
        "border: 2px solid #16a34a; "
        "}");
    rightColumnLayout->addWidget(noteEdit);

    // Buttons
    QString btnStyle = "QPushButton { "
                       "background-color: %1; "
                       "color: white; "
                       "border: none; "
                       "border-radius: 4px; "
                       "padding: 8px 10px; "
                       "font-weight: bold; "
                       "font-size: 13px; "
                       "min-width: 200px; " // Fixed width 200px
                       "max-width: 200px; " // Fixed width 200px
                       "min-height: 36px; "
                       "max-height: 36px; "
                       "} "
                       "QPushButton:hover { background-color: %2; } ";

    saveBtn = new QPushButton("✓ Lưu", formPanel);
    saveBtn->setStyleSheet(btnStyle.arg("#16a34a").arg("#15803d"));
    rightColumnLayout->addWidget(saveBtn);

    deleteBtn = new QPushButton("✗ Hủy", formPanel);
    deleteBtn->setStyleSheet(btnStyle.arg("#ef4444").arg("#dc2626"));
    deleteBtn->setEnabled(true);
    rightColumnLayout->addWidget(deleteBtn);

    rightColumnLayout->addStretch();

    // Add form grid and right column to main content layout
    mainContentLayout->addLayout(gridLayout, 7);        // 70% for form
    mainContentLayout->addLayout(rightColumnLayout, 3); // 30% for note + buttons

    formLayout->addLayout(mainContentLayout);
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
}

void TimelineTab::setupConnections()
{
    // Calendar
    connect(calendar, &QCalendarWidget::clicked, this, &TimelineTab::onDateSelected);

    // Form - auto-search on phone text changed
    connect(phoneEdit, &QLineEdit::textChanged, this, &TimelineTab::onPhoneSearchClicked);
    connect(fieldCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TimelineTab::onFieldChanged);

    // Buttons
    connect(saveBtn, &QPushButton::clicked, this, &TimelineTab::onSaveClicked);
    connect(deleteBtn, &QPushButton::clicked, this, &TimelineTab::onCancelSelectionClicked); // Cancel selection button
    // Removed: checkin button connection

    // Duration quick select
    connect(duration30Btn, &QPushButton::clicked, this, &TimelineTab::onDuration30Clicked);
    connect(duration60Btn, &QPushButton::clicked, this, &TimelineTab::onDuration60Clicked);
    connect(duration90Btn, &QPushButton::clicked, this, &TimelineTab::onDuration90Clicked);
    connect(duration120Btn, &QPushButton::clicked, this, &TimelineTab::onDuration120Clicked);

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
            // Removed suffix as requested
            fieldCombo->addItem(fieldName, i);
        }
    }

    if (fieldCombo->count() > 0)
    {
        fieldCombo->setCurrentIndex(0);
        onFieldChanged(0);
    }

    // Load customers into combo
    QComboBox *customerCombo = formPanel->findChild<QComboBox *>("customerCombo");
    if (customerCombo)
    {
        customerCombo->clear();
        customerCombo->addItem("-- Chọn khách hàng --", "");

        QuanLyKhachHang *qlkh = system->layQuanLyKhachHang();
        if (qlkh)
        {
            const MangDong<KhachHang *> &customers = qlkh->layDanhSachKhachHang();
            for (int i = 0; i < customers.size(); i++)
            {
                KhachHang *kh = customers[i];
                if (kh)
                {
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
    // Reload fields in case they changed
    loadFields();

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
    nameEdit->clear();
    nameEdit->setReadOnly(true);
    nameEdit->setStyleSheet("padding: 6px 10px; border: 1px solid #e5e7eb; border-radius: 4px; background-color: #f9fafb; color: #6b7280; font-size: 13px; min-height: 36px;");

    // Reset customer combo to "-- Chọn khách hàng --"
    QComboBox *customerCombo = formPanel->findChild<QComboBox *>("customerCombo");
    if (customerCombo)
    {
        customerCombo->setCurrentIndex(0);
    }

    if (fieldCombo->count() > 0)
    {
        fieldCombo->setCurrentIndex(0);
        // Force update area for the first field
        onFieldChanged(0);
    }
    statusCombo->setCurrentIndex(0); // Default to "Đã đặt"
    typeCombo->setCurrentIndex(0);
    dateEdit->setDate(selectedDate);
    fromTimeEdit->setTime(QTime(0, 0));
    toTimeEdit->setTime(QTime(0, 0)); // Default 00:00
    noteEdit->clear();

    if (calendar)
        calendar->setEnabled(true);

    currentBooking = nullptr;
    isEditMode = false;

    // Reset buttons state
    saveBtn->setEnabled(true);
    saveBtn->setStyleSheet("QPushButton { background-color: #16a34a; color: white; border: none; border-radius: 4px; padding: 8px 10px; font-weight: bold; font-size: 13px; min-width: 200px; max-width: 200px; min-height: 36px; max-height: 36px; } QPushButton:hover { background-color: #15803d; }");

    deleteBtn->setText("✗ Hủy");
    deleteBtn->setEnabled(false); // Disable by default when clearing form
    deleteBtn->setStyleSheet("QPushButton { background-color: #ef4444; color: white; border: none; border-radius: 4px; padding: 8px 10px; font-weight: bold; font-size: 13px; min-width: 200px; max-width: 200px; min-height: 36px; max-height: 36px; } QPushButton:hover { background-color: #dc2626; }");

    // Reset inputs to editable
    phoneEdit->setReadOnly(false);
    phoneEdit->setStyleSheet("padding: 6px 10px; border: 1px solid #d1d5db; border-radius: 4px; background-color: white; font-size: 13px; min-height: 36px;");

    noteEdit->setReadOnly(false);
    noteEdit->setStyleSheet("QTextEdit { padding: 6px 10px; border: 1px solid #4b5563; border-radius: 4px; background-color: white; font-size: 13px; } QTextEdit:focus { border: 2px solid #16a34a; }");

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
        nameEdit->setText(QString::fromStdString(customer->layHoTen()));
        nameEdit->setReadOnly(true);
        nameEdit->setStyleSheet("padding: 6px 10px; border: 1px solid #e5e7eb; border-radius: 4px; background-color: #f9fafb; color: #6b7280; font-size: 13px; min-height: 36px;");

        // Sync customer combo
        QComboBox *customerCombo = formPanel->findChild<QComboBox *>("customerCombo");
        if (customerCombo)
        {
            for (int i = 0; i < customerCombo->count(); i++)
            {
                if (customerCombo->itemData(i).toString() == phone)
                {
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

        // Update Area combo explicitly (though onFieldChanged might handle it, better safe)
        QComboBox *areaCombo = formPanel->findChild<QComboBox *>("areaCombo");
        if (areaCombo)
        {
            int areaIndex = static_cast<int>(san->layKhuVuc());
            if (areaIndex >= 0 && areaIndex < areaCombo->count())
            {
                areaCombo->setCurrentIndex(areaIndex);
            }
        }
    }

    // Status
    int statusIndex = static_cast<int>(booking->getTrangThai());
    if (statusIndex >= 0 && statusIndex < statusCombo->count())
    {
        statusCombo->setCurrentIndex(statusIndex);
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
    noteEdit->setText(QString::fromStdString(booking->getGhiChu()));

    // ===== READ-ONLY MODE FOR EXISTING BOOKINGS =====
    // Disable all inputs
    phoneEdit->setReadOnly(true);
    phoneEdit->setStyleSheet("padding: 6px 10px; border: 1px solid #e5e7eb; border-radius: 4px; background-color: #f9fafb; color: #6b7280; font-size: 13px; min-height: 36px;");

    noteEdit->setReadOnly(true);
    noteEdit->setStyleSheet("padding: 6px 10px; border: 1px solid #e5e7eb; border-radius: 4px; background-color: #f9fafb; color: #6b7280; font-size: 13px;");

    // Disable buttons (Grayed out)
    saveBtn->setEnabled(false);
    saveBtn->setStyleSheet("QPushButton { background-color: #9ca3af; color: white; border: none; border-radius: 4px; padding: 8px 10px; font-weight: bold; font-size: 13px; min-width: 200px; max-width: 200px; min-height: 36px; max-height: 36px; }");

    deleteBtn->setText("✗ Hủy đặt sân");
    deleteBtn->setEnabled(false); // Disabled as requested (cancellation moved to Tab 2)
    deleteBtn->setStyleSheet("QPushButton { background-color: #9ca3af; color: white; border: none; border-radius: 4px; padding: 8px 10px; font-weight: bold; font-size: 13px; min-width: 200px; max-width: 200px; min-height: 36px; max-height: 36px; }");

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

    if (mins == 0)
    {
        durationLabel->setText(QString("%1h").arg(hours));
    }
    else
    {
        durationLabel->setText(QString("%1h%2m").arg(hours).arg(mins, 2, 10, QChar('0')));
    }

    // Auto-calculate price based on duration and field price
    int fieldIndex = fieldCombo->currentData().toInt();
    if (fieldIndex >= 0 && fieldIndex < fields.size() && minutes > 0)
    {
        San *san = fields[fieldIndex];
        if (san)
        {
            double pricePerHour = san->layGiaThueGio();
            double durationHours = minutes / 60.0;
            double totalPrice = pricePerHour * durationHours;
            priceLabel->setText(formatCurrency(totalPrice));

            // Tính cọc 30% tiền sân
            double depositAmount = totalPrice * 0.3;
            depositLabel->setText(formatCurrency(depositAmount));
        }
    }
    else
    {
        priceLabel->setText("0 VND");
        depositLabel->setText("0 VND");
    }
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
        nameEdit->clear();
        nameEdit->setReadOnly(true);
        phoneEdit->setProperty("hasCustomer", false);
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
        // Found - auto fill name
        nameEdit->setText(QString::fromStdString(customer->layHoTen()));
        nameEdit->setReadOnly(true);
        nameEdit->setStyleSheet("padding: 6px 10px; border: 1px solid #e5e7eb; border-radius: 4px; background-color: #f9fafb; color: #6b7280; font-size: 13px; min-height: 36px;");
        phoneEdit->setProperty("hasCustomer", true);
    }
    else
    {
        // Not found - allow entering new name
        if (phone.length() >= 10)
        {
            nameEdit->setReadOnly(false);
            nameEdit->setPlaceholderText("Nhập tên khách hàng mới...");
            nameEdit->setStyleSheet("padding: 6px 10px; border: 1px solid #d1d5db; border-radius: 4px; background-color: white; font-size: 13px; min-height: 36px;");
            phoneEdit->setProperty("hasCustomer", false);
        }
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

        // Update type combo
        if (san->layLoaiSan() == LoaiSan::SAN_5)
        {
            typeCombo->setCurrentIndex(0); // 5 a side
        }
        else
        {
            typeCombo->setCurrentIndex(1); // 7 a side
        }

        // Update Area combo
        QComboBox *areaCombo = formPanel->findChild<QComboBox *>("areaCombo");
        if (areaCombo)
        {
            // Map KhuVuc enum to combo index
            // KhuVuc::A = 0, B = 1, C = 2, D = 3
            int areaIndex = static_cast<int>(san->layKhuVuc());
            if (areaIndex >= 0 && areaIndex < areaCombo->count())
            {
                areaCombo->setCurrentIndex(areaIndex);
            }
        }

        // Update duration and price
        updateDuration();
    }
}

void TimelineTab::onSaveClicked()
{
    qDebug() << "=== SAVE BOOKING DEBUG ===";

    // 1. Validate Slot Selection First
    QTime from = fromTimeEdit->time();
    QTime to = toTimeEdit->time();

    if (from == to)
    {
        QMessageBox::warning(this, "❌ Lỗi", "Vui lòng chọn khung giờ trên lịch!");
        return;
    }

    if (fieldCombo->currentIndex() < 0)
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn sân!");
        return;
    }

    if (from >= to)
    {
        QMessageBox::warning(this, "Lỗi", "Giờ kết thúc phải sau giờ bắt đầu!");
        return;
    }

    // 2. Validate Customer
    QString phone = phoneEdit->text().trimmed();
    QString name = nameEdit->text().trimmed();

    qDebug() << "Phone from phoneEdit:" << phone;
    qDebug() << "Has customer property:" << phoneEdit->property("hasCustomer").toBool();

    if (phone.isEmpty())
    {
        QMessageBox::warning(this, "❌ Lỗi", "Vui lòng nhập số điện thoại!");
        return;
    }

    // Validate phone format: Must start with 0 and have 10 digits
    QRegularExpression phoneRegex("^0[0-9]{9}$");
    if (!phoneRegex.match(phone).hasMatch())
    {
        QMessageBox::warning(this, "❌ Lỗi", "Số điện thoại không hợp lệ!\nPhải bắt đầu bằng số 0 và có đúng 10 chữ số.");
        return;
    }

    // Find customer
    QuanLyKhachHang *quanLyKH = system->layQuanLyKhachHang();
    std::string phoneStd = phone.toStdString();
    KhachHang *customer = quanLyKH ? quanLyKH->timKhachHangTheoSDT(phoneStd) : nullptr;

    // If customer not found but name is entered -> Create new customer
    if (!customer)
    {
        if (name.isEmpty())
        {
            QMessageBox::warning(this, "Lỗi", "Khách hàng chưa tồn tại. Vui lòng nhập tên khách hàng!");
            nameEdit->setFocus();
            return;
        }

        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Khách hàng mới",
            QString("Số điện thoại %1 chưa có trong hệ thống.\nBạn có muốn thêm khách hàng '%2' và tiếp tục đặt sân?").arg(phone).arg(name),
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No)
        {
            return;
        }

        // Create new customer
        try
        {
            customer = new KhachHang(
                name.toStdString(),
                phoneStd,
                "", // empty address
                quanLyKH->taoMaKhachHangMoi());
            quanLyKH->themKhachHang(customer);

            // Save system immediately to persist customer
            try
            {
                system->luuHeThong("D:/PBL2-/Data/data.bin");
            }
            catch (...)
            {
                qDebug() << "Warning: Could not save to data.bin";
            }

            phoneEdit->setProperty("hasCustomer", true);
            nameEdit->setReadOnly(true);
            nameEdit->setStyleSheet("padding: 6px 10px; border: 1px solid #e5e7eb; border-radius: 4px; background-color: #f9fafb; color: #6b7280; font-size: 13px; min-height: 36px;");
        }
        catch (const std::exception &e)
        {
            QMessageBox::critical(this, "Lỗi", QString("Không thể tạo khách hàng mới: %1").arg(e.what()));
            return;
        }
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
    // FIX: Correct order is Day, Month, Year, Hour, Minute, Second
    NgayGio ngayGio(date.day(), date.month(), date.year(), from.hour(), from.minute(), 0);

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

        if (reply == QMessageBox::No)
        {
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
            QString updateMsg = QString(
                                    "✅ CẬP NHẬT THÀNH CÔNG\n\n"
                                    "📋 Mã đặt sân: %1\n"
                                    "⚽ Sân: %2\n"
                                    "🕐 Thời gian: %3 - %4\n"
                                    "📅 Ngày: %5\n\n"
                                    "Thông tin đã được cập nhật!")
                                    .arg(QString::fromStdString(currentBooking->getMaDatSan()))
                                    .arg(QString::fromStdString(currentBooking->getSan()->getTenSan()))
                                    .arg(QString::fromStdString(currentBooking->getKhungGio().layGioBatDau().toString()))
                                    .arg(QString::fromStdString(currentBooking->getKhungGio().layGioKetThuc().toString()))
                                    .arg(QString::fromStdString(currentBooking->getThoiGianDat().getNgayThang().toString()));
            QMessageBox::information(this, "✅ Thành công", updateMsg);
        }
        else
        {
            // Create new booking
            DatSan *newBooking = quanLyDS->taoDatSan(customer, san, ngayGio, khungGio);
            if (newBooking)
            {
                // Set Note
                newBooking->setGhiChu(noteEdit->toPlainText().toStdString());

                // ===== TỰ ĐỘNG TÍNH TIỀN CỌC (30%) =====
                newBooking->tinhTienCoc();

                // ===== SAVE DATA TO FILE =====
                try
                {
                    system->luuHeThong("D:/PBL2-/Data/data.bin");
                }
                catch (...)
                {
                    qDebug() << "Warning: Could not save to data.bin";
                }

                // ===== REFRESH TIMELINE TRƯỚC =====
                timelineGrid->clearPendingSelection();
                timelineGrid->setDate(selectedDate);
                timelineGrid->loadBookings();
                timelineGrid->repaint(); // Force repaint to show new booking immediately

                // ===== UNLOCK GRID để cho phép chọn ô mới =====
                if (timelineGrid)
                {
                    timelineGrid->setProperty("isLocked", false);
                }

                // ===== EMIT SIGNAL SAU KHI REFRESH (để Tab2 thấy dữ liệu mới) =====
                emit bookingDataChanged();

                // ===== CLEAR FORM =====
                clearForm();

                // ===== THÔNG BÁO =====
                QMessageBox::information(this, "✅ Thành công",
                                         "Đặt sân thành công!\n\n"
                                         "✓ Trạng thái: Đã đặt\n"
                                         "💰 Tiền cọc (30%): " +
                                             formatCurrency(newBooking->getTienCoc()) + "\n\n"
                                                                                        "📋 Xem chi tiết tại Tab 2");
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
    // Check if we are in Edit Mode (canceling a booking) or Selection Mode (canceling a selection)
    if (isEditMode && currentBooking)
    {
        onDeleteClicked(); // Call the actual delete/cancel booking logic
        return;
    }

    // Clear pending selection in timeline grid
    if (timelineGrid)
    {
        timelineGrid->clearPendingSelection();
        // UNLOCK timeline grid to allow new selection
        timelineGrid->setProperty("isLocked", false);
    }

    // Clear form
    clearForm();

    // QMessageBox::information(this, "✓ Đã hủy", "Đã xóa vùng chọn. Bạn có thể chọn lại."); // Removed popup for smoother UX
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
                    try
                    {
                        system->luuHeThong("D:/PBL2-/Data/data.bin");
                    }
                    catch (...)
                    {
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

void TimelineTab::onTimelineSlotSelected(int fieldIndex, int startHour, int startMinute, int durationMinutes)
{
    // Check for past time
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QDate currentDate = currentDateTime.date();
    QTime currentTime = currentDateTime.time();

    QTime slotTime(startHour, startMinute);

    if (selectedDate < currentDate)
    {
        QMessageBox::warning(this, "Lỗi", "Không thể đặt sân ở quá khứ!");
        return;
    }
    else if (selectedDate == currentDate && slotTime < currentTime)
    {
        QMessageBox::warning(this, "Lỗi", "Không thể đặt sân ở khung giờ đã qua!");
        return;
    }

    // LOCK MECHANISM: If already has pending selection, don't allow new selection
    // User must click Cancel button to clear selection first
    if (timelineGrid && timelineGrid->property("isLocked").toBool())
    {
        QMessageBox::warning(this, "⚠️ Bị khóa",
                             "Bạn đã chọn một ô!\n\n"
                             "Để chọn ô khác, vui lòng nhấn nút Hủy trước.");
        return;
    }

    // IMPORTANT: Don't call clearForm() - keep customer selection!
    // Just reset time and field, keep customer info

    // Reset booking mode
    currentBooking = nullptr;
    isEditMode = false;
    deleteBtn->setEnabled(false);

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

    // LOCK timeline grid to prevent re-selection
    timelineGrid->setProperty("isLocked", true);
    if (calendar)
        calendar->setEnabled(false);

    // Update Cancel button state
    deleteBtn->setText("✗ Hủy chọn");
    deleteBtn->setEnabled(true);
    deleteBtn->setStyleSheet("QPushButton { background-color: #ef4444; color: white; border: none; border-radius: 4px; padding: 8px 10px; font-weight: bold; font-size: 13px; min-width: 200px; max-width: 200px; min-height: 36px; max-height: 36px; } QPushButton:hover { background-color: #dc2626; }");

    // Enable Save button
    saveBtn->setEnabled(true);
    saveBtn->setStyleSheet("QPushButton { background-color: #16a34a; color: white; border: none; border-radius: 4px; padding: 8px 10px; font-weight: bold; font-size: 13px; min-width: 200px; max-width: 200px; min-height: 36px; max-height: 36px; } QPushButton:hover { background-color: #15803d; }");

    // Enable inputs
    phoneEdit->setReadOnly(false);
    phoneEdit->setStyleSheet("padding: 6px 10px; border: 1px solid #d1d5db; border-radius: 4px; background-color: white; font-size: 13px; min-height: 36px;");
    noteEdit->setReadOnly(false);
    noteEdit->setStyleSheet("QTextEdit { padding: 6px 10px; border: 1px solid #4b5563; border-radius: 4px; background-color: white; font-size: 13px; } QTextEdit:focus { border: 2px solid #16a34a; }");

    // If no customer selected, focus phone field
    if (phoneEdit->text().trimmed().isEmpty())
    {
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

    connect(saveBtn, &QPushButton::clicked, [=]()
            {
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
                
                // Save to binary file
                HeThongQuanLy::getInstance()->luuHeThong("D:/PBL2-/Data/data.bin");
                
                QMessageBox::information(dialog, "Thành công", "Đã thêm khách hàng!");
                
                // Update form
                phoneEdit->setText(phone);
                nameEdit->setText(name);
                nameEdit->setReadOnly(true);
                phoneEdit->setProperty("hasCustomer", true);
                
                dialog->accept();
            } catch (const std::exception &e) {
                QMessageBox::critical(dialog, "Lỗi", QString("Không thể thêm khách hàng: %1").arg(e.what()));
            }
        } });

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

void TimelineTab::reloadFields()
{
    fieldCombo->clear();
    fields.clear();

    QuanLySan *quanLySan = system->layQuanLySan();
    if (quanLySan)
    {
        fields = quanLySan->layDanhSachSan();
        for (int i = 0; i < fields.size(); ++i)
        {
            fieldCombo->addItem(QString::fromStdString(fields[i]->getTenSan()), i);
        }
    }
}

void TimelineTab::updateCalendarDates()
{
    // Reset formatting
    QTextCharFormat defaultFormat;
    calendar->setDateTextFormat(QDate(), defaultFormat);

    QuanLyDatSan *quanLyDS = system->layQuanLyDatSan();
    if (!quanLyDS)
        return;

    // Get all bookings
    MangDong<DatSan *> bookings = quanLyDS->layDanhSachDatSan();

    // Map to store status for each date
    // 0: no booking, 1: has booking
    std::map<QDate, int> dateStatus;

    for (int i = 0; i < bookings.size(); ++i)
    {
        DatSan *booking = bookings[i];
        if (booking->getTrangThai() == TrangThaiDatSan::DA_HUY)
            continue;

        NgayGio ng = booking->getThoiGianDat();
        QDate date(ng.getNam(), ng.getThang(), ng.getNgay());
        dateStatus[date] = 1;
    }

    // Apply formatting
    QTextCharFormat hasBookingFormat;
    hasBookingFormat.setFontWeight(QFont::Bold);
    hasBookingFormat.setForeground(QBrush(QColor("#16a34a"))); // Green for days with bookings

    for (const auto &pair : dateStatus)
    {
        calendar->setDateTextFormat(pair.first, hasBookingFormat);
    }
}

bool TimelineTab::checkBookingConflict(San *san, const NgayGio &ngayGio, const KhungGio &khungGio)
{
    QuanLyDatSan *quanLyDS = system->layQuanLyDatSan();
    if (!quanLyDS)
        return false;

    MangDong<DatSan *> bookings = quanLyDS->layDanhSachDatSan();

    // Convert input time to minutes for easier comparison
    int startMin = khungGio.layGioBatDau().getGio() * 60 + khungGio.layGioBatDau().getPhut();
    int endMin = khungGio.layGioKetThuc().getGio() * 60 + khungGio.layGioKetThuc().getPhut();

    for (int i = 0; i < bookings.size(); ++i)
    {
        DatSan *booking = bookings[i];
        // Skip cancelled bookings
        if (booking->getTrangThai() == TrangThaiDatSan::DA_HUY)
            continue;

        // Skip current booking if in edit mode
        if (isEditMode && currentBooking && booking->getMaDatSan() == currentBooking->getMaDatSan())
            continue;

        // Check same field
        if (booking->getSan()->getMaSan() != san->getMaSan())
            continue;

        // Check same date
        NgayGio bNgay = booking->getThoiGianDat();
        if (bNgay.getNgay() != ngayGio.getNgay() ||
            bNgay.getThang() != ngayGio.getThang() ||
            bNgay.getNam() != ngayGio.getNam())
            continue;

        // Check time overlap
        KhungGio bKhung = booking->getKhungGio();
        int bStart = bKhung.layGioBatDau().getGio() * 60 + bKhung.layGioBatDau().getPhut();
        int bEnd = bKhung.layGioKetThuc().getGio() * 60 + bKhung.layGioKetThuc().getPhut();

        // Overlap logic: (StartA < EndB) and (EndA > StartB)
        if (startMin < bEnd && endMin > bStart)
        {
            return true;
        }
    }

    return false;
}