#ifndef CHARTCONTAINER_H
#define CHARTCONTAINER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QPainter>

/**
 * @class ChartContainer
 * @brief Modern container wrapper for Qt Charts with title, filters, and legend
 *
 * Features:
 * - Title and subtitle
 * - Filter dropdowns
 * - Action buttons (export, refresh)
 * - Responsive chart area
 * - Built-in loading state
 * - Modern styling with shadows
 */
class ChartContainer : public QWidget
{
    Q_OBJECT

public:
    explicit ChartContainer(QWidget *parent = nullptr);
    ChartContainer(const QString &title, QWidget *parent = nullptr);
    ~ChartContainer();

    // ===== CHART MANAGEMENT =====
    void setChart(QChart *chart);
    QChart *chart() const;
    QChartView *chartView() const;

    // ===== CONFIGURATION =====
    void setTitle(const QString &title);
    void setSubtitle(const QString &subtitle);
    void addFilter(const QString &label, const QStringList &options);
    void setFilterValue(int filterIndex, int optionIndex);
    void showExportButton(bool show);
    void showRefreshButton(bool show);
    void setMinimumChartHeight(int height);

    // ===== STATE =====
    void setLoading(bool loading);
    void setError(const QString &errorMessage);
    void clearError();

    // ===== STYLING =====
    void setChartTheme(QChart::ChartTheme theme);
    void setAnimationEnabled(bool enabled);
    void setLegendVisible(bool visible);
    void setLegendAlignment(Qt::Alignment alignment);

    // ===== GETTERS =====
    QString title() const;
    int filterValue(int filterIndex) const;

signals:
    void filterChanged(int filterIndex, int optionIndex);
    void exportClicked();
    void refreshClicked();

private slots:
    void onFilterChanged(int index);
    void onExportClicked();
    void onRefreshClicked();

private:
    void setupUI();
    void applyStyles();
    void updateLoadingState();

protected:
    void paintEvent(QPaintEvent *event) override;

    // UI Components
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_headerLayout;
    QHBoxLayout *m_filterLayout;

    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    QWidget *m_filterWidget;
    QList<QComboBox *> m_filters;
    QPushButton *m_exportBtn;
    QPushButton *m_refreshBtn;

    QChartView *m_chartView;
    QChart *m_chart;

    // Loading overlay
    QWidget *m_loadingOverlay;
    QLabel *m_loadingLabel;

    // Error display
    QWidget *m_errorWidget;
    QLabel *m_errorLabel;

    // State
    QString m_title;
    QString m_subtitle;
    bool m_isLoading;
    bool m_hasError;
    int m_minChartHeight;

    // Shadow effect
    QGraphicsDropShadowEffect *m_shadowEffect;
};

#endif // CHARTCONTAINER_H
