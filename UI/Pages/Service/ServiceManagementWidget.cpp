/**
 * @file ServiceManagementWidget.cpp
 * @brief Implementation of Service Management Widget (CRUD) - Tab 2
 * @author khninh22 - Service Module
 * @date 2025-11-14
 */

#include "ServiceManagementWidget.h"
#include <QHeaderView>
#include <QFont>
#include <QScrollArea>
#include <QPixmap>
#include <QFile>
#include <QLabel>
#include <QMessageBox>
#include <QFileInfo>

ServiceManagementWidget::ServiceManagementWidget(QWidget *parent)
    : QWidget(parent),
      system(HeThongQuanLy::getInstance()),
      currentService(nullptr),
      isEditMode(false)
{
    setupUI();
    setupConnections();
    applyStyles();
    loadServices();
    updateStatsCards();
}

ServiceManagementWidget::~ServiceManagementWidget()
{
}

void ServiceManagementWidget::setupUI()
{
    // Main horizontal layout (Left 70% + Right 30%)
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    // ===== LEFT PANEL (70%) =====
    QWidget *leftWidget = new QWidget(this);
    leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setSpacing(15);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    // Search & Filters Row
    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->setSpacing(10);

    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("🔍 Tìm theo tên dịch vụ...");
    searchEdit->setObjectName("searchEdit");
    searchEdit->setMinimumHeight(40);
    filterLayout->addWidget(searchEdit, 3);

    categoryCombo = new QComboBox(this);
    categoryCombo->setObjectName("filterCombo");
    categoryCombo->addItem("Loại: Tất cả", "ALL");
    categoryCombo->addItem("Đồ uống", "DO_UONG");
    categoryCombo->addItem("Thiết bị", "THIET_BI");
    categoryCombo->addItem("Khác", "KHAC");
    categoryCombo->setMinimumHeight(40);
    filterLayout->addWidget(categoryCombo, 1);

    priceCombo = new QComboBox(this);
    priceCombo->setObjectName("filterCombo");
    priceCombo->addItem("Giá: Tất cả", "ALL");
    priceCombo->addItem("< 20,000đ", "LT_20K");
    priceCombo->addItem("20K - 50K", "20K_50K");
    priceCombo->addItem("50K - 100K", "50K_100K");
    priceCombo->addItem("> 100,000đ", "GT_100K");
    priceCombo->setMinimumHeight(40);
    filterLayout->addWidget(priceCombo, 1);

    reloadBtn = new QPushButton("🔄 Làm mới", this);
    reloadBtn->setObjectName("secondaryButton");
    reloadBtn->setMinimumHeight(40);
    filterLayout->addWidget(reloadBtn);

    leftLayout->addLayout(filterLayout);

    // Stats Cards
    QFrame *statsFrame = new QFrame(this);
    statsFrame->setObjectName("statsContainer");
    QHBoxLayout *statsLayout = new QHBoxLayout(statsFrame);
    statsLayout->setSpacing(15);
    statsLayout->setContentsMargins(0, 0, 0, 0);

    // Total card
    QFrame *totalCard = new QFrame(this);
    totalCard->setObjectName("statCard");
    QVBoxLayout *totalCardLayout = new QVBoxLayout(totalCard);
    QLabel *totalTitle = new QLabel("📦 Tổng dịch vụ", this);
    totalTitle->setObjectName("statTitle");
    totalServicesLabel = new QLabel("0", this);
    totalServicesLabel->setObjectName("statValue");
    totalCardLayout->addWidget(totalTitle);
    totalCardLayout->addWidget(totalServicesLabel);
    statsLayout->addWidget(totalCard);

    // Drink card
    QFrame *drinkCard = new QFrame(this);
    drinkCard->setObjectName("statCard");
    QVBoxLayout *drinkCardLayout = new QVBoxLayout(drinkCard);
    QLabel *drinkTitle = new QLabel("🍹 Đồ uống", this);
    drinkTitle->setObjectName("statTitle");
    drinkServicesLabel = new QLabel("0", this);
    drinkServicesLabel->setObjectName("statValue");
    drinkCardLayout->addWidget(drinkTitle);
    drinkCardLayout->addWidget(drinkServicesLabel);
    statsLayout->addWidget(drinkCard);

    // Equipment card
    QFrame *equipCard = new QFrame(this);
    equipCard->setObjectName("statCard");
    QVBoxLayout *equipCardLayout = new QVBoxLayout(equipCard);
    QLabel *equipTitle = new QLabel("⚽ Thiết bị", this);
    equipTitle->setObjectName("statTitle");
    equipmentServicesLabel = new QLabel("0", this);
    equipmentServicesLabel->setObjectName("statValue");
    equipCardLayout->addWidget(equipTitle);
    equipCardLayout->addWidget(equipmentServicesLabel);
    statsLayout->addWidget(equipCard);

    leftLayout->addWidget(statsFrame);

    // Service Table
    serviceTable = new QTableWidget(0, 9, this);
    serviceTable->setObjectName("dataTable");
    serviceTable->setHorizontalHeaderLabels({"Ảnh", "Mã DV", "Tên DV", "Loại", "Đơn vị", "Giá", "Số lượng", "Đã bán", "Trạng thái"});
    serviceTable->horizontalHeader()->setStretchLastSection(false);
    serviceTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);  // Ảnh
    serviceTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);  // Mã
    serviceTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch); // Tên
    serviceTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);  // Loại
    serviceTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);  // Đơn vị
    serviceTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);  // Giá
    serviceTable->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed);  // Số lượng
    serviceTable->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);  // Đã bán
    serviceTable->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Fixed);  // Trạng thái
    serviceTable->setColumnWidth(0, 60);   // Ảnh
    serviceTable->setColumnWidth(1, 80);   // Mã DV
    serviceTable->setColumnWidth(3, 90);   // Loại
    serviceTable->setColumnWidth(4, 80);   // Đơn vị
    serviceTable->setColumnWidth(5, 90);   // Giá
    serviceTable->setColumnWidth(6, 80);   // Số lượng
    serviceTable->setColumnWidth(7, 80);   // Đã bán
    serviceTable->setColumnWidth(8, 100);  // Trạng thái
    serviceTable->verticalHeader()->setVisible(false);
    serviceTable->verticalHeader()->setDefaultSectionSize(70); // Chiều cao hàng 70px cho ảnh
    serviceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    serviceTable->setSelectionMode(QAbstractItemView::SingleSelection);
    serviceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    leftLayout->addWidget(serviceTable);

    mainLayout->addWidget(leftWidget, 7); // 70%

    // ===== RIGHT PANEL (30%) =====
    QWidget *rightWidget = new QWidget(this);
    rightWidget->setMinimumWidth(350);
    rightWidget->setMaximumWidth(450);
    rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setSpacing(0);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    // Scroll area for form
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    formFrame = new QFrame();
    formFrame->setObjectName("formFrame");
    QVBoxLayout *formLayout = new QVBoxLayout(formFrame);
    formLayout->setSpacing(8);
    formLayout->setContentsMargins(15, 15, 15, 15);

    // Title
    QLabel *formTitle = new QLabel("📝 THÔNG TIN DỊCH VỤ", this);
    formTitle->setObjectName("sectionTitle");
    formTitle->setWordWrap(true);
    QFont titleFont = formTitle->font();
    titleFont.setPointSize(11);
    titleFont.setBold(true);
    formTitle->setFont(titleFont);
    formLayout->addWidget(formTitle);

    // Add New Button
    addNewBtn = new QPushButton("+ Thêm dịch vụ mới", this);
    addNewBtn->setObjectName("primaryButton");
    addNewBtn->setFixedHeight(38);
    formLayout->addWidget(addNewBtn);

    formLayout->addSpacing(15);

    // Service Code - horizontal layout
    QHBoxLayout *codeLayout = new QHBoxLayout();
    codeLayout->setSpacing(10);
    QLabel *codeLabel = new QLabel("Mã:", this);
    codeLabel->setObjectName("formLabel");
    codeLabel->setFixedWidth(90);
    codeLayout->addWidget(codeLabel);
    codeEdit = new QLineEdit(this);
    codeEdit->setObjectName("formInput");
    codeEdit->setReadOnly(true);
    codeEdit->setEnabled(false);
    codeLayout->addWidget(codeEdit, 1);
    formLayout->addLayout(codeLayout);
    formLayout->addSpacing(10);

    // Service Name - horizontal layout
    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->setSpacing(10);
    QLabel *nameLabel = new QLabel("Tên:", this);
    nameLabel->setObjectName("formLabel");
    nameLabel->setFixedWidth(90);
    nameLayout->addWidget(nameLabel);
    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Nhập tên dịch vụ");
    nameEdit->setObjectName("formInput");
    nameLayout->addWidget(nameEdit, 1);
    formLayout->addLayout(nameLayout);
    formLayout->addSpacing(10);

    // Category - horizontal layout
    QHBoxLayout *categoryLayout = new QHBoxLayout();
    categoryLayout->setSpacing(10);
    QLabel *categoryLabel = new QLabel("Loại:", this);
    categoryLabel->setObjectName("formLabel");
    categoryLabel->setFixedWidth(90);
    categoryLayout->addWidget(categoryLabel);
    categoryEdit = new QComboBox(this);
    categoryEdit->setObjectName("formInput");
    categoryEdit->addItem("Đồ uống", static_cast<int>(LoaiDichVu::DO_UONG));
    categoryEdit->addItem("Thiết bị", static_cast<int>(LoaiDichVu::THIET_BI));
    categoryEdit->addItem("Khác", static_cast<int>(LoaiDichVu::KHAC));
    categoryLayout->addWidget(categoryEdit, 1);
    formLayout->addLayout(categoryLayout);
    formLayout->addSpacing(10);

    // Price - horizontal layout
    QHBoxLayout *priceLayout = new QHBoxLayout();
    priceLayout->setSpacing(10);
    QLabel *priceLabel = new QLabel("Giá (đ):", this);
    priceLabel->setObjectName("formLabel");
    priceLabel->setFixedWidth(90);
    priceLayout->addWidget(priceLabel);
    priceEdit = new QLineEdit(this);
    priceEdit->setPlaceholderText("15000");
    priceEdit->setObjectName("formInput");
    priceLayout->addWidget(priceEdit, 1);
    formLayout->addLayout(priceLayout);
    formLayout->addSpacing(10);

    // Unit - horizontal layout
    QHBoxLayout *unitLayout = new QHBoxLayout();
    unitLayout->setSpacing(10);
    QLabel *unitLabel = new QLabel("Đơn vị:", this);
    unitLabel->setObjectName("formLabel");
    unitLabel->setFixedWidth(90);
    unitLayout->addWidget(unitLabel);
    unitEdit = new QLineEdit(this);
    unitEdit->setPlaceholderText("Lon, Chai...");
    unitEdit->setObjectName("formInput");
    unitLayout->addWidget(unitEdit, 1);
    formLayout->addLayout(unitLayout);
    formLayout->addSpacing(10);

    // Available checkbox
    availableCheckBox = new QCheckBox("✓ Có sẵn", this);
    availableCheckBox->setObjectName("formCheckbox");
    availableCheckBox->setChecked(true);
    formLayout->addWidget(availableCheckBox);
    formLayout->addSpacing(10);

    // Description
    QLabel *descLabel = new QLabel("Mô tả:", this);
    descLabel->setObjectName("fieldLabel");
    descLabel->setWordWrap(true);
    formLayout->addWidget(descLabel);
    descriptionEdit = new QTextEdit(this);
    descriptionEdit->setPlaceholderText("Mô tả...");
    descriptionEdit->setObjectName("formTextArea");
    descriptionEdit->setFixedHeight(45);
    formLayout->addWidget(descriptionEdit);

    formLayout->addSpacing(15);

    // Action Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(6);

    saveBtn = new QPushButton("Lưu", this);
    saveBtn->setObjectName("primaryButton");
    saveBtn->setFixedHeight(34);
    saveBtn->setEnabled(false);
    btnLayout->addWidget(saveBtn);

    deleteBtn = new QPushButton("Xóa", this);
    deleteBtn->setObjectName("dangerButton");
    deleteBtn->setFixedHeight(34);
    deleteBtn->setEnabled(false);
    btnLayout->addWidget(deleteBtn);

    formLayout->addLayout(btnLayout);
    formLayout->addSpacing(15);
    formLayout->addStretch();

    scrollArea->setWidget(formFrame);
    rightLayout->addWidget(scrollArea);
    mainLayout->addWidget(rightWidget, 3); // 30%

    setLayout(mainLayout);
}

