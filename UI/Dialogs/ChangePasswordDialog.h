#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

/**
 * @class ChangePasswordDialog
 * @brief Dialog để đổi mật khẩu với 3 trường input và toggle visibility
 */
class ChangePasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePasswordDialog(QWidget *parent = nullptr);
    ~ChangePasswordDialog();

    QString getOldPassword() const;
    QString getNewPassword() const;
    QString getConfirmPassword() const;

private slots:
    void onToggleOldPasswordVisibility();
    void onToggleNewPasswordVisibility();
    void onToggleConfirmPasswordVisibility();
    void onConfirmClicked();
    void onCancelClicked();

private:
    void setupUI();
    void setupConnections();

    // Input fields
    QLineEdit *m_oldPasswordEdit;
    QLineEdit *m_newPasswordEdit;
    QLineEdit *m_confirmPasswordEdit;

    // Toggle visibility buttons
    QPushButton *m_toggleOldPasswordButton;
    QPushButton *m_toggleNewPasswordButton;
    QPushButton *m_toggleConfirmPasswordButton;

    // Action buttons
    QPushButton *m_confirmButton;
    QPushButton *m_cancelButton;

    // State flags
    bool m_isOldPasswordVisible;
    bool m_isNewPasswordVisible;
    bool m_isConfirmPasswordVisible;
};

#endif // CHANGEPASSWORDDIALOG_H
