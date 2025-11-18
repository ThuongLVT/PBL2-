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
#include <QPixmap>
#include <QFile>
#include <QDebug>

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
    loadServices();
}

ServiceOrderWidget::~ServiceOrderWidget()
{
}

void ServiceOrderWidget::setupUI()
{
    // Main horizontal layout (Left 60% + Right 40%)
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    setupLeftPanel();
    setupRightPanel();

    setLayout(mainLayout);
}

void ServiceOrderWidget::setupLeftPanel()
{
    // Left container (60%)
    QWidget *leftWidget = new QWidget(this);
    leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setSpacing(15);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    // ===== TOP 40%: CART TABLE =====
    cartFrame = new QFrame(this);
    cartFrame->setObjectName("cartFrame");
    cartLayout = new QVBoxLayout(cartFrame);
    cartLayout->setSpacing(10);
    cartLayout->setContentsMargins(15, 15, 15, 15);

    // Cart title
    QLabel *cartTitle = new QLabel("🛒 GIỎ HÀNG (Cart)", this);
    cartTitle->setObjectName("sectionTitle");
    QFont titleFont = cartTitle->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    cartTitle->setFont(titleFont);
    cartLayout->addWidget(cartTitle);

    // Cart table
    cartTable = new QTableWidget(0, 6, this);
    cartTable->setObjectName("dataTable");
    cartTable->setHorizontalHeaderLabels({"Ảnh", "Tên dịch vụ", "Đơn vị", "Đơn giá", "SL", "Xóa"});
    cartTable->horizontalHeader()->setStretchLastSection(false);
    cartTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    cartTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    cartTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    cartTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    cartTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    cartTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
    cartTable->setColumnWidth(0, 60);  // Image
    cartTable->setColumnWidth(2, 70);  // Unit
    cartTable->setColumnWidth(3, 100); // Price
    cartTable->setColumnWidth(4, 50);  // Quantity
    cartTable->setColumnWidth(5, 50);  // Delete
    cartTable->setRowHeight(0, 55);    // Row height for images
    cartTable->verticalHeader()->setVisible(false);
    cartTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    cartTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    cartTable->setMaximumHeight(200);
    cartLayout->addWidget(cartTable);

    // Cart total
    cartTotalLabel = new QLabel("Tổng: 0 đ", this);
    cartTotalLabel->setObjectName("cartTotal");
    QFont totalFont = cartTotalLabel->font();
    totalFont.setPointSize(12);
    totalFont.setBold(true);
    cartTotalLabel->setFont(totalFont);
    cartTotalLabel->setAlignment(Qt::AlignRight);
    cartLayout->addWidget(cartTotalLabel);

    leftLayout->addWidget(cartFrame, 4); // 40% of left panel

    // ===== BOTTOM 60%: SERVICE GRID =====
    serviceFrame = new QFrame(this);
    serviceFrame->setObjectName("serviceFrame");
    serviceLayout = new QVBoxLayout(serviceFrame);
    serviceLayout->setSpacing(10);
    serviceLayout->setContentsMargins(15, 15, 15, 15);

    // Search & filter row
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->setSpacing(10);

    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("🔍 Tìm dịch vụ...");
    searchEdit->setObjectName("searchEdit");
    searchEdit->setMinimumHeight(40);
    searchLayout->addWidget(searchEdit, 3);

    categoryCombo = new QComboBox(this);
    categoryCombo->setObjectName("filterCombo");
    categoryCombo->addItem("🔽 Tất cả", "ALL");
    categoryCombo->addItem("Đồ uống", "DO_UONG");
    categoryCombo->addItem("Thiết bị", "THIET_BI");
    categoryCombo->addItem("Khác", "KHAC");
    categoryCombo->setMinimumHeight(40);
    searchLayout->addWidget(categoryCombo, 1);

    reloadBtn = new QPushButton("🔄", this);
    reloadBtn->setObjectName("iconButton");
    reloadBtn->setMinimumHeight(40);
    reloadBtn->setMaximumWidth(50);
    reloadBtn->setToolTip("Tải lại danh sách dịch vụ");
    searchLayout->addWidget(reloadBtn);

    serviceLayout->addLayout(searchLayout);

    // Service grid scroll area
    serviceScrollArea = new QScrollArea(this);
    serviceScrollArea->setWidgetResizable(true);
    serviceScrollArea->setObjectName("serviceScrollArea");
    serviceScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    serviceGridWidget = new QWidget();
    serviceGridLayout = new QGridLayout(serviceGridWidget);
    serviceGridLayout->setSpacing(15);
    serviceGridLayout->setContentsMargins(10, 10, 10, 10);

    serviceScrollArea->setWidget(serviceGridWidget);
    serviceLayout->addWidget(serviceScrollArea);

    leftLayout->addWidget(serviceFrame, 6); // 60% of left panel

    mainLayout->addWidget(leftWidget, 6); // 60% of main layout
}

