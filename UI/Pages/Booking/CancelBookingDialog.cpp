/**
 * @file CancelBookingDialog.cpp
 * @brief Implementation of CancelBookingDialog
 */

#include "CancelBookingDialog.h"
#include <QMessageBox>
#include <QGroupBox>
#include <iomanip>
#include <sstream>

CancelBookingDialog::CancelBookingDialog(DatSan *booking, QWidget *parent)
    : QDialog(parent),
      currentBooking(booking),
      refundDeposit(false)
{
    setWindowTitle("❌ Hủy đặt sân");
    setMinimumSize(500, 450);
    setModal(true);

    setupUI();
}

CancelBookingDialog::~CancelBookingDialog()
{
    // Qt handles cleanup
}

void CancelBookingDialog::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    // Title
    QLabel *titleLabel = new QLabel("❌ HỦY ĐẶT SÂN");
    titleLabel->setStyleSheet(
        "font-size: 18px; "
        "font-weight: bold; "
        "color: #dc2626; "
        "padding: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Booking Info
    QGroupBox *infoGroup = new QGroupBox("📋 Thông tin đơn đặt sân");
    QVBoxLayout *infoLayout = new QVBoxLayout(infoGroup);
    infoLayout->setSpacing(8);

    QString bookingInfo = QString(
                              "Mã đơn: <b>%1</b><br>"
                              "Khách hàng: <b>%2</b><br>"
                              "Sân: <b>%3</b><br>"
                              "Ngày: <b>%4</b><br>"
                              "Giờ: <b>%5 - %6</b>")
                              .arg(QString::fromStdString(currentBooking->getMaDatSan()))
                              .arg(currentBooking->getKhachHang() ? QString::fromStdString(currentBooking->getKhachHang()->getHoTen()) : "N/A")
                              .arg(currentBooking->getSan() ? QString::fromStdString(currentBooking->getSan()->getTenSan()) : "N/A")
                              .arg(QString::fromStdString(currentBooking->getThoiGianDat().toString()))
                              .arg(QString::fromStdString(currentBooking->getKhungGio().getGioBatDau().toString()))
                              .arg(QString::fromStdString(currentBooking->getKhungGio().getGioKetThuc().toString()));

    bookingInfoLabel = new QLabel(bookingInfo);
    bookingInfoLabel->setWordWrap(true);
    bookingInfoLabel->setStyleSheet("font-size: 13px; padding: 10px;");
    infoLayout->addWidget(bookingInfoLabel);

    depositAmountLabel = new QLabel(QString("💰 Tiền cọc: <b style='color: #16a34a;'>%1</b>")
                                        .arg(formatCurrency(currentBooking->getTienCoc())));
    depositAmountLabel->setStyleSheet("font-size: 14px; padding: 5px; font-weight: 600;");
    infoLayout->addWidget(depositAmountLabel);

    mainLayout->addWidget(infoGroup);

    // Deposit Handling Group
    QGroupBox *depositGroup = new QGroupBox("⚙️ Xử lý tiền cọc");
    QVBoxLayout *depositLayout = new QVBoxLayout(depositGroup);
    depositLayout->setSpacing(15);

    this->depositGroup = new QButtonGroup(this);

    refundRadio = new QRadioButton("✅ Hoàn cọc 100%");
    refundRadio->setStyleSheet(
        "QRadioButton { "
        "font-size: 14px; "
        "font-weight: 600; "
        "color: #16a34a; "
        "padding: 8px; "
        "} "
        "QRadioButton::indicator { "
        "width: 18px; "
        "height: 18px; "
        "}");
    QLabel *refundHint = new QLabel("    ↳ Khách hủy sớm, lý do hợp lý");
    refundHint->setStyleSheet("font-size: 12px; color: #6b7280; margin-left: 30px;");

    forfeitRadio = new QRadioButton("❌ Mất cọc 100%");
    forfeitRadio->setStyleSheet(
        "QRadioButton { "
        "font-size: 14px; "
        "font-weight: 600; "
        "color: #dc2626; "
        "padding: 8px; "
        "} "
        "QRadioButton::indicator { "
        "width: 18px; "
        "height: 18px; "
        "}");
    QLabel *forfeitHint = new QLabel("    ↳ Khách hủy gấp/bom sân");
    forfeitHint->setStyleSheet("font-size: 12px; color: #6b7280; margin-left: 30px;");

    this->depositGroup->addButton(refundRadio, 1);
    this->depositGroup->addButton(forfeitRadio, 2);
    refundRadio->setChecked(true); // Default

    depositLayout->addWidget(refundRadio);
    depositLayout->addWidget(refundHint);
    depositLayout->addSpacing(10);
    depositLayout->addWidget(forfeitRadio);
    depositLayout->addWidget(forfeitHint);

    mainLayout->addWidget(depositGroup);

    // Reason Group
    QGroupBox *reasonGroup = new QGroupBox("📝 Lý do hủy (bắt buộc)");
    QVBoxLayout *reasonLayout = new QVBoxLayout(reasonGroup);

    reasonEdit = new QTextEdit();
    reasonEdit->setPlaceholderText(
        "VD:\n"
        "- Khách báo trước 1 ngày, lý do hợp lý\n"
        "- Khách hủy gấp, còn 1h không kịp cho thuê lại\n"
        "- Không đến không báo (bom sân)\n"
        "- Trời mưa to, sân không sử dụng được");
    reasonEdit->setMinimumHeight(100);
    reasonEdit->setMaximumHeight(150);
    reasonEdit->setStyleSheet(
        "QTextEdit { "
        "border: 2px solid #e5e7eb; "
        "border-radius: 6px; "
        "padding: 10px; "
        "font-size: 13px; "
        "background-color: white; "
        "} "
        "QTextEdit:focus { "
        "border: 2px solid #dc2626; "
        "}");

    reasonLayout->addWidget(reasonEdit);
    mainLayout->addWidget(reasonGroup);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);
    buttonLayout->addStretch();

    cancelBtn = new QPushButton("🚪 Hủy bỏ");
    cancelBtn->setMinimumSize(120, 45);
    cancelBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #6b7280; "
        "color: white; "
        "border: none; "
        "border-radius: 6px; "
        "font-size: 14px; "
        "font-weight: 600; "
        "} "
        "QPushButton:hover { "
        "background-color: #4b5563; "
        "}");
    connect(cancelBtn, &QPushButton::clicked, this, &CancelBookingDialog::onCancelClicked);

    confirmBtn = new QPushButton("✅ Xác nhận hủy");
    confirmBtn->setMinimumSize(140, 45);
    confirmBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #dc2626; "
        "color: white; "
        "border: none; "
        "border-radius: 6px; "
        "font-size: 14px; "
        "font-weight: 600; "
        "} "
        "QPushButton:hover { "
        "background-color: #b91c1c; "
        "}");
    connect(confirmBtn, &QPushButton::clicked, this, &CancelBookingDialog::onConfirmClicked);

    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addWidget(confirmBtn);

    mainLayout->addSpacing(10);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // Dialog styling
    setStyleSheet(
        "QDialog { "
        "background-color: #f9fafb; "
        "} "
        "QGroupBox { "
        "background-color: white; "
        "border: 2px solid #e5e7eb; "
        "border-radius: 8px; "
        "padding: 15px; "
        "font-size: 14px; "
        "font-weight: bold; "
        "color: #1f2937; "
        "margin-top: 10px; "
        "} "
        "QGroupBox::title { "
        "subcontrol-origin: margin; "
        "subcontrol-position: top left; "
        "padding: 5px 10px; "
        "}");
}

