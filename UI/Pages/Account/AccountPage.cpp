#include "AccountPage.h"
#include "UI/Dialogs/EditPersonalInfoDialog.h"
#include "UI/Dialogs/ChangePasswordDialog.h"
#include "Core/QuanLy/HeThongQuanLy.h"
#include <QScrollArea>
#include <QGridLayout>
#include <QMessageBox>
#include <QFrame>

AccountPage::AccountPage(QWidget *parent)
    : QWidget(parent),
      m_currentUser(nullptr)
{
    setupUI();
    setupConnections();
}

AccountPage::~AccountPage()
{
}

void AccountPage::setupUI()
{
    // Main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // Scroll area for content
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setObjectName("AccountScrollArea");

    m_scrollContent = new QWidget();
    m_scrollContent->setObjectName("AccountScrollContent");
    QVBoxLayout *contentLayout = new QVBoxLayout(m_scrollContent);
    contentLayout->setContentsMargins(32, 32, 32, 32);
    contentLayout->setSpacing(24);

    // ===== Personal Information Section =====
    m_personalInfoGroup = new QGroupBox("Thông Tin Cá Nhân");
    m_personalInfoGroup->setObjectName("PersonalInfoGroup");
    m_personalInfoGroup->setStyleSheet("QGroupBox { font-size: 20px; font-weight: bold; color: #1a1a1a; }");
    QGridLayout *personalLayout = new QGridLayout();
    personalLayout->setSpacing(16);
    personalLayout->setColumnStretch(1, 1);

    int row = 0;

    // Name
    m_nameLabel = new QLabel("Họ và Tên");
    m_nameLabel->setObjectName("FieldLabel");
    m_nameEdit = new QLineEdit();
    m_nameEdit->setObjectName("AccountLineEdit");
    m_nameEdit->setReadOnly(true);
    personalLayout->addWidget(m_nameLabel, row, 0);
    personalLayout->addWidget(m_nameEdit, row, 1);
    row++;

    // Phone
    m_phoneLabel = new QLabel("Số Điện Thoại");
    m_phoneLabel->setObjectName("FieldLabel");
    m_phoneEdit = new QLineEdit();
    m_phoneEdit->setObjectName("AccountLineEdit");
    m_phoneEdit->setReadOnly(true);
    personalLayout->addWidget(m_phoneLabel, row, 0);
    personalLayout->addWidget(m_phoneEdit, row, 1);
    row++;

    // Gender
    m_genderLabel = new QLabel("Giới Tính");
    m_genderLabel->setObjectName("FieldLabel");
    m_genderEdit = new QLineEdit();
    m_genderEdit->setObjectName("AccountLineEdit");
    m_genderEdit->setReadOnly(true);
    personalLayout->addWidget(m_genderLabel, row, 0);
    personalLayout->addWidget(m_genderEdit, row, 1);
    row++;

    // Date of Birth
    m_dobLabel = new QLabel("Ngày Sinh");
    m_dobLabel->setObjectName("FieldLabel");
    m_dobEdit = new QLineEdit();
    m_dobEdit->setObjectName("AccountLineEdit");
    m_dobEdit->setReadOnly(true);
    personalLayout->addWidget(m_dobLabel, row, 0);
    personalLayout->addWidget(m_dobEdit, row, 1);
    row++;

    // Role
    m_roleLabel = new QLabel("Vai Trò");
    m_roleLabel->setObjectName("FieldLabel");
    m_roleEdit = new QLineEdit();
    m_roleEdit->setObjectName("AccountLineEdit");
    m_roleEdit->setReadOnly(true);
    personalLayout->addWidget(m_roleLabel, row, 0);
    personalLayout->addWidget(m_roleEdit, row, 1);
    row++;

    // Edit button
    m_editPersonalInfoButton = new QPushButton("Chỉnh sửa thông tin");
    m_editPersonalInfoButton->setObjectName("PrimaryButton");
    m_editPersonalInfoButton->setCursor(Qt::PointingHandCursor);
    personalLayout->addWidget(m_editPersonalInfoButton, row, 1);

    m_personalInfoGroup->setLayout(personalLayout);
    contentLayout->addWidget(m_personalInfoGroup);

    // ===== Login Information Section =====
    m_loginInfoGroup = new QGroupBox("Thông Tin Đăng Nhập");
    m_loginInfoGroup->setObjectName("LoginInfoGroup");
    m_loginInfoGroup->setStyleSheet("QGroupBox { font-size: 20px; font-weight: bold; color: #1a1a1a; }");
    QGridLayout *loginLayout = new QGridLayout();
    loginLayout->setSpacing(16);
    loginLayout->setColumnStretch(1, 1);

    row = 0;

    // Username (read-only)
    m_usernameLabel = new QLabel("Tên Đăng Nhập");
    m_usernameLabel->setObjectName("FieldLabel");
    m_usernameEdit = new QLineEdit();
    m_usernameEdit->setObjectName("AccountLineEdit");
    m_usernameEdit->setReadOnly(true);
    loginLayout->addWidget(m_usernameLabel, row, 0);
    loginLayout->addWidget(m_usernameEdit, row, 1);
    row++;

    // Password with eye icon
    m_passwordLabel = new QLabel("Mật Khẩu");
    m_passwordLabel->setObjectName("FieldLabel");

    QWidget *passwordContainer = new QWidget();
    passwordContainer->setStyleSheet("background: transparent;"); // Bỏ nền xám
    QHBoxLayout *passwordLayout = new QHBoxLayout(passwordContainer);
    passwordLayout->setContentsMargins(0, 0, 0, 0);
    passwordLayout->setSpacing(8);

    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setObjectName("AccountLineEdit");
    m_passwordEdit->setReadOnly(true);
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    m_togglePasswordButton = new QPushButton();
    m_togglePasswordButton->setIcon(QIcon(":/icons/eye-off.svg"));
    m_togglePasswordButton->setIconSize(QSize(24, 24));
    m_togglePasswordButton->setFixedSize(32, 32);
    m_togglePasswordButton->setCursor(Qt::PointingHandCursor);
    m_togglePasswordButton->setFlat(true);
    m_togglePasswordButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "    background: transparent;"
        "    padding: 0px;"
        "}"
        "QPushButton:hover {"
        "    background: rgba(0, 0, 0, 0.05);"
        "    border-radius: 4px;"
        "}");

    passwordLayout->addWidget(m_passwordEdit, 1);
    passwordLayout->addWidget(m_togglePasswordButton, 0, Qt::AlignVCenter);

    loginLayout->addWidget(m_passwordLabel, row, 0);
    loginLayout->addWidget(passwordContainer, row, 1);
    row++;

    // Change password button (green like edit info button)
    m_changePasswordButton = new QPushButton("Đổi mật khẩu");
    m_changePasswordButton->setObjectName("PrimaryButton");
    m_changePasswordButton->setCursor(Qt::PointingHandCursor);
    loginLayout->addWidget(m_changePasswordButton, row, 1);

    m_loginInfoGroup->setLayout(loginLayout);
    contentLayout->addWidget(m_loginInfoGroup);

    contentLayout->addStretch();

    m_scrollArea->setWidget(m_scrollContent);
    m_mainLayout->addWidget(m_scrollArea);
}

