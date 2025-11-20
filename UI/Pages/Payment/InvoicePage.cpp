#include "InvoicePage.h"
#include "Core/QuanLy/HeThongQuanLy.h"
#include <QDateTime>
#include <QHeaderView>

InvoicePage::InvoicePage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    refreshData();
}

InvoicePage::~InvoicePage()
{
}

void InvoicePage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    // Title
    QLabel *titleLabel = new QLabel("Quản Lý Hoá Đơn", this);
    QFont titleFont;
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    // Tabs
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setStyleSheet(
        "QTabWidget::pane { border: 1px solid #E0E0E0; border-radius: 8px; background: white; }"
        "QTabBar::tab { background: #F5F5F5; color: #333; padding: 10px 20px; border-top-left-radius: 8px; border-top-right-radius: 8px; margin-right: 4px; }"
        "QTabBar::tab:selected { background: #4CAF50; color: white; }"
        "QTabBar::tab:hover:!selected { background: #E0E0E0; }");

    setupBookingTab();
    setupServiceTab();

    m_tabWidget->addTab(m_bookingInvoiceTab, "Hoá Đơn Đặt Sân");
    m_tabWidget->addTab(m_serviceInvoiceTab, "Hoá Đơn Dịch Vụ");

    mainLayout->addWidget(m_tabWidget);
}

void InvoicePage::setupBookingTab()
{
    m_bookingInvoiceTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(m_bookingInvoiceTab);

    // Search bar
    QHBoxLayout *searchLayout = new QHBoxLayout();
    m_searchBookingInput = new QLineEdit();
    m_searchBookingInput->setPlaceholderText("Tìm kiếm theo tên khách hàng hoặc mã đặt sân...");
    m_searchBookingInput->setStyleSheet("QLineEdit { padding: 8px; border: 1px solid #ddd; border-radius: 4px; }");
    connect(m_searchBookingInput, &QLineEdit::textChanged, this, &InvoicePage::filterBookingData);

    searchLayout->addWidget(m_searchBookingInput);
    layout->addLayout(searchLayout);

    // Table
    m_bookingTable = new QTableWidget();
    m_bookingTable->setColumnCount(7);
    m_bookingTable->setHorizontalHeaderLabels({"Mã Đặt Sân", "Khách Hàng", "Sân", "Ngày", "Giờ", "Tổng Tiền", "Trạng Thái"});
    m_bookingTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_bookingTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_bookingTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_bookingTable->setStyleSheet("QTableWidget { border: none; gridline-color: #f0f0f0; } QHeaderView::section { background-color: #f8f9fa; padding: 8px; border: none; font-weight: bold; }");

    layout->addWidget(m_bookingTable);
}

void InvoicePage::setupServiceTab()
{
    m_serviceInvoiceTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(m_serviceInvoiceTab);

    // Search bar
    QHBoxLayout *searchLayout = new QHBoxLayout();
    m_searchServiceInput = new QLineEdit();
    m_searchServiceInput->setPlaceholderText("Tìm kiếm theo tên khách hàng hoặc mã đơn hàng...");
    m_searchServiceInput->setStyleSheet("QLineEdit { padding: 8px; border: 1px solid #ddd; border-radius: 4px; }");
    connect(m_searchServiceInput, &QLineEdit::textChanged, this, &InvoicePage::filterServiceData);

    searchLayout->addWidget(m_searchServiceInput);
    layout->addLayout(searchLayout);

    // Table
    m_serviceTable = new QTableWidget();
    m_serviceTable->setColumnCount(5);
    m_serviceTable->setHorizontalHeaderLabels({"Mã Đơn Hàng", "Khách Hàng", "Ngày Tạo", "Tổng Tiền", "Trạng Thái"});
    m_serviceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_serviceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_serviceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_serviceTable->setStyleSheet("QTableWidget { border: none; gridline-color: #f0f0f0; } QHeaderView::section { background-color: #f8f9fa; padding: 8px; border: none; font-weight: bold; }");

    layout->addWidget(m_serviceTable);
}

void InvoicePage::refreshData()
{
    loadBookingData();
    loadServiceData();
}

