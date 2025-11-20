#include "Sidebar.h"
#include <QIcon>
#include <QFont>
#include <QFrame>
#include <QShortcut>
#include <QKeySequence>
#include <QStyle>

Sidebar::Sidebar(QWidget *parent)
    : QWidget(parent), m_isExpanded(false), m_isAdmin(true), m_currentMenuIndex(0), m_isAccountActive(false), m_mainLayout(nullptr), m_menuLayout(nullptr), m_toggleButton(nullptr), m_brandLabel(nullptr), m_toggleWidget(nullptr), m_toggleLayout(nullptr), m_menuButtonGroup(nullptr), m_bookingButton(nullptr), m_paymentButton(nullptr), m_fieldButton(nullptr), m_customerButton(nullptr), m_serviceButton(nullptr), m_staffButton(nullptr), m_statisticsButton(nullptr), m_adminButton(nullptr), m_logoutButton(nullptr), m_widthAnimation(nullptr)
{
    setupUI();

    // Start collapsed - use min/max width for animation
    setMinimumWidth(COLLAPSED_WIDTH);
    setMaximumWidth(COLLAPSED_WIDTH);

    // Set property for CSS styling
    setProperty("collapsed", true);
    style()->unpolish(this);
    style()->polish(this);
}

Sidebar::~Sidebar()
{
}

void Sidebar::setupUI()
{
    setObjectName("Sidebar");

    // Main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // Create 3 sections
    createToggleSection();
    createMenuSection();
    createAdminSection();

    setLayout(m_mainLayout);

    // Setup animation
    m_widthAnimation = new QPropertyAnimation(this, "maximumWidth");
    m_widthAnimation->setDuration(300);
    m_widthAnimation->setEasingCurve(QEasingCurve::InOutCubic);

    // Keyboard shortcuts
    QShortcut *toggleShortcut = new QShortcut(QKeySequence("Ctrl+M"), this);
    connect(toggleShortcut, &QShortcut::activated, this, &Sidebar::onToggleButtonClicked);
}

void Sidebar::createToggleSection()
{
    // Toggle section widget (height: 60px - same as Header)
    m_toggleWidget = new QWidget(this);
    m_toggleWidget->setObjectName("ToggleSection");
    m_toggleWidget->setFixedHeight(80);

    m_toggleLayout = new QHBoxLayout(m_toggleWidget);
    m_toggleLayout->setContentsMargins(8, 0, 0, 0); // 8px left, 0px right - nh�ch BrandLabel qua tr�i
    m_toggleLayout->setSpacing(1);                  // Gi?m spacing d? BrandLabel g?n toggle button hon

    // Toggle button (hamburger menu)
    m_toggleButton = new QPushButton(this);
    m_toggleButton->setObjectName("ToggleButton");
    m_toggleButton->setFixedSize(54, 54); // Square button for centering
    m_toggleButton->setIcon(QIcon(":/icons/menu.svg"));
    m_toggleButton->setIconSize(QSize(24, 24));
    m_toggleButton->setCursor(Qt::PointingHandCursor);
    m_toggleButton->setFlat(true);
    // m_toggleButton->setToolTip("Thu g?n/M? r?ng menu"); // Tooltip removed
    connect(m_toggleButton, &QPushButton::clicked, this, &Sidebar::onToggleButtonClicked);

    // Brand label
    m_brandLabel = new QLabel("SÂN BÓNG DUT", this);
    m_brandLabel->setObjectName("BrandLabel");
    QFont brandFont;
    brandFont.setFamily("Segoe UI");
    brandFont.setPointSize(16);        // Increased from 14 to 16
    brandFont.setWeight(QFont::Black); // Extra bold (900)
    m_brandLabel->setFont(brandFont);
    m_brandLabel->setVisible(false); // Hidden when collapsed

    m_toggleLayout->addWidget(m_toggleButton);
    m_toggleLayout->addWidget(m_brandLabel);
    m_toggleLayout->addStretch();

    m_mainLayout->addWidget(m_toggleWidget);

    // Divider line
    QFrame *divider1 = new QFrame(this);
    divider1->setObjectName("Divider");
    divider1->setFrameShape(QFrame::HLine);
    divider1->setFixedHeight(1);
    m_mainLayout->addWidget(divider1);
}

