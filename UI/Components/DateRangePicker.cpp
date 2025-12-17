#include "DateRangePicker.h"
#include <QDate>
#include <QButtonGroup>

// ========== CONSTRUCTORS ==========

DateRangePicker::DateRangePicker(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(nullptr),
      m_presetWidget(nullptr),
      m_presetLayout(nullptr),
      m_todayBtn(nullptr),
      m_weekBtn(nullptr),
      m_monthBtn(nullptr),
      m_quarterBtn(nullptr),
      m_yearBtn(nullptr),
      m_customBtn(nullptr),
      m_dateWidget(nullptr),
      m_dateLayout(nullptr),
      m_fromLabel(nullptr),
      m_fromDateEdit(nullptr),
      m_toLabel(nullptr),
      m_toDateEdit(nullptr),
      m_presetCombo(nullptr),
      m_dateRangeBtn(nullptr),
      m_dateMenu(nullptr),
      m_currentPreset(Today),
      m_showPresetButtons(true),
      m_compactMode(false),
      m_shadowEffect(nullptr)
{
    // Default to today
    auto dates = getPresetDates(Today);
    m_fromDate = dates.first;
    m_toDate = dates.second;

    setupUI();
    applyStyles();
}

DateRangePicker::~DateRangePicker()
{
}

// ========== SETUP UI ==========

void DateRangePicker::setupUI()
{
    if (m_compactMode)
    {
        setupCompactUI();
    }
    else
    {
        setupFullUI();
    }
}

void DateRangePicker::setupFullUI()
{
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(12, 8, 12, 8);
    m_mainLayout->setSpacing(12);

    // Preset buttons
    m_presetWidget = new QWidget(this);
    m_presetLayout = new QHBoxLayout(m_presetWidget);
    m_presetLayout->setContentsMargins(0, 0, 0, 0);
    m_presetLayout->setSpacing(4);

    auto createPresetBtn = [this](const QString &text, PresetRange preset) -> QPushButton *
    {
        QPushButton *btn = new QPushButton(text, this);
        btn->setProperty("preset", static_cast<int>(preset));
        btn->setCheckable(true);
        btn->setCursor(Qt::PointingHandCursor);
        connect(btn, &QPushButton::clicked, this, &DateRangePicker::onPresetClicked);
        return btn;
    };

    m_todayBtn = createPresetBtn("Ngày", Today);
    // m_weekBtn = createPresetBtn("Tuần này", ThisWeek);
    m_monthBtn = createPresetBtn("Tháng", ThisMonth);
    // m_quarterBtn = createPresetBtn("Quý này", ThisQuarter);
    m_yearBtn = createPresetBtn("Năm", ThisYear);
    // m_customBtn = createPresetBtn("Tùy chọn", Custom);

    // Set default checked
    m_todayBtn->setChecked(true);

    m_presetLayout->addWidget(m_todayBtn);
    // m_presetLayout->addWidget(m_weekBtn);
    m_presetLayout->addWidget(m_monthBtn);
    // m_presetLayout->addWidget(m_quarterBtn);
    m_presetLayout->addWidget(m_yearBtn);
    // m_presetLayout->addWidget(m_customBtn);

    // Date selection widget
    m_dateWidget = new QWidget(this);
    m_dateLayout = new QHBoxLayout(m_dateWidget);
    m_dateLayout->setContentsMargins(0, 0, 0, 0);
    m_dateLayout->setSpacing(8);

    m_fromLabel = new QLabel("Thời gian:", this);
    m_fromDateEdit = new QDateEdit(m_fromDate, this);
    m_fromDateEdit->setCalendarPopup(true);
    m_fromDateEdit->setDisplayFormat("dd/MM/yyyy");
    m_fromDateEdit->setMaximumDate(QDate::currentDate());

    // Hide To Date components
    m_toLabel = new QLabel("Đến:", this);
    m_toDateEdit = new QDateEdit(m_toDate, this);
    m_toLabel->setVisible(false);
    m_toDateEdit->setVisible(false);

    connect(m_fromDateEdit, &QDateEdit::dateChanged, this, &DateRangePicker::onFromDateChanged);
    // connect(m_toDateEdit, &QDateEdit::dateChanged, this, &DateRangePicker::onToDateChanged);

    m_dateLayout->addWidget(m_fromLabel);
    m_dateLayout->addWidget(m_fromDateEdit);
    // m_dateLayout->addWidget(m_toLabel);
    // m_dateLayout->addWidget(m_toDateEdit);

    // Add to main layout
    m_mainLayout->addWidget(m_presetWidget);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_dateWidget);

    // Shadow effect
    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setBlurRadius(10);
    m_shadowEffect->setXOffset(0);
    m_shadowEffect->setYOffset(2);
    m_shadowEffect->setColor(QColor(0, 0, 0, 20));
    setGraphicsEffect(m_shadowEffect);
}

