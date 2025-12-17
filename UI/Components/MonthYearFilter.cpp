#include "MonthYearFilter.h"
#include <QGraphicsDropShadowEffect>

MonthYearFilter::MonthYearFilter(QWidget *parent, bool showExportBtn)
    : QWidget(parent),
      m_layout(nullptr),
      m_filterModeGroup(nullptr),
      m_monthBtn(nullptr),
      m_yearBtn(nullptr),
      m_separator(nullptr),
      m_selectorWidget(nullptr),
      m_selectorLayout(nullptr),
      m_monthCombo(nullptr),
      m_yearCombo(nullptr),
      m_exportPdfBtn(nullptr),
      m_showExportBtn(showExportBtn),
      m_filterMode(ByMonth),
      m_selectedMonth(QDate::currentDate().month()),
      m_selectedYear(QDate::currentDate().year())
{
    setupUI();
}

MonthYearFilter::~MonthYearFilter()
{
}

// ========== PUBLIC METHODS ==========

void MonthYearFilter::setFilterMode(FilterMode mode)
{
    if (m_filterMode != mode)
    {
        m_filterMode = mode;
        if (mode == ByMonth)
            m_monthBtn->setChecked(true);
        else
            m_yearBtn->setChecked(true);
        updateFilterVisibility();
        emitFilterChanged();
    }
}

void MonthYearFilter::setMonth(int month)
{
    if (month >= 1 && month <= 12 && m_selectedMonth != month)
    {
        m_selectedMonth = month;
        m_monthCombo->setCurrentIndex(month - 1);
        emitFilterChanged();
    }
}

void MonthYearFilter::setYear(int year)
{
    if (m_selectedYear != year)
    {
        m_selectedYear = year;
        // Find year in combo
        for (int i = 0; i < m_yearCombo->count(); i++)
        {
            if (m_yearCombo->itemData(i).toInt() == year)
            {
                m_yearCombo->setCurrentIndex(i);
                break;
            }
        }
        emitFilterChanged();
    }
}

void MonthYearFilter::setShowExportButton(bool show)
{
    m_showExportBtn = show;
    if (m_exportPdfBtn)
        m_exportPdfBtn->setVisible(show);
}

QDate MonthYearFilter::getFromDate() const
{
    if (m_filterMode == ByMonth)
    {
        return QDate(m_selectedYear, m_selectedMonth, 1);
    }
    else
    {
        return QDate(m_selectedYear, 1, 1);
    }
}

QDate MonthYearFilter::getToDate() const
{
    if (m_filterMode == ByMonth)
    {
        QDate firstDay(m_selectedYear, m_selectedMonth, 1);
        return QDate(m_selectedYear, m_selectedMonth, firstDay.daysInMonth());
    }
    else
    {
        return QDate(m_selectedYear, 12, 31);
    }
}

// ========== PRIVATE SLOTS ==========

void MonthYearFilter::onFilterModeChanged(int mode)
{
    m_filterMode = static_cast<FilterMode>(mode);
    updateFilterVisibility();
    emitFilterChanged();
}

void MonthYearFilter::onMonthChanged(int index)
{
    m_selectedMonth = index + 1;
    emitFilterChanged();
}

void MonthYearFilter::onYearChanged(int index)
{
    m_selectedYear = m_yearCombo->itemData(index).toInt();
    emitFilterChanged();
}

// ========== PRIVATE METHODS ==========

