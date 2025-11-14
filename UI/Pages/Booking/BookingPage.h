/**
 * @file BookingPage.h
 * @brief Main booking page with 2 tabs: Timeline View + Table View
 * @details Week 2 Day 7-12 implementation
 *          Tab 1: Timeline view for visual booking
 *          Tab 2: Table view for booking management
 *
 * @author Football Field Management System
 * @date 2025-11-14
 */

#ifndef BOOKINGPAGE_H
#define BOOKINGPAGE_H

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>

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

private:
    void setupUI();
    void setupConnections();

private:
    QVBoxLayout *mainLayout;
    QTabWidget *tabWidget;

    // Tab 1: Timeline View
    TimelineTab *timelineTab;

    // Tab 2: Table View (placeholder for now)
    QWidget *tableTab;
};

#endif // BOOKINGPAGE_H
