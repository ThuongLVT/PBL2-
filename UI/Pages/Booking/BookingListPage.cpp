#include "BookingListPage.h"
#include <QFont>

BookingListPage::BookingListPage(QWidget *parent)
    : QWidget(parent), m_comingSoonLabel(nullptr)
{
    setupUI();
}

BookingListPage::~BookingListPage()
{
}

void BookingListPage::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    m_comingSoonLabel = new QLabel("📅 Đặt Sân - Coming Soon", this);
    m_comingSoonLabel->setObjectName("comingSoonLabel");
    QFont font;
    font.setPointSize(20);
    font.setBold(true);
    m_comingSoonLabel->setFont(font);

    layout->addWidget(m_comingSoonLabel);
    setLayout(layout);
}