void ServiceOrderWidget::setupRightPanel()
{
    // Right container (40%)
    QWidget *rightWidget = new QWidget(this);
    rightWidget->setFixedWidth(480);
    rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setSpacing(15);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    // ===== COMBINED CUSTOMER & PAYMENT INFORMATION =====
    customerFrame = new QFrame(this);
    customerFrame->setObjectName("formFrame");
    customerLayout = new QVBoxLayout(customerFrame);
    customerLayout->setSpacing(8);
    customerLayout->setContentsMargins(15, 15, 15, 15);

    // Title
    QLabel *customerTitle = new QLabel("📋 THÔNG TIN KHÁCH HÀNG", this);
    customerTitle->setObjectName("sectionTitle");
    QFont custTitleFont = customerTitle->font();
    custTitleFont.setPointSize(12);
    custTitleFont.setBold(true);
    customerTitle->setFont(custTitleFont);
    customerLayout->addWidget(customerTitle);

    // Customer list with search
    QLabel *selectLabel = new QLabel("Chọn khách hàng (click để chọn):", this);
    selectLabel->setObjectName("fieldLabel");
    customerLayout->addWidget(selectLabel);

    // Search box
    QLineEdit *customerSearchEdit = new QLineEdit(this);
    customerSearchEdit->setPlaceholderText("Tìm theo SĐT hoặc tên...");
    customerSearchEdit->setObjectName("formInput");
    customerSearchEdit->setMinimumHeight(38);
    customerLayout->addWidget(customerSearchEdit);

    // Customer list
    customerComboBox = new QComboBox(this);
    customerComboBox->setObjectName("formInput");
    customerComboBox->setMinimumHeight(40);
    customerComboBox->setEditable(false);
    customerLayout->addWidget(customerComboBox);

    // Add new customer button
    QPushButton *addNewCustomerBtn = new QPushButton("+ Thêm khách hàng mới", this);
    addNewCustomerBtn->setObjectName("secondaryButton");
    addNewCustomerBtn->setMinimumHeight(36);
    customerLayout->addWidget(addNewCustomerBtn);
    connect(addNewCustomerBtn, &QPushButton::clicked, this, &ServiceOrderWidget::onAddNewCustomerClicked);

    // Search connection
    connect(customerSearchEdit, &QLineEdit::textChanged, [this, customerSearchEdit]()
            { filterCustomers(customerSearchEdit->text()); });

    // Spacing
    customerLayout->addSpacing(10);

    // Selected customer display (read-only)
    QLabel *selectedLabel = new QLabel("Khách hàng đã chọn:", this);
    selectedLabel->setObjectName("fieldLabel");
    customerLayout->addWidget(selectedLabel);

    QHBoxLayout *phoneLayout = new QHBoxLayout();
    QLabel *phoneLabelText = new QLabel("SĐT:", this);
    phoneLabelText->setObjectName("fieldLabel");
    phoneLayout->addWidget(phoneLabelText);

    phoneLineEdit = new QLineEdit(this);
    phoneLineEdit->setPlaceholderText("-");
    phoneLineEdit->setObjectName("formInput");
    phoneLineEdit->setMinimumHeight(38);
    phoneLineEdit->setReadOnly(true);
    phoneLayout->addWidget(phoneLineEdit, 2);

    customerLayout->addLayout(phoneLayout);

    // Customer name (auto-fill after select) - on same row
    QHBoxLayout *nameLayout = new QHBoxLayout();
    QLabel *nameLabel = new QLabel("Tên khách hàng:", this);
    nameLabel->setObjectName("fieldLabel");
    nameLabel->setMinimumWidth(100);
    nameLayout->addWidget(nameLabel);

    nameLineEdit = new QLineEdit(this);
    nameLineEdit->setPlaceholderText("Tự động điền sau khi chọn");
    nameLineEdit->setObjectName("formInput");
    nameLineEdit->setReadOnly(true);
    nameLineEdit->setMinimumHeight(38);
    nameLayout->addWidget(nameLineEdit, 1);

    customerLayout->addLayout(nameLayout);

    // Divider line
    QFrame *divider = new QFrame(this);
    divider->setFrameShape(QFrame::HLine);
    divider->setFrameShadow(QFrame::Sunken);
    divider->setStyleSheet("background-color: #e5e7eb; max-height: 1px;");
    customerLayout->addWidget(divider);

    // Payment info title
    QLabel *paymentTitle = new QLabel("💳 THÔNG TIN THANH TOÁN", this);
    paymentTitle->setObjectName("sectionTitle");
    QFont payTitleFont = paymentTitle->font();
    payTitleFont.setPointSize(12);
    payTitleFont.setBold(true);
    paymentTitle->setFont(payTitleFont);
    customerLayout->addWidget(paymentTitle);

    // Customer display (read-only) - on same row
    QHBoxLayout *custLayout = new QHBoxLayout();
    QLabel *custLabel = new QLabel("Khách hàng:", this);
    custLabel->setObjectName("fieldLabel");
    custLabel->setFixedWidth(120);
    custLayout->addWidget(custLabel);
    customerNameLabel = new QLabel("(Chưa chọn)", this);
    customerNameLabel->setObjectName("infoLabel");
    custLayout->addWidget(customerNameLabel, 1);
    customerLayout->addLayout(custLayout);

    // Phone display - on same row
    QHBoxLayout *phoneDispLayout = new QHBoxLayout();
    QLabel *phoneDispLabel = new QLabel("SĐT:", this);
    phoneDispLabel->setObjectName("fieldLabel");
    phoneDispLabel->setFixedWidth(120);
    phoneDispLayout->addWidget(phoneDispLabel);
    phoneLabel = new QLabel("-", this);
    phoneLabel->setObjectName("infoLabel");
    phoneDispLayout->addWidget(phoneLabel, 1);
    customerLayout->addLayout(phoneDispLayout);

    // Membership - on same row
    QHBoxLayout *memberLayout = new QHBoxLayout();
    QLabel *memberLabel = new QLabel("Hạng thành viên:", this);
    memberLabel->setObjectName("fieldLabel");
    memberLabel->setFixedWidth(120);
    memberLayout->addWidget(memberLabel);
    membershipLabel = new QLabel("-", this);
    membershipLabel->setObjectName("infoLabel");
    memberLayout->addWidget(membershipLabel, 1);
    customerLayout->addLayout(memberLayout);

    // Note - on same row with compact textarea
    QHBoxLayout *noteLayout = new QHBoxLayout();
    QLabel *noteLabel = new QLabel("Ghi chú:", this);
    noteLabel->setObjectName("fieldLabel");
    noteLabel->setFixedWidth(120);
    noteLabel->setAlignment(Qt::AlignTop);
    noteLayout->addWidget(noteLabel);
    noteTextEdit = new QTextEdit(this);
    noteTextEdit->setPlaceholderText("Nhập ghi chú (không bắt buộc)");
    noteTextEdit->setObjectName("formTextArea");
    noteTextEdit->setFixedHeight(50);
    noteLayout->addWidget(noteTextEdit, 1);
    customerLayout->addLayout(noteLayout);

    // Totals
    customerLayout->addSpacing(5);
    totalPriceLabel = new QLabel("Tổng tiền: 0 đ", this);
    totalPriceLabel->setObjectName("totalLabel");
    customerLayout->addWidget(totalPriceLabel);

    discountLabel = new QLabel("Giảm giá: 0 đ", this);
    discountLabel->setObjectName("discountLabel");
    customerLayout->addWidget(discountLabel);

    finalAmountLabel = new QLabel("Thành tiền: 0 đ", this);
    finalAmountLabel->setObjectName("finalLabel");
    QFont finalFont = finalAmountLabel->font();
    finalFont.setPointSize(14);
    finalFont.setBold(true);
    finalAmountLabel->setFont(finalFont);
    customerLayout->addWidget(finalAmountLabel);

    rightLayout->addWidget(customerFrame);

    // ===== ACTION BUTTONS =====
    actionFrame = new QFrame(this);
    actionLayout = new QHBoxLayout(actionFrame);
    actionLayout->setSpacing(10);
    actionLayout->setContentsMargins(0, 0, 0, 0);

    clearBtn = new QPushButton("Xóa", this);
    clearBtn->setObjectName("secondaryButton");
    clearBtn->setMinimumHeight(45);
    actionLayout->addWidget(clearBtn);

    paymentBtn = new QPushButton("Thanh toán", this);
    paymentBtn->setObjectName("primaryButton");
    paymentBtn->setMinimumHeight(45);
    actionLayout->addWidget(paymentBtn);

    exportBtn = new QPushButton("Xuất", this);
    exportBtn->setObjectName("secondaryButton");
    exportBtn->setMinimumHeight(45);
    actionLayout->addWidget(exportBtn);

    rightLayout->addWidget(actionFrame);
    rightLayout->addStretch();

    mainLayout->addWidget(rightWidget, 4); // 40% of main layout
}

