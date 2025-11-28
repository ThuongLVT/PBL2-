#ifndef DATERANGEPICKER_H
#define DATERANGEPICKER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QMenu>
#include <QCalendarWidget>
#include <QGraphicsDropShadowEffect>

/**
 * @class DateRangePicker
 * @brief Modern date range picker with preset options
 *
 * Features:
 * - Quick preset buttons (Today, This Week, This Month, etc.)
 * - Custom date range selection
 * - Calendar popup for date picking
 * - Clean modern design
 */
class DateRangePicker : public QWidget
{
    Q_OBJECT

public:
    /**
     * @enum PresetRange
     * @brief Predefined date ranges
     */
    enum PresetRange
    {
        Today,
        Yesterday,
        ThisWeek,
        LastWeek,
        ThisMonth,
        LastMonth,
        ThisQuarter,
        ThisYear,
        Last7Days,
        Last30Days,
        Last90Days,
        Custom
    };
    Q_ENUM(PresetRange)

    explicit DateRangePicker(QWidget *parent = nullptr);
    ~DateRangePicker();

    // ===== SETTERS =====
    void setDateRange(const QDate &from, const QDate &to);
    void setPreset(PresetRange preset);
    void setMinDate(const QDate &date);
    void setMaxDate(const QDate &date);
    void setShowPresetButtons(bool show);
    void setCompactMode(bool compact);

    // ===== GETTERS =====
    QDate fromDate() const;
    QDate toDate() const;
    PresetRange currentPreset() const;
    int dayCount() const;

    // ===== UTILITY =====
    static QString presetToString(PresetRange preset);
    static QPair<QDate, QDate> getPresetDates(PresetRange preset);

signals:
    void dateRangeChanged(const QDate &from, const QDate &to);
    void presetChanged(PresetRange preset);

private slots:
    void onFromDateChanged(const QDate &date);
    void onToDateChanged(const QDate &date);
    void onPresetClicked();
    void onPresetComboChanged(int index);

private:
    void setupUI();
    void setupCompactUI();
    void setupFullUI();
    void applyStyles();
    void updatePresetFromDates();
    void applyPreset(PresetRange preset);

    // UI Components - Full mode
    QHBoxLayout *m_mainLayout;
    QWidget *m_presetWidget;
    QHBoxLayout *m_presetLayout;
    QPushButton *m_todayBtn;
    QPushButton *m_weekBtn;
    QPushButton *m_monthBtn;
    QPushButton *m_quarterBtn;
    QPushButton *m_yearBtn;
    QPushButton *m_customBtn;

    // Date selection
    QWidget *m_dateWidget;
    QHBoxLayout *m_dateLayout;
    QLabel *m_fromLabel;
    QDateEdit *m_fromDateEdit;
    QLabel *m_toLabel;
    QDateEdit *m_toDateEdit;

    // Compact mode
    QComboBox *m_presetCombo;
    QPushButton *m_dateRangeBtn;
    QMenu *m_dateMenu;

    // State
    QDate m_fromDate;
    QDate m_toDate;
    PresetRange m_currentPreset;
    bool m_showPresetButtons;
    bool m_compactMode;

    // Shadow effect
    QGraphicsDropShadowEffect *m_shadowEffect;
};

#endif // DATERANGEPICKER_H
