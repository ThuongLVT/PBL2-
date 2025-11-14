#include "ChangePasswordDialog.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QTimer>
#include <QShowEvent>

ChangePasswordDialog::ChangePasswordDialog(QWidget *parent)
    : QDialog(parent),
      m_isOldPasswordVisible(false),
      m_isNewPasswordVisible(false),
      m_isConfirmPasswordVisible(false)
{
    setupUI();
    setupConnections();

    setWindowTitle("Đổi mật khẩu");
    setModal(true);
    setFixedWidth(520);
}

ChangePasswordDialog::~ChangePasswordDialog()
{
}

void ChangePasswordDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Title
    QLabel *titleLabel = new QLabel("Đổi mật khẩu");
    titleLabel->setObjectName("DialogTitle");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #1a1a1a;");
    mainLayout->addWidget(titleLabel);

    // Form layout
    QGridLayout *formLayout = new QGridLayout();
    formLayout->setSpacing(15);
    formLayout->setColumnStretch(1, 1);

    int row = 0;

    // Old password
    QLabel *oldPasswordLabel = new QLabel("Mật khẩu hiện tại");
    oldPasswordLabel->setStyleSheet("font-size: 14px; color: #333333; font-weight: 500;");
    formLayout->addWidget(oldPasswordLabel, row, 0, Qt::AlignLeft);
    row++;

    QWidget *oldPasswordContainer = new QWidget();
    oldPasswordContainer->setStyleSheet("background: transparent;"); // Bỏ nền xám
    QHBoxLayout *oldPasswordLayout = new QHBoxLayout(oldPasswordContainer);
    oldPasswordLayout->setContentsMargins(0, 0, 0, 0);
    oldPasswordLayout->setSpacing(8);

    m_oldPasswordEdit = new QLineEdit();
    m_oldPasswordEdit->setEchoMode(QLineEdit::Password);
    m_oldPasswordEdit->setPlaceholderText("Nhập mật khẩu hiện tại");
    m_oldPasswordEdit->setStyleSheet(
        "QLineEdit {"
        "    border: 1px solid #dcdcdc;"
        "    border-radius: 8px;"
        "    padding: 12px;"
        "    font-size: 14px;"
        "    background: white;"
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid #4CAF50;"
        "    padding: 11px;"
        "}");

    m_toggleOldPasswordButton = new QPushButton();
    m_toggleOldPasswordButton->setIcon(QIcon(":/icons/eye-off.svg"));
    m_toggleOldPasswordButton->setIconSize(QSize(24, 24));
    m_toggleOldPasswordButton->setFixedSize(32, 32);
    m_toggleOldPasswordButton->setCursor(Qt::PointingHandCursor);
    m_toggleOldPasswordButton->setFlat(true);
    m_toggleOldPasswordButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "    background: transparent;"
        "    padding: 0px;"
        "}"
        "QPushButton:hover {"
        "    background: rgba(0, 0, 0, 0.05);"
        "    border-radius: 4px;"
        "}");

    oldPasswordLayout->addWidget(m_oldPasswordEdit, 1);
    oldPasswordLayout->addWidget(m_toggleOldPasswordButton, 0, Qt::AlignVCenter);

    formLayout->addWidget(oldPasswordContainer, row, 0, 1, 2);
    row++;

    // New password
    QLabel *newPasswordLabel = new QLabel("Mật khẩu mới");
    newPasswordLabel->setStyleSheet("font-size: 14px; color: #333333; font-weight: 500;");
    formLayout->addWidget(newPasswordLabel, row, 0, Qt::AlignLeft);
    row++;

    QWidget *newPasswordContainer = new QWidget();
    newPasswordContainer->setStyleSheet("background: transparent;"); // Bỏ nền xám
    QHBoxLayout *newPasswordLayout = new QHBoxLayout(newPasswordContainer);
    newPasswordLayout->setContentsMargins(0, 0, 0, 0);
    newPasswordLayout->setSpacing(8);

    m_newPasswordEdit = new QLineEdit();
    m_newPasswordEdit->setEchoMode(QLineEdit::Password);
    m_newPasswordEdit->setPlaceholderText("Nhập mật khẩu mới");
    m_newPasswordEdit->setStyleSheet(
        "QLineEdit {"
        "    border: 1px solid #dcdcdc;"
        "    border-radius: 8px;"
        "    padding: 12px;"
        "    font-size: 14px;"
        "    background: white;"
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid #4CAF50;"
        "    padding: 11px;"
        "}");

    m_toggleNewPasswordButton = new QPushButton();
    m_toggleNewPasswordButton->setIcon(QIcon(":/icons/eye-off.svg"));
    m_toggleNewPasswordButton->setIconSize(QSize(24, 24));
    m_toggleNewPasswordButton->setFixedSize(32, 32);
    m_toggleNewPasswordButton->setCursor(Qt::PointingHandCursor);
    m_toggleNewPasswordButton->setFlat(true);
    m_toggleNewPasswordButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "    background: transparent;"
        "    padding: 0px;"
        "}"
        "QPushButton:hover {"
        "    background: rgba(0, 0, 0, 0.05);"
        "    border-radius: 4px;"
        "}");

    newPasswordLayout->addWidget(m_newPasswordEdit, 1);
    newPasswordLayout->addWidget(m_toggleNewPasswordButton, 0, Qt::AlignVCenter);

    formLayout->addWidget(newPasswordContainer, row, 0, 1, 2);
    row++;

    // Confirm password
    QLabel *confirmPasswordLabel = new QLabel("Xác nhận mật khẩu mới");
    confirmPasswordLabel->setStyleSheet("font-size: 14px; color: #333333; font-weight: 500;");
    formLayout->addWidget(confirmPasswordLabel, row, 0, Qt::AlignLeft);
    row++;

    QWidget *confirmPasswordContainer = new QWidget();
    confirmPasswordContainer->setStyleSheet("background: transparent;"); // Bỏ nền xám
    QHBoxLayout *confirmPasswordLayout = new QHBoxLayout(confirmPasswordContainer);
    confirmPasswordLayout->setContentsMargins(0, 0, 0, 0);
    confirmPasswordLayout->setSpacing(8);

    m_confirmPasswordEdit = new QLineEdit();
    m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    m_confirmPasswordEdit->setPlaceholderText("Nhập lại mật khẩu mới");
    m_confirmPasswordEdit->setStyleSheet(
        "QLineEdit {"
        "    border: 1px solid #dcdcdc;"
        "    border-radius: 8px;"
        "    padding: 12px;"
        "    font-size: 14px;"
        "    background: white;"
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid #4CAF50;"
        "    padding: 11px;"
        "}");

    m_toggleConfirmPasswordButton = new QPushButton();
    m_toggleConfirmPasswordButton->setIcon(QIcon(":/icons/eye-off.svg"));
    m_toggleConfirmPasswordButton->setIconSize(QSize(24, 24));
    m_toggleConfirmPasswordButton->setFixedSize(32, 32);
    m_toggleConfirmPasswordButton->setCursor(Qt::PointingHandCursor);
    m_toggleConfirmPasswordButton->setFlat(true);
    m_toggleConfirmPasswordButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "    background: transparent;"
        "    padding: 0px;"
        "}"
        "QPushButton:hover {"
        "    background: rgba(0, 0, 0, 0.05);"
        "    border-radius: 4px;"
        "}");

    confirmPasswordLayout->addWidget(m_confirmPasswordEdit, 1);
    confirmPasswordLayout->addWidget(m_toggleConfirmPasswordButton, 0, Qt::AlignVCenter);

    formLayout->addWidget(confirmPasswordContainer, row, 0, 1, 2);

    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(10);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);

    m_cancelButton = new QPushButton("Hủy");
    m_cancelButton->setFixedSize(120, 44);
    m_cancelButton->setCursor(Qt::PointingHandCursor);
    m_cancelButton->setStyleSheet(
        "QPushButton {"
        "    background: #f5f5f5;"
        "    border: none;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "    font-weight: 500;"
        "    color: #666666;"
        "}"
        "QPushButton:hover {"
        "    background: #e0e0e0;"
        "}");

    m_confirmButton = new QPushButton("Cập nhật");
    m_confirmButton->setFixedSize(120, 44);
    m_confirmButton->setCursor(Qt::PointingHandCursor);
    m_confirmButton->setStyleSheet(
        "QPushButton {"
        "    background: #4CAF50;"
        "    border: none;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "    font-weight: 500;"
        "    color: white;"
        "}"
        "QPushButton:hover {"
        "    background: #45a049;"
        "}"
        "QPushButton:pressed {"
        "    background: #3d8b40;"
        "}");

    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addWidget(m_confirmButton);

    mainLayout->addLayout(buttonLayout);
}