void AccountPage::setupConnections()
{
    connect(m_editPersonalInfoButton, &QPushButton::clicked, this, &AccountPage::onEditPersonalInfoClicked);
    connect(m_changePasswordButton, &QPushButton::clicked, this, &AccountPage::onChangePasswordClicked);
    connect(m_togglePasswordButton, &QPushButton::clicked, this, [this]()
            {
        if (m_passwordEdit->echoMode() == QLineEdit::Password) {
            m_passwordEdit->setEchoMode(QLineEdit::Normal);
            m_togglePasswordButton->setIcon(QIcon(":/icons/eye.svg"));
        } else {
            m_passwordEdit->setEchoMode(QLineEdit::Password);
            m_togglePasswordButton->setIcon(QIcon(":/icons/eye-off.svg"));
        } });
}

void AccountPage::setCurrentUser(NguoiDung *user)
{
    m_currentUser = user;
    loadUserData();
}

void AccountPage::loadUserData()
{
    if (!m_currentUser)
        return;

    m_nameEdit->setText(QString::fromStdString(m_currentUser->layHoTen()));
    m_phoneEdit->setText(QString::fromStdString(m_currentUser->laySoDienThoai()));

    // Load gender and DOB from model
    m_genderEdit->setText(QString::fromStdString(m_currentUser->layGioiTinh()));
    m_dobEdit->setText(QString::fromStdString(m_currentUser->layNgaySinh()));

    m_usernameEdit->setText(QString::fromStdString(m_currentUser->layTenDangNhap()));
    m_passwordEdit->setText(QString::fromStdString(m_currentUser->layMatKhau()));

    // Set role based on user type
    if (m_currentUser->layVaiTro() == VaiTro::QUAN_TRI_VIEN)
    {
        m_roleEdit->setText("Quản Trị Viên");
    }
    else
    {
        m_roleEdit->setText("Nhân Viên");
    }
}

