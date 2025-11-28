#include "InvoiceDetailDialog.h"
#include "../../../Core/QuanLy/HeThongQuanLy.h"
#include <QHeaderView>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QMessageBox>
#include <QDate>
#include <QDesktopServices>
#include <QUrl>
#include <QGridLayout>
#include <QScrollArea>
#include <QGroupBox>

InvoiceDetailDialog::InvoiceDetailDialog(const QString &id, InvoiceType type, QWidget *parent)
    : QDialog(parent), m_id(id), m_type(type), m_booking(nullptr), m_order(nullptr)
{
    setWindowTitle("Chi Tiết Hoá Đơn");
    setFixedSize(900, 900);

    setupUI();
    applyStyles();

    if (m_type == BOOKING)
    {
        loadBookingData(id);
    }
    else
    {
        loadServiceData(id);
    }
}

InvoiceDetailDialog::~InvoiceDetailDialog()
{
}

QFrame *InvoiceDetailDialog::createSeparator()
{
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("background-color: #e0e0e0; max-height: 1px;");
    return line;
}

void InvoiceDetailDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(30, 25, 30, 20);

    // ===== HEADER SECTION =====
    m_companyNameLabel = new QLabel("⚽ SÂN BÓNG ĐÁ DUT");
    m_companyNameLabel->setObjectName("companyName");
    m_companyNameLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_companyNameLabel);
    mainLayout->addWidget(createSeparator());

    // ===== INVOICE TITLE =====
    m_titleLabel = new QLabel("HÓA ĐƠN THANH TOÁN");
    m_titleLabel->setObjectName("invoiceTitle");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_titleLabel);

    QHBoxLayout *invoiceMetaLayout = new QHBoxLayout();
    invoiceMetaLayout->setAlignment(Qt::AlignCenter);

    m_invoiceNumberLabel = new QLabel();
    m_invoiceNumberLabel->setObjectName("invoiceMeta");

    m_invoiceDateLabel = new QLabel();
    m_invoiceDateLabel->setObjectName("invoiceMeta");

    invoiceMetaLayout->addWidget(m_invoiceNumberLabel);
    invoiceMetaLayout->addSpacing(40);
    invoiceMetaLayout->addWidget(m_invoiceDateLabel);
    mainLayout->addLayout(invoiceMetaLayout);

    // ===== CUSTOMER & BOOKING INFO =====
    QWidget *infoWidget = new QWidget();
    infoWidget->setObjectName("infoSection");
    QGridLayout *infoGrid = new QGridLayout(infoWidget);
    infoGrid->setContentsMargins(15, 10, 15, 10);
    infoGrid->setHorizontalSpacing(15);
    infoGrid->setVerticalSpacing(6);
    infoGrid->setColumnStretch(1, 1);
    infoGrid->setColumnStretch(3, 1);

    QLabel *lblName = new QLabel("Khách hàng:");
    lblName->setObjectName("infoLabel");
    m_customerNameLabel = new QLabel();
    m_customerNameLabel->setObjectName("infoValue");

    QLabel *lblPhone = new QLabel("SĐT:");
    lblPhone->setObjectName("infoLabel");
    m_customerPhoneLabel = new QLabel();
    m_customerPhoneLabel->setObjectName("infoValue");

    infoGrid->addWidget(lblName, 0, 0);
    infoGrid->addWidget(m_customerNameLabel, 0, 1);
    infoGrid->addWidget(lblPhone, 0, 2);
    infoGrid->addWidget(m_customerPhoneLabel, 0, 3);

    m_bookingInfoLabel = new QLabel();
    m_bookingInfoLabel->setObjectName("bookingInfo");
    m_bookingInfoLabel->setWordWrap(true);
    infoGrid->addWidget(m_bookingInfoLabel, 1, 0, 1, 4);

    mainLayout->addWidget(infoWidget);

    // ===== DETAILS TABLE =====
    QLabel *detailsTitle = new QLabel("CHI TIẾT DỊCH VỤ");
    detailsTitle->setObjectName("sectionTitle");
    mainLayout->addWidget(detailsTitle);

    m_detailsTable = new QTableWidget();
    m_detailsTable->setObjectName("invoiceTable");
    m_detailsTable->setColumnCount(5);
    m_detailsTable->setHorizontalHeaderLabels({"STT", "Mô tả", "Đơn giá", "SL", "Thành tiền"});
    m_detailsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_detailsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_detailsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    m_detailsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    m_detailsTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    m_detailsTable->setColumnWidth(0, 50);
    m_detailsTable->setColumnWidth(2, 130);
    m_detailsTable->setColumnWidth(3, 50);
    m_detailsTable->setColumnWidth(4, 140);
    m_detailsTable->verticalHeader()->setVisible(false);
    m_detailsTable->setSelectionMode(QAbstractItemView::NoSelection);
    m_detailsTable->setFocusPolicy(Qt::NoFocus);
    m_detailsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_detailsTable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_detailsTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_detailsTable->setMinimumHeight(180);
    m_detailsTable->setMaximumHeight(220);

    mainLayout->addWidget(m_detailsTable);

    // ===== SUMMARY SECTION =====
    QWidget *summaryWidget = new QWidget();
    summaryWidget->setObjectName("summarySection");
    QGridLayout *summaryGrid = new QGridLayout(summaryWidget);
    summaryGrid->setContentsMargins(10, 10, 10, 10);
    summaryGrid->setColumnStretch(0, 1);
    summaryGrid->setHorizontalSpacing(15);
    summaryGrid->setVerticalSpacing(5);

    int row = 0;

    QLabel *lblSubtotal = new QLabel("Tạm tính:");
    lblSubtotal->setObjectName("summaryLabel");
    m_subtotalLabel = new QLabel("0 VNĐ");
    m_subtotalLabel->setObjectName("summaryValue");
    m_subtotalLabel->setAlignment(Qt::AlignRight);
    summaryGrid->addWidget(lblSubtotal, row, 1);
    summaryGrid->addWidget(m_subtotalLabel, row++, 2);

    QLabel *lblDeposit = new QLabel("Đã cọc:");
    lblDeposit->setObjectName("summaryLabel");
    m_depositLabel = new QLabel("0 VNĐ");
    m_depositLabel->setObjectName("summaryValue");
    m_depositLabel->setAlignment(Qt::AlignRight);
    m_depositLabel->setStyleSheet("color: #16a34a;");
    summaryGrid->addWidget(lblDeposit, row, 1);
    summaryGrid->addWidget(m_depositLabel, row++, 2);

    QLabel *lblDiscount = new QLabel("Giảm giá:");
    lblDiscount->setObjectName("summaryLabel");
    m_discountLabel = new QLabel("0 VNĐ");
    m_discountLabel->setObjectName("summaryValue");
    m_discountLabel->setAlignment(Qt::AlignRight);
    m_discountLabel->setStyleSheet("color: #16a34a;");
    summaryGrid->addWidget(lblDiscount, row, 1);
    summaryGrid->addWidget(m_discountLabel, row++, 2);

    // Separator line
    QFrame *sumLine = new QFrame();
    sumLine->setFrameShape(QFrame::HLine);
    sumLine->setStyleSheet("background-color: #333; max-height: 2px;");
    summaryGrid->addWidget(sumLine, row++, 1, 1, 2);

    QLabel *lblTotal = new QLabel("TỔNG THANH TOÁN:");
    lblTotal->setObjectName("totalLabel");
    m_totalLabel = new QLabel("0 VNĐ");
    m_totalLabel->setObjectName("totalValue");
    m_totalLabel->setAlignment(Qt::AlignRight);
    summaryGrid->addWidget(lblTotal, row, 1);
    summaryGrid->addWidget(m_totalLabel, row++, 2);

    // Amount in words - hidden for compactness
    m_amountInWordsLabel = new QLabel();
    m_amountInWordsLabel->setObjectName("amountWords");
    m_amountInWordsLabel->setWordWrap(true);
    m_amountInWordsLabel->hide(); // Hidden for compact view

    mainLayout->addWidget(summaryWidget);

    // ===== STATUS =====
    QHBoxLayout *statusLayout = new QHBoxLayout();
    m_statusLabel = new QLabel();
    m_statusLabel->setObjectName("statusLabel");
    statusLayout->addStretch();
    statusLayout->addWidget(m_statusLabel);
    mainLayout->addLayout(statusLayout);

    // ===== FOOTER =====
    mainLayout->addWidget(createSeparator());
    QLabel *footerLabel = new QLabel("Cảm ơn quý khách đã sử dụng dịch vụ!");
    footerLabel->setObjectName("footerText");
    footerLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(footerLabel);

    mainLayout->addStretch();

    // ===== BUTTONS =====
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    m_exportBtn = new QPushButton("📄 Xuất PDF");
    m_exportBtn->setObjectName("exportBtn");
    m_exportBtn->setMinimumSize(140, 45);
    m_exportBtn->setCursor(Qt::PointingHandCursor);
    connect(m_exportBtn, &QPushButton::clicked, this, &InvoiceDetailDialog::onExportPdfClicked);

    m_closeBtn = new QPushButton("Đóng");
    m_closeBtn->setObjectName("closeBtn");
    m_closeBtn->setMinimumSize(100, 45);
    m_closeBtn->setCursor(Qt::PointingHandCursor);
    connect(m_closeBtn, &QPushButton::clicked, this, &QDialog::accept);

    btnLayout->addWidget(m_exportBtn);
    btnLayout->addWidget(m_closeBtn);
    mainLayout->addLayout(btnLayout);
}

