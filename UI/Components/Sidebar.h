#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include <QMenu>
#include <QButtonGroup>

class Sidebar : public QWidget
{
    Q_OBJECT

public:
    explicit Sidebar(QWidget *parent = nullptr);
    ~Sidebar();

    void setUserRole(bool isAdmin);
    int currentMenuIndex() const;
    void setAccountActive(bool active); // New method to highlight account button

signals:
    void menuItemClicked(int index);
    void adminClicked();
    void logoutClicked();

private slots:
    void onToggleButtonClicked();
    void onMenuButtonClicked();
    void onAdminButtonClicked();
    void onLogoutButtonClicked();

private:
    void setupUI();
    void createToggleSection();
    void createMenuSection();
    void createAdminSection();
    void updateButtonStates();

    // State
    bool m_isExpanded;
    bool m_isAdmin;
    int m_currentMenuIndex;
    bool m_isAccountActive; // Track if account page is active

    // Layouts
    QVBoxLayout *m_mainLayout;
    QVBoxLayout *m_menuLayout;

    // Toggle section
    QPushButton *m_toggleButton;
    QLabel *m_brandLabel;
    QWidget *m_toggleWidget;
    QHBoxLayout *m_toggleLayout;

    // Menu buttons (7 items)
    QButtonGroup *m_menuButtonGroup;
    QPushButton *m_bookingButton;
    QPushButton *m_paymentButton;
    QPushButton *m_fieldButton;
    QPushButton *m_customerButton;
    QPushButton *m_serviceButton;
    QPushButton *m_staffButton;
    QPushButton *m_statisticsButton;

    // Admin section (Account + Logout)
    QPushButton *m_adminButton;
    QPushButton *m_logoutButton;

    // Animation
    QPropertyAnimation *m_widthAnimation;

    // Constants
    static const int COLLAPSED_WIDTH = 70;
    static const int EXPANDED_WIDTH = 260;
};

#endif // SIDEBAR_H