void ServiceOrderWidget::setupConnections()
{
    // Search & Filter
    connect(searchEdit, &QLineEdit::textChanged, this, &ServiceOrderWidget::onSearchTextChanged);
    connect(categoryCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ServiceOrderWidget::onCategoryFilterChanged);
    connect(reloadBtn, &QPushButton::clicked, this, &ServiceOrderWidget::onReloadClicked);

    // Customer - use activated instead of currentIndexChanged (only fires on user click)
    connect(customerComboBox, QOverload<int>::of(&QComboBox::activated),
            this, &ServiceOrderWidget::onCustomerSelected);

    // Actions
    connect(clearBtn, &QPushButton::clicked, this, &ServiceOrderWidget::onClearCartClicked);
    connect(paymentBtn, &QPushButton::clicked, this, &ServiceOrderWidget::onPaymentClicked);
    connect(exportBtn, &QPushButton::clicked, this, &ServiceOrderWidget::onExportClicked);
}

void ServiceOrderWidget::applyStyles()
{
    // Use light green-white theme consistent with other pages
    setStyleSheet(R"(
        QWidget {
            background-color: #f9fafb;
            color: #1f2937;
        }
        
        #cartFrame, #serviceFrame, #formFrame {
            background-color: white;
            border-radius: 8px;
            border: 1px solid #e5e7eb;
        }
        
        #sectionTitle {
            color: #1f2937;
            font-size: 14px;
            font-weight: bold;
            padding: 5px 0px;
        }
        
        #searchEdit, #filterCombo, #formInput {
            background-color: white;
            border: 1px solid #d1d5db;
            border-radius: 6px;
            padding: 8px 12px;
            color: #1f2937;
            font-size: 14px;
        }
        
        #searchEdit:focus, #formInput:focus {
            border: 2px solid #16a34a;
            outline: none;
        }
        
        #dataTable {
            background-color: white;
            border: 1px solid #e5e7eb;
            border-radius: 8px;
            gridline-color: #f3f4f6;
        }
        
        #dataTable::item {
            padding: 10px;
            color: #1f2937;
            border-bottom: 1px solid #f3f4f6;
        }
        
        #dataTable::item:selected {
            background-color: #f0fdf4;
            color: #16a34a;
        }
        
        #dataTable QHeaderView::section {
            background-color: #f9fafb;
            color: #6b7280;
            padding: 12px 10px;
            border: none;
            border-bottom: 2px solid #e5e7eb;
            font-weight: 600;
            text-transform: uppercase;
            font-size: 12px;
        }
        
        #serviceCard {
            background-color: white;
            border: 1px solid #e5e7eb;
            border-radius: 8px;
            padding: 12px;
        }
        
        #serviceCard:hover {
            background-color: #f0fdf4;
            border: 2px solid #16a34a;
            box-shadow: 0 4px 6px -1px rgba(0,0,0,0.1);
        }
        
        #primaryButton {
            background-color: #16a34a;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 10px 20px;
            font-weight: 600;
            font-size: 14px;
        }
        
        #primaryButton:hover {
            background-color: #15803d;
        }
        
        #primaryButton:pressed {
            background-color: #14532d;
        }
        
        #secondaryButton {
            background-color: white;
            color: #6b7280;
            border: 1px solid #d1d5db;
            border-radius: 6px;
            padding: 10px 20px;
            font-weight: 600;
            font-size: 14px;
        }
        
        #secondaryButton:hover {
            background-color: #f9fafb;
            border-color: #9ca3af;
        }
        
        #iconButton {
            background-color: white;
            color: #ef4444;
            border: 1px solid #e5e7eb;
            border-radius: 6px;
            font-size: 16px;
            padding: 5px 10px;
        }
        
        #iconButton:hover {
            background-color: #fef2f2;
            border: 1px solid #ef4444;
        }
        
        #cartTotal {
            color: #1f2937;
            font-size: 14px;
            font-weight: 600;
            padding: 8px 0px;
        }
        
        #fieldLabel {
            color: #1f2937;
            font-size: 14px;
            font-weight: 600;
            padding: 2px 0px;
        }
        
        #infoLabel {
            color: #1f2937;
            font-size: 14px;
            font-weight: 500;
            padding: 2px 0px;
        }
        
        #totalLabel {
            color: #1f2937;
            font-size: 14px;
            font-weight: 600;
            padding: 2px 0px;
        }
        
        #discountLabel {
            color: #16a34a;
            font-size: 14px;
            font-weight: 600;
            padding: 2px 0px;
        }
        
        #finalLabel {
            color: #16a34a;
            font-size: 16px;
            font-weight: bold;
            padding: 4px 0px;
        }
        
        #formTextArea {
            background-color: white;
            border: 1px solid #d1d5db;
            border-radius: 6px;
            padding: 8px;
            color: #1f2937;
            font-size: 14px;
        }
        
        #formTextArea:focus {
            border: 2px solid #16a34a;
            outline: none;
        }
        
        QScrollArea {
            border: none;
            background-color: transparent;
        }
        
        QScrollBar:vertical {
            background-color: #f9fafb;
            width: 12px;
            border-radius: 6px;
        }
        
        QScrollBar::handle:vertical {
            background-color: #d1d5db;
            border-radius: 6px;
            min-height: 20px;
        }
        
        QScrollBar::handle:vertical:hover {
            background-color: #9ca3af;
        }
    )");
}

