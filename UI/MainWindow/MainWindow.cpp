#include "MainWindow.h"
#include "UI/Pages/Login/LoginDialog.h"
#include "Core/QuanLy/HeThongQuanLy.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QStatusBar>
#include <QApplication>

MainWindow::MainWindow(QuanTriVien *currentUser, QWidget *parent)
    : QMainWindow(parent), m_currentUser(currentUser), m_mainSplitter(nullptr), m_sidebar(nullptr), m_rightPanel(nullptr), m_header(nullptr), m_contentStack(nullptr), m_bookingPage(nullptr), m_paymentPage(nullptr), m_fieldPage(nullptr), m_customerPage(nullptr), m_servicePage(nullptr), m_staffPage(nullptr), m_statisticsPage(nullptr), m_accountPage(nullptr)
{
    // Setup page titles
    m_pageTitles << "Quản Lý Đặt Sân"
                 << "Quản Lý Thanh Toán"
                 << "Quản Lý Sân Bóng"
                 << "Quản Lý Khách Hàng"
                 << "Quản Lý Dịch Vụ"
                 << "Quản Lý Nhân Viên"
                 << "Quản Lý Thống Kê"
                 << "Quản Lý Tài Khoản";

    setupUI();
    setupConnections();

    // Set window properties
    setWindowTitle("Football Field Manager - DUT");
    setMinimumSize(1200, 700);
    resize(1400, 800);
}

MainWindow::~MainWindow()
{
    // Save system data when closing
    HeThongQuanLy *system = HeThongQuanLy::getInstance();
    if (system)
    {
        // Auto save system data
        system->luuHeThong("D:/QT_PBL2/Data/data.bin");
    }
}

void MainWindow::setupUI()
{
    // Central widget
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setObjectName("CentralWidget");
    setCentralWidget(centralWidget);

    // Main horizontal layout
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Create sidebar
    m_sidebar = new Sidebar(this);

    // Check user role and set sidebar permissions
    // For now, assume all users are admin
    // TODO: Implement proper role checking
    bool isAdmin = true; // m_currentUser->layLoai() == "Admin" or similar
    m_sidebar->setUserRole(isAdmin);

    // Create right panel
    m_rightPanel = new QWidget(this);
    m_rightPanel->setObjectName("RightPanel");
    QVBoxLayout *rightLayout = new QVBoxLayout(m_rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    // Create header
    m_header = new Header(this);
    m_header->setTitle(m_pageTitles[0]); // Default: Đặt Sân

    // Create content stack
    m_contentStack = new QStackedWidget(this);
    m_contentStack->setObjectName("ContentStack");

    // Create pages
    m_bookingPage = new BookingPage(this);
    m_paymentPage = new PaymentListPage(this);
    m_fieldPage = new FieldManagementPage(this);
    m_customerPage = new CustomerManagementPage(this);
    m_servicePage = new ServiceManagementPage(this);
    m_staffPage = new StaffManagementPage(this);
    m_statisticsPage = new StatisticsPage(this);
    m_accountPage = new AccountPage(this);

    // Set current user for account page
    m_accountPage->setCurrentUser(m_currentUser);

    // Add pages to stack
    m_contentStack->addWidget(m_bookingPage);    // Index 0
    m_contentStack->addWidget(m_paymentPage);    // Index 1
    m_contentStack->addWidget(m_fieldPage);      // Index 2
    m_contentStack->addWidget(m_customerPage);   // Index 3
    m_contentStack->addWidget(m_servicePage);    // Index 4
    m_contentStack->addWidget(m_staffPage);      // Index 5
    m_contentStack->addWidget(m_statisticsPage); // Index 6
    m_contentStack->addWidget(m_accountPage);    // Index 7

    // Set default page
    m_contentStack->setCurrentIndex(0);

    // Assemble right panel
    rightLayout->addWidget(m_header);
    rightLayout->addWidget(m_contentStack, 1); // Stretch factor 1

    // Add to main layout
    mainLayout->addWidget(m_sidebar);
    mainLayout->addWidget(m_rightPanel, 1); // Stretch factor 1 for right panel
}

void MainWindow::setupConnections()
{
    // Connect sidebar menu clicks to page switching
    connect(m_sidebar, &Sidebar::menuItemClicked, this, &MainWindow::onMenuItemClicked);

    // Connect admin and logout actions
    connect(m_sidebar, &Sidebar::adminClicked, this, &MainWindow::onAdminClicked);
    connect(m_sidebar, &Sidebar::logoutClicked, this, &MainWindow::onLogoutClicked);
}

void MainWindow::onMenuItemClicked(int index)
{
    // Switch to the corresponding page
    if (index >= 0 && index < m_contentStack->count())
    {
        m_contentStack->setCurrentIndex(index);
        updateHeaderTitle(index);
        m_sidebar->setAccountActive(false); // Deactivate account button
    }
}

void MainWindow::updateHeaderTitle(int pageIndex)
{
    if (pageIndex >= 0 && pageIndex < m_pageTitles.size())
    {
        m_header->setTitle(m_pageTitles[pageIndex]);
    }
}

void MainWindow::onAdminClicked()
{
    // Show account page (Index 7)
    m_contentStack->setCurrentIndex(7);
    m_header->setTitle("Quản Lý Tài Khoản");
    m_sidebar->setAccountActive(true); // Activate account button
}

void MainWindow::onLogoutClicked()
{
    // Confirm logout
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Đăng xuất",
        "Bạn có chắc chắn muốn đăng xuất?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        // Close main window - this will return to login dialog in main.cpp
        close();

        // Show login dialog again
        LoginDialog loginDialog;
        if (loginDialog.exec() == QDialog::Accepted)
        {
            // Get HeThongQuanLy instance
            HeThongQuanLy *heThong = HeThongQuanLy::getInstance();

            // Get new logged in user
            QuanTriVien *currentUser = heThong->timQuanTriVien(loginDialog.getLoggedInUsername().toStdString());
            if (currentUser)
            {
                // Create and show new MainWindow with new user
                MainWindow *newMainWindow = new MainWindow(currentUser);
                newMainWindow->showMaximized();
            }
        }
        else
        {
            // User cancelled login, exit application
            qApp->quit();
        }
    }
}
