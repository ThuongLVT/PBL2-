/**
 * @file BookingDetailDialog.cpp
 * @brief Implementation of BookingDetailDialog
 */

#include "BookingDetailDialog.h"
#include "CancelBookingDialog.h"
#include "../../Dialogs/ServiceSelectionDialog.h"
#include "../../../Core/QuanLy/QuanLyThanhToan.h"
#include <QMessageBox>
#include <QSpinBox>
#include <QDialog>
#include <QHeaderView>
#include <QDateTime>
#include <iomanip>
#include <sstream>
#include <QScreen>
#include <QGuiApplication>

BookingDetailDialog::BookingDetailDialog(DatSan *booking, QWidget *parent)
    : QDialog(parent),
      currentBooking(booking),
      system(nullptr)
{
    system = HeThongQuanLy::getInstance();
    fields = system->layDanhSachSan();

    setWindowTitle("Chi tiết đặt sân");
    setMinimumSize(1100, 800); // Fixed height 800
    setModal(true);

    setupUI();
    populateForm();
    loadServices();

    // Center the dialog
    if (QScreen *screen = QGuiApplication::primaryScreen())
    {
        QRect screenGeometry = screen->availableGeometry();
        int x = (screenGeometry.width() - width()) / 2;
        int y = (screenGeometry.height() - height()) / 2;
        move(x, y);
    }
}

BookingDetailDialog::~BookingDetailDialog()
{
    // Qt handles cleanup
}

void BookingDetailDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 1. Header Section
    setupHeader(mainLayout);

    // 2. Content Section (Split Left/Right)
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(20);

    // Left Column (Info) - 40%
    QFrame *infoFrame = createInfoSection();
    // Ensure info frame expands
    infoFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    contentLayout->addWidget(infoFrame, 40);

    // Right Column (Services & Payment) - 60%
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(20);

    QFrame *servicesFrame = createServicesSection();
    QFrame *paymentFrame = createPaymentSection();

    // Split 50/50 vertically
    rightLayout->addWidget(servicesFrame, 1);
    rightLayout->addWidget(paymentFrame, 1);

    contentLayout->addLayout(rightLayout, 60);

    mainLayout->addLayout(contentLayout);

    // Global Styles
    setStyleSheet(
        "QDialog { background-color: #f3f4f6; }"
        "QFrame#card { "
        "background-color: white; "
        "border: 1px solid #e5e7eb; "
        "border-radius: 8px; "
        "}"
        "QLabel#sectionTitle { "
        "font-size: 14px; "
        "font-weight: bold; "
        "color: #374151; "
        "padding-bottom: 5px; "
        "border-bottom: 1px solid #e5e7eb; "
        "}");
}

void BookingDetailDialog::setupHeader(QVBoxLayout *parentLayout)
{
    QFrame *headerFrame = new QFrame();
    headerFrame->setObjectName("card");
    headerFrame->setStyleSheet("#card { background-color: white; border-bottom: 1px solid #e5e7eb; }");

    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(20, 15, 20, 15);

    // Left: ID
    QVBoxLayout *idLayout = new QVBoxLayout();
    headerIdLabel = new QLabel("BK000");
    headerIdLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #111827;");

    idLayout->addWidget(headerIdLabel);

    // Right: Status Badge
    headerStatusLabel = new QLabel("Trạng thái");
    headerStatusLabel->setAlignment(Qt::AlignCenter);
    headerStatusLabel->setFixedSize(100, 34);
    headerStatusLabel->setStyleSheet(
        "background-color: #3b82f6; "
        "color: white; "
        "font-weight: bold; "
        "border-radius: 17px;");

    headerLayout->addLayout(idLayout);
    headerLayout->addStretch();
    headerLayout->addWidget(headerStatusLabel);

    parentLayout->addWidget(headerFrame);
}

