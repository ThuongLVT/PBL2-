#ifndef ACCOUNTPAGE_H
#define ACCOUNTPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QDate>
#include "Core/Models/QuanTriVien.h"
#include "Core/Models/NhanVien.h"

/**
 * @class AccountPage
 * @brief Page để hiển thị thông tin tài khoản người dùng hiện tại
 *
 * Chức năng:
 * - Hiển thị thông tin cá nhân (Họ tên, SĐT, Giới tính, Ngày sinh, Vai trò)
 * - Hiển thị thông tin đăng nhập (Tên đăng nhập - không sửa được)
 * - Nút chỉnh sửa thông tin cá nhân (mở dialog)
 * - Nút đổi mật khẩu (mở dialog)
 */
class AccountPage : public QWidget
{
    Q_OBJECT

public:
    explicit AccountPage(QWidget *parent = nullptr);
    ~AccountPage();

    /**
     * @brief Đặt người dùng hiện tại để hiển thị thông tin
     * @param user Pointer tới QuanTriVien hoặc NhanVien
     */
    void setCurrentUser(QuanTriVien *user);

private slots:
    void onEditPersonalInfoClicked();
    void onChangePasswordClicked();

private:
    void setupUI();
    void setupConnections();
    void loadUserData();

    // Current user
    QuanTriVien *m_currentUser;

    // Local storage for gender and DOB (not in ConNguoi model)
    QString m_currentGender;
    QString m_currentDOB;

    // Main layout
    QVBoxLayout *m_mainLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_scrollContent;

    // ===== Personal Information Section =====
    QGroupBox *m_personalInfoGroup;
    QLabel *m_nameLabel;
    QLineEdit *m_nameEdit;
    QLabel *m_phoneLabel;
    QLineEdit *m_phoneEdit;
    QLabel *m_genderLabel;
    QLineEdit *m_genderEdit;
    QLabel *m_dobLabel;
    QLineEdit *m_dobEdit;
    QLabel *m_roleLabel;
    QLineEdit *m_roleEdit;
    QPushButton *m_editPersonalInfoButton;

    // ===== Login Information Section =====
    QGroupBox *m_loginInfoGroup;
    QLabel *m_usernameLabel;
    QLineEdit *m_usernameEdit;
    QLabel *m_passwordLabel;
    QLineEdit *m_passwordEdit;
    QPushButton *m_togglePasswordButton;
    QPushButton *m_changePasswordButton;
};

#endif // ACCOUNTPAGE_H