void Sidebar::createMenuSection()
{
    // Menu section
    QWidget *menuWidget = new QWidget(this);
    menuWidget->setObjectName("MenuSection");

    m_menuLayout = new QVBoxLayout(menuWidget);
    m_menuLayout->setContentsMargins(0, 16, 0, 8);
    m_menuLayout->setSpacing(4);

    // Button group for exclusive selection
    m_menuButtonGroup = new QButtonGroup(this);
    m_menuButtonGroup->setExclusive(true);

    // Menu items configuration
    struct MenuItem
    {
        QString text;
        QString iconPath;
        QPushButton **button;
    };

    MenuItem menuItems[] = {
        {"   Đặt Sân", ":/icons/booking.svg", &m_bookingButton},
        {"   Hoá Đơn", ":/icons/payment.svg", &m_paymentButton},
        {"   Sân Bóng", ":/icons/field.svg", &m_fieldButton},
        {"   Khách Hàng", ":/icons/customer.svg", &m_customerButton},
        {"   Dịch Vụ", ":/icons/service.svg", &m_serviceButton},
        {"   Nhân Viên", ":/icons/staff.svg", &m_staffButton},
        {"   Thống Kê", ":/icons/statistics.svg", &m_statisticsButton}};

    for (int i = 0; i < 7; ++i)
    {
        QPushButton *button = new QPushButton(this);
        button->setObjectName("MenuButton");
        button->setText(""); // Start with empty text (collapsed state)
        button->setIcon(QIcon(menuItems[i].iconPath));
        button->setIconSize(QSize(24, 24));
        button->setCheckable(true);
        button->setCursor(Qt::PointingHandCursor);
        button->setFlat(true);
        button->setMinimumHeight(48);
        button->setProperty("menuIndex", i);
        button->setProperty("fullText", menuItems[i].text); // Store full text

        // Tooltip removed

        connect(button, &QPushButton::clicked, this, &Sidebar::onMenuButtonClicked);

        m_menuButtonGroup->addButton(button, i);
        m_menuLayout->addWidget(button);

        *(menuItems[i].button) = button;
    }

    // Set first button as checked
    m_bookingButton->setChecked(true);

    m_mainLayout->addWidget(menuWidget);

    // Add spacer widget with green background to push admin section to bottom
    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setObjectName("MenuSection"); // Use same style as menu
    m_mainLayout->addWidget(spacerWidget, 1);   // Stretch factor 1

    // Divider line before admin section
    QFrame *divider2 = new QFrame(this);
    divider2->setObjectName("Divider");
    divider2->setFrameShape(QFrame::HLine);
    divider2->setFixedHeight(1);
    m_mainLayout->addWidget(divider2);
}
void Sidebar::createAdminSection()
{
    // Admin section container with background
    QWidget *adminWidget = new QWidget(this);
    adminWidget->setObjectName("AdminSection");

    QVBoxLayout *adminLayout = new QVBoxLayout(adminWidget);
    adminLayout->setContentsMargins(0, 8, 0, 10); // Bottom margin 10px
    adminLayout->setSpacing(4);

    // Admin/Account button
    m_adminButton = new QPushButton(adminWidget);
    m_adminButton->setObjectName("AdminButton");
    m_adminButton->setText(""); // Start with empty text (collapsed state)
    m_adminButton->setIcon(QIcon(":/icons/admin.svg"));
    m_adminButton->setIconSize(QSize(24, 24));
    m_adminButton->setCursor(Qt::PointingHandCursor);
    m_adminButton->setFlat(true);
    m_adminButton->setCheckable(true); // Make it checkable like menu buttons
    m_adminButton->setMinimumHeight(48);
    connect(m_adminButton, &QPushButton::clicked, this, &Sidebar::onAdminButtonClicked);

    // Logout button
    m_logoutButton = new QPushButton(adminWidget);
    m_logoutButton->setObjectName("LogoutButton");
    m_logoutButton->setText(""); // Start with empty text (collapsed state)
    m_logoutButton->setIcon(QIcon(":/icons/logout.svg"));
    m_logoutButton->setIconSize(QSize(24, 24));
    m_logoutButton->setCursor(Qt::PointingHandCursor);
    m_logoutButton->setFlat(true);
    m_logoutButton->setMinimumHeight(48);
    connect(m_logoutButton, &QPushButton::clicked, this, &Sidebar::onLogoutButtonClicked);

    adminLayout->addWidget(m_adminButton);
    adminLayout->addWidget(m_logoutButton);

    m_mainLayout->addWidget(adminWidget);
}

