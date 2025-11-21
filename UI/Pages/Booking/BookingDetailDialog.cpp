/**
 * @file BookingDetailDialog.cpp
 * @brief Implementation of BookingDetailDialog
 */

#include "BookingDetailDialog.h"
#include "CancelBookingDialog.h"
#include <QMessageBox>
#include <QSpinBox>
#include <QDialog>
#include <iomanip>
#include <sstream>

BookingDetailDialog::BookingDetailDialog(DatSan *booking, QWidget *parent)
    : QDialog(parent),
      currentBooking(booking),
      system(nullptr)
{
    system = HeThongQuanLy::getInstance();
    fields = system->layDanhSachSan();

    setWindowTitle("Chi tiết đặt sân");
    setMinimumSize(600, 700);
    setModal(true);

    setupUI();
    populateForm();
    loadServices();
}

BookingDetailDialog::~BookingDetailDialog()
{
    // Qt handles cleanup
}

void BookingDetailDialog::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    // Main content: Split Info (Left) and Services (Right)
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(20);

    // Left side: Info
    QWidget *leftWidget = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    setupInfoSection(leftLayout); // Modified to take layout
    leftWidget->setLayout(leftLayout);

    // Right side: Services
    QWidget *rightWidget = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    setupServicesSection(rightLayout); // Modified to take layout
    rightWidget->setLayout(rightLayout);

    // Add to content layout with ratio
    contentLayout->addWidget(leftWidget, 6);  // 60%
    contentLayout->addWidget(rightWidget, 4); // 40%

    mainLayout->addLayout(contentLayout);
    setupActionButtons();

    setLayout(mainLayout);

    // Apply dialog styling
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
        "} "
        "QGroupBox::title { "
        "subcontrol-origin: margin; "
        "subcontrol-position: top left; "
        "padding: 5px 10px; "
        "} "
        "QLabel { "
        "color: #374151; "
        "font-size: 13px; "
        "} "
        "QLineEdit, QComboBox, QDateEdit, QTimeEdit, QTextEdit { "
        "padding: 8px 12px; "
        "border: 2px solid #e5e7eb; "
        "border-radius: 6px; "
        "background-color: white; "
        "font-size: 13px; "
        "} "
        "QLineEdit:focus, QComboBox:focus, QDateEdit:focus, QTimeEdit:focus, QTextEdit:focus { "
        "border: 2px solid #16a34a; "
        "} "
        "QPushButton { "
        "padding: 10px 20px; "
        "border: none; "
        "border-radius: 6px; "
        "font-size: 14px; "
        "font-weight: 600; "
        "} "
        "QPushButton:hover { "
        "opacity: 0.9; "
        "}");
}

