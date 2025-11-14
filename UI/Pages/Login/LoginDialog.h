#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QString>

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    QString getLoggedInUsername() const { return m_username; }
    bool isAdmin() const { return m_isAdmin; }

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onLoginClicked();
    void onCloseClicked();

private:
    void setupUI();
    void applyStyles();
    bool authenticate(const QString &username, const QString &password);

    // UI Components
    QLabel *m_titleLabel;
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QPushButton *m_loginButton;
    QPushButton *m_closeButton;

    // Authentication data
    QString m_username;
    bool m_isAdmin;
};

#endif // LOGINDIALOG_H
