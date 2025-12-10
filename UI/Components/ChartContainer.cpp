#include "ChartContainer.h"
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

// ========== CONSTRUCTORS ==========

ChartContainer::ChartContainer(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(nullptr),
      m_headerLayout(nullptr),
      m_filterLayout(nullptr),
      m_titleLabel(nullptr),
      m_subtitleLabel(nullptr),
      m_filterWidget(nullptr),
      m_exportBtn(nullptr),
      m_refreshBtn(nullptr),
      m_chartView(nullptr),
      m_chart(nullptr),
      m_loadingOverlay(nullptr),
      m_loadingLabel(nullptr),
      m_errorWidget(nullptr),
      m_errorLabel(nullptr),
      m_isLoading(false),
      m_hasError(false),
      m_minChartHeight(300),
      m_shadowEffect(nullptr)
{
    setupUI();
    applyStyles();
}

ChartContainer::ChartContainer(const QString &title, QWidget *parent)
    : ChartContainer(parent)
{
    setTitle(title);
}

ChartContainer::~ChartContainer()
{
}

// ========== SETUP UI ==========

void ChartContainer::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(16, 16, 16, 16);
    m_mainLayout->setSpacing(12);

    // Header section (title + filters + buttons)
    m_headerLayout = new QHBoxLayout();
    m_headerLayout->setSpacing(12);

    // Title area
    QVBoxLayout *titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(4);

    m_titleLabel = new QLabel(this);
    m_titleLabel->setStyleSheet(R"(
        QLabel {
            color: #1e293b;
            font-size: 16px;
            font-weight: 600;
        }
    )");

    m_subtitleLabel = new QLabel(this);
    m_subtitleLabel->setStyleSheet(R"(
        QLabel {
            color: #64748b;
            font-size: 12px;
        }
    )");
    m_subtitleLabel->hide();

    titleLayout->addWidget(m_titleLabel);
    titleLayout->addWidget(m_subtitleLabel);

    // Filter area
    m_filterWidget = new QWidget(this);
    m_filterLayout = new QHBoxLayout(m_filterWidget);
    m_filterLayout->setContentsMargins(0, 0, 0, 0);
    m_filterLayout->setSpacing(8);

    // Action buttons
    m_refreshBtn = new QPushButton(this);
    m_refreshBtn->setText("↻");
    m_refreshBtn->setToolTip("Làm mới");
    m_refreshBtn->setFixedSize(32, 32);
    m_refreshBtn->setCursor(Qt::PointingHandCursor);
    m_refreshBtn->hide();
    connect(m_refreshBtn, &QPushButton::clicked, this, &ChartContainer::onRefreshClicked);

    m_exportBtn = new QPushButton(this);
    m_exportBtn->setText("⬇");
    m_exportBtn->setToolTip("Xuất");
    m_exportBtn->setFixedSize(32, 32);
    m_exportBtn->setCursor(Qt::PointingHandCursor);
    m_exportBtn->hide();
    connect(m_exportBtn, &QPushButton::clicked, this, &ChartContainer::onExportClicked);

    m_headerLayout->addLayout(titleLayout);
    m_headerLayout->addStretch();
    m_headerLayout->addWidget(m_filterWidget);
    m_headerLayout->addWidget(m_refreshBtn);
    m_headerLayout->addWidget(m_exportBtn);

    // Chart view
    m_chartView = new QChartView(this);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setMinimumHeight(m_minChartHeight);

    // Create default empty chart
    m_chart = new QChart();
    m_chart->setBackgroundVisible(false);
    m_chart->setMargins(QMargins(0, 0, 0, 0));
    m_chart->legend()->setAlignment(Qt::AlignBottom);
    m_chartView->setChart(m_chart);

    // Loading overlay
    m_loadingOverlay = new QWidget(this);
    m_loadingOverlay->setStyleSheet("background-color: rgba(255, 255, 255, 0.9);");
    m_loadingOverlay->hide();

    QVBoxLayout *loadingLayout = new QVBoxLayout(m_loadingOverlay);
    loadingLayout->setAlignment(Qt::AlignCenter);

    m_loadingLabel = new QLabel("Đang tải...", m_loadingOverlay);
    m_loadingLabel->setStyleSheet(R"(
        QLabel {
            color: #3b82f6;
            font-size: 14px;
            font-weight: 500;
        }
    )");
    m_loadingLabel->setAlignment(Qt::AlignCenter);
    loadingLayout->addWidget(m_loadingLabel);

    // Error widget
    m_errorWidget = new QWidget(this);
    m_errorWidget->setStyleSheet("background-color: #fef2f2; border-radius: 8px;");
    m_errorWidget->hide();

    QVBoxLayout *errorLayout = new QVBoxLayout(m_errorWidget);
    errorLayout->setAlignment(Qt::AlignCenter);

    m_errorLabel = new QLabel(m_errorWidget);
    m_errorLabel->setStyleSheet(R"(
        QLabel {
            color: #dc2626;
            font-size: 13px;
        }
    )");
    m_errorLabel->setAlignment(Qt::AlignCenter);
    errorLayout->addWidget(m_errorLabel);

    // Add to main layout
    m_mainLayout->addLayout(m_headerLayout);
    m_mainLayout->addWidget(m_chartView, 1);
    m_mainLayout->addWidget(m_errorWidget);

    // Shadow effect
    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setBlurRadius(15);
    m_shadowEffect->setXOffset(0);
    m_shadowEffect->setYOffset(2);
    m_shadowEffect->setColor(QColor(0, 0, 0, 25));
    setGraphicsEffect(m_shadowEffect);
}

