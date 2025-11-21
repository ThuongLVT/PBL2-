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

class ServiceManagementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServiceManagementWidget(QWidget *parent = nullptr);
    ~ServiceManagementWidget();

private:
    void setupUI();
    void setupConnections();
    void loadServices();
    void loadServiceToForm(DichVu *service);
    void clearForm();
    bool validateServiceData();
    void updateUnitFilter(); // Helper to populate unit filter

private slots:
    void onSearchTextChanged(const QString &text);
    void onStatusFilterChanged(int index);
    void onSortByChanged(int index);
    void onSortOrderChanged(int index);
    void onUnitFilterChanged(int index);
    void onReloadClicked();
    void onAddNewClicked();
    void onTableRowClicked(int row);
    void onSaveClicked();
    void onDeleteClicked();
    void onUploadImageClicked();

private:
    // ===== LAYOUTS =====
    QHBoxLayout *mainLayout;
    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;

    // ===== LEFT PANEL (70%) =====
    // Search & Filters
    QLineEdit *searchEdit;
    QPushButton *searchBtn;       // Added search button
    QCompleter *searchCompleter;  // Search suggestions
    QComboBox *statusFilterCombo; // Active/Inactive
    QComboBox *sortByCombo;       // Price, Stock, Sold
    QComboBox *sortOrderCombo;    // Asc, Desc
    QComboBox *unitFilterCombo;   // Unit
    QPushButton *reloadBtn;

    // Table
    QTableWidget *serviceTable;

    // ===== RIGHT PANEL (30%) =====
    QFrame *formFrame;

    // Image Upload
    QLabel *imagePreviewLabel;
    QPushButton *uploadImageBtn;
    QString currentImagePath;

    // Form fields
    QLineEdit *codeEdit;
    QLineEdit *nameEdit;
    QComboBox *categoryEdit;
    QLineEdit *priceEdit;
    QLineEdit *unitEdit;
    QLineEdit *stockEdit;
    QComboBox *statusEdit; // Replaces availableCheckBox
    QTextEdit *descriptionEdit;

    // Action buttons
    QPushButton *addNewBtn;
    QPushButton *saveBtn;
    QPushButton *deleteBtn;

    // ===== DATA =====
    HeThongQuanLy *system;
    DichVu *currentService;
    QList<DichVu *> allServices;
    QList<DichVu *> displayedServices; // Filtered and sorted
    bool isEditMode;

    // ===== HELPER METHODS =====
    QString generateNextServiceCode();
    void sortServices();
    void filterServices();
};

#endif // SERVICEMANAGEMENTWIDGET_H
