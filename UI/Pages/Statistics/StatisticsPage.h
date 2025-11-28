#ifndef STATISTICSPAGE_H
#define STATISTICSPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>

// Forward declarations
class RevenueTab;
class BookingFieldTab;
class CustomerTab;
class ServiceTab;
class HeThongQuanLy;

/**
 * @class StatisticsPage
 * @brief Trang thống kê chính với 4 tab
 *
 * Bao gồm:
 * - Tab 1: Doanh Thu (Revenue Analytics)
 * - Tab 2: Booking & Sân
 * - Tab 3: Khách Hàng
 * - Tab 4: Dịch Vụ
 */
class StatisticsPage : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsPage(QWidget *parent = nullptr);
    ~StatisticsPage();

    void setHeThong(HeThongQuanLy *ht);
    void refreshData();

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void onTabChanged(int index);

private:
    void setupUI();
    void styleTabWidget();

    // UI Components
    QVBoxLayout *m_mainLayout;
    QTabWidget *m_tabWidget;

    // Tabs
    RevenueTab *m_revenueTab;
    BookingFieldTab *m_bookingFieldTab;
    CustomerTab *m_customerTab;
    ServiceTab *m_serviceTab;

    // Data
    HeThongQuanLy *m_heThong;
};

#endif // STATISTICSPAGE_H
