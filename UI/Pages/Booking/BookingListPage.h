#ifndef BOOKINGLISTPAGE_H
#define BOOKINGLISTPAGE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class BookingListPage : public QWidget
{
    Q_OBJECT

public:
    explicit BookingListPage(QWidget *parent = nullptr);
    ~BookingListPage();

private:
    void setupUI();

    QLabel *m_comingSoonLabel;
};

#endif // BOOKINGLISTPAGE_H