void ServiceOrderWidget::loadServices()
{
    allServices.clear();
    filteredServices.clear();

    const MangDong<DichVu *> &services = system->layDanhSachDichVu();
    for (int i = 0; i < services.size(); i++)
    {
        allServices.append(services[i]);
        filteredServices.append(services[i]);
    }

    createServiceCards();
    loadCustomers();
}

void ServiceOrderWidget::loadCustomers()
{
    customerComboBox->blockSignals(true);
    customerComboBox->clear();
    customerComboBox->addItem("-- Click để chọn khách hàng --", "");

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

void ServiceOrderWidget::filterCustomers(const QString &searchText)
{
    customerComboBox->blockSignals(true);
    customerComboBox->clear();
    customerComboBox->addItem("-- Click để chọn khách hàng --", "");

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
                QString qPhone = QString::fromStdString(phone);
                QString qName = QString::fromStdString(name);

                // Filter
                if (searchText.isEmpty() ||
                    qPhone.contains(searchText, Qt::CaseInsensitive) ||
                    qName.contains(searchText, Qt::CaseInsensitive))
                {
                    QString displayText = qPhone + " - " + qName;
                    customerComboBox->addItem(displayText, qPhone);
                }
            }
        }
    }

    customerComboBox->blockSignals(false);
}

