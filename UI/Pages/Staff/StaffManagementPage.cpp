/**
 * @file StaffManagementPage.cpp
 * @brief Implementation of Staff Management (Admin Only)
 */

#include "StaffManagementPage.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QDebug>

StaffManagementPage::StaffManagementPage(QWidget *parent)
    : QWidget(parent),
      system(HeThongQuanLy::getInstance()),
      staffManager(nullptr),
      currentAdmin(nullptr),
      selectedStaff(nullptr),
      isEditMode(false)
{
    staffManager = system->layQuanLyNhanVien();
    setupUI();
    setupConnections();
    // Note: loadStaff() will be called after setCurrentAdmin()
}

StaffManagementPage::~StaffManagementPage()
{
}

void StaffManagementPage::setCurrentAdmin(QuanTriVien *admin)
{
    currentAdmin = admin;
    if (staffManager) {
        staffManager->setCurrentUser(admin);
    }
    
    // Only load staff if admin is valid
    if (currentAdmin) {
        loadStaff();
    }
}

void StaffManagementPage::setupUI()
{
    // Main horizontal layout
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    // ===== LEFT PANEL: Table (70%) =====
    leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(15);

    tableFrame = new QFrame(this);
    tableFrame->setObjectName("tableFrame");
    tableFrame->setStyleSheet(
        "#tableFrame {"
        "   background-color: white;"
        "   border-radius: 8px;"
        "   border: 1px solid #e5e7eb;"
        "}");

    QVBoxLayout *tableLayout = new QVBoxLayout(tableFrame);
    tableLayout->setContentsMargins(20, 20, 20, 20);
    tableLayout->setSpacing(15);

    // Title + Search + Print
    QHBoxLayout *topLayout = new QHBoxLayout();
    
    QLabel *title = new QLabel("👨‍💼 QUẢN LÝ NHÂN VIÊN", this);
    QFont titleFont;
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setStyleSheet("color: #16a34a;");
    topLayout->addWidget(title);
    
    topLayout->addStretch();
    
    // Search
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("🔍 Tìm theo tên, SĐT, username...");
    searchEdit->setMinimumWidth(300);
    searchEdit->setMinimumHeight(38);
    searchEdit->setStyleSheet(
        "QLineEdit {"
        "   padding: 8px 12px;"
        "   border: 1px solid #d1d5db;"
        "   border-radius: 6px;"
        "   background: white;"
        "   font-size: 13px;"
        "}"
        "QLineEdit:focus {"
        "   border: 2px solid #16a34a;"
        "   outline: none;"
        "}");
    topLayout->addWidget(searchEdit);
    
    // Print button
    printBtn = new QPushButton("🖨️ Print", this);
    printBtn->setMinimumHeight(38);
    printBtn->setCursor(Qt::PointingHandCursor);
    printBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #6b7280;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 8px 16px;"
        "   font-weight: bold;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #4b5563;"
        "}");
    topLayout->addWidget(printBtn);
    
    tableLayout->addLayout(topLayout);

    // Staff Table
    staffTable = new QTableWidget(0, 9, this);
    staffTable->setHorizontalHeaderLabels({
        "ID", "Họ Tên", "Giới Tính", "Ngày Sinh", 
        "SĐT", "Username", "Password 👁", "Vai Trò", "Trạng Thái"
    });
    
    staffTable->horizontalHeader()->setStretchLastSection(false);
    staffTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    staffTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    staffTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    staffTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    staffTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    staffTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    staffTable->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    staffTable->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    staffTable->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
    
    staffTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    staffTable->setSelectionMode(QAbstractItemView::SingleSelection);
    staffTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    staffTable->setAlternatingRowColors(true);
    staffTable->verticalHeader()->setVisible(false);
    staffTable->setStyleSheet(
        "QTableWidget {"
        "   background-color: white;"
        "   border: 1px solid #e5e7eb;"
        "   border-radius: 6px;"
        "   gridline-color: #f3f4f6;"
        "}"
        "QHeaderView::section {"
        "   background-color: #f9fafb;"
        "   padding: 8px;"
        "   border: none;"
        "   border-bottom: 2px solid #e5e7eb;"
        "   font-weight: bold;"
        "   font-size: 13px;"
        "   color: #374151;"
        "}"
        "QTableWidget::item {"
        "   padding: 8px;"
        "   font-size: 13px;"
        "}"
        "QTableWidget::item:selected {"
        "   background-color: #dcfce7;"
        "   color: #166534;"
        "}"
        "QTableWidget::item:alternate {"
        "   background-color: #f9fafb;"
        "}");
    
    tableLayout->addWidget(staffTable);
    leftLayout->addWidget(tableFrame);

    // ===== RIGHT PANEL: Form (30%) with Scroll =====
    rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(0);
    
    // Scroll area for form
    QScrollArea *formScrollArea = new QScrollArea(this);
    formScrollArea->setWidgetResizable(true);
    formScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    formScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    formScrollArea->setFrameShape(QFrame::NoFrame);

    formFrame = new QFrame();
    formFrame->setObjectName("formFrame");
    formFrame->setStyleSheet(
        "#formFrame {"
        "   background-color: white;"
        "   border-radius: 8px;"
        "   border: 1px solid #e5e7eb;"
        "}");

    QVBoxLayout *formLayout = new QVBoxLayout(formFrame);
    formLayout->setContentsMargins(20, 20, 20, 20);
    formLayout->setSpacing(12);

    // Form Title
    QLabel *formTitle = new QLabel("THÔNG TIN NHÂN VIÊN", this);
    QFont formTitleFont;
    formTitleFont.setPointSize(14);
    formTitleFont.setBold(true);
    formTitle->setFont(formTitleFont);
    formTitle->setStyleSheet("color: #16a34a;");
    formLayout->addWidget(formTitle);

    // Name
    QLabel *nameLabel = new QLabel("Họ và tên:", this);
    nameLabel->setStyleSheet("font-weight: bold; font-size: 13px; color: #374151;");
    formLayout->addWidget(nameLabel);
    
    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Nhập họ tên");
    nameEdit->setMinimumHeight(38);
    nameEdit->setStyleSheet(
        "QLineEdit {"
        "   padding: 8px 12px;"
        "   border: 1px solid #d1d5db;"
        "   border-radius: 6px;"
        "   font-size: 13px;"
        "}"
        "QLineEdit:focus {"
        "   border: 2px solid #16a34a;"
        "}");
    formLayout->addWidget(nameEdit);

    // Gender
    QLabel *genderLabel = new QLabel("Giới tính:", this);
    genderLabel->setStyleSheet("font-weight: bold; font-size: 13px; color: #374151; margin-top: 8px;");
    formLayout->addWidget(genderLabel);
    
    QHBoxLayout *genderLayout = new QHBoxLayout();
    genderGroup = new QButtonGroup(this);
    
    maleRadio = new QRadioButton("Nam", this);
    femaleRadio = new QRadioButton("Nữ", this);
    otherRadio = new QRadioButton("Khác", this);
    
    maleRadio->setChecked(true);
    maleRadio->setStyleSheet("font-size: 13px;");
    femaleRadio->setStyleSheet("font-size: 13px;");
    otherRadio->setStyleSheet("font-size: 13px;");
    
    genderGroup->addButton(maleRadio, 0);
    genderGroup->addButton(femaleRadio, 1);
    genderGroup->addButton(otherRadio, 2);
    
    genderLayout->addWidget(maleRadio);
    genderLayout->addWidget(femaleRadio);
    genderLayout->addWidget(otherRadio);
    genderLayout->addStretch();
    formLayout->addLayout(genderLayout);

    // Date of Birth
    QLabel *dobLabel = new QLabel("Ngày sinh:", this);
    dobLabel->setStyleSheet("font-weight: bold; font-size: 13px; color: #374151; margin-top: 8px;");
    formLayout->addWidget(dobLabel);
    
    dobEdit = new QDateEdit(this);
    dobEdit->setDate(QDate::currentDate().addYears(-20));
    dobEdit->setCalendarPopup(true);
    dobEdit->setDisplayFormat("dd/MM/yyyy");
    dobEdit->setMinimumHeight(38);
    dobEdit->setStyleSheet(
        "QDateEdit {"
        "   padding: 8px 12px;"
        "   border: 1px solid #d1d5db;"
        "   border-radius: 6px;"
        "   font-size: 13px;"
        "}"
        "QDateEdit:focus {"
        "   border: 2px solid #16a34a;"
        "}");
    formLayout->addWidget(dobEdit);

    // Phone
    QLabel *phoneLabel = new QLabel("Số điện thoại:", this);
    phoneLabel->setStyleSheet("font-weight: bold; font-size: 13px; color: #374151; margin-top: 8px;");
    formLayout->addWidget(phoneLabel);
    
    phoneEdit = new QLineEdit(this);
    phoneEdit->setPlaceholderText("Nhập SĐT");
    phoneEdit->setMinimumHeight(38);
    phoneEdit->setStyleSheet(nameEdit->styleSheet());
    formLayout->addWidget(phoneEdit);

    // Username
    QLabel *usernameLabel = new QLabel("Tên đăng nhập:", this);
    usernameLabel->setStyleSheet("font-weight: bold; font-size: 13px; color: #374151; margin-top: 8px;");
    formLayout->addWidget(usernameLabel);
    
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Nhập username");
    usernameEdit->setMinimumHeight(38);
    usernameEdit->setStyleSheet(nameEdit->styleSheet());
    formLayout->addWidget(usernameEdit);

    // Password
    QLabel *passwordLabel = new QLabel("Mật khẩu:", this);
    passwordLabel->setStyleSheet("font-weight: bold; font-size: 13px; color: #374151; margin-top: 8px;");
    formLayout->addWidget(passwordLabel);
    
    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Nhập mật khẩu");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setMinimumHeight(38);
    passwordEdit->setStyleSheet(nameEdit->styleSheet());
    formLayout->addWidget(passwordEdit);

    // Role (Staff Management only handles NhanVien)
    QLabel *roleLabel = new QLabel("Vai trò:", this);
    roleLabel->setStyleSheet("font-weight: bold; font-size: 13px; color: #374151; margin-top: 8px;");
    formLayout->addWidget(roleLabel);
    
    roleCombo = new QComboBox(this);
    roleCombo->addItem("Nhân viên", static_cast<int>(VaiTro::NHAN_VIEN));
    roleCombo->setEnabled(false); // Disabled, staff only
    roleCombo->setMinimumHeight(38);
    roleCombo->setStyleSheet(
        "QComboBox {"
        "   padding: 8px 12px;"
        "   border: 1px solid #d1d5db;"
        "   border-radius: 6px;"
        "   font-size: 13px;"
        "}"
        "QComboBox:focus {"
        "   border: 2px solid #16a34a;"
        "}"
        "QComboBox::drop-down {"
        "   border: none;"
        "   padding-right: 8px;"
        "}");
    formLayout->addWidget(roleCombo);

    formLayout->addSpacing(10);

    // Action Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);

    addNewBtn = new QPushButton("➕ Thêm mới", this);
    addNewBtn->setMinimumHeight(40);
    addNewBtn->setCursor(Qt::PointingHandCursor);
    addNewBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #16a34a;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 10px 16px;"
        "   font-weight: bold;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #15803d;"
        "}");
    btnLayout->addWidget(addNewBtn);

    saveBtn = new QPushButton("💾 Lưu", this);
    saveBtn->setMinimumHeight(40);
    saveBtn->setCursor(Qt::PointingHandCursor);
    saveBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #3b82f6;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 10px 16px;"
        "   font-weight: bold;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2563eb;"
        "}");
    btnLayout->addWidget(saveBtn);

    deleteBtn = new QPushButton("🗑️ Xóa", this);
    deleteBtn->setMinimumHeight(40);
    deleteBtn->setCursor(Qt::PointingHandCursor);
    deleteBtn->setEnabled(false);
    deleteBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #ef4444;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 10px 16px;"
        "   font-weight: bold;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #dc2626;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #d1d5db;"
        "   color: #9ca3af;"
        "}");
    btnLayout->addWidget(deleteBtn);

    formLayout->addLayout(btnLayout);
    formLayout->addStretch();

    rightLayout->addWidget(formFrame);

    // Add panels to main layout
    mainLayout->addLayout(leftLayout, 7); // 70%
    mainLayout->addLayout(rightLayout, 3); // 30%

    // Apply page background
    setStyleSheet("QWidget { background-color: #f9fafb; }");
}

