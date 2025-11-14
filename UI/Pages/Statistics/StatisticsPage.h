#ifndef STATISTICSPAGE_H
#define STATISTICSPAGE_H

#include <QWidget>

class StatisticsPage : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsPage(QWidget *parent = nullptr);
    ~StatisticsPage();

private:
    void setupUI();
};

#endif // STATISTICSPAGE_H
