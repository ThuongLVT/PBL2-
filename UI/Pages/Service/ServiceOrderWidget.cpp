/**
 * @file ServiceOrderWidget.cpp
 * @brief Implementation of Service Order Widget (Tab 1)
 * @author khninh22 - Service Module
 * @date 2025-11-14
 */

#include "ServiceOrderWidget.h"
#include "../../Dialogs/AddCustomerDialog.h"
#include <QHeaderView>
#include <QFont>
#include <QDateTime>
#include <QDebug>
#include <QFile>

ServiceOrderWidget::ServiceOrderWidget(QWidget *parent)
    : QWidget(parent),
      system(HeThongQuanLy::getInstance()),
      selectedCustomer(nullptr),
      totalPrice(0),
      discount(0),
      finalAmount(0)
{
    setupUI();
    setupConnections();
    applyStyles();
    loadCustomers();
}

ServiceOrderWidget::~ServiceOrderWidget()
{
}



void ServiceOrderWidget::setupUI()
{
    // Main Scroll Area to prevent layout issues on small screens
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea { background-color: transparent; }");

    QWidget *contentWidget = new QWidget();
    contentWidget->setStyleSheet("background-color: transparent;");
    
    // Main Horizontal Layout (7|3 Split)
    mainLayout = new QHBoxLayout(contentWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    // --- LEFT COLUMN (70%) ---
    QWidget *leftWidget = new QWidget();
    leftColumnLayout = new QVBoxLayout(leftWidget);
    leftColumnLayout->setContentsMargins(0, 0, 0, 0);
    leftColumnLayout->setSpacing(20);
    
    // 1. Customer Section
    setupCustomerSection();

    // 2. Select Service Button
    selectServiceBtn = new QPushButton("🛒 CHỌN / THÊM DỊCH VỤ", this);
    selectServiceBtn->setObjectName("selectServiceBtn");
    selectServiceBtn->setCursor(Qt::PointingHandCursor);
    selectServiceBtn->setFixedHeight(60);
    selectServiceBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #16a34a;
            color: white;
            font-weight: bold;
            font-size: 16px;
            border-radius: 8px;
            text-align: center;
        }
        QPushButton:hover {
            background-color: #15803d;
        }
    )");
    leftColumnLayout->addWidget(selectServiceBtn);

    // 3. Cart Section (Table)
    setupCartSection();

    // Add Left Widget to Main Layout with Stretch Factor 7
    mainLayout->addWidget(leftWidget, 7);


    // --- RIGHT COLUMN (30%) ---
    QWidget *rightWidget = new QWidget();
    rightColumnLayout = new QVBoxLayout(rightWidget);
    rightColumnLayout->setContentsMargins(0, 0, 0, 0);
    rightColumnLayout->setSpacing(20);

    // 4. Payment Section
    setupPaymentSection();
    
    // Add spacer to push payment section to top if needed, or let it expand
    rightColumnLayout->addStretch();

    // Add Right Widget to Main Layout with Stretch Factor 3
    mainLayout->addWidget(rightWidget, 3);

    // Set scroll area widget
    scrollArea->setWidget(contentWidget);

    // Main layout for the widget itself
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->addWidget(scrollArea);
}

void ServiceOrderWidget::setupCustomerSection()
{
    customerFrame = new QFrame(this);
    customerFrame->setObjectName("infoFrame");
    QVBoxLayout *layout = new QVBoxLayout(customerFrame);
    layout->setSpacing(15);

    QLabel *title = new QLabel("👤 THÔNG TIN KHÁCH HÀNG", this);
    title->setObjectName("sectionTitle");
    layout->addWidget(title);

    QGridLayout *formLayout = new QGridLayout();
    formLayout->setColumnStretch(1, 1);

    // Phone Search
    QLabel *phoneLbl = new QLabel("Số điện thoại:", this);
    QHBoxLayout *phoneLayout = new QHBoxLayout();
    phoneLineEdit = new QLineEdit(this);
    phoneLineEdit->setPlaceholderText("Nhập SĐT để tìm...");
    QPushButton *addCustomerBtn = new QPushButton("+", this);
    addCustomerBtn->setFixedSize(30, 30);
    addCustomerBtn->setToolTip("Thêm khách hàng mới");
    connect(addCustomerBtn, &QPushButton::clicked, this, &ServiceOrderWidget::onAddNewCustomerClicked);
    
    phoneLayout->addWidget(phoneLineEdit);
    phoneLayout->addWidget(addCustomerBtn);

    // Customer Dropdown
    QLabel *nameLbl = new QLabel("Khách hàng:", this);
    customerComboBox = new QComboBox(this);
    customerComboBox->setEditable(true);

    // Info Labels
    customerNameLabel = new QLabel("---", this);
    membershipLabel = new QLabel("Hạng: ---", this);
    membershipLabel->setStyleSheet("color: #6b7280; font-style: italic;");

    formLayout->addWidget(phoneLbl, 0, 0);
    formLayout->addLayout(phoneLayout, 0, 1);
    formLayout->addWidget(nameLbl, 1, 0);
    formLayout->addWidget(customerComboBox, 1, 1);
    formLayout->addWidget(new QLabel("Hạng thành viên:", this), 2, 0);
    formLayout->addWidget(membershipLabel, 2, 1);

    layout->addLayout(formLayout);
    
    // Add to Left Column
    leftColumnLayout->addWidget(customerFrame);
}

