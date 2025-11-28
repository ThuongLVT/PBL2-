#include "ServiceManagementPage.h"
#include "ServiceManagementWidget.h"
#include <QLabel>
#include <QFont>

ServiceManagementPage::ServiceManagementPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    applyStyles();
}

ServiceManagementPage::~ServiceManagementPage()
{
}

void ServiceManagementPage::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // No page title here - already shown in main window header

    // Tab widget
    tabWidget = new QTabWidget(this);
    tabWidget->setObjectName("serviceTabWidget");

    // Tab 1: Order Service (Đặt dịch vụ)
    orderWidget = new ServiceOrderWidget(this);
    tabWidget->addTab(orderWidget, "🛒 Đặt Dịch Vụ");

    // Tab 2: Service Management (Quản lý dịch vụ)
    managementWidget = new ServiceManagementWidget(this);
    tabWidget->addTab(managementWidget, "⚙️ Quản Lý Dịch Vụ");

    // TODO: Tab 3: Order History (Lịch sử đơn hàng) - khninh22 will add later

    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
}

void ServiceManagementPage::applyStyles()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #f9fafb;
            color: #1f2937;
        }
        
        #serviceTabWidget {
            background-color: transparent;
            border: none;
        }
        
        #serviceTabWidget::pane {
            background-color: #f9fafb;
            border: none;
            border-top: 2px solid #e5e7eb;
        }
        
        #serviceTabWidget QTabBar::tab {
            background-color: white;
            color: #6b7280;
            border: 1px solid #e5e7eb;
            border-bottom: none;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
            padding: 12px 24px;
            margin-right: 4px;
            font-size: 14px;
            font-weight: 600;
        }
        
        #serviceTabWidget QTabBar::tab:selected {
            background-color: #f9fafb;
            color: #16a34a;
            border-bottom: 2px solid #16a34a;
            border-bottom-left-radius: 0px;
            border-bottom-right-radius: 0px;
        }
        
        #serviceTabWidget QTabBar::tab:hover:!selected {
            background-color: #f0fdf4;
            color: #15803d;
        }
    )");
}

void ServiceManagementPage::setUserRole(bool isAdmin)
{
    // If not admin, remove or disable the management tab
    if (!isAdmin)
    {
        // Check if tab 1 exists and is the management widget
        if (tabWidget->count() > 1 && tabWidget->widget(1) == managementWidget)
        {
            tabWidget->removeTab(1);
        }
    }
    else
    {
        // If admin and tab is missing, add it back
        bool hasManagementTab = false;
        for (int i = 0; i < tabWidget->count(); ++i)
        {
            if (tabWidget->widget(i) == managementWidget)
            {
                hasManagementTab = true;
                break;
            }
        }

        if (!hasManagementTab)
        {
            tabWidget->addTab(managementWidget, "⚙️ Quản Lý Dịch Vụ");
        }
    }
}
