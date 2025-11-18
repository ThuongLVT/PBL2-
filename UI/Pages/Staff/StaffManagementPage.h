/**
 * @file StaffManagementPage.h
 * @brief Staff management page (Admin only)
 * @details Similar to Customer management with form + table
 *          Only accessible by Admin users
 *
 * @author Football Field Management System
 * @date 2025-11-18
 */

#ifndef STAFFMANAGEMENTPAGE_H
#define STAFFMANAGEMENTPAGE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QFrame>
#include <QRadioButton>
#include <QButtonGroup>
#include <QDateEdit>
#include "../../Core/QuanLy/HeThongQuanLy.h"
#include "../../Core/QuanLy/QuanLyNhanVien.h"
#include "../../Core/Models/NhanVien.h"
#include "../../Core/Models/QuanTriVien.h"

/**
 * @class StaffManagementPage
 * @brief Staff CRUD management (Admin only)
 */
class StaffManagementPage : public QWidget
{
    Q_OBJECT

public:
    explicit StaffManagementPage(QWidget *parent = nullptr);
    ~StaffManagementPage();

    /**
     * @brief Set current admin user for access control
     * @param admin Pointer to admin user
     */
    void setCurrentAdmin(QuanTriVien *admin);

    /**
     * @brief Load all staff from Core
     */
    void loadStaff();

private slots:
    void onSearchTextChanged(const QString &text);
    void onStaffRowClicked(int row);
    void onAddNewClicked();
    void onSaveClicked();
    void onDeleteClicked();
    void onPrintClicked();

private:
    void setupUI();
    void setupConnections();
    void clearForm();
    void populateForm(NhanVien *staff);
    bool validateForm();
    
    // Layouts
    QHBoxLayout *mainLayout;
    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;
    
    // LEFT: Table (70%)
    QFrame *tableFrame;
    QLineEdit *searchEdit;
    QPushButton *printBtn;
    QTableWidget *staffTable;
    
    // RIGHT: Form (30%)
    QFrame *formFrame;
    QLineEdit *nameEdit;
    QButtonGroup *genderGroup;
    QRadioButton *maleRadio;
    QRadioButton *femaleRadio;
    QRadioButton *otherRadio;
    QDateEdit *dobEdit;
    QLineEdit *phoneEdit;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QComboBox *roleCombo;
    
    QPushButton *addNewBtn;
    QPushButton *saveBtn;
    QPushButton *deleteBtn;
    
    // Data
    HeThongQuanLy *system;
    QuanLyNhanVien *staffManager;
    QuanTriVien *currentAdmin;
    NhanVien *selectedStaff;
    bool isEditMode;
};

#endif // STAFFMANAGEMENTPAGE_H