QFrame *BookingDetailDialog::createInfoSection()
{
    QFrame *card = new QFrame();
    card->setObjectName("card");

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel *title = new QLabel("Thông tin đặt sân");
    title->setObjectName("sectionTitle");
    layout->addWidget(title);

    // Form Content
    QGridLayout *grid = new QGridLayout();
    grid->setVerticalSpacing(12);
    grid->setHorizontalSpacing(10);

    // Helper to add rows
    auto addRow = [&](int row, QString label, QLabel *&valueLabel, bool isBold = false)
    {
        QLabel *lbl = new QLabel(label);
        lbl->setStyleSheet("color: #6b7280;");
        valueLabel = new QLabel("-");
        valueLabel->setStyleSheet(isBold ? "font-weight: bold; color: #111827;" : "color: #111827;");
        valueLabel->setWordWrap(true);
        grid->addWidget(lbl, row, 0);
        grid->addWidget(valueLabel, row, 1);
    };

    addRow(0, "Khách hàng:", customerNameLabel, true);
    addRow(1, "SĐT:", customerPhoneLabel);
    addRow(2, "Hạng TV:", customerTierLabel);

    // Divider
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #e5e7eb;");
    grid->addWidget(line, 3, 0, 1, 2);

    addRow(4, "Sân bóng:", fieldNameLabel, true);
    addRow(5, "Ngày đá:", dateLabel);
    addRow(6, "Khung giờ:", timeLabel, true); // Blue color applied in populate
    addRow(7, "Ghi chú:", noteLabel);
    noteLabel->setWordWrap(true);

    layout->addLayout(grid);
    layout->addStretch();

    // Action Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);

    rescheduleBtn = new QPushButton("📅 Đổi lịch");
    rescheduleBtn->setCursor(Qt::PointingHandCursor);
    rescheduleBtn->setMinimumHeight(40);
    rescheduleBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #3b82f6; "
        "color: white; "
        "border: none; "
        "border-radius: 6px; "
        "font-weight: 600; "
        "} "
        "QPushButton:hover { background-color: #2563eb; }");

    cancelBtn = new QPushButton("❌ Hủy lịch");
    cancelBtn->setCursor(Qt::PointingHandCursor);
    cancelBtn->setMinimumHeight(40);
    cancelBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #ef4444; "
        "color: white; "
        "border: none; "
        "border-radius: 6px; "
        "font-weight: 600; "
        "} "
        "QPushButton:hover { background-color: #dc2626; }");

    closeBtn = new QPushButton("Đóng");
    closeBtn->setMinimumHeight(40);
    closeBtn->setCursor(Qt::PointingHandCursor);
    closeBtn->setStyleSheet(
        "QPushButton { "
        "background-color: white; "
        "border: 1px solid #d1d5db; "
        "border-radius: 6px; "
        "color: #374151; "
        "font-weight: 500; "
        "} "
        "QPushButton:hover { "
        "background-color: #f3f4f6; "
        "}");
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::reject);

    btnLayout->addWidget(rescheduleBtn);
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(closeBtn);
    layout->addLayout(btnLayout);

    // Connect buttons
    connect(cancelBtn, &QPushButton::clicked, this, &BookingDetailDialog::onCancelBookingClicked);

    return card;
}