void AccountPage::onEditPersonalInfoClicked()
{
    if (!m_currentUser)
        return;

    EditPersonalInfoDialog dialog(this);

    // Set current values from model
    dialog.setName(QString::fromStdString(m_currentUser->layHoTen()));
    dialog.setPhone(QString::fromStdString(m_currentUser->laySoDienThoai()));
    dialog.setGender(QString::fromStdString(m_currentUser->layGioiTinh()));
    dialog.setDateOfBirth(QString::fromStdString(m_currentUser->layNgaySinh()));

    if (dialog.exec() == QDialog::Accepted)
    {
        // Get new values
        QString name = dialog.getName();
        QString phone = dialog.getPhone();
        QString gender = dialog.getGender();
        QString dob = dialog.getDateOfBirth();

        // Update user object (now includes gender and DOB in NguoiDung model)
        m_currentUser->datHoTen(name.toStdString());
        m_currentUser->datSoDienThoai(phone.toStdString());
        m_currentUser->datGioiTinh(gender.toStdString());
        m_currentUser->datNgaySinh(dob.toStdString());

        // Save to CSV (admin to admin.csv, staff to nhanvien.csv)
        HeThongQuanLy *hethong = HeThongQuanLy::getInstance();
        QuanLyNhanVien *staffMgr = hethong->layQuanLyNhanVien();

        if (m_currentUser->layVaiTro() == VaiTro::QUAN_TRI_VIEN)
        {
            // Save admin to admin.csv
            staffMgr->luuAdminCSV("admin.csv");
        }
        else
        {
            // Save staff to nhanvien.csv
            hethong->luuNhanVienCSV("nhanvien.csv");
        }

        // Save to binary file
        if (hethong->luuHeThong("D:/PBL2-/Data/data.bin"))
        {
            QMessageBox::information(this, "Thành công", "Đã cập nhật thông tin cá nhân!");
            loadUserData(); // Refresh display
        }
        else
        {
            QMessageBox::warning(this, "Lỗi", "Không thể lưu thông tin!");
        }
    }
}

void AccountPage::onChangePasswordClicked()
{
    if (!m_currentUser)
        return;

    ChangePasswordDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        QString oldPassword = dialog.getOldPassword();
        QString newPassword = dialog.getNewPassword();

        // Verify old password
        if (QString::fromStdString(m_currentUser->layMatKhau()) != oldPassword)
        {
            QMessageBox::warning(this, "Lỗi", "Mật khẩu hiện tại không đúng!");
            return;
        }

        // Change password
        m_currentUser->doiMatKhau(oldPassword.toStdString(), newPassword.toStdString());

        // Save to file
        HeThongQuanLy *hethong = HeThongQuanLy::getInstance();
        if (hethong->luuHeThong("D:/PBL2-/Data/data.bin"))
        {
            QMessageBox::information(this, "Thành công", "Đã đổi mật khẩu thành công!");
            loadUserData(); // Refresh display with new password
        }
        else
        {
            QMessageBox::warning(this, "Lỗi", "Không thể lưu mật khẩu mới!");
        }
    }
}
