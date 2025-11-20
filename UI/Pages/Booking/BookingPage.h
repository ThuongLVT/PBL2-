/**
 * @file BookingPage.h
 * @brief Main booking page with 2 tabs
 * @details Tab 1: Đặt sân (Timeline View) - Completed
 *          Tab 2: Danh sách đặt sân (Table View) - Completed
 *
 * @author Football Field Management System
 * @date 2025-11-17
 */

#ifndef BOOKINGPAGE_H
#define BOOKINGPAGE_H

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>

// Forward declarations
class TimelineTab;
class BookingTableTab;

/**
 * @class BookingPage
 * @brief Container for booking management tabs
 */
class BookingPage : public QWidget
{
    Q_OBJECT

public:
    explicit BookingPage(QWidget *parent = nullptr);
    ~BookingPage();

    /**
     * @brief Refresh data when page is shown
     */
    void refreshData();

    /**
     * @brief Reload fields from Core
     */
    void reloadFields();

private:
    void setupUI();
    void setupConnections();

private:
    QVBoxLayout *mainLayout;
    QTabWidget *tabWidget;

    // Tab 1: Đặt sân (Timeline View)
    TimelineTab *timelineTab;

    // Tab 2: Danh sách đặt sân (Table View)
    BookingTableTab *bookingTableTab;
};

#endif // BOOKINGPAGE_H
