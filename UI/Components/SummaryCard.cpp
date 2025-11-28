#include "SummaryCard.h"
#include <QPainter>
#include <QMouseEvent>
#include <QLocale>
#include <cmath>

// ========== CONSTRUCTORS ==========

SummaryCard::SummaryCard(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(nullptr),
      m_headerLayout(nullptr),
      m_trendLayout(nullptr),
      m_iconLabel(nullptr),
      m_titleLabel(nullptr),
      m_valueLabel(nullptr),
      m_trendLabel(nullptr),
      m_trendIconLabel(nullptr),
      m_subtitleLabel(nullptr),
      m_progressBar(nullptr),
      m_progressFill(nullptr),
      m_cardStyle(Primary),
      m_backgroundColor(QColor("#3b82f6")),
      m_textColor(Qt::white),
      m_trendPercentage(0.0),
      m_trendDirection(Neutral),
      m_progress(0.0),
      m_showProgress(false),
      m_shadowEffect(nullptr),
      m_hoverAnimation(nullptr)
{
    setupUI();
    applyStyle();
}

SummaryCard::SummaryCard(const QString &title, const QString &icon, QWidget *parent)
    : SummaryCard(parent)
{
    setTitle(title);
    setIconText(icon);
}

SummaryCard::~SummaryCard()
{
}

// ========== SETUP UI ==========

void SummaryCard::setupUI()
{
    setMinimumSize(CARD_MIN_WIDTH, CARD_MIN_HEIGHT);
    setMaximumHeight(180);
    setCursor(Qt::PointingHandCursor);

    // Main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(20, 16, 20, 16);
    m_mainLayout->setSpacing(8);

    // Header layout (icon + title)
    m_headerLayout = new QHBoxLayout();
    m_headerLayout->setSpacing(10);

    m_iconLabel = new QLabel(this);
    m_iconLabel->setFixedSize(40, 40);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    m_iconLabel->setStyleSheet(R"(
        QLabel {
            background-color: rgba(255, 255, 255, 0.2);
            border-radius: 8px;
            font-size: 20px;
        }
    )");

    m_titleLabel = new QLabel(this);
    m_titleLabel->setStyleSheet(R"(
        QLabel {
            color: rgba(255, 255, 255, 0.9);
            font-size: 13px;
            font-weight: 500;
        }
    )");

    m_headerLayout->addWidget(m_iconLabel);
    m_headerLayout->addWidget(m_titleLabel, 1);

    // Value label
    m_valueLabel = new QLabel(this);
    m_valueLabel->setStyleSheet(R"(
        QLabel {
            color: white;
            font-size: 28px;
            font-weight: 700;
        }
    )");

    // Trend layout
    m_trendLayout = new QHBoxLayout();
    m_trendLayout->setSpacing(4);

    m_trendIconLabel = new QLabel(this);
    m_trendIconLabel->setFixedSize(16, 16);

    m_trendLabel = new QLabel(this);
    m_trendLabel->setStyleSheet(R"(
        QLabel {
            color: rgba(255, 255, 255, 0.85);
            font-size: 12px;
        }
    )");

    m_trendLayout->addWidget(m_trendIconLabel);
    m_trendLayout->addWidget(m_trendLabel);
    m_trendLayout->addStretch();

    // Subtitle label
    m_subtitleLabel = new QLabel(this);
    m_subtitleLabel->setStyleSheet(R"(
        QLabel {
            color: rgba(255, 255, 255, 0.7);
            font-size: 11px;
        }
    )");
    m_subtitleLabel->hide();

    // Progress bar
    m_progressBar = new QWidget(this);
    m_progressBar->setFixedHeight(6);
    m_progressBar->setStyleSheet(R"(
        background-color: rgba(255, 255, 255, 0.3);
        border-radius: 3px;
    )");

    m_progressFill = new QWidget(m_progressBar);
    m_progressFill->setFixedHeight(6);
    m_progressFill->setStyleSheet(R"(
        background-color: rgba(255, 255, 255, 0.9);
        border-radius: 3px;
    )");
    m_progressBar->hide();

    // Add to main layout
    m_mainLayout->addLayout(m_headerLayout);
    m_mainLayout->addWidget(m_valueLabel);
    m_mainLayout->addLayout(m_trendLayout);
    m_mainLayout->addWidget(m_subtitleLabel);
    m_mainLayout->addWidget(m_progressBar);
    m_mainLayout->addStretch();

    // Shadow effect
    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setBlurRadius(20);
    m_shadowEffect->setXOffset(0);
    m_shadowEffect->setYOffset(4);
    m_shadowEffect->setColor(QColor(0, 0, 0, 40));
    setGraphicsEffect(m_shadowEffect);

    // Hover animation
    m_hoverAnimation = new QPropertyAnimation(m_shadowEffect, "blurRadius", this);
    m_hoverAnimation->setDuration(150);
}

// ========== SETTERS ==========

void SummaryCard::setTitle(const QString &title)
{
    m_title = title;
    m_titleLabel->setText(title);
}

void SummaryCard::setValue(const QString &value)
{
    m_value = value;
    m_valueLabel->setText(value);
}

void SummaryCard::setValue(double value, const QString &suffix)
{
    m_value = formatNumber(value) + (suffix.isEmpty() ? "" : " " + suffix);
    m_valueLabel->setText(m_value);
}

void SummaryCard::setValueInt(int value, const QString &suffix)
{
    m_value = formatNumber(value) + (suffix.isEmpty() ? "" : " " + suffix);
    m_valueLabel->setText(m_value);
}

void SummaryCard::setIcon(const QString &iconPath)
{
    m_iconPath = iconPath;
    QPixmap pixmap(iconPath);
    if (!pixmap.isNull())
    {
        m_iconLabel->setPixmap(pixmap.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void SummaryCard::setIconText(const QString &emoji)
{
    m_iconLabel->setText(emoji);
}

void SummaryCard::setTrend(double percentage, TrendDirection direction)
{
    m_trendPercentage = percentage;
    m_trendDirection = direction;
    updateTrendLabel();
}

void SummaryCard::setTrendText(const QString &text)
{
    m_trendLabel->setText(text);
}

void SummaryCard::setSubtitle(const QString &subtitle)
{
    m_subtitleLabel->setText(subtitle);
    m_subtitleLabel->setVisible(!subtitle.isEmpty());
}

void SummaryCard::setCardStyle(CardStyle style)
{
    m_cardStyle = style;
    applyStyle();
}

void SummaryCard::setCustomColors(const QColor &bg, const QColor &text)
{
    m_cardStyle = Custom;
    m_backgroundColor = bg;
    m_textColor = text;
    applyStyle();
}

void SummaryCard::setProgress(double percentage)
{
    m_progress = qBound(0.0, percentage, 100.0);
    if (m_progressBar->isVisible())
    {
        int fillWidth = static_cast<int>((m_progressBar->width() - 4) * m_progress / 100.0);
        m_progressFill->setFixedWidth(fillWidth);
    }
}

void SummaryCard::showProgress(bool show)
{
    m_showProgress = show;
    m_progressBar->setVisible(show);
}

void SummaryCard::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
    update();
}

// ========== GETTERS ==========

QString SummaryCard::title() const { return m_title; }
QString SummaryCard::value() const { return m_value; }
SummaryCard::CardStyle SummaryCard::cardStyle() const { return m_cardStyle; }
QColor SummaryCard::backgroundColor() const { return m_backgroundColor; }

// ========== UTILITY ==========

QString SummaryCard::formatNumber(double value, int decimals)
{
    QLocale locale(QLocale::Vietnamese);
    return locale.toString(value, 'f', decimals);
}

QString SummaryCard::formatCurrency(double value, const QString &currency)
{
    QLocale locale(QLocale::Vietnamese);
    QString formatted = locale.toString(value, 'f', 0);
    return formatted + " " + currency;
}

// ========== PROTECTED EVENTS ==========

void SummaryCard::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event);
    // Hover effect đơn giản - chỉ tăng shadow, không có animation
    m_shadowEffect->setBlurRadius(25);
}

void SummaryCard::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    // Reset shadow về mặc định
    m_shadowEffect->setBlurRadius(20);
}