void InvoicePage::loadBookingData()
{
    m_bookingTable->setRowCount(0);
    auto bookings = HeThongQuanLy::getInstance()->layQuanLyDatSan()->layDanhSachDatSan();

    for (int i = 0; i < bookings.size(); ++i)
    {
        DatSan *ds = bookings[i];
        // Only show completed bookings
        if (ds->getTrangThai() != TrangThaiDatSan::HOAN_THANH)
        {
            continue;
        }

        m_bookingTable->insertRow(m_bookingTable->rowCount());
        int row = m_bookingTable->rowCount() - 1;

        m_bookingTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(ds->getMaDatSan())));
        m_bookingTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(ds->getKhachHang()->layHoTen())));
        m_bookingTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(ds->getSan()->layTenSan())));
        m_bookingTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(ds->getThoiGianDat().toString())));

        std::string khungGioStr = ds->getKhungGio().layGioBatDau().toString() + " - " + ds->getKhungGio().layGioKetThuc().toString();
        m_bookingTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(khungGioStr)));

        double price = ds->getTongTien();
        m_bookingTable->setItem(row, 5, new QTableWidgetItem(QString::number(price, 'f', 0) + " VND"));

        QString status;
        switch (ds->getTrangThai())
        {
        case TrangThaiDatSan::DA_DAT:
            status = "Đã Đặt";
            break;
        case TrangThaiDatSan::DA_HUY:
            status = "Đã Hủy";
            break;
        case TrangThaiDatSan::HOAN_THANH:
            status = "Hoàn Tất";
            break;
        }
        m_bookingTable->setItem(row, 6, new QTableWidgetItem(status));
    }
}

void InvoicePage::loadServiceData()
{
    m_serviceTable->setRowCount(0);
    auto orders = HeThongQuanLy::getInstance()->layQuanLyDonHangDichVu()->layDanhSachDonHang();

    for (int i = 0; i < orders.size(); ++i)
    {
        DonHangDichVu *dh = orders[i];
        m_serviceTable->insertRow(i);

        m_serviceTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(dh->getMaDonHang())));

        QString customerName = "Khách Vãng Lai";
        if (dh->getKhachHang())
        {
            customerName = QString::fromStdString(dh->getKhachHang()->layHoTen());
        }
        m_serviceTable->setItem(i, 1, new QTableWidgetItem(customerName));

        m_serviceTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(dh->getNgayTao().toString())));
        m_serviceTable->setItem(i, 3, new QTableWidgetItem(QString::number(dh->getThanhTien(), 'f', 0) + " VND"));

        QString status;
        switch (dh->getTrangThai())
        {
        case TrangThaiDonHang::CHO_XU_LY:
            status = "Chờ Xử Lý";
            break;
        case TrangThaiDonHang::DANG_CHUAN_BI:
            status = "Đang Chuẩn Bị";
            break;
        case TrangThaiDonHang::HOAN_THANH:
            status = "Hoàn Thành";
            break;
        case TrangThaiDonHang::DA_HUY:
            status = "Đã Hủy";
            break;
        }
        m_serviceTable->setItem(i, 4, new QTableWidgetItem(status));
    }
}

void InvoicePage::filterBookingData(const QString &text)
{
    for (int i = 0; i < m_bookingTable->rowCount(); ++i)
    {
        bool match = false;
        for (int j = 0; j < m_bookingTable->columnCount(); ++j)
        {
            QTableWidgetItem *item = m_bookingTable->item(i, j);
            if (item && item->text().contains(text, Qt::CaseInsensitive))
            {
                match = true;
                break;
            }
        }
        m_bookingTable->setRowHidden(i, !match);
    }
}

void InvoicePage::filterServiceData(const QString &text)
{
    for (int i = 0; i < m_serviceTable->rowCount(); ++i)
    {
        bool match = false;
        for (int j = 0; j < m_serviceTable->columnCount(); ++j)
        {
            QTableWidgetItem *item = m_serviceTable->item(i, j);
            if (item && item->text().contains(text, Qt::CaseInsensitive))
            {
                match = true;
                break;
            }
        }
        m_serviceTable->setRowHidden(i, !match);
    }
}
