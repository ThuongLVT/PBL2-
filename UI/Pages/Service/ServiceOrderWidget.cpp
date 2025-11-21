#include "ServiceOrderWidget.h"
#include "../../UI/Dialogs/ServiceSelectionDialog.h"
#include "../../Core/QuanLy/HeThongQuanLy.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>
#include <QDate>
#include <QStringListModel>
#include <QCompleter>
#include <QRegularExpressionValidator>
#include <QTimer>

ServiceOrderWidget::ServiceOrderWidget(QWidget *parent) : QWidget(parent)
{
    qlDichVu = HeThongQuanLy::getInstance()->layQuanLyDichVu();
    qlKhachHang = HeThongQuanLy::getInstance()->layQuanLyKhachHang();

    isCustomerFound = false;
    currentDiscountRate = 0.0;

    setupUI();
    refreshData();
}

ServiceOrderWidget::~ServiceOrderWidget()
{
}

void ServiceOrderWidget::setupUI()
{
    // Main Layout with Light Gray Background
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(20);
    this->setStyleSheet("background-color: #f3f4f6;"); // Light Gray Background

    // Helper to create styled panels with header
    auto createPanel = [this](const QString &title, QLayout *contentLayout) -> QFrame *
    {
        QFrame *panel = new QFrame(this);
        panel->setStyleSheet("background-color: white; border-radius: 10px; border: 1px solid #e5e7eb;");
        panel->setAttribute(Qt::WA_StyledBackground, true);

        QVBoxLayout *panelLayout = new QVBoxLayout(panel);
        panelLayout->setContentsMargins(0, 0, 0, 0);
        panelLayout->setSpacing(0);

        // Header
        QLabel *lblTitle = new QLabel(title);
        lblTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #111827; padding: 15px 20px; border: none; background-color: transparent;");
        panelLayout->addWidget(lblTitle);

        // Divider
        QFrame *line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Plain);
        line->setFixedHeight(1);
        line->setStyleSheet("background-color: #e5e7eb; border: none;");
        panelLayout->addWidget(line);

        // Content Area
        QWidget *contentWidget = new QWidget();
        contentWidget->setStyleSheet("background-color: white; border: none; border-radius: 0px; border-bottom-left-radius: 10px; border-bottom-right-radius: 10px;");
        contentWidget->setLayout(contentLayout);

        // Ensure content layout has margins
        contentLayout->setContentsMargins(20, 20, 20, 20);

        panelLayout->addWidget(contentWidget);

        return panel;
    };

    // ================= LEFT SIDE (60%) - CART =================
    QVBoxLayout *cartContentLayout = new QVBoxLayout();
    cartContentLayout->setSpacing(15);

    cartTable = new QTableWidget(this);
    cartTable->setColumnCount(7); // Increased to 7 for Unit
    cartTable->setHorizontalHeaderLabels({"STT", "Mã DV", "Tên dịch vụ", "Đơn giá", "Đơn vị", "Số lượng", "Thành tiền"});

    cartTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    cartTable->setColumnWidth(0, 50);                                             // STT width
    cartTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch); // Name stretch
    cartTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    cartTable->setColumnWidth(4, 80); // Unit width

    // Hide vertical header (row numbers) as requested
    cartTable->verticalHeader()->setVisible(false);

    cartTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    cartTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Updated stylesheet: Remove focus outline, custom grid lines, lighter blue selection
    cartTable->setStyleSheet(
        "QTableWidget { border: 1px solid #d1d5db; border-radius: 5px; background-color: white; outline: none; }"
        "QHeaderView::section { background-color: #f9fafb; border: none; border-bottom: 1px solid #d1d5db; padding: 5px; }"
        "QTableWidget::item { border-bottom: 1px solid #e5e7eb; color: #374151; }"
        "QTableWidget::item:selected { background-color: #60a5fa; color: white; }"
        "QTableWidget::item:focus { border: none; outline: none; }");
    cartTable->setShowGrid(false); // Using item border-bottom instead

    cartContentLayout->addWidget(cartTable);

    // Set Header Alignments
    if (cartTable->horizontalHeaderItem(0))
        cartTable->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignCenter);
    if (cartTable->horizontalHeaderItem(1))
        cartTable->horizontalHeaderItem(1)->setTextAlignment(Qt::AlignCenter);
    if (cartTable->horizontalHeaderItem(2))
        cartTable->horizontalHeaderItem(2)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    if (cartTable->horizontalHeaderItem(3))
        cartTable->horizontalHeaderItem(3)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    if (cartTable->horizontalHeaderItem(4))
        cartTable->horizontalHeaderItem(4)->setTextAlignment(Qt::AlignCenter);
    if (cartTable->horizontalHeaderItem(5))
        cartTable->horizontalHeaderItem(5)->setTextAlignment(Qt::AlignCenter);
    if (cartTable->horizontalHeaderItem(6))
        cartTable->horizontalHeaderItem(6)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QHBoxLayout *cartBtnLayout = new QHBoxLayout();

    btnAddService = new QPushButton("Thêm dịch vụ", this);
    btnAddService->setIcon(QIcon(":/icons/add.png"));
    btnAddService->setCursor(Qt::PointingHandCursor);
    btnAddService->setStyleSheet("QPushButton { background-color: #3b82f6; color: white; font-weight: bold; padding: 8px 15px; border-radius: 5px; border: none; } QPushButton:hover { background-color: #2563eb; }");

    btnRemoveService = new QPushButton("Xóa dịch vụ", this);
    btnRemoveService->setIcon(QIcon(":/icons/delete.png"));
    btnRemoveService->setCursor(Qt::PointingHandCursor);
    btnRemoveService->setStyleSheet("QPushButton { background-color: #ef4444; color: white; font-weight: bold; padding: 8px 15px; border-radius: 5px; border: none; } QPushButton:hover { background-color: #dc2626; }");

    btnClearCart = new QPushButton("Làm mới giỏ hàng", this);
    btnClearCart->setCursor(Qt::PointingHandCursor);
    btnClearCart->setStyleSheet("QPushButton { background-color: #6b7280; color: white; font-weight: bold; padding: 8px 15px; border-radius: 5px; border: none; } QPushButton:hover { background-color: #4b5563; }");

    cartBtnLayout->addWidget(btnAddService);
    cartBtnLayout->addWidget(btnRemoveService);
    cartBtnLayout->addWidget(btnClearCart);
    cartBtnLayout->addStretch();

    cartContentLayout->addLayout(cartBtnLayout);

    QFrame *leftPanel = createPanel("Giỏ hàng dịch vụ", cartContentLayout);

    // ================= RIGHT SIDE (40%) =================
    QWidget *rightWidget = new QWidget(this);
    rightWidget->setStyleSheet("background-color: transparent;"); // Ensure transparency
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(20);

    // --- Customer Info (Top 40% of Right) ---
    QVBoxLayout *customerContentLayout = new QVBoxLayout();
    customerContentLayout->setSpacing(15);

    // Helper for form rows
    auto addFormRow = [&](const QString &label, QWidget *widget)
    {
        QHBoxLayout *row = new QHBoxLayout();
        QLabel *lbl = new QLabel(label);
        lbl->setFixedWidth(110);
        lbl->setStyleSheet("color: #374151; border: none; background-color: transparent;");

        if (qobject_cast<QLineEdit *>(widget))
        {
            // Removed read-only background color as requested
            widget->setStyleSheet("QLineEdit { border: 1px solid #d1d5db; border-radius: 5px; padding: 5px; background-color: white; } QLineEdit:focus { border-color: #3b82f6; } QLineEdit:read-only { color: #6b7280; }");
        }

        row->addWidget(lbl);
        row->addWidget(widget);
        customerContentLayout->addLayout(row);
    };

    txtPhone = new QLineEdit(this);
    txtPhone->setPlaceholderText("Nhập SĐT (Gợi ý)...");
    // Validator: Starts with 0, digits only, max 10
    QRegularExpression rxPhone("^0[0-9]{0,9}$");
    txtPhone->setValidator(new QRegularExpressionValidator(rxPhone, this));

    // Completer setup for Phone
    phoneCompleter = new QCompleter(this);
    phoneCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    phoneCompleter->setFilterMode(Qt::MatchContains);
    txtPhone->setCompleter(phoneCompleter);

    addFormRow("Số điện thoại:", txtPhone);

    txtCustomerName = new QLineEdit(this);
    txtCustomerName->setPlaceholderText("Tên khách hàng (Gợi ý)...");
    // Validator: Letters and spaces only
    QRegularExpression rxName("^[\\p{L}\\s]*$");
    txtCustomerName->setValidator(new QRegularExpressionValidator(rxName, this));

    // Completer setup for Name
    QCompleter *nameCompleter = new QCompleter(this);
    nameCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    nameCompleter->setFilterMode(Qt::MatchContains);
    txtCustomerName->setCompleter(nameCompleter);

    addFormRow("Tên khách hàng:", txtCustomerName);

    txtCustomerRank = new QLineEdit(this);
    txtCustomerRank->setPlaceholderText("Hạng thành viên");
    txtCustomerRank->setReadOnly(true);
    addFormRow("Hạng thành viên:", txtCustomerRank);

    customerContentLayout->addStretch();

    QFrame *customerPanel = createPanel("Thông tin khách hàng", customerContentLayout);

    // --- Payment Info (Bottom 60% of Right) ---
    QVBoxLayout *paymentContentLayout = new QVBoxLayout();
    paymentContentLayout->setSpacing(15);

    QGridLayout *gridPayment = new QGridLayout();
    gridPayment->setVerticalSpacing(15);

    lblSubTotal = new QLabel("0 VNĐ", this);
    lblDiscount = new QLabel("0 VNĐ", this);
    lblFinalTotal = new QLabel("0 VNĐ", this);

    QString labelStyle = "color: #374151; border: none; background-color: transparent;";
    QString valueStyle = "font-weight: bold; color: #111827; border: none; background-color: transparent;";
    QString finalValueStyle = "font-weight: bold; font-size: 18px; color: #ef4444; border: none; background-color: transparent;";

    auto addPaymentRow = [&](int row, const QString &text, QLabel *valueLbl, const QString &style)
    {
        QLabel *lbl = new QLabel(text);
        lbl->setStyleSheet(labelStyle);
        valueLbl->setStyleSheet(style);
        valueLbl->setAlignment(Qt::AlignRight);
        gridPayment->addWidget(lbl, row, 0);
        gridPayment->addWidget(valueLbl, row, 1);
    };

    addPaymentRow(0, "Tổng tiền hàng:", lblSubTotal, valueStyle);
    addPaymentRow(1, "Giảm giá:", lblDiscount, valueStyle);
    addPaymentRow(2, "THÀNH TIỀN:", lblFinalTotal, finalValueStyle);

    paymentContentLayout->addLayout(gridPayment);
    paymentContentLayout->addStretch();

    btnPay = new QPushButton("THANH TOÁN", this);
    btnPay->setCursor(Qt::PointingHandCursor);
    btnPay->setMinimumHeight(50);
    btnPay->setStyleSheet("QPushButton { background-color: #10b981; color: white; font-weight: bold; font-size: 16px; border-radius: 5px; border: none; } QPushButton:hover { background-color: #059669; }");
    paymentContentLayout->addWidget(btnPay);

    QFrame *paymentPanel = createPanel("Thanh toán", paymentContentLayout);

    rightLayout->addWidget(customerPanel, 4); // 40% height
    rightLayout->addWidget(paymentPanel, 6);  // 60% height

    // Add to main layout
    mainLayout->addWidget(leftPanel, 6);   // 60% width
    mainLayout->addWidget(rightWidget, 4); // 40% width

    // Connections
    connect(btnAddService, &QPushButton::clicked, this, &ServiceOrderWidget::onAddServiceClicked);
    connect(btnRemoveService, &QPushButton::clicked, this, &ServiceOrderWidget::onRemoveServiceClicked);
    connect(btnClearCart, &QPushButton::clicked, this, &ServiceOrderWidget::onClearCartClicked);
    connect(btnPay, &QPushButton::clicked, this, &ServiceOrderWidget::onPayClicked);

    // Completer Logic - Shared Handler
    auto handleCompletion = [this](const QString &text)
    {
        // Text format: "Phone - Name"
        QStringList parts = text.split(" - ");
        if (parts.size() >= 2)
        {
            QString phone = parts.first().trimmed();
            // Use QTimer to ensure this runs after the completer has finished its default action
            QTimer::singleShot(0, this, [this, phone]()
                               {
                                   txtPhone->setText(phone);
                                   onPhoneSearch(); // This will fill Name and Rank
                               });
        }
    };

    connect(phoneCompleter, QOverload<const QString &>::of(&QCompleter::activated), this, handleCompletion);
    if (txtCustomerName->completer())
    {
        connect(txtCustomerName->completer(), QOverload<const QString &>::of(&QCompleter::activated), this, handleCompletion);
    }

    // Also trigger on enter or editing finished if valid phone
    connect(txtPhone, &QLineEdit::returnPressed, this, &ServiceOrderWidget::onPhoneSearch);
}