void DateRangePicker::setupCompactUI()
{
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(8);

    // Preset combo
    m_presetCombo = new QComboBox(this);
    m_presetCombo->addItem("Hôm nay", Today);
    m_presetCombo->addItem("Hôm qua", Yesterday);
    m_presetCombo->addItem("7 ngày qua", Last7Days);
    m_presetCombo->addItem("30 ngày qua", Last30Days);
    m_presetCombo->addItem("Tuần này", ThisWeek);
    m_presetCombo->addItem("Tháng này", ThisMonth);
    m_presetCombo->addItem("Quý này", ThisQuarter);
    m_presetCombo->addItem("Năm nay", ThisYear);
    m_presetCombo->setCurrentIndex(5); // This Month

    connect(m_presetCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DateRangePicker::onPresetComboChanged);

    // Date range button showing current range
    m_dateRangeBtn = new QPushButton(this);
    m_dateRangeBtn->setText(QString("%1 - %2")
                                .arg(m_fromDate.toString("dd/MM/yyyy"))
                                .arg(m_toDate.toString("dd/MM/yyyy")));

    m_mainLayout->addWidget(m_presetCombo);
    m_mainLayout->addWidget(m_dateRangeBtn);
}

void DateRangePicker::applyStyles()
{
    setStyleSheet(R"(
        DateRangePicker {
            background-color: white;
            border-radius: 8px;
        }
    )");

    QString presetBtnStyle = R"(
        QPushButton {
            background-color: #f1f5f9;
            color: #475569;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-size: 13px;
            font-weight: 500;
        }
        QPushButton:hover {
            background-color: #e2e8f0;
        }
        QPushButton:checked {
            background-color: #3b82f6;
            color: white;
        }
    )";

    if (m_todayBtn)
        m_todayBtn->setStyleSheet(presetBtnStyle);
    if (m_weekBtn)
        m_weekBtn->setStyleSheet(presetBtnStyle);
    if (m_monthBtn)
        m_monthBtn->setStyleSheet(presetBtnStyle);
    if (m_quarterBtn)
        m_quarterBtn->setStyleSheet(presetBtnStyle);
    if (m_yearBtn)
        m_yearBtn->setStyleSheet(presetBtnStyle);
    if (m_customBtn)
        m_customBtn->setStyleSheet(presetBtnStyle);

    QString dateEditStyle = R"(
        QDateEdit {
            background-color: #f8fafc;
            border: 1px solid #e2e8f0;
            border-radius: 6px;
            padding: 6px 12px;
            font-size: 13px;
            color: #1e293b;
            min-width: 110px;
        }
        QDateEdit:focus {
            border-color: #3b82f6;
        }
        QDateEdit::drop-down {
            border: none;
            width: 24px;
        }
        QDateEdit::down-arrow {
            image: url(:/icons/calendar.svg);
            width: 14px;
            height: 14px;
        }
        QCalendarWidget {
            background-color: white;
        }
    )";

    if (m_fromDateEdit)
        m_fromDateEdit->setStyleSheet(dateEditStyle);
    if (m_toDateEdit)
        m_toDateEdit->setStyleSheet(dateEditStyle);

    QString labelStyle = R"(
        QLabel {
            color: #64748b;
            font-size: 13px;
        }
    )";

    if (m_fromLabel)
        m_fromLabel->setStyleSheet(labelStyle);
    if (m_toLabel)
        m_toLabel->setStyleSheet(labelStyle);

    // Compact mode styles
    if (m_presetCombo)
    {
        m_presetCombo->setStyleSheet(R"(
            QComboBox {
                background-color: #f8fafc;
                border: 1px solid #e2e8f0;
                border-radius: 6px;
                padding: 8px 12px;
                font-size: 13px;
                color: #1e293b;
                min-width: 120px;
            }
            QComboBox:hover {
                border-color: #3b82f6;
            }
            QComboBox::drop-down {
                border: none;
                width: 24px;
            }
        )");
    }
}

