/**
 * @file ServiceOrderWidget.h
 * @brief Service order widget - Tab 1 of Service Management
 * @details Left 60%: Cart table (40%) + Service grid (60%)
 *          Right 40%: Customer info + Payment info
 *
 * @author khninh22 - Service Module
 * @date 2025-11-14
 */

#ifndef SERVICEORDERWIDGET_H
#define SERVICEORDERWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QTextEdit>
#include <QMessageBox>
#include "../../Core/QuanLy/HeThongQuanLy.h"
#include "../../Core/Models/DonHangDichVu.h"
#include "../../Dialogs/ServiceSelectionDialog.h"

/**
 * @struct CartItem
 * @brief Struct để lưu item trong giỏ hàng tạm thời
 */
struct CartItem
{
    QString maDichVu;
    QString tenDichVu;
    QString donVi;
    double donGia;
    int soLuong;
    double thanhTien;

    CartItem() : donGia(0), soLuong(0), thanhTien(0) {}
    CartItem(QString ma, QString ten, QString dv, double gia, int sl)
        : maDichVu(ma), tenDichVu(ten), donVi(dv), donGia(gia), soLuong(sl)
    {
        thanhTien = donGia * soLuong;
    }
};

/**
 * @class ServiceOrderWidget
 * @brief Widget for ordering services (Redesigned)
 */
class ServiceOrderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServiceOrderWidget(QWidget *parent = nullptr);
    ~ServiceOrderWidget();

    void loadServices(); // Keep for compatibility, though dialog handles loading now

signals:
    void orderCreated(const QString &orderId);

private slots:
    // Customer slots
    void onCustomerSelected(int index);
    void onAddNewCustomerClicked();
    
    // Selection slots
    void onSelectServiceClicked();
    
    // Cart slots
    void onRemoveFromCartClicked(int row);
    void onClearCartClicked();
    
    // Payment slots
    void onPaymentClicked();

    // Cart modification slots
    void onIncreaseQuantity(int row);
    void onDecreaseQuantity(int row);

private:
    // ===== SETUP METHODS =====
    void setupUI();
    void setupCustomerSection();
    void setupCartSection();
    void setupPaymentSection();
    void setupConnections();
    void applyStyles();

    // ===== LOGIC METHODS =====
    void updateCartTable();
    void updateTotals();
    void loadCustomers();
    void updateCustomerInfo();

    // ===== UI COMPONENTS =====
    QHBoxLayout *mainLayout;
    QVBoxLayout *leftColumnLayout;
    QVBoxLayout *rightColumnLayout;

    // 1. Customer Section
    QFrame *customerFrame;
    QLineEdit *phoneLineEdit;
    QComboBox *customerComboBox;
    QLabel *customerNameLabel;
    QLabel *membershipLabel;
    
    // 2. Selection Button
    QPushButton *selectServiceBtn;

    // 3. Cart Section
    QTableWidget *cartTable;
    
    // 4. Payment Section
    QFrame *paymentFrame;
    QLabel *totalPriceLabel;
    QLabel *discountLabel;
    QLabel *finalAmountLabel;
    QTextEdit *noteTextEdit;
    QPushButton *paymentBtn;

    // ===== DATA =====
    HeThongQuanLy *system;
    QList<CartItem> cartItems;
    KhachHang *selectedCustomer;
    
    // Totals
    double totalPrice;
    double discount;
    double finalAmount;
};

#endif // SERVICEORDERWIDGET_H
