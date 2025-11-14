/**
 * @file MaintenanceDialog.cpp
 * @brief Implementation của MaintenanceDialog
 */

#include "MaintenanceDialog.h"
#include <QMessageBox>

MaintenanceDialog::MaintenanceDialog(San *san, QWidget *parent)
    : QDialog(parent), m_san(san)
{
    setWindowTitle("⚙️ XÁC NHẬN ĐƯA SÂN VÀO BẢO TRÌ");
    setModal(true);
    setMinimumWidth(500);

    setupUI();
    applyStyles();
}

MaintenanceDialog::~MaintenanceDialog()
{
}

void MaintenanceDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Title
    QLabel *titleLabel = new QLabel("⚙️ XÁC NHẬN ĐƯA SÂN VÀO BẢO TRÌ");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #f59e0b;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Field info (readonly)
    QFormLayout *infoLayout = new QFormLayout();
    infoLayout->setSpacing(15);
    infoLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    maSanLabel = new QLabel(QString::fromStdString(m_san->layMaSan()));
    maSanLabel->setStyleSheet("font-weight: bold; color: #1f2937;");

    tenSanLabel = new QLabel(QString::fromStdString(m_san->layTenSan()));
    tenSanLabel->setStyleSheet("font-weight: bold; color: #1f2937;");

    loaiSanLabel = new QLabel(QString::fromStdString(m_san->layTenLoaiSan()));
    loaiSanLabel->setStyleSheet("color: #6b7280;");

    khuVucLabel = new QLabel(QString::fromStdString(m_san->layTenKhuVuc()));
    khuVucLabel->setStyleSheet("color: #6b7280;");

    infoLayout->addRow("Mã sân:", maSanLabel);
    infoLayout->addRow("Tên sân:", tenSanLabel);
    infoLayout->addRow("Loại sân:", loaiSanLabel);
    infoLayout->addRow("Khu vực:", khuVucLabel);

    mainLayout->addLayout(infoLayout);

    // Separator
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("background-color: #e5e7eb;");
    mainLayout->addWidget(line);

    // Reason input
    QLabel *reasonLabel = new QLabel("Lý do bảo trì:");
    reasonLabel->setStyleSheet("font-weight: bold; color: #374151;");
    mainLayout->addWidget(reasonLabel);

    reasonEdit = new QTextEdit();
    reasonEdit->setPlaceholderText("Nhập lý do bảo trì (ví dụ: Thay cỏ nhân tạo, Sửa chữa đèn chiếu sáng...)");
    reasonEdit->setMinimumHeight(100);
    reasonEdit->setMaximumHeight(150);
    mainLayout->addWidget(reasonEdit);

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);

    confirmBtn = new QPushButton("✓ Xác nhận bảo trì");
    cancelBtn = new QPushButton("✗ Huỷ");

    btnLayout->addStretch();
    btnLayout->addWidget(confirmBtn);
    btnLayout->addWidget(cancelBtn);

    mainLayout->addLayout(btnLayout);

    // Connections
    connect(confirmBtn, &QPushButton::clicked, this, &MaintenanceDialog::onConfirmClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &MaintenanceDialog::onCancelClicked);

    setLayout(mainLayout);
}

void MaintenanceDialog::applyStyles()
{
    setStyleSheet(R"(
        QDialog {
            background-color: white;
        }
        QTextEdit {
            border: 1px solid #d1d5db;
            border-radius: 6px;
            padding: 8px;
            font-size: 13px;
        }
        QTextEdit:focus {
            border: 2px solid #3b82f6;
        }
    )");

    confirmBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #f59e0b;
            color: white;
            padding: 10px 24px;
            border-radius: 6px;
            font-weight: bold;
            border: none;
            min-width: 150px;
        }
        QPushButton:hover {
            background-color: #d97706;
        }
    )");

    cancelBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #6b7280;
            color: white;
            padding: 10px 24px;
            border-radius: 6px;
            font-weight: bold;
            border: none;
            min-width: 100px;
        }
        QPushButton:hover {
            background-color: #4b5563;
        }
    )");
}

void MaintenanceDialog::onConfirmClicked()
{
    QString reason = reasonEdit->toPlainText().trimmed();

    if (reason.isEmpty())
    {
        QMessageBox::warning(this, "Thiếu thông tin",
                             "Vui lòng nhập lý do bảo trì!");
        reasonEdit->setFocus();
        return;
    }

    accept();
}

void MaintenanceDialog::onCancelClicked()
{
    reject();
}

QString MaintenanceDialog::getMaintenanceReason() const
{
    return reasonEdit->toPlainText().trimmed();
}
