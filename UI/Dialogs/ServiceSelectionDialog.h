#ifndef SERVICESELECTIONDIALOG_H
#define SERVICESELECTIONDIALOG_H

#include <QDialog>
#include <QMap>
#include <string>
#include <vector>

class DichVu;
class QLineEdit;
class QComboBox;
class QGridLayout;
class QLabel;
class QScrollArea;
class QTableWidget;

class ServiceSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServiceSelectionDialog(QWidget *parent = nullptr);
    ~ServiceSelectionDialog();

    // Get selected services: Map<ServiceCode, Quantity>
    QMap<std::string, int> getSelectedServices() const;

private slots:
    void onSearchTextChanged(const QString &text);
    void onFilterChanged(int index);
    void onServiceCardClicked(DichVu *service);
    void onTableDoubleClicked(int row, int column);
    void onRemoveServiceClicked();
    void onConfirmClicked();

private:
    void setupUI();
    void loadServices();
    void updateGrid();
    void updateTable();
    void openQuantityDialog(DichVu *service, int currentQty);

    // Helper to create a service card
    QWidget *createServiceCard(DichVu *service);

    // UI Elements
    QLineEdit *searchEdit;
    QComboBox *filterCombo;
    QTableWidget *selectedTable; // Top Frame
    QScrollArea *scrollArea;     // Bottom Frame
    QWidget *gridContainer;
    QGridLayout *gridLayout;

    // Data
    std::vector<DichVu *> allServices;
    std::vector<DichVu *> displayedServices;
    QMap<std::string, int> selectedQuantities; // ServiceID -> Quantity
};

#endif // SERVICESELECTIONDIALOG_H
