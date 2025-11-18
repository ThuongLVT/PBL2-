/**
 * @file BookingPage.cpp
 * @brief Implementation of BookingPage
 */

#include "BookingPage.h"
#include "TimelineTab.h"
#include "BookingTableTab.h"
#include "../../../Core/QuanLy/HeThongQuanLy.h"
#include "../../../Core/QuanLy/QuanLyDatSan.h"
#include "../../../Core/QuanLy/QuanLyKhachHang.h"
#include "../../../Core/QuanLy/QuanLySan.h"
#include "../../../Core/Models/DatSan.h"
#include "../../../Core/Models/KhachHang.h"
#include "../../../Core/Models/San.h"
#include <QLabel>
#include <QMessageBox>
#include <QDate>

BookingPage::BookingPage(QWidget *parent)
    : QWidget(parent), mainLayout(nullptr), tabWidget(nullptr), timelineTab(nullptr), bookingTableTab(nullptr)
{
    setupUI();
    setupConnections();
}

BookingPage::~BookingPage()
{
    // Qt handles cleanup via parent-child relationship
}

void BookingPage::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Create tab widget
    tabWidget = new QTabWidget(this);
    tabWidget->setObjectName("bookingTabWidget");

    // Tab 1: Đặt sân (Timeline View)
    timelineTab = new TimelineTab(this);
    tabWidget->addTab(timelineTab, "📅 Đặt sân");

    // Tab 2: Danh sách đặt sân (Table View)
    bookingTableTab = new BookingTableTab(this);
    tabWidget->addTab(bookingTableTab, "📋 Danh sách đặt sân");

    mainLayout->addWidget(tabWidget);
}

void BookingPage::setupConnections()
{
    // Connect TimelineTab signal to refresh table
    if (timelineTab)
    {
        connect(timelineTab, &TimelineTab::bookingDataChanged, this, [this]()
                {
                    if (bookingTableTab)
                    {
                        bookingTableTab->refreshData();
                    } });
    }

    // Connect BookingTableTab signal to refresh timeline
    if (bookingTableTab)
    {
        connect(bookingTableTab, &BookingTableTab::bookingDataChanged, this, [this]()
                {
                    if (timelineTab)
                    {
                        timelineTab->refreshData();
                    } });
    }
}

void BookingPage::refreshData()
{
    // Refresh both tabs when page becomes visible
    if (timelineTab)
    {
        timelineTab->refreshData();
    }

    if (bookingTableTab)
    {
        bookingTableTab->refreshData();
    }
}
