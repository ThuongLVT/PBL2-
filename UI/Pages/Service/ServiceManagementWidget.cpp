#include "ServiceManagementWidget.h"
#include "Core/ThuatToan/MergeSort.h"
#include "Core/CauTrucDuLieu/MangDong.h"
#include <QHeaderView>
#include <QFont>
#include <QScrollArea>
#include <QPixmap>
#include <QFile>
#include <QLabel>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QCompleter>
#include <QStringListModel>
#include <QScrollBar>
#include <algorithm>
#include <QStyledItemDelegate>

// Delegate to keep Price column Green even when selected
class PriceDelegate : public QStyledItemDelegate
{
public:
    PriceDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override
    {
        QStyledItemDelegate::initStyleOption(option, index);
        if (index.column() == 3) // Price column
        {
            // Force text color to be Green (#059669) in all states (Normal & Selected)
            option->palette.setColor(QPalette::Text, QColor("#059669"));
            option->palette.setColor(QPalette::HighlightedText, QColor("#059669"));
        }
    }
};

ServiceManagementWidget::ServiceManagementWidget(QWidget *parent)
    : QWidget(parent),
      system(HeThongQuanLy::getInstance()),
      currentService(nullptr),
      isEditMode(false)
{
    setupUI();
    setupConnections();
    loadServices();
}

ServiceManagementWidget::~ServiceManagementWidget()
{
}

