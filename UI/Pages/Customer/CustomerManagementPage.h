/**
 * @file CustomerManagementPage.h
 * @brief Customer management page with 3-panel layout
 * @details Left 70%: search, filters, stats, table
 *          Right top 60%: customer details form
 *          Right bottom 40%: booking history
 *
 * @author Football Field Management System
 * @date 2025-11-11
 */

#ifndef CUSTOMERMANAGEMENTPAGE_H
#define CUSTOMERMANAGEMENTPAGE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QListWidget>
#include "../../Core/QuanLy/HeThongQuanLy.h"

/**
 * @class CustomerManagementPage
 * @brief Customer CRUD management with search, filters, and stats
 */
class CustomerManagementPage : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerManagementPage(QWidget *parent = nullptr);
    ~CustomerManagementPage();

    /**
     * @brief Load all customers from Core and display
     */
    void loadCustomers();

    /**
     * @brief Refresh data (alias for loadCustomers)
     */
    void refreshData() { loadCustomers(); }

private slots:
    /**
     * @brief Search customers by name or phone
     */
    void onSearchTextChanged(const QString &text);

    /**
     * @brief Filter by member tier
     */
    void onTierFilterChanged(int index);

    /**
     * @brief Filter by spending range
     */
    void onSpendingFilterChanged(int index);

    /**
     * @brief Customer row clicked - show details
     */
    void onCustomerRowClicked(int row);

    /**
     * @brief Add new customer button clicked
     */
    void onAddCustomerClicked();

    /**
     * @brief Save customer (add or edit)
     */
    void onSaveCustomerClicked();

    /**
     * @brief Delete customer with confirmation
     */
    void onDeleteCustomerClicked();

private:
    // ===== LAYOUTS =====
    QHBoxLayout *mainLayout;
    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;
    QVBoxLayout *rightTopLayout;
    QVBoxLayout *rightBottomLayout;

    // ===== LEFT PANEL (70%) =====
    // Search & Filters
    QLineEdit *searchEdit;
    QComboBox *tierFilterCombo;
    QComboBox *spendingFilterCombo;

    // Stats Cards (3 cards)
    QFrame *statsContainer;
    QLabel *totalCustomersLabel;
    QLabel *newCustomersLabel;
    QLabel *vipCustomersLabel;

    // Customer Table
    QTableWidget *customerTable;

    // ===== RIGHT PANEL (30%) =====
    // Right Top (60%) - Customer Details Form
    QFrame *detailsContainer;
    QLabel *detailsTitleLabel;
    QLineEdit *nameEdit;
    QLineEdit *phoneEdit;
    QLineEdit *spendingEdit; // Read-only
    QPushButton *addButton;
    QPushButton *saveButton;
    QPushButton *deleteButton;

    // Right Bottom (40%) - Booking History
    QFrame *historyContainer;
    QLabel *historyTitleLabel;
    QScrollArea *historyScrollArea;
    QListWidget *historyList;

    // ===== DATA =====
    HeThongQuanLy *system;
    KhachHang *currentCustomer; // Currently selected customer
    bool isEditMode;            // true = edit, false = add new

    // ===== HELPER METHODS =====
    void setupUI();
    void setupLeftPanel();
    void setupRightPanel();
    void setupTable();
    void setupConnections();
    void applyStyles();

    void updateStatsCards();
    void loadSpendingHistory(const std::string &maKH);
    void clearCustomerForm();
    bool validateCustomerData();
    QString formatCurrency(double amount) const;
    QString getTierDisplayName(HangKhachHang hang) const;
};

#endif // CUSTOMERMANAGEMENTPAGE_H
