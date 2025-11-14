/**
 * @file BookingPage.cpp
 * @brief Implementation of BookingPage
 */

#include "BookingPage.h"
#include "TimelineTab.h"
#include <QLabel>

BookingPage::BookingPage(QWidget *parent)
    : QWidget(parent), mainLayout(nullptr), tabWidget(nullptr), timelineTab(nullptr), tableTab(nullptr)
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

    // Tab 1: Timeline View
    timelineTab = new TimelineTab(this);
    tabWidget->addTab(timelineTab, "📅 Timeline View");

    // Tab 2: Table View (placeholder)
    tableTab = new QWidget(this);
    QVBoxLayout *tablePlaceholderLayout = new QVBoxLayout(tableTab);
    QLabel *placeholderLabel = new QLabel("Table View - Coming soon (Week 2 Day 9-10)", tableTab);
    placeholderLabel->setAlignment(Qt::AlignCenter);
    placeholderLabel->setStyleSheet("font-size: 16px; color: #6b7280;");
    tablePlaceholderLayout->addWidget(placeholderLabel);
    tabWidget->addTab(tableTab, "📋 Table View");

    mainLayout->addWidget(tabWidget);
}

void BookingPage::setupConnections()
{
    // Connect tab changes if needed
}

void BookingPage::refreshData()
{
    // Refresh timeline tab data when page becomes visible
    if (timelineTab)
    {
        timelineTab->refreshData();
    }
}
