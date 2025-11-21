/**
 * @file ServiceManagementWidget.h
 * @brief Service Management Widget (CRUD) - Tab 2
 * @author khninh22 - Service Module
 * @date 2025-11-14
 */

#ifndef SERVICEMANAGEMENTWIDGET_H
#define SERVICEMANAGEMENTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QTextEdit>
#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>
#include "../../Core/QuanLy/HeThongQuanLy.h"
#include "../../Core/Models/DichVu.h"

/**
 * @class ServiceManagementWidget
 * @brief Widget for managing services (CRUD operations) - Tab 2
 */
class ServiceManagementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServiceManagementWidget(QWidget *parent = nullptr);
    ~ServiceManagementWidget();

private:
    void setupUI();
    void setupConnections();
    void applyStyles();
    void loadServices();
    void loadServiceToForm(DichVu *service);
    void clearForm();
    void updateStatsCards();
    bool validateServiceData();

private slots:
    void onSearchTextChanged(const QString &text);
    void onCategoryFilterChanged(int index);
    void onPriceFilterChanged(int index);
    void onReloadClicked();
    void onAddNewClicked();
    void onTableRowClicked(int row);
    void onSaveClicked();
    void onDeleteClicked();

signals:
    void serviceUpdated();

private:
    // ===== LAYOUTS =====
    QHBoxLayout *mainLayout;
    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;

    // ===== LEFT PANEL (70%) =====
    // Search & Filters
    QLineEdit *searchEdit;
    QComboBox *categoryCombo;
    QComboBox *priceCombo;
    QPushButton *reloadBtn;

    // Stats Cards
    QLabel *totalServicesLabel;
    QLabel *drinkServicesLabel;
    QLabel *equipmentServicesLabel;

    // Table
    QTableWidget *serviceTable;

    // ===== RIGHT PANEL (30%) =====
    QFrame *formFrame;
    QPushButton *addNewBtn;

    // Form fields
    QLineEdit *codeEdit;
    QLineEdit *nameEdit;
    QComboBox *categoryEdit;
    QLineEdit *priceEdit;
    QLineEdit *unitEdit;
    QLineEdit *stockEdit; // Added stock edit field
    QCheckBox *availableCheckBox;
    QTextEdit *descriptionEdit;

    // Action buttons
    QPushButton *saveBtn;
    QPushButton *deleteBtn;

    // ===== DATA =====
    HeThongQuanLy *system;
    DichVu *currentService;
    QList<DichVu *> allServices;
    QList<DichVu *> filteredServices;
    bool isEditMode;

    // ===== HELPER METHODS =====
    QString generateNextServiceCode();
};

#endif // SERVICEMANAGEMENTWIDGET_H
