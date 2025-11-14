#ifndef PAYMENTLISTPAGE_H
#define PAYMENTLISTPAGE_H

#include <QWidget>
#include <QLabel>

class PaymentListPage : public QWidget
{
    Q_OBJECT

public:
    explicit PaymentListPage(QWidget *parent = nullptr);
    ~PaymentListPage();

private:
    void setupUI();
};

#endif // PAYMENTLISTPAGE_H