void StaffManagementPage::setupConnections()
{
    connect(searchEdit, &QLineEdit::textChanged, this, &StaffManagementPage::onSearchTextChanged);
    connect(staffTable, &QTableWidget::cellClicked, this, [this](int row, int column) {
        // Column 6 is password - toggle visibility
        if (column == 6) {
            QTableWidgetItem *passwordItem = staffTable->item(row, column);
            if (passwordItem) {
                // Get stored password and visibility state
                QString realPassword = passwordItem->data(Qt::UserRole).toString();
                bool isVisible = passwordItem->data(Qt::UserRole + 1).toBool();
                
                if (isVisible) {
                    // Hide password
                    passwordItem->setText(QString(realPassword.length(), QChar(0x2022)));
                    passwordItem->setData(Qt::UserRole + 1, false);
                } else {
                    // Show password
                    passwordItem->setText(realPassword);
                    passwordItem->setData(Qt::UserRole + 1, true);
                }
            }
        } else {
            // For other columns, handle row selection
            onStaffRowClicked(row);
        }
    });
    connect(addNewBtn, &QPushButton::clicked, this, &StaffManagementPage::onAddNewClicked);
    connect(saveBtn, &QPushButton::clicked, this, &StaffManagementPage::onSaveClicked);
    connect(deleteBtn, &QPushButton::clicked, this, &StaffManagementPage::onDeleteClicked);
    connect(printBtn, &QPushButton::clicked, this, &StaffManagementPage::onPrintClicked);
}

