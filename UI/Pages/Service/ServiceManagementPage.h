#ifndef SERVICEMANAGEMENTPAGE_H
#define SERVICEMANAGEMENTPAGE_H

#include <QWidget>

class ServiceManagementPage : public QWidget
{
    Q_OBJECT

public:
    explicit ServiceManagementPage(QWidget *parent = nullptr);
    ~ServiceManagementPage();

private:
    void setupUI();
};

#endif // SERVICEMANAGEMENTPAGE_H
