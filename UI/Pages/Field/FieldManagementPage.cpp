/**
 * @file FieldManagementPage.cpp
 * @brief Implementation quản lý sân bóng với layout 60/40
 */

#include "FieldManagementPage.h"
#include "../../Dialogs/FieldDialog.h"
#include "../../Dialogs/MaintenanceDialog.h"
#include "../../Core/Models/San.h"
#include "../../Core/QuanLy/HeThongQuanLy.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QGridLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QEvent>
#include <QLocale>
#include <algorithm>
#include <QStyledItemDelegate>
#include <QPainter>

// Delegate to preserve text color when selected
class StatusDelegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        // If selected, force the text color to be the same as unselected (which is stored in ForegroundRole)
        if (opt.state & QStyle::State_Selected)
        {
            QVariant colorData = index.data(Qt::ForegroundRole);
            if (colorData.isValid())
            {
                opt.palette.setColor(QPalette::HighlightedText, colorData.value<QColor>());
            }
        }

        QStyledItemDelegate::paint(painter, opt, index);
    }
};

FieldManagementPage::FieldManagementPage(QWidget *parent)
    : QWidget(parent), quanLySan(nullptr), selectedFieldId(""), currentStatsFilter(FILTER_ALL)
{
    // Get QuanLySan from HeThongQuanLy
    HeThongQuanLy *sys = HeThongQuanLy::getInstance();
    quanLySan = sys->layQuanLySan();

    setupUI();
    setupConnections();
    applyStyles();
    loadData();
}

FieldManagementPage::~FieldManagementPage()
{
    // quanLySan được quản lý bởi HeThongQuanLy
}

void FieldManagementPage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // Create 70/30 splitter with gap between panels
    mainSplitter = new QSplitter(Qt::Horizontal, this);
    mainSplitter->setChildrenCollapsible(false);
    mainSplitter->setHandleWidth(8); // Small gap between panels
    mainSplitter->setStyleSheet("QSplitter::handle { background-color: #f9fafb; }");

    setupLeftPanel();
    setupRightPanel();

    mainSplitter->addWidget(leftPanel);
    mainSplitter->addWidget(rightPanel);

    // Set right panel fixed at 350px, disable resizing
    rightPanel->setMinimumWidth(350);
    rightPanel->setMaximumWidth(350);
    mainSplitter->setStretchFactor(0, 1); // Left panel stretches
    mainSplitter->setStretchFactor(1, 0); // Right panel fixed

    // Disable resizing after widgets are added
    if (mainSplitter->count() > 1)
    {
        mainSplitter->handle(1)->setEnabled(false);
    }

    mainLayout->addWidget(mainSplitter);
    setLayout(mainLayout);
}

