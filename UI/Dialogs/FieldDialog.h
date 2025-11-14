/**
 * @file FieldDialog.h
 * @brief Dialog thêm/sửa sân bóng
 * @details Week 3 Day 14 - Field Add/Edit Dialog
 *
 * @author Football Field Management System
 * @date 2025-11-12
 */

#ifndef FIELDDIALOG_H
#define FIELDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include "../../Core/Models/San.h"

/**
 * @class FieldDialog
 * @brief Dialog cho thêm hoặc sửa sân bóng
 */
class FieldDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FieldDialog(QWidget *parent = nullptr);
    ~FieldDialog();

    // Getters
    QString getTenSan() const;
    LoaiSan getLoaiSan() const;
    KhuVuc getKhuVuc() const;
    double getGiaThue() const;

    // Setters (for edit mode)
    void setData(San *san);

private slots:
    void onLoaiSanChanged(int index);
    void onSaveClicked();
    void onCancelClicked();

private:
    // Form fields
    QLineEdit *tenSanEdit;
    QComboBox *loaiSanCombo;
    QComboBox *khuVucCombo;
    QSpinBox *giaThueSpinBox;

    // Buttons
    QPushButton *saveBtn;
    QPushButton *cancelBtn;

    // Setup methods
    void setupUI();
    void setupConnections();
    void applyStyles();

    // Validation
    bool validateInput();
};

#endif // FIELDDIALOG_H