void InvoiceDetailDialog::applyStyles()
{
    setStyleSheet(R"(
        QDialog {
            background-color: white;
        }
        
        QLabel#companyName {
            font-size: 22px;
            font-weight: bold;
            color: #16a34a;
            padding: 5px;
        }
        
        QLabel#invoiceTitle {
            font-size: 24px;
            font-weight: bold;
            color: #1a1a1a;
            padding: 8px 0;
        }
        
        QLabel#invoiceMeta {
            font-size: 13px;
            color: #555;
        }
        
        QWidget#infoSection {
            background-color: #f8f9fa;
            border: 1px solid #e5e7eb;
            border-radius: 8px;
        }
        
        QLabel#infoLabel {
            font-weight: bold;
            color: #555;
            font-size: 13px;
        }
        
        QLabel#infoValue {
            color: #1a1a1a;
            font-size: 13px;
            font-weight: 600;
        }
        
        QLabel#bookingInfo {
            font-size: 13px;
            color: #444;
            padding: 3px 0;
        }
        
        QLabel#sectionTitle {
            font-size: 14px;
            font-weight: bold;
            color: #333;
            padding: 8px 0 3px 0;
        }
        
        QTableWidget#invoiceTable {
            border: 1px solid #e0e0e0;
            border-radius: 6px;
            gridline-color: #f0f0f0;
            background-color: white;
        }
        
        QTableWidget#invoiceTable QHeaderView::section {
            background-color: #16a34a;
            color: white;
            font-weight: bold;
            font-size: 12px;
            padding: 10px 5px;
            border: none;
        }
        
        QTableWidget#invoiceTable::item {
            padding: 8px;
            font-size: 13px;
        }
        
        QWidget#summarySection {
            background-color: #fafafa;
            border: 1px solid #e5e7eb;
            border-radius: 8px;
        }
        
        QLabel#summaryLabel {
            font-size: 14px;
            color: #555;
        }
        
        QLabel#summaryValue {
            font-size: 14px;
            color: #333;
            min-width: 130px;
        }
        
        QLabel#totalLabel {
            font-size: 15px;
            font-weight: bold;
            color: #1a1a1a;
        }
        
        QLabel#totalValue {
            font-size: 18px;
            font-weight: bold;
            color: #dc2626;
            min-width: 150px;
        }
        
        QLabel#statusLabel {
            font-size: 13px;
            font-weight: bold;
            padding: 6px 16px;
            border-radius: 15px;
        }
        
        QLabel#footerText {
            font-size: 12px;
            color: #888;
            font-style: italic;
            padding: 8px;
        }
        
        QPushButton#exportBtn {
            background-color: #16a34a;
            color: white;
            font-size: 14px;
            font-weight: bold;
            border: none;
            border-radius: 8px;
            padding: 12px 25px;
        }
        
        QPushButton#exportBtn:hover {
            background-color: #15803d;
        }
        
        QPushButton#closeBtn {
            background-color: #6b7280;
            color: white;
            font-size: 14px;
            font-weight: bold;
            border: none;
            border-radius: 8px;
            padding: 12px 25px;
        }
        
        QPushButton#closeBtn:hover {
            background-color: #4b5563;
        }
    )");
}

