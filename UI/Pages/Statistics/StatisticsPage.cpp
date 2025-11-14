#include "StatisticsPage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>

StatisticsPage::StatisticsPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

StatisticsPage::~StatisticsPage()
{
}

void StatisticsPage::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *label = new QLabel("📊 Thống Kê - Coming Soon", this);
    label->setObjectName("comingSoonLabel");
    QFont font;
    font.setPointSize(20);
    font.setBold(true);
    label->setFont(font);

    layout->addWidget(label);
    setLayout(layout);
}
