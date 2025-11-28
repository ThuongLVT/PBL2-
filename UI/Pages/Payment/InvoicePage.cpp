#include "InvoicePage.h"
#include "InvoiceDetailDialog.h"
#include "Core/QuanLy/HeThongQuanLy.h"
#include <QDateTime>
#include <QHeaderView>
#include <QDebug>

InvoicePage::InvoicePage(QWidget *parent)
    : QWidget(parent), m_isAdmin(true)
{
    // Default date range: this month
    m_toDate = QDate::currentDate();
    m_fromDate = QDate(m_toDate.year(), m_toDate.month(), 1);

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

    // Header Layout (Title + Date Picker)
    QHBoxLayout *headerLayout = new QHBoxLayout();

    // Title
    QLabel *titleLabel = new QLabel("Quản Lý Hoá Đơn", this);
    QFont titleFont;
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    headerLayout->addWidget(titleLabel);

    headerLayout->addStretch();

    // Date Picker
    m_datePicker = new DateRangePicker(this);
    m_datePicker->setDateRange(m_fromDate, m_toDate);
    connect(m_datePicker, &DateRangePicker::dateRangeChanged, this, &InvoicePage::onDateRangeChanged);
    headerLayout->addWidget(m_datePicker);

    mainLayout->addLayout(headerLayout);

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
    m_bookingTable->setColumnCount(8);
    m_bookingTable->setHorizontalHeaderLabels({"Mã Đặt Sân", "Khách Hàng", "Sân", "Ngày Đặt", "Giờ Đặt", "Thời Gian Thanh Toán", "Tổng Tiền", "Trạng Thái"});
    m_bookingTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_bookingTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_bookingTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_bookingTable->setFocusPolicy(Qt::NoFocus); // Remove focus frame
    m_bookingTable->setStyleSheet(
        "QTableWidget { border: none; gridline-color: #f0f0f0; outline: none; }"
        "QHeaderView::section { background-color: #f8f9fa; padding: 8px; border: none; font-weight: bold; }"
        "QTableWidget::item:focus { border: none; outline: none; }" // Remove focus border
    );
    connect(m_bookingTable, &QTableWidget::itemDoubleClicked, this, &InvoicePage::onTableItemDoubleClicked);

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
    m_serviceTable->setFocusPolicy(Qt::NoFocus); // Remove focus frame
    m_serviceTable->setStyleSheet(
        "QTableWidget { border: none; gridline-color: #f0f0f0; outline: none; }"
        "QHeaderView::section { background-color: #f8f9fa; padding: 8px; border: none; font-weight: bold; }"
        "QTableWidget::item:focus { border: none; outline: none; }" // Remove focus border
    );
    connect(m_serviceTable, &QTableWidget::itemDoubleClicked, this, &InvoicePage::onTableItemDoubleClicked);

    layout->addWidget(m_serviceTable);
}

void InvoicePage::refreshData()
{
    loadBookingData();
    loadServiceData();
}

void InvoicePage::onDateRangeChanged(const QDate &from, const QDate &to)
{
    m_fromDate = from;
    m_toDate = to;
    refreshData();
}

