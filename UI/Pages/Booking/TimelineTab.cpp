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
    : QWidget(parent), mainLayout(nullptr), topPanel(nullptr), topPanelLayout(nullptr), calendarPanel(nullptr), calendarLayout(nullptr), calendar(nullptr), formPanel(nullptr), formLayout(nullptr), phoneEdit(nullptr), nameLabel(nullptr), fieldCombo(nullptr), priceLabel(nullptr), statusCombo(nullptr), typeCombo(nullptr), dateEdit(nullptr), fromTimeEdit(nullptr), toTimeEdit(nullptr), durationLabel(nullptr), duration30Btn(nullptr), duration60Btn(nullptr), duration90Btn(nullptr), duration120Btn(nullptr), noteEdit(nullptr), saveBtn(nullptr), deleteBtn(nullptr), timelinePanel(nullptr), timelineLayout(nullptr), timelineGrid(nullptr), system(nullptr), currentBooking(nullptr), isEditMode(false)
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
    calendar->setMinimumDate(QDate::currentDate());
    calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    // Remove fixed height to allow calendar to expand
    calendar->setMinimumHeight(200);

    // Highlight current date with custom format
    QTextCharFormat todayFormat;
    todayFormat.setBackground(QBrush(QColor(220, 252, 231))); // Light green background (#dcfce7)
    todayFormat.setForeground(QBrush(QColor(22, 163, 74)));   // Green text (#16a34a)
    todayFormat.setFontWeight(QFont::Bold);
    calendar->setDateTextFormat(QDate::currentDate(), todayFormat);

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

    // Main layout: Form + Buttons
    QHBoxLayout *mainContentLayout = new QHBoxLayout();
    mainContentLayout->setSpacing(100); // Khoảng cách 100px giữa form và buttons

    // Grid layout for form fields
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setVerticalSpacing(5); // Giảm từ 10px xuống 5px

    // Cấu trúc 5 cột:
    // Col 0: Label cột 1
    // Col 1: Box cột 1 (20px spacing auto)
    // Col 2: Spacer 100px (giữa 2 nhóm)
    // Col 3: Label cột 2
    // Col 4: Box cột 2 (20px spacing auto)

    gridLayout->setColumnMinimumWidth(1, 200); // Box cột 1: 200px
    gridLayout->setColumnMinimumWidth(2, 10);  // Spacer giữa 2 cột: 10px
    gridLayout->setColumnMinimumWidth(4, 200); // Box cột 2: 200px (bằng cột 1)

    gridLayout->setHorizontalSpacing(10); // Giảm spacing giữa label và box xuống 10px

    int row = 0;

    // === Độ RỘNG và CAO CỦA CÁC BOXES ===
    // Độ rộng boxes:
    //   - gridLayout->setColumnMinimumWidth(1, 200) ở dòng 156: Box cột 1 = 200px
    //   - gridLayout->setColumnMinimumWidth(4, 200) ở dòng 158: Box cột 2 = 200px
    // Độ cao boxes:
    //   - min-height: 36px trong inputStyle và readOnlyStyle (dòng 165, 172)
    //   - Ghi chú: 60px (setMaximumHeight ở dòng 290)
    //   - Ngày/Giờ: 30px (setMinimumHeight/setMaximumHeight ở dòng 320, 340, 360)

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

    // Style for readonly combobox - no dropdown arrow
    QString readOnlyComboStyle = readOnlyStyle + " QComboBox::drop-down { border: none; width: 0px; background: transparent; } QComboBox::down-arrow { image: none; border: none; width: 0px; height: 0px; } QComboBox { padding-right: 10px; }";

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
    fieldCombo->setStyleSheet("QComboBox { " + readOnlyComboStyle + " }");
    gridLayout->addWidget(fieldCombo, row, 4); // Col 4
    row++;

    // === ROW 1: Họ tên | Loại sân ===
    QLabel *nameFieldLabel = new QLabel("Họ tên:", formPanel);
    nameFieldLabel->setStyleSheet(labelStyle);
    nameFieldLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    gridLayout->addWidget(nameFieldLabel, row, 0);

    nameLabel = new QLabel("-", formPanel);
    nameLabel->setMinimumWidth(200);
    nameLabel->setMaximumWidth(200);
    nameLabel->setStyleSheet("QLabel { " + readOnlyStyle + " }");
    gridLayout->addWidget(nameLabel, row, 1);

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
    typeCombo->setStyleSheet("QComboBox { " + readOnlyComboStyle + " }");
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
    areaCombo->addItem("Khu A");
    areaCombo->addItem("Khu B");
    areaCombo->addItem("Khu C");
    areaCombo->setEnabled(false);
    areaCombo->setMinimumWidth(200);
    areaCombo->setMaximumWidth(200);
    areaCombo->setStyleSheet("QComboBox { " + readOnlyComboStyle + " }");
    gridLayout->addWidget(areaCombo, row, 4); // Col 4
    row++;

    // === ROW 3: Trạng thái | Ghi chú (cùng dòng) ===
    QLabel *statusLabel = new QLabel("Trạng thái:", formPanel);
    statusLabel->setStyleSheet(labelStyle);
    statusLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    gridLayout->addWidget(statusLabel, row, 0); // Col 0

    statusCombo = new QComboBox(formPanel);
    statusCombo->addItem("Chờ xác nhận");
    statusCombo->addItem("Đã xác nhận");
    statusCombo->setEnabled(false);
    statusCombo->setMinimumWidth(200);
    statusCombo->setMaximumWidth(200);
    statusCombo->setStyleSheet("QComboBox { " + readOnlyComboStyle + " }");
    gridLayout->addWidget(statusCombo, row, 1); // Col 1

    QLabel *noteLabel = new QLabel("Ghi chú:", formPanel);
    noteLabel->setStyleSheet(labelStyle);
    noteLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    gridLayout->addWidget(noteLabel, row, 3); // Col 3

    noteEdit = new QTextEdit(formPanel);
    noteEdit->setPlaceholderText("Ghi chú...");
    noteEdit->setMinimumWidth(200);
    noteEdit->setMaximumWidth(200);
    noteEdit->setMinimumHeight(36); // Giống box số điện thoại
    noteEdit->setMaximumHeight(36);
    noteEdit->setStyleSheet(
        "QTextEdit { "
        "padding: 6px 10px; "
        "border: 1px solid #d1d5db; "
        "border-radius: 4px; "
        "background-color: white; "
        "font-size: 13px; "
        "} "
        "QTextEdit:focus { "
        "border: 1px solid #16a34a; "
        "}");
    gridLayout->addWidget(noteEdit, row, 4); // Col 4
    row++;

    // === ROW 4: Ngày: box từ box đến box | Cọc: sotien ===
    QLabel *ngayLabel = new QLabel("Ngày:", formPanel);
    ngayLabel->setStyleSheet(labelStyle);
    ngayLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    gridLayout->addWidget(ngayLabel, row, 0); // Cột 0, dưới Trạng thái

    QHBoxLayout *dateTimeLayout = new QHBoxLayout();
    dateTimeLayout->setSpacing(6); // Giảm từ 8px xuống 6px

    // Date box
    dateEdit = new QDateEdit(formPanel);
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    dateEdit->setCalendarPopup(false); // Tắt popup calendar
    dateEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    dateEdit->setReadOnly(true); // Chỉ đọc, không cho chỉnh sửa
    dateEdit->setMinimumWidth(95);
    dateEdit->setMaximumWidth(110);
    dateEdit->setMinimumHeight(30);
    dateEdit->setMaximumHeight(30);
    dateEdit->setStyleSheet("QDateEdit { " + readOnlyDateTimeStyle + " min-height: 30px; max-height: 30px; padding: 4px 8px; }");
    dateTimeLayout->addWidget(dateEdit);

    // "từ" label - IN ĐẬM
    QLabel *fromLabel = new QLabel("từ", formPanel);
    fromLabel->setStyleSheet("color: #1f2937; font-size: 13px; font-weight: bold;");
    dateTimeLayout->addWidget(fromLabel);

    // From time box
    fromTimeEdit = new QTimeEdit(formPanel);
    fromTimeEdit->setDisplayFormat("HH:mm");
    fromTimeEdit->setTime(QTime(7, 0));
    fromTimeEdit->setReadOnly(true);
    fromTimeEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    fromTimeEdit->setMinimumWidth(60);
    fromTimeEdit->setMaximumWidth(70);
    fromTimeEdit->setMinimumHeight(30);
    fromTimeEdit->setMaximumHeight(30);
    fromTimeEdit->setStyleSheet("QTimeEdit { " + readOnlyDateTimeStyle + " min-height: 30px; max-height: 30px; padding: 4px 8px; }");
    dateTimeLayout->addWidget(fromTimeEdit);

    // "đến" label - IN ĐẬM
    QLabel *toLabel = new QLabel("đến", formPanel);
    toLabel->setStyleSheet("color: #1f2937; font-size: 13px; font-weight: bold;");
    dateTimeLayout->addWidget(toLabel);

    // To time box
    toTimeEdit = new QTimeEdit(formPanel);
    toTimeEdit->setDisplayFormat("HH:mm");
    toTimeEdit->setTime(QTime(7, 30));
    toTimeEdit->setReadOnly(true);
    toTimeEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    toTimeEdit->setMinimumWidth(60);
    toTimeEdit->setMaximumWidth(70);
    toTimeEdit->setMinimumHeight(30);
    toTimeEdit->setMaximumHeight(30);
    toTimeEdit->setStyleSheet("QTimeEdit { " + readOnlyDateTimeStyle + " min-height: 30px; max-height: 30px; padding: 4px 8px; }");
    dateTimeLayout->addWidget(toTimeEdit);

    // Duration label - màu vàng
    durationLabel = new QLabel("0h30m", formPanel);
    durationLabel->setStyleSheet(
        "color: #f59e0b; "
        "font-weight: bold; "
        "font-size: 12px; "
        "padding: 4px 8px; "
        "background-color: #fef3c7; "
        "border-radius: 4px;");
    dateTimeLayout->addWidget(durationLabel);
    dateTimeLayout->addStretch();

    QWidget *dateTimeWidget = new QWidget(formPanel);
    dateTimeWidget->setLayout(dateTimeLayout);
    dateTimeWidget->setStyleSheet("background-color: transparent;"); // Bỏ nền xám
    gridLayout->addWidget(dateTimeWidget, row, 1);                   // Cột 1, dưới Giá sân

    // Cọc label (bên phải cột 2)
    QLabel *cocLabel = new QLabel("Cọc:", formPanel);
    cocLabel->setStyleSheet(labelStyle);
    cocLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    gridLayout->addWidget(cocLabel, row, 3); // Col 3

    // Deposit amount label - màu xanh lá
    depositLabel = new QLabel("0 VND", formPanel);
    depositLabel->setMinimumWidth(200);
    depositLabel->setMaximumWidth(200);
    depositLabel->setStyleSheet(
        "color: #16a34a; "
        "font-weight: bold; "
        "padding: 6px 10px; "
        "border: 1px solid #d1fae5; "
        "border-radius: 4px; "
        "background-color: #f0fdf4; "
        "font-size: 13px; "
        "min-height: 36px;");
    gridLayout->addWidget(depositLabel, row, 4); // Col 4
    row++;

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
            nameLabel->setText("-");
            phoneEdit->setProperty("hasCustomer", false);
            return;
        }
        
        QuanLyKhachHang *qlkh = system->layQuanLyKhachHang();
        if (qlkh) {
            KhachHang *customer = qlkh->timKhachHangTheoSDT(phone.toStdString());
            if (customer) {
                nameLabel->setText(QString::fromStdString(customer->layHoTen()));
                phoneEdit->setProperty("hasCustomer", true);
            } else {
                nameLabel->setText("(Không tìm thấy)");
                phoneEdit->setProperty("hasCustomer", false);
            }
        } });

    // Buttons layout - vertical on right side
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->setSpacing(10);
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    QString btnStyle = "QPushButton { "
                       "background-color: %1; "
                       "color: white; "
                       "border: none; "
                       "border-radius: 4px; "
                       "padding: 8px 10px; " // Giảm padding
                       "font-weight: bold; "
                       "font-size: 13px; "
                       "min-width: 60px; " // Thu nhỏ từ 70px xuống 60px
                       "min-height: 36px; "
                       "max-height: 36px; "
                       "} "
                       "QPushButton:hover { background-color: %2; } "
                       "QPushButton:disabled { background-color: #9ca3af; }";

    saveBtn = new QPushButton("✓ Lưu", formPanel);
    saveBtn->setStyleSheet(btnStyle.arg("#16a34a").arg("#15803d"));
    buttonLayout->addWidget(saveBtn);

    deleteBtn = new QPushButton("✗ Hủy", formPanel);                  // Changed label to ✗ Hủy
    deleteBtn->setStyleSheet(btnStyle.arg("#ef4444").arg("#dc2626")); // Already red
    deleteBtn->setEnabled(true);                                      // Always enabled for canceling selection
    buttonLayout->addWidget(deleteBtn);

    // Removed: checkin button (no longer needed)

    buttonLayout->addStretch();

    // Add form grid and buttons to main content layout
    mainContentLayout->addLayout(gridLayout, 7);   // 70% for form
    mainContentLayout->addLayout(buttonLayout, 3); // 30% for buttons

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
    QComboBox *customerCombo = formPanel->findChild<QComboBox *>("customerCombo");
    if (customerCombo)
    {
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

    // Note: deleteBtn (Cancel button) stays enabled for clearing selection
    // Removed: checkinBtn (no longer exists)

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
    // Removed: checkinBtn (no longer exists)

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

            // T\u00ednh c\u1ecdc 30% ti\u1ec1n s\u00e2n
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
        nameLabel->setText("-");
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
        nameLabel->setText(QString::fromStdString(customer->layHoTen()));
        phoneEdit->setProperty("hasCustomer", true);
    }
    else
    {
        // Not found - show message, user can continue
        nameLabel->setText("(Khách hàng mới)");
        phoneEdit->setProperty("hasCustomer", false);
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

        // Update duration and price
        updateDuration();
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
        if (quanLyKH)
        {
            const MangDong<KhachHang *> &customers = quanLyKH->layDanhSachKhachHang();
            qDebug() << "Total customers:" << customers.size();
            for (int i = 0; i < qMin(5, customers.size()); i++)
            {
                if (customers[i])
                {
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
                // ===== TỰ ĐỘNG TÍNH TIỀN CỌC (30%) =====
                newBooking->tinhTienCoc();

                // ===== SAVE DATA TO FILE =====
                try
                {
                    system->luuHeThong("D:/QT_PBL2/Data/data.bin");
                }
                catch (...)
                {
                    qDebug() << "Warning: Could not save to data.bin";
                }

                // ===== REFRESH TIMELINE TRƯỚC =====
                timelineGrid->clearPendingSelection();
                timelineGrid->setDate(selectedDate);
                timelineGrid->loadBookings();

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
    // Clear pending selection in timeline grid
    if (timelineGrid)
    {
        timelineGrid->clearPendingSelection();
        // UNLOCK timeline grid to allow new selection
        timelineGrid->setProperty("isLocked", false);
    }

    // Clear form
    clearForm();

    QMessageBox::information(this, "✓ Đã hủy", "Đã xóa vùng chọn. Bạn có thể chọn lại.");
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
                        system->luuHeThong("D:/QT_PBL2/Data/data.bin");
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
                HeThongQuanLy::getInstance()->luuHeThong("D:/QT_PBL2/Data/data.bin");
                
                QMessageBox::information(dialog, "Thành công", "Đã thêm khách hàng!");
                
                // Update form
                phoneEdit->setText(phone);
                nameLabel->setText(name);
                
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

void TimelineTab::setupFilters()
{
    // Bỏ bộ lọc - không còn sử dụng
}

bool TimelineTab::checkBookingConflict(San *san, const NgayGio &ngayGio, const KhungGio &khungGio)
{
    QuanLyDatSan *quanLyDS = system->layQuanLyDatSan();
    if (!quanLyDS)
        return false;

    // Check if time slot is already booked
    const MangDong<DatSan *> &allBookings = quanLyDS->layDanhSachDatSan();

    for (int i = 0; i < allBookings.size(); i++)
    {
        DatSan *existingBooking = allBookings[i];

        // Skip if different field
        if (existingBooking->getSan() != san)
            continue;

        // Skip if different date
        NgayGio existingNgayGio = existingBooking->getThoiGianDat();
        if (existingNgayGio.getNgay() != ngayGio.getNgay() ||
            existingNgayGio.getThang() != ngayGio.getThang() ||
            existingNgayGio.getNam() != ngayGio.getNam())
        {
            continue;
        }

        // Skip current booking in edit mode
        if (isEditMode && existingBooking == currentBooking)
            continue;

        // Check time overlap
        KhungGio existingKhungGio = existingBooking->getKhungGio();

        int newStart = khungGio.getGioBatDau().getGio() * 60 + khungGio.getGioBatDau().getPhut();
        int newEnd = khungGio.getGioKetThuc().getGio() * 60 + khungGio.getGioKetThuc().getPhut();
        int existingStart = existingKhungGio.getGioBatDau().getGio() * 60 + existingKhungGio.getGioBatDau().getPhut();
        int existingEnd = existingKhungGio.getGioKetThuc().getGio() * 60 + existingKhungGio.getGioKetThuc().getPhut();

        // Check overlap
        if (!(newEnd <= existingStart || newStart >= existingEnd))
        {
            return true; // Conflict found
        }
    }

    return false; // No conflict
}

void TimelineTab::updateCalendarDates()
{
    if (!calendar)
        return;

    // Clear all date formats first
    QTextCharFormat defaultFormat;
    defaultFormat.setForeground(Qt::black);
    calendar->setDateTextFormat(QDate(), defaultFormat);

    // Get all bookings
    QuanLyDatSan *quanLyDS = system->layQuanLyDatSan();
    if (!quanLyDS)
        return;

    const MangDong<DatSan *> &allBookings = quanLyDS->layDanhSachDatSan();

    // Create format for dates with bookings
    QTextCharFormat bookedFormat;
    bookedFormat.setBackground(QColor("#dcfce7")); // Light green
    bookedFormat.setForeground(QColor("#16a34a")); // Dark green
    bookedFormat.setFontWeight(QFont::Bold);

    // Highlight dates with bookings
    QSet<QDate> bookedDates;
    for (int i = 0; i < allBookings.size(); i++)
    {
        DatSan *booking = allBookings[i];
        if (booking)
        {
            NgayGio ngayGio = booking->getThoiGianDat();
            QDate bookingDate(ngayGio.getNam(), ngayGio.getThang(), ngayGio.getNgay());
            bookedDates.insert(bookingDate);
        }
    }

    // Apply format to all booked dates
    for (const QDate &date : bookedDates)
    {
        calendar->setDateTextFormat(date, bookedFormat);
    }

    // Highlight today with different color
    QTextCharFormat todayFormat;
    todayFormat.setBackground(QColor("#dbeafe")); // Light blue
    todayFormat.setForeground(QColor("#1e40af")); // Dark blue
    todayFormat.setFontWeight(QFont::Bold);
    calendar->setDateTextFormat(QDate::currentDate(), todayFormat);
}