void FieldManagementPage::setupLeftPanel()
{
    leftPanel = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(leftPanel);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(10);

    // ===== SEARCH BAR + FILTERS ON SAME ROW - Fill full width =====
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->setSpacing(12);

    // Search bar - longer to fill space
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("🔍 Tìm theo tên sân...");
    searchEdit->setFixedHeight(36);
    searchEdit->setMinimumWidth(400);

    // Loại sân filter - label and combo on same row
    QLabel *loaiSanLabel = new QLabel("Loại sân:");
    loaiSanLabel->setStyleSheet("color: #374151; font-size: 13px; font-weight: 500;");
    loaiSanLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    loaiSanCombo = new QComboBox();
    loaiSanCombo->addItem("Tất cả", -1);
    loaiSanCombo->addItem("Sân 5 người", static_cast<int>(LoaiSan::SAN_5));
    loaiSanCombo->addItem("Sân 7 người", static_cast<int>(LoaiSan::SAN_7));
    loaiSanCombo->setMinimumWidth(160);
    loaiSanCombo->setFixedHeight(36);

    // Khu vực filter - label and combo on same row
    QLabel *khuVucLabel = new QLabel("Khu vực:");
    khuVucLabel->setStyleSheet("color: #374151; font-size: 13px; font-weight: 500;");
    khuVucLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    khuVucCombo = new QComboBox();
    khuVucCombo->addItem("Tất cả", -1);
    khuVucCombo->addItem("Khu A", static_cast<int>(KhuVuc::A));
    khuVucCombo->addItem("Khu B", static_cast<int>(KhuVuc::B));
    khuVucCombo->addItem("Khu C", static_cast<int>(KhuVuc::C));
    khuVucCombo->addItem("Khu D", static_cast<int>(KhuVuc::D));
    khuVucCombo->setMinimumWidth(160);
    khuVucCombo->setFixedHeight(36);

    searchLayout->addWidget(searchEdit, 1); // Stretch to fill
    searchLayout->addWidget(loaiSanLabel);
    searchLayout->addWidget(loaiSanCombo);
    searchLayout->addWidget(khuVucLabel);
    searchLayout->addWidget(khuVucCombo);
    searchLayout->addStretch();

    layout->addLayout(searchLayout);
    layout->addSpacing(15); // Add spacing between filters and stats cards

    // ===== 4 STATS CARDS =====
    QFrame *statsContainer = new QFrame();
    statsContainer->setObjectName("statsContainer");
    QHBoxLayout *statsLayout = new QHBoxLayout(statsContainer);
    statsLayout->setSpacing(15);
    statsLayout->setContentsMargins(0, 0, 0, 0);

    // Card 1: Tổng số sân
    totalCard = new QFrame();
    totalCard->setObjectName("statCard");
    totalCard->setFocusPolicy(Qt::NoFocus);
    QVBoxLayout *totalCardLayout = new QVBoxLayout(totalCard);
    QLabel *totalTitle = new QLabel("Tổng số sân");
    totalTitle->setObjectName("statCardTitle");
    totalTitle->setFocusPolicy(Qt::NoFocus);
    totalTitle->setStyleSheet("outline: none; border: none;");
    totalLabel = new QLabel("0");
    totalLabel->setObjectName("statCardValue");
    totalLabel->setFocusPolicy(Qt::NoFocus);
    totalLabel->setStyleSheet("outline: none; border: none; color: #000000;");
    totalCardLayout->addWidget(totalTitle);
    totalCardLayout->addWidget(totalLabel);
    statsLayout->addWidget(totalCard);

    // Card 2: Đang hoạt động
    activeCard = new QFrame();
    activeCard->setObjectName("statCard");
    activeCard->setFocusPolicy(Qt::NoFocus);
    QVBoxLayout *activeCardLayout = new QVBoxLayout(activeCard);
    QLabel *activeTitle = new QLabel("Đang hoạt động");
    activeTitle->setObjectName("statCardTitle");
    activeTitle->setFocusPolicy(Qt::NoFocus);
    activeTitle->setStyleSheet("outline: none; border: none;");
    activeLabel = new QLabel("0");
    activeLabel->setObjectName("statCardValue");
    activeLabel->setFocusPolicy(Qt::NoFocus);
    activeLabel->setStyleSheet("outline: none; border: none; color: #4CAF50;");
    activeCardLayout->addWidget(activeTitle);
    activeCardLayout->addWidget(activeLabel);
    statsLayout->addWidget(activeCard);

    // Card 3: Đang bảo trì
    maintenanceCard = new QFrame();
    maintenanceCard->setObjectName("statCard");
    maintenanceCard->setFocusPolicy(Qt::NoFocus);
    QVBoxLayout *maintenanceCardLayout = new QVBoxLayout(maintenanceCard);
    QLabel *maintenanceTitle = new QLabel("Đang bảo trì");
    maintenanceTitle->setObjectName("statCardTitle");
    maintenanceTitle->setFocusPolicy(Qt::NoFocus);
    maintenanceTitle->setStyleSheet("outline: none; border: none;");
    maintenanceLabel = new QLabel("0");
    maintenanceLabel->setObjectName("statCardValue");
    maintenanceLabel->setFocusPolicy(Qt::NoFocus);
    maintenanceLabel->setStyleSheet("outline: none; border: none; color: #FF9800;");
    maintenanceCardLayout->addWidget(maintenanceTitle);
    maintenanceCardLayout->addWidget(maintenanceLabel);
    statsLayout->addWidget(maintenanceCard);

    // Card 4: Ngừng hoạt động
    suspendedCard = new QFrame();
    suspendedCard->setObjectName("statCard");
    suspendedCard->setFocusPolicy(Qt::NoFocus);
    QVBoxLayout *suspendedCardLayout = new QVBoxLayout(suspendedCard);
    QLabel *suspendedTitle = new QLabel("Ngừng hoạt động");
    suspendedTitle->setObjectName("statCardTitle");
    suspendedTitle->setFocusPolicy(Qt::NoFocus);
    suspendedTitle->setStyleSheet("outline: none; border: none;");
    suspendedLabel = new QLabel("0");
    suspendedLabel->setObjectName("statCardValue");
    suspendedLabel->setFocusPolicy(Qt::NoFocus);
    suspendedLabel->setStyleSheet("outline: none; border: none; color: #F44336;");
    suspendedCardLayout->addWidget(suspendedTitle);
    suspendedCardLayout->addWidget(suspendedLabel);
    statsLayout->addWidget(suspendedCard);

    layout->addWidget(statsContainer);

    // ===== DATA TABLE (7 columns) - Proportional widths that resize with sidebar =====
    dataTable = new QTableWidget(0, 7);
    dataTable->setHorizontalHeaderLabels({"Mã sân", "Tên sân", "Loại sân", "Khu vực", "Giá thuê", "Trạng thái", "Ghi chú"});
    dataTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    dataTable->setSelectionMode(QAbstractItemView::SingleSelection);
    dataTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Proportional column widths - resize when sidebar opens/closes
    dataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    dataTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive); // Mã sân
    dataTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive); // Tên sân
    dataTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive); // Loại sân
    dataTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive); // Khu vực
    dataTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Interactive); // Giá thuê
    dataTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Interactive); // Trạng thái
    dataTable->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);     // Ghi chú - stretches

    // Set initial widths (will scale proportionally)
    dataTable->setColumnWidth(0, 95);  // Mã sân
    dataTable->setColumnWidth(1, 110); // Tên sân
    dataTable->setColumnWidth(2, 105); // Loại sân
    dataTable->setColumnWidth(3, 90);  // Khu vực
    dataTable->setColumnWidth(4, 140); // Giá thuê
    dataTable->setColumnWidth(5, 150); // Trạng thái
    // Ghi chú - auto fills remaining space

    // Set header text alignment
    dataTable->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignCenter);                  // Mã sân
    dataTable->horizontalHeaderItem(1)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter); // Tên sân
    dataTable->horizontalHeaderItem(2)->setTextAlignment(Qt::AlignCenter);                  // Loại sân
    dataTable->horizontalHeaderItem(3)->setTextAlignment(Qt::AlignCenter);                  // Khu vực
    dataTable->horizontalHeaderItem(4)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter); // Giá thuê
    dataTable->horizontalHeaderItem(5)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter); // Trạng thái
    dataTable->horizontalHeaderItem(6)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter); // Ghi chú

    // Set custom delegate for Status column (index 5) to preserve text color on selection
    dataTable->setItemDelegateForColumn(5, new StatusDelegate(dataTable));

    layout->addWidget(dataTable);
}