void ServiceManagementWidget::setupUI()
{
    // Main horizontal layout
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(20); // Gap between panels

    // ================= LEFT PANEL (70%) =================
    QFrame *leftPanel = new QFrame(this);
    leftPanel->setStyleSheet("background-color: white; border-radius: 10px; border: 1px solid #e5e7eb;");
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setSpacing(15);
    leftLayout->setContentsMargins(20, 20, 20, 20);

    // --- Header: Search & Filters ---
    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->setSpacing(10);

    // Search
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("Tìm theo tên dịch vụ...");
    searchEdit->setMinimumHeight(35);
    searchEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    searchEdit->setStyleSheet("border: 1px solid #d1d5db; border-radius: 5px; padding: 5px;");

    // Search Button
    searchBtn = new QPushButton(this);
    searchBtn->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
    searchBtn->setFixedWidth(35);
    searchBtn->setMinimumHeight(35);
    searchBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    searchBtn->setCursor(Qt::PointingHandCursor);
    searchBtn->setStyleSheet("QPushButton { border: 1px solid #d1d5db; border-radius: 5px; background-color: #f3f4f6; } QPushButton:hover { background-color: #e5e7eb; }");

    // Search Completer
    searchCompleter = new QCompleter(this);
    searchCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    searchCompleter->setFilterMode(Qt::MatchContains);
    searchEdit->setCompleter(searchCompleter);

    filterLayout->addWidget(searchEdit, 3);
    filterLayout->addWidget(searchBtn);

    // Reload Button
    reloadBtn = new QPushButton(this);
    reloadBtn->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    reloadBtn->setToolTip("Làm mới dữ liệu");
    reloadBtn->setFixedWidth(35);
    reloadBtn->setMinimumHeight(35);
    reloadBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    reloadBtn->setStyleSheet("QPushButton { border: 1px solid #d1d5db; border-radius: 5px; background-color: #f3f4f6; } QPushButton:hover { background-color: #e5e7eb; }");
    filterLayout->addWidget(reloadBtn);

    // Filter 1: Status (Moved to first)
    statusFilterCombo = new QComboBox(this);
    statusFilterCombo->addItem("Tất cả", "ALL");
    statusFilterCombo->addItem("Hoạt động", "ACTIVE");
    statusFilterCombo->addItem("Ngừng bán", "INACTIVE");
    statusFilterCombo->setMinimumHeight(35);
    statusFilterCombo->setStyleSheet("border: 1px solid #d1d5db; border-radius: 5px; padding: 5px;");
    filterLayout->addWidget(statusFilterCombo, 1);

    // Filter 2: Sort By (Price, Stock, Sold)
    sortByCombo = new QComboBox(this);
    sortByCombo->addItem("Mặc định", -1);
    sortByCombo->addItem("Giá", 0);
    sortByCombo->addItem("Tồn kho", 1);
    sortByCombo->addItem("Đã bán", 2);
    sortByCombo->setMinimumHeight(35);
    sortByCombo->setStyleSheet("border: 1px solid #d1d5db; border-radius: 5px; padding: 5px;");
    filterLayout->addWidget(sortByCombo, 1);

    // Filter 3: Sort Order (Asc/Desc)
    sortOrderCombo = new QComboBox(this);
    sortOrderCombo->addItem("Tăng dần", 0);
    sortOrderCombo->addItem("Giảm dần", 1);
    sortOrderCombo->setMinimumHeight(35);
    sortOrderCombo->setStyleSheet("border: 1px solid #d1d5db; border-radius: 5px; padding: 5px;");
    filterLayout->addWidget(sortOrderCombo, 1);

    // Filter 4: Unit
    unitFilterCombo = new QComboBox(this);
    unitFilterCombo->addItem("Tất cả", "ALL");
    unitFilterCombo->setMinimumHeight(35);
    unitFilterCombo->setStyleSheet("border: 1px solid #d1d5db; border-radius: 5px; padding: 5px;");
    filterLayout->addWidget(unitFilterCombo, 1);

    leftLayout->addLayout(filterLayout);

    // --- Table ---
    serviceTable = new QTableWidget(0, 9, this);
    serviceTable->setHorizontalHeaderLabels({"Ảnh", "Mã DV", "Tên dịch vụ", "Giá", "Đơn vị", "Kho", "Bán", "Trạng thái", "Mô tả"});

    // Table Styling
    serviceTable->setShowGrid(false);                                       // Hide all grid lines first
    serviceTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel); // Smooth scrolling
    serviceTable->verticalScrollBar()->setStyleSheet(
        "QScrollBar:vertical { border: none; background: #f1f1f1; width: 8px; margin: 0px; }"
        "QScrollBar::handle:vertical { background: #c1c1c1; min-height: 20px; border-radius: 4px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }");
    serviceTable->setStyleSheet(
        "QTableWidget { border: none; selection-background-color: #e0f2fe; selection-color: #000; background-color: white; }"
        "QHeaderView::section { background-color: white; padding: 8px; border: none; border-bottom: 2px solid #f3f4f6; font-weight: bold; color: #6b7280; }"
        "QTableWidget::item { border-bottom: 1px solid #f3f4f6; padding: 5px; }");
    serviceTable->setFocusPolicy(Qt::NoFocus);
    serviceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    serviceTable->setSelectionMode(QAbstractItemView::SingleSelection);
    serviceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    serviceTable->verticalHeader()->setVisible(false);
    serviceTable->verticalHeader()->setDefaultSectionSize(80); // Height for 64x64 image

    // Column Sizing & Alignment
    serviceTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed); // Image
    serviceTable->setColumnWidth(0, 80);
    serviceTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed); // Code
    serviceTable->setColumnWidth(1, 80);
    serviceTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed); // Name
    serviceTable->setColumnWidth(2, 150);
    serviceTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed); // Price
    serviceTable->setColumnWidth(3, 100);
    serviceTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed); // Unit
    serviceTable->setColumnWidth(4, 80);
    serviceTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed); // Stock
    serviceTable->setColumnWidth(5, 80);
    serviceTable->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed); // Sold
    serviceTable->setColumnWidth(6, 80);
    serviceTable->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed); // Status
    serviceTable->setColumnWidth(7, 120);
    serviceTable->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch); // Description

    // Header Alignment
    serviceTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // Apply Price Delegate
    serviceTable->setItemDelegateForColumn(3, new PriceDelegate(serviceTable));

    // Set specific header alignments
    // Note: QTableWidget headers are items. We need to ensure they exist or use a delegate.
    // But setHorizontalHeaderLabels creates items.
    if (serviceTable->horizontalHeaderItem(0))
        serviceTable->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignCenter); // Image
    if (serviceTable->horizontalHeaderItem(1))
        serviceTable->horizontalHeaderItem(1)->setTextAlignment(Qt::AlignCenter); // Code
    if (serviceTable->horizontalHeaderItem(2))
        serviceTable->horizontalHeaderItem(2)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter); // Name
    if (serviceTable->horizontalHeaderItem(3))
        serviceTable->horizontalHeaderItem(3)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter); // Price
    if (serviceTable->horizontalHeaderItem(4))
        serviceTable->horizontalHeaderItem(4)->setTextAlignment(Qt::AlignCenter); // Unit
    if (serviceTable->horizontalHeaderItem(5))
        serviceTable->horizontalHeaderItem(5)->setTextAlignment(Qt::AlignCenter); // Stock
    if (serviceTable->horizontalHeaderItem(6))
        serviceTable->horizontalHeaderItem(6)->setTextAlignment(Qt::AlignCenter); // Sold
    if (serviceTable->horizontalHeaderItem(7))
        serviceTable->horizontalHeaderItem(7)->setTextAlignment(Qt::AlignCenter); // Status
    if (serviceTable->horizontalHeaderItem(8))
        serviceTable->horizontalHeaderItem(8)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter); // Desc

    leftLayout->addWidget(serviceTable);
    mainLayout->addWidget(leftPanel, 7); // 70%

    // ================= RIGHT PANEL (30%) =================
    QFrame *rightPanel = new QFrame(this);
    rightPanel->setStyleSheet("background-color: white; border-radius: 10px; border: 1px solid #e5e7eb;");
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setSpacing(15);
    rightLayout->setContentsMargins(20, 20, 20, 20);

    // --- Image Upload Area ---
    QVBoxLayout *imageLayout = new QVBoxLayout();

    imagePreviewLabel = new QLabel("Chưa có ảnh", this);
    imagePreviewLabel->setAlignment(Qt::AlignCenter);
    imagePreviewLabel->setFixedSize(120, 120);
    imagePreviewLabel->setStyleSheet("border: 2px dashed #d1d5db; border-radius: 8px; color: #9ca3af; margin-bottom: 10px;");

    uploadImageBtn = new QPushButton("📷 Tải ảnh lên", this);
    uploadImageBtn->setCursor(Qt::PointingHandCursor);
    uploadImageBtn->setStyleSheet("QPushButton { border: 1px solid #d1d5db; border-radius: 5px; padding: 5px 15px; background-color: white; } QPushButton:hover { background-color: #f9fafb; border-color: #9ca3af; }");

    imageLayout->addWidget(imagePreviewLabel, 0, Qt::AlignCenter);
    imageLayout->addWidget(uploadImageBtn, 0, Qt::AlignCenter);

    rightLayout->addLayout(imageLayout);
    rightLayout->addSpacing(10);

    // --- Form Fields ---
    QVBoxLayout *formLayout = new QVBoxLayout();
    formLayout->setSpacing(12);

    // Helper lambda for row layout
    auto addRow = [&](const QString &label, QWidget *widget)
    {
        QHBoxLayout *row = new QHBoxLayout();
        QLabel *lbl = new QLabel(label, this);
        lbl->setFixedWidth(70);
        lbl->setStyleSheet("font-weight: bold; color: #374151; border: none;"); // No border for labels

        // Style inputs
        if (qobject_cast<QLineEdit *>(widget))
        {
            widget->setStyleSheet("QLineEdit { border: 1px solid #d1d5db; border-radius: 5px; padding: 5px; } QLineEdit:focus { border-color: #3b82f6; }");
        }
        else if (qobject_cast<QComboBox *>(widget))
        {
            widget->setStyleSheet("QComboBox { border: 1px solid #d1d5db; border-radius: 5px; padding: 5px; }");
        }
        else if (qobject_cast<QTextEdit *>(widget))
        {
            widget->setStyleSheet("QTextEdit { border: 1px solid #d1d5db; border-radius: 5px; padding: 5px; }");
        }

        row->addWidget(lbl);
        row->addWidget(widget);
        formLayout->addLayout(row);
    };

    // Code (Hidden from UI but initialized)
    codeEdit = new QLineEdit(this);
    codeEdit->setVisible(false);

    // Name
    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Nhập tên dịch vụ...");
    addRow("Tên:", nameEdit);

    // Category
    categoryEdit = new QComboBox(this);
    categoryEdit->addItem("Đồ uống", static_cast<int>(LoaiDichVu::DO_UONG));
    categoryEdit->addItem("Đồ ăn", static_cast<int>(LoaiDichVu::DO_AN));
    categoryEdit->addItem("Thiết bị", static_cast<int>(LoaiDichVu::THIET_BI));
    addRow("Loại:", categoryEdit);

    // Price
    priceEdit = new QLineEdit(this);
    priceEdit->setPlaceholderText("0");
    addRow("Giá (đ):", priceEdit);

    // Unit
    unitEdit = new QLineEdit(this);
    unitEdit->setPlaceholderText("Lon, Chai, Cái...");
    addRow("Đơn vị:", unitEdit);

    // Stock
    stockEdit = new QLineEdit(this);
    stockEdit->setPlaceholderText("0");
    addRow("Kho:", stockEdit);

    // Status (Replaces Available Checkbox)
    statusEdit = new QComboBox(this);
    statusEdit->addItem("Hoạt động", true);
    statusEdit->addItem("Ngừng bán", false);
    addRow("Trạng thái:", statusEdit);

    // Description
    descriptionEdit = new QTextEdit(this);
    descriptionEdit->setPlaceholderText("Mô tả chi tiết...");
    descriptionEdit->setFixedHeight(60);
    descriptionEdit->setStyleSheet("QTextEdit { border: 1px solid #9ca3af; border-radius: 5px; padding: 5px; }");
    addRow("Mô tả:", descriptionEdit);

    rightLayout->addLayout(formLayout);
    rightLayout->addStretch();

    // --- Action Buttons ---
    QHBoxLayout *btnLayout = new QHBoxLayout();

    addNewBtn = new QPushButton("Thêm mới", this);
    addNewBtn->setCursor(Qt::PointingHandCursor);
    addNewBtn->setStyleSheet("QPushButton { background-color: #3b82f6; color: white; font-weight: bold; padding: 8px; border-radius: 5px; border: none; } QPushButton:hover { background-color: #2563eb; }");

    saveBtn = new QPushButton("Lưu", this);
    saveBtn->setCursor(Qt::PointingHandCursor);
    saveBtn->setStyleSheet("QPushButton { background-color: #10b981; color: white; font-weight: bold; padding: 8px; border-radius: 5px; border: none; } QPushButton:hover { background-color: #059669; } QPushButton:disabled { background-color: #d1d5db; }");
    saveBtn->setEnabled(false);

    deleteBtn = new QPushButton("Xóa", this);
    deleteBtn->setCursor(Qt::PointingHandCursor);
    deleteBtn->setStyleSheet("QPushButton { background-color: #ef4444; color: white; font-weight: bold; padding: 8px; border-radius: 5px; border: none; } QPushButton:hover { background-color: #dc2626; } QPushButton:disabled { background-color: #d1d5db; }");
    deleteBtn->setEnabled(false);

    btnLayout->addWidget(addNewBtn);
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(deleteBtn);

    rightLayout->addLayout(btnLayout);

    mainLayout->addWidget(rightPanel, 3); // 30%
}