void ServiceManagementWidget::setupConnections()
{
    // Search & Filters
    connect(searchEdit, &QLineEdit::textChanged, this, &ServiceManagementWidget::onSearchTextChanged);
    connect(categoryCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ServiceManagementWidget::onCategoryFilterChanged);
    connect(priceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ServiceManagementWidget::onPriceFilterChanged);
    connect(reloadBtn, &QPushButton::clicked, this, &ServiceManagementWidget::onReloadClicked);

    // Table
    connect(serviceTable, &QTableWidget::cellClicked, this, &ServiceManagementWidget::onTableRowClicked);

    // Form Actions
    connect(addNewBtn, &QPushButton::clicked, this, &ServiceManagementWidget::onAddNewClicked);
    connect(saveBtn, &QPushButton::clicked, this, &ServiceManagementWidget::onSaveClicked);
    connect(deleteBtn, &QPushButton::clicked, this, &ServiceManagementWidget::onDeleteClicked);
}

void ServiceManagementWidget::applyStyles()
{
    // Styles already applied via global QSS (app.qss)
    // Just need to ensure objectNames are set correctly
}

void ServiceManagementWidget::loadServices()
{
    // TODO: Implement loading services from system
    allServices.clear();
    filteredServices.clear();

    const MangDong<DichVu *> &services = system->layDanhSachDichVu();
    for (int i = 0; i < services.size(); i++)
    {
        allServices.append(services[i]);
        filteredServices.append(services[i]);
    }

    // Update table
    serviceTable->setRowCount(filteredServices.size());
    for (int i = 0; i < filteredServices.size(); i++)
    {
        DichVu *service = filteredServices[i];
        
        // Column 0: Ảnh (Image)
        QLabel *imgLabel = new QLabel();
        imgLabel->setAlignment(Qt::AlignCenter);
        imgLabel->setFixedSize(50, 50);
        imgLabel->setScaledContents(false);
        
        QString imagePath = QString::fromStdString(service->layHinhAnh());
        if (!imagePath.isEmpty())
        {
            // Try with Data/ prefix
            QString fullPath = "Data/" + imagePath;
            if (QFile::exists(fullPath))
            {
                QPixmap pixmap(fullPath);
                imgLabel->setPixmap(pixmap.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
            else if (QFile::exists(imagePath))
            {
                QPixmap pixmap(imagePath);
                imgLabel->setPixmap(pixmap.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
            else
            {
                imgLabel->setStyleSheet("font-size: 24px;");
                imgLabel->setText("📦");
            }
        }
        else
        {
            imgLabel->setStyleSheet("font-size: 24px;");
            imgLabel->setText("📦");
        }
        serviceTable->setCellWidget(i, 0, imgLabel);
        
        // Column 1: Mã DV
        serviceTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(service->layMaDichVu())));
        
        // Column 2: Tên DV
        serviceTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(service->layTenDichVu())));
        
        // Column 3: Loại
        QString category;
        switch (service->layLoaiDichVu())
        {
        case LoaiDichVu::DO_UONG:
            category = "Đồ uống";
            break;
        case LoaiDichVu::THIET_BI:
            category = "Thiết bị";
            break;
        case LoaiDichVu::KHAC:
            category = "Khác";
            break;
        }
        serviceTable->setItem(i, 3, new QTableWidgetItem(category));
        
        // Column 4: Đơn vị
        serviceTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(service->layDonVi())));
        
        // Column 5: Giá
        serviceTable->setItem(i, 5, new QTableWidgetItem(QString::number(service->layDonGia(), 'f', 0) + "đ"));
        
        // Column 6: Số lượng (giả sử có 50)
        serviceTable->setItem(i, 6, new QTableWidgetItem("50"));
        
        // Column 7: Đã bán
        serviceTable->setItem(i, 7, new QTableWidgetItem(QString::number(service->laySoLuongBan())));
        
        // Column 8: Trạng thái
        QString status = service->coConHang() ? "✓ Còn hàng" : "✗ Hết hàng";
        serviceTable->setItem(i, 8, new QTableWidgetItem(status));
    }
}