void FieldManagementPage::setupRightPanel()
{
    rightPanel = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(rightPanel);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(15);

    // Title
    QLabel *titleLabel = new QLabel("Chi tiết sân bóng");
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    layout->addWidget(titleLabel);

    // Form - labels aligned left, inputs aligned right with consistent height and width
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(14);
    formLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    formLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);

    // Tên sân
    tenSanEdit = new QLineEdit();
    tenSanEdit->setReadOnly(true);
    tenSanEdit->setMinimumHeight(40);
    tenSanEdit->setMinimumWidth(200);
    tenSanEdit->setStyleSheet("QLineEdit { border-radius: 6px; padding: 8px; }");
    formLayout->addRow("Tên sân:", tenSanEdit);

    // Loại sân
    loaiSanDetailCombo = new QComboBox();
    loaiSanDetailCombo->addItem("Sân 5 người", static_cast<int>(LoaiSan::SAN_5));
    loaiSanDetailCombo->addItem("Sân 7 người", static_cast<int>(LoaiSan::SAN_7));
    loaiSanDetailCombo->setEnabled(false);
    loaiSanDetailCombo->setMinimumHeight(40);
    loaiSanDetailCombo->setMinimumWidth(200);
    loaiSanDetailCombo->setStyleSheet("QComboBox { border-radius: 6px; padding: 8px; }");
    formLayout->addRow("Loại sân:", loaiSanDetailCombo);

    // Khu vực
    khuVucDetailCombo = new QComboBox();
    khuVucDetailCombo->addItem("Khu A", static_cast<int>(KhuVuc::A));
    khuVucDetailCombo->addItem("Khu B", static_cast<int>(KhuVuc::B));
    khuVucDetailCombo->addItem("Khu C", static_cast<int>(KhuVuc::C));
    khuVucDetailCombo->addItem("Khu D", static_cast<int>(KhuVuc::D));
    khuVucDetailCombo->setEnabled(false);
    khuVucDetailCombo->setMinimumHeight(40);
    khuVucDetailCombo->setMinimumWidth(200);
    khuVucDetailCombo->setStyleSheet("QComboBox { border-radius: 6px; padding: 8px; }");
    formLayout->addRow("Khu vực:", khuVucDetailCombo);

    // Trạng thái
    trangThaiCombo = new QComboBox();
    trangThaiCombo->addItem("Hoạt động", static_cast<int>(TrangThaiSan::HOAT_DONG));
    trangThaiCombo->addItem("Ngừng hoạt động", static_cast<int>(TrangThaiSan::NGUNG_HOAT_DONG));
    trangThaiCombo->setEnabled(false);
    trangThaiCombo->setMinimumHeight(40);
    trangThaiCombo->setMinimumWidth(200);
    trangThaiCombo->setStyleSheet("QComboBox { border-radius: 6px; padding: 8px; }");
    formLayout->addRow("Trạng thái:", trangThaiCombo);

    // Giá thuê
    giaThueSpinBox = new QSpinBox();
    giaThueSpinBox->setRange(0, 10000000);
    giaThueSpinBox->setSingleStep(50000);
    giaThueSpinBox->setSuffix("đ/60p");
    giaThueSpinBox->setReadOnly(true);
    giaThueSpinBox->setGroupSeparatorShown(true);
    giaThueSpinBox->setMinimumHeight(40);
    giaThueSpinBox->setMinimumWidth(200);
    giaThueSpinBox->setStyleSheet("QSpinBox { border-radius: 6px; padding: 8px; }");
    formLayout->addRow("Giá thuê:", giaThueSpinBox);

    // Ghi chú - larger text area with visible styling
    ghiChuEdit = new QTextEdit();
    ghiChuEdit->setReadOnly(true);
    ghiChuEdit->setMinimumHeight(120);
    ghiChuEdit->setMaximumHeight(180);
    ghiChuEdit->setMinimumWidth(200);
    ghiChuEdit->setStyleSheet("QTextEdit { border: 1px solid #d1d5db; border-radius: 6px; padding: 8px; background-color: white; }");
    formLayout->addRow("Ghi chú:", ghiChuEdit);

    layout->addLayout(formLayout);

    layout->addStretch();

    // Action buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);

    addNewBtn = new QPushButton("➕ Thêm sân mới");
    saveBtn = new QPushButton("💾 Lưu");
    maintenanceBtn = new QPushButton("⚙️ Bảo trì");
    deleteBtn = new QPushButton("🗑️ Xoá");

    saveBtn->setEnabled(false);
    maintenanceBtn->setEnabled(false);
    deleteBtn->setEnabled(false);

    btnLayout->addWidget(addNewBtn);
    btnLayout->addWidget(saveBtn);
    layout->addLayout(btnLayout);

    QHBoxLayout *btnLayout2 = new QHBoxLayout();
    btnLayout2->addWidget(maintenanceBtn);
    btnLayout2->addWidget(deleteBtn);
    layout->addLayout(btnLayout2);
}

