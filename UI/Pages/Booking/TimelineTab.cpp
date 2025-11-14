/**
 * @file TimelineTab.cpp
 * @brief Implementation of TimelineTab
 */

#include "TimelineTab.h"
#include "TimelineGridWidget.h"
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QDateTime>
#include <QScrollArea>

TimelineTab::TimelineTab(QWidget *parent)
    : QWidget(parent), mainLayout(nullptr), verticalSplitter(nullptr), topPanel(nullptr), topPanelLayout(nullptr), calendarPanel(nullptr), calendarLayout(nullptr), calendar(nullptr), formPanel(nullptr), formLayout(nullptr), phoneEdit(nullptr), nameLabel(nullptr), fieldCombo(nullptr), priceLabel(nullptr), statusCombo(nullptr), typeCombo(nullptr), dateEdit(nullptr), fromTimeEdit(nullptr), toTimeEdit(nullptr), durationLabel(nullptr), noteEdit(nullptr), saveBtn(nullptr), deleteBtn(nullptr), checkinBtn(nullptr), timelinePanel(nullptr), timelineLayout(nullptr), timelineGrid(nullptr), system(nullptr), currentBooking(nullptr), isEditMode(false)
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

    // Phone search
    QLabel *phoneLabel = new QLabel("Phone:", formPanel);
    phoneLabel->setStyleSheet("font-size: 11px; font-weight: bold; color: #1f2937;");
    leftColumnLayout->addWidget(phoneLabel);

    phoneEdit = new QLineEdit(formPanel);
    phoneEdit->setPlaceholderText("Nhập SĐT");
    phoneEdit->setStyleSheet(
        "QLineEdit { "
        "padding: 6px; "
        "border: 1px solid #d1d5db; "
        "border-radius: 4px; "
        "background-color: white; "
        "font-size: 12px; "
        "} "
        "QLineEdit:focus { "
        "border: 1px solid #16a34a; "
        "}");
    leftColumnLayout->addWidget(phoneEdit);

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

    saveBtn = new QPushButton("💾 Lưu", formPanel);
    saveBtn->setMinimumWidth(80);
    saveBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #16a34a; "
        "color: white; "
        "border: none; "
        "border-radius: 4px; "
        "padding: 8px 16px; "
        "font-weight: bold; "
        "font-size: 11px; "
        "} "
        "QPushButton:hover { "
        "background-color: #15803d; "
        "} "
        "QPushButton:disabled { "
        "background-color: #9ca3af; "
        "}");

    deleteBtn = new QPushButton("🗑️ Hủy", formPanel);
    deleteBtn->setMinimumWidth(80);
    deleteBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #ef4444; "
        "color: white; "
        "border: none; "
        "border-radius: 4px; "
        "padding: 8px 16px; "
        "font-weight: bold; "
        "font-size: 11px; "
        "} "
        "QPushButton:hover { "
        "background-color: #dc2626; "
        "} "
        "QPushButton:disabled { "
        "background-color: #9ca3af; "
        "}");

    checkinBtn = new QPushButton("✓ Check-in", formPanel);
    checkinBtn->setMinimumWidth(80);
    checkinBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #3b82f6; "
        "color: white; "
        "border: none; "
        "border-radius: 4px; "
        "padding: 8px 16px; "
        "font-weight: bold; "
        "font-size: 11px; "
        "} "
        "QPushButton:hover { "
        "background-color: #2563eb; "
        "} "
        "QPushButton:disabled { "
        "background-color: #9ca3af; "
        "}");

    buttonLayout->addWidget(saveBtn);
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
    connect(deleteBtn, &QPushButton::clicked, this, &TimelineTab::onDeleteClicked);
    connect(checkinBtn, &QPushButton::clicked, this, &TimelineTab::onCheckinClicked);

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
}

void TimelineTab::refreshData()
{
    // Reload bookings for selected date
    timelineGrid->setDate(selectedDate);
    timelineGrid->loadBookings();
}

void TimelineTab::clearForm()
{
    phoneEdit->clear();
    nameLabel->setText("-");
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
        phoneEdit->setText(QString::fromStdString(customer->laySoDienThoai()));
        nameLabel->setText(QString::fromStdString(customer->layHoTen()));
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

    KhachHang *customer = quanLyKH->timKhachHang(phone.toStdString());
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
    // Validate form
    QString phone = phoneEdit->text().trimmed();
    if (phone.isEmpty())
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập số điện thoại khách hàng!");
        return;
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
    KhachHang *customer = quanLyKH ? quanLyKH->timKhachHang(phone.toStdString()) : nullptr;

    if (!customer)
    {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy khách hàng. Vui lòng tìm kiếm trước!");
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
                QMessageBox::information(this, "Thành công", "Đặt sân thành công!");
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
                    QMessageBox::information(this, "Thành công", "Đã hủy đặt sân!");
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
    // Clear current form and prepare for new booking
    clearForm();

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

    // Focus phone field for customer search
    phoneEdit->setFocus();
}

void TimelineTab::onBookingBlockClicked(DatSan *booking)
{
    populateForm(booking);
}