void ServiceManagementWidget::loadServiceToForm(DichVu *service)
{
    if (!service)
        return;

    currentService = service;
    isEditMode = true;

    codeEdit->setText(QString::fromStdString(service->layMaDichVu()));
    nameEdit->setText(QString::fromStdString(service->layTenDichVu()));
    priceEdit->setText(QString::number(service->layDonGia(), 'f', 0));
    unitEdit->setText(QString::fromStdString(service->layDonVi()));
    descriptionEdit->setPlainText(QString::fromStdString(service->layMoTa()));
    availableCheckBox->setChecked(service->coConHang());
    
    // Set category
    int categoryIndex = categoryEdit->findData(static_cast<int>(service->layLoaiDichVu()));
    if (categoryIndex >= 0)
        categoryEdit->setCurrentIndex(categoryIndex);

    // Enable buttons
    saveBtn->setEnabled(true);
    deleteBtn->setEnabled(true);
}

void ServiceManagementWidget::clearForm()
{
    currentService = nullptr;
    isEditMode = false;

    // Auto-generate new service code
    QString newCode = generateNextServiceCode();
    codeEdit->setText(newCode);
    
    nameEdit->clear();
    priceEdit->clear();
    unitEdit->clear();
    descriptionEdit->clear();
    availableCheckBox->setChecked(true);
    categoryEdit->setCurrentIndex(0);

    nameEdit->setEnabled(false);
    priceEdit->setEnabled(false);
    unitEdit->setEnabled(false);
    descriptionEdit->setEnabled(false);
    availableCheckBox->setEnabled(false);
    categoryEdit->setEnabled(false);

    saveBtn->setEnabled(false);
    deleteBtn->setEnabled(false);
}

