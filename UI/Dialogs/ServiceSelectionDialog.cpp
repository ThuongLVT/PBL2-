#include "ServiceSelectionDialog.h"
#include <QFrame>
#include <QDebug>
#include <QSpinBox>

ServiceSelectionDialog::ServiceSelectionDialog(QWidget *parent)
    : QDialog(parent), system(HeThongQuanLy::getInstance())
{
    setWindowTitle("Chọn Dịch Vụ");
    resize(1000, 700); // Large dialog
    setupUI();
    loadServices();
}

ServiceSelectionDialog::~ServiceSelectionDialog()
{
}

QList<ServiceSelectionItem> ServiceSelectionDialog::getSelectedServices() const
{
    QList<ServiceSelectionItem> result;
    for (auto it = tempSelection.begin(); it != tempSelection.end(); ++it) {
        if (it.value() > 0 && serviceMap.contains(it.key())) {
            result.append({serviceMap[it.key()], it.value()});
        }
    }
    return result;
}

void ServiceSelectionDialog::setCurrentSelection(const QMap<QString, int>& currentQuantities)
{
    tempSelection = currentQuantities;
    updateCartTable();
}

void ServiceSelectionDialog::setExistingCart(const QMap<QString, int>& existingQuantities)
{
    existingCart = existingQuantities;
    createServiceCards(); // Refresh cards to update stock display
}

void ServiceSelectionDialog::setupUI()
{
    // Main Layout: Horizontal Split (Left: Services, Right: Cart)
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // --- LEFT SIDE: Service Grid (55%) ---
    QWidget *leftWidget = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(15);

    // 1. Header & Filter Section
    QHBoxLayout *headerLayout = new QHBoxLayout();
    
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("🔍 Tìm kiếm dịch vụ...");
    searchEdit->setFixedHeight(40);
    searchEdit->setStyleSheet("QLineEdit { border: 1px solid #d1d5db; border-radius: 6px; padding: 0 10px; }");
    
    categoryCombo = new QComboBox(this);
    categoryCombo->addItem("Tất cả loại");
    categoryCombo->addItem("Đồ uống");
    categoryCombo->addItem("Đồ ăn");
    categoryCombo->addItem("Dụng cụ");
    categoryCombo->setFixedHeight(40);
    categoryCombo->setStyleSheet("QComboBox { border: 1px solid #d1d5db; border-radius: 6px; padding: 0 10px; }");

    headerLayout->addWidget(searchEdit, 1);
    headerLayout->addWidget(categoryCombo);
    
    leftLayout->addLayout(headerLayout);

    // 2. Service Grid Area
    serviceScrollArea = new QScrollArea(this);
    serviceScrollArea->setWidgetResizable(true);
    serviceScrollArea->setFrameShape(QFrame::NoFrame);
    serviceScrollArea->setStyleSheet("QScrollArea { background-color: #f9fafb; border: 1px solid #e5e7eb; border-radius: 8px; }");

    serviceGridWidget = new QWidget();
    serviceGridWidget->setStyleSheet("background-color: transparent;");
    serviceGridLayout = new QGridLayout(serviceGridWidget);
    serviceGridLayout->setSpacing(15);
    serviceGridLayout->setContentsMargins(15, 15, 15, 15);

    serviceScrollArea->setWidget(serviceGridWidget);
    leftLayout->addWidget(serviceScrollArea);
    
    mainLayout->addWidget(leftWidget, 55); // Stretch 55%

    // --- RIGHT SIDE: Mini Cart (45%) ---
    QWidget *rightWidget = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(15);
    
    QLabel *cartTitle = new QLabel("🛒 Đã chọn", this);
    cartTitle->setStyleSheet("font-weight: bold; font-size: 16px; color: #1f2937;");
    rightLayout->addWidget(cartTitle);
    
    // Mini Cart Table
    cartTable = new QTableWidget(0, 4, this);
    cartTable->setHorizontalHeaderLabels({"Tên món", "SL", "Thành tiền", ""});
    cartTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    cartTable->setColumnWidth(1, 120); // Increased width for Quantity
    cartTable->setColumnWidth(2, 90);
    cartTable->setColumnWidth(3, 40);
    cartTable->verticalHeader()->setVisible(false);
    cartTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    cartTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    cartTable->setShowGrid(false);
    cartTable->setStyleSheet("QTableWidget { border: 1px solid #e5e7eb; border-radius: 8px; background-color: white; }");
    
    rightLayout->addWidget(cartTable);
    
    // Summary
    summaryLabel = new QLabel("Tổng cộng: 0 đ", this);
    summaryLabel->setAlignment(Qt::AlignRight);
    summaryLabel->setStyleSheet("font-weight: bold; font-size: 16px; color: #dc2626; margin-top: 10px;");
    rightLayout->addWidget(summaryLabel);

    // Buttons
    QHBoxLayout *footerLayout = new QHBoxLayout();
    
    cancelBtn = new QPushButton("Hủy bỏ", this);
    cancelBtn->setFixedSize(100, 40);
    cancelBtn->setStyleSheet("QPushButton { background-color: white; border: 1px solid #d1d5db; border-radius: 6px; color: #374151; } QPushButton:hover { background-color: #f3f4f6; }");
    
    confirmBtn = new QPushButton("✅ Xác nhận", this);
    confirmBtn->setFixedHeight(40);
    confirmBtn->setStyleSheet("QPushButton { background-color: #16a34a; border: none; border-radius: 6px; color: white; font-weight: bold; } QPushButton:hover { background-color: #15803d; }");

    footerLayout->addWidget(cancelBtn);
    footerLayout->addWidget(confirmBtn);

    rightLayout->addLayout(footerLayout);
    
    mainLayout->addWidget(rightWidget, 45); // Stretch 45%

    // Connections
    connect(searchEdit, &QLineEdit::textChanged, this, &ServiceSelectionDialog::onSearchTextChanged);
    connect(categoryCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ServiceSelectionDialog::onCategoryFilterChanged);
    connect(cancelBtn, &QPushButton::clicked, this, &ServiceSelectionDialog::onCancelClicked);
    connect(confirmBtn, &QPushButton::clicked, this, &ServiceSelectionDialog::onConfirmClicked);
}

