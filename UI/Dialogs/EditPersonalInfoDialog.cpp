#include "EditPersonalInfoDialog.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDate>

EditPersonalInfoDialog::EditPersonalInfoDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setupConnections();

    setWindowTitle("Chỉnh sửa thông tin cá nhân");
    setModal(true);
    setFixedWidth(520);
}

EditPersonalInfoDialog::~EditPersonalInfoDialog()
{
}

void EditPersonalInfoDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Title
    QLabel *titleLabel = new QLabel("Chỉnh sửa thông tin cá nhân");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #1a1a1a;");
    mainLayout->addWidget(titleLabel);

    // Form layout
    QGridLayout *formLayout = new QGridLayout();
    formLayout->setSpacing(15);
    formLayout->setColumnStretch(1, 1);

    int row = 0;

    // Name
    QLabel *nameLabel = new QLabel("Họ và Tên");
    nameLabel->setStyleSheet("font-size: 14px; color: #333333; font-weight: 500;");
    formLayout->addWidget(nameLabel, row, 0, Qt::AlignLeft);
    row++;

    m_nameEdit = new QLineEdit();
    m_nameEdit->setPlaceholderText("Nhập họ và tên");
    m_nameEdit->setStyleSheet(
        "QLineEdit {"
        "    border: 1px solid #dcdcdc;"
        "    border-radius: 8px;"
        "    padding: 12px;"
        "    font-size: 14px;"
        "    background: white;"
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid #4CAF50;"
        "}");
    formLayout->addWidget(m_nameEdit, row, 0, 1, 2);
    row++;

    // Phone
    QLabel *phoneLabel = new QLabel("Số điện thoại");
    phoneLabel->setStyleSheet("font-size: 14px; color: #333333; font-weight: 500;");
    formLayout->addWidget(phoneLabel, row, 0, Qt::AlignLeft);
    row++;

    m_phoneEdit = new QLineEdit();
    m_phoneEdit->setPlaceholderText("Nhập số điện thoại");
    m_phoneEdit->setStyleSheet(
        "QLineEdit {"
        "    border: 1px solid #dcdcdc;"
        "    border-radius: 8px;"
        "    padding: 12px;"
        "    font-size: 14px;"
        "    background: white;"
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid #4CAF50;"
        "}");
    formLayout->addWidget(m_phoneEdit, row, 0, 1, 2);
    row++;

    // Gender
    QLabel *genderLabel = new QLabel("Giới tính");
    genderLabel->setStyleSheet("font-size: 14px; color: #333333; font-weight: 500;");
    formLayout->addWidget(genderLabel, row, 0, Qt::AlignLeft);
    row++;

    QWidget *genderWidget = new QWidget();
    genderWidget->setStyleSheet("background: transparent;"); // Bỏ nền xám
    QHBoxLayout *genderLayout = new QHBoxLayout(genderWidget);
    genderLayout->setContentsMargins(0, 0, 0, 0);
    genderLayout->setSpacing(0); // Equal spacing

    m_genderGroup = new QButtonGroup(this);

    m_maleRadio = new QRadioButton("Nam");
    m_maleRadio->setStyleSheet(
        "QRadioButton {"
        "    font-size: 14px;"
        "    color: #333333;"
        "    spacing: 8px;"
        "}"
        "QRadioButton::indicator {"
        "    width: 18px;"
        "    height: 18px;"
        "}"
        "QRadioButton::indicator:unchecked {"
        "    border: 2px solid #dcdcdc;"
        "    border-radius: 9px;"
        "    background: white;"
        "}"
        "QRadioButton::indicator:checked {"
        "    border: 2px solid #4CAF50;"
        "    border-radius: 9px;"
        "    background: #4CAF50;"
        "}");
    m_genderGroup->addButton(m_maleRadio, 0);

    m_femaleRadio = new QRadioButton("Nữ");
    m_femaleRadio->setStyleSheet(m_maleRadio->styleSheet());
    m_genderGroup->addButton(m_femaleRadio, 1);

    m_otherRadio = new QRadioButton("Khác");
    m_otherRadio->setStyleSheet(m_maleRadio->styleSheet());
    m_genderGroup->addButton(m_otherRadio, 2);

    // Equal spacing between 3 radio buttons
    genderLayout->addStretch(1);
    genderLayout->addWidget(m_maleRadio);
    genderLayout->addStretch(1);
    genderLayout->addWidget(m_femaleRadio);
    genderLayout->addStretch(1);
    genderLayout->addWidget(m_otherRadio);
    genderLayout->addStretch(1);

    formLayout->addWidget(genderWidget, row, 0, 1, 2);
    row++;

    // Date of Birth
    QLabel *dobLabel = new QLabel("Ngày sinh");
    dobLabel->setStyleSheet("font-size: 14px; color: #333333; font-weight: 500;");
    formLayout->addWidget(dobLabel, row, 0, Qt::AlignLeft);
    row++;

    // Date picker with calendar icon inside
    QWidget *dobWidget = new QWidget();
    dobWidget->setStyleSheet("background: transparent;"); // No gray background
    QHBoxLayout *dobLayout = new QHBoxLayout(dobWidget);
    dobLayout->setContentsMargins(0, 0, 0, 0);
    dobLayout->setSpacing(0);

    m_dobEdit = new QLineEdit();
    m_dobEdit->setPlaceholderText("dd/MM/yyyy");
    m_dobEdit->setStyleSheet(
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

    dobLayout->addWidget(m_dobEdit);

    formLayout->addWidget(dobWidget, row, 0, 1, 2);

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

    m_confirmButton = new QPushButton("Lưu");
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

void EditPersonalInfoDialog::setupConnections()
{
    connect(m_confirmButton, &QPushButton::clicked, this, &EditPersonalInfoDialog::onConfirmClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &EditPersonalInfoDialog::onCancelClicked);
}

void EditPersonalInfoDialog::onConfirmClicked()
{
    QString name = m_nameEdit->text().trimmed();
    QString phone = m_phoneEdit->text().trimmed();
    QString dob = m_dobEdit->text().trimmed();

    // Validation
    if (name.isEmpty())
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập họ và tên!");
        return;
    }

    if (phone.isEmpty())
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập số điện thoại!");
        return;
    }

    // Validate phone number (10-11 digits)
    QRegularExpression phoneRegex("^\\d{10,11}$");
    if (!phoneRegex.match(phone).hasMatch())
    {
        QMessageBox::warning(this, "Lỗi", "Số điện thoại phải có 10-11 chữ số!");
        return;
    }

    if (m_genderGroup->checkedButton() == nullptr)
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn giới tính!");
        return;
    }

    // Validate date of birth format (dd/MM/yyyy)
    if (dob.isEmpty())
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập ngày sinh!");
        return;
    }

    QRegularExpression dobRegex("^(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/\\d{4}$");
    if (!dobRegex.match(dob).hasMatch())
    {
        QMessageBox::warning(this, "Lỗi", "Ngày sinh phải có định dạng dd/MM/yyyy (ví dụ: 15/03/1990)!");
        return;
    }

    // Validate date is valid
    QStringList parts = dob.split('/');
    if (parts.size() == 3)
    {
        int day = parts[0].toInt();
        int month = parts[1].toInt();
        int year = parts[2].toInt();

        QDate date(year, month, day);
        if (!date.isValid())
        {
            QMessageBox::warning(this, "Lỗi", "Ngày sinh không hợp lệ!");
            return;
        }

        // Check if date is not in the future
        if (date > QDate::currentDate())
        {
            QMessageBox::warning(this, "Lỗi", "Ngày sinh không thể ở tương lai!");
            return;
        }
    }

    accept(); // Close dialog with Accepted result
}