void ServiceManagementWidget::setupConnections()
{
    // Filters
    // connect(searchEdit, &QLineEdit::textChanged, this, &ServiceManagementWidget::onSearchTextChanged); // Removed auto-search
    connect(searchEdit, &QLineEdit::returnPressed, this, [this]()
            { filterServices(); }); // Search on Enter
    connect(searchBtn, &QPushButton::clicked, this, [this]()
            { filterServices(); }); // Search on Button Click

    connect(sortByCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ServiceManagementWidget::onSortByChanged);
    connect(sortOrderCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ServiceManagementWidget::onSortOrderChanged);
    connect(unitFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ServiceManagementWidget::onUnitFilterChanged);
    connect(statusFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ServiceManagementWidget::onStatusFilterChanged);
    connect(reloadBtn, &QPushButton::clicked, this, &ServiceManagementWidget::onReloadClicked);

    // Table
    connect(serviceTable, &QTableWidget::cellClicked, this, &ServiceManagementWidget::onTableRowClicked);

    // Actions
    connect(uploadImageBtn, &QPushButton::clicked, this, &ServiceManagementWidget::onUploadImageClicked);
    connect(addNewBtn, &QPushButton::clicked, this, &ServiceManagementWidget::onAddNewClicked);
    connect(saveBtn, &QPushButton::clicked, this, &ServiceManagementWidget::onSaveClicked);
    connect(deleteBtn, &QPushButton::clicked, this, &ServiceManagementWidget::onDeleteClicked);
}

