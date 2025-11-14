#ifndef STAFFMANAGEMENTPAGE_H
#define STAFFMANAGEMENTPAGE_H

#include <QWidget>

class StaffManagementPage : public QWidget
{
    Q_OBJECT

public:
    explicit StaffManagementPage(QWidget *parent = nullptr);
    ~StaffManagementPage();

private:
    void setupUI();
};

#endif // STAFFMANAGEMENTPAGE_H