void ServiceOrderWidget::setupCartSection()
{
    QLabel *cartTitle = new QLabel("📋 GIỎ HÀNG", this);
    cartTitle->setObjectName("sectionTitle");
    leftColumnLayout->addWidget(cartTitle);

    cartTable = new QTableWidget(0, 6, this);
    cartTable->setObjectName("dataTable");
    cartTable->setHorizontalHeaderLabels({"Ảnh", "Thông tin dịch vụ", "Đơn giá", "Số lượng", "Thành tiền", ""});
    
    // Column widths
    cartTable->setColumnWidth(0, 80);  // Image
    cartTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); // Name
    cartTable->setColumnWidth(2, 100); // Price
    cartTable->setColumnWidth(3, 150); // Quantity
    cartTable->setColumnWidth(4, 120); // Total
    cartTable->setColumnWidth(5, 50);  // Delete
    
    cartTable->verticalHeader()->setVisible(false);
    cartTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    cartTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    cartTable->setAlternatingRowColors(true);
    cartTable->setShowGrid(false);
    cartTable->setStyleSheet("QTableWidget { border: 1px solid #e5e7eb; border-radius: 8px; background-color: white; } QTableWidget::item { border-bottom: 1px solid #f3f4f6; }");
    
    // Minimum height for cart
    cartTable->setMinimumHeight(400); // Increased height
    
    leftColumnLayout->addWidget(cartTable);
}

void ServiceOrderWidget::setupPaymentSection()
{
    paymentFrame = new QFrame(this);
    paymentFrame->setObjectName("infoFrame");
    QVBoxLayout *layout = new QVBoxLayout(paymentFrame);
    layout->setSpacing(15);

    QLabel *title = new QLabel("💰 THÔNG TIN THANH TOÁN", this);
    title->setObjectName("sectionTitle");
    layout->addWidget(title);

    QGridLayout *grid = new QGridLayout();
    
    totalPriceLabel = new QLabel("0 đ", this);
    totalPriceLabel->setAlignment(Qt::AlignRight);
    
    discountLabel = new QLabel("0 đ", this);
    discountLabel->setAlignment(Qt::AlignRight);
    discountLabel->setStyleSheet("color: #16a34a;");
    
    finalAmountLabel = new QLabel("0 đ", this);
    finalAmountLabel->setAlignment(Qt::AlignRight);
    finalAmountLabel->setStyleSheet("color: #dc2626; font-weight: bold; font-size: 20px;"); // Bigger font

    grid->addWidget(new QLabel("Tổng tiền hàng:", this), 0, 0);
    grid->addWidget(totalPriceLabel, 0, 1);
    grid->addWidget(new QLabel("Giảm giá:", this), 1, 0);
    grid->addWidget(discountLabel, 1, 1);
    grid->addWidget(new QLabel("THÀNH TIỀN:", this), 2, 0);
    grid->addWidget(finalAmountLabel, 2, 1);

    layout->addLayout(grid);

    // Note
    layout->addWidget(new QLabel("Ghi chú:", this));
    noteTextEdit = new QTextEdit(this);
    noteTextEdit->setPlaceholderText("Ghi chú đơn hàng...");
    noteTextEdit->setFixedHeight(80);
    layout->addWidget(noteTextEdit);

    // Buttons
    QPushButton *clearBtn = new QPushButton("🗑️ Xóa giỏ hàng", this);
    clearBtn->setFixedHeight(40);
    clearBtn->setCursor(Qt::PointingHandCursor);
    clearBtn->setStyleSheet("QPushButton { background-color: white; border: 1px solid #ef4444; color: #ef4444; border-radius: 6px; font-weight: bold; } QPushButton:hover { background-color: #fee2e2; }");
    connect(clearBtn, &QPushButton::clicked, this, &ServiceOrderWidget::onClearCartClicked);
    layout->addWidget(clearBtn);
    
    paymentBtn = new QPushButton("💳 THANH TOÁN\n& IN HÓA ĐƠN", this);
    paymentBtn->setObjectName("paymentBtn");
    paymentBtn->setCursor(Qt::PointingHandCursor);
    paymentBtn->setFixedHeight(80); // Bigger button
    paymentBtn->setStyleSheet("QPushButton { background-color: #16a34a; color: white; font-weight: bold; font-size: 16px; border-radius: 8px; border: none; } QPushButton:hover { background-color: #15803d; }");
    
    layout->addWidget(paymentBtn);
    
    // Add to Right Column
    rightColumnLayout->addWidget(paymentFrame);
}