void InvoiceDetailDialog::loadBookingData(const QString &id)
{
    m_booking = HeThongQuanLy::getInstance()->timDatSan(id.toStdString());
    if (!m_booking)
        return;

    // Store invoice metadata
    m_invoiceNumber = id;

    // Date
    NgayGio ntt = m_booking->getNgayThanhToan();
    if (ntt.getNam() > 0)
    {
        m_invoiceDate = QString::fromStdString(ntt.toString());
    }
    else
    {
        m_invoiceDate = QDate::currentDate().toString("dd/MM/yyyy");
    }

    m_invoiceNumberLabel->setText("Số: " + m_invoiceNumber);
    m_invoiceDateLabel->setText("Ngày: " + m_invoiceDate);

    // Customer Info
    if (m_booking->getKhachHang())
    {
        m_customerName = QString::fromStdString(m_booking->getKhachHang()->layHoTen());
        m_customerPhone = QString::fromStdString(m_booking->getKhachHang()->getSoDienThoai());
    }
    else
    {
        m_customerName = "Khách Vãng Lai";
        m_customerPhone = "N/A";
    }

    m_customerNameLabel->setText(m_customerName);
    m_customerPhoneLabel->setText(m_customerPhone);

    // Booking Info
    NgayGio thoiGianDat = m_booking->getThoiGianDat();
    QString ngayDat = QString("%1/%2/%3")
                          .arg(thoiGianDat.getNgay(), 2, 10, QChar('0'))
                          .arg(thoiGianDat.getThang(), 2, 10, QChar('0'))
                          .arg(thoiGianDat.getNam());

    m_bookingInfo = QString("🏟️ Sân: %1\n⏰ Khung giờ: %2 - %3\n📅 Ngày đặt: %4")
                        .arg(QString::fromStdString(m_booking->getSan()->layTenSan()))
                        .arg(QString::fromStdString(m_booking->getKhungGio().layGioBatDau().toString()))
                        .arg(QString::fromStdString(m_booking->getKhungGio().layGioKetThuc().toString()))
                        .arg(ngayDat);
    m_bookingInfoLabel->setText(m_bookingInfo);

    m_detailsTable->setRowCount(0);

    // Calculate field price
    double fieldPrice = 0;
    double hours = m_booking->getKhungGio().tinhSoGio();
    if (m_booking->getSan())
    {
        fieldPrice = m_booking->getSan()->getGiaThue() * hours;
    }

    // Row 1: Field Rental
    m_detailsTable->insertRow(0);
    m_detailsTable->setItem(0, 0, new QTableWidgetItem("1"));
    m_detailsTable->setItem(0, 1, new QTableWidgetItem(QString("Thuê sân %1 (%2 giờ)").arg(QString::fromStdString(m_booking->getSan()->layTenSan())).arg(hours, 0, 'f', 1)));
    m_detailsTable->setItem(0, 2, new QTableWidgetItem(QString::number(m_booking->getSan()->getGiaThue(), 'f', 0) + " VNĐ"));
    m_detailsTable->setItem(0, 3, new QTableWidgetItem(QString::number(hours, 'f', 1)));
    m_detailsTable->setItem(0, 4, new QTableWidgetItem(QString::number(fieldPrice, 'f', 0) + " VNĐ"));

    // Center align STT and SL columns
    m_detailsTable->item(0, 0)->setTextAlignment(Qt::AlignCenter);
    m_detailsTable->item(0, 3)->setTextAlignment(Qt::AlignCenter);
    m_detailsTable->item(0, 2)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_detailsTable->item(0, 4)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    // Services
    double servicePrice = 0;
    auto services = m_booking->getDanhSachDichVu();
    for (int i = 0; i < services.size(); ++i)
    {
        int row = m_detailsTable->rowCount();
        m_detailsTable->insertRow(row);

        double unitPrice = services[i].getDichVu()->layDonGia();
        double total = services[i].getSoLuong() * unitPrice;

        m_detailsTable->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
        m_detailsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(services[i].getDichVu()->layTenDichVu())));
        m_detailsTable->setItem(row, 2, new QTableWidgetItem(QString::number(unitPrice, 'f', 0) + " VNĐ"));
        m_detailsTable->setItem(row, 3, new QTableWidgetItem(QString::number(services[i].getSoLuong())));
        m_detailsTable->setItem(row, 4, new QTableWidgetItem(QString::number(total, 'f', 0) + " VNĐ"));

        // Alignment
        m_detailsTable->item(row, 0)->setTextAlignment(Qt::AlignCenter);
        m_detailsTable->item(row, 3)->setTextAlignment(Qt::AlignCenter);
        m_detailsTable->item(row, 2)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_detailsTable->item(row, 4)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        servicePrice += total;
    }

    // Summary
    double subtotal = fieldPrice + servicePrice;
    double deposit = m_booking->getTienCoc();
    double total = m_booking->getTongTien();

    m_subtotalLabel->setText(QString::number(subtotal, 'f', 0) + " VNĐ");
    m_depositLabel->setText("-" + QString::number(deposit, 'f', 0) + " VNĐ");
    m_discountLabel->setText("0 VNĐ");
    m_totalLabel->setText(QString::number(total, 'f', 0) + " VNĐ");

    // Amount in words (simplified)
    m_amountInWordsLabel->setText(QString("(Bằng chữ: %1 đồng)").arg(QString::number((qlonglong)total)));

    // Status
    QString statusText;
    QString statusStyle;
    if (m_booking->getTrangThai() == TrangThaiDatSan::HOAN_THANH)
    {
        statusText = "✓ ĐÃ THANH TOÁN";
        statusStyle = "background-color: #dcfce7; color: #16a34a;";
        m_status = "Hoàn thành";
    }
    else if (m_booking->getTrangThai() == TrangThaiDatSan::DA_HUY)
    {
        if (m_booking->getTrangThaiCoc() == TrangThaiCoc::MAT_COC)
        {
            statusText = "✗ PHẠT CỌC";
            statusStyle = "background-color: #fee2e2; color: #dc2626;";
            m_status = "Phạt cọc";
        }
        else
        {
            statusText = "↩ HOÀN CỌC";
            statusStyle = "background-color: #fef3c7; color: #d97706;";
            m_status = "Hoàn cọc";
        }
    }
    else
    {
        statusText = "⏳ CHỜ THANH TOÁN";
        statusStyle = "background-color: #fef3c7; color: #d97706;";
        m_status = "Chờ thanh toán";
    }

    m_statusLabel->setText(statusText);
    m_statusLabel->setStyleSheet(m_statusLabel->styleSheet() + statusStyle);
}

