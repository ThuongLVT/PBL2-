#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QSplitter>
#include "UI/Components/Sidebar.h"
#include "UI/Components/Header.h"
#include "UI/Pages/Booking/BookingListPage.h"
#include "UI/Pages/Payment/PaymentListPage.h"
#include "UI/Pages/Field/FieldManagementPage.h"
#include "UI/Pages/Customer/CustomerManagementPage.h"
#include "UI/Pages/Service/ServiceManagementPage.h"
#include "UI/Pages/Staff/StaffManagementPage.h"
#include "UI/Pages/Statistics/StatisticsPage.h"
#include "UI/Pages/Account/AccountPage.h"
#include "Core/QuanLy/HeThongQuanLy.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QuanTriVien *currentUser, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onMenuItemClicked(int index);
    void onAdminClicked();
    void onLogoutClicked();

private:
    void setupUI();
    void setupConnections();
    void updateHeaderTitle(int pageIndex);

    // Current user
    QuanTriVien *m_currentUser;

    // Main layout components
    QSplitter *m_mainSplitter;
    Sidebar *m_sidebar;
    QWidget *m_rightPanel;
    Header *m_header;
    QStackedWidget *m_contentStack;

    // Pages
    BookingListPage *m_bookingPage;
    PaymentListPage *m_paymentPage;
    FieldManagementPage *m_fieldPage;
    CustomerManagementPage *m_customerPage;
    ServiceManagementPage *m_servicePage;
    StaffManagementPage *m_staffPage;
    StatisticsPage *m_statisticsPage;
    AccountPage *m_accountPage;

    // Page titles
    QStringList m_pageTitles;
};

#endif // MAINWINDOW_H