QFrame *BookingDetailDialog::createServicesSection()
{
    QFrame *card = new QFrame();
    card->setObjectName("card");

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel *title = new QLabel("Dịch vụ đi kèm");
    title->setObjectName("sectionTitle");
    layout->addWidget(title);

    // Table
    serviceTable = new QTableWidget();
    serviceTable->setColumnCount(7);
    serviceTable->setHorizontalHeaderLabels({"STT", "Mã DV", "Tên dịch vụ", "Đơn vị", "Đơn giá", "SL", "Thành tiền"});
    serviceTable->verticalHeader()->setVisible(false);
    serviceTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); // STT
    serviceTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents); // Ma DV
    serviceTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);          // Ten DV
    serviceTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents); // Don vi
    serviceTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents); // Don gia
    serviceTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents); // SL
    serviceTable->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents); // Thanh tien
    serviceTable->setShowGrid(false);
    serviceTable->setFrameShape(QFrame::NoFrame);
    serviceTable->setSelectionMode(QAbstractItemView::SingleSelection);
    serviceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    serviceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    serviceTable->setFocusPolicy(Qt::NoFocus);
    serviceTable->setStyleSheet(
        "QTableWidget { background-color: #f9fafb; border-radius: 6px; }"
        "QHeaderView::section { background-color: #f3f4f6; border: none; padding: 8px; font-weight: bold; color: #6b7280; }"
        "QTableWidget::item { padding: 8px; border-bottom: 1px solid #e5e7eb; outline: none; }"
        "QTableWidget::item:selected { background-color: #e0f2fe; color: #111827; }");
    // serviceTable->setMinimumHeight(150); // Let layout handle height
    layout->addWidget(serviceTable);

    // Add Button
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    removeServiceBtn = new QPushButton("🗑️ Xóa dịch vụ");
    removeServiceBtn->setCursor(Qt::PointingHandCursor);
    removeServiceBtn->setFixedSize(140, 36);
    removeServiceBtn->setEnabled(false); // Disabled by default
    removeServiceBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #ef4444; "
        "color: white; "
        "border: none; "
        "border-radius: 6px; "
        "font-weight: 600; "
        "} "
        "QPushButton:hover { background-color: #dc2626; }"
        "QPushButton:disabled { background-color: #9ca3af; }");

    addServiceBtn = new QPushButton("➕ Thêm dịch vụ");
    addServiceBtn->setCursor(Qt::PointingHandCursor);
    addServiceBtn->setFixedSize(140, 36);
    addServiceBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #10b981; "
        "color: white; "
        "border: none; "
        "border-radius: 6px; "
        "font-weight: 600; "
        "} "
        "QPushButton:hover { background-color: #059669; }");

    btnLayout->addWidget(removeServiceBtn);
    btnLayout->addWidget(addServiceBtn);
    layout->addLayout(btnLayout);

    connect(addServiceBtn, &QPushButton::clicked, this, &BookingDetailDialog::onAddServiceClicked);
    connect(removeServiceBtn, &QPushButton::clicked, this, &BookingDetailDialog::onRemoveServiceClicked);

    // Enable remove button when row selected
    connect(serviceTable, &QTableWidget::itemSelectionChanged, [this]()
            { removeServiceBtn->setEnabled(!serviceTable->selectedItems().isEmpty()); });

    return card;
}

QFrame *BookingDetailDialog::createPaymentSection()
{
    QFrame *card = new QFrame();
    card->setObjectName("card");

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setSpacing(10);
    layout->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel *title = new QLabel("Thanh toán");
    title->setObjectName("sectionTitle");
    layout->addWidget(title);

    // Rows
    auto addRow = [&](QString label, QLabel *&valueLabel, QString color = "#111827", bool isBold = false)
    {
        QHBoxLayout *row = new QHBoxLayout();
        QLabel *lbl = new QLabel(label);
        lbl->setStyleSheet("color: #6b7280;");

        valueLabel = new QLabel("0đ");
        QString style = QString("color: %1;").arg(color);
        if (isBold)
            style += " font-weight: bold;";
        valueLabel->setStyleSheet(style);

        row->addWidget(lbl);
        row->addStretch();
        row->addWidget(valueLabel);
        layout->addLayout(row);
    };

    addRow("Tiền sân:", fieldPriceLabel);
    addRow("Tiền dịch vụ:", servicePriceLabel);
    addRow("Giảm giá:", discountLabel, "#10b981");
    addRow("Đã cọc:", depositLabel, "#f59e0b", true);

    // Divider
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #e5e7eb; margin: 5px 0;");
    layout->addWidget(line);

    // Totals
    QHBoxLayout *totalRow = new QHBoxLayout();
    QLabel *lblTotal = new QLabel("TỔNG CỘNG:");
    lblTotal->setStyleSheet("font-weight: bold; color: #374151; font-size: 14px;");
    totalLabel = new QLabel("0đ");
    totalLabel->setStyleSheet("font-weight: bold; color: #111827; font-size: 16px;");
    totalRow->addWidget(lblTotal);
    totalRow->addStretch();
    totalRow->addWidget(totalLabel);
    layout->addLayout(totalRow);

    QHBoxLayout *payRow = new QHBoxLayout();
    QLabel *lblPay = new QLabel("CẦN THANH TOÁN:");
    lblPay->setStyleSheet("font-weight: bold; color: #ef4444; font-size: 14px;");
    toPayLabel = new QLabel("0đ");
    toPayLabel->setStyleSheet("font-weight: bold; color: #ef4444; font-size: 18px;");
    payRow->addWidget(lblPay);
    payRow->addStretch();
    payRow->addWidget(toPayLabel);
    layout->addLayout(payRow);

    // Pay Button
    payNowBtn = new QPushButton("💰 Thanh toán ngay");
    payNowBtn->setCursor(Qt::PointingHandCursor);
    payNowBtn->setMinimumHeight(44);
    payNowBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #2563eb; "
        "color: white; "
        "border: none; "
        "border-radius: 6px; "
        "font-weight: bold; "
        "font-size: 15px;"
        "} "
        "QPushButton:hover { background-color: #1d4ed8; }"
        "QPushButton:disabled { background-color: #9ca3af; }");
    layout->addWidget(payNowBtn);

    connect(payNowBtn, &QPushButton::clicked, this, &BookingDetailDialog::onPaymentClicked);
    connect(rescheduleBtn, &QPushButton::clicked, [this]() {
        emit rescheduleRequested(currentBooking);
        accept(); // Close the dialog
    });

    return card;
}