void BookingDetailDialog::setupInfoSection(QVBoxLayout *parentLayout)
{
    QGroupBox *infoGroup = new QGroupBox("📋 Thông tin đặt sân");
    QFormLayout *formLayout = new QFormLayout(infoGroup);
    formLayout->setSpacing(12);
    formLayout->setContentsMargins(15, 20, 15, 15);

    // Mã đơn (read-only)
    bookingIdLabel = new QLabel("-");
    bookingIdLabel->setStyleSheet("color: #6b7280; font-weight: bold; font-size: 14px;");
    formLayout->addRow("Mã đơn:", bookingIdLabel);

    // Khách hàng (read-only)
    customerLabel = new QLabel("-");
    customerLabel->setStyleSheet("font-weight: 500;");
    formLayout->addRow("Khách hàng:", customerLabel);

    // SĐT (read-only)
    phoneLabel = new QLabel("-");
    phoneLabel->setStyleSheet("font-weight: 500;");
    formLayout->addRow("SĐT:", phoneLabel);

    // Sân
    fieldCombo = new QComboBox();
    fieldCombo->setMinimumHeight(38);
    for (int i = 0; i < fields.size(); i++)
    {
        San *field = fields[i];
        if (field)
        {
            QString label = QString("Sân %1 - %2")
                                .arg(QString::fromStdString(field->getTenSan()))
                                .arg(QString::fromStdString(field->layTenLoaiSan()));
            fieldCombo->addItem(label, QString::fromStdString(field->getMaSan()));
        }
    }
    formLayout->addRow("Sân:", fieldCombo);

    // Ngày đặt
    dateEdit = new QDateEdit();
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    dateEdit->setMinimumHeight(38);
    formLayout->addRow("Ngày đặt:", dateEdit);

    // Giờ bắt đầu
    fromTimeEdit = new QTimeEdit();
    fromTimeEdit->setDisplayFormat("HH:mm");
    fromTimeEdit->setMinimumHeight(38);
    formLayout->addRow("Từ giờ:", fromTimeEdit);

    // Giờ kết thúc
    toTimeEdit = new QTimeEdit();
    toTimeEdit->setDisplayFormat("HH:mm");
    toTimeEdit->setMinimumHeight(38);
    formLayout->addRow("Đến giờ:", toTimeEdit);

    // Trạng thái
    statusCombo = new QComboBox();
    statusCombo->addItem("🟢 Đã đặt", static_cast<int>(TrangThaiDatSan::DA_DAT));
    statusCombo->addItem("✓ Hoàn thành", static_cast<int>(TrangThaiDatSan::HOAN_THANH));
    statusCombo->addItem("❌ Đã hủy", static_cast<int>(TrangThaiDatSan::DA_HUY));
    statusCombo->setMinimumHeight(38);
    formLayout->addRow("Trạng thái:", statusCombo);

    // Ghi chú
    noteEdit = new QTextEdit();
    noteEdit->setMaximumHeight(80);
    noteEdit->setPlaceholderText("Nhập ghi chú...");
    formLayout->addRow("Ghi chú:", noteEdit);

    // Tổng tiền
    totalPriceLabel = new QLabel("0đ");
    totalPriceLabel->setStyleSheet("color: #16a34a; font-weight: bold; font-size: 18px;");
    formLayout->addRow("Tổng tiền:", totalPriceLabel);

    // ===== DEPOSIT SECTION =====
    // Tiền cọc (30%)
    depositAmountLabel = new QLabel("0đ");
    depositAmountLabel->setStyleSheet("color: #f59e0b; font-weight: bold; font-size: 16px;");
    formLayout->addRow("Tiền cọc (30%):", depositAmountLabel);

    // Trạng thái cọc
    QHBoxLayout *depositStatusLayout = new QHBoxLayout();
    depositStatusLabel = new QLabel("Chưa cọc");
    depositStatusLabel->setStyleSheet("color: #6b7280; font-weight: 500;");
    depositStatusLayout->addWidget(depositStatusLabel);

    payDepositBtn = new QPushButton("💳 Đặt cọc");
    payDepositBtn->setMinimumHeight(32);
    payDepositBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #f59e0b; "
        "color: white; "
        "padding: 6px 12px; "
        "} "
        "QPushButton:hover { "
        "background-color: #d97706; "
        "}");
    depositStatusLayout->addWidget(payDepositBtn);
    depositStatusLayout->addStretch();

    QWidget *depositStatusWidget = new QWidget();
    depositStatusWidget->setLayout(depositStatusLayout);
    formLayout->addRow("Trạng thái cọc:", depositStatusWidget);

    parentLayout->addWidget(infoGroup);
}

