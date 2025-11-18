/**
 * @file BookingDetailDialog.h
 * @brief Dialog for viewing and editing booking details
 * @details Handles edit, cancel, and payment actions
 *
 * @author Football Field Management System
 * @date 2025-11-18
 */

#ifndef BOOKINGDETAILDIALOG_H
#define BOOKINGDETAILDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QDateEdit>
#include <QTimeEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QGroupBox>
#include "../../../Core/QuanLy/HeThongQuanLy.h"
#include "../../../Core/Models/DatSan.h"
#include "../../../Core/Models/KhachHang.h"
#include "../../../Core/Models/San.h"

/**
 * @class BookingDetailDialog
 * @brief Dialog for booking details with edit/cancel/payment actions
 */
class BookingDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookingDetailDialog(DatSan *booking, QWidget *parent = nullptr);
    ~BookingDetailDialog();

private slots:
    void onSaveClicked();
    void onCancelBookingClicked();
    void onPaymentClicked();
    void onAddServiceClicked();
    void onRemoveServiceClicked();
    void onPayDepositClicked();

private:
    void setupUI();
    void setupInfoSection();
    void setupServicesSection();
    void setupActionButtons();
    void populateForm();
    void loadServices();
    void updateTotalPrice();
    QString formatCurrency(double amount) const;
    QString getStatusText(TrangThaiDatSan status) const;

private:
    DatSan *currentBooking;
    HeThongQuanLy *system;
    MangDong<San *> fields;

    // Main layout
    QVBoxLayout *mainLayout;

    // Info section
    QLabel *bookingIdLabel;
    QLabel *customerLabel;
    QLabel *phoneLabel;
    QComboBox *fieldCombo;
    QDateEdit *dateEdit;
    QTimeEdit *fromTimeEdit;
    QTimeEdit *toTimeEdit;
    QComboBox *statusCombo;
    QTextEdit *noteEdit;
    QLabel *totalPriceLabel;
    QLabel *depositAmountLabel;
    QLabel *depositStatusLabel;
    QPushButton *payDepositBtn;

    // Services section
    QListWidget *serviceList;
    QPushButton *addServiceBtn;
    QPushButton *removeServiceBtn;

    // Action buttons
    QPushButton *saveBtn;
    QPushButton *cancelBookingBtn;
    QPushButton *paymentBtn;
    QPushButton *closeBtn;
};

#endif // BOOKINGDETAILDIALOG_H
