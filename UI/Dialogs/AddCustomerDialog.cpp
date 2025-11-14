/**
 * @file AddCustomerDialog.cpp
 * @brief Implementation of Add Customer Dialog
 * 
 * @author khninh22 - Service Module
 * @date 2025-11-15
 */

#include "AddCustomerDialog.h"
#include "../../Core/QuanLy/HeThongQuanLy.h"
#include <QMessageBox>
#include <QFont>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

AddCustomerDialog::AddCustomerDialog(QWidget *parent)
    : QDialog(parent),
      newCustomer(nullptr)
{
    setWindowTitle("Thêm khách hàng mới");
    setMinimumSize(450, 350);
    
    setupUI();
}

AddCustomerDialog::~AddCustomerDialog()
{
}

KhachHang *AddCustomerDialog::getNewCustomer() const
{
    return newCustomer;
}

void AddCustomerDialog::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel *titleLabel = new QLabel("➕ Thêm khách hàng mới", this);
    titleLabel->setObjectName("dialogTitle");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    QLabel *descLabel = new QLabel("Nhập thông tin khách hàng:", this);
    descLabel->setObjectName("infoLabel");
    mainLayout->addWidget(descLabel);

    mainLayout->addSpacing(10);

    // Phone number (required)
    QLabel *phoneLabel = new QLabel("Số điện thoại: *", this);
    phoneLabel->setObjectName("fieldLabel");
    mainLayout->addWidget(phoneLabel);

    phoneEdit = new QLineEdit(this);
    phoneEdit->setPlaceholderText("VD: 0912345678");
    phoneEdit->setObjectName("formInput");
    phoneEdit->setFixedHeight(40);
    phoneEdit->setMaxLength(11);
    
    // Chỉ cho phép nhập số
    QRegularExpression phoneRegex("^[0-9]*$");
    QRegularExpressionValidator *phoneValidator = new QRegularExpressionValidator(phoneRegex, this);
    phoneEdit->setValidator(phoneValidator);
    
    mainLayout->addWidget(phoneEdit);

    // Name (required)
    QLabel *nameLabel = new QLabel("Họ tên: *", this);
    nameLabel->setObjectName("fieldLabel");
    mainLayout->addWidget(nameLabel);

    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("VD: Nguyễn Văn A");
    nameEdit->setObjectName("formInput");
    nameEdit->setFixedHeight(40);
    mainLayout->addWidget(nameEdit);

    // Address (optional)
    QLabel *addressLabel = new QLabel("Địa chỉ:", this);
    addressLabel->setObjectName("fieldLabel");
    mainLayout->addWidget(addressLabel);

    addressEdit = new QLineEdit(this);
    addressEdit->setPlaceholderText("VD: 123 Đường ABC, Quận XYZ");
    addressEdit->setObjectName("formInput");
    addressEdit->setFixedHeight(40);
    mainLayout->addWidget(addressEdit);

    mainLayout->addSpacing(10);

    // Note
    QLabel *noteLabel = new QLabel("(*) Trường bắt buộc", this);
    noteLabel->setObjectName("infoLabel");
    noteLabel->setStyleSheet("color: #dc2626; font-size: 11px;");
    mainLayout->addWidget(noteLabel);

    mainLayout->addStretch();

    // Buttons
    buttonFrame = new QFrame(this);
    buttonLayout = new QHBoxLayout(buttonFrame);
    buttonLayout->setSpacing(10);

    buttonLayout->addStretch();

    cancelBtn = new QPushButton("Hủy", this);
    cancelBtn->setObjectName("secondaryButton");
    cancelBtn->setFixedHeight(40);
    cancelBtn->setMinimumWidth(100);
    buttonLayout->addWidget(cancelBtn);

    addBtn = new QPushButton("Thêm", this);
    addBtn->setObjectName("primaryButton");
    addBtn->setFixedHeight(40);
    addBtn->setMinimumWidth(100);
    buttonLayout->addWidget(addBtn);

    mainLayout->addWidget(buttonFrame);

    // Connect signals
    connect(addBtn, &QPushButton::clicked, this, &AddCustomerDialog::onAddClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &AddCustomerDialog::onCancelClicked);
    
    // Focus
    phoneEdit->setFocus();
}

bool AddCustomerDialog::validateInput()
{
    QString phone = phoneEdit->text().trimmed();
    QString name = nameEdit->text().trimmed();

    if (phone.isEmpty())
    {
        QMessageBox::warning(this, "Thiếu thông tin", "Vui lòng nhập số điện thoại!");
        phoneEdit->setFocus();
        return false;
    }

    if (phone.length() < 10)
    {
        QMessageBox::warning(this, "Số điện thoại không hợp lệ", "Số điện thoại phải có ít nhất 10 số!");
        phoneEdit->setFocus();
        return false;
    }

    if (name.isEmpty())
    {
        QMessageBox::warning(this, "Thiếu thông tin", "Vui lòng nhập họ tên!");
        nameEdit->setFocus();
        return false;
    }

    // Kiểm tra SĐT đã tồn tại chưa
    HeThongQuanLy *system = HeThongQuanLy::getInstance();
    if (system->timKhachHangTheoSDT(phone.toStdString()) != nullptr)
    {
        QMessageBox::warning(this, "Số điện thoại đã tồn tại", 
                           "Số điện thoại này đã được đăng ký!\nVui lòng tìm kiếm trong danh sách.");
        phoneEdit->setFocus();
        return false;
    }

    return true;
}

void AddCustomerDialog::onAddClicked()
{
    if (!validateInput())
    {
        return;
    }

    QString phone = phoneEdit->text().trimmed();
    QString name = nameEdit->text().trimmed();
    QString address = addressEdit->text().trimmed();

    if (address.isEmpty())
    {
        address = "Chưa cập nhật";
    }

    // Tạo mã khách hàng mới
    HeThongQuanLy *system = HeThongQuanLy::getInstance();
    std::string newCode = system->layQuanLyKhachHang()->taoMaKhachHangMoi();

    // Tạo khách hàng mới
    newCustomer = new KhachHang(
        name.toStdString(),
        phone.toStdString(),
        address.toStdString(),
        newCode
    );

    // Đặt ngày đăng ký là hôm nay
    NgayThang today(15, 11, 2025);  // TODO: Lấy ngày hiện tại thực tế
    newCustomer->datNgayDangKy(today);

    accept();
}

void AddCustomerDialog::onCancelClicked()
{
    newCustomer = nullptr;
    reject();
}