void BookingDetailDialog::populateForm()
{
    if (!currentBooking)
        return;

    // Header
    headerIdLabel->setText(QString::fromStdString(currentBooking->getMaDatSan()));

    // Status Badge
    TrangThaiDatSan status = currentBooking->getTrangThai();
    QString statusText;
    QString statusColor;
    switch (status)
    {
    case TrangThaiDatSan::DA_DAT:
        statusText = "Đã đặt";
        statusColor = "#3b82f6";
        break;
    case TrangThaiDatSan::HOAN_THANH:
        statusText = "Hoàn thành";
        statusColor = "#10b981";
        break;
    case TrangThaiDatSan::DA_HUY:
        statusText = "Đã hủy";
        statusColor = "#ef4444";
        break;
    default:
        statusText = "N/A";
        statusColor = "#6b7280";
        break;
    }
    headerStatusLabel->setText(statusText);
    headerStatusLabel->setStyleSheet(QString("background-color: %1; color: white; font-weight: bold; border-radius: 17px;").arg(statusColor));

    // Info Section
    KhachHang *kh = currentBooking->getKhachHang();
    customerNameLabel->setText(kh ? QString::fromStdString(kh->getHoTen()) : "N/A");
    customerPhoneLabel->setText(kh ? QString::fromStdString(kh->getSoDienThoai()) : "N/A");

    if (kh)
    {
        QString tierName;
        switch (kh->layHang())
        {
        case HangKhachHang::THUONG:
            tierName = "Thường";
            break;
        case HangKhachHang::DONG:
            tierName = "Đồng";
            break;
        case HangKhachHang::BAC:
            tierName = "Bạc";
            break;
        case HangKhachHang::VANG:
            tierName = "Vàng";
            break;
        case HangKhachHang::KIM_CUONG:
            tierName = "Kim Cương";
            break;
        default:
            tierName = "Không xác định";
            break;
        }
        int discount = kh->layPhanTramGiamGia();
        customerTierLabel->setText(QString("%1 (-%2%)").arg(tierName).arg(discount));
    }
    else
    {
        customerTierLabel->setText("Khách vãng lai");
    }

    San *san = currentBooking->getSan();
    fieldNameLabel->setText(san ? QString::fromStdString(san->getTenSan()) : "N/A");

    NgayGio tg = currentBooking->getThoiGianDat();
    dateLabel->setText(QString("%1/%2/%3").arg(tg.getNgay()).arg(tg.getThang()).arg(tg.getNam()));

    QString timeStr = QString("%1:00 - %2:00").arg(tg.getGio()).arg(tg.getGio() + 1);
    timeLabel->setText(timeStr);
    timeLabel->setStyleSheet("color: #2563eb; font-weight: bold;");

    noteLabel->setText(QString::fromStdString(currentBooking->getGhiChu()));

    // Payment Section
    double fieldPrice = san ? san->getGiaThue() : 0;
    fieldPriceLabel->setText(formatCurrency(fieldPrice));

    double deposit = currentBooking->getTienCoc();
    depositLabel->setText(formatCurrency(deposit));

    // Buttons State
    bool canEdit = (status == TrangThaiDatSan::DA_DAT);
    rescheduleBtn->setEnabled(canEdit);
    cancelBtn->setEnabled(canEdit);
    addServiceBtn->setEnabled(canEdit);
    payNowBtn->setEnabled(canEdit);

    if (status == TrangThaiDatSan::HOAN_THANH)
    {
        payNowBtn->setText("Đã thanh toán");
        payNowBtn->setEnabled(false);
        payNowBtn->setStyleSheet("background-color: #10b981; color: white; border-radius: 6px; font-weight: bold;");
    }
}

