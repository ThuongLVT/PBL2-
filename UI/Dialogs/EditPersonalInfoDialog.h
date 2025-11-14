#ifndef EDITPERSONALINFODIALOG_H
#define EDITPERSONALINFODIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>

/**
 * @class EditPersonalInfoDialog
 * @brief Dialog để sửa thông tin cá nhân (Họ tên, SĐT, Giới tính, Ngày sinh)
 */
class EditPersonalInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditPersonalInfoDialog(QWidget *parent = nullptr);
    ~EditPersonalInfoDialog();

    // Set initial values
    void setName(const QString &name);
    void setPhone(const QString &phone);
    void setGender(const QString &gender);
    void setDateOfBirth(const QString &dob);

    // Get values
    QString getName() const;
    QString getPhone() const;
    QString getGender() const;
    QString getDateOfBirth() const;

private slots:
    void onConfirmClicked();
    void onCancelClicked();

private:
    void setupUI();
    void setupConnections();

    // Input fields
    QLineEdit *m_nameEdit;
    QLineEdit *m_phoneEdit;

    // Gender radio buttons
    QRadioButton *m_maleRadio;
    QRadioButton *m_femaleRadio;
    QRadioButton *m_otherRadio;
    QButtonGroup *m_genderGroup;

    // Date of birth
    QLineEdit *m_dobEdit;

    // Action buttons
    QPushButton *m_confirmButton;
    QPushButton *m_cancelButton;
};

#endif // EDITPERSONALINFODIALOG_H