void BookingDetailDialog::setupServicesSection(QVBoxLayout *parentLayout)
{
    QGroupBox *serviceGroup = new QGroupBox("🍽️ Dịch vụ đã đặt");
    QVBoxLayout *serviceLayout = new QVBoxLayout(serviceGroup);
    serviceLayout->setSpacing(10);
    serviceLayout->setContentsMargins(15, 20, 15, 15);

    serviceList = new QListWidget();
    serviceList->setMinimumHeight(150);
    serviceList->setStyleSheet(
        "QListWidget { "
        "border: 2px solid #e5e7eb; "
        "border-radius: 6px; "
        "background-color: #fafafa; "
        "padding: 5px; "
        "} "
        "QListWidget::item { "
        "padding: 8px; "
        "border-bottom: 1px solid #e5e7eb; "
        "} "
        "QListWidget::item:selected { "
        "background-color: #dcfce7; "
        "color: #166534; "
        "}");
    serviceLayout->addWidget(serviceList);

    // Service action buttons
    QHBoxLayout *serviceBtnLayout = new QHBoxLayout();
    serviceBtnLayout->setSpacing(10);

    addServiceBtn = new QPushButton("➕ Thêm");
    addServiceBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #16a34a; "
        "color: white; "
        "} "
        "QPushButton:hover { "
        "background-color: #15803d; "
        "}");
    serviceBtnLayout->addWidget(addServiceBtn);

    removeServiceBtn = new QPushButton("➖ Xóa");
    removeServiceBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #dc2626; "
        "color: white; "
        "} "
        "QPushButton:hover { "
        "background-color: #b91c1c; "
        "}");
    serviceBtnLayout->addWidget(removeServiceBtn);

    serviceLayout->addLayout(serviceBtnLayout);

    parentLayout->addWidget(serviceGroup);

    // Connect service buttons
    connect(addServiceBtn, &QPushButton::clicked, this, &BookingDetailDialog::onAddServiceClicked);
    connect(removeServiceBtn, &QPushButton::clicked, this, &BookingDetailDialog::onRemoveServiceClicked);
    connect(payDepositBtn, &QPushButton::clicked, this, &BookingDetailDialog::onPayDepositClicked);
}

void BookingDetailDialog::setupActionButtons()
{
    QHBoxLayout *actionLayout = new QHBoxLayout();
    actionLayout->setSpacing(10);

    // Save button
    saveBtn = new QPushButton("💾 Lưu thay đổi");
    saveBtn->setMinimumHeight(44);
    saveBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #16a34a; "
        "color: white; "
        "} "
        "QPushButton:hover { "
        "background-color: #15803d; "
        "}");
    actionLayout->addWidget(saveBtn);

    // Payment button
    paymentBtn = new QPushButton("💰 Thanh toán");
    paymentBtn->setMinimumHeight(44);
    paymentBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #3b82f6; "
        "color: white; "
        "} "
        "QPushButton:hover { "
        "background-color: #2563eb; "
        "}");
    actionLayout->addWidget(paymentBtn);

    // Cancel booking button
    cancelBookingBtn = new QPushButton("❌ Hủy lịch");
    cancelBookingBtn->setMinimumHeight(44);
    cancelBookingBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #dc2626; "
        "color: white; "
        "} "
        "QPushButton:hover { "
        "background-color: #b91c1c; "
        "}");
    actionLayout->addWidget(cancelBookingBtn);

    // Close button
    closeBtn = new QPushButton("🚪 Đóng");
    closeBtn->setMinimumHeight(44);
    closeBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #6b7280; "
        "color: white; "
        "} "
        "QPushButton:hover { "
        "background-color: #4b5563; "
        "}");
    actionLayout->addWidget(closeBtn);

    mainLayout->addLayout(actionLayout);

    // Connect action buttons
    connect(saveBtn, &QPushButton::clicked, this, &BookingDetailDialog::onSaveClicked);
    connect(paymentBtn, &QPushButton::clicked, this, &BookingDetailDialog::onPaymentClicked);
    connect(cancelBookingBtn, &QPushButton::clicked, this, &BookingDetailDialog::onCancelBookingClicked);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void BookingDetailDialog::populateForm()
{
    if (!currentBooking)
        return;

    // Booking ID
    bookingIdLabel->setText(QString::fromStdString(currentBooking->getMaDatSan()));

    // Customer info
    KhachHang *kh = currentBooking->getKhachHang();
    if (kh)
    {
        customerLabel->setText(QString::fromStdString(kh->getHoTen()));
        phoneLabel->setText(QString::fromStdString(kh->getSoDienThoai()));
    }

    // Field
    San *san = currentBooking->getSan();
    if (san)
    {
        QString maSan = QString::fromStdString(san->getMaSan());
        for (int i = 0; i < fieldCombo->count(); i++)
        {
            if (fieldCombo->itemData(i).toString() == maSan)
            {
                fieldCombo->setCurrentIndex(i);
                break;
            }
        }
    }

    // Date and time
    NgayGio thoiGian = currentBooking->getThoiGianDat();
    dateEdit->setDate(QDate(thoiGian.getNam(), thoiGian.getThang(), thoiGian.getNgay()));
    fromTimeEdit->setTime(QTime(thoiGian.getGio(), thoiGian.getPhut()));
    toTimeEdit->setTime(QTime((thoiGian.getGio() + 1) % 24, thoiGian.getPhut()));

    // Status
    int statusValue = static_cast<int>(currentBooking->getTrangThai());
    for (int i = 0; i < statusCombo->count(); i++)
    {
        if (statusCombo->itemData(i).toInt() == statusValue)
        {
            statusCombo->setCurrentIndex(i);
            break;
        }
    }

    // Note
    noteEdit->setPlainText(QString::fromStdString(currentBooking->getGhiChu()));

    // Total price
    totalPriceLabel->setText(formatCurrency(currentBooking->getTongTien()));

    // ===== DEPOSIT INFO =====
    depositAmountLabel->setText(formatCurrency(currentBooking->getTienCoc()));

    TrangThaiCoc depositStatus = currentBooking->getTrangThaiCoc();
    QString depositStatusText = QString::fromStdString(currentBooking->getTrangThaiCocText());
    depositStatusLabel->setText(depositStatusText);

    // Color based on deposit status
    switch (depositStatus)
    {
    case DA_COC:
        depositStatusLabel->setStyleSheet("color: #16a34a; font-weight: bold;");
        payDepositBtn->setVisible(false);
        break;
    case HOAN_COC:
        depositStatusLabel->setStyleSheet("color: #3b82f6; font-weight: bold;");
        payDepositBtn->setVisible(false);
        break;
    case MAT_COC:
        depositStatusLabel->setStyleSheet("color: #dc2626; font-weight: bold;");
        payDepositBtn->setVisible(false);
        break;
    default:
        depositStatusLabel->setStyleSheet("color: #6b7280; font-weight: 500;");
        payDepositBtn->setVisible(false);
        break;
    }

    // Enable/disable controls based on status
    TrangThaiDatSan status = currentBooking->getTrangThai();
    bool canEdit = (status == TrangThaiDatSan::DA_DAT);
    bool canPay = (status == TrangThaiDatSan::DA_DAT);

    fieldCombo->setEnabled(false); // Không cho edit sân
    dateEdit->setEnabled(false);   // Không cho edit ngày giờ
    fromTimeEdit->setEnabled(false);
    toTimeEdit->setEnabled(false);
    addServiceBtn->setEnabled(canEdit || canPay);
    removeServiceBtn->setEnabled(canEdit || canPay);
    cancelBookingBtn->setEnabled(status == TrangThaiDatSan::DA_DAT);
    paymentBtn->setEnabled(canPay && depositStatus == DA_COC); // Chỉ thanh toán khi đã cọc
}