void BookingDetailDialog::loadServices()
{
    if (!currentBooking)
        return;

    const MangDong<DichVuDat> &services = currentBooking->getDanhSachDichVu();
    serviceTable->setRowCount(services.size());

    double totalServicePrice = 0;

    for (int i = 0; i < services.size(); i++)
    {
        const DichVuDat &dvDat = services[i];
        DichVu *dv = dvDat.getDichVu();
        if (dv)
        {
            double unitPrice = dv->layDonGia();
            double total = unitPrice * dvDat.getSoLuong();
            totalServicePrice += total;

            QTableWidgetItem *sttItem = new QTableWidgetItem(QString::number(i + 1));
            QTableWidgetItem *codeItem = new QTableWidgetItem(QString::fromStdString(dv->layMaDichVu()));
            QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(dv->layTenDichVu()));
            QTableWidgetItem *unitItem = new QTableWidgetItem(QString::fromStdString(dv->layDonVi()));
            QTableWidgetItem *priceItem = new QTableWidgetItem(formatCurrency(unitPrice));
            QTableWidgetItem *qtyItem = new QTableWidgetItem(QString::number(dvDat.getSoLuong()));
            QTableWidgetItem *totalItem = new QTableWidgetItem(formatCurrency(total));

            sttItem->setTextAlignment(Qt::AlignCenter);
            codeItem->setTextAlignment(Qt::AlignCenter);
            unitItem->setTextAlignment(Qt::AlignCenter);
            priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            qtyItem->setTextAlignment(Qt::AlignCenter);
            totalItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

            serviceTable->setItem(i, 0, sttItem);
            serviceTable->setItem(i, 1, codeItem);
            serviceTable->setItem(i, 2, nameItem);
            serviceTable->setItem(i, 3, unitItem);
            serviceTable->setItem(i, 4, priceItem);
            serviceTable->setItem(i, 5, qtyItem);
            serviceTable->setItem(i, 6, totalItem);
        }
    }

    // Update Payment Totals
    servicePriceLabel->setText(formatCurrency(totalServicePrice));

    double fieldPrice = currentBooking->getSan() ? currentBooking->getSan()->getGiaThue() : 0;
    double subTotal = fieldPrice + totalServicePrice;

    double discountAmount = 0;
    KhachHang *kh = currentBooking->getKhachHang();
    if (kh)
    {
        discountAmount = subTotal * kh->layPhanTramGiamGia() / 100.0;
    }
    discountLabel->setText("-" + formatCurrency(discountAmount));

    double total = subTotal - discountAmount;
    totalLabel->setText(formatCurrency(total));

    double toPay = total - currentBooking->getTienCoc();
    if (toPay < 0)
        toPay = 0;
    toPayLabel->setText(formatCurrency(toPay));
}

void BookingDetailDialog::updateTotalPrice()
{
    // Re-use loadServices to update totals
    loadServices();
}

QString BookingDetailDialog::formatCurrency(double amount) const
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(0) << amount;
    QString result = QString::fromStdString(ss.str());

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
    Q_UNUSED(status);
    // Helper not used anymore but kept for compatibility
    return "";
}

// ===== SLOTS (Keep existing logic but adapt where needed) =====

void BookingDetailDialog::onSaveClicked() { /* Not used in new UI */ }

