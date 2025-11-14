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
 * @brief Widget for ordering services (standalone - not related to field booking)
 */
class ServiceOrderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServiceOrderWidget(QWidget *parent = nullptr);
    ~ServiceOrderWidget();

    /**
     * @brief Load all services from Core
     */
    void loadServices();

signals:
    /**
     * @brief Emitted when an order is created successfully
     */
    void orderCreated(const QString &orderId);

private slots:
    /**
     * @brief Search services
     */
    void onSearchTextChanged(const QString &text);

    /**
     * @brief Filter by service category
     */
    void onCategoryFilterChanged(int index);

    /**
     * @brief Service card clicked - add to cart
     */
    void onServiceCardClicked(DichVu *service);

    /**
     * @brief Remove item from cart
     */
    void onRemoveFromCartClicked(int row);

    /**
     * @brief Customer selected from dropdown
     */
    void onCustomerSelected(int index);

    /**
     * @brief Add new customer clicked
     */
    void onAddNewCustomerClicked();

    /**
     * @brief Payment button clicked
     */
    void onPaymentClicked();

    /**
     * @brief Clear cart
     */
    void onClearCartClicked();

    /**
     * @brief Export order to PDF
     */
    void onExportClicked();

    /**
     * @brief Reload services
     */
    void onReloadClicked();

private:
    // ===== SETUP METHODS =====
    void setupUI();
    void setupLeftPanel();
    void setupRightPanel();
    void setupConnections();
    void applyStyles();

    // ===== CART METHODS =====
    void addToCart(DichVu *service);
    void updateCart();
    void clearCart();
    void updateTotals();

    // ===== SERVICE GRID METHODS =====
    void createServiceCards();
    QWidget *createServiceCard(DichVu *service);

    // ===== CUSTOMER METHODS =====
    void loadCustomers();
    void filterCustomers(const QString &searchText);
    void selectCustomer(KhachHang *customer);
    void updateCustomerInfo();

    // ===== LAYOUTS =====
    QHBoxLayout *mainLayout;
    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;

    // ===== LEFT PANEL (60%) =====
    // Top 40%: Cart Table
    QFrame *cartFrame;
    QVBoxLayout *cartLayout;
    QTableWidget *cartTable;
    QLabel *cartTotalLabel;

    // Bottom 60%: Service Grid
    QFrame *serviceFrame;
    QVBoxLayout *serviceLayout;
    QLineEdit *searchEdit;
    QComboBox *categoryCombo;
    QPushButton *reloadBtn;
    QScrollArea *serviceScrollArea;
    QWidget *serviceGridWidget;
    QGridLayout *serviceGridLayout;

    // ===== RIGHT PANEL (40%) =====
    // Customer & Payment Info (Combined)
    QFrame *customerFrame;
    QVBoxLayout *customerLayout;
    QLineEdit *phoneLineEdit;
    QComboBox *customerComboBox;
    QLineEdit *nameLineEdit;
    QLabel *customerNameLabel;
    QLabel *phoneLabel;
    QLabel *membershipLabel;
    QTextEdit *noteTextEdit;
    QLabel *totalPriceLabel;
    QLabel *discountLabel;
    QLabel *finalAmountLabel;

    // Action Buttons
    QFrame *actionFrame;
    QHBoxLayout *actionLayout;
    QPushButton *clearBtn;
    QPushButton *paymentBtn;
    QPushButton *exportBtn;

    // ===== DATA =====
    HeThongQuanLy *system;
    QList<CartItem> cartItems;
    KhachHang *selectedCustomer;
    QList<DichVu *> allServices;
    QList<DichVu *> filteredServices;

    // Totals
    double totalPrice;
    double discount;
    double finalAmount;
};

#endif // SERVICEORDERWIDGET_H