void BookingDetailDialog::loadServices()
{
    serviceList->clear();

    if (!currentBooking)
        return;

    const MangDong<DichVuDat> &services = currentBooking->getDanhSachDichVu();

    if (services.size() == 0)
    {
        QListWidgetItem *item = new QListWidgetItem("Chưa có dịch vụ nào");
        item->setForeground(QBrush(QColor(156, 163, 175)));
        serviceList->addItem(item);
        return;
    }

    for (int i = 0; i < services.size(); i++)
    {
        const DichVuDat &dvDat = services[i];
        DichVu *dv = dvDat.getDichVu();
        if (dv)
        {
            QString text = QString("%1 x%2 - %3")
                               .arg(QString::fromStdString(dv->layTenDichVu()))
                               .arg(dvDat.getSoLuong())
                               .arg(formatCurrency(dv->layDonGia() * dvDat.getSoLuong()));
            serviceList->addItem(text);
        }
    }
}

void BookingDetailDialog::updateTotalPrice()
{
    if (!currentBooking)
        return;

    totalPriceLabel->setText(formatCurrency(currentBooking->getTongTien()));
}

QString BookingDetailDialog::formatCurrency(double amount) const
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(0) << amount;
    QString result = QString::fromStdString(ss.str());

    // Add thousand separators
    int pos = result.length() - 3;
    while (pos > 0)
    {
        result.insert(pos, '.');
        pos -= 3;
    }

    return result + "đ";
}

