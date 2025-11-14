#include "PaymentListPage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>

PaymentListPage::PaymentListPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

PaymentListPage::~PaymentListPage()
{
}

void PaymentListPage::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *label = new QLabel("💳 Thanh Toán - Coming Soon", this);
    label->setObjectName("comingSoonLabel");
    QFont font;
    font.setPointSize(20);
    font.setBold(true);
    label->setFont(font);

    layout->addWidget(label);
    setLayout(layout);
}