void ServiceOrderWidget::setupConnections()
{
    // Customer
    connect(customerComboBox, QOverload<int>::of(&QComboBox::activated),
            this, &ServiceOrderWidget::onCustomerSelected);
    
    // Service Selection
    connect(selectServiceBtn, &QPushButton::clicked, this, &ServiceOrderWidget::onSelectServiceClicked);

    // Payment Actions
    connect(paymentBtn, &QPushButton::clicked, this, &ServiceOrderWidget::onPaymentClicked);
}

void ServiceOrderWidget::applyStyles()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #f9fafb;
            color: #1f2937;
        }
        QFrame {
            background-color: white;
            border: 1px solid #e5e7eb;
            border-radius: 8px;
        }
        QLineEdit, QComboBox, QTextEdit {
            border: 1px solid #d1d5db;
            border-radius: 2px;
            padding: 8px;
            background-color: white;
        }
        QLineEdit:focus, QComboBox:focus, QTextEdit:focus {
            border: 2px solid #16a34a;
        }
        QTableWidget {
            border: 1px solid #e5e7eb;
            gridline-color: #f3f4f6;
            background-color: white;
        }
        QHeaderView::section {
            background-color: #f9fafb;
            padding: 8px;
            border: none;
            border-bottom: 2px solid #e5e7eb;
            font-weight: bold;
        }
        #selectServiceBtn {
            background-color: #16a34a;
            color: white;
            font-weight: bold;
            font-size: 14px;
            border-radius: 6px;
            text-align: left;
            padding-left: 15px;
        }
        #selectServiceBtn:hover {
            background-color: #15803d;
        }
        #paymentBtn {
            background-color: #16a34a;
            color: white;
            font-weight: bold;
            font-size: 16px;
            border-radius: 6px;
        }
        #paymentBtn:hover {
            background-color: #15803d;
        }
    )");
}

void ServiceOrderWidget::loadServices()
{
    loadCustomers();
}

void ServiceOrderWidget::loadCustomers()
{
    customerComboBox->blockSignals(true);
    customerComboBox->clear();
    customerComboBox->addItem("-- Chọn khách hàng --", "");

    const MangDong<KhachHang *> &customers = system->layDanhSachKhachHang();
    for (int i = 0; i < customers.size(); i++)
    {
        KhachHang *cust = customers[i];
        if (cust)
        {
            std::string phone = cust->laySoDienThoai();
            std::string name = cust->layHoTen();

            if (!phone.empty())
            {
                QString displayText = QString::fromStdString(phone + " - " + name);
                customerComboBox->addItem(displayText, QString::fromStdString(phone));
            }
        }
    }

    customerComboBox->blockSignals(false);
}

void ServiceOrderWidget::onCustomerSelected(int index)
{
    if (index <= 0) {
        selectedCustomer = nullptr;
        updateCustomerInfo();
        updateTotals();
        return;
    }

    QString phone = customerComboBox->itemData(index).toString();
    if (phone.isEmpty()) return;

    KhachHang *customer = system->timKhachHangTheoSDT(phone.toStdString());
    if (customer)
    {
        selectedCustomer = customer;
        
        // Update UI fields
        phoneLineEdit->setText(QString::fromStdString(customer->laySoDienThoai()));
        
        updateCustomerInfo();
        updateTotals();
    }
}