void CancelBookingDialog::onConfirmClicked()
{
    // Validate reason
    QString reason = reasonEdit->toPlainText().trimmed();
    if (reason.isEmpty())
    {
        QMessageBox::warning(this, "Lỗi",
                             "⚠️ Vui lòng ghi lý do hủy!\n\n"
                             "Lý do hủy là bắt buộc để tra cứu và giải quyết tranh chấp sau này.");
        reasonEdit->setFocus();
        return;
    }

    if (reason.length() < 10)
    {
        QMessageBox::warning(this, "Lỗi",
                             "⚠️ Lý do hủy quá ngắn!\n\n"
                             "Vui lòng nhập ít nhất 10 ký tự để mô tả rõ lý do.");
        reasonEdit->setFocus();
        return;
    }

    // Get deposit choice
    refundDeposit = refundRadio->isChecked();
    cancelReason = reason;

    // Confirm action
    QString confirmMsg;
    if (refundDeposit)
    {
        confirmMsg = QString(
                         "✅ Xác nhận hoàn cọc?\n\n"
                         "Sẽ hoàn lại %1 cho khách hàng.\n"
                         "Lý do: %2")
                         .arg(formatCurrency(currentBooking->getTienCoc()))
                         .arg(cancelReason);
    }
    else
    {
        confirmMsg = QString(
                         "❌ Xác nhận giữ cọc?\n\n"
                         "Sẽ giữ lại %1 (tính vào doanh thu).\n"
                         "Lý do: %2")
                         .arg(formatCurrency(currentBooking->getTienCoc()))
                         .arg(cancelReason);
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Xác nhận", confirmMsg,
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        accept();
    }
}

void CancelBookingDialog::onCancelClicked()
{
    reject();
}

QString CancelBookingDialog::formatCurrency(double amount) const
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(0) << amount;
    return QString::fromStdString(ss.str()) + " VND";
}