void Sidebar::onToggleButtonClicked()
{
    m_isExpanded = !m_isExpanded;

    int targetWidth = m_isExpanded ? EXPANDED_WIDTH : COLLAPSED_WIDTH;

    // Animate both minimum and maximum width for smooth transition
    m_widthAnimation->setStartValue(width());
    m_widthAnimation->setEndValue(targetWidth);
    m_widthAnimation->start();

    // Also set minimum width to match
    setMinimumWidth(targetWidth);

    // Update brand label visibility
    m_brandLabel->setVisible(m_isExpanded);

    // Update button text visibility and alignment
    for (QAbstractButton *button : m_menuButtonGroup->buttons())
    {
        QPushButton *menuButton = qobject_cast<QPushButton *>(button);
        if (menuButton)
        {
            if (m_isExpanded)
            {
                // Show text when expanded
                QString fullText = menuButton->property("fullText").toString();
                menuButton->setText(fullText);
            }
            else
            {
                // Hide text when collapsed
                menuButton->setText("");
            }
        }
    }

    // Update admin and logout button text
    if (m_isExpanded)
    {
        m_adminButton->setText("   Tài Khoản");
        m_logoutButton->setText("   Đăng Xuất");
    }
    else
    {
        m_adminButton->setText("");
        m_logoutButton->setText("");
    }

    // Update property for CSS styling
    setProperty("collapsed", !m_isExpanded);
    style()->unpolish(this);
    style()->polish(this);
}

void Sidebar::onMenuButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button)
    {
        int index = button->property("menuIndex").toInt();
        m_currentMenuIndex = index;
        m_isAccountActive = false; // Deactivate account when menu is clicked
        updateButtonStates();
        emit menuItemClicked(index);
    }
}

void Sidebar::onAdminButtonClicked()
{
    // Admin/Account button clicked - emit signal to show account page
    m_isAccountActive = true;
    updateButtonStates();
    emit adminClicked();
}

void Sidebar::onLogoutButtonClicked()
{
    emit logoutClicked();
}

void Sidebar::setAccountActive(bool active)
{
    m_isAccountActive = active;
    updateButtonStates();
}

void Sidebar::setUserRole(bool isAdmin)
{
    m_isAdmin = isAdmin;

    // Show/hide admin-only menu items
    m_staffButton->setVisible(isAdmin);
    m_statisticsButton->setVisible(isAdmin);
}

int Sidebar::currentMenuIndex() const
{
    return m_currentMenuIndex;
}

void Sidebar::updateButtonStates()
{
    // First, explicitly uncheck ALL menu buttons
    for (QAbstractButton *button : m_menuButtonGroup->buttons())
    {
        QPushButton *menuButton = qobject_cast<QPushButton *>(button);
        if (menuButton)
        {
            menuButton->setChecked(false);
        }
    }

    // Then update menu button states based on current state
    for (QAbstractButton *button : m_menuButtonGroup->buttons())
    {
        QPushButton *menuButton = qobject_cast<QPushButton *>(button);
        if (menuButton)
        {
            int index = menuButton->property("menuIndex").toInt();
            bool shouldBeChecked = !m_isAccountActive && index == m_currentMenuIndex;
            if (shouldBeChecked)
            {
                menuButton->setChecked(true);
            }
            // Force style update
            menuButton->setProperty("checked", shouldBeChecked);
            menuButton->style()->unpolish(menuButton);
            menuButton->style()->polish(menuButton);
        }
    }

    // Update account button state
    if (m_adminButton)
    {
        m_adminButton->setChecked(m_isAccountActive);
        m_adminButton->setProperty("checked", m_isAccountActive);
        m_adminButton->style()->unpolish(m_adminButton);
        m_adminButton->style()->polish(m_adminButton);
    }
}
