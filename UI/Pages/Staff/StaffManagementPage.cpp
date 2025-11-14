#include "StaffManagementPage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>

StaffManagementPage::StaffManagementPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

StaffManagementPage::~StaffManagementPage()
{
}

void StaffManagementPage::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *label = new QLabel("👨‍💼 Nhân Viên - Coming Soon", this);
    label->setObjectName("comingSoonLabel");
    QFont font;
    font.setPointSize(20);
    font.setBold(true);
    label->setFont(font);

    layout->addWidget(label);
    setLayout(layout);
}
