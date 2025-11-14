/**
 * @file AddCustomerDialog.h
 * @brief Dialog nhanh để thêm khách hàng mới
 * @details Dialog đơn giản chỉ cần SĐT, Tên, Địa chỉ
 * 
 * @author khninh22 - Service Module
 * @date 2025-11-15
 */

#ifndef ADDCUSTOMERDIALOG_H
#define ADDCUSTOMERDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include "../../Core/Models/KhachHang.h"

/**
 * @class AddCustomerDialog
 * @brief Dialog đơn giản để thêm khách hàng mới nhanh
 * 
 * Chỉ cần:
 * - Số điện thoại (bắt buộc)
 * - Họ tên (bắt buộc)
 * - Địa chỉ (tùy chọn)
 */
class AddCustomerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddCustomerDialog(QWidget *parent = nullptr);
    ~AddCustomerDialog();

    /**
     * @brief Lấy khách hàng mới đã tạo
     * @return Con trỏ KhachHang mới, hoặc nullptr nếu hủy
     */
    KhachHang *getNewCustomer() const;

private slots:
    /**
     * @brief Xử lý khi click nút Thêm
     */
    void onAddClicked();

    /**
     * @brief Xử lý khi click nút Hủy
     */
    void onCancelClicked();

private:
    /**
     * @brief Setup giao diện
     */
    void setupUI();

    /**
     * @brief Validate input
     */
    bool validateInput();

    // ===== UI COMPONENTS =====
    QVBoxLayout *mainLayout;
    
    QLineEdit *phoneEdit;
    QLineEdit *nameEdit;
    QLineEdit *addressEdit;
    
    QFrame *buttonFrame;
    QHBoxLayout *buttonLayout;
    QPushButton *addBtn;
    QPushButton *cancelBtn;

    // ===== DATA =====
    KhachHang *newCustomer;
};

#endif // ADDCUSTOMERDIALOG_H