void ServiceManagementWidget::loadServices()
{
    allServices.clear();
    QStringList serviceNames;
    const MangDong<DichVu *> &services = system->layDanhSachDichVu();
    for (int i = 0; i < services.size(); i++)
    {
        allServices.append(services[i]);
        serviceNames << QString::fromStdString(services[i]->layTenDichVu());
    }

    if (searchCompleter)
    {
        searchCompleter->setModel(new QStringListModel(serviceNames, searchCompleter));
    }

    updateUnitFilter();
    filterServices();
}

void ServiceManagementWidget::updateUnitFilter()
{
    QString currentUnit = unitFilterCombo->currentData().toString();
    unitFilterCombo->blockSignals(true);
    unitFilterCombo->clear();
    unitFilterCombo->addItem("Tất cả", "ALL");

    QStringList units;
    for (DichVu *dv : allServices)
    {
        QString u = QString::fromStdString(dv->layDonVi()).trimmed();
        if (!u.isEmpty() && !units.contains(u))
        {
            units.append(u);
        }
    }
    units.sort();

    for (const QString &u : units)
    {
        unitFilterCombo->addItem(u, u);
    }

    int idx = unitFilterCombo->findData(currentUnit);
    if (idx != -1)
        unitFilterCombo->setCurrentIndex(idx);
    unitFilterCombo->blockSignals(false);
}