void ServiceSelectionDialog::loadServices()
{
    allServices.clear();
    const MangDong<DichVu *> &services = system->layDanhSachDichVu();
    for (int i = 0; i < services.size(); i++)
    {
        allServices.append(services[i]);
    }
    filteredServices = allServices;
    
    // Map for quick lookup
    for(DichVu* dv : allServices) {
        serviceMap[QString::fromStdString(dv->layMaDichVu())] = dv;
    }
    
    createServiceCards();
}

void ServiceSelectionDialog::createServiceCards()
{
    // Clear existing
    QLayoutItem *child;
    while ((child = serviceGridLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    int row = 0;
    int col = 0;
    int maxCols = 3; // Reduced columns for split layout

    for (DichVu *service : filteredServices) {
        if (!service->coConHang()) continue;

        QWidget *card = createServiceCard(service);
        serviceGridLayout->addWidget(card, row, col);

        col++;
        if (col >= maxCols) {
            col = 0;
            row++;
        }
    }
    
    // Spacer to push items to top-left
    if (filteredServices.isEmpty()) {
        QLabel *emptyLabel = new QLabel("Không tìm thấy dịch vụ nào", serviceGridWidget);
        emptyLabel->setAlignment(Qt::AlignCenter);
        serviceGridLayout->addWidget(emptyLabel, 0, 0, 1, maxCols);
    } else {
        serviceGridLayout->setRowStretch(row + 1, 1);
        serviceGridLayout->setColumnStretch(maxCols, 1);
    }
}

QWidget *ServiceSelectionDialog::createServiceCard(DichVu *service)
{
    QFrame *card = new QFrame();
    card->setFixedSize(140, 180); // Slightly smaller cards
    card->setStyleSheet(R"(
        QFrame {
            background-color: white;
            border: 1px solid #e5e7eb;
            border-radius: 8px;
        }
        QFrame:hover {
            border: 2px solid #3b82f6;
            background-color: #eff6ff;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(2);

    // Icon
    QLabel *icon = new QLabel("📷");
    icon->setAlignment(Qt::AlignCenter);
    icon->setStyleSheet("background-color: #f3f4f6; border-radius: 4px; font-size: 24px; color: #9ca3af;");
    icon->setFixedHeight(70);
    layout->addWidget(icon);

    // Name
    QLabel *name = new QLabel(QString::fromStdString(service->layTenDichVu()));
    name->setWordWrap(true);
    name->setAlignment(Qt::AlignCenter);
    name->setStyleSheet("font-weight: bold; color: #1f2937; font-size: 12px; border: none; background: transparent;");
    layout->addWidget(name);

    // Price
    QLabel *price = new QLabel(QString("%1 đ").arg(QString::number(service->layDonGia(), 'f', 0)));
    price->setAlignment(Qt::AlignCenter);
    price->setStyleSheet("color: #dc2626; font-weight: bold; font-size: 12px; border: none; background: transparent;");
    layout->addWidget(price);
    
    // Stock
    int available = getAvailableStock(service);
    QLabel *stock = new QLabel(QString("Còn: %1").arg(available));
    stock->setAlignment(Qt::AlignCenter);
    stock->setStyleSheet("color: #6b7280; font-size: 11px; border: none; background: transparent;");
    layout->addWidget(stock);

    // Add Button Overlay
    QPushButton *btn = new QPushButton(card);
    btn->resize(140, 180);
    btn->setStyleSheet("background-color: transparent; border: none;");
    btn->setCursor(Qt::PointingHandCursor);
    
    connect(btn, &QPushButton::clicked, [this, service, available]() {
        if (available > 0) {
            onServiceCardClicked(service);
        } else {
            // Optional: Show out of stock message or visual feedback
        }
    });

    return card;
}

int ServiceSelectionDialog::getAvailableStock(DichVu* service)
{
    // Check if service is available (TrangThai)
    if (!service->coConHang()) {
        return 0;
    }

    QString id = QString::fromStdString(service->layMaDichVu());
    int inCart = existingCart.value(id, 0);
    int inTemp = tempSelection.value(id, 0);
    
    // Use real stock from system
    int totalStock = service->laySoLuongTon();
    
    return totalStock - inCart - inTemp;
}

void ServiceSelectionDialog::onServiceCardClicked(DichVu *service)
{
    QString id = QString::fromStdString(service->layMaDichVu());
    
    // Check stock before adding
    if (getAvailableStock(service) > 0) {
        tempSelection[id]++;
        updateCartTable();
        createServiceCards(); // Refresh to update stock counts
    }
}

void ServiceSelectionDialog::updateCartTable()
{
    cartTable->setRowCount(0);
    double total = 0;
    
    QMapIterator<QString, int> i(tempSelection);
    while (i.hasNext()) {
        i.next();
        if (i.value() <= 0) continue;
        
        QString id = i.key();
        int qty = i.value();
        
        if (!serviceMap.contains(id)) continue;
        DichVu *service = serviceMap[id];
        
        int row = cartTable->rowCount();
        cartTable->insertRow(row);
        
        // Name
        cartTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(service->layTenDichVu())));
        
        // Quantity Control (Custom Widget: - [Value] +)
        QWidget *qtyWidget = new QWidget();
        QHBoxLayout *qtyLayout = new QHBoxLayout(qtyWidget);
        qtyLayout->setContentsMargins(0, 0, 0, 0);
        qtyLayout->setSpacing(0);
        qtyLayout->setAlignment(Qt::AlignCenter);

        QPushButton *minusBtn = new QPushButton("-");
        minusBtn->setFixedSize(30, 30);
        minusBtn->setCursor(Qt::PointingHandCursor);
        minusBtn->setStyleSheet("QPushButton { border: 1px solid #d1d5db; border-top-left-radius: 4px; border-bottom-left-radius: 4px; background-color: #f3f4f6; color: #374151; font-weight: bold; font-size: 16px; padding-bottom: 2px; } QPushButton:hover { background-color: #e5e7eb; }");
        
        QLabel *qtyLabel = new QLabel(QString::number(qty));
        qtyLabel->setFixedSize(50, 30);
        qtyLabel->setAlignment(Qt::AlignCenter);
        qtyLabel->setStyleSheet("border-top: 1px solid #d1d5db; border-bottom: 1px solid #d1d5db; background-color: white; color: #1f2937; font-weight: bold; font-size: 14px;");
        
        QPushButton *plusBtn = new QPushButton("+");
        plusBtn->setFixedSize(30, 30);
        plusBtn->setCursor(Qt::PointingHandCursor);
        plusBtn->setStyleSheet("QPushButton { border: 1px solid #d1d5db; border-top-right-radius: 4px; border-bottom-right-radius: 4px; background-color: #f3f4f6; color: #374151; font-weight: bold; font-size: 16px; padding-bottom: 2px; } QPushButton:hover { background-color: #e5e7eb; }");

        // Connect buttons
        connect(minusBtn, &QPushButton::clicked, [this, row]() { onDecreaseQuantity(row); });
        connect(plusBtn, &QPushButton::clicked, [this, row]() { onIncreaseQuantity(row); });
        
        qtyLayout->addWidget(minusBtn);
        qtyLayout->addWidget(qtyLabel);
        qtyLayout->addWidget(plusBtn);
        
        cartTable->setCellWidget(row, 1, qtyWidget);
        
        // Total
        double lineTotal = service->layDonGia() * qty;
        total += lineTotal;
        cartTable->setItem(row, 2, new QTableWidgetItem(QString::number(lineTotal, 'f', 0)));
        
        // Delete
        QPushButton *delBtn = new QPushButton("🗑️");
        delBtn->setFixedSize(30, 30);
        delBtn->setCursor(Qt::PointingHandCursor);
        delBtn->setStyleSheet("QPushButton { border: none; background-color: transparent; color: #ef4444; } QPushButton:hover { background-color: #fee2e2; border-radius: 4px; }");
        connect(delBtn, &QPushButton::clicked, [this, row]() { onRemoveItem(row); });
        cartTable->setCellWidget(row, 3, delBtn);
        
        // Store ID in item data for reference
        cartTable->item(row, 0)->setData(Qt::UserRole, id);
    }
    
    summaryLabel->setText(QString("Tổng cộng: %1 đ").arg(QString::number(total, 'f', 0)));
}

void ServiceSelectionDialog::onQuantityChanged(int row, int value)
{
    QString id = cartTable->item(row, 0)->data(Qt::UserRole).toString();
    if (tempSelection.contains(id)) {
        // Check stock limit
        int current = tempSelection[id];
        int diff = value - current;
        
        if (diff > 0) {
            // Increasing
            if (getAvailableStock(serviceMap[id]) >= diff) {
                tempSelection[id] = value;
            } else {
                // Revert if not enough stock
                QSpinBox *sb = qobject_cast<QSpinBox*>(cartTable->cellWidget(row, 1));
                if (sb) {
                    sb->blockSignals(true);
                    sb->setValue(current);
                    sb->blockSignals(false);
                }
                return; 
            }
        } else {
            // Decreasing
            tempSelection[id] = value;
        }
        
        updateCartTable();
        createServiceCards(); // Refresh stock display
    }
}

void ServiceSelectionDialog::onIncreaseQuantity(int row)
{
    QString id = cartTable->item(row, 0)->data(Qt::UserRole).toString();
    if (tempSelection.contains(id)) {
        tempSelection[id]++;
        updateCartTable();
    }
}

void ServiceSelectionDialog::onDecreaseQuantity(int row)
{
    QString id = cartTable->item(row, 0)->data(Qt::UserRole).toString();
    if (tempSelection.contains(id)) {
        if (tempSelection[id] > 1) {
            tempSelection[id]--;
        } else {
            tempSelection.remove(id);
        }
        updateCartTable();
    }
}

void ServiceSelectionDialog::onRemoveItem(int row)
{
    QString id = cartTable->item(row, 0)->data(Qt::UserRole).toString();
    tempSelection.remove(id);
    updateCartTable();
}

void ServiceSelectionDialog::onSearchTextChanged(const QString &text)
{
    filteredServices.clear();
    QString lower = text.toLower();
    for (DichVu *s : allServices) {
        if (QString::fromStdString(s->layTenDichVu()).toLower().contains(lower)) {
            filteredServices.append(s);
        }
    }
    createServiceCards();
}

void ServiceSelectionDialog::onCategoryFilterChanged(int index)
{
    // Implement category filtering logic if needed
    // For now just re-trigger search to refresh
    onSearchTextChanged(searchEdit->text());
}

void ServiceSelectionDialog::onConfirmClicked()
{
    accept();
}

void ServiceSelectionDialog::onCancelClicked()
{
    reject();
}