void ServiceOrderWidget::refreshData()
{
    // Reload customer list for completer
    QStringList customerList;
    const MangDong<KhachHang *> &customers = qlKhachHang->layDanhSachKhachHang();
    for (int i = 0; i < customers.size(); i++)
    {
        KhachHang *kh = customers[i];
        QString item = QString::fromStdString(kh->laySoDienThoai()) + " - " + QString::fromStdString(kh->layHoTen());
        customerList << item;
    }

    QStringListModel *model = new QStringListModel(customerList, this);
    if (phoneCompleter)
    {
        phoneCompleter->setModel(model);
    }
    if (txtCustomerName->completer())
    {
        txtCustomerName->completer()->setModel(model);
    }
}

void ServiceOrderWidget::onAddServiceClicked()
{
    ServiceSelectionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QMap<std::string, int> selected = dialog.getSelectedServices();

        // Merge with current cart
        for (auto it = selected.begin(); it != selected.end(); ++it)
        {
            std::string id = it.key();
            int qty = it.value();

            if (currentCart.contains(id))
            {
                currentCart[id] += qty;
            }
            else
            {
                currentCart.insert(id, qty);
            }
        }
        updateCartTable();
    }
}

void ServiceOrderWidget::onRemoveServiceClicked()
{
    if (cartTable->selectedItems().isEmpty())
    {
        QMessageBox::warning(this, "Thông báo", "Phải chọn dịch vụ cần xoá!");
        return;
    }

    int row = cartTable->currentRow();
    if (row >= 0)
    {
        // ID is now at column 1 (0: STT, 1: ID)
        QString id = cartTable->item(row, 1)->text();
        currentCart.remove(id.toStdString());
        cartTable->removeRow(row);
        updateTotals();
        // Re-index STT
        for (int i = 0; i < cartTable->rowCount(); ++i)
        {
            cartTable->item(i, 0)->setText(QString::number(i + 1));
        }
    }
}