void SummaryCard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
    QWidget::mousePressEvent(event);
}

void SummaryCard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw rounded rectangle with gradient
    QLinearGradient gradient(0, 0, width(), height());

    switch (m_cardStyle)
    {
    case Primary:
        gradient.setColorAt(0, QColor("#3b82f6"));
        gradient.setColorAt(1, QColor("#1d4ed8"));
        break;
    case Success:
        gradient.setColorAt(0, QColor("#10b981"));
        gradient.setColorAt(1, QColor("#059669"));
        break;
    case Warning:
        gradient.setColorAt(0, QColor("#f59e0b"));
        gradient.setColorAt(1, QColor("#d97706"));
        break;
    case Danger:
        gradient.setColorAt(0, QColor("#ef4444"));
        gradient.setColorAt(1, QColor("#dc2626"));
        break;
    case Info:
        gradient.setColorAt(0, QColor("#06b6d4"));
        gradient.setColorAt(1, QColor("#0891b2"));
        break;
    case Purple:
        gradient.setColorAt(0, QColor("#8b5cf6"));
        gradient.setColorAt(1, QColor("#7c3aed"));
        break;
    case Custom:
        gradient.setColorAt(0, m_backgroundColor);
        gradient.setColorAt(1, m_backgroundColor.darker(110));
        break;
    }

    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), BORDER_RADIUS, BORDER_RADIUS);
}

