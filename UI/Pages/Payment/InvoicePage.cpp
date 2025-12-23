#include "InvoicePage.h"
#include "../../Dialogs/InvoiceDialog.h"
#include "Core/Utils/InvoiceGenerator.h"
#include "Core/QuanLy/HeThongQuanLy.h"
#include <QDateTime>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QPrinter>
#include <QTextDocument>
#include <QFileInfo>
#include <QDir>

InvoicePage::InvoicePage(QWidget *parent)
    : QWidget(parent), m_isAdmin(true)
{
    // Default date: Today
    m_selectedDate = QDate::currentDate();

    setupUI();

    // Initialize with Today's data and styles
    onTodayClicked();
}

InvoicePage::~InvoicePage()
{
}

void InvoicePage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    setupToolbar(mainLayout);
    setupTable(mainLayout);
}

void InvoicePage::setupToolbar(QVBoxLayout *mainLayout)
{
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    toolbarLayout->setSpacing(15);

    // 1. Search Input
    m_searchInput = new QLineEdit(this);
    m_searchInput->setPlaceholderText("🔍 Tìm mã HĐ, tên khách, SĐT...");
    m_searchInput->setMinimumWidth(250);
    m_searchInput->setMinimumHeight(36);
    m_searchInput->setStyleSheet(
        "QLineEdit { "
        "padding: 0 10px; "
        "border: 1px solid #d1d5db; "
        "border-radius: 6px; "
        "background-color: white; "
        "} "
        "QLineEdit:focus { border: 2px solid #3b82f6; }");
    connect(m_searchInput, &QLineEdit::textChanged, this, &InvoicePage::onSearchTextChanged);
    toolbarLayout->addWidget(m_searchInput);

    // 2. Type Filter
    QLabel *typeLabel = new QLabel("Loại:", this);
    typeLabel->setStyleSheet("font-weight: bold; color: #374151;");
    toolbarLayout->addWidget(typeLabel);

    m_typeFilter = new QComboBox(this);
    m_typeFilter->addItem("Tất cả", 0);
    m_typeFilter->addItem("Đặt sân", 1);
    m_typeFilter->addItem("Phạt cọc", 2);
    m_typeFilter->addItem("Dịch vụ", 3);
    m_typeFilter->setFixedSize(105, 36); // Reduced width by 5px
    m_typeFilter->setStyleSheet(
        "QComboBox { "
        "padding: 0 10px; "
        "border: 1px solid #d1d5db; "
        "border-radius: 6px; "
        "background-color: white; "
        "}");
    connect(m_typeFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &InvoicePage::onTypeFilterChanged);
    toolbarLayout->addWidget(m_typeFilter);

    // 3. Sort Filter
    QLabel *sortLabel = new QLabel("Sắp xếp:", this);
    sortLabel->setStyleSheet("font-weight: bold; color: #374151; margin-left: 10px;");
    toolbarLayout->addWidget(sortLabel);

    m_sortFilter = new QComboBox(this);
    m_sortFilter->addItem("Mới nhất", 0);
    m_sortFilter->addItem("Giá: Thấp -> Cao", 1);
    m_sortFilter->addItem("Giá: Cao -> Thấp", 2);
    m_sortFilter->setFixedSize(160, 36); // Increased width by 5px
    m_sortFilter->setStyleSheet(m_typeFilter->styleSheet());
    connect(m_sortFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &InvoicePage::onSortFilterChanged);
    toolbarLayout->addWidget(m_sortFilter);

    // 4. Date Selection (Today | Yesterday | DatePicker)

    // Today Button
    m_todayBtn = new QPushButton("Hôm nay", this);
    m_todayBtn->setFixedSize(100, 36); // Fixed size
    m_todayBtn->setCursor(Qt::PointingHandCursor);
    connect(m_todayBtn, &QPushButton::clicked, this, &InvoicePage::onTodayClicked);
    toolbarLayout->addWidget(m_todayBtn);

    // Yesterday Button
    m_yesterdayBtn = new QPushButton("Hôm qua", this);
    m_yesterdayBtn->setFixedSize(100, 36); // Fixed size
    m_yesterdayBtn->setCursor(Qt::PointingHandCursor);
    connect(m_yesterdayBtn, &QPushButton::clicked, this, &InvoicePage::onYesterdayClicked);
    toolbarLayout->addWidget(m_yesterdayBtn);

    // Date Picker
    m_datePicker = new QDateEdit(QDate::currentDate(), this);
    m_datePicker->setCalendarPopup(true);
    m_datePicker->setDisplayFormat("dd/MM/yyyy");
    m_datePicker->setFixedSize(130, 36); // Fixed size
    m_datePicker->setStyleSheet(
        "QDateEdit { "
        "padding: 0 10px; "
        "border: 1px solid #d1d5db; "
        "border-radius: 6px; "
        "background-color: white; "
        "}");
    connect(m_datePicker, &QDateEdit::dateChanged, this, &InvoicePage::onDateChanged);
    toolbarLayout->addWidget(m_datePicker);

    toolbarLayout->addStretch();

    // 4. Total Revenue
    m_totalRevenueLabel = new QLabel("Tổng thu: 0 đ", this);
    m_totalRevenueLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #16a34a; margin-right: 15px;");
    toolbarLayout->addWidget(m_totalRevenueLabel);

    // 5. Export Button
    QPushButton *exportBtn = new QPushButton("📥 Xuất Excel", this);
    exportBtn->setCursor(Qt::PointingHandCursor);
    exportBtn->setMinimumHeight(36);
    exportBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #10b981; "
        "color: white; "
        "border: none; "
        "border-radius: 6px; "
        "padding: 0 15px; "
        "font-weight: 600; "
        "} "
        "QPushButton:hover { background-color: #059669; }");
    connect(exportBtn, &QPushButton::clicked, this, &InvoicePage::onExportClicked);
    toolbarLayout->addWidget(exportBtn);

    mainLayout->addLayout(toolbarLayout);
}