void ServiceOrderWidget::onClearCartClicked()
{
    if (currentCart.isEmpty())
        return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Xác nhận", "Bạn có chắc chắn muốn làm mới giỏ hàng không?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        currentCart.clear();
        cartTable->setRowCount(0);
        updateTotals();
    }
}

void ServiceOrderWidget::updateCartTable()
{
    cartTable->setRowCount(0);
    int stt = 1;

    for (auto it = currentCart.begin(); it != currentCart.end(); ++it)
    {
        std::string id = it.key();
        int qty = it.value();

        DichVu *dv = qlDichVu->timDichVu(id);
        if (dv)
        {
            int row = cartTable->rowCount();
            cartTable->insertRow(row);

            // STT (Col 0) - Center
            QTableWidgetItem *sttItem = new QTableWidgetItem(QString::number(stt++));
            sttItem->setTextAlignment(Qt::AlignCenter);
            cartTable->setItem(row, 0, sttItem);

            // ID (Col 1) - Center
            QTableWidgetItem *idItem = new QTableWidgetItem(QString::fromStdString(dv->layMaDichVu()));
            idItem->setTextAlignment(Qt::AlignCenter);
            cartTable->setItem(row, 1, idItem);

            // Name (Col 2) - Left
            QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(dv->layTenDichVu()));
            nameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            cartTable->setItem(row, 2, nameItem);

            // Price (Col 3) - Right
            QTableWidgetItem *priceItem = new QTableWidgetItem(QString::number(dv->layDonGia(), 'f', 0) + "đ");
            priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            cartTable->setItem(row, 3, priceItem);

            // Unit (Col 4) - Center
            QTableWidgetItem *unitItem = new QTableWidgetItem(QString::fromStdString(dv->layDonVi()));
            unitItem->setTextAlignment(Qt::AlignCenter);
            cartTable->setItem(row, 4, unitItem);

            // Qty (Col 5) - Center
            QTableWidgetItem *qtyItem = new QTableWidgetItem(QString::number(qty));
            qtyItem->setTextAlignment(Qt::AlignCenter);
            cartTable->setItem(row, 5, qtyItem);

            // Total (Col 6) - Right
            QTableWidgetItem *totalItem = new QTableWidgetItem(QString::number(dv->layDonGia() * qty, 'f', 0) + "đ");
            totalItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            cartTable->setItem(row, 6, totalItem);
        }
    }
    updateTotals();
}