void StaffManagementPage::loadStaff()
{
    staffTable->setRowCount(0);
    
    // Guard against null staffManager or currentAdmin
    if (!staffManager || !currentAdmin) {
        qDebug() << "StaffManagementPage: Cannot load staff - staffManager or currentAdmin is null";
        return;
    }
    
    const MangDong<NguoiDung *> &staffList = staffManager->layDanhSachNhanVien();
    
    for (int i = 0; i < staffList.size(); i++) {
        NguoiDung *user = staffList[i];
        if (!user) continue;
        
        // Skip admin - only show staff
        if (user->layVaiTro() == VaiTro::QUAN_TRI_VIEN) continue;
        
        // Cast to NhanVien
        NhanVien *nv = dynamic_cast<NhanVien*>(user);
        if (!nv) continue;
        
        int row = staffTable->rowCount();
        staffTable->insertRow(row);
        
        // Col 0: ID (NhanVien has maNV, QuanTriVien uses username)
        QString maNV = nv ? QString::fromStdString(nv->layMaNhanVien()) : QString::fromStdString(user->layTenDangNhap());
        QTableWidgetItem *idItem = new QTableWidgetItem(maNV);
        idItem->setTextAlignment(Qt::AlignCenter);
        staffTable->setItem(row, 0, idItem);
        
        // Col 1: Name
        QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(user->layHoTen()));
        nameItem->setTextAlignment(Qt::AlignCenter);
        staffTable->setItem(row, 1, nameItem);
        
        // Col 2: Gender
        QTableWidgetItem *genderItem = new QTableWidgetItem(QString::fromStdString(user->layGioiTinh()));
        genderItem->setTextAlignment(Qt::AlignCenter);
        staffTable->setItem(row, 2, genderItem);
        
        // Col 3: DOB
        QTableWidgetItem *dobItem = new QTableWidgetItem(QString::fromStdString(user->layNgaySinh()));
        dobItem->setTextAlignment(Qt::AlignCenter);
        staffTable->setItem(row, 3, dobItem);
        
        // Col 4: Phone
        QTableWidgetItem *phoneItem = new QTableWidgetItem(QString::fromStdString(user->laySoDienThoai()));
        phoneItem->setTextAlignment(Qt::AlignCenter);
        staffTable->setItem(row, 4, phoneItem);
        
        // Col 5: Username
        QTableWidgetItem *usernameItem = new QTableWidgetItem(QString::fromStdString(user->layTenDangNhap()));
        usernameItem->setTextAlignment(Qt::AlignCenter);
        staffTable->setItem(row, 5, usernameItem);
        
        // Col 6: Password (real password, initially masked)
        QString password = QString::fromStdString(user->layMatKhau());
        QTableWidgetItem *passwordItem = new QTableWidgetItem(QString(password.length(), QChar(0x2022))); // Unicode bullet
        passwordItem->setData(Qt::UserRole, password); // Store real password
        passwordItem->setData(Qt::UserRole + 1, false); // Store visibility state
        passwordItem->setTextAlignment(Qt::AlignCenter);
        staffTable->setItem(row, 6, passwordItem);
        
        // Col 7: Salary
        QString salary = QString::number(nv->layLuongCoBan(), 'f', 0) + " VNĐ";
        QTableWidgetItem *salaryItem = new QTableWidgetItem(salary);
        salaryItem->setTextAlignment(Qt::AlignCenter);
        staffTable->setItem(row, 7, salaryItem);
    }
    
    qDebug() << "Loaded" << staffList.size() << "staff members";
}