void EditPersonalInfoDialog::onCancelClicked()
{
    reject(); // Close dialog with Rejected result
}

// Setters
void EditPersonalInfoDialog::setName(const QString &name)
{
    m_nameEdit->setText(name);
}

void EditPersonalInfoDialog::setPhone(const QString &phone)
{
    m_phoneEdit->setText(phone);
}

void EditPersonalInfoDialog::setGender(const QString &gender)
{
    if (gender == "Nam")
        m_maleRadio->setChecked(true);
    else if (gender == "Nữ")
        m_femaleRadio->setChecked(true);
    else
        m_otherRadio->setChecked(true);
}

void EditPersonalInfoDialog::setDateOfBirth(const QString &dob)
{
    m_dobEdit->setText(dob);
}

// Getters
QString EditPersonalInfoDialog::getName() const
{
    return m_nameEdit->text();
}

QString EditPersonalInfoDialog::getPhone() const
{
    return m_phoneEdit->text();
}

QString EditPersonalInfoDialog::getGender() const
{
    if (m_maleRadio->isChecked())
        return "Nam";
    else if (m_femaleRadio->isChecked())
        return "Nữ";
    else
        return "Khác";
}

QString EditPersonalInfoDialog::getDateOfBirth() const
{
    return m_dobEdit->text();
}
