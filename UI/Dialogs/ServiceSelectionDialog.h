#ifndef SERVICESELECTIONDIALOG_H
#define SERVICESELECTIONDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>
#include <QMap>
#include "../../Core/Models/DichVu.h"
#include "../../Core/QuanLy/HeThongQuanLy.h"

#include <QTableWidget>
#include <QHeaderView>

// Structure to hold selection data
struct ServiceSelectionItem {
    DichVu* service;
    int quantity;
};

class ServiceSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServiceSelectionDialog(QWidget *parent = nullptr);
    ~ServiceSelectionDialog();

    // Get the list of selected services and their quantities
    QList<ServiceSelectionItem> getSelectedServices() const;

    // Pre-fill dialog with existing cart items if needed (optional)
    void setCurrentSelection(const QMap<QString, int>& currentQuantities);
    
    // Set existing cart items to calculate available stock
    void setExistingCart(const QMap<QString, int>& existingQuantities);

private slots:
    void onSearchTextChanged(const QString &text);
    void onCategoryFilterChanged(int index);
    void onServiceCardClicked(DichVu *service);
    void onConfirmClicked();
    void onCancelClicked();
    
    // Cart slots
    void onIncreaseQuantity(int row);
    void onDecreaseQuantity(int row);
    void onRemoveItem(int row);
    void onQuantityChanged(int row, int value);

private:
    void setupUI();
    void loadServices();
    void createServiceCards();
    QWidget *createServiceCard(DichVu *service);
    void updateSelectionSummary();
    void updateCartTable(); // New method
    int getAvailableStock(DichVu* service);

    // UI Components
    QLineEdit *searchEdit;
    QComboBox *categoryCombo;
    QScrollArea *serviceScrollArea;
    QWidget *serviceGridWidget;
    QGridLayout *serviceGridLayout;
    
    // Cart Components
    QTableWidget *cartTable;
    QLabel *totalPriceLabel;
    
    QLabel *summaryLabel;
    QPushButton *confirmBtn;
    QPushButton *cancelBtn;

    // Data
    HeThongQuanLy *system;
    QList<DichVu *> allServices;
    QList<DichVu *> filteredServices;
    
    // Map to track temporary selection in dialog: ServiceID -> Quantity
    QMap<QString, int> tempSelection; 
    // Map to hold service pointers for easy access: ServiceID -> DichVu*
    QMap<QString, DichVu*> serviceMap;
    // Map to hold existing quantities in main cart
    QMap<QString, int> existingCart;
};

#endif // SERVICESELECTIONDIALOG_H