void ServiceManagementWidget::filterServices()
{
    displayedServices.clear();
    QString searchText = searchEdit->text().trimmed().toLower();
    QString unitFilter = unitFilterCombo->currentData().toString();
    QString statusFilter = statusFilterCombo->currentData().toString();

    for (DichVu *dv : allServices)
    {
        // Search Filter
        if (!searchText.isEmpty())
        {
            if (!QString::fromStdString(dv->layTenDichVu()).toLower().contains(searchText))
            {
                continue;
            }
        }

        // Unit Filter
        if (unitFilter != "ALL")
        {
            if (QString::fromStdString(dv->layDonVi()) != unitFilter)
            {
                continue;
            }
        }

        // Status Filter
        if (statusFilter != "ALL")
        {
            bool isActive = dv->coConHang();
            if (statusFilter == "ACTIVE" && !isActive)
                continue;
            if (statusFilter == "INACTIVE" && isActive)
                continue;
        }

        displayedServices.append(dv);
    }

    sortServices();
}

// Comparison functions for MergeSort
static bool comparePriceAsc(DichVu* const &a, DichVu* const &b) {
    return a->layDonGia() < b->layDonGia();
}
static bool comparePriceDesc(DichVu* const &a, DichVu* const &b) {
    return a->layDonGia() > b->layDonGia();
}

static bool compareStockAsc(DichVu* const &a, DichVu* const &b) {
    return a->laySoLuongTon() < b->laySoLuongTon();
}
static bool compareStockDesc(DichVu* const &a, DichVu* const &b) {
    return a->laySoLuongTon() > b->laySoLuongTon();
}

static bool compareSoldAsc(DichVu* const &a, DichVu* const &b) {
    return a->laySoLuongBan() < b->laySoLuongBan();
}
static bool compareSoldDesc(DichVu* const &a, DichVu* const &b) {
    return a->laySoLuongBan() > b->laySoLuongBan();
}

