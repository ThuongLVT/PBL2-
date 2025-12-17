#include "InvoiceDialog.h"
#include <QFont>
#include <QFontMetrics>

InvoiceDialog::InvoiceDialog(const std::string &invoiceText, QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Hóa đơn thanh toán");

    // Remove help button and maximize button
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Create Scroll Area
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(false); // Let label determine its size
    scrollArea->setStyleSheet("QScrollArea { border: none; background-color: #f0f0f0; }");
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Create Invoice Label (The Paper)
    invoiceLabel = new QLabel(this);
    invoiceLabel->setTextFormat(Qt::PlainText);
    invoiceLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    invoiceLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    // Use a monospaced font for alignment
    QFont font("Consolas", 11);
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    invoiceLabel->setFont(font);

    invoiceLabel->setText(QString::fromStdString(invoiceText));

    // Calculate required width
    QFontMetrics fm(font);
    int charWidth = fm.horizontalAdvance(' ');
    // Invoice is exactly 64 chars wide.
    // Reduced buffer to 66 chars to balance left/right whitespace
    int contentWidth = charWidth * 66;

    // Padding: 15px left + 15px right = 30px
    // No scrollbar inside label anymore
    invoiceLabel->setFixedWidth(contentWidth + 35);

    // Style the label to look like paper
    invoiceLabel->setStyleSheet("QLabel { font-family: 'Consolas'; font-size: 11pt; background-color: #ffffff; color: #000000; border: 1px solid #cccccc; padding: 15px; }");

    // Add label to scroll area
    scrollArea->setWidget(invoiceLabel);
    scrollArea->setAlignment(Qt::AlignCenter); // Center the paper in the scroll area

    // Set a reasonable minimum size for the scroll area
    scrollArea->setMinimumHeight(700);
    scrollArea->setMinimumWidth(contentWidth + 60); // Paper width + some margin for scrollbar

    mainLayout->addWidget(scrollArea);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    btnCancel = new QPushButton("Hủy", this);
    btnPay = new QPushButton("Thanh toán ngay", this);
    btnPayAndExport = new QPushButton("Thanh toán và Xuất hóa đơn", this);

    // Style buttons
    btnCancel->setCursor(Qt::PointingHandCursor);
    btnPay->setCursor(Qt::PointingHandCursor);
    btnPayAndExport->setCursor(Qt::PointingHandCursor);

    btnCancel->setStyleSheet(
        "QPushButton { "
        "background-color: white; "
        "color: #374151; "
        "padding: 10px 20px; "
        "font-weight: 600; "
        "border: 1px solid #d1d5db; "
        "border-radius: 6px; "
        "} "
        "QPushButton:hover { background-color: #f3f4f6; }");

    btnPay->setStyleSheet(
        "QPushButton { "
        "background-color: #3b82f6; " // Blue
        "color: white; "
        "padding: 10px 20px; "
        "font-weight: 600; "
        "border-radius: 6px; "
        "border: none; "
        "} "
        "QPushButton:hover { background-color: #2563eb; }");

    btnPayAndExport->setStyleSheet(
        "QPushButton { "
        "background-color: #10b981; " // Green
        "color: white; "
        "padding: 10px 20px; "
        "font-weight: 600; "
        "border-radius: 6px; "
        "border: none; "
        "} "
        "QPushButton:hover { background-color: #059669; }");

    buttonLayout->addWidget(btnCancel);
    buttonLayout->addStretch();
    buttonLayout->addWidget(btnPay);
    buttonLayout->addWidget(btnPayAndExport);

    mainLayout->addLayout(buttonLayout);

    // Set fixed size for the dialog based on content
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    connect(btnPay, &QPushButton::clicked, this, &InvoiceDialog::onPayClicked);
    connect(btnPayAndExport, &QPushButton::clicked, this, &InvoiceDialog::onPayAndExportClicked);
    connect(btnCancel, &QPushButton::clicked, this, &InvoiceDialog::onCancelClicked);
}

InvoiceDialog::~InvoiceDialog() {}

void InvoiceDialog::setReadOnly(bool isReadOnly)
{
    if (isReadOnly)
    {
        btnPay->setVisible(false);
        btnPayAndExport->setText("Xuất hóa đơn");
        btnCancel->setText("Đóng");
        setWindowTitle("Chi tiết hóa đơn");
    }
}

void InvoiceDialog::onPayClicked()
{
    // Return 1 (Accepted)
    done(QDialog::Accepted);
}

void InvoiceDialog::onPayAndExportClicked()
{
    // Return 2 (Custom code for Pay + Export)
    done(2);
}

void InvoiceDialog::onCancelClicked()
{
    reject();
}
