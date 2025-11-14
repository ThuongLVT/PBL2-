/**
 * @file FieldDialog.cpp
 * @brief Implementation của FieldDialog
 * @details Week 3 Day 14 - Field Add/Edit Dialog
 *
 * @author Football Field Management System
 * @date 2025-11-12
 */

#include "FieldDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>

FieldDialog::FieldDialog(QWidget *parent)
    : QDialog(parent),
      tenSanEdit(nullptr),
      loaiSanCombo(nullptr),
      khuVucCombo(nullptr),
      giaThueSpinBox(nullptr),
      saveBtn(nullptr),
      cancelBtn(nullptr)
{
    setupUI();
    setupConnections();
    applyStyles();

    setFixedWidth(450);
    setFixedHeight(300);
}

FieldDialog::~FieldDialog()
{
}

// ========== UI SETUP ==========

void FieldDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // Form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignRight);
    formLayout->setSpacing(16);

    // Tên sân
    tenSanEdit = new QLineEdit();
    tenSanEdit->setPlaceholderText("Nhập tên sân...");
    tenSanEdit->setFixedHeight(40);
    formLayout->addRow("Tên sân:", tenSanEdit);

    // Loại sân
    loaiSanCombo = new QComboBox();
    loaiSanCombo->addItem("5 người", static_cast<int>(LoaiSan::SAN_5));
    loaiSanCombo->addItem("7 người", static_cast<int>(LoaiSan::SAN_7));
    loaiSanCombo->setFixedHeight(40);
    formLayout->addRow("Loại sân:", loaiSanCombo);

    // Khu vực
    khuVucCombo = new QComboBox();
    khuVucCombo->addItem("A", static_cast<int>(KhuVuc::A));
    khuVucCombo->addItem("B", static_cast<int>(KhuVuc::B));
    khuVucCombo->addItem("C", static_cast<int>(KhuVuc::C));
    khuVucCombo->addItem("D", static_cast<int>(KhuVuc::D));
    khuVucCombo->setFixedHeight(40);
    formLayout->addRow("Khu vực:", khuVucCombo);

    // Giá thuê
    giaThueSpinBox = new QSpinBox();
    giaThueSpinBox->setMinimum(0);
    giaThueSpinBox->setMaximum(10000000);
    giaThueSpinBox->setSingleStep(50000);
    giaThueSpinBox->setValue(200000);
    giaThueSpinBox->setSuffix(" VNĐ");
    giaThueSpinBox->setFixedHeight(40);
    giaThueSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    formLayout->addRow("Giá thuê:", giaThueSpinBox);

    mainLayout->addLayout(formLayout);
    mainLayout->addStretch();

    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);

    saveBtn = new QPushButton("Lưu");
    saveBtn->setObjectName("primaryButton");
    saveBtn->setFixedHeight(40);
    saveBtn->setFixedWidth(120);

    cancelBtn = new QPushButton("Huỷ");
    cancelBtn->setObjectName("secondaryButton");
    cancelBtn->setFixedHeight(40);
    cancelBtn->setFixedWidth(120);

    buttonLayout->addStretch();
    buttonLayout->addWidget(saveBtn);
    buttonLayout->addWidget(cancelBtn);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // Set default price based on field type
    onLoaiSanChanged(0);
}

void FieldDialog::setupConnections()
{
    connect(loaiSanCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FieldDialog::onLoaiSanChanged);
    connect(saveBtn, &QPushButton::clicked, this, &FieldDialog::onSaveClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &FieldDialog::onCancelClicked);
}

void FieldDialog::applyStyles()
{
    // Styles are handled by app.qss
}

// ========== GETTERS ==========

QString FieldDialog::getTenSan() const
{
    return tenSanEdit->text().trimmed();
}

LoaiSan FieldDialog::getLoaiSan() const
{
    return static_cast<LoaiSan>(loaiSanCombo->currentData().toInt());
}

KhuVuc FieldDialog::getKhuVuc() const
{
    return static_cast<KhuVuc>(khuVucCombo->currentData().toInt());
}

double FieldDialog::getGiaThue() const
{
    return giaThueSpinBox->value();
}

// ========== SETTERS ==========

void FieldDialog::setData(San *san)
{
    if (!san)
        return;

    tenSanEdit->setText(QString::fromStdString(san->layTenSan()));

    // Set loai san
    int loaiIndex = loaiSanCombo->findData(static_cast<int>(san->layLoaiSan()));
    if (loaiIndex >= 0)
    {
        loaiSanCombo->setCurrentIndex(loaiIndex);
    }

    // Set khu vuc
    int khuVucIndex = khuVucCombo->findData(static_cast<int>(san->layKhuVuc()));
    if (khuVucIndex >= 0)
    {
        khuVucCombo->setCurrentIndex(khuVucIndex);
    }

    // Set gia thue
    giaThueSpinBox->setValue(static_cast<int>(san->layGiaThueGio()));
}

// ========== SLOTS ==========

void FieldDialog::onLoaiSanChanged(int /*index*/)
{
    LoaiSan loai = getLoaiSan();

    // Auto-set price based on field type
    switch (loai)
    {
    case LoaiSan::SAN_5:
        giaThueSpinBox->setValue(200000);
        break;
    case LoaiSan::SAN_7:
        giaThueSpinBox->setValue(400000);
        break;
    }
}

void FieldDialog::onSaveClicked()
{
    if (validateInput())
    {
        accept();
    }
}

void FieldDialog::onCancelClicked()
{
    reject();
}

// ========== VALIDATION ==========

bool FieldDialog::validateInput()
{
    // Check tên sân
    if (tenSanEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập tên sân!");
        tenSanEdit->setFocus();
        return false;
    }

    // Check giá thuê
    if (giaThueSpinBox->value() <= 0)
    {
        QMessageBox::warning(this, "Lỗi", "Giá thuê phải lớn hơn 0!");
        giaThueSpinBox->setFocus();
        return false;
    }

    return true;
}