void ServiceManagementWidget::sortServices()
{
    int sortBy = sortByCombo->currentData().toInt(); // -1: Default, 0: Price, 1: Stock, 2: Sold
    int sortOrder = sortOrderCombo->currentIndex();  // 0: Asc, 1: Desc

    if (sortBy != -1)
    {
        // Convert QList to MangDong for MergeSort
        MangDong<DichVu*> tempArray;
        for(DichVu* dv : displayedServices) {
            tempArray.push_back(dv);
        }

        // Select Comparator
        bool (*comparator)(DichVu* const &, DichVu* const &) = nullptr;

        if (sortBy == 0) { // Price
            comparator = (sortOrder == 0) ? comparePriceAsc : comparePriceDesc;
        } else if (sortBy == 1) { // Stock
            comparator = (sortOrder == 0) ? compareStockAsc : compareStockDesc;
        } else if (sortBy == 2) { // Sold
            comparator = (sortOrder == 0) ? compareSoldAsc : compareSoldDesc;
        }

        // Apply MergeSort
        if (comparator) {
            MergeSort<DichVu*>::sort(tempArray, comparator);
        }

        // Convert back to QList
        displayedServices.clear();
        for(int i = 0; i < tempArray.size(); i++) {
            displayedServices.append(tempArray[i]);
        }
    }

    // Update Table
    serviceTable->setRowCount(0);
    serviceTable->setRowCount(displayedServices.size());

    for (int i = 0; i < displayedServices.size(); i++)
    {
        DichVu *dv = displayedServices[i];

        // Image
        QLabel *imgLabel = new QLabel();
        imgLabel->setAlignment(Qt::AlignCenter);
        QString imgPath = QString::fromStdString(dv->layHinhAnh());
        if (!imgPath.isEmpty() && QFile::exists(imgPath))
        {
            QPixmap pix(imgPath);
            imgLabel->setPixmap(pix.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        else
        {
            imgLabel->setText("📦");
            imgLabel->setStyleSheet("font-size: 24px; color: #ccc;");
        }
        serviceTable->setCellWidget(i, 0, imgLabel);

        // Code
        QTableWidgetItem *itemCode = new QTableWidgetItem(QString::fromStdString(dv->layMaDichVu()));
        itemCode->setTextAlignment(Qt::AlignCenter);
        serviceTable->setItem(i, 1, itemCode);

        // Name
        QTableWidgetItem *itemName = new QTableWidgetItem(QString::fromStdString(dv->layTenDichVu()));
        itemName->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        serviceTable->setItem(i, 2, itemName);

        // Price
        QTableWidgetItem *itemPrice = new QTableWidgetItem(QString::number(dv->layDonGia(), 'f', 0) + "đ");
        itemPrice->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        itemPrice->setForeground(QBrush(QColor("#059669"))); // Dark Green
        itemPrice->setFont(QFont("Segoe UI", 10, QFont::Bold));
        serviceTable->setItem(i, 3, itemPrice);

        // Unit
        QTableWidgetItem *itemUnit = new QTableWidgetItem(QString::fromStdString(dv->layDonVi()));
        itemUnit->setTextAlignment(Qt::AlignCenter);
        serviceTable->setItem(i, 4, itemUnit);

        // Stock
        QTableWidgetItem *itemStock = new QTableWidgetItem(QString::number(dv->laySoLuongTon()));
        itemStock->setTextAlignment(Qt::AlignCenter);
        serviceTable->setItem(i, 5, itemStock);

        // Sold
        QTableWidgetItem *itemSold = new QTableWidgetItem(QString::number(dv->laySoLuongBan()));
        itemSold->setTextAlignment(Qt::AlignCenter);
        serviceTable->setItem(i, 6, itemSold);

        // Status
        QLabel *statusLabel = new QLabel();
        statusLabel->setAlignment(Qt::AlignCenter);
        if (dv->coConHang())
        {
            statusLabel->setText("Hoạt động");
            statusLabel->setStyleSheet("color: #059669; font-weight: bold; background-color: #d1fae5; border-radius: 10px; padding: 2px 8px;");
        }
        else
        {
            statusLabel->setText("Ngừng bán");
            statusLabel->setStyleSheet("color: #dc2626; font-weight: bold; background-color: #fee2e2; border-radius: 10px; padding: 2px 8px;");
        }

        // Wrap label in a widget to center it properly with padding
        QWidget *statusWidget = new QWidget();
        statusWidget->setStyleSheet("background-color: transparent; border: none;"); // Ensure no border on container
        QHBoxLayout *statusLayout = new QHBoxLayout(statusWidget);
        statusLayout->setContentsMargins(0, 0, 0, 0);
        statusLayout->setAlignment(Qt::AlignCenter);
        statusLayout->addWidget(statusLabel);
        serviceTable->setCellWidget(i, 7, statusWidget);

        // Description
        QTableWidgetItem *itemDesc = new QTableWidgetItem(QString::fromStdString(dv->layMoTa()));
        itemDesc->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        serviceTable->setItem(i, 8, itemDesc);
    }
}

void ServiceManagementWidget::onSearchTextChanged(const QString &text) { filterServices(); }
void ServiceManagementWidget::onSortByChanged(int index) { sortServices(); }
void ServiceManagementWidget::onSortOrderChanged(int index) { sortServices(); }
void ServiceManagementWidget::onUnitFilterChanged(int index) { filterServices(); }
void ServiceManagementWidget::onStatusFilterChanged(int index) { filterServices(); }
void ServiceManagementWidget::onReloadClicked()
{
    searchEdit->clear();
    loadServices();
    clearForm();
}

void ServiceManagementWidget::onAddNewClicked()
{
    clearForm();
    codeEdit->setText(generateNextServiceCode());
    nameEdit->setFocus();
    saveBtn->setEnabled(true);
    deleteBtn->setEnabled(false);
}

void ServiceManagementWidget::onTableRowClicked(int row)
{
    if (row < 0 || row >= displayedServices.size())
        return;

    DichVu *dv = displayedServices[row];
    loadServiceToForm(dv);
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
    stockEdit->setText(QString::number(service->laySoLuongTon()));
    descriptionEdit->setPlainText(QString::fromStdString(service->layMoTa()));

    // Set Status
    int statusIdx = statusEdit->findData(service->coConHang());
    if (statusIdx != -1)
        statusEdit->setCurrentIndex(statusIdx);

    int catIdx = categoryEdit->findData(static_cast<int>(service->layLoaiDichVu()));
    if (catIdx != -1)
        categoryEdit->setCurrentIndex(catIdx);

    // Load Image Preview
    currentImagePath = QString::fromStdString(service->layHinhAnh());
    if (!currentImagePath.isEmpty() && QFile::exists(currentImagePath))
    {
        QPixmap pix(currentImagePath);
        imagePreviewLabel->setPixmap(pix.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        imagePreviewLabel->setText("");
    }
    else
    {
        imagePreviewLabel->clear();
        imagePreviewLabel->setText("Chưa có ảnh");
    }

    saveBtn->setEnabled(true);
    deleteBtn->setEnabled(true);
}

void ServiceManagementWidget::clearForm()
{
    currentService = nullptr;
    isEditMode = false;
    currentImagePath.clear();

    codeEdit->clear();
    nameEdit->clear();
    priceEdit->clear();
    unitEdit->clear();
    stockEdit->clear();
    descriptionEdit->clear();
    statusEdit->setCurrentIndex(0); // Default Active
    categoryEdit->setCurrentIndex(0);

    imagePreviewLabel->clear();
    imagePreviewLabel->setText("Chưa có ảnh");

    saveBtn->setEnabled(false);
    deleteBtn->setEnabled(false);
}

void ServiceManagementWidget::onUploadImageClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Chọn ảnh dịch vụ", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (fileName.isEmpty())
        return;

    // Preview
    QPixmap pix(fileName);
    if (!pix.isNull())
    {
        imagePreviewLabel->setPixmap(pix.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        imagePreviewLabel->setText("");
        currentImagePath = fileName; // Store temp path, will copy on save
    }
}

void ServiceManagementWidget::onSaveClicked()
{
    if (!validateServiceData())
        return;

    QString name = nameEdit->text().trimmed();
    double price = priceEdit->text().toDouble();
    QString unit = unitEdit->text().trimmed();
    int stock = stockEdit->text().toInt();
    QString desc = descriptionEdit->toPlainText().trimmed();
    bool available = statusEdit->currentData().toBool();
    LoaiDichVu type = static_cast<LoaiDichVu>(categoryEdit->currentData().toInt());

    // Handle Image Copy
    QString finalImagePath = "";
    if (!currentImagePath.isEmpty())
    {
        QFileInfo fileInfo(currentImagePath);
        if (fileInfo.exists())
        {
            // Create Data/images directory if not exists
            QString dataDir = "Data/images";
            QDir dir;
            if (!dir.exists(dataDir))
                dir.mkpath(dataDir);

            QString newFileName = codeEdit->text() + "." + fileInfo.suffix();
            QString destPath = dataDir + "/" + newFileName;

            // If it's a new file or different from existing
            if (QFileInfo(currentImagePath).absoluteFilePath() != QFileInfo(destPath).absoluteFilePath())
            {
                if (QFile::exists(destPath))
                    QFile::remove(destPath);
                QFile::copy(currentImagePath, destPath);
            }
            finalImagePath = destPath;
        }
        else
        {
            // Keep old path if file not found (maybe already relative)
            finalImagePath = currentImagePath;
        }
    }

    if (isEditMode && currentService)
    {
        currentService->datTenDichVu(name.toStdString());
        currentService->datDonGia(price);
        currentService->datDonVi(unit.toStdString());
        currentService->datSoLuongTon(stock);
        currentService->datMoTa(desc.toStdString());
        currentService->datConHang(available);
        if (!finalImagePath.isEmpty())
            currentService->datHinhAnh(finalImagePath.toStdString());

        QMessageBox::information(this, "Thành công", "Cập nhật dịch vụ thành công!");
    }
    else
    {
        QString code = codeEdit->text();
        DichVu *newService = new DichVu(code.toStdString(), name.toStdString(), price, type);
        newService->datDonVi(unit.toStdString());
        newService->datSoLuongTon(stock);
        newService->datMoTa(desc.toStdString());
        newService->datConHang(available);
        newService->datSoLuongBan(0);
        if (!finalImagePath.isEmpty())
            newService->datHinhAnh(finalImagePath.toStdString());

        if (system->layQuanLyDichVu()->themDichVu(newService))
        {
            QMessageBox::information(this, "Thành công", "Thêm dịch vụ mới thành công!");
        }
        else
        {
            delete newService;
            QMessageBox::warning(this, "Lỗi", "Không thể thêm dịch vụ (Mã trùng?)");
            return;
        }
    }

    // Save System
    system->luuHeThong("Data/data.bin");
    system->luuDichVuCSV("Data/dichvu.csv");

    loadServices();
    clearForm();
}

void ServiceManagementWidget::onDeleteClicked()
{
    if (!currentService)
        return;

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Xác nhận",
                                                              "Bạn có chắc muốn xóa dịch vụ này?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        std::string id = currentService->layMaDichVu();
        if (system->layQuanLyDichVu()->xoaDichVu(id))
        {
            system->luuHeThong("Data/data.bin");
            system->luuDichVuCSV("Data/dichvu.csv");
            QMessageBox::information(this, "Thành công", "Đã xóa dịch vụ!");
            loadServices();
            clearForm();
        }
        else
        {
            QMessageBox::warning(this, "Lỗi", "Không thể xóa dịch vụ!");
        }
    }
}

bool ServiceManagementWidget::validateServiceData()
{
    if (nameEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Lỗi", "Tên dịch vụ không được để trống!");
        return false;
    }
    bool ok;
    double p = priceEdit->text().toDouble(&ok);
    if (!ok || p < 0)
    {
        QMessageBox::warning(this, "Lỗi", "Giá không hợp lệ!");
        return false;
    }
    return true;
}

QString ServiceManagementWidget::generateNextServiceCode()
{
    int maxId = 0;
    for (DichVu *dv : allServices)
    {
        QString code = QString::fromStdString(dv->layMaDichVu());
        if (code.startsWith("DV"))
        {
            int id = code.mid(2).toInt();
            if (id > maxId)
                maxId = id;
        }
    }
    return QString("DV%1").arg(maxId + 1, 3, 10, QChar('0'));
}