void ServiceOrderWidget::createServiceCards()
{
    // Clear existing cards
    QLayoutItem *item;
    while ((item = serviceGridLayout->takeAt(0)) != nullptr)
    {
        if (item->widget())
        {
            delete item->widget();
        }
        delete item;
    }

    // Create cards in grid (3 columns for better fit)
    int row = 0;
    int col = 0;
    const int columns = 3;

    for (DichVu *service : filteredServices)
    {
        QWidget *card = createServiceCard(service);
        serviceGridLayout->addWidget(card, row, col);

        col++;
        if (col >= columns)
        {
            col = 0;
            row++;
        }
    }

    // Add stretch to push cards to top
    serviceGridLayout->setRowStretch(row + 1, 1);
}

QWidget *ServiceOrderWidget::createServiceCard(DichVu *service)
{
    QFrame *card = new QFrame();
    card->setObjectName("serviceCard");
    card->setFixedSize(180, 230);
    card->setCursor(Qt::PointingHandCursor);

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(8);
    cardLayout->setContentsMargins(12, 12, 12, 12);

    // Image placeholder (square)
    QLabel *imageLabel = new QLabel(card);
    imageLabel->setFixedSize(140, 140);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("background-color: #4C4C4C; border-radius: 5px;");

    QString imagePath = QString::fromStdString(service->layHinhAnh());
    if (!imagePath.isEmpty())
    {
        // Try with absolute path first
        QString fullPath = "D:/QT_PBL2/Data/" + imagePath;
        if (QFile::exists(fullPath))
        {
            QPixmap pixmap(fullPath);
            imageLabel->setPixmap(pixmap.scaled(138, 138, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        else if (QFile::exists(imagePath))
        {
            QPixmap pixmap(imagePath);
            imageLabel->setPixmap(pixmap.scaled(138, 138, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        else
        {
            imageLabel->setStyleSheet("background-color: #4C4C4C; border-radius: 5px; font-size: 48px;");
            imageLabel->setText("📦");
        }
    }
    else
    {
        imageLabel->setStyleSheet("background-color: #4C4C4C; border-radius: 5px; font-size: 48px;");
        imageLabel->setText("📦");
    }
    cardLayout->addWidget(imageLabel, 0, Qt::AlignCenter);

    // Service name
    QLabel *nameLabel = new QLabel(QString::fromStdString(service->layTenDichVu()), card);
    nameLabel->setWordWrap(true);
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setStyleSheet("font-size: 13px; font-weight: bold; color: #1f2937;");
    nameLabel->setMaximumHeight(40);
    cardLayout->addWidget(nameLabel);

    // Price
    QLabel *priceLabel = new QLabel(
        QString::number(service->layDonGia(), 'f', 0) + " đ",
        card);
    priceLabel->setAlignment(Qt::AlignCenter);
    priceLabel->setStyleSheet("font-size: 12px; font-weight: bold; color: #16a34a;");
    cardLayout->addWidget(priceLabel);

    // Add button
    QPushButton *addBtn = new QPushButton("+ Thêm", card);
    addBtn->setObjectName("primaryButton");
    addBtn->setFixedHeight(32);
    addBtn->setCursor(Qt::PointingHandCursor);
    cardLayout->addWidget(addBtn);

    // Connect click event
    connect(addBtn, &QPushButton::clicked, [this, service]()
            { onServiceCardClicked(service); });

    return card;
}

void ServiceOrderWidget::onServiceCardClicked(DichVu *service)
{
    if (!service)
        return;
    addToCart(service);
}

void ServiceOrderWidget::addToCart(DichVu *service)
{
    QString maDV = QString::fromStdString(service->layMaDichVu());

    // Check if already in cart
    for (int i = 0; i < cartItems.size(); i++)
    {
        if (cartItems[i].maDichVu == maDV)
        {
            // Increase quantity
            cartItems[i].soLuong++;
            cartItems[i].thanhTien = cartItems[i].donGia * cartItems[i].soLuong;
            updateCart();
            return;
        }
    }

    // Add new item
    CartItem item;
    item.maDichVu = maDV;
    item.tenDichVu = QString::fromStdString(service->layTenDichVu());
    item.donVi = "Cái"; // TODO: Get from service if available
    item.donGia = service->layDonGia();
    item.soLuong = 1;
    item.thanhTien = item.donGia;

    cartItems.append(item);
    updateCart();
}

void ServiceOrderWidget::updateCart()
{
    cartTable->setRowCount(0);

    for (int i = 0; i < cartItems.size(); i++)
    {
        const CartItem &item = cartItems[i];
        int row = cartTable->rowCount();
        cartTable->insertRow(row);
        cartTable->setRowHeight(row, 55);

        // Image - load from service with centered widget
        DichVu *service = system->timDichVu(item.maDichVu.toStdString());

        QWidget *imgContainer = new QWidget();
        QHBoxLayout *imgLayout = new QHBoxLayout(imgContainer);
        imgLayout->setContentsMargins(5, 2, 5, 2);
        imgLayout->setAlignment(Qt::AlignCenter);

        QLabel *imgLabel = new QLabel();
        imgLabel->setFixedSize(50, 50);
        imgLabel->setAlignment(Qt::AlignCenter);
        imgLabel->setScaledContents(false);

        if (service)
        {
            std::string imgPath = service->layHinhAnh();
            if (!imgPath.empty())
            {
                QString fullPath = "D:/QT_PBL2/Data/" + QString::fromStdString(imgPath);
                if (QFile::exists(fullPath))
                {
                    QPixmap pixmap(fullPath);
                    imgLabel->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                }
                else
                {
                    imgLabel->setStyleSheet("background-color: #e5e7eb; border-radius: 3px; font-size: 20px;");
                    imgLabel->setText("📦");
                }
            }
            else
            {
                imgLabel->setStyleSheet("background-color: #e5e7eb; border-radius: 3px; font-size: 20px;");
                imgLabel->setText("📦");
            }
        }
        else
        {
            imgLabel->setStyleSheet("background-color: #e5e7eb; border-radius: 3px; font-size: 20px;");
            imgLabel->setText("❌");
        }

        imgLayout->addWidget(imgLabel);
        cartTable->setCellWidget(row, 0, imgContainer);

        // Name
        cartTable->setItem(row, 1, new QTableWidgetItem(item.tenDichVu));

        // Unit
        cartTable->setItem(row, 2, new QTableWidgetItem(item.donVi));

        // Price
        cartTable->setItem(row, 3, new QTableWidgetItem(QString::number(item.donGia, 'f', 0) + " đ"));

        // Quantity
        cartTable->setItem(row, 4, new QTableWidgetItem(QString::number(item.soLuong)));

        // Delete button - centered red box with X text only
        QWidget *deleteContainer = new QWidget();
        QHBoxLayout *deleteLayout = new QHBoxLayout(deleteContainer);
        deleteLayout->setContentsMargins(5, 2, 5, 2);
        deleteLayout->setAlignment(Qt::AlignCenter);

        QPushButton *deleteBtn = new QPushButton("X");
        deleteBtn->setFixedSize(35, 30);
        deleteBtn->setStyleSheet(R"(
            QPushButton {
                background-color: #DC3545;
                color: #FFFFFF;
                border: none;
                border-radius: 4px;
                font-size: 14px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #C82333;
            }
        )");
        deleteBtn->setCursor(Qt::PointingHandCursor);
        connect(deleteBtn, &QPushButton::clicked, [this, i]()
                { onRemoveFromCartClicked(i); });

        deleteLayout->addWidget(deleteBtn);
        cartTable->setCellWidget(row, 5, deleteContainer);
    }

    updateTotals();
}

void ServiceOrderWidget::onRemoveFromCartClicked(int index)
{
    if (index >= 0 && index < cartItems.size())
    {
        cartItems.removeAt(index);
        updateCart();
    }
}

void ServiceOrderWidget::updateTotals()
{
    totalPrice = 0;
    for (const CartItem &item : cartItems)
    {
        totalPrice += item.thanhTien;
    }

    // Calculate discount (TODO: based on membership)
    discount = 0;
    if (selectedCustomer != nullptr)
    {
        // TODO: Implement discount logic based on membership tier
        // For now, no discount
        discount = 0;
    }

    finalAmount = totalPrice - discount;

    // Update labels
    cartTotalLabel->setText(QString("Tổng: %1 đ")
                                .arg(QString::number(totalPrice, 'f', 0)));

    totalPriceLabel->setText(QString("Tổng tiền: %1 đ")
                                 .arg(QString::number(totalPrice, 'f', 0)));

    discountLabel->setText(QString("Giảm giá: %1 đ")
                               .arg(QString::number(discount, 'f', 0)));

    finalAmountLabel->setText(QString("Thành tiền: %1 đ")
                                  .arg(QString::number(finalAmount, 'f', 0)));
}

void ServiceOrderWidget::onSearchTextChanged(const QString &text)
{
    filteredServices.clear();

    QString searchLower = text.toLower();
    LoaiDichVu selectedCategory = LoaiDichVu::DO_UONG; // Default
    QString categoryData = categoryCombo->currentData().toString();

    for (DichVu *service : allServices)
    {
        // Filter by search text
        QString name = QString::fromStdString(service->layTenDichVu()).toLower();
        bool matchSearch = searchLower.isEmpty() || name.contains(searchLower);

        // Filter by category
        bool matchCategory = (categoryData == "ALL");
        if (!matchCategory)
        {
            if (categoryData == "DO_UONG")
                matchCategory = (service->layLoaiDichVu() == LoaiDichVu::DO_UONG);
            else if (categoryData == "THIET_BI")
                matchCategory = (service->layLoaiDichVu() == LoaiDichVu::THIET_BI);
            else if (categoryData == "KHAC")
                matchCategory = (service->layLoaiDichVu() == LoaiDichVu::KHAC);
        }

        if (matchSearch && matchCategory)
        {
            filteredServices.append(service);
        }
    }

    createServiceCards();
}

void ServiceOrderWidget::onCategoryFilterChanged(int index)
{
    Q_UNUSED(index);
    onSearchTextChanged(searchEdit->text());
}

void ServiceOrderWidget::onCustomerSelected(int index)
{
    if (index <= 0)
        return;

    QString phone = customerComboBox->itemData(index).toString();
    if (phone.isEmpty())
        return;

    KhachHang *customer = system->timKhachHangTheoSDT(phone.toStdString());
    if (customer)
    {
        selectCustomer(customer);
    }
}

void ServiceOrderWidget::onAddNewCustomerClicked()
{
    try
    {
        qDebug() << "Opening AddCustomerDialog...";
        AddCustomerDialog dialog(this);

        if (dialog.exec() == QDialog::Accepted)
        {
            qDebug() << "Dialog accepted, getting new customer...";
            KhachHang *newCustomer = dialog.getNewCustomer();

            if (!newCustomer)
            {
                qDebug() << "Error: newCustomer is null";
                QMessageBox::warning(this, "Lỗi", "Không thể tạo khách hàng mới!");
                return;
            }

            qDebug() << "Adding customer:" << QString::fromStdString(newCustomer->layHoTen());

            if (system->themKhachHang(newCustomer))
            {
                qDebug() << "Customer added successfully, saving to CSV...";
                system->luuHeThong("D:/QT_PBL2/Data/data.bin");

                qDebug() << "Reloading customer list...";
                loadCustomers();

                qDebug() << "Auto-selecting new customer...";
                // Auto select new customer by phone
                QString newPhone = QString::fromStdString(newCustomer->laySoDienThoai());
                for (int i = 1; i < customerComboBox->count(); i++)
                {
                    QString phone = customerComboBox->itemData(i).toString();
                    if (phone == newPhone)
                    {
                        customerComboBox->blockSignals(true);
                        customerComboBox->setCurrentIndex(i);
                        customerComboBox->blockSignals(false);

                        // Manually trigger selection
                        selectCustomer(newCustomer);
                        break;
                    }
                }

                QMessageBox::information(this, "Thành công",
                                         "Đã thêm khách hàng: " + QString::fromStdString(newCustomer->layHoTen()));
            }
            else
            {
                qDebug() << "Failed to add customer to system";
                QMessageBox::warning(this, "Lỗi", "Không thể thêm khách hàng vào hệ thống!");
            }
        }
        else
        {
            qDebug() << "Dialog cancelled";
        }
    }
    catch (const std::exception &e)
    {
        qDebug() << "Error in onAddNewCustomerClicked:" << e.what();
        QMessageBox::critical(this, "Lỗi", QString("Lỗi thêm khách hàng: %1").arg(e.what()));
    }
    catch (...)
    {
        qDebug() << "Unknown error in onAddNewCustomerClicked";
        QMessageBox::critical(this, "Lỗi", "Lỗi không xác định khi thêm khách hàng!");
    }
}

void ServiceOrderWidget::onPaymentClicked()
{
    // Validate cart
    if (cartItems.isEmpty())
    {
        QMessageBox::warning(this, "Cảnh báo", "Giỏ hàng trống!\nVui lòng chọn dịch vụ.");
        return;
    }

    // Create order
    DonHangDichVu *order = system->taoDonHangDichVu(selectedCustomer);
    if (!order)
    {
        QMessageBox::critical(this, "Lỗi", "Không thể tạo đơn hàng!");
        return;
    }

    // Add services to order
    for (const CartItem &item : cartItems)
    {
        DichVu *dv = system->timDichVu(item.maDichVu.toStdString());
        if (dv)
        {
            DichVuDat dvDat(dv, item.soLuong);
            order->themDichVu(dvDat);
        }
    }

    // Calculate totals
    order->tinhTongTien();
    order->tinhGiamGia();
    order->tinhThanhTien();

    // Set note
    if (!noteTextEdit->toPlainText().isEmpty())
    {
        order->setGhiChu(noteTextEdit->toPlainText().toStdString());
    }

    // Confirm payment
    QString msg = QString(
                      "Xác nhận thanh toán?\n\n"
                      "Mã đơn: %1\n"
                      "Tổng tiền: %2 đ\n"
                      "Giảm giá: %3 đ\n"
                      "Thành tiền: %4 đ")
                      .arg(QString::fromStdString(order->getMaDonHang()))
                      .arg(QString::number(order->getTongTien(), 'f', 0))
                      .arg(QString::number(order->getGiamGia(), 'f', 0))
                      .arg(QString::number(order->getThanhTien(), 'f', 0));

    int ret = QMessageBox::question(this, "Xác nhận thanh toán", msg);

    if (ret == QMessageBox::Yes)
    {
        order->setTrangThai(TrangThaiDonHang::HOAN_THANH);

        QMessageBox::information(this, "Thành công",
                                 QString("Đã thanh toán đơn hàng %1")
                                     .arg(QString::fromStdString(order->getMaDonHang())));

        emit orderCreated(QString::fromStdString(order->getMaDonHang()));

        // Clear cart
        clearCart();
    }
}

void ServiceOrderWidget::onClearCartClicked()
{
    if (cartItems.isEmpty())
        return;

    int ret = QMessageBox::question(this, "Xác nhận",
                                    "Bạn có chắc muốn xóa toàn bộ giỏ hàng?");

    if (ret == QMessageBox::Yes)
    {
        clearCart();
    }
}

void ServiceOrderWidget::clearCart()
{
    cartItems.clear();
    selectedCustomer = nullptr;
    phoneLineEdit->clear();
    nameLineEdit->clear();
    noteTextEdit->clear();
    customerNameLabel->setText("(Chưa chọn)");
    phoneLabel->setText("-");
    membershipLabel->setText("-");
    updateCart();
}

void ServiceOrderWidget::onExportClicked()
{
    QMessageBox::information(this, "Xuất hóa đơn",
                             "Tính năng xuất hóa đơn PDF sẽ được thêm sau.");
}

void ServiceOrderWidget::onReloadClicked()
{
    loadServices();
    QMessageBox::information(this, "Thông báo", "Đã tải lại danh sách dịch vụ!");
}

void ServiceOrderWidget::selectCustomer(KhachHang *customer)
{
    if (!customer)
    {
        return;
    }

    selectedCustomer = customer;

    // Fill customer info with safety checks
    std::string phone = customer->laySoDienThoai();
    std::string name = customer->layHoTen();

    if (!phone.empty())
    {
        phoneLineEdit->setText(QString::fromStdString(phone));
    }

    if (!name.empty())
    {
        nameLineEdit->setText(QString::fromStdString(name));
    }

    // Update payment info display
    updateCustomerInfo();

    // Recalculate totals with discount
    updateTotals();
}

void ServiceOrderWidget::updateCustomerInfo()
{
    if (!selectedCustomer)
    {
        customerNameLabel->setText("(Chưa chọn)");
        phoneLabel->setText("-");
        membershipLabel->setText("-");
        return;
    }

    std::string name = selectedCustomer->layHoTen();
    std::string phone = selectedCustomer->laySoDienThoai();
    std::string rank = selectedCustomer->layTenHang();

    customerNameLabel->setText(name.empty() ? "(Không rõ)" : QString::fromStdString(name));
    phoneLabel->setText(phone.empty() ? "-" : QString::fromStdString(phone));
    membershipLabel->setText(rank.empty() ? "-" : QString::fromStdString(rank));
}
