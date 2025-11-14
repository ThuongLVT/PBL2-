/**
 * @file MaintenanceDialog.h
 * @brief Dialog xác nhận đưa sân vào bảo trì
 */

#ifndef MAINTENANCEDIALOG_H
#define MAINTENANCEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include "../../Core/Models/San.h"

class MaintenanceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MaintenanceDialog(San *san, QWidget *parent = nullptr);
    ~MaintenanceDialog();

    /**
     * @brief Lấy lý do bảo trì
     */
    QString getMaintenanceReason() const;

private slots:
    void onConfirmClicked();
    void onCancelClicked();

private:
    San *m_san;

    // UI Components
    QLabel *maSanLabel;
    QLabel *tenSanLabel;
    QLabel *loaiSanLabel;
    QLabel *khuVucLabel;
    QTextEdit *reasonEdit;
    QPushButton *confirmBtn;
    QPushButton *cancelBtn;

    void setupUI();
    void applyStyles();
};

#endif // MAINTENANCEDIALOG_H