void ServiceOrderWidget::onPhoneSearch()
{
    QString text = txtPhone->text().trimmed();
    if (text.isEmpty())
        return;

    // Handle "Phone - Name" format if present
    QString phone = text.split(" - ").first().trimmed();

    KhachHang *kh = qlKhachHang->timKhachHangTheoSDT(phone.toStdString());
    if (kh)
    {
        isCustomerFound = true;
        currentCustomerId = kh->layMaKhachHang();
        txtCustomerName->setText(QString::fromStdString(kh->layHoTen()));
        txtCustomerRank->setText(QString::fromStdString(kh->layTenHang()));

        // Simple discount logic based on rank (Example)
        std::string rank = kh->layTenHang();
        if (rank == "VIP" || rank == "Kim Cương")
            currentDiscountRate = 0.1; // 10%
        else if (rank == "Gold" || rank == "Vàng")
            currentDiscountRate = 0.05; // 5%
        else if (rank == "Silver" || rank == "Bạc")
            currentDiscountRate = 0.02; // 2%
        else
            currentDiscountRate = 0.0;

        // Don't show popup on every search, just fill data
        // QMessageBox::information(this, "Tìm thấy", "Đã tìm thấy khách hàng: " + QString::fromStdString(kh->layHoTen()));
    }
    else
    {
        isCustomerFound = false;
        currentCustomerId = "";
        currentDiscountRate = 0.0;
        txtCustomerName->clear();
        txtCustomerRank->clear();
        // Only warn if it looks like a complete phone number (e.g. > 9 digits)
        if (phone.length() >= 9)
        {
            QMessageBox::warning(this, "Không tìm thấy", "Không tìm thấy khách hàng với SĐT này!");
        }
    }
    updateTotals();
}