void StaffManagementPage::onSearchTextChanged(const QString &text)
{
    QString searchLower = text.toLower();
    
    for (int row = 0; row < staffTable->rowCount(); row++) {
        bool match = false;
        
        // Search in Name, Phone, Username columns
        for (int col : {1, 4, 5}) {
            QTableWidgetItem *item = staffTable->item(row, col);
            if (item && item->text().toLower().contains(searchLower)) {
                match = true;
                break;
            }
        }
        
        staffTable->setRowHidden(row, !match);
    }
}

void StaffManagementPage::onStaffRowClicked(int row)
{
    if (row < 0) return;
    
    // Find staff in Core by ID
    QString maNV = staffTable->item(row, 0)->text();
    selectedStaff = staffManager->timNhanVien(maNV.toStdString());
    
    if (!selectedStaff) {
        qDebug() << "Staff not found:" << maNV;
        return;
    }
    
    // Populate form with selected staff data
    nameEdit->setText(QString::fromStdString(selectedStaff->layHoTen()));
    
    QString gender = QString::fromStdString(selectedStaff->layGioiTinh());
    if (gender == "Nam") maleRadio->setChecked(true);
    else if (gender == "Nữ") femaleRadio->setChecked(true);
    else otherRadio->setChecked(true);
    
    QString dobStr = QString::fromStdString(selectedStaff->layNgaySinh());
    dobEdit->setDate(QDate::fromString(dobStr, "dd/MM/yyyy"));
    
    phoneEdit->setText(QString::fromStdString(selectedStaff->laySoDienThoai()));
    usernameEdit->setText(QString::fromStdString(selectedStaff->layTenDangNhap()));
    passwordEdit->setText(""); // Don't show password
    
    // Always show staff role (admin managed separately)
    roleCombo->setCurrentIndex(0);
    
    deleteBtn->setEnabled(true);
    isEditMode = true;
}