void ChangePasswordDialog::setupConnections()
{
    connect(m_toggleOldPasswordButton, &QPushButton::clicked, this, &ChangePasswordDialog::onToggleOldPasswordVisibility);
    connect(m_toggleNewPasswordButton, &QPushButton::clicked, this, &ChangePasswordDialog::onToggleNewPasswordVisibility);
    connect(m_toggleConfirmPasswordButton, &QPushButton::clicked, this, &ChangePasswordDialog::onToggleConfirmPasswordVisibility);
    connect(m_confirmButton, &QPushButton::clicked, this, &ChangePasswordDialog::onConfirmClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &ChangePasswordDialog::onCancelClicked);
}

void ChangePasswordDialog::onToggleOldPasswordVisibility()
{
    m_isOldPasswordVisible = !m_isOldPasswordVisible;
    m_oldPasswordEdit->setEchoMode(m_isOldPasswordVisible ? QLineEdit::Normal : QLineEdit::Password);
    m_toggleOldPasswordButton->setIcon(QIcon(m_isOldPasswordVisible ? ":/icons/eye.svg" : ":/icons/eye-off.svg"));
}

void ChangePasswordDialog::onToggleNewPasswordVisibility()
{
    m_isNewPasswordVisible = !m_isNewPasswordVisible;
    m_newPasswordEdit->setEchoMode(m_isNewPasswordVisible ? QLineEdit::Normal : QLineEdit::Password);
    m_toggleNewPasswordButton->setIcon(QIcon(m_isNewPasswordVisible ? ":/icons/eye.svg" : ":/icons/eye-off.svg"));
}

