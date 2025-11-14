#include "ServiceManagementPage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>

ServiceManagementPage::ServiceManagementPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

ServiceManagementPage::~ServiceManagementPage()
{
}

void ServiceManagementPage::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *label = new QLabel("🛍️ Dịch Vụ - Coming Soon", this);
    label->setObjectName("comingSoonLabel");
    QFont font;
    font.setPointSize(20);
    font.setBold(true);
    label->setFont(font);

    layout->addWidget(label);
    setLayout(layout);
}