void BookingDetailDialog::onCancelBookingClicked()
{
    if (!currentBooking || !system)
        return;

    CancelBookingDialog cancelDialog(currentBooking, this);
    if (cancelDialog.exec() == QDialog::Accepted)
    {
        bool refundDeposit = cancelDialog.isRefundDeposit();
        QString reason = cancelDialog.getCancelReason();

        currentBooking->huyBooking(refundDeposit, reason.toStdString());

        // Lưu hệ thống
        if (system->luuHeThong("D:/PBL2-/Data/booking.dat"))
        {
            QMessageBox::information(this, "Thành công", "Đã hủy lịch đặt sân!");
            accept();
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

    QString message = QString("Xác nhận thanh toán:\n\nCần thanh toán: %1")
                          .arg(toPayLabel->text());

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Xác nhận thanh toán",
        message,
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        // Sử dụng QuanLyThanhToan để tạo thanh toán và cập nhật chi tiêu
        QuanLyThanhToan *qltt = system->layQuanLyThanhToan();
        if (qltt)
        {
            // Mặc định thanh toán tiền mặt
            ThanhToan *tt = qltt->taoThanhToan(currentBooking, PhuongThucThanhToan::TIEN_MAT);

            if (tt)
            {
                // Update stock for services
                const MangDong<DichVuDat> &services = currentBooking->getDanhSachDichVu();
                for (int i = 0; i < services.size(); i++)
                {
                    DichVu *dv = services[i].getDichVu();
                    if (dv)
                    {
                        int qty = services[i].getSoLuong();
                        dv->datSoLuongTon(dv->laySoLuongTon() - qty);
                        dv->datSoLuongBan(dv->laySoLuongBan() + qty);
                    }
                }

                currentBooking->setTrangThai(TrangThaiDatSan::HOAN_THANH);

                if (system->luuHeThong("D:/PBL2-/Data/booking.dat"))
                {
                    // Lưu thêm dữ liệu thanh toán nếu cần (tùy thuộc vào implementation của system)
                    // system->luuHeThong("D:/PBL2-/Data/thanhtoan.dat"); // Giả sử có file này

                    QMessageBox::information(this, "Thành công", "Thanh toán thành công!");
                    accept();
                }
                else
                {
                    QMessageBox::critical(this, "Lỗi", "Không thể lưu dữ liệu!");
                }
            }
            else
            {
                QMessageBox::critical(this, "Lỗi", "Không thể tạo thanh toán (có thể đã thanh toán rồi)!");
            }
        }
        else
        {
            // Fallback nếu không lấy được quản lý thanh toán
            currentBooking->setTrangThai(TrangThaiDatSan::HOAN_THANH);
            system->luuHeThong("D:/PBL2-/Data/booking.dat");
            accept();
        }
    }
}

void BookingDetailDialog::onAddServiceClicked()
{
    if (!currentBooking || !system)
        return;

    ServiceSelectionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QMap<std::string, int> selectedServices = dialog.getSelectedServices();
        const MangDong<DichVu *> &allServices = system->layDanhSachDichVu();

        if (selectedServices.isEmpty())
            return;

        for (auto it = selectedServices.begin(); it != selectedServices.end(); ++it)
        {
            std::string serviceId = it.key();
            int quantity = it.value();

            // Find the service object
            DichVu *service = nullptr;
            for (int i = 0; i < allServices.size(); i++)
            {
                if (allServices[i]->layMaDichVu() == serviceId)
                {
                    service = allServices[i];
                    break;
                }
            }

            if (service)
            {
                DichVuDat dvDat(service, quantity);
                currentBooking->themDichVu(dvDat);
            }
        }

        // Refresh table and totals
        loadServices();

        // Optional: Save changes immediately if required, or wait for a "Save" action.
        // The original code didn't save immediately in onAddServiceClicked,
        // but onPaymentClicked does save.
        // However, since we are modifying the booking in memory,
        // and this dialog seems to be a "viewer/editor",
        // we should probably save the system state if we want it to persist
        // after closing the dialog without paying.
        // But looking at onCancelBookingClicked, it saves.
        // The original onAddServiceClicked didn't save to file, just updated memory.
        // I will stick to updating memory and refreshing UI.
    }
}

void BookingDetailDialog::onRemoveServiceClicked()
{
    if (!currentBooking)
        return;

    int currentRow = serviceTable->currentRow();
    if (currentRow < 0)
    {
        QMessageBox::warning(this, "Thông báo", "Vui lòng chọn dịch vụ cần xóa!");
        return;
    }

    // Get Service ID from column 1 (Mã DV)
    QString serviceId = serviceTable->item(currentRow, 1)->text();

    // Confirm deletion
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Xác nhận",
        "Bạn có chắc chắn muốn xóa dịch vụ này không?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        // Remove service from booking
        const MangDong<DichVuDat> &services = currentBooking->getDanhSachDichVu();
        for (int i = 0; i < services.size(); i++)
        {
            if (services[i].getDichVu() && services[i].getDichVu()->layMaDichVu() == serviceId.toStdString())
            {
                currentBooking->xoaDichVu(i);
                break;
            }
        }

        loadServices();
        removeServiceBtn->setEnabled(false);
    }
}
void BookingDetailDialog::onPayDepositClicked() { /* Not used in new UI */ }