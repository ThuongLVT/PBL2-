#ifndef SERVICEORDERWIDGET_H
#define SERVICEORDERWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QMap>
#include <string>
#include "../../Core/QuanLy/QuanLyDichVu.h"
#include "../../Core/QuanLy/QuanLyKhachHang.h"
#include "../../Core/QuanLy/QuanLyDatSan.h" // For checking if customer has active booking if needed, or just general usage
#include <QCompleter>

class ServiceOrderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServiceOrderWidget(QWidget *parent = nullptr);
    ~ServiceOrderWidget();

    void refreshData(); // Reload data if needed

private slots:
    void onAddServiceClicked();
    void onRemoveServiceClicked();
    void onClearCartClicked();
    void onPhoneSearch(); // Triggered by enter or button
    void onPayClicked();
    void onTableItemChanged(QTableWidgetItem *item); // Handle direct quantity edits

private:
    void setupUI();
    void updateCartTable();
    void updateTotals();
    void clearForm();

    // UI Components
    // Left Side (Cart)
    QTableWidget *cartTable;
    QPushButton *btnAddService;
    QPushButton *btnRemoveService;
    QPushButton *btnClearCart;

    // Right Side (Customer)
    QLineEdit *txtPhone;
    QLineEdit *txtCustomerName;
    QLineEdit *txtCustomerRank;
    QCompleter *phoneCompleter; // Added

    // Right Side (Payment)
    QLabel *lblSubTotal;
    QLabel *lblDiscount;
    QLabel *lblFinalTotal;
    QPushButton *btnPay;

    // Data
    QMap<std::string, int> currentCart; // ServiceID -> Quantity
    QuanLyDichVu *qlDichVu;
    QuanLyKhachHang *qlKhachHang;

    // Current Customer State
    bool isCustomerFound;
    std::string currentCustomerId;
    double currentDiscountRate;
};

#endif // SERVICEORDERWIDGET_H