void FieldManagementPage::setupConnections()
{
    // Left panel
    connect(searchEdit, &QLineEdit::textChanged, this, &FieldManagementPage::onSearchTextChanged);
    connect(loaiSanCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FieldManagementPage::onFilterChanged);
    connect(khuVucCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FieldManagementPage::onFilterChanged);
    connect(dataTable, &QTableWidget::cellClicked, this, &FieldManagementPage::onTableRowClicked);

    // Stats cards - use mousePressEvent would be better, but lambda is simpler
    totalCard->installEventFilter(this);
    activeCard->installEventFilter(this);
    maintenanceCard->installEventFilter(this);
    suspendedCard->installEventFilter(this);

    // Right panel
    connect(addNewBtn, &QPushButton::clicked, this, &FieldManagementPage::onAddNewFieldClicked);
    connect(saveBtn, &QPushButton::clicked, this, &FieldManagementPage::onSaveFieldClicked);
    connect(maintenanceBtn, &QPushButton::clicked, this, &FieldManagementPage::onMaintenanceFieldClicked);
    connect(deleteBtn, &QPushButton::clicked, this, &FieldManagementPage::onDeleteFieldClicked);

    // Auto-generate name when combos change (only in Add New mode)
    connect(loaiSanDetailCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FieldManagementPage::generateFieldName);
    connect(khuVucDetailCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FieldManagementPage::generateFieldName);
}

