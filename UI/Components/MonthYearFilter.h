#ifndef MONTHYEARFILTER_H
#define MONTHYEARFILTER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QComboBox>
#include <QFrame>
#include <QDate>

/**
 * @class MonthYearFilter
 * @brief Component bộ lọc Tháng/Năm dùng chung cho các tab thống kê
 *
 * Giao diện:
 * [Theo tháng] [Theo năm] | [Tháng X ▼] [Năm Y ▼] | [Xuất PDF] (optional)
 *
 * Signals:
 * - filterChanged(int month, int year, FilterMode mode)
 * - exportPdfClicked()
 */
class MonthYearFilter : public QWidget
{
    Q_OBJECT

public:
    /**
     * @enum FilterMode
     * @brief Chế độ lọc thống kê
     */
    enum FilterMode
    {
        ByMonth = 0, // Theo tháng
        ByYear       // Theo năm
    };
    Q_ENUM(FilterMode)

    explicit MonthYearFilter(QWidget *parent = nullptr, bool showExportBtn = false);
    ~MonthYearFilter();

    // Getters
    FilterMode filterMode() const { return m_filterMode; }
    int selectedMonth() const { return m_selectedMonth; }
    int selectedYear() const { return m_selectedYear; }

    // Setters
    void setFilterMode(FilterMode mode);
    void setMonth(int month);
    void setYear(int year);
    void setShowExportButton(bool show);

    // Get date range based on current filter
    QDate getFromDate() const;
    QDate getToDate() const;

signals:
    /**
     * @brief Emitted when filter changes (mode, month, or year)
     * @param month Selected month (1-12)
     * @param year Selected year
     * @param mode Current filter mode
     */
    void filterChanged(int month, int year, MonthYearFilter::FilterMode mode);

    /**
     * @brief Emitted when Export PDF button is clicked
     */
    void exportPdfClicked();

private slots:
    void onFilterModeChanged(int mode);
    void onMonthChanged(int index);
    void onYearChanged(int index);

private:
    void setupUI();
    void updateFilterVisibility();
    void populateMonthCombo();
    void populateYearCombo();
    void emitFilterChanged();

    // UI Components
    QHBoxLayout *m_layout;
    QButtonGroup *m_filterModeGroup;
    QPushButton *m_monthBtn;
    QPushButton *m_yearBtn;
    QFrame *m_separator;

    QWidget *m_selectorWidget;
    QHBoxLayout *m_selectorLayout;
    QComboBox *m_monthCombo;
    QComboBox *m_yearCombo;

    QPushButton *m_exportPdfBtn;
    bool m_showExportBtn;

    // State
    FilterMode m_filterMode;
    int m_selectedMonth;
    int m_selectedYear;
};

#endif // MONTHYEARFILTER_H
