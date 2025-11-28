/**
 * @file BookingTableTab.h
 * @brief Tab 2: Danh sách đặt sân (Full Table View)
 * @details Week 2 Day 9-10 implementation - Redesigned
 *          Layout: 100% Table View with search, filters, stats cards
 *          Double-click row to open detail dialog for edit/cancel/payment
 *
 * @author Football Field Management System
 * @date 2025-11-18
 */

#ifndef BOOKINGTABLETAB_H
#define BOOKINGTABLETAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QDateEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include "../../../Core/QuanLy/HeThongQuanLy.h"
#include "../../../Core/Models/DatSan.h"
#include "../../../Core/Models/KhachHang.h"
#include "../../../Core/Models/San.h"

/**
 * @class BookingTableTab
 * @brief Full-width table view for booking management
 */
class BookingTableTab : public QWidget
{
    Q_OBJECT

public:
    explicit BookingTableTab(QWidget *parent = nullptr);
    ~BookingTableTab();

    /**
     * @brief Refresh table data
     */
    void refreshData();

signals:
    /**
     * @brief Emitted when booking data changes
     */
    void bookingDataChanged();
    
    /**
     * @brief Emitted when user requests to reschedule a booking
     */
    void rescheduleRequested(DatSan *booking);

private slots:
    void onTableRowDoubleClicked(int row, int column);
    void onSearchTextChanged();
    void onFilterChanged();

private:
    void setupUI();
    void setupSearchAndFilters();
    void setupStatsCards();
    void setupTable();
    void setupConnections();
    void loadTableData();
    QString getStatusText(TrangThaiDatSan status) const;
    QColor getStatusColor(TrangThaiDatSan status) const;
    QString getPaymentStatusText(bool paid) const;
    QColor getPaymentStatusColor(bool paid) const;
    DatSan *getBookingAtRow(int row);
    QString formatCurrency(double amount) const;

private:
    HeThongQuanLy *system;
    MangDong<San *> fields;

    // Main layout
    QVBoxLayout *mainLayout;

    // Search & Filters
    QLineEdit *searchBox;
    QComboBox *fieldFilterCombo;
    QComboBox *statusFilterCombo;
    QDateEdit *fromDateEdit;
    QDateEdit *toDateEdit;

    // Stats Cards
    QFrame *totalCard;
    QFrame *confirmedCard;
    QFrame *pendingCard;
    QFrame *revenueCard;
    QLabel *totalLabel;
    QLabel *confirmedLabel;
    QLabel *pendingLabel;
    QLabel *revenueLabel;

    // Table
    QTableWidget *bookingTable;
};

#endif // BOOKINGTABLETAB_H
