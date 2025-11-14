#ifndef HEADER_H
#define HEADER_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class Header : public QWidget
{
    Q_OBJECT

public:
    explicit Header(QWidget *parent = nullptr);
    ~Header();

    void setTitle(const QString &title);
    QString title() const;

private:
    void setupUI();
    void applyStyles();

    QLabel *m_titleLabel;
    QHBoxLayout *m_mainLayout;
};

#endif // HEADER_H