void StaffManagementPage::onAddNewClicked()
{
    clearForm();
    deleteBtn->setEnabled(false);
    isEditMode = false;
}

void StaffManagementPage::onSaveClicked()
{
    if (!validateForm()) {
        return;
    }
    
    if (!staffManager || !currentAdmin) {
        QMessageBox::warning(this, "Lỗi", "Chưa khởi tạo hệ thống!");
        return;
    }
    
    // Get form data
    QString name = nameEdit->text();
    QString gender = maleRadio->isChecked() ? "Nam" : (femaleRadio->isChecked() ? "Nữ" : "Khác");
    QString dob = dobEdit->date().toString("dd/MM/yyyy");
    QString phone = phoneEdit->text();
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    // Always create NhanVien (staff), admin managed separately
    
    if (isEditMode && selectedStaff) {
        // Update existing staff
        NhanVien* nv = dynamic_cast<NhanVien*>(selectedStaff);
        if (!nv) {
            QMessageBox::warning(this, "Lỗi", "Lỗi kiểu dữ liệu!");
            return;
        }
        
        NhanVien nvMoi(
            name.toStdString(),
            phone.toStdString(),
            "", // address - not in form
            username.toStdString(),
            password.isEmpty() ? nv->layMatKhau() : password.toStdString(),
            nv->layMaNhanVien(),
            nv->layLuongCoBan(),
            nv->layCaLamViec()
        );
        nvMoi.datGioiTinh(gender.toStdString());
        nvMoi.datNgaySinh(dob.toStdString());
        nvMoi.datVaiTro(VaiTro::NHAN_VIEN); // Always staff
        nvMoi.datHoatDong(true);
        
        if (staffManager->capNhatNhanVien(nv->layMaNhanVien(), nvMoi)) {
            QMessageBox::information(this, "Thành công", "Đã cập nhật thông tin nhân viên!");
            loadStaff();
            clearForm();
        } else {
            QMessageBox::warning(this, "Lỗi", "Không thể cập nhật nhân viên!");
        }
    } else {
        // Add new staff
        if (password.isEmpty()) {
            QMessageBox::warning(this, "Lỗi", "Vui lòng nhập mật khẩu cho nhân viên mới!");
            return;
        }
        
        std::string maNV = staffManager->taoMaNhanVienMoi();
        
        // Always create NhanVien (staff only)
        NhanVien *nv = new NhanVien(
            name.toStdString(),
            phone.toStdString(),
            "N/A",
            username.toStdString(),
            password.toStdString(),
            maNV,
            0, // Default salary
            CaLamViec::SANG // Default shift
        );
        
        nv->datGioiTinh(gender.toStdString());
        nv->datNgaySinh(dob.toStdString());
        nv->datHoatDong(true);
        
        if (staffManager->themNhanVien(nv)) {
            QMessageBox::information(this, "Thành công", "Đã thêm nhân viên mới!");
            loadStaff();
            clearForm();
        } else {
            QMessageBox::warning(this, "Lỗi", "Không thể thêm nhân viên! Username hoặc SĐT đã tồn tại.");
            delete nv;
        }
    }
}

