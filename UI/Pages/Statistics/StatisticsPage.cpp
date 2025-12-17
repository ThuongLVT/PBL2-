#include "StatisticsPage.h"
#include "RevenueTab.h"
#include "BookingFieldTab.h"
#include "CustomerTab.h"
#include "ServiceTab.h"
#include "../../../Core/QuanLy/HeThongQuanLy.h"
#include <QShowEvent>

// ========== CONSTRUCTOR ==========

StatisticsPage::StatisticsPage(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(nullptr),
      m_tabWidget(nullptr),
      m_revenueTab(nullptr),
      m_bookingFieldTab(nullptr),
      m_customerTab(nullptr),
      m_serviceTab(nullptr),
      m_heThong(nullptr)
{
    setupUI();
}

StatisticsPage::~StatisticsPage()
{
}

// ========== PUBLIC METHODS ==========

void StatisticsPage::setHeThong(HeThongQuanLy *ht)
{
    m_heThong = ht;

    // Pass to all tabs
    if (m_revenueTab)
        m_revenueTab->setHeThong(ht);
    if (m_bookingFieldTab)
        m_bookingFieldTab->setHeThong(ht);
    if (m_customerTab)
        m_customerTab->setHeThong(ht);
    if (m_serviceTab)
        m_serviceTab->setHeThong(ht);
}

void StatisticsPage::refreshData()
{
    // Refresh current tab
    int index = m_tabWidget->currentIndex();
    switch (index)
    {
    case 0:
        if (m_revenueTab)
            m_revenueTab->refreshData();
        break;
    case 1:
        if (m_bookingFieldTab)
            m_bookingFieldTab->refreshData();
        break;
    case 2:
        if (m_customerTab)
            m_customerTab->refreshData();
        break;
    case 3:
        if (m_serviceTab)
            m_serviceTab->refreshData();
        break;
    }
}

// ========== SLOTS ==========

void StatisticsPage::onTabChanged(int index)
{
    // Refresh data when switching tabs
    switch (index)
    {
    case 0:
        if (m_revenueTab)
            m_revenueTab->refreshData();
        break;
    case 1:
        if (m_bookingFieldTab)
            m_bookingFieldTab->refreshData();
        break;
    case 2:
        if (m_customerTab)
            m_customerTab->refreshData();
        break;
    case 3:
        if (m_serviceTab)
            m_serviceTab->refreshData();
        break;
    }
}

// ========== EVENTS ==========

void StatisticsPage::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    refreshData();
}

// ========== SETUP UI ==========

void StatisticsPage::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // Create tab widget
    m_tabWidget = new QTabWidget(this);
    styleTabWidget();

    // Create tabs
    m_revenueTab = new RevenueTab(this);
    m_bookingFieldTab = new BookingFieldTab(this);
    m_customerTab = new CustomerTab(this);
    m_serviceTab = new ServiceTab(this);

    // Add tabs with icons
    m_tabWidget->addTab(m_revenueTab, "💰 Doanh Thu");
    m_tabWidget->addTab(m_bookingFieldTab, "⚽ Đặt Sân");
    m_tabWidget->addTab(m_customerTab, "👥 Khách Hàng");
    m_tabWidget->addTab(m_serviceTab, "🛒 Dịch Vụ");

    connect(m_tabWidget, &QTabWidget::currentChanged, this, &StatisticsPage::onTabChanged);

    m_mainLayout->addWidget(m_tabWidget);
}

void StatisticsPage::styleTabWidget()
{
    m_tabWidget->setStyleSheet(R"(
        QTabWidget::pane {
            border: none;
            background-color: #f8fafc;
        }
        QTabBar::tab {
            background-color: #e2e8f0;
            color: #475569;
            padding: 12px 24px;
            margin-right: 4px;
            border-top-left-radius: 8px;
            border-top-right-radius: 8px;
            font-size: 14px;
            font-weight: 500;
        }
        QTabBar::tab:selected {
            background-color: #f8fafc;
            color: #1e293b;
            font-weight: 600;
        }
        QTabBar::tab:hover:!selected {
            background-color: #cbd5e1;
        }
    )");
}