QString BookingDetailDialog::getStatusText(TrangThaiDatSan status) const
{
    switch (status)
    {
    case TrangThaiDatSan::DA_DAT:
        return "🟢 Đã đặt";
    case TrangThaiDatSan::HOAN_THANH:
        return "✓ Hoàn thành";
    case TrangThaiDatSan::DA_HUY:
        return "❌ Đã hủy";
    default:
        return "N/A";
    }
}

// ===== SLOTS =====

void BookingDetailDialog::onSaveClicked()
{
    if (!currentBooking || !system)
        return;

    // Validate and update booking
    QString selectedFieldId = fieldCombo->currentData().toString();
    San *newField = nullptr;
    for (int i = 0; i < fields.size(); i++)
    {
        if (QString::fromStdString(fields[i]->getMaSan()) == selectedFieldId)
        {
            newField = fields[i];
            break;
        }
    }

    if (!newField)
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn sân!");
        return;
    }

    // Update booking info
    currentBooking->setSan(newField);

    // Update date/time
    QDate date = dateEdit->date();
    QTime fromTime = fromTimeEdit->time();
    NgayGio newTime(date.year(), date.month(), date.day(), fromTime.hour(), fromTime.minute());
    currentBooking->setThoiGianDat(newTime);

    // Update status
    int statusIndex = statusCombo->currentIndex();
    TrangThaiDatSan newStatus = static_cast<TrangThaiDatSan>(statusCombo->itemData(statusIndex).toInt());
    currentBooking->setTrangThai(newStatus);

    // Update note
    currentBooking->setGhiChu(noteEdit->toPlainText().toStdString());

    // Save to system
    if (system->luuHeThong("D:/PBL2-/Data/booking.dat"))
    {
        QMessageBox::information(this, "Thành công", "Đã cập nhật thông tin đặt sân!");
        accept(); // Close dialog with success
    }
    else
    {
        QMessageBox::critical(this, "Lỗi", "Không thể lưu dữ liệu!");
    }
}

void BookingDetailDialog::onCancelBookingClicked()
{
    if (!currentBooking || !system)
        return;

    // Chỉ cho phép hủy booking đang đặt
    if (currentBooking->getTrangThai() != TrangThaiDatSan::DA_DAT)
    {
        QMessageBox::warning(this, "Lỗi", "Chỉ có thể hủy booking đang đặt!");
        return;
    }

    // Mở CancelBookingDialog
    CancelBookingDialog cancelDialog(currentBooking, this);
    if (cancelDialog.exec() == QDialog::Accepted)
    {
        // Lấy kết quả từ dialog
        bool refundDeposit = cancelDialog.isRefundDeposit();
        QString reason = cancelDialog.getCancelReason();

        // Gọi method hủy booking
        currentBooking->huyBooking(refundDeposit, reason.toStdString());

        // Lưu hệ thống
        if (system->luuHeThong("D:/PBL2-/Data/booking.dat"))
        {
            QString resultMsg = "✅ Đã hủy lịch đặt sân!\n\n";
            if (refundDeposit)
            {
                resultMsg += "💰 Hoàn cọc: " + formatCurrency(currentBooking->getTienCoc());
            }
            else
            {
                resultMsg += "❌ Mất cọc: " + formatCurrency(currentBooking->getTienCoc());
            }
            resultMsg += "\n📝 Lý do: " + reason;

            QMessageBox::information(this, "Thành công", resultMsg);
            accept(); // Close dialog with success
        }
        else
        {
            QMessageBox::critical(this, "Lỗi", "Không thể lưu dữ liệu!");
        }
    }
}

void BookingDetailDialog::onPaymentClicked()
{
    if (!currentBooking || !system)
        return;

    // Confirm payment
    QString message = QString("Xác nhận thanh toán:\n\nTổng tiền: %1")
                          .arg(formatCurrency(currentBooking->getTongTien()));

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Xác nhận thanh toán",
        message,
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        currentBooking->setTrangThai(TrangThaiDatSan::HOAN_THANH);

        if (system->luuHeThong("D:/PBL2-/Data/booking.dat"))
        {
            QMessageBox::information(this, "Thành công",
                                     "Thanh toán thành công!\n\nĐơn đặt sân đã hoàn thành.");
            accept(); // Close dialog with success
        }
        else
        {
            QMessageBox::critical(this, "Lỗi", "Không thể xử lý thanh toán!");
        }
    }
}