void ChartContainer::applyStyles()
{
    setStyleSheet(R"(
        ChartContainer {
            background-color: white;
            border-radius: 12px;
        }
    )");

    // Ensure chart view has transparent background to show container's white background
    m_chartView->setBackgroundBrush(Qt::NoBrush);
    if (m_chart)
    {
        m_chart->setBackgroundBrush(Qt::NoBrush);
    }

    QString btnStyle = R"(
        QPushButton {
            background-color: #f1f5f9;
            color: #475569;
            border: none;
            border-radius: 6px;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #e2e8f0;
        }
        QPushButton:pressed {
            background-color: #cbd5e1;
        }
    )";

    m_refreshBtn->setStyleSheet(btnStyle);
    m_exportBtn->setStyleSheet(btnStyle);

    m_chartView->setStyleSheet(R"(
        QChartView {
            background-color: transparent;
            border: none;
        }
    )");
}

// ========== CHART MANAGEMENT ==========

void ChartContainer::setChart(QChart *chart)
{
    if (chart)
    {
        m_chart = chart;
        m_chart->setBackgroundVisible(false);
        m_chart->setAnimationOptions(QChart::SeriesAnimations);
        m_chartView->setChart(m_chart);
    }
}

QChart *ChartContainer::chart() const
{
    return m_chart;
}

QChartView *ChartContainer::chartView() const
{
    return m_chartView;
}

// ========== CONFIGURATION ==========

void ChartContainer::setTitle(const QString &title)
{
    m_title = title;
    m_titleLabel->setText(title);
}

void ChartContainer::setSubtitle(const QString &subtitle)
{
    m_subtitle = subtitle;
    m_subtitleLabel->setText(subtitle);
    m_subtitleLabel->setVisible(!subtitle.isEmpty());
}

void ChartContainer::addFilter(const QString &label, const QStringList &options)
{
    QComboBox *filter = new QComboBox(this);
    filter->addItems(options);
    filter->setProperty("filterIndex", m_filters.size());

    filter->setStyleSheet(R"(
        QComboBox {
            background-color: #f8fafc;
            border: 1px solid #e2e8f0;
            border-radius: 6px;
            padding: 6px 12px;
            font-size: 12px;
            color: #1e293b;
            min-width: 100px;
        }
        QComboBox:hover {
            border-color: #3b82f6;
        }
        QComboBox::drop-down {
            border: none;
            width: 20px;
        }
    )");

    if (!label.isEmpty())
    {
        QLabel *filterLabel = new QLabel(label, this);
        filterLabel->setStyleSheet("color: #64748b; font-size: 12px;");
        m_filterLayout->addWidget(filterLabel);
    }

    m_filterLayout->addWidget(filter);
    m_filters.append(filter);

    connect(filter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ChartContainer::onFilterChanged);
}

void ChartContainer::setFilterValue(int filterIndex, int optionIndex)
{
    if (filterIndex >= 0 && filterIndex < m_filters.size())
    {
        m_filters[filterIndex]->setCurrentIndex(optionIndex);
    }
}

void ChartContainer::showExportButton(bool show)
{
    m_exportBtn->setVisible(show);
}

void ChartContainer::showRefreshButton(bool show)
{
    m_refreshBtn->setVisible(show);
}

void ChartContainer::setMinimumChartHeight(int height)
{
    m_minChartHeight = height;
    m_chartView->setMinimumHeight(height);
}

// ========== STATE ==========

void ChartContainer::setLoading(bool loading)
{
    m_isLoading = loading;
    updateLoadingState();
}

void ChartContainer::setError(const QString &errorMessage)
{
    m_hasError = true;
    m_errorLabel->setText(errorMessage);
    m_errorWidget->show();
    m_chartView->hide();
}

void ChartContainer::clearError()
{
    m_hasError = false;
    m_errorWidget->hide();
    m_chartView->show();
}

// ========== STYLING ==========

void ChartContainer::setChartTheme(QChart::ChartTheme theme)
{
    if (m_chart)
    {
        m_chart->setTheme(theme);
    }
}

void ChartContainer::setAnimationEnabled(bool enabled)
{
    if (m_chart)
    {
        m_chart->setAnimationOptions(enabled ? QChart::SeriesAnimations : QChart::NoAnimation);
    }
}

void ChartContainer::setLegendVisible(bool visible)
{
    if (m_chart)
    {
        m_chart->legend()->setVisible(visible);
    }
}

void ChartContainer::setLegendAlignment(Qt::Alignment alignment)
{
    if (m_chart)
    {
        m_chart->legend()->setAlignment(alignment);
    }
}

// ========== GETTERS ==========

QString ChartContainer::title() const
{
    return m_title;
}

int ChartContainer::filterValue(int filterIndex) const
{
    if (filterIndex >= 0 && filterIndex < m_filters.size())
    {
        return m_filters[filterIndex]->currentIndex();
    }
    return -1;
}

// ========== SLOTS ==========

void ChartContainer::onFilterChanged(int index)
{
    QComboBox *filter = qobject_cast<QComboBox *>(sender());
    if (filter)
    {
        int filterIndex = filter->property("filterIndex").toInt();
        emit filterChanged(filterIndex, index);
    }
}

void ChartContainer::onExportClicked()
{
    emit exportClicked();
}

void ChartContainer::onRefreshClicked()
{
    emit refreshClicked();
}

// ========== PRIVATE METHODS ==========

void ChartContainer::updateLoadingState()
{
    if (m_isLoading)
    {
        m_loadingOverlay->setGeometry(m_chartView->geometry());
        m_loadingOverlay->show();
        m_loadingOverlay->raise();
    }
    else
    {
        m_loadingOverlay->hide();
    }
}