void InvoicePage::setupTable(QVBoxLayout *mainLayout)
{
    m_invoiceTable = new QTableWidget(this);
    m_invoiceTable->setColumnCount(9);
    m_invoiceTable->setHorizontalHeaderLabels({"STT", "Mã HĐ", "Thời gian", "Khách hàng", "SĐT", "Nội dung", "Loại", "Tổng tiền", "Chi tiết"});

    // Column sizing
    m_invoiceTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_invoiceTable->setColumnWidth(0, 50);                                                      // STT
    m_invoiceTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents); // Mã HĐ
    m_invoiceTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);            // Thời gian
    m_invoiceTable->setColumnWidth(2, 140);                                                     // Fixed width for Time (increased spacing)
    m_invoiceTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);            // Khách hàng
    m_invoiceTable->setColumnWidth(3, 300);                                                     // Fixed width for Customer
    m_invoiceTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);            // SĐT
    m_invoiceTable->setColumnWidth(4, 120);                                                     // Fixed width for Phone (increased spacing)
    m_invoiceTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);          // Nội dung
    m_invoiceTable->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed);            // Loại (Fixed width)
    m_invoiceTable->setColumnWidth(6, 110);                                                     // Wider column
    m_invoiceTable->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents); // Tổng tiền
    m_invoiceTable->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Fixed);
    m_invoiceTable->setColumnWidth(8, 80); // Chi tiết

    m_invoiceTable->verticalHeader()->setVisible(false);
    m_invoiceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_invoiceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_invoiceTable->setFocusPolicy(Qt::NoFocus);
    m_invoiceTable->setShowGrid(false);
    m_invoiceTable->setAlternatingRowColors(true);
    m_invoiceTable->verticalHeader()->setDefaultSectionSize(48); // Adjusted row height

    m_invoiceTable->setStyleSheet(
        "QTableWidget { "
        "background-color: white; "
        "border: 1px solid #e5e7eb; "
        "border-radius: 8px; "
        "gridline-color: #f3f4f6; "
        "} "
        "QHeaderView::section { "
        "background-color: #f9fafb; "
        "padding: 12px 8px; "
        "border: none; "
        "border-bottom: 2px solid #e5e7eb; "
        "font-weight: bold; "
        "color: #374151; "
        "} "
        "QTableWidget::item { "
        "padding: 8px; "
        "border-bottom: 1px solid #f3f4f6; "
        "} "
        "QTableWidget::item:selected { "
        "background-color: #eff6ff; "
        "color: #1e40af; "
        "}");

    mainLayout->addWidget(m_invoiceTable);
}