void BookingDetailDialog::onAddServiceClicked()
{
    if (!currentBooking || !system)
        return;

    // Get available services
    const MangDong<DichVu *> &allServices = system->layDanhSachDichVu();
    if (allServices.size() == 0)
    {
        QMessageBox::information(this, "Thông báo", "Không có dịch vụ nào!");
        return;
    }

    // Create dialog to select service
    QDialog selectDialog(this);
    selectDialog.setWindowTitle("Chọn dịch vụ");
    selectDialog.setMinimumSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(&selectDialog);

    QComboBox *serviceCombo = new QComboBox();
    for (int i = 0; i < allServices.size(); i++)
    {
        DichVu *dv = allServices[i];
        if (dv)
        {
            QString text = QString("%1 - %2")
                               .arg(QString::fromStdString(dv->layTenDichVu()))
                               .arg(formatCurrency(dv->layDonGia()));
            serviceCombo->addItem(text, QString::fromStdString(dv->layMaDichVu()));
        }
    }
    layout->addWidget(new QLabel("Chọn dịch vụ:"));
    layout->addWidget(serviceCombo);

    QSpinBox *quantitySpin = new QSpinBox();
    quantitySpin->setMinimum(1);
    quantitySpin->setMaximum(100);
    quantitySpin->setValue(1);
    layout->addWidget(new QLabel("Số lượng:"));
    layout->addWidget(quantitySpin);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *okBtn = new QPushButton("Thêm");
    QPushButton *cancelBtn = new QPushButton("Hủy");
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, &selectDialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &selectDialog, &QDialog::reject);

    if (selectDialog.exec() == QDialog::Accepted)
    {
        QString maDV = serviceCombo->currentData().toString();
        int quantity = quantitySpin->value();

        // Find service
        DichVu *selectedService = nullptr;
        for (int i = 0; i < allServices.size(); i++)
        {
            if (QString::fromStdString(allServices[i]->layMaDichVu()) == maDV)
            {
                selectedService = allServices[i];
                break;
            }
        }

        if (selectedService)
        {
            DichVuDat dvDat(selectedService, quantity);
            currentBooking->themDichVu(dvDat);
            loadServices();
            updateTotalPrice();
            QMessageBox::information(this, "Thành công", "Đã thêm dịch vụ!");
        }
    }
}

void BookingDetailDialog::onRemoveServiceClicked()
{
    if (!currentBooking)
        return;

    int currentRow = serviceList->currentRow();
    if (currentRow < 0)
    {
        QMessageBox::warning(this, "Thông báo", "Vui lòng chọn dịch vụ cần xóa!");
        return;
    }

    const MangDong<DichVuDat> &services = currentBooking->getDanhSachDichVu();
    if (currentRow >= services.size())
        return;

    // Remove service
    // Remove service by index
    currentBooking->xoaDichVu(currentRow);
    loadServices();
    updateTotalPrice();
    QMessageBox::information(this, "Thành công", "Đã xóa dịch vụ!");
}

void BookingDetailDialog::onPayDepositClicked()
{
    if (!currentBooking || !system)
        return;

    // Kiểm tra trạng thái - chỉ cho phép đặt cọc nếu chưa có cọc
    if (currentBooking->getTrangThaiCoc() != TrangThaiCoc::DA_COC)
    {
        QMessageBox::warning(this, "Lỗi", "Booking này đã xử lý cọc rồi!");
        return;
    }

    // Tính tiền cọc (30%)
    currentBooking->tinhTienCoc();
    double depositAmount = currentBooking->getTienCoc();

    // Confirm deposit payment
    QString message = QString("Xác nhận đặt cọc:\n\nTổng tiền: %1\nTiền cọc (30%%): %2\n\nChú ý: Booking đã tự động đặt cọc khi tạo!")
                          .arg(formatCurrency(currentBooking->getTongTien()))
                          .arg(formatCurrency(depositAmount));

    QMessageBox::information(this, "Thông tin", message);
}
