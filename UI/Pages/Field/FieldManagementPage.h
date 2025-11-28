/**
 * @file FieldManagementPage.h
 * @brief Trang quản lý sân bóng với layout 60/40 (data/form)
 * @details Week 3 Day 14 - Field Management CRUD với UI redesign
 *
 * @author Football Field Management System
 * @date 2025-11-12
 */

#ifndef FIELDMANAGEMENTPAGE_H
#define FIELDMANAGEMENTPAGE_H

#include <QWidget>
#include <QSplitter>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include "../../Core/QuanLy/QuanLySan.h"

/**
 * @class FieldManagementPage
 * @brief Trang quản lý sân bóng với layout 60/40
 * Bên trái (60%): Search bar + filters + 4 stats cards + table
 * Bên phải (40%): Detail form với actions
 */
class FieldManagementPage : public QWidget
{
    Q_OBJECT

public:
    explicit FieldManagementPage(QWidget *parent = nullptr);
    ~FieldManagementPage();

    /**
     * @brief Load dữ liệu từ QuanLySan
     */
    void loadData();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    // Left panel: Filters & Table
    void onSearchTextChanged();
    void onFilterChanged();
    void onTableRowClicked(int row, int column);
    void onStatsCardClicked(); // 4 stats cards filter

    // Right panel: Actions
    void onAddNewFieldClicked();
    void onSaveFieldClicked();
    void onMaintenanceFieldClicked();
    void onActivateFieldClicked(); // Kích hoạt lại sân sau bảo trì
    void onDeleteFieldClicked();

private:
    // Core manager
    QuanLySan *quanLySan;

    // ===== MAIN LAYOUT =====
    QSplitter *mainSplitter; // 60/40 split

    // ===== LEFT PANEL (60%) - DATA =====
    QWidget *leftPanel;

    // Search bar
    QLineEdit *searchEdit;

    // Filters
    QComboBox *trangThaiCombo; // Bộ lọc trạng thái sân
    QComboBox *loaiSanCombo;
    QComboBox *khuVucCombo;

    // 4 Stats cards (clickable)
    QFrame *totalCard;
    QFrame *activeCard;
    QFrame *maintenanceCard;
    QFrame *suspendedCard;

    QLabel *totalLabel;
    QLabel *activeLabel;
    QLabel *maintenanceLabel;
    QLabel *suspendedLabel;

    // Table (7 columns)
    QTableWidget *dataTable;
    QString selectedFieldId; // Mã sân được chọn

    // ===== RIGHT PANEL (40%) - DETAIL FORM =====
    QWidget *rightPanel;

    // Form fields (readonly by default)
    QLineEdit *tenSanEdit;
    QComboBox *loaiSanDetailCombo;
    QComboBox *khuVucDetailCombo;
    QComboBox *trangThaiDetailCombo; // Trạng thái trong form chi tiết
    QSpinBox *giaThueSpinBox;
    QTextEdit *ghiChuEdit;

    // Action buttons
    QPushButton *addNewBtn;
    QPushButton *saveBtn;
    QPushButton *maintenanceBtn;
    QPushButton *activateBtn; // Nút kích hoạt lại sân
    QPushButton *deleteBtn;

    // Current filter state
    enum StatsFilter
    {
        FILTER_ALL,
        FILTER_ACTIVE,
        FILTER_MAINTENANCE,
        FILTER_SUSPENDED
    };
    StatsFilter currentStatsFilter;

    // UI Setup methods
    void setupUI();
    void setupLeftPanel();
    void setupRightPanel();
    void setupConnections();
    void applyStyles();

    // Data loading methods
    void loadTableData();
    void updateStatsCards();

    // Helper methods
    void populateTable(const std::vector<San *> &fields);
    std::vector<San *> getFilteredFields();
    void setFormMode(bool isNew, bool isEditing);
    void clearForm();
    void loadFieldToForm(San *san);
    San *getSelectedField();

    // Auto-generate field name
    void generateFieldName();

signals:
    void dataChanged();
};

#endif // FIELDMANAGEMENTPAGE_H