void InvoicePage::refreshData()
{
    loadData();
}

void InvoicePage::onTodayClicked()
{
    m_selectedDate = QDate::currentDate();
    m_datePicker->setDate(m_selectedDate);

    // Update styles
    m_todayBtn->setStyleSheet(
        "QPushButton { "
        "padding: 0 15px; "
        "border: 2px solid #16a34a; "
        "border-radius: 6px; "
        "font-weight: 600; "
        "background-color: #16a34a; "
        "color: white; "
        "}");

    m_yesterdayBtn->setStyleSheet(
        "QPushButton { "
        "padding: 0 15px; "
        "border: 2px solid #e5e7eb; "
        "border-radius: 6px; "
        "font-weight: 600; "
        "background-color: white; "
        "color: #374151; "
        "} "
        "QPushButton:hover { background-color: #f9fafb; }");

    loadData();
}

void InvoicePage::onYesterdayClicked()
{
    m_selectedDate = QDate::currentDate().addDays(-1);
    m_datePicker->setDate(m_selectedDate);

    // Update styles
    m_yesterdayBtn->setStyleSheet(
        "QPushButton { "
        "padding: 0 15px; "
        "border: 2px solid #16a34a; "
        "border-radius: 6px; "
        "font-weight: 600; "
        "background-color: #16a34a; "
        "color: white; "
        "}");

    m_todayBtn->setStyleSheet(
        "QPushButton { "
        "padding: 0 15px; "
        "border: 2px solid #e5e7eb; "
        "border-radius: 6px; "
        "font-weight: 600; "
        "background-color: white; "
        "color: #374151; "
        "} "
        "QPushButton:hover { background-color: #f9fafb; }");

    loadData();
}

void InvoicePage::onDateChanged()
{
    m_selectedDate = m_datePicker->date();
    QDate today = QDate::currentDate();
    QDate yesterday = today.addDays(-1);

    if (m_selectedDate == today)
    {
        onTodayClicked();
        return;
    }
    else if (m_selectedDate == yesterday)
    {
        onYesterdayClicked();
        return;
    }

    // Neither today nor yesterday
    m_todayBtn->setStyleSheet(
        "QPushButton { "
        "padding: 0 15px; "
        "border: 2px solid #e5e7eb; "
        "border-radius: 6px; "
        "font-weight: 600; "
        "background-color: white; "
        "color: #374151; "
        "} "
        "QPushButton:hover { background-color: #f9fafb; }");

    m_yesterdayBtn->setStyleSheet(
        "QPushButton { "
        "padding: 0 15px; "
        "border: 2px solid #e5e7eb; "
        "border-radius: 6px; "
        "font-weight: 600; "
        "background-color: white; "
        "color: #374151; "
        "} "
        "QPushButton:hover { background-color: #f9fafb; }");

    loadData();
}

void InvoicePage::onSearchTextChanged(const QString &text)
{
    Q_UNUSED(text);
    loadData(); // Reload to apply search filter
}

void InvoicePage::onTypeFilterChanged(int index)
{
    Q_UNUSED(index);
    loadData(); // Reload to apply type filter
}