void ServiceManagementWidget::updateStatsCards()
{
    int totalCount = allServices.size();
    int drinkCount = 0;
    int equipCount = 0;

    for (DichVu *service : allServices)
    {
        switch (service->layLoaiDichVu())
        {
        case LoaiDichVu::DO_UONG:
            drinkCount++;
            break;
        case LoaiDichVu::THIET_BI:
            equipCount++;
            break;
        default:
            break;
        }
    }

    totalServicesLabel->setText(QString::number(totalCount));
    drinkServicesLabel->setText(QString::number(drinkCount));
    equipmentServicesLabel->setText(QString::number(equipCount));
}

bool ServiceManagementWidget::validateServiceData()
{
    if (nameEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập tên dịch vụ!");
        return false;
    }

    if (priceEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập đơn giá!");
        return false;
    }

    bool ok;
    double price = priceEdit->text().toDouble(&ok);
    if (!ok || price < 0)
    {
        QMessageBox::warning(this, "Lỗi", "Đơn giá không hợp lệ!");
        return false;
    }

    return true;
}

void ServiceManagementWidget::onSearchTextChanged(const QString &text)
{
    // TODO: Implement search filter
    Q_UNUSED(text);
}

void ServiceManagementWidget::onCategoryFilterChanged(int index)
{
    // TODO: Implement category filter
    Q_UNUSED(index);
}