void InvoiceDetailDialog::loadServiceData(const QString &id)
{
    m_order = HeThongQuanLy::getInstance()->timDonHangDichVu(id.toStdString());
    if (!m_order)
        return;

    m_invoiceNumber = id;
    m_invoiceDate = QString::fromStdString(m_order->getNgayTao().toString());

    m_invoiceNumberLabel->setText("Số: " + m_invoiceNumber);
    m_invoiceDateLabel->setText("Ngày: " + m_invoiceDate);

    if (m_order->getKhachHang())
    {
        m_customerName = QString::fromStdString(m_order->getKhachHang()->layHoTen());
        m_customerPhone = QString::fromStdString(m_order->getKhachHang()->getSoDienThoai());
    }
    else
    {
        m_customerName = "Khách Vãng Lai";
        m_customerPhone = "N/A";
    }

    m_customerNameLabel->setText(m_customerName);
    m_customerPhoneLabel->setText(m_customerPhone);

    m_bookingInfo = "🛒 Mua dịch vụ tại quầy";
    m_bookingInfoLabel->setText(m_bookingInfo);

    m_detailsTable->setRowCount(0);
    auto services = m_order->getDanhSachDichVu();
    double servicePrice = 0;

    for (int i = 0; i < services.size(); ++i)
    {
        int row = m_detailsTable->rowCount();
        m_detailsTable->insertRow(row);

        double unitPrice = services[i].getDichVu()->layDonGia();
        double total = services[i].getSoLuong() * unitPrice;

        m_detailsTable->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
        m_detailsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(services[i].getDichVu()->layTenDichVu())));
        m_detailsTable->setItem(row, 2, new QTableWidgetItem(QString::number(unitPrice, 'f', 0) + " VNĐ"));
        m_detailsTable->setItem(row, 3, new QTableWidgetItem(QString::number(services[i].getSoLuong())));
        m_detailsTable->setItem(row, 4, new QTableWidgetItem(QString::number(total, 'f', 0) + " VNĐ"));

        // Alignment
        m_detailsTable->item(row, 0)->setTextAlignment(Qt::AlignCenter);
        m_detailsTable->item(row, 3)->setTextAlignment(Qt::AlignCenter);
        m_detailsTable->item(row, 2)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_detailsTable->item(row, 4)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        servicePrice += total;
    }

    // Summary
    m_subtotalLabel->setText(QString::number(servicePrice, 'f', 0) + " VNĐ");
    m_depositLabel->setText("0 VNĐ");
    m_discountLabel->setText("0 VNĐ");
    m_totalLabel->setText(QString::number(m_order->getThanhTien(), 'f', 0) + " VNĐ");

    m_amountInWordsLabel->setText(QString("(Bằng chữ: %1 đồng)").arg(QString::number((qlonglong)m_order->getThanhTien())));

    // Status
    QString statusText;
    QString statusStyle;
    switch (m_order->getTrangThai())
    {
    case TrangThaiDonHang::HOAN_THANH:
        statusText = "✓ ĐÃ THANH TOÁN";
        statusStyle = "background-color: #dcfce7; color: #16a34a;";
        m_status = "Hoàn thành";
        break;
    case TrangThaiDonHang::DA_HUY:
        statusText = "✗ ĐÃ HỦY";
        statusStyle = "background-color: #fee2e2; color: #dc2626;";
        m_status = "Đã hủy";
        break;
    default:
        statusText = "⏳ CHỜ XỬ LÝ";
        statusStyle = "background-color: #fef3c7; color: #d97706;";
        m_status = "Chờ xử lý";
        break;
    }

    m_statusLabel->setText(statusText);
    m_statusLabel->setStyleSheet(m_statusLabel->styleSheet() + statusStyle);
}