void InvoicePage::onSortFilterChanged(int index)
{
    Q_UNUSED(index);
    loadData(); // Reload to apply sort filter
}

void InvoicePage::loadData()
{
    m_invoiceTable->setRowCount(0);
    double totalRevenue = 0;

    // Get filters
    QString searchText = m_searchInput->text().toLower();
    int typeFilter = m_typeFilter->currentData().toInt(); // 0: All, 1: Booking, 2: Penalty, 3: Service

    // 1. Load Bookings (DatSan)
    auto bookings = HeThongQuanLy::getInstance()->layQuanLyDatSan()->layDanhSachDatSan();

    // Use a struct to hold unified invoice data for sorting
    struct InvoiceItem
    {
        QString id;
        QDateTime time;
        QString customerName;
        QString customerPhone;
        QString content;
        QString type; // "Booking", "Penalty", "Service"
        double amount;
        void *ptr;      // Pointer to original object
        bool isBooking; // true if DatSan, false if DonHangDichVu
    };

    QList<InvoiceItem> invoiceList;

    // Process Bookings
    for (int i = 0; i < bookings.size(); ++i)
    {
        DatSan *ds = bookings[i];
        if (!ds)
            continue;

        bool isCompleted = (ds->getTrangThai() == TrangThaiDatSan::HOAN_THANH);
        bool isCancelled = (ds->getTrangThai() == TrangThaiDatSan::DA_HUY);

        if (!isCompleted && !isCancelled)
            continue;

        // Determine Type & Amount
        QString type = "Booking";
        double amount = 0;

        if (isCompleted)
        {
            type = "Booking";
            // Tính thành tiền sau giảm giá theo hạng khách hàng
            double tongTien = ds->getTongTien();
            KhachHang *kh = ds->getKhachHang();
            if (kh)
            {
                int phanTramGiam = kh->layPhanTramGiamGia();
                double giamGia = tongTien * phanTramGiam / 100.0;
                amount = tongTien - giamGia;
            }
            else
            {
                amount = tongTien;
            }
        }
        else if (isCancelled)
        {
            if (ds->getTrangThaiCoc() == TrangThaiCoc::MAT_COC)
            {
                type = "Penalty";
                amount = ds->getTienCoc();
            }
            else
            {
                // Refunded or just cancelled without deposit -> Not an invoice
                continue;
            }
        }

        // Filter by Type
        if (typeFilter == 1 && type != "Booking")
            continue;
        if (typeFilter == 2 && type != "Penalty")
            continue;
        if (typeFilter == 3)
            continue; // Filter is Service, skip bookings

        // Determine Time
        // Use Payment Date (NgayThanhToan) for sorting and display
        // But use Booking Date (ThoiGianDat) for filtering by date to match Booking List
        NgayGio ngDat = ds->getThoiGianDat();
        QDate bookingDate(ngDat.getNam(), ngDat.getThang(), ngDat.getNgay());

        // Filter by Date (Single Day) - Based on Booking Date
        if (bookingDate != m_selectedDate)
            continue;

        // Determine Payment Time for Sorting
        NgayGio ngTT = ds->getNgayThanhToan();
        if (ngTT.getNam() == 0) // Fallback if not set
            ngTT = ds->getNgayTao();

        QDateTime paymentTime = QDateTime(QDate(ngTT.getNam(), ngTT.getThang(), ngTT.getNgay()),
                                          QTime(ngTT.getGio(), ngTT.getPhut(), ngTT.getGiay()));

        // Filter by Search
        QString id = QString::fromStdString(ds->getMaDatSan());
        QString name = QString::fromStdString(ds->getKhachHang()->layHoTen());
        QString phone = QString::fromStdString(ds->getKhachHang()->getSoDienThoai());

        if (!searchText.isEmpty())
        {
            if (!id.toLower().contains(searchText) &&
                !name.toLower().contains(searchText) &&
                !phone.toLower().contains(searchText))
            {
                continue;
            }
        }

        // Content Summary
        std::string timeStr = ds->getKhungGio().layGioBatDau().toString() + " - " + ds->getKhungGio().layGioKetThuc().toString();
        QString content = QString("Sân %1 (%2)").arg(QString::fromStdString(ds->getSan()->layTenSan())).arg(QString::fromStdString(timeStr));

        invoiceList.append({id, paymentTime, name, phone, content, type, amount, ds, true});
    }

    // Process Services (DonHangDichVu) - If needed and if type filter allows
    if (typeFilter == 0 || typeFilter == 3)
    {
        auto orders = HeThongQuanLy::getInstance()->layQuanLyDonHangDichVu()->layDanhSachDonHang();
        for (int i = 0; i < orders.size(); ++i)
        {
            DonHangDichVu *dh = orders[i];
            if (!dh)
                continue;

            // Only completed orders? Assuming yes for invoices
            // if (dh->getTrangThai() != TrangThaiDonHang::HOAN_THANH) continue;

            QString type = "Service";
            double amount = dh->getThanhTien();

            NgayGio ng = dh->getNgayTao();
            QDateTime time(QDate(ng.getNam(), ng.getThang(), ng.getNgay()), QTime(ng.getGio(), ng.getPhut(), ng.getGiay()));

            if (time.date() != m_selectedDate)
                continue;

            QString id = QString::fromStdString(dh->getMaDonHang());
            QString name = dh->getKhachHang() ? QString::fromStdString(dh->getKhachHang()->layHoTen()) : "Khách vãng lai";
            QString phone = dh->getKhachHang() ? QString::fromStdString(dh->getKhachHang()->getSoDienThoai()) : "";

            if (!searchText.isEmpty())
            {
                if (!id.toLower().contains(searchText) &&
                    !name.toLower().contains(searchText) &&
                    !phone.toLower().contains(searchText))
                {
                    continue;
                }
            }

            QString content = "Đơn hàng dịch vụ"; // Could list items if available easily

            invoiceList.append({id, time, name, phone, content, type, amount, dh, false});
        }
    }

    // Sort
    int sortOption = m_sortFilter->currentIndex();
    std::sort(invoiceList.begin(), invoiceList.end(), [sortOption](const InvoiceItem &a, const InvoiceItem &b)
              {
                  switch (sortOption)
                  {
                  case 1: // Price: Low -> High
                      return a.amount < b.amount;
                  case 2: // Price: High -> Low
                      return a.amount > b.amount;
                  case 0: // Newest (Time DESC)
                  default:
                      return a.time > b.time;
                  } });

    // Populate Table
    for (int i = 0; i < invoiceList.size(); ++i)
    {
        const InvoiceItem &item = invoiceList[i];

        int row = m_invoiceTable->rowCount();
        m_invoiceTable->insertRow(row);

        // 0. STT
        QTableWidgetItem *sttItem = new QTableWidgetItem(QString::number(i + 1));
        sttItem->setTextAlignment(Qt::AlignCenter);
        m_invoiceTable->setItem(row, 0, sttItem);

        // 1. Mã HĐ
        QTableWidgetItem *idItem = new QTableWidgetItem(item.id);
        idItem->setTextAlignment(Qt::AlignCenter);
        idItem->setFont(QFont("Consolas", 10, QFont::Bold));
        m_invoiceTable->setItem(row, 1, idItem);

        // 2. Thời gian
        QTableWidgetItem *timeItem = new QTableWidgetItem(item.time.toString("dd/MM/yyyy HH:mm"));
        timeItem->setTextAlignment(Qt::AlignCenter);
        m_invoiceTable->setItem(row, 2, timeItem);

        // 3. Khách hàng
        QTableWidgetItem *customerItem = new QTableWidgetItem(item.customerName);
        customerItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        m_invoiceTable->setItem(row, 3, customerItem);

        // 4. SĐT
        QTableWidgetItem *phoneItem = new QTableWidgetItem(item.customerPhone);
        phoneItem->setTextAlignment(Qt::AlignCenter);
        m_invoiceTable->setItem(row, 4, phoneItem);

        // 5. Nội dung
        QTableWidgetItem *contentItem = new QTableWidgetItem(item.content);
        contentItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        m_invoiceTable->setItem(row, 5, contentItem);

        // 6. Loại
        QWidget *badge = nullptr;
        if (item.type == "Booking")
            badge = createStatusBadge(QString::fromUtf8("Đặt sân"), "#3b82f6"); // Blue
        else if (item.type == "Penalty")
            badge = createStatusBadge(QString::fromUtf8("Phạt cọc"), "#ef4444"); // Red
        else
            badge = createStatusBadge(QString::fromUtf8("Dịch vụ"), "#f59e0b"); // Orange

        m_invoiceTable->setCellWidget(row, 6, badge);

        // 7. Tổng tiền
        QTableWidgetItem *priceItem = new QTableWidgetItem(formatCurrency(item.amount));
        priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        priceItem->setFont(QFont("Segoe UI", 10, QFont::Bold));
        priceItem->setForeground(QBrush(QColor("#16a34a")));
        m_invoiceTable->setItem(row, 7, priceItem);

        // 8. Chi tiết
        m_invoiceTable->setCellWidget(row, 8, createActionWidget(row));

        totalRevenue += item.amount;
    }

    m_totalRevenueLabel->setText("Tổng thu: " + formatCurrency(totalRevenue));
}