void MonthYearFilter::setupUI()
{
    // Outer layout (no margins, this widget is transparent)
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);

    // Container widget with styling (this gets the shadow and background)
    QWidget *container = new QWidget(this);
    container->setObjectName("filterContainer");
    container->setStyleSheet(R"(
        QWidget#filterContainer {
            background-color: white;
            border-radius: 12px;
            border: 1px solid #e2e8f0;
        }
    )");

    // Shadow effect on container
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(container);
    shadow->setBlurRadius(10);
    shadow->setXOffset(0);
    shadow->setYOffset(2);
    shadow->setColor(QColor(0, 0, 0, 20));
    container->setGraphicsEffect(shadow);

    // Layout inside container
    m_layout = new QHBoxLayout(container);
    m_layout->setContentsMargins(16, 12, 16, 12);
    m_layout->setSpacing(16);

    // Filter mode buttons
    m_filterModeGroup = new QButtonGroup(this);

    QString btnStyle = R"(
        QPushButton {
            background-color: #f1f5f9;
            color: #475569;
            border: none;
            border-radius: 8px;
            padding: 10px 24px;
            font-size: 14px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #e2e8f0;
        }
        QPushButton:checked {
            background-color: #3b82f6;
            color: white;
        }
    )";

    m_monthBtn = new QPushButton("Theo tháng", this);
    m_monthBtn->setCheckable(true);
    m_monthBtn->setChecked(true);
    m_monthBtn->setStyleSheet(btnStyle);
    m_monthBtn->setCursor(Qt::PointingHandCursor);

    m_yearBtn = new QPushButton("Theo năm", this);
    m_yearBtn->setCheckable(true);
    m_yearBtn->setStyleSheet(btnStyle);
    m_yearBtn->setCursor(Qt::PointingHandCursor);

    m_filterModeGroup->addButton(m_monthBtn, ByMonth);
    m_filterModeGroup->addButton(m_yearBtn, ByYear);
    m_filterModeGroup->setExclusive(true);

    connect(m_filterModeGroup, &QButtonGroup::idClicked,
            this, &MonthYearFilter::onFilterModeChanged);

    m_layout->addWidget(m_monthBtn);
    m_layout->addWidget(m_yearBtn);

    // Separator
    m_separator = new QFrame(this);
    m_separator->setFrameShape(QFrame::VLine);
    m_separator->setStyleSheet("QFrame { color: #e2e8f0; }");
    m_layout->addWidget(m_separator);

    // Selector widget (month/year pickers)
    m_selectorWidget = new QWidget(this);
    m_selectorWidget->setStyleSheet("background: transparent;");
    m_selectorLayout = new QHBoxLayout(m_selectorWidget);
    m_selectorLayout->setContentsMargins(0, 0, 0, 0);
    m_selectorLayout->setSpacing(8);

    QString comboStyle = R"(
        QComboBox {
            background-color: white;
            border: 1px solid #e2e8f0;
            border-radius: 8px;
            padding: 8px 16px;
            font-size: 14px;
            color: #1e293b;
            min-width: 100px;
        }
        QComboBox:hover {
            border-color: #3b82f6;
        }
        QComboBox::drop-down {
            border: none;
            width: 24px;
        }
        QComboBox QAbstractItemView {
            background-color: white;
            border: 1px solid #e2e8f0;
            border-radius: 8px;
            selection-background-color: #3b82f6;
        }
    )";

    // Month combo
    m_monthCombo = new QComboBox(this);
    m_monthCombo->setStyleSheet(comboStyle);
    populateMonthCombo();
    m_monthCombo->setCurrentIndex(m_selectedMonth - 1);
    connect(m_monthCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MonthYearFilter::onMonthChanged);

    // Year combo
    m_yearCombo = new QComboBox(this);
    m_yearCombo->setStyleSheet(comboStyle);
    populateYearCombo();
    connect(m_yearCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MonthYearFilter::onYearChanged);

    m_selectorLayout->addWidget(m_monthCombo);
    m_selectorLayout->addWidget(m_yearCombo);

    m_layout->addWidget(m_selectorWidget);
    m_layout->addStretch();

    // Export PDF button (optional)
    QString exportBtnStyle = R"(
        QPushButton {
            background-color: #3b82f6;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 10px 16px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #2563eb;
        }
    )";

    m_exportPdfBtn = new QPushButton("📥 Xuất PDF", this);
    m_exportPdfBtn->setStyleSheet(exportBtnStyle);
    m_exportPdfBtn->setCursor(Qt::PointingHandCursor);
    m_exportPdfBtn->setVisible(m_showExportBtn);
    connect(m_exportPdfBtn, &QPushButton::clicked, this, &MonthYearFilter::exportPdfClicked);

    m_layout->addWidget(m_exportPdfBtn);

    // Add container to outer layout
    outerLayout->addWidget(container);

    // Set initial visibility
    updateFilterVisibility();
}

void MonthYearFilter::updateFilterVisibility()
{
    // Month combo only visible in ByMonth mode
    m_monthCombo->setVisible(m_filterMode == ByMonth);
}

void MonthYearFilter::populateMonthCombo()
{
    m_monthCombo->clear();
    QStringList months = {"Tháng 1", "Tháng 2", "Tháng 3", "Tháng 4",
                          "Tháng 5", "Tháng 6", "Tháng 7", "Tháng 8",
                          "Tháng 9", "Tháng 10", "Tháng 11", "Tháng 12"};
    m_monthCombo->addItems(months);
}

void MonthYearFilter::populateYearCombo()
{
    m_yearCombo->clear();
    int currentYear = QDate::currentDate().year();
    for (int year = currentYear; year >= currentYear - 5; year--)
    {
        m_yearCombo->addItem(QString::number(year), year);
    }
}

void MonthYearFilter::emitFilterChanged()
{
    emit filterChanged(m_selectedMonth, m_selectedYear, m_filterMode);
}
