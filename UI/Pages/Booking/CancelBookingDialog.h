/**
 * @file CancelBookingDialog.h
 * @brief Dialog for canceling booking with reason
 * @details Allows staff to choose refund/forfeit deposit and enter cancellation reason
 *
 * @author Football Field Management System
 * @date 2025-11-18
 */

#ifndef CANCELBOOKINGDIALOG_H
#define CANCELBOOKINGDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QTextEdit>
#include <QButtonGroup>
#include "../../../Core/Models/DatSan.h"

/**
 * @class CancelBookingDialog
 * @brief Simple dialog for canceling bookings with deposit handling
 */
class CancelBookingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CancelBookingDialog(DatSan *booking, QWidget *parent = nullptr);
    ~CancelBookingDialog();

    bool isRefundDeposit() const { return refundDeposit; }
    QString getCancelReason() const { return cancelReason; }

private slots:
    void onConfirmClicked();
    void onCancelClicked();

private:
    void setupUI();
    QString formatCurrency(double amount) const;

private:
    DatSan *currentBooking;

    // UI Elements
    QVBoxLayout *mainLayout;
    QLabel *bookingInfoLabel;
    QLabel *depositAmountLabel;

    QRadioButton *refundRadio;
    QRadioButton *forfeitRadio;
    QButtonGroup *depositGroup;

    QTextEdit *reasonEdit;

    QPushButton *confirmBtn;
    QPushButton *cancelBtn;

    // Result
    bool refundDeposit;
    QString cancelReason;
};

#endif // CANCELBOOKINGDIALOG_H