QWidget *InvoicePage::createStatusBadge(const QString &text, const QString &color)
{
    QLabel *label = new QLabel(text);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(QString(
                             "background-color: %1; "
                             "color: white; "
                             "border-radius: 10px; "
                             "padding: 4px 12px; " // Increased padding
                             "font-weight: bold; "
                             "font-size: 14px;") // Increased font size
                             .arg(color));

    QWidget *container = new QWidget();
    container->setStyleSheet("background: transparent;"); // Transparent background
    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    return container;
}

QWidget *InvoicePage::createActionWidget(int row)
{
    QPushButton *btn = new QPushButton();
    btn->setIcon(QIcon("d:/PBL2-/UI/Resources/icons/eye.svg"));
    btn->setIconSize(QSize(20, 20));
    btn->setFixedSize(30, 30); // Increased size
    btn->setCursor(Qt::PointingHandCursor);
    btn->setStyleSheet(
        "QPushButton { "
        "background: transparent; " // Transparent
        "border: none; "
        "padding: 0; "
        "} "
        "QPushButton:hover { "
        "background: rgba(0, 0, 0, 0.1); " // Slight hover effect
        "border-radius: 15px; "
        "}");

    connect(btn, &QPushButton::clicked, this, [this, row]()
            { onViewDetailsClicked(row); });

    QWidget *container = new QWidget();
    container->setStyleSheet("background: transparent;"); // Transparent background
    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(btn);
    return container;
}