void ServiceManagementWidget::onPriceFilterChanged(int index)
{
    // TODO: Implement price filter
    Q_UNUSED(index);
}

void ServiceManagementWidget::onReloadClicked()
{
    loadServices();
    updateStatsCards();
    clearForm();
}

void ServiceManagementWidget::onAddNewClicked()
{
    clearForm();
    
    // Enable form for new entry
    nameEdit->setEnabled(true);
    priceEdit->setEnabled(true);
    unitEdit->setEnabled(true);
    descriptionEdit->setEnabled(true);
    availableCheckBox->setEnabled(true);
    categoryEdit->setEnabled(true);
    saveBtn->setEnabled(true);
    
    // Focus on name field
    nameEdit->setFocus();
}

void ServiceManagementWidget::onTableRowClicked(int row)
{
    if (row < 0 || row >= filteredServices.size())
        return;

    DichVu *service = filteredServices[row];
    loadServiceToForm(service);
    
    // Enable form for editing
    nameEdit->setEnabled(true);
    priceEdit->setEnabled(true);
    unitEdit->setEnabled(true);
    descriptionEdit->setEnabled(true);
    availableCheckBox->setEnabled(true);
    categoryEdit->setEnabled(true);
}

void ServiceManagementWidget::onSaveClicked()
{
    if (!validateServiceData())
        return;

    QString name = nameEdit->text().trimmed();
    QString priceStr = priceEdit->text().trimmed();
    QString unit = unitEdit->text().trimmed();
    QString description = descriptionEdit->toPlainText().trimmed();
    bool available = availableCheckBox->isChecked();
    LoaiDichVu category = static_cast<LoaiDichVu>(categoryEdit->currentData().toInt());
    
    double price = priceStr.toDouble();

    if (isEditMode && currentService)
    {
        // Edit existing service
        currentService->datTenDichVu(name.toStdString());
        currentService->datDonGia(price);
        currentService->datDonVi(unit.toStdString());
        currentService->datMoTa(description.toStdString());
        currentService->datConHang(available);

        // Save to CSV
        system->luuCSV("D:/QT_PBL2/Data");

        QMessageBox::information(this, "Thành công",
                                 "Cập nhật dịch vụ thành công!");
    }
    else
    {
        // Add new service
        // Generate unique service ID
        QString maDV = QString("DV%1").arg(system->layQuanLyDichVu()->tongSoDichVu() + 1, 3, 10, QChar('0'));
        
        DichVu *newService = new DichVu(
            maDV.toStdString(),
            name.toStdString(),
            price,
            category
        );
        
        newService->datDonVi(unit.toStdString());
        newService->datMoTa(description.toStdString());
        newService->datConHang(available);
        newService->datSoLuongBan(0);
        newService->datHinhAnh("images/product.jpg");

        bool added = system->layQuanLyDichVu()->themDichVu(newService);
        
        if (added)
        {
            // Save to CSV
            system->luuCSV("D:/QT_PBL2/Data");
            
            QMessageBox::information(this, "Thành công",
                                     QString("Thêm dịch vụ mới thành công!\nMã DV: %1").arg(maDV));
            clearForm();
        }
        else
        {
            delete newService;
            QMessageBox::warning(this, "Lỗi", "Không thể thêm dịch vụ!");
        }
    }

    loadServices();
    updateStatsCards();
}