void StaffManagementPage::onDeleteClicked()
{
    if (!selectedStaff) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn nhân viên cần xóa!");
        return;
    }
    
    QString name = QString::fromStdString(selectedStaff->layHoTen());
    QString maNV = QString::fromStdString(selectedStaff->layMaNhanVien());
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Xác nhận xóa",
        QString("Bạn có chắc chắn muốn xóa nhân viên '%1' (%2)?").arg(name, maNV),
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        if (staffManager->xoaNhanVien(maNV.toStdString())) {
            QMessageBox::information(this, "Thành công", "Đã xóa nhân viên!");
            loadStaff();
            clearForm();
            deleteBtn->setEnabled(false);
        } else {
            QMessageBox::warning(this, "Lỗi", "Không thể xóa nhân viên!");
        }
    }
}

void StaffManagementPage::onPrintClicked()
{
    QMessageBox::information(this, "Print", "Chức năng in đang được phát triển!");
}

void StaffManagementPage::clearForm()
{
    nameEdit->clear();
    maleRadio->setChecked(true);
    dobEdit->setDate(QDate::currentDate().addYears(-20));
    phoneEdit->clear();
    usernameEdit->clear();
    passwordEdit->clear();
    roleCombo->setCurrentIndex(0);
    
    staffTable->clearSelection();
    selectedStaff = nullptr;
    isEditMode = false;
}

void StaffManagementPage::populateForm(NhanVien *staff)
{
    if (!staff) return;
    
    nameEdit->setText(QString::fromStdString(staff->layHoTen()));
    phoneEdit->setText(QString::fromStdString(staff->laySoDienThoai()));
    usernameEdit->setText(QString::fromStdString(staff->layTenDangNhap()));
    
    QString gender = QString::fromStdString(staff->layGioiTinh());
    if (gender == "Nam") maleRadio->setChecked(true);
    else if (gender == "Nữ") femaleRadio->setChecked(true);
    else otherRadio->setChecked(true);
    
    QString dobStr = QString::fromStdString(staff->layNgaySinh());
    dobEdit->setDate(QDate::fromString(dobStr, "dd/MM/yyyy"));
    
    roleCombo->setCurrentIndex(staff->layVaiTro() == VaiTro::QUAN_TRI_VIEN ? 1 : 0);
    
    passwordEdit->clear(); // Don't show password
    
    deleteBtn->setEnabled(true);
    isEditMode = true;
    selectedStaff = staff;
}

bool StaffManagementPage::validateForm()
{
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập họ tên!");
        nameEdit->setFocus();
        return false;
    }
    
    if (phoneEdit->text().trimmed().length() < 10) {
        QMessageBox::warning(this, "Lỗi", "Số điện thoại phải có ít nhất 10 số!");
        phoneEdit->setFocus();
        return false;
    }
    
    if (usernameEdit->text().trimmed().length() < 3) {
        QMessageBox::warning(this, "Lỗi", "Username phải có ít nhất 3 ký tự!");
        usernameEdit->setFocus();
        return false;
    }
    
    if (!isEditMode && passwordEdit->text().length() < 6) {
        QMessageBox::warning(this, "Lỗi", "Mật khẩu phải có ít nhất 6 ký tự!");
        passwordEdit->setFocus();
        return false;
    }
    
    return true;
}