void InvoiceDetailDialog::onExportPdfClicked()
{
    QString defaultName = QString("HoaDon_%1_%2.pdf")
                              .arg(m_invoiceNumber)
                              .arg(QDate::currentDate().toString("ddMMyyyy"));

    QString fileName = QFileDialog::getSaveFileName(this, "Xuất Hoá Đơn PDF", defaultName, "PDF Files (*.pdf)");
    if (fileName.isEmpty())
        return;

    if (QFileInfo(fileName).suffix().isEmpty())
    {
        fileName.append(".pdf");
    }

    printInvoice(fileName);
}

void InvoiceDetailDialog::printInvoice(const QString &filename)
{
    QPdfWriter writer(filename);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageMargins(QMarginsF(20, 20, 20, 20), QPageLayout::Millimeter);
    writer.setResolution(300);

    QPainter painter(&writer);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    int pageWidth = writer.width();
    int pageHeight = writer.height();
    int margin = 150;
    int contentWidth = pageWidth - 2 * margin;
    int y = margin;

    // Colors
    QColor primaryColor(22, 163, 74); // Green
    QColor textColor(26, 26, 26);
    QColor grayColor(100, 100, 100);
    QColor lightGray(240, 240, 240);

    // ===== HEADER =====
    // Company Name
    QFont companyFont("Arial", 16, QFont::Bold);
    painter.setFont(companyFont);
    painter.setPen(primaryColor);
    painter.drawText(margin, y, "SÂN BÓNG ĐÁ DUT");
    y += 80;

    // Company Info
    QFont smallFont("Arial", 9);
    painter.setFont(smallFont);
    painter.setPen(grayColor);
    painter.drawText(margin, y, "Địa chỉ: 54 Nguyễn Lương Bằng, Liên Chiểu, Đà Nẵng");
    y += 50;
    painter.drawText(margin, y, "Hotline: 0236 3841 999");
    y += 80;

    // Separator line
    painter.setPen(QPen(primaryColor, 3));
    painter.drawLine(margin, y, pageWidth - margin, y);
    y += 80;

    // ===== INVOICE TITLE =====
    QFont titleFont("Arial", 22, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(textColor);
    QRect titleRect(margin, y, contentWidth, 100);
    painter.drawText(titleRect, Qt::AlignCenter, "HÓA ĐƠN THANH TOÁN");
    y += 120;

    // Invoice Number & Date
    QFont metaFont("Arial", 10);
    painter.setFont(metaFont);
    painter.setPen(grayColor);
    QString metaText = QString("Số: %1          Ngày: %2").arg(m_invoiceNumber).arg(m_invoiceDate);
    QRect metaRect(margin, y, contentWidth, 50);
    painter.drawText(metaRect, Qt::AlignCenter, metaText);
    y += 100;

    // ===== CUSTOMER INFO BOX =====
    QFont sectionFont("Arial", 11, QFont::Bold);
    QFont normalFont("Arial", 10);

    // Box background
    int boxHeight = 180;
    painter.fillRect(margin, y, contentWidth, boxHeight, lightGray);
    painter.setPen(QPen(QColor(200, 200, 200), 2));
    painter.drawRect(margin, y, contentWidth, boxHeight);

    y += 50;
    painter.setFont(sectionFont);
    painter.setPen(textColor);
    painter.drawText(margin + 30, y, "THÔNG TIN KHÁCH HÀNG");
    y += 60;

    painter.setFont(normalFont);
    painter.drawText(margin + 30, y, QString("Họ tên: %1").arg(m_customerName));
    painter.drawText(margin + contentWidth / 2, y, QString("Điện thoại: %1").arg(m_customerPhone));
    y += boxHeight - 60;

    // ===== BOOKING INFO =====
    y += 50;
    painter.setFont(sectionFont);
    painter.setPen(textColor);
    painter.drawText(margin, y, "THÔNG TIN ĐẶT SÂN");
    y += 50;

    painter.setFont(normalFont);
    painter.setPen(grayColor);
    // Clean up booking info for PDF (remove emojis)
    QString cleanBookingInfo = m_bookingInfo;
    cleanBookingInfo.replace("🏟️", "-");
    cleanBookingInfo.replace("⏰", "-");
    cleanBookingInfo.replace("📅", "-");
    cleanBookingInfo.replace("🛒", "-");
    cleanBookingInfo.replace("\n", "    ");
    painter.drawText(margin, y, cleanBookingInfo);
    y += 80;

    // ===== DETAILS TABLE =====
    painter.setFont(sectionFont);
    painter.setPen(textColor);
    painter.drawText(margin, y, "CHI TIẾT DỊCH VỤ");
    y += 60;

    // Table Header
    int col1 = margin;                   // STT
    int col2 = margin + 80;              // Description
    int col3 = pageWidth - margin - 450; // Unit Price
    int col4 = pageWidth - margin - 280; // Qty
    int col5 = pageWidth - margin - 180; // Total
    int rowHeight = 60;

    // Header background
    painter.fillRect(margin, y - 40, contentWidth, rowHeight, primaryColor);

    QFont tableHeaderFont("Arial", 10, QFont::Bold);
    painter.setFont(tableHeaderFont);
    painter.setPen(Qt::white);
    painter.drawText(col1 + 10, y, "STT");
    painter.drawText(col2, y, "Mô tả");
    painter.drawText(col3, y, "Đơn giá");
    painter.drawText(col4, y, "SL");
    painter.drawText(col5, y, "Thành tiền");
    y += rowHeight;

    // Table Rows
    QFont tableFont("Arial", 10);
    painter.setFont(tableFont);
    painter.setPen(textColor);

    for (int i = 0; i < m_detailsTable->rowCount(); ++i)
    {
        // Alternate row background
        if (i % 2 == 0)
        {
            painter.fillRect(margin, y - 40, contentWidth, rowHeight, QColor(250, 250, 250));
        }

        painter.setPen(textColor);
        painter.drawText(col1 + 20, y, m_detailsTable->item(i, 0)->text());
        painter.drawText(col2, y, m_detailsTable->item(i, 1)->text());
        painter.drawText(col3, y, m_detailsTable->item(i, 2)->text());
        painter.drawText(col4 + 10, y, m_detailsTable->item(i, 3)->text());
        painter.drawText(col5, y, m_detailsTable->item(i, 4)->text());

        // Row separator
        painter.setPen(QPen(QColor(230, 230, 230), 1));
        painter.drawLine(margin, y + 20, pageWidth - margin, y + 20);

        y += rowHeight;
    }

    y += 40;

    // ===== SUMMARY =====
    int summaryX = pageWidth - margin - 500;
    int valueX = pageWidth - margin - 180;

    painter.setFont(normalFont);
    painter.setPen(textColor);

    painter.drawText(summaryX, y, "Tạm tính:");
    painter.drawText(valueX, y, m_subtotalLabel->text());
    y += 50;

    painter.setPen(primaryColor);
    painter.drawText(summaryX, y, "Đã cọc:");
    painter.drawText(valueX, y, m_depositLabel->text());
    y += 50;

    painter.drawText(summaryX, y, "Giảm giá:");
    painter.drawText(valueX, y, m_discountLabel->text());
    y += 50;

    // Total separator
    painter.setPen(QPen(textColor, 2));
    painter.drawLine(summaryX, y, pageWidth - margin, y);
    y += 60;

    // Total
    QFont totalFont("Arial", 14, QFont::Bold);
    painter.setFont(totalFont);
    painter.setPen(textColor);
    painter.drawText(summaryX, y, "TỔNG THANH TOÁN:");

    painter.setPen(QColor(220, 38, 38)); // Red
    painter.drawText(valueX, y, m_totalLabel->text());
    y += 80;

    // Amount in words
    painter.setFont(QFont("Arial", 9, QFont::Normal, true)); // Italic
    painter.setPen(grayColor);
    painter.drawText(margin, y, m_amountInWordsLabel->text());
    y += 100;

    // ===== STATUS =====
    painter.setFont(QFont("Arial", 11, QFont::Bold));
    QString statusForPdf = QString("Trạng thái: %1").arg(m_status);
    painter.setPen(primaryColor);
    QRect statusRect(margin, y, contentWidth, 50);
    painter.drawText(statusRect, Qt::AlignRight, statusForPdf);
    y += 100;

    // ===== FOOTER =====
    // Separator
    painter.setPen(QPen(QColor(200, 200, 200), 1));
    painter.drawLine(margin, y, pageWidth - margin, y);
    y += 60;

    painter.setFont(QFont("Arial", 10, QFont::Normal, true));
    painter.setPen(grayColor);
    QRect footerRect(margin, y, contentWidth, 100);
    painter.drawText(footerRect, Qt::AlignCenter, "Cảm ơn quý khách đã sử dụng dịch vụ! Hẹn gặp lại quý khách!");

    painter.end();

    QMessageBox::information(this, "Thành Công", "Đã xuất hoá đơn ra file PDF thành công!");
    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));
}