void ChangePasswordDialog::onToggleConfirmPasswordVisibility()
{
    m_isConfirmPasswordVisible = !m_isConfirmPasswordVisible;
    m_confirmPasswordEdit->setEchoMode(m_isConfirmPasswordVisible ? QLineEdit::Normal : QLineEdit::Password);
    m_toggleConfirmPasswordButton->setIcon(QIcon(m_isConfirmPasswordVisible ? ":/icons/eye.svg" : ":/icons/eye-off.svg"));
}

void ChangePasswordDialog::onConfirmClicked()
{
    QString oldPassword = m_oldPasswordEdit->text().trimmed();
    QString newPassword = m_newPasswordEdit->text().trimmed();
    QString confirmPassword = m_confirmPasswordEdit->text().trimmed();

    // Validation
    if (oldPassword.isEmpty() || newPassword.isEmpty() || confirmPassword.isEmpty())
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng điền đầy đủ thông tin!");
        return;
    }

    if (newPassword != confirmPassword)
    {
        QMessageBox::warning(this, "Lỗi", "Mật khẩu mới và xác nhận mật khẩu không khớp!");
        return;
    }

    if (newPassword.length() < 1)
    {
        QMessageBox::warning(this, "Lỗi", "Mật khẩu mới phải có ít nhất 1 ký tự!");
        return;
    }

    accept(); // Close dialog with Accepted result
}

void ChangePasswordDialog::onCancelClicked()
{
    reject(); // Close dialog with Rejected result
}

QString ChangePasswordDialog::getOldPassword() const
{
    return m_oldPasswordEdit->text();
}

QString ChangePasswordDialog::getNewPassword() const
{
    return m_newPasswordEdit->text();
}

QString ChangePasswordDialog::getConfirmPassword() const
{
    return m_confirmPasswordEdit->text();
}