void ServiceManagementWidget::onDeleteClicked()
{
    if (!currentService)
        return;

    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                              "Xác nhận xóa",
                                                              QString("Bạn có chắc muốn xóa dịch vụ '%1'?\n\n"
                                                                      "Lưu ý: Mã dịch vụ này sẽ không được tái sử dụng.")
                                                                  .arg(QString::fromStdString(currentService->layTenDichVu())),
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        std::string maDV = currentService->layMaDichVu();
        if (system->layQuanLyDichVu()->xoaDichVu(maDV))
        {
            // Save to CSV
            system->luuCSV("D:/QT_PBL2/Data");
            
            QMessageBox::information(this, "Thành công", "Xóa dịch vụ thành công!");
            clearForm();
            loadServices();
            updateStatsCards();
        }
        else
        {
            QMessageBox::warning(this, "Lỗi", "Không thể xóa dịch vụ!");
        }
    }
}

QString ServiceManagementWidget::generateNextServiceCode()
{
    const MangDong<DichVu *> &allServices = system->layDanhSachDichVu();
    int maxNumber = 0;

    for (int i = 0; i < allServices.size(); i++)
    {
        QString code = QString::fromStdString(allServices[i]->layMaDichVu());
        if (code.startsWith("DV"))
        {
            bool ok;
            int num = code.mid(2).toInt(&ok);
            if (ok && num > maxNumber)
            {
                maxNumber = num;
            }
        }
    }

    return QString("DV%1").arg(maxNumber + 1, 3, 10, QChar('0'));
}
