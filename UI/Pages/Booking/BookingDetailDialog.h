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
#include <QTableWidget>
#include <QGroupBox>
#include <QFrame>
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
    void setupHeader(QVBoxLayout *parentLayout);
    void setupInfoSection(QVBoxLayout *parentLayout);
    void setupServicesSection(QVBoxLayout *parentLayout);
    void setupPaymentSection(QVBoxLayout *parentLayout);
    void populateForm();
    void loadServices();
    void updateTotalPrice();
    QString formatCurrency(double amount) const;
    QString getStatusText(TrangThaiDatSan status) const;

private:
    DatSan *currentBooking;
    HeThongQuanLy *system;
    MangDong<San *> fields;

    // Header
    QLabel *headerIdLabel;
    QLabel *headerStatusLabel;

    // Info section
    QLabel *customerNameLabel;
    QLabel *customerPhoneLabel;
    QLabel *customerTierLabel;
    QLabel *fieldNameLabel;
    QLabel *dateLabel;
    QLabel *timeLabel;
    QLabel *noteLabel;
    QPushButton *rescheduleBtn;
    QPushButton *cancelBtn;

    // Services section
    QTableWidget *serviceTable;
    QPushButton *addServiceBtn;

    // Payment section
    QLabel *fieldPriceLabel;
    QLabel *servicePriceLabel;
    QLabel *discountLabel;
    QLabel *depositLabel;
    QLabel *totalLabel;
    QLabel *toPayLabel;
    QPushButton *payNowBtn;
    QPushButton *closeBtn;
};

#endif // BOOKINGDETAILDIALOG_H
