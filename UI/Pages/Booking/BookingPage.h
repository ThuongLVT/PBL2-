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
#include <QTableWidget>
#include <QDateEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QHeaderView>

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

private slots:
    void applyTableFilters();
    void loadTableData();
    void onViewBooking(int row);
    void onEditBooking(int row);
    void onCancelBooking(int row);
    void onPaymentBooking(int row);
    void onAddServices(int row);
    void onTableRowDoubleClicked(int row, int column);

private:
    void setupUI();
    void setupConnections();
    void loadFieldsToFilter();
    QString getStatusText(int status) const;
    QColor getStatusColor(int status) const;

private:
    QVBoxLayout *mainLayout;
    QTabWidget *tabWidget;

    // Tab 1: Timeline View
    TimelineTab *timelineTab;

    // Tab 2: Table View
    QWidget *tableTab;
    QTableWidget *bookingTable;
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;
    QComboBox *fieldFilterCombo;
    QComboBox *statusFilterCombo;
    QLineEdit *searchBox;
};

#endif // BOOKINGPAGE_H