void InvoicePage::onViewDetailsClicked(int row)
{
    QString id = m_invoiceTable->item(row, 1)->text();

    // Determine type from ID prefix or stored data
    // Simple check: BK = Booking, DH = Order

    std::string invoiceContent = "";
    HeThongQuanLy *hethong = HeThongQuanLy::getInstance();

    if (id.startsWith("BK"))
    {
        DatSan *booking = hethong->layQuanLyDatSan()->timDatSan(id.toStdString());
        if (booking)
        {
            if (booking->getTrangThai() == TrangThaiDatSan::DA_HUY &&
                booking->getTrangThaiCoc() == TrangThaiCoc::MAT_COC)
            {
                invoiceContent = InvoiceGenerator::generateCancellationInvoice(*booking, booking->getLyDoHuy());
            }
            else
            {
                invoiceContent = InvoiceGenerator::generateBookingInvoice(*booking);
            }
        }
    }
    else
    {
        DonHangDichVu *order = hethong->layQuanLyDonHangDichVu()->timDonHang(id.toStdString());
        if (order)
        {
            invoiceContent = InvoiceGenerator::generateServiceInvoice(*order);
        }
    }

    if (!invoiceContent.empty())
    {
        InvoiceDialog dialog(invoiceContent, this);
        dialog.setReadOnly(true);
        int result = dialog.exec();

        if (result == 2) // Export requested
        {
            QString exportDir = "D:/REPORT/hoadon";
            QDir().mkpath(exportDir);

            QString defaultName = exportDir + "/HoaDon_" +
                                  QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".pdf";
            QString fileName = QFileDialog::getSaveFileName(this, "Xuất hóa đơn PDF",
                                                            defaultName, "PDF Files (*.pdf)");
            if (!fileName.isEmpty())
            {
                if (QFileInfo(fileName).suffix().isEmpty())
                    fileName.append(".pdf");

                QPrinter printer(QPrinter::PrinterResolution);
                printer.setOutputFormat(QPrinter::PdfFormat);
                printer.setPageSize(QPageSize(QPageSize::A4));
                printer.setOutputFileName(fileName);

                QTextDocument doc;
                // Use Consolas or Courier New to ensure fixed width characters in PDF
                // Also set line-height to ensure proper vertical spacing
                QString htmlContent =
                    "<html>"
                    "<head>"
                    "<style>"
                    "body { font-family: 'Consolas', 'Courier New', monospace; font-size: 11pt; white-space: pre; }"
                    "</style>"
                    "</head>"
                    "<body>" +
                    QString::fromStdString(invoiceContent).toHtmlEscaped() +
                    "</body>"
                    "</html>";

                doc.setHtml(htmlContent);

                // Set document font explicitly as fallback
                QFont font("Consolas", 11);
                font.setStyleHint(QFont::Monospace);
                doc.setDefaultFont(font);

                doc.print(&printer);

                QMessageBox::information(this, "Thành công", "Đã xuất hóa đơn thành công!");
            }
        }
    }
}

