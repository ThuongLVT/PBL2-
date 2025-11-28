#ifndef SUMMARYCARD_H
#define SUMMARYCARD_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>

/**
 * @class SummaryCard
 * @brief Modern summary card component with icon, value, and trend indicator
 *
 * Design features:
 * - Gradient or solid background colors
 * - Icon display
 * - Large value display with formatting
 * - Trend indicator (up/down percentage)
 * - Hover animation effects
 * - Optional progress bar
 */
class SummaryCard : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)

public:
    /**
     * @enum CardStyle
     * @brief Predefined card color styles
     */
    enum CardStyle
    {
        Primary, // Blue gradient
        Success, // Green gradient
        Warning, // Orange gradient
        Danger,  // Red gradient
        Info,    // Cyan gradient
        Purple,  // Purple gradient
        Custom   // Custom color
    };
    Q_ENUM(CardStyle)

    /**
     * @enum TrendDirection
     * @brief Trend arrow direction
     */
    enum TrendDirection
    {
        Up,     // Positive trend (green arrow up)
        Down,   // Negative trend (red arrow down)
        Neutral // No change (gray dash)
    };
    Q_ENUM(TrendDirection)

    explicit SummaryCard(QWidget *parent = nullptr);
    SummaryCard(const QString &title, const QString &icon, QWidget *parent = nullptr);
    ~SummaryCard();

    // ===== SETTERS =====
    void setTitle(const QString &title);
    void setValue(const QString &value);
    void setValue(double value, const QString &suffix = "");
    void setValueInt(int value, const QString &suffix = "");
    void setIcon(const QString &iconPath);
    void setIconText(const QString &emoji);
    void setTrend(double percentage, TrendDirection direction);
    void setTrendText(const QString &text);
    void setSubtitle(const QString &subtitle);
    void setCardStyle(CardStyle style);
    void setCustomColors(const QColor &bg, const QColor &text);
    void setProgress(double percentage); // 0-100
    void showProgress(bool show);
    void setBackgroundColor(const QColor &color);

    // ===== GETTERS =====
    QString title() const;
    QString value() const;
    CardStyle cardStyle() const;
    QColor backgroundColor() const;

    // ===== UTILITY =====
    static QString formatNumber(double value, int decimals = 0);
    static QString formatCurrency(double value, const QString &currency = "VND");

signals:
    void clicked();

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUI();
    void applyStyle();
    void updateTrendLabel();
    QString getGradientStyle(CardStyle style);

    // UI Components
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_headerLayout;
    QHBoxLayout *m_trendLayout;

    QLabel *m_iconLabel;
    QLabel *m_titleLabel;
    QLabel *m_valueLabel;
    QLabel *m_trendLabel;
    QLabel *m_trendIconLabel;
    QLabel *m_subtitleLabel;
    QWidget *m_progressBar;
    QWidget *m_progressFill;

    // State
    QString m_title;
    QString m_value;
    QString m_iconPath;
    CardStyle m_cardStyle;
    QColor m_backgroundColor;
    QColor m_textColor;
    double m_trendPercentage;
    TrendDirection m_trendDirection;
    double m_progress;
    bool m_showProgress;

    // Animation
    QGraphicsDropShadowEffect *m_shadowEffect;
    QPropertyAnimation *m_hoverAnimation;

    // Constants
    static const int CARD_MIN_WIDTH = 200;
    static const int CARD_MIN_HEIGHT = 120;
    static const int BORDER_RADIUS = 12;
};

#endif // SUMMARYCARD_H