// ========== SETTERS ==========

void DateRangePicker::setDateRange(const QDate &from, const QDate &to)
{
    m_fromDate = from;
    m_toDate = to;

    if (m_fromDateEdit)
    {
        m_fromDateEdit->blockSignals(true);
        m_fromDateEdit->setDate(from);
        m_fromDateEdit->blockSignals(false);
    }

    if (m_toDateEdit)
    {
        m_toDateEdit->blockSignals(true);
        m_toDateEdit->setDate(to);
        m_toDateEdit->blockSignals(false);
    }

    updatePresetFromDates();
    emit dateRangeChanged(m_fromDate, m_toDate);
}

void DateRangePicker::setPreset(PresetRange preset)
{
    applyPreset(preset);
}

void DateRangePicker::setMinDate(const QDate &date)
{
    if (m_fromDateEdit)
        m_fromDateEdit->setMinimumDate(date);
    if (m_toDateEdit)
        m_toDateEdit->setMinimumDate(date);
}

void DateRangePicker::setMaxDate(const QDate &date)
{
    if (m_fromDateEdit)
        m_fromDateEdit->setMaximumDate(date);
    if (m_toDateEdit)
        m_toDateEdit->setMaximumDate(date);
}

void DateRangePicker::setShowPresetButtons(bool show)
{
    m_showPresetButtons = show;
    if (m_presetWidget)
    {
        m_presetWidget->setVisible(show);
    }
}

void DateRangePicker::setCompactMode(bool compact)
{
    if (m_compactMode != compact)
    {
        m_compactMode = compact;
        // Clear existing layout
        QLayoutItem *item;
        while ((item = m_mainLayout->takeAt(0)) != nullptr)
        {
            delete item->widget();
            delete item;
        }
        setupUI();
        applyStyles();
    }
}

// ========== GETTERS ==========

QDate DateRangePicker::fromDate() const { return m_fromDate; }
QDate DateRangePicker::toDate() const { return m_toDate; }
DateRangePicker::PresetRange DateRangePicker::currentPreset() const { return m_currentPreset; }

int DateRangePicker::dayCount() const
{
    return m_fromDate.daysTo(m_toDate) + 1;
}

// ========== UTILITY ==========

QString DateRangePicker::presetToString(PresetRange preset)
{
    switch (preset)
    {
    case Today:
        return "Hôm nay";
    case Yesterday:
        return "Hôm qua";
    case ThisWeek:
        return "Tuần này";
    case LastWeek:
        return "Tuần trước";
    case ThisMonth:
        return "Tháng này";
    case LastMonth:
        return "Tháng trước";
    case ThisQuarter:
        return "Quý này";
    case ThisYear:
        return "Năm nay";
    case Last7Days:
        return "7 ngày qua";
    case Last30Days:
        return "30 ngày qua";
    case Last90Days:
        return "90 ngày qua";
    case Custom:
        return "Tùy chọn";
    default:
        return "";
    }
}

QPair<QDate, QDate> DateRangePicker::getPresetDates(PresetRange preset)
{
    QDate today = QDate::currentDate();
    QDate from, to;

    switch (preset)
    {
    case Today:
        from = to = today;
        break;
    case Yesterday:
        from = to = today.addDays(-1);
        break;
    case ThisWeek:
        from = today.addDays(-(today.dayOfWeek() - 1));
        to = today;
        break;
    case LastWeek:
        from = today.addDays(-(today.dayOfWeek() + 6));
        to = today.addDays(-today.dayOfWeek());
        break;
    case ThisMonth:
        from = QDate(today.year(), today.month(), 1);
        to = today;
        break;
    case LastMonth:
        from = QDate(today.year(), today.month(), 1).addMonths(-1);
        to = QDate(today.year(), today.month(), 1).addDays(-1);
        break;
    case ThisQuarter:
    {
        int quarter = (today.month() - 1) / 3;
        from = QDate(today.year(), quarter * 3 + 1, 1);
        to = today;
        break;
    }
    case ThisYear:
        from = QDate(today.year(), 1, 1);
        to = today;
        break;
    case Last7Days:
        from = today.addDays(-6);
        to = today;
        break;
    case Last30Days:
        from = today.addDays(-29);
        to = today;
        break;
    case Last90Days:
        from = today.addDays(-89);
        to = today;
        break;
    case Custom:
    default:
        from = QDate(today.year(), today.month(), 1);
        to = today;
        break;
    }

    return qMakePair(from, to);
}