bool FieldManagementPage::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        if (watched == totalCard)
        {
            currentStatsFilter = FILTER_ALL;
            loadTableData();
            return true;
        }
        else if (watched == activeCard)
        {
            currentStatsFilter = FILTER_ACTIVE;
            loadTableData();
            return true;
        }
        else if (watched == maintenanceCard)
        {
            currentStatsFilter = FILTER_MAINTENANCE;
            loadTableData();
            return true;
        }
        else if (watched == suspendedCard)
        {
            currentStatsFilter = FILTER_SUSPENDED;
            loadTableData();
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void FieldManagementPage::applyStyles()
{
    // Main background - very light gray
    setStyleSheet("FieldManagementPage { background-color: #f9fafb; }");

    // Stats cards styling - simple white cards on light gray, no focus border
    QString cardStyle = R"(
        QFrame {
            background-color: white;
            border: 1px solid #e5e7eb;
            border-radius: 10px;
            outline: none;
        }
        QFrame:hover {
            border: 1px solid #3b82f6;
        }
    )";

    totalCard->setStyleSheet(cardStyle);
    activeCard->setStyleSheet(cardStyle);
    maintenanceCard->setStyleSheet(cardStyle);
    suspendedCard->setStyleSheet(cardStyle);

    // Remove focus policy from cards
    totalCard->setFocusPolicy(Qt::NoFocus);
    activeCard->setFocusPolicy(Qt::NoFocus);
    maintenanceCard->setFocusPolicy(Qt::NoFocus);
    suspendedCard->setFocusPolicy(Qt::NoFocus);

    // Right panel - white background with rounded corners
    rightPanel->setStyleSheet(R"(
        QWidget {
            background-color: white;
            border-radius: 12px;
        }
    )");

    // Button styling
    addNewBtn->setStyleSheet(R"(
        QPushButton { 
            background-color: #10b981; 
            color: white; 
            padding: 10px 20px; 
            border-radius: 6px; 
            font-weight: bold;
            border: none;
        }
        QPushButton:hover {
            background-color: #059669;
        }
    )");

    saveBtn->setStyleSheet(R"(
        QPushButton { 
            background-color: #3b82f6; 
            color: white; 
            padding: 10px 20px; 
            border-radius: 6px; 
            font-weight: bold;
            border: none;
        }
        QPushButton:hover {
            background-color: #2563eb;
        }
        QPushButton:disabled {
            background-color: #d1d5db;
        }
    )");

    maintenanceBtn->setStyleSheet(R"(
        QPushButton { 
            background-color: #f59e0b; 
            color: white; 
            padding: 10px 20px; 
            border-radius: 6px; 
            font-weight: bold;
            border: none;
        }
        QPushButton:hover {
            background-color: #d97706;
        }
        QPushButton:disabled {
            background-color: #d1d5db;
        }
    )");

    deleteBtn->setStyleSheet(R"(
        QPushButton { 
            background-color: #ef4444; 
            color: white; 
            padding: 10px 20px; 
            border-radius: 6px; 
            font-weight: bold;
            border: none;
        }
        QPushButton:hover {
            background-color: #dc2626;
        }
        QPushButton:disabled {
            background-color: #d1d5db;
        }
    )");

    // Table styling - consistent horizontal lines, no vertical gridlines
    dataTable->setStyleSheet(R"(
        QTableWidget {
            background-color: white;
            border: 1px solid #e5e7eb;
            border-radius: 8px;
            gridline-color: transparent;
            outline: none;
        }
        QTableWidget::item {
            padding: 10px 8px;
            border: none;
            border-bottom: 1px solid #e5e7eb;
            outline: none;
        }
        QTableWidget::item:selected {
            background-color: #d1fae5;
            outline: none;
            border-bottom: 1px solid #e5e7eb;
        }
        QTableWidget::item:focus {
            background-color: #d1fae5;
            outline: none;
            border: none;
            border-bottom: 1px solid #e5e7eb;
        }
        QHeaderView::section {
            background-color: #f9fafb;
            padding: 10px 8px;
            border: none;
            border-bottom: 2px solid #d1d5db;
            font-weight: bold;
            color: #374151;
        }
    )");

    dataTable->setShowGrid(false); // Disable grid completely
    dataTable->verticalHeader()->setVisible(false);
    dataTable->setFocusPolicy(Qt::NoFocus); // Remove focus outline

    // Set custom delegate for status column to preserve text color on selection
    dataTable->setItemDelegateForColumn(5, new StatusDelegate(this));
}

void FieldManagementPage::loadData()
{
    if (!quanLySan)
    {
        return;
    }

    updateStatsCards();
    loadTableData();
}

void FieldManagementPage::updateStatsCards()
{
    if (!quanLySan)
    {
        return;
    }

    int total = quanLySan->tongSoSan();
    int active = quanLySan->demSanHoatDong();
    int maintenance = quanLySan->demSanBaoTri();
    int suspended = quanLySan->demSanNgungHoatDong();

    totalLabel->setText(QString::number(total));
    activeLabel->setText(QString::number(active));
    maintenanceLabel->setText(QString::number(maintenance));
    suspendedLabel->setText(QString::number(suspended));
}

void FieldManagementPage::loadTableData()
{
    std::vector<San *> fields = getFilteredFields();
    populateTable(fields);
}

std::vector<San *> FieldManagementPage::getFilteredFields()
{
    if (!quanLySan)
    {
        return {};
    }

    // Get all fields or filtered by stats card
    std::vector<San *> fields;

    if (currentStatsFilter == FILTER_ACTIVE)
    {
        MangDong<San *> temp = quanLySan->laySanDangHoatDong();
        for (int i = 0; i < temp.size(); i++)
        {
            fields.push_back(temp[i]);
        }
    }
    else if (currentStatsFilter == FILTER_MAINTENANCE)
    {
        MangDong<San *> temp = quanLySan->laySanBaoTri();
        for (int i = 0; i < temp.size(); i++)
        {
            fields.push_back(temp[i]);
        }
    }
    else if (currentStatsFilter == FILTER_SUSPENDED)
    {
        MangDong<San *> temp = quanLySan->laySanNgungHoatDong();
        for (int i = 0; i < temp.size(); i++)
        {
            fields.push_back(temp[i]);
        }
    }
    else
    {
        // FILTER_ALL
        MangDong<San *> temp = quanLySan->layDanhSachSan();
        for (int i = 0; i < temp.size(); i++)
        {
            fields.push_back(temp[i]);
        }
    }

    // Apply loại sân filter
    int loaiSanIndex = loaiSanCombo->currentData().toInt();
    if (loaiSanIndex != -1)
    {
        LoaiSan loaiSan = static_cast<LoaiSan>(loaiSanIndex);
        fields.erase(std::remove_if(fields.begin(), fields.end(),
                                    [loaiSan](San *s)
                                    { return s->layLoaiSan() != loaiSan; }),
                     fields.end());
    }

    // Apply khu vực filter
    int khuVucIndex = khuVucCombo->currentData().toInt();
    if (khuVucIndex != -1)
    {
        KhuVuc khuVuc = static_cast<KhuVuc>(khuVucIndex);
        fields.erase(std::remove_if(fields.begin(), fields.end(),
                                    [khuVuc](San *s)
                                    { return s->layKhuVuc() != khuVuc; }),
                     fields.end());
    }

    // Apply search text
    QString searchText = searchEdit->text().trimmed().toLower();
    if (!searchText.isEmpty())
    {
        fields.erase(std::remove_if(fields.begin(), fields.end(),
                                    [searchText](San *s)
                                    {
                                        QString tenSan = QString::fromStdString(s->layTenSan()).toLower();
                                        return !tenSan.contains(searchText);
                                    }),
                     fields.end());
    }

    return fields;
}

void FieldManagementPage::populateTable(const std::vector<San *> &fields)
{
    dataTable->setRowCount(0);

    for (size_t i = 0; i < fields.size(); i++)
    {
        San *san = fields[i];
        int row = dataTable->rowCount();
        dataTable->insertRow(row);

        // Mã sân (center)
        QTableWidgetItem *maSanItem = new QTableWidgetItem(QString::fromStdString(san->layMaSan()));
        maSanItem->setTextAlignment(Qt::AlignCenter);
        dataTable->setItem(row, 0, maSanItem);

        // Tên sân (left)
        QTableWidgetItem *tenSanItem = new QTableWidgetItem(QString::fromStdString(san->layTenSan()));
        tenSanItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        dataTable->setItem(row, 1, tenSanItem);

        // Loại sân (center)
        QTableWidgetItem *loaiSanItem = new QTableWidgetItem(QString::fromStdString(san->layTenLoaiSan()));
        loaiSanItem->setTextAlignment(Qt::AlignCenter);
        dataTable->setItem(row, 2, loaiSanItem);

        // Khu vực (center)
        QTableWidgetItem *khuVucItem = new QTableWidgetItem(QString::fromStdString(san->layTenKhuVuc()));
        khuVucItem->setTextAlignment(Qt::AlignCenter);
        dataTable->setItem(row, 3, khuVucItem);

        // Giá thuê (left) - Format with comma separator and đ/60p
        int giaThue = san->layGiaThueGio();
        // Format number with comma separator (e.g., 200,000)
        QString giaThueStr = QString("%L1đ/60p").arg(giaThue);
        giaThueStr.replace(".", ","); // Replace dot with comma for Vietnamese style
        QTableWidgetItem *giaThueItem = new QTableWidgetItem(giaThueStr);
        giaThueItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        dataTable->setItem(row, 4, giaThueItem);

        // Trạng thái (left) - BOLD with color preservation
        QTableWidgetItem *trangThaiItem = new QTableWidgetItem(QString::fromStdString(san->layTenTrangThai()));
        trangThaiItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        QFont trangThaiBoldFont = trangThaiItem->font();
        trangThaiBoldFont.setBold(true);
        trangThaiItem->setFont(trangThaiBoldFont);

        // Color coding - set as data role to preserve on selection
        QColor statusColor;
        if (san->dangHoatDong())
        {
            statusColor = QColor("#4CAF50");
        }
        else if (san->dangBaoTri())
        {
            statusColor = QColor("#FF9800");
        }
        else
        {
            statusColor = QColor("#F44336");
        }
        trangThaiItem->setData(Qt::ForegroundRole, statusColor); // Use ForegroundRole for proper color preservation
        trangThaiItem->setData(Qt::UserRole, statusColor);       // Also store for backup
        dataTable->setItem(row, 5, trangThaiItem);

        // Ghi chú (left)
        QTableWidgetItem *ghiChuItem = new QTableWidgetItem(QString::fromStdString(san->layGhiChu()));
        ghiChuItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        dataTable->setItem(row, 6, ghiChuItem);
    }
}

void FieldManagementPage::onSearchTextChanged()
{
    loadTableData();
}

void FieldManagementPage::onFilterChanged()
{
    loadTableData();
}

void FieldManagementPage::onTableRowClicked(int row, int column)
{
    Q_UNUSED(column);

    if (row < 0 || row >= dataTable->rowCount())
    {
        return;
    }

    selectedFieldId = dataTable->item(row, 0)->text();
    San *san = getSelectedField();

    if (san)
    {
        loadFieldToForm(san);
        setFormMode(false, true); // Enable editing when selecting field
        maintenanceBtn->setEnabled(true);
        deleteBtn->setEnabled(true);
    }
}

void FieldManagementPage::onStatsCardClicked()
{
    // This is handled by eventFilter now
}

void FieldManagementPage::onAddNewFieldClicked()
{
    clearForm();
    setFormMode(true, true); // New mode
    selectedFieldId = "";

    // Trigger auto-generation for initial default values
    generateFieldName();
}

void FieldManagementPage::onSaveFieldClicked()
{
    if (!quanLySan)
    {
        return;
    }

    QString tenSan = tenSanEdit->text().trimmed();
    if (tenSan.isEmpty())
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập tên sân!");
        return;
    }

    LoaiSan loaiSan = static_cast<LoaiSan>(loaiSanDetailCombo->currentData().toInt());
    KhuVuc khuVuc = static_cast<KhuVuc>(khuVucDetailCombo->currentData().toInt());
    TrangThaiSan trangThai = static_cast<TrangThaiSan>(trangThaiCombo->currentData().toInt());
    int giaThue = giaThueSpinBox->value();
    QString ghiChu = ghiChuEdit->toPlainText().trimmed();

    if (selectedFieldId.isEmpty())
    {
        // Add new
        std::string maSan = quanLySan->taoMaSanTuDong(khuVuc, loaiSan);
        San *newSan = new San(maSan, tenSan.toStdString(), loaiSan, khuVuc, giaThue);
        newSan->datTrangThai(trangThai);
        newSan->datGhiChu(ghiChu.toStdString());

        if (quanLySan->themSan(newSan))
        {
            quanLySan->saveToCSV();
            QMessageBox::information(this, "Thành công", "Thêm sân mới thành công!");
            loadData();
            clearForm();
            setFormMode(false, false);
            emit dataChanged();
        }
        else
        {
            QMessageBox::warning(this, "Lỗi", "Không thể thêm sân!");
            delete newSan;
        }
    }
    else
    {
        // Update existing
        San sanMoi(selectedFieldId.toStdString(), tenSan.toStdString(), loaiSan, khuVuc, giaThue);
        sanMoi.datTrangThai(trangThai);
        sanMoi.datGhiChu(ghiChu.toStdString());

        if (quanLySan->capNhatSan(selectedFieldId.toStdString(), sanMoi))
        {
            quanLySan->saveToCSV();
            QMessageBox::information(this, "Thành công", "Cập nhật sân thành công!");
            loadData();
            setFormMode(false, false);
            emit dataChanged();
        }
        else
        {
            QMessageBox::warning(this, "Lỗi", "Không thể cập nhật sân!");
        }
    }
}

void FieldManagementPage::onMaintenanceFieldClicked()
{
    if (selectedFieldId.isEmpty())
    {
        return;
    }

    San *san = getSelectedField();
    if (!san)
    {
        return;
    }

    // Check if field is already in maintenance
    if (san->dangBaoTri())
    {
        QMessageBox::information(this, "Thông báo",
                                 "Sân này đã ở trạng thái bảo trì!");
        return;
    }

    // Open maintenance dialog
    MaintenanceDialog dialog(san, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString reason = dialog.getMaintenanceReason();

        // Update field status to maintenance
        san->datTrangThai(TrangThaiSan::BAO_TRI);
        san->datGhiChu(reason.toStdString());

        // Update in QuanLySan and save to CSV
        San sanCapNhat(san->layMaSan(), san->layTenSan(), san->layLoaiSan(),
                       san->layKhuVuc(), san->layGiaThueGio());
        sanCapNhat.datTrangThai(TrangThaiSan::BAO_TRI);
        sanCapNhat.datGhiChu(reason.toStdString());

        if (quanLySan->capNhatSan(selectedFieldId.toStdString(), sanCapNhat))
        {
            quanLySan->saveToCSV();
            QMessageBox::information(this, "Thành công",
                                     QString("Đã đưa sân %1 vào bảo trì!\nLý do: %2")
                                         .arg(QString::fromStdString(san->layTenSan()))
                                         .arg(reason));
            loadData();
            loadFieldToForm(getSelectedField());
            emit dataChanged();
        }
        else
        {
            QMessageBox::warning(this, "Lỗi", "Không thể cập nhật trạng thái sân!");
        }
    }
}

void FieldManagementPage::onDeleteFieldClicked()
{
    if (selectedFieldId.isEmpty())
    {
        return;
    }

    San *san = getSelectedField();
    if (!san)
    {
        return;
    }

    auto reply = QMessageBox::question(this, "Xác nhận xoá",
                                       QString("Bạn có chắc muốn xoá sân %1?").arg(QString::fromStdString(san->layTenSan())),
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (quanLySan->xoaSan(selectedFieldId.toStdString()))
        {
            quanLySan->saveToCSV();
            QMessageBox::information(this, "Thành công", "Xoá sân thành công!");
            clearForm();
            setFormMode(false, false);
            loadData();
            emit dataChanged();
        }
        else
        {
            QMessageBox::warning(this, "Lỗi", "Không thể xoá sân!");
        }
    }
}

void FieldManagementPage::setFormMode(bool isNew, bool isEditing)
{
    tenSanEdit->setReadOnly(!isEditing);
    loaiSanDetailCombo->setEnabled(isEditing); // Enable when editing
    khuVucDetailCombo->setEnabled(isEditing);  // Enable when editing
    trangThaiCombo->setEnabled(isEditing);
    giaThueSpinBox->setReadOnly(!isEditing);
    ghiChuEdit->setReadOnly(!isEditing);

    saveBtn->setEnabled(isEditing);
    maintenanceBtn->setEnabled(!isNew && !isEditing);
    deleteBtn->setEnabled(!isNew && !isEditing);
}

void FieldManagementPage::clearForm()
{
    tenSanEdit->clear();
    loaiSanDetailCombo->setCurrentIndex(0);
    khuVucDetailCombo->setCurrentIndex(0);
    trangThaiCombo->setCurrentIndex(0);
    giaThueSpinBox->setValue(0);
    ghiChuEdit->clear();
    selectedFieldId = "";
}

void FieldManagementPage::loadFieldToForm(San *san)
{
    if (!san)
    {
        return;
    }

    tenSanEdit->setText(QString::fromStdString(san->layTenSan()));

    // Set loại sân
    for (int i = 0; i < loaiSanDetailCombo->count(); i++)
    {
        if (loaiSanDetailCombo->itemData(i).toInt() == static_cast<int>(san->layLoaiSan()))
        {
            loaiSanDetailCombo->setCurrentIndex(i);
            break;
        }
    }

    // Set khu vực
    for (int i = 0; i < khuVucDetailCombo->count(); i++)
    {
        if (khuVucDetailCombo->itemData(i).toInt() == static_cast<int>(san->layKhuVuc()))
        {
            khuVucDetailCombo->setCurrentIndex(i);
            break;
        }
    }

    // Set trạng thái
    for (int i = 0; i < trangThaiCombo->count(); i++)
    {
        if (trangThaiCombo->itemData(i).toInt() == static_cast<int>(san->layTrangThai()))
        {
            trangThaiCombo->setCurrentIndex(i);
            break;
        }
    }

    giaThueSpinBox->setValue(san->layGiaThueGio());
    ghiChuEdit->setPlainText(QString::fromStdString(san->layGhiChu()));
}

San *FieldManagementPage::getSelectedField()
{
    if (!quanLySan || selectedFieldId.isEmpty())
    {
        return nullptr;
    }

    return quanLySan->timSan(selectedFieldId.toStdString());
}

void FieldManagementPage::generateFieldName()
{
    // Only generate if adding new field (selectedFieldId is empty)
    if (!selectedFieldId.isEmpty() || !quanLySan)
    {
        return;
    }

    // Get selected Area and Type
    int loaiSanIndex = loaiSanDetailCombo->currentData().toInt();
    int khuVucIndex = khuVucDetailCombo->currentData().toInt();

    LoaiSan loaiSan = static_cast<LoaiSan>(loaiSanIndex);
    KhuVuc khuVuc = static_cast<KhuVuc>(khuVucIndex);

    // Determine characters
    QString areaChar;
    switch (khuVuc)
    {
    case KhuVuc::A:
        areaChar = "A";
        break;
    case KhuVuc::B:
        areaChar = "B";
        break;
    case KhuVuc::C:
        areaChar = "C";
        break;
    case KhuVuc::D:
        areaChar = "D";
        break;
    default:
        areaChar = "A";
        break;
    }

    QString typeChar = (loaiSan == LoaiSan::SAN_5) ? "5" : "7";

    // Prefix: "Sân A5-"
    QString prefix = QString("Sân %1%2-").arg(areaChar, typeChar);

    // Find max number for this prefix
    int maxNum = 0;
    MangDong<San *> allFields = quanLySan->layDanhSachSan();

    for (int i = 0; i < allFields.size(); i++)
    {
        QString name = QString::fromStdString(allFields[i]->layTenSan());
        if (name.startsWith(prefix))
        {
            QString numPart = name.mid(prefix.length());
            bool ok;
            int num = numPart.toInt(&ok);
            if (ok && num > maxNum)
            {
                maxNum = num;
            }
        }
    }

    // Generate next number
    int nextNum = maxNum + 1;
    QString nextNumStr = QString("%1").arg(nextNum, 2, 10, QChar('0')); // Pad with 0 (e.g., 01, 02)

    // Set text
    tenSanEdit->setText(prefix + nextNumStr);
}