void ServiceOrderWidget::updateTotals()
{
    double subTotal = 0;

    for (auto it = currentCart.begin(); it != currentCart.end(); ++it)
    {
        std::string id = it.key();
        int qty = it.value();
        DichVu *dv = qlDichVu->timDichVu(id);
        if (dv)
        {
            subTotal += dv->layDonGia() * qty;
        }
    }

    double discount = subTotal * currentDiscountRate;
    double finalTotal = subTotal - discount;

    lblSubTotal->setText(QString::number((long long)subTotal) + "đ");
    lblDiscount->setText(QString::number((long long)discount) + "đ");
    lblFinalTotal->setText(QString::number((long long)finalTotal) + "đ");
}

void ServiceOrderWidget::onPayClicked()
{
    if (currentCart.isEmpty())
    {
        QMessageBox::warning(this, "Lỗi", "Giỏ hàng đang trống!");
        return;
    }

    if (!isCustomerFound)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Xác nhận", "Chưa chọn khách hàng. Bạn có muốn thanh toán cho khách vãng lai không?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
            return;
    }

    // Create Order
    KhachHang *kh = isCustomerFound ? qlKhachHang->timKhachHang(currentCustomerId) : nullptr;
    DonHangDichVu *donHang = HeThongQuanLy::getInstance()->taoDonHangDichVu(kh);

    if (!donHang)
    {
        QMessageBox::critical(this, "Lỗi", "Không thể tạo đơn hàng!");
        return;
    }

    // Add items
    for (auto it = currentCart.begin(); it != currentCart.end(); ++it)
    {
        std::string id = it.key();
        int qty = it.value();
        DichVu *dv = qlDichVu->timDichVu(id);
        if (dv)
        {
            DichVuDat dvd(dv, qty);
            donHang->themDichVu(dvd);
        }
    }

    // Calculate totals
    donHang->tinhTongTien();
    donHang->tinhGiamGia();
    donHang->tinhThanhTien();
    donHang->setTrangThai(TrangThaiDonHang::HOAN_THANH); // Auto complete for now

    // Save System
    // Note: taoDonHangDichVu already adds it to the manager list, but we might need to trigger a save to file
    // HeThongQuanLy::getInstance()->luuHeThong("Data/data.bin"); // Or similar if needed

    QMessageBox::information(this, "Thành công", "Thanh toán đơn hàng thành công!\nMã đơn: " + QString::fromStdString(donHang->getMaDonHang()));

    // Reset UI
    onClearCartClicked();
    txtPhone->clear();
    txtCustomerName->clear();
    txtCustomerRank->clear();
    isCustomerFound = false;
    currentDiscountRate = 0.0;
    updateTotals();
}

void ServiceOrderWidget::onTableItemChanged(QTableWidgetItem *item)
{
    // Placeholder for future implementation
}