void InvoicePage::loadBookingData()
{
    m_bookingTable->setRowCount(0);
    auto bookings = HeThongQuanLy::getInstance()->layQuanLyDatSan()->layDanhSachDatSan();

    for (int i = 0; i < bookings.size(); ++i)
    {
        DatSan *ds = bookings[i];

        // Logic hiển thị hóa đơn:
        // 1. Hoàn thành (Đã thanh toán full)
        // 2. Đã hủy (Có thể là Phạt cọc hoặc Hoàn cọc)
        bool isCompleted = (ds->getTrangThai() == TrangThaiDatSan::HOAN_THANH);
        bool isCancelled = (ds->getTrangThai() == TrangThaiDatSan::DA_HUY);

        if (!isCompleted && !isCancelled)
        {
            continue; // Bỏ qua các trạng thái khác (Đã đặt, Chờ duyệt...)
        }

        // Determine Payment Date
        NgayGio ngayThanhToan = ds->getNgayThanhToan();

        // Fallback for existing data or if not set
        if (ngayThanhToan.getNam() == 0)
        {
            if (isCompleted)
            {
                // If completed but no payment date, assume date of play
                ngayThanhToan = ds->getThoiGianDat();
            }
            else if (isCancelled)
            {
                // If cancelled, assume date of creation (deposit date) or date of play
                ngayThanhToan = ds->getNgayTao();
            }
        }

        // Date Filter based on Payment Date
        QDate paymentDate(ngayThanhToan.getNam(), ngayThanhToan.getThang(), ngayThanhToan.getNgay());
        if (paymentDate < m_fromDate || paymentDate > m_toDate)
        {
            continue;
        }

        // Kiểm tra xem có phải phạt cọc không
        bool isPenalty = false;
        if (isCancelled)
        {
            // Check deposit status directly
            if (ds->getTrangThaiCoc() == TrangThaiCoc::MAT_COC)
            {
                isPenalty = true;
            }
            // Fallback to note check for old data
            else
            {
                QString note = QString::fromStdString(ds->getGhiChu());
                if (note.contains("[MAT_COC]") || note.contains("[PHAT_COC]"))
                {
                    isPenalty = true;
                }
            }
        }

        m_bookingTable->insertRow(m_bookingTable->rowCount());
        int row = m_bookingTable->rowCount() - 1;

        m_bookingTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(ds->getMaDatSan())));
        m_bookingTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(ds->getKhachHang()->layHoTen())));
        m_bookingTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(ds->getSan()->layTenSan())));

        // Ngày đặt sân (chỉ ngày)
        NgayGio thoiGianDat = ds->getThoiGianDat();
        QString ngayDat = QString("%1/%2/%3")
                              .arg(thoiGianDat.getNgay(), 2, 10, QChar('0'))
                              .arg(thoiGianDat.getThang(), 2, 10, QChar('0'))
                              .arg(thoiGianDat.getNam());
        m_bookingTable->setItem(row, 3, new QTableWidgetItem(ngayDat));

        // Giờ đặt sân
        std::string khungGioStr = ds->getKhungGio().layGioBatDau().toString() + " - " + ds->getKhungGio().layGioKetThuc().toString();
        m_bookingTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(khungGioStr)));

        // Ngày thanh toán (Thời gian thanh toán)
        QString ngayThanhToanStr = QString("%1/%2/%3 %4:%5:%6")
                                       .arg(ngayThanhToan.getNgay(), 2, 10, QChar('0'))
                                       .arg(ngayThanhToan.getThang(), 2, 10, QChar('0'))
                                       .arg(ngayThanhToan.getNam())
                                       .arg(ngayThanhToan.getGio(), 2, 10, QChar('0'))
                                       .arg(ngayThanhToan.getPhut(), 2, 10, QChar('0'))
                                       .arg(ngayThanhToan.getGiay(), 2, 10, QChar('0'));
        m_bookingTable->setItem(row, 5, new QTableWidgetItem(ngayThanhToanStr));

        // Tổng tiền & Trạng thái
        double price = 0;
        QString statusText;
        QColor statusColor;

        if (isCompleted)
        {
            price = ds->getTongTien();
            statusText = "Hoàn Tất";
            statusColor = QColor("#16a34a"); // Green
        }
        else if (isPenalty)
        {
            price = ds->getTienCoc(); // Phạt cọc thì thu tiền cọc
            statusText = "Phạt Cọc (Đã Hủy)";
            statusColor = QColor("#dc2626"); // Red
        }
        else
        {
            price = 0; // Hoàn cọc thì doanh thu = 0 (hoặc hiển thị tiền cọc nhưng gạch ngang?)
            // Để 0 cho đúng bản chất doanh thu thực thu
            statusText = "Hoàn Cọc (Đã Hủy)";
            statusColor = QColor("#9ca3af"); // Gray
        }

        m_bookingTable->setItem(row, 6, new QTableWidgetItem(QString::number(price, 'f', 0) + " VND"));

        QTableWidgetItem *statusItem = new QTableWidgetItem(statusText);
        statusItem->setForeground(statusColor);
        statusItem->setFont(QFont("Segoe UI", 9, QFont::Bold));
        m_bookingTable->setItem(row, 7, statusItem);
    }
}

void InvoicePage::loadServiceData()
{
    m_serviceTable->setRowCount(0);
    auto orders = HeThongQuanLy::getInstance()->layQuanLyDonHangDichVu()->layDanhSachDonHang();
    qDebug() << "[DEBUG] InvoicePage::loadServiceData - Found" << orders.size() << "orders";

    for (int i = 0; i < orders.size(); ++i)
    {
        DonHangDichVu *dh = orders[i];

        // Date Filter
        NgayGio ngayTao = dh->getNgayTao();
        QDate orderDate(ngayTao.getNam(), ngayTao.getThang(), ngayTao.getNgay());
        if (orderDate < m_fromDate || orderDate > m_toDate)
        {
            continue;
        }

        m_serviceTable->insertRow(m_serviceTable->rowCount());
        int row = m_serviceTable->rowCount() - 1;

        m_serviceTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(dh->getMaDonHang())));

        QString customerName = "Khách Vãng Lai";
        if (dh->getKhachHang())
        {
            customerName = QString::fromStdString(dh->getKhachHang()->layHoTen());
        }
        m_serviceTable->setItem(row, 1, new QTableWidgetItem(customerName));

        m_serviceTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(dh->getNgayTao().toString())));
        m_serviceTable->setItem(row, 3, new QTableWidgetItem(QString::number(dh->getThanhTien(), 'f', 0) + " VND"));

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
        m_serviceTable->setItem(row, 4, new QTableWidgetItem(status));
    }
}

void InvoicePage::onTableItemDoubleClicked(QTableWidgetItem *item)
{
    if (!item)
        return;

    QTableWidget *table = item->tableWidget();
    int row = item->row();
    QString id = table->item(row, 0)->text();

    InvoiceDetailDialog::InvoiceType type;
    if (table == m_bookingTable)
    {
        type = InvoiceDetailDialog::BOOKING;
    }
    else
    {
        type = InvoiceDetailDialog::SERVICE;
    }

    InvoiceDetailDialog dialog(id, type, this);
    dialog.exec();
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

void InvoicePage::setUserRole(bool isAdmin)
{
    m_isAdmin = isAdmin;
    if (!m_isAdmin)
    {
        // Force date range to today
        m_fromDate = QDate::currentDate();
        m_toDate = QDate::currentDate();

        // Update date picker and disable it
        if (m_datePicker)
        {
            m_datePicker->setDateRange(m_fromDate, m_toDate);
            m_datePicker->setEnabled(false);
        }

        refreshData();
    }
    else
    {
        // Enable date picker for admin
        if (m_datePicker)
        {
            m_datePicker->setEnabled(true);
        }
    }
}
