#include "Header.h"
#include <QFont>

Header::Header(QWidget *parent)
    : QWidget(parent), m_titleLabel(nullptr), m_mainLayout(nullptr)
{
    setupUI();
}

Header::~Header()
{
}

void Header::setupUI()
{
    // Set object name for QSS styling
    setObjectName("Header");

    // Fixed height: 80px (same as sidebar toggle section)
    setFixedHeight(80);
    setMinimumHeight(80);
    setMaximumHeight(80);

    // Main layout
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(24, 0, 24, 0);
    m_mainLayout->setSpacing(0);

    // Title label
    m_titleLabel = new QLabel("Quản lý Đặt Sân", this);
    m_titleLabel->setObjectName("HeaderTitle");

    // Font styling
    QFont titleFont;
    titleFont.setFamily("Segoe UI");
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);

    m_mainLayout->addWidget(m_titleLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    m_mainLayout->addStretch();

    setLayout(m_mainLayout);
}

void Header::setTitle(const QString &title)
{
    m_titleLabel->setText(title);
}

QString Header::title() const
{
    return m_titleLabel->text();
}