// ========== SLOTS ==========

void DateRangePicker::onFromDateChanged(const QDate &date)
{
    if (date > m_toDate)
    {
        m_toDateEdit->setDate(date);
    }
    m_fromDate = date;
    m_currentPreset = Custom;
    updatePresetFromDates();
    emit dateRangeChanged(m_fromDate, m_toDate);
}

void DateRangePicker::onToDateChanged(const QDate &date)
{
    if (date < m_fromDate)
    {
        m_fromDateEdit->setDate(date);
    }
    m_toDate = date;
    m_currentPreset = Custom;
    updatePresetFromDates();
    emit dateRangeChanged(m_fromDate, m_toDate);
}

void DateRangePicker::onPresetClicked()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (!btn)
        return;

    PresetRange preset = static_cast<PresetRange>(btn->property("preset").toInt());

    // Uncheck all buttons
    if (m_todayBtn)
        m_todayBtn->setChecked(false);
    if (m_weekBtn)
        m_weekBtn->setChecked(false);
    if (m_monthBtn)
        m_monthBtn->setChecked(false);
    if (m_quarterBtn)
        m_quarterBtn->setChecked(false);
    if (m_yearBtn)
        m_yearBtn->setChecked(false);
    if (m_customBtn)
        m_customBtn->setChecked(false);

    btn->setChecked(true);

    applyPreset(preset);
}

void DateRangePicker::onPresetComboChanged(int index)
{
    if (m_presetCombo)
    {
        PresetRange preset = static_cast<PresetRange>(m_presetCombo->itemData(index).toInt());
        applyPreset(preset);
    }
}

// ========== PRIVATE METHODS ==========

void DateRangePicker::updatePresetFromDates()
{
    // Check if current dates match any preset
    QList<PresetRange> presets = {Today, Yesterday, ThisWeek, LastWeek, ThisMonth,
                                  LastMonth, ThisQuarter, ThisYear, Last7Days, Last30Days};

    for (PresetRange preset : presets)
    {
        auto dates = getPresetDates(preset);
        if (dates.first == m_fromDate && dates.second == m_toDate)
        {
            m_currentPreset = preset;

            // Update button states
            if (m_todayBtn)
                m_todayBtn->setChecked(preset == Today);
            if (m_weekBtn)
                m_weekBtn->setChecked(preset == ThisWeek);
            if (m_monthBtn)
                m_monthBtn->setChecked(preset == ThisMonth);
            if (m_quarterBtn)
                m_quarterBtn->setChecked(preset == ThisQuarter);
            if (m_yearBtn)
                m_yearBtn->setChecked(preset == ThisYear);
            if (m_customBtn)
                m_customBtn->setChecked(false);

            return;
        }
    }

    // No preset matched
    m_currentPreset = Custom;
    if (m_customBtn)
        m_customBtn->setChecked(true);
}

void DateRangePicker::applyPreset(PresetRange preset)
{
    m_currentPreset = preset;

    // Update display format based on preset
    if (m_fromDateEdit)
    {
        m_fromDateEdit->blockSignals(true);
        if (preset == Today)
        {
            m_fromDateEdit->setDisplayFormat("dd/MM/yyyy");
            m_fromDateEdit->setDate(QDate::currentDate());
        }
        else if (preset == ThisMonth)
        {
            m_fromDateEdit->setDisplayFormat("MM/yyyy");
            m_fromDateEdit->setDate(QDate::currentDate());
        }
        else if (preset == ThisYear)
        {
            m_fromDateEdit->setDisplayFormat("yyyy");
            m_fromDateEdit->setDate(QDate::currentDate());
        }
        m_fromDateEdit->blockSignals(false);
    }

    // Calculate range
    QDate date = m_fromDateEdit ? m_fromDateEdit->date() : QDate::currentDate();
    if (preset == Today)
    {
        m_fromDate = date;
        m_toDate = date;
    }
    else if (preset == ThisMonth)
    {
        m_fromDate = QDate(date.year(), date.month(), 1);
        m_toDate = QDate(date.year(), date.month(), date.daysInMonth());
    }
    else if (preset == ThisYear)
    {
        m_fromDate = QDate(date.year(), 1, 1);
        m_toDate = QDate(date.year(), 12, 31);
    }

    emit presetChanged(preset);
    emit dateRangeChanged(m_fromDate, m_toDate);
}