void InvoicePage::onExportClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Xuất danh sách hóa đơn", "", "Excel Files (*.csv)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        // Write BOM for Excel UTF-8
        out << "\xEF\xBB\xBF";

        // Header
        out << "STT,Mã HĐ,Thời gian,Khách hàng,SĐT,Nội dung,Loại,Tổng tiền\n";

        // Data
        for (int i = 0; i < m_invoiceTable->rowCount(); ++i)
        {
            out << m_invoiceTable->item(i, 0)->text() << ",";
            out << m_invoiceTable->item(i, 1)->text() << ",";
            out << m_invoiceTable->item(i, 2)->text() << ",";
            out << m_invoiceTable->item(i, 3)->text() << ",";
            out << m_invoiceTable->item(i, 4)->text() << ",";
            out << m_invoiceTable->item(i, 5)->text().replace(",", " ") << ",";

            // Get type from badge widget? Harder. Re-infer or store in hidden column.
            // For simplicity, just export what we can see or re-fetch.
            // Let's just put "Hóa đơn" for now or improve later.
            out << "Hóa đơn" << ",";

            out << m_invoiceTable->item(i, 7)->text().replace(",", "").replace(" VND", "").replace("đ", "") << "\n";
        }

        file.close();
        QMessageBox::information(this, "Thành công", "Đã xuất file thành công!");
    }
}

QString InvoicePage::formatCurrency(double amount) const
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(0) << amount;
    QString result = QString::fromStdString(ss.str());

    int pos = result.length() - 3;
    while (pos > 0)
    {
        result.insert(pos, '.');
        pos -= 3;
    }

    return result + " đ";
}

void InvoicePage::setUserRole(bool isAdmin)
{
    m_isAdmin = isAdmin;
    // Admin check logic if needed
}