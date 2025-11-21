/**
 * @file CustomerManagementPage.cpp
 * @brief Implementation of Customer Management Page
 * @date 2025-11-11
 */

#include "CustomerManagementPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QMessageBox>
#include <QHeaderView>
#include <QDateTime>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <sstream>
#include <iomanip>

CustomerManagementPage::CustomerManagementPage(QWidget *parent)
    : QWidget(parent),
      system(HeThongQuanLy::getInstance()),
      currentCustomer(nullptr),
      isEditMode(false)
{
    setupUI();
    setupConnections();
    applyStyles();
    loadCustomers();
}

CustomerManagementPage::~CustomerManagementPage()
{
}

void CustomerManagementPage::setupUI()
{
    // Main horizontal layout
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    setupLeftPanel();
    setupRightPanel();

    setLayout(mainLayout);
}

void CustomerManagementPage::setupLeftPanel()
{
    // Left container (70%)
    QWidget *leftWidget = new QWidget(this);
    leftWidget->setMinimumWidth(400); // Minimum width for left panel
    leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setSpacing(15);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    // ===== SEARCH & FILTERS ROW =====
    QHBoxLayout *searchFilterLayout = new QHBoxLayout();
    searchFilterLayout->setSpacing(10);

    // Search box
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("🔍 Tìm theo tên hoặc số điện thoại...");
    searchEdit->setObjectName("searchEdit");
    searchEdit->setMinimumHeight(40);
    searchFilterLayout->addWidget(searchEdit, 3);

    // Tier filter
    tierFilterCombo = new QComboBox(this);
    tierFilterCombo->setObjectName("filterCombo");
    tierFilterCombo->addItem("Tất cả hạng", "ALL");
    tierFilterCombo->addItem("Thường", "THUONG");
    tierFilterCombo->addItem("Đồng", "DONG");
    tierFilterCombo->addItem("Bạc", "BAC");
    tierFilterCombo->addItem("Vàng", "VANG");
    tierFilterCombo->addItem("Kim cương", "KIM_CUONG");
    tierFilterCombo->setMinimumHeight(40);
    searchFilterLayout->addWidget(tierFilterCombo, 1);

    // Spending filter
    spendingFilterCombo = new QComboBox(this);
    spendingFilterCombo->setObjectName("filterCombo");
    spendingFilterCombo->addItem("Tất cả chi tiêu", "ALL");
    spendingFilterCombo->addItem("< 1 triệu", "LT_1M");
    spendingFilterCombo->addItem("1-5 triệu", "1M_5M");
    spendingFilterCombo->addItem("5-10 triệu", "5M_10M");
    spendingFilterCombo->addItem("10-20 triệu", "10M_20M");
    spendingFilterCombo->addItem(">= 20 triệu", "GTE_20M");
    spendingFilterCombo->setMinimumHeight(40);
    searchFilterLayout->addWidget(spendingFilterCombo, 1);

    leftLayout->addLayout(searchFilterLayout);

    // ===== STATS CARDS =====
    statsContainer = new QFrame(this);
    statsContainer->setObjectName("statsContainer");
    QHBoxLayout *statsLayout = new QHBoxLayout(statsContainer);
    statsLayout->setSpacing(15);
    statsLayout->setContentsMargins(0, 0, 0, 0);

    // Card 1: Total customers
    QFrame *card1 = new QFrame(this);
    card1->setObjectName("statCard");
    QVBoxLayout *card1Layout = new QVBoxLayout(card1);
    QLabel *card1Title = new QLabel("Tổng khách hàng", this);
    card1Title->setObjectName("statCardTitle");
    totalCustomersLabel = new QLabel("0", this);
    totalCustomersLabel->setObjectName("statCardValue");
    card1Layout->addWidget(card1Title);
    card1Layout->addWidget(totalCustomersLabel);
    statsLayout->addWidget(card1);

    // Card 2: New customers (this month)
    QFrame *card2 = new QFrame(this);
    card2->setObjectName("statCard");
    QVBoxLayout *card2Layout = new QVBoxLayout(card2);
    QLabel *card2Title = new QLabel("Khách hàng mới (tháng này)", this);
    card2Title->setObjectName("statCardTitle");
    newCustomersLabel = new QLabel("0", this);
    newCustomersLabel->setObjectName("statCardValue");
    card2Layout->addWidget(card2Title);
    card2Layout->addWidget(newCustomersLabel);
    statsLayout->addWidget(card2);

    // Card 3: VIP customers
    QFrame *card3 = new QFrame(this);
    card3->setObjectName("statCard");
    QVBoxLayout *card3Layout = new QVBoxLayout(card3);
    QLabel *card3Title = new QLabel("Khách hàng VIP", this);
    card3Title->setObjectName("statCardTitle");
    vipCustomersLabel = new QLabel("0", this);
    vipCustomersLabel->setObjectName("statCardValue");
    card3Layout->addWidget(card3Title);
    card3Layout->addWidget(vipCustomersLabel);
    statsLayout->addWidget(card3);

    leftLayout->addWidget(statsContainer);

    // ===== CUSTOMER TABLE =====
    setupTable();
    leftLayout->addWidget(customerTable, 1);

    mainLayout->addWidget(leftWidget, 7); // 70%
}

void CustomerManagementPage::setupRightPanel()
{
    // Right container (30%) - Fixed width
    QWidget *rightWidget = new QWidget(this);
    rightWidget->setFixedWidth(420); // Fixed width to prevent resizing when sidebar opens
    rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setSpacing(15);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    // ===== RIGHT TOP (50%) - Customer Details =====
    detailsContainer = new QFrame(this);
    detailsContainer->setObjectName("detailsContainer");
    rightTopLayout = new QVBoxLayout(detailsContainer);
    rightTopLayout->setSpacing(15);
    rightTopLayout->setContentsMargins(0, 0, 0, 0);

    detailsTitleLabel = new QLabel("Khách hàng", this);
    detailsTitleLabel->setObjectName("sectionTitle");
    rightTopLayout->addWidget(detailsTitleLabel);

    // Form content with padding
    QWidget *formContent = new QWidget(this);
    QVBoxLayout *formContentLayout = new QVBoxLayout(formContent);
    formContentLayout->setSpacing(15);
    formContentLayout->setContentsMargins(15, 15, 15, 15);

    // Name field - horizontal layout
    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->setSpacing(10);
    QLabel *nameLabel = new QLabel("Họ tên:", this);
    nameLabel->setObjectName("formLabel");
    nameLabel->setFixedWidth(100);
    nameLayout->addWidget(nameLabel);
    nameEdit = new QLineEdit(this);
    nameEdit->setObjectName("formInput");
    nameEdit->setPlaceholderText("Nhập họ tên");
    // Name validation - No numbers or special characters (Vietnamese supported)
    QRegularExpression nameRegex("^[a-zA-Z\\sÀÁÂÃÈÉÊÌÍÒÓÔÕÙÚĂĐĨŨƠàáâãèéêìíòóôõùúăđĩũơƯĂẠẢẤẦẨẪẬẮẰẲẴẶẸẺẼỀỀỂưăạảấầẩẫậắằẳẵặẹẻẽềềểỄỆỈỊỌỎỐỒỔỖỘỚỜỞỠỢỤỦỨỪễệỉịọỏốồổỗộớờởỡợụủứừỬỮỰỲỴÝỶỸửữựỳỵýỷỹ]+$");
    QRegularExpressionValidator *nameValidator = new QRegularExpressionValidator(nameRegex, this);
    nameEdit->setValidator(nameValidator);
    nameLayout->addWidget(nameEdit, 1);
    formContentLayout->addLayout(nameLayout);

    // Phone field - horizontal layout
    QHBoxLayout *phoneLayout = new QHBoxLayout();
    phoneLayout->setSpacing(10);
    QLabel *phoneLabel = new QLabel("Số điện thoại:", this);
    phoneLabel->setObjectName("formLabel");
    phoneLabel->setFixedWidth(100);
    phoneLayout->addWidget(phoneLabel);
    phoneEdit = new QLineEdit(this);
    phoneEdit->setObjectName("formInput");
    phoneEdit->setPlaceholderText("Nhập số điện thoại");
    // Phone validation - Exactly 10 digits, starts with 0
    QRegularExpression phoneRegex("^0[0-9]{0,9}$");
    QRegularExpressionValidator *phoneValidator = new QRegularExpressionValidator(phoneRegex, this);
    phoneEdit->setValidator(phoneValidator);
    phoneLayout->addWidget(phoneEdit, 1);
    formContentLayout->addLayout(phoneLayout);

    // Spending field - horizontal layout (read-only)
    QHBoxLayout *spendingLayout = new QHBoxLayout();
    spendingLayout->setSpacing(10);
    // Spending info
    QLabel *spendingLabel = new QLabel("Tổng chi tiêu:", this);
    spendingLabel->setObjectName("formLabel");
    spendingLabel->setFixedWidth(100);
    spendingLayout->addWidget(spendingLabel);
    spendingEdit = new QLineEdit(this);
    spendingEdit->setObjectName("formInput");
    spendingEdit->setReadOnly(true);
    spendingEdit->setText("0đ");
    spendingLayout->addWidget(spendingEdit, 1);
    formContentLayout->addLayout(spendingLayout);

    // Buttons
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(10);

    // Add button
    addButton = new QPushButton("+ Thêm khách hàng", this);
    addButton->setObjectName("addButton");
    addButton->setMinimumHeight(44);
    addButton->setCursor(Qt::PointingHandCursor);
    buttonsLayout->addWidget(addButton);

    saveButton = new QPushButton("Lưu", this);
    saveButton->setObjectName("primaryButton");
    saveButton->setMinimumHeight(40);
    saveButton->setEnabled(false);
    buttonsLayout->addWidget(saveButton);

    deleteButton = new QPushButton("Xoá", this);
    deleteButton->setObjectName("dangerButton");
    deleteButton->setMinimumHeight(40);
    deleteButton->setEnabled(false);
    buttonsLayout->addWidget(deleteButton);

    formContentLayout->addLayout(buttonsLayout);

    rightTopLayout->addWidget(formContent);
    rightTopLayout->addStretch();

    rightLayout->addWidget(detailsContainer, 1); // 50%

    // ===== RIGHT BOTTOM (50%) - Booking History =====
    historyContainer = new QFrame(this);
    historyContainer->setObjectName("historyContainer");
    rightBottomLayout = new QVBoxLayout(historyContainer);
    rightBottomLayout->setSpacing(0);
    rightBottomLayout->setContentsMargins(0, 0, 0, 0);

    historyTitleLabel = new QLabel("Lịch sử đặt sân", this);
    historyTitleLabel->setObjectName("sectionTitle");
    rightBottomLayout->addWidget(historyTitleLabel);

    historyList = new QListWidget(this);
    historyList->setObjectName("historyList");
    rightBottomLayout->addWidget(historyList);

    rightLayout->addWidget(historyContainer, 1); // 50%

    mainLayout->addWidget(rightWidget, 3); // 30%
}

void CustomerManagementPage::setupTable()
{
    customerTable = new QTableWidget(this);
    customerTable->setObjectName("customerTable");
    customerTable->setColumnCount(6);
    customerTable->setHorizontalHeaderLabels({"Mã KH", "Tên KH", "Số điện thoại",
                                              "Hội viên", "Tổng chi tiêu", "Ngày đăng ký"});

    // Set column alignments and styling for headers
    QFont headerFont;
    headerFont.setBold(true);
    headerFont.setPointSize(10);

    for (int i = 0; i < customerTable->columnCount(); i++)
    {
        QTableWidgetItem *headerItem = new QTableWidgetItem(customerTable->horizontalHeaderItem(i)->text());

        // Set bold black font
        headerItem->setFont(headerFont);
        headerItem->setForeground(QBrush(QColor(0, 0, 0))); // Black text

        // Set alignment based on column
        if (i == 0 || i == 2) // Mã KH, SĐT - center
        {
            headerItem->setTextAlignment(Qt::AlignCenter);
        }
        else if (i == 3) // Hội viên - center
        {
            headerItem->setTextAlignment(Qt::AlignCenter);
        }
        else if (i == 4 || i == 5) // Tổng chi tiêu, Ngày đăng ký - right
        {
            headerItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        }
        else // Tên KH - left
        {
            headerItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        }

        customerTable->setHorizontalHeaderItem(i, headerItem);
    }

    customerTable->horizontalHeader()->setStretchLastSection(true);
    customerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    customerTable->verticalHeader()->setVisible(false);
    customerTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    customerTable->setSelectionMode(QAbstractItemView::SingleSelection);
    customerTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    customerTable->setAlternatingRowColors(false); // Disable alternating colors
    customerTable->setShowGrid(false);             // Hide all gridlines initially

    // Enable scrolling
    customerTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    customerTable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void CustomerManagementPage::setupConnections()
{
    // Search & Filters
    connect(searchEdit, &QLineEdit::textChanged,
            this, &CustomerManagementPage::onSearchTextChanged);
    connect(tierFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CustomerManagementPage::onTierFilterChanged);
    connect(spendingFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CustomerManagementPage::onSpendingFilterChanged);

    // Table
    connect(customerTable, &QTableWidget::cellClicked,
            this, &CustomerManagementPage::onCustomerRowClicked);

    // Buttons
    connect(addButton, &QPushButton::clicked,
            this, &CustomerManagementPage::onAddCustomerClicked);
    connect(saveButton, &QPushButton::clicked,
            this, &CustomerManagementPage::onSaveCustomerClicked);
    connect(deleteButton, &QPushButton::clicked,
            this, &CustomerManagementPage::onDeleteCustomerClicked);
}

void CustomerManagementPage::loadCustomers()
{
    if (!system)
        return;

    customerTable->setRowCount(0);
    const MangDong<KhachHang *> &customers = system->layDanhSachKhachHang();

    // Get filter values
    QString searchText = searchEdit->text().toLower();
    QString tierFilter = tierFilterCombo->currentData().toString();
    QString spendingFilter = spendingFilterCombo->currentData().toString();

    for (int i = 0; i < customers.size(); i++)
    {
        KhachHang *kh = customers[i];
        if (!kh)
            continue;

        // Apply search filter
        QString name = QString::fromStdString(kh->getHoTen()).toLower();
        QString phone = QString::fromStdString(kh->getSoDienThoai());
        if (!searchText.isEmpty())
        {
            if (!name.contains(searchText) && !phone.contains(searchText))
            {
                continue;
            }
        }

        // Apply tier filter
        if (tierFilter != "ALL")
        {
            QString customerTier = QString::fromStdString(kh->layTenHang()).toUpper();
            if (tierFilter != customerTier)
            {
                continue;
            }
        }

        // Apply spending filter
        double spending = kh->layTongChiTieu();
        if (spendingFilter != "ALL")
        {
            if (spendingFilter == "LT_1M" && spending >= 1000000)
                continue;
            if (spendingFilter == "1M_5M" && (spending < 1000000 || spending >= 5000000))
                continue;
            if (spendingFilter == "5M_10M" && (spending < 5000000 || spending >= 10000000))
                continue;
            if (spendingFilter == "10M_20M" && (spending < 10000000 || spending >= 20000000))
                continue;
            if (spendingFilter == "GTE_20M" && spending < 20000000)
                continue;
        }

        // Add row
        int row = customerTable->rowCount();
        customerTable->insertRow(row);

        // Mã KH - căn giữa
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::fromStdString(kh->layMaKhachHang()));
        idItem->setTextAlignment(Qt::AlignCenter);
        customerTable->setItem(row, 0, idItem);

        // Tên KH - căn trái
        QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(kh->getHoTen()));
        nameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        customerTable->setItem(row, 1, nameItem);

        // SĐT - căn giữa
        QTableWidgetItem *phoneItem = new QTableWidgetItem(QString::fromStdString(kh->getSoDienThoai()));
        phoneItem->setTextAlignment(Qt::AlignCenter);
        customerTable->setItem(row, 2, phoneItem);

        // Hội viên - căn giữa
        QTableWidgetItem *tierItem = new QTableWidgetItem(getTierDisplayName(kh->layHang()));
        tierItem->setTextAlignment(Qt::AlignCenter);
        customerTable->setItem(row, 3, tierItem);

        // Tổng chi tiêu - căn phải
        QTableWidgetItem *spendingItem = new QTableWidgetItem(formatCurrency(spending));
        spendingItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        customerTable->setItem(row, 4, spendingItem);

        // Ngày đăng ký - căn phải
        NgayThang ngayDK = kh->layNgayDangKy();
        QString dateStr = QString("%1/%2/%3")
                              .arg(ngayDK.getNgay(), 2, 10, QChar('0'))
                              .arg(ngayDK.getThang(), 2, 10, QChar('0'))
                              .arg(ngayDK.getNam());
        QTableWidgetItem *dateItem = new QTableWidgetItem(dateStr);
        dateItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        customerTable->setItem(row, 5, dateItem);
    }

    updateStatsCards();
}

void CustomerManagementPage::updateStatsCards()
{
    if (!system)
        return;

    const MangDong<KhachHang *> &customers = system->layDanhSachKhachHang();
    int total = customers.size();
    int newThisMonth = 0;
    int vip = 0;

    QDate currentDate = QDate::currentDate();
    int currentMonth = currentDate.month();
    int currentYear = currentDate.year();

    for (int i = 0; i < customers.size(); i++)
    {
        KhachHang *kh = customers[i];
        if (!kh)
            continue;

        // Check if registered this month
        NgayThang ngayDK = kh->layNgayDangKy();
        if (ngayDK.getThang() == currentMonth && ngayDK.getNam() == currentYear)
        {
            newThisMonth++;
        }

        // Check if VIP (Kim Cương)
        if (kh->layHang() == HangKhachHang::KIM_CUONG)
        {
            vip++;
        }
    }

    totalCustomersLabel->setText(QString::number(total));
    newCustomersLabel->setText(QString::number(newThisMonth));
    vipCustomersLabel->setText(QString::number(vip));
}

void CustomerManagementPage::onSearchTextChanged(const QString &text)
{
    Q_UNUSED(text);
    loadCustomers();
}

void CustomerManagementPage::onTierFilterChanged(int index)
{
    Q_UNUSED(index);
    loadCustomers();
}

void CustomerManagementPage::onSpendingFilterChanged(int index)
{
    Q_UNUSED(index);
    loadCustomers();
}

void CustomerManagementPage::onCustomerRowClicked(int row)
{
    if (row < 0 || row >= customerTable->rowCount())
        return;

    QString maKH = customerTable->item(row, 0)->text();
    currentCustomer = system->timKhachHang(maKH.toStdString());

    if (!currentCustomer)
        return;

    isEditMode = true;

    // Enable form fields
    nameEdit->setEnabled(true);
    phoneEdit->setEnabled(true);
    nameEdit->setPlaceholderText("Nhập họ tên");
    phoneEdit->setPlaceholderText("Nhập số điện thoại");

    // Populate form
    nameEdit->setText(QString::fromStdString(currentCustomer->getHoTen()));
    phoneEdit->setText(QString::fromStdString(currentCustomer->getSoDienThoai()));
    spendingEdit->setText(formatCurrency(currentCustomer->layTongChiTieu()));

    // Enable buttons
    saveButton->setEnabled(true);
    deleteButton->setEnabled(true);

    // Load booking history
    loadBookingHistory(maKH.toStdString());
}

void CustomerManagementPage::onAddCustomerClicked()
{
    clearCustomerForm();
    isEditMode = false;
    currentCustomer = nullptr;

    // Enable form fields for new customer
    nameEdit->setEnabled(true);
    phoneEdit->setEnabled(true);
    nameEdit->setPlaceholderText("Nhập họ tên");
    phoneEdit->setPlaceholderText("Nhập số điện thoại");

    nameEdit->setFocus();
    saveButton->setEnabled(true);
    deleteButton->setEnabled(false);

    historyList->clear();
}

void CustomerManagementPage::onSaveCustomerClicked()
{
    if (!validateCustomerData())
        return;

    QString name = nameEdit->text().trimmed();
    QString phone = phoneEdit->text().trimmed();

    if (isEditMode && currentCustomer)
    {
        // Edit existing customer
        // Check phone uniqueness (exclude current customer)
        if (system->layQuanLyKhachHang()->kiemTraSDTTonTai(
                phone.toStdString(), currentCustomer->layMaKhachHang()))
        {
            QMessageBox::warning(this, "Lỗi",
                                 "Số điện thoại đã được sử dụng bởi khách hàng khác!");
            return;
        }

        currentCustomer->datHoTen(name.toStdString());
        currentCustomer->datSoDienThoai(phone.toStdString());

        system->luuHeThong("D:/PBL2-/Data/data.bin");

        QMessageBox::information(this, "Thành công",
                                 "Cập nhật thông tin khách hàng thành công!");
    }
    else
    {
        // Add new customer
        qDebug() << "=== Adding new customer ===";

        // Check phone uniqueness
        if (system->layQuanLyKhachHang()->kiemTraSDTTonTai(phone.toStdString()))
        {
            qDebug() << "Phone number already exists:" << phone;
            QMessageBox::warning(this, "Lỗi",
                                 "Số điện thoại đã được sử dụng!");
            return;
        }

        // Generate unique customer ID
        std::string maKH = system->layQuanLyKhachHang()->taoMaKhachHangMoi();
        qDebug() << "Generated customer ID:" << QString::fromStdString(maKH);

        KhachHang *newCustomer = new KhachHang(
            name.toStdString(),
            phone.toStdString(),
            "N/A", // địa chỉ - default value
            maKH);

        bool added = system->themKhachHang(newCustomer);
        qDebug() << "Customer added to system:" << added;

        if (added)
        {
            bool saved = system->luuHeThong("D:/PBL2-/Data/data.bin");
            qDebug() << "Data saved to CSV:" << saved;

            QMessageBox::information(this, "Thành công",
                                     QString("Thêm khách hàng mới thành công!\nMã KH: %1")
                                         .arg(QString::fromStdString(maKH)));
            clearCustomerForm();
        }
        else
        {
            delete newCustomer;
            qDebug() << "Failed to add customer to system";
            QMessageBox::warning(this, "Lỗi",
                                 "Không thể thêm khách hàng!");
        }
    }

    loadCustomers();
}

void CustomerManagementPage::onDeleteCustomerClicked()
{
    if (!currentCustomer)
        return;

    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                              "Xác nhận xoá",
                                                              QString("Bạn có chắc muốn xoá khách hàng '%1'?\n\n"
                                                                      "Lưu ý: Mã khách hàng này sẽ không được tái sử dụng.")
                                                                  .arg(QString::fromStdString(currentCustomer->getHoTen())),
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        std::string maKH = currentCustomer->layMaKhachHang();
        if (system->xoaKhachHang(maKH))
        {
            system->luuHeThong("D:/PBL2-/Data/data.bin");
            QMessageBox::information(this, "Thành công",
                                     "Xoá khách hàng thành công!");
            clearCustomerForm();
            loadCustomers();
        }
        else
        {
            QMessageBox::warning(this, "Lỗi",
                                 "Không thể xoá khách hàng!");
        }
    }
}

void CustomerManagementPage::loadBookingHistory(const std::string &maKH)
{
    historyList->clear();

    if (!system)
        return;

    MangDong<DatSan *> bookings = system->timDatSanTheoKhachHang(maKH);

    if (bookings.size() == 0)
    {
        historyList->addItem("Chưa có lịch sử đặt sân");
        return;
    }

    for (int i = 0; i < bookings.size(); i++)
    {
        DatSan *ds = bookings[i];
        if (!ds)
            continue;

        San *san = ds->getSan();
        NgayGio thoiGian = ds->getThoiGianDat();

        QString itemText = QString("%1 - %2/%3/%4 %5:%6 - %7đ")
                               .arg(san ? QString::fromStdString(san->layTenSan()) : "N/A")
                               .arg(thoiGian.getNgay(), 2, 10, QChar('0'))
                               .arg(thoiGian.getThang(), 2, 10, QChar('0'))
                               .arg(thoiGian.getNam())
                               .arg(thoiGian.getGio(), 2, 10, QChar('0'))
                               .arg(thoiGian.getPhut(), 2, 10, QChar('0'))
                               .arg(formatCurrency(ds->getTongTien()));

        historyList->addItem(itemText);
    }
}

void CustomerManagementPage::clearCustomerForm()
{
    nameEdit->clear();
    phoneEdit->clear();
    spendingEdit->setText("0đ");

    // Disable and gray out form when no customer selected
    nameEdit->setEnabled(false);
    phoneEdit->setEnabled(false);
    nameEdit->setPlaceholderText("");
    phoneEdit->setPlaceholderText("");

    saveButton->setEnabled(false);
    deleteButton->setEnabled(false);
    currentCustomer = nullptr;
    historyList->clear();
}

bool CustomerManagementPage::validateCustomerData()
{
    QString name = nameEdit->text().trimmed();
    QString phone = phoneEdit->text().trimmed();

    if (name.isEmpty())
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập họ tên!");
        nameEdit->setFocus();
        return false;
    }

    if (phone.isEmpty())
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập số điện thoại!");
        phoneEdit->setFocus();
        return false;
    }

    if (phone.length() != 10 || !phone.startsWith('0'))
    {
        QMessageBox::warning(this, "Lỗi",
                             "Số điện thoại phải có đúng 10 chữ số và bắt đầu bằng số 0!");
        phoneEdit->setFocus();
        return false;
    }

    // Manual name validation
    QRegularExpression nameRegex("^[a-zA-Z\\sÀÁÂÃÈÉÊÌÍÒÓÔÕÙÚĂĐĨŨƠàáâãèéêìíòóôõùúăđĩũơƯĂẠẢẤẦẨẪẬẮẰẲẴẶẸẺẼỀỀỂưăạảấầẩẫậắằẳẵặẹẻẽềềểỄỆỈỊỌỎỐỒỔỖỘỚỜỞỠỢỤỦỨỪễệỉịọỏốồổỗộớờởỡợụủứừỬỮỰỲỴÝỶỸửữựỳỵýỷỹ]+$");
    if (!nameRegex.match(name).hasMatch())
    {
        QMessageBox::warning(this, "Lỗi", "Tên khách hàng không được chứa số hoặc ký tự đặc biệt!");
        nameEdit->setFocus();
        return false;
    }

    return true;
}

QString CustomerManagementPage::formatCurrency(double amount) const
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(0) << amount;
    QString result = QString::fromStdString(ss.str());

    // Add thousand separators
    int pos = result.length() - 3;
    while (pos > 0)
    {
        result.insert(pos, ',');
        pos -= 3;
    }

    return result + "đ";
}

QString CustomerManagementPage::getTierDisplayName(HangKhachHang hang) const
{
    switch (hang)
    {
    case HangKhachHang::THUONG:
        return "Thường";
    case HangKhachHang::DONG:
        return "Đồng";
    case HangKhachHang::BAC:
        return "Bạc";
    case HangKhachHang::VANG:
        return "Vàng";
    case HangKhachHang::KIM_CUONG:
        return "Kim cương";
    default:
        return "N/A";
    }
}

void CustomerManagementPage::applyStyles()
{
    // Styles are applied via QSS in app.qss
}
