#ifndef SERVICEMANAGEMENTPAGE_H
#define SERVICEMANAGEMENTPAGE_H

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include "ServiceOrderWidget.h"

class ServiceManagementWidget;

class ServiceManagementPage : public QWidget
{
    Q_OBJECT

public:
    explicit ServiceManagementPage(QWidget *parent = nullptr);
    ~ServiceManagementPage();

    void refreshData();

private:
    void setupUI();
    void applyStyles();

    QVBoxLayout *mainLayout;
    QTabWidget *tabWidget;
    ServiceOrderWidget *orderWidget;
    ServiceManagementWidget *managementWidget;
};

#endif // SERVICEMANAGEMENTPAGE_H