// ========== PRIVATE METHODS ==========

void SummaryCard::applyStyle()
{
    // Update text colors based on style
    QString textColor = "white";

    m_titleLabel->setStyleSheet(QString(R"(
        QLabel {
            color: rgba(255, 255, 255, 0.9);
            font-size: 13px;
            font-weight: 500;
            background: transparent;
        }
    )"));

    m_valueLabel->setStyleSheet(QString(R"(
        QLabel {
            color: %1;
            font-size: 28px;
            font-weight: 700;
            background: transparent;
        }
    )")
                                    .arg(textColor));

    update();
}

void SummaryCard::updateTrendLabel()
{
    QString arrow;
    QString color;

    switch (m_trendDirection)
    {
    case Up:
        arrow = "↑";
        color = "rgba(134, 239, 172, 1)"; // Light green
        break;
    case Down:
        arrow = "↓";
        color = "rgba(252, 165, 165, 1)"; // Light red
        break;
    case Neutral:
        arrow = "→";
        color = "rgba(255, 255, 255, 0.7)";
        break;
    }

    m_trendIconLabel->setText(arrow);
    m_trendIconLabel->setStyleSheet(QString(R"(
        QLabel {
            color: %1;
            font-size: 14px;
            font-weight: bold;
            background: transparent;
        }
    )")
                                        .arg(color));

    QString trendText = QString::number(std::abs(m_trendPercentage), 'f', 1) + "% so với kỳ trước";
    m_trendLabel->setText(trendText);
    m_trendLabel->setStyleSheet(QString(R"(
        QLabel {
            color: %1;
            font-size: 12px;
            background: transparent;
        }
    )")
                                    .arg(color));
}

QString SummaryCard::getGradientStyle(CardStyle style)
{
    switch (style)
    {
    case Primary:
        return "qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #3b82f6, stop:1 #1d4ed8)";
    case Success:
        return "qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #10b981, stop:1 #059669)";
    case Warning:
        return "qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f59e0b, stop:1 #d97706)";
    case Danger:
        return "qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #ef4444, stop:1 #dc2626)";
    case Info:
        return "qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #06b6d4, stop:1 #0891b2)";
    case Purple:
        return "qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #8b5cf6, stop:1 #7c3aed)";
    default:
        return "";
    }
}