void ServiceOrderWidget::onAddNewCustomerClicked()
{
    AddCustomerDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        KhachHang *newCustomer = dialog.getNewCustomer();
        if (newCustomer && system->themKhachHang(newCustomer))
        {
            system->luuHeThong("D:/PBL2-/Data/data.bin");
            loadCustomers();
            
            // Select the new customer
            QString newPhone = QString::fromStdString(newCustomer->laySoDienThoai());
            int index = customerComboBox->findData(newPhone);
            if (index >= 0) {
                customerComboBox->setCurrentIndex(index);
                onCustomerSelected(index);
            }
            
            QMessageBox::information(this, "Thành công", "Đã thêm khách hàng mới!");
        }
    }
}

void ServiceOrderWidget::onSelectServiceClicked()
{
    ServiceSelectionDialog dialog(this);
    
    // Pass current cart quantities to dialog to calculate available stock
    QMap<QString, int> currentQuantities;
    for(const auto& item : cartItems) {
        currentQuantities[item.maDichVu] = item.soLuong;
    }
    dialog.setExistingCart(currentQuantities);
    
    if (dialog.exec() == QDialog::Accepted)
    {
        QList<ServiceSelectionItem> selectedItems = dialog.getSelectedServices();
        
        for (const auto& item : selectedItems) {
            if (!item.service) continue;
            
            QString maDV = QString::fromStdString(item.service->layMaDichVu());
            
            // Check if exists in cart
            bool found = false;
            for (int i = 0; i < cartItems.size(); i++) {
                if (cartItems[i].maDichVu == maDV) {
                    cartItems[i].soLuong += item.quantity;
                    cartItems[i].thanhTien = cartItems[i].donGia * cartItems[i].soLuong;
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                CartItem newItem;
                newItem.maDichVu = maDV;
                newItem.tenDichVu = QString::fromStdString(item.service->layTenDichVu());
                newItem.donVi = "Cái"; // Default
                newItem.donGia = item.service->layDonGia();
                newItem.soLuong = item.quantity;
                newItem.thanhTien = newItem.donGia * newItem.soLuong;
                cartItems.append(newItem);
            }
        }
        
        updateCartTable();
    }
}

void ServiceOrderWidget::onRemoveFromCartClicked(int row)
{
    if (row >= 0 && row < cartItems.size()) {
        cartItems.removeAt(row);
        updateCartTable();
    }
}

void ServiceOrderWidget::onClearCartClicked()
{
    if (cartItems.isEmpty()) return;
    
    if (QMessageBox::question(this, "Xác nhận", "Xóa toàn bộ giỏ hàng?") == QMessageBox::Yes) {
        cartItems.clear();
        updateCartTable();
    }
}

void ServiceOrderWidget::onPaymentClicked()
{
    if (cartItems.isEmpty()) {
        QMessageBox::warning(this, "Thông báo", "Giỏ hàng trống!");
        return;
    }
    
    // Create order
    DonHangDichVu *order = system->taoDonHangDichVu(selectedCustomer);
    if (!order) {
        QMessageBox::critical(this, "Lỗi", "Không thể tạo đơn hàng!");
        return;
    }
    
    // Add items and update stock
    for (const CartItem &item : cartItems) {
        DichVu *dv = system->timDichVu(item.maDichVu.toStdString());
        if (dv) {
            DichVuDat dvDat(dv, item.soLuong);
            order->themDichVu(dvDat);
            
            // Update stock and sold count
            int currentStock = dv->laySoLuongTon();
            int currentSold = dv->laySoLuongBan();
            
            dv->datSoLuongTon(currentStock - item.soLuong);
            dv->datSoLuongBan(currentSold + item.soLuong);
        }
    }
    
    // Calculate
    order->tinhTongTien();
    order->tinhGiamGia();
    order->tinhThanhTien();
    
    if (!noteTextEdit->toPlainText().isEmpty()) {
        order->setGhiChu(noteTextEdit->toPlainText().toStdString());
    }
    
    // Confirm
    QString msg = QString("Xác nhận thanh toán đơn hàng?\nTổng tiền: %1 đ")
                    .arg(QString::number(order->getThanhTien(), 'f', 0));
                    
    if (QMessageBox::question(this, "Thanh toán", msg) == QMessageBox::Yes) {
        order->setTrangThai(TrangThaiDonHang::HOAN_THANH);
        
        // Save system state (including updated stock)
        system->luuHeThong("D:/PBL2-/Data/data.bin");
        // Also save to CSV to persist changes
        system->luuDichVuCSV("D:/PBL2-/Data/dichvu.csv");
        
        QMessageBox::information(this, "Thành công", "Thanh toán thành công!");
        emit orderCreated(QString::fromStdString(order->getMaDonHang()));
        
        // Clear
        cartItems.clear();
        updateCartTable();
        noteTextEdit->clear();
    }
}

void ServiceOrderWidget::updateCartTable()
{
    cartTable->setRowCount(0);
    
    for (int i = 0; i < cartItems.size(); i++) {
        const CartItem &item = cartItems[i];
        int row = cartTable->rowCount();
        cartTable->insertRow(row);
        cartTable->setRowHeight(row, 80); // Taller rows for cart look
        
        // 0. Image
        DichVu *service = system->timDichVu(item.maDichVu.toStdString());
        QWidget *imgContainer = new QWidget();
        QHBoxLayout *imgLayout = new QHBoxLayout(imgContainer);
        imgLayout->setContentsMargins(5, 5, 5, 5);
        imgLayout->setAlignment(Qt::AlignCenter);
        QLabel *imgLabel = new QLabel();
        imgLabel->setFixedSize(60, 60);
        imgLabel->setScaledContents(true);
        imgLabel->setStyleSheet("background-color: #f3f4f6; border-radius: 4px;");
        
        if (service && !service->layHinhAnh().empty()) {
            QString fullPath = "D:/PBL2-/Data/" + QString::fromStdString(service->layHinhAnh());
            if (QFile::exists(fullPath)) {
                imgLabel->setPixmap(QPixmap(fullPath));
            } else {
                imgLabel->setText("📦");
                imgLabel->setAlignment(Qt::AlignCenter);
            }
        } else {
            imgLabel->setText("📦");
            imgLabel->setAlignment(Qt::AlignCenter);
        }
        imgLayout->addWidget(imgLabel);
        cartTable->setCellWidget(row, 0, imgContainer);

        // 1. Info (Name + Unit)
        QWidget *infoWidget = new QWidget();
        QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
        infoLayout->setContentsMargins(10, 5, 5, 5);
        infoLayout->setSpacing(2);
        infoLayout->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        
        QLabel *nameLabel = new QLabel(item.tenDichVu);
        nameLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #1f2937;");
        nameLabel->setWordWrap(true);
        
        QLabel *unitLabel = new QLabel("Đơn vị: " + item.donVi);
        unitLabel->setStyleSheet("color: #6b7280; font-size: 12px;");
        
        infoLayout->addWidget(nameLabel);
        infoLayout->addWidget(unitLabel);
        cartTable->setCellWidget(row, 1, infoWidget);

        // 2. Price
        QLabel *priceLabel = new QLabel(QString::number(item.donGia, 'f', 0) + " đ");
        priceLabel->setAlignment(Qt::AlignCenter);
        priceLabel->setStyleSheet("font-weight: 500; color: #374151;");
        cartTable->setCellWidget(row, 2, priceLabel);

        // 3. Quantity Control
        QWidget *qtyWidget = new QWidget();
        QHBoxLayout *qtyLayout = new QHBoxLayout(qtyWidget);
        qtyLayout->setContentsMargins(5, 5, 5, 5);
        qtyLayout->setSpacing(0);
        qtyLayout->setAlignment(Qt::AlignCenter);
        
        QPushButton *minusBtn = new QPushButton("-");
        minusBtn->setFixedSize(30, 30);
        minusBtn->setCursor(Qt::PointingHandCursor);
        minusBtn->setStyleSheet("QPushButton { border: 1px solid #d1d5db; border-top-left-radius: 4px; border-bottom-left-radius: 4px; background-color: #f3f4f6; color: #374151; font-weight: bold; font-size: 18px; } QPushButton:hover { background-color: #e5e7eb; }");
        connect(minusBtn, &QPushButton::clicked, [this, i]() { onDecreaseQuantity(i); });
        
        QLabel *qtyLabel = new QLabel(QString::number(item.soLuong));
        qtyLabel->setFixedSize(60, 30);
        qtyLabel->setAlignment(Qt::AlignCenter);
        qtyLabel->setStyleSheet("border-top: 1px solid #d1d5db; border-bottom: 1px solid #d1d5db; background-color: white; color: #1f2937; font-weight: bold; font-size: 14px;");
        
        QPushButton *plusBtn = new QPushButton("+");
        plusBtn->setFixedSize(30, 30);
        plusBtn->setCursor(Qt::PointingHandCursor);
        plusBtn->setStyleSheet("QPushButton { border: 1px solid #d1d5db; border-top-right-radius: 4px; border-bottom-right-radius: 4px; background-color: #f3f4f6; color: #374151; font-weight: bold; font-size: 18px; } QPushButton:hover { background-color: #e5e7eb; }");
        connect(plusBtn, &QPushButton::clicked, [this, i]() { onIncreaseQuantity(i); });
        
        qtyLayout->addWidget(minusBtn);
        qtyLayout->addWidget(qtyLabel);
        qtyLayout->addWidget(plusBtn);
        cartTable->setCellWidget(row, 3, qtyWidget);

        // 4. Total
        QLabel *totalLabel = new QLabel(QString::number(item.thanhTien, 'f', 0) + " đ");
        totalLabel->setAlignment(Qt::AlignCenter);
        totalLabel->setStyleSheet("font-weight: bold; color: #16a34a; font-size: 14px;");
        cartTable->setCellWidget(row, 4, totalLabel);
        
        // 5. Delete
        QWidget *delContainer = new QWidget();
        QHBoxLayout *delLayout = new QHBoxLayout(delContainer);
        delLayout->setContentsMargins(0,0,0,0);
        delLayout->setAlignment(Qt::AlignCenter);
        
        QPushButton *delBtn = new QPushButton("🗑️");
        delBtn->setFixedSize(30, 30);
        delBtn->setCursor(Qt::PointingHandCursor);
        delBtn->setToolTip("Xóa khỏi giỏ hàng");
        delBtn->setStyleSheet("QPushButton { border: none; background-color: transparent; color: #ef4444; font-size: 16px; } QPushButton:hover { background-color: #fee2e2; border-radius: 4px; }");
        connect(delBtn, &QPushButton::clicked, [this, i]() {
            onRemoveFromCartClicked(i);
        });
        
        delLayout->addWidget(delBtn);
        cartTable->setCellWidget(row, 5, delContainer);
    }
    
    updateTotals();
}

void ServiceOrderWidget::onIncreaseQuantity(int row)
{
    if (row >= 0 && row < cartItems.size()) {
        cartItems[row].soLuong++;
        cartItems[row].thanhTien = cartItems[row].donGia * cartItems[row].soLuong;
        updateCartTable();
    }
}

void ServiceOrderWidget::onDecreaseQuantity(int row)
{
    if (row >= 0 && row < cartItems.size()) {
        if (cartItems[row].soLuong > 1) {
            cartItems[row].soLuong--;
            cartItems[row].thanhTien = cartItems[row].donGia * cartItems[row].soLuong;
            updateCartTable();
        } else {
            // Ask to remove if quantity becomes 0
            onRemoveFromCartClicked(row);
        }
    }
}

void ServiceOrderWidget::updateTotals()
{
    totalPrice = 0;
    for (const CartItem &item : cartItems) {
        totalPrice += item.thanhTien;
    }
    
    discount = 0; // Implement discount logic if needed
    finalAmount = totalPrice - discount;
    
    totalPriceLabel->setText(QString("%1 đ").arg(QString::number(totalPrice, 'f', 0)));
    discountLabel->setText(QString("%1 đ").arg(QString::number(discount, 'f', 0)));
    finalAmountLabel->setText(QString("%1 đ").arg(QString::number(finalAmount, 'f', 0)));
}

void ServiceOrderWidget::updateCustomerInfo()
{
    if (selectedCustomer) {
        customerNameLabel->setText(QString::fromStdString(selectedCustomer->layHoTen()));
        membershipLabel->setText(QString::fromStdString(selectedCustomer->layTenHang()));
    } else {
        customerNameLabel->setText("---");
        membershipLabel->setText("---");
    }
}