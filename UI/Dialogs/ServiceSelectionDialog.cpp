#include "ServiceSelectionDialog.h"
#include "Core/QuanLy/HeThongQuanLy.h"
#include "Core/Models/DichVu.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QScrollArea>
#include <QGridLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QMouseEvent>

// =================================================================================
// Helper Class: ServiceQuantityDialog
// =================================================================================
class ServiceQuantityDialog : public QDialog
{
public:
    ServiceQuantityDialog(DichVu *service, int maxQuantity, QWidget *parent = nullptr)
        : QDialog(parent), m_service(service), m_quantity(1), m_maxQuantity(maxQuantity)
    {
        setWindowTitle("Chọn số lượng");
        setFixedSize(500, 280);
        setStyleSheet("background-color: white; color: #1f2937;"); // Light theme

        QHBoxLayout *mainLayout = new QHBoxLayout(this);
        mainLayout->setSpacing(20);
        mainLayout->setContentsMargins(20, 20, 20, 20);

        // --- Left: Image ---
        QLabel *imgLabel = new QLabel(this);
        imgLabel->setFixedSize(160, 220);
        imgLabel->setStyleSheet("background-color: #f3f4f6; border-radius: 8px; border: 1px solid #e5e7eb;");

        // Load image from Data/images/ID.png
        QString imgPath = "D:/PBL2-/Data/images/" + QString::fromStdString(service->layMaDichVu()) + ".png";
        QPixmap pixmap(imgPath);
        if (pixmap.isNull())
        {
            imgLabel->setText("Không có ảnh");
            imgLabel->setAlignment(Qt::AlignCenter);
        }
        else
        {
            imgLabel->setPixmap(pixmap.scaled(160, 220, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imgLabel->setAlignment(Qt::AlignCenter);
        }
        mainLayout->addWidget(imgLabel);

        // --- Right: Info & Controls ---
        QVBoxLayout *rightLayout = new QVBoxLayout();
        rightLayout->setSpacing(10);
        rightLayout->setAlignment(Qt::AlignTop);

        // Name
        QLabel *lblName = new QLabel(QString::fromStdString(service->layTenDichVu()), this);
        lblName->setStyleSheet("font-size: 20px; font-weight: bold; color: #111827;");
        lblName->setWordWrap(true);
        rightLayout->addWidget(lblName);

        // Info (Unit & Stock)
        QHBoxLayout *infoLayout = new QHBoxLayout();
        QLabel *lblUnit = new QLabel("Đơn vị: " + QString::fromStdString(service->layDonVi()), this);
        QLabel *lblStock = new QLabel("Tồn kho: " + QString::number(m_maxQuantity), this);

        lblUnit->setStyleSheet("font-size: 14px; color: #4b5563;");
        lblStock->setStyleSheet("font-size: 14px; color: #4b5563;");

        infoLayout->addWidget(lblUnit);
        infoLayout->addSpacing(20);
        infoLayout->addWidget(lblStock);
        infoLayout->addStretch();
        rightLayout->addLayout(infoLayout);

        // Price
        QLabel *lblPrice = new QLabel(QString::number(service->layDonGia(), 'f', 0) + "đ", this);
        lblPrice->setStyleSheet("font-size: 24px; font-weight: bold; color: #16a34a;");
        lblPrice->setAlignment(Qt::AlignRight);
        rightLayout->addWidget(lblPrice);

        rightLayout->addStretch();

        // --- Quantity Controls (Row 1) ---
        QHBoxLayout *qtyLayout = new QHBoxLayout();
        qtyLayout->setAlignment(Qt::AlignCenter);

        // Minus Button
        QPushButton *btnMinus = new QPushButton("-", this);
        btnMinus->setFixedSize(45, 45); // Fixed square size
        btnMinus->setCursor(Qt::PointingHandCursor);
        // Improved styling for centering and visibility
        btnMinus->setStyleSheet("QPushButton { background-color: #f3f4f6; border: 1px solid #d1d5db; border-radius: 5px; font-size: 30px; font-weight: bold; color: #374151; padding: 0px; padding-bottom: 5px; } QPushButton:hover { background-color: #e5e7eb; }");

        // Quantity Box
        txtQty = new QLineEdit("1", this);
        txtQty->setFixedSize(80, 45); // Same height as buttons
        txtQty->setAlignment(Qt::AlignCenter);
        txtQty->setStyleSheet("background-color: white; border: 1px solid #d1d5db; border-radius: 5px; color: #111827; font-weight: bold; font-size: 18px;");

        // Plus Button
        QPushButton *btnPlus = new QPushButton("+", this);
        btnPlus->setFixedSize(45, 45); // Fixed square size
        btnPlus->setCursor(Qt::PointingHandCursor);
        // Improved styling for centering and visibility
        btnPlus->setStyleSheet("QPushButton { background-color: #3b82f6; border: none; border-radius: 5px; font-size: 30px; font-weight: bold; color: white; padding: 0px; padding-bottom: 5px; } QPushButton:hover { background-color: #2563eb; }");

        qtyLayout->addWidget(btnMinus);
        qtyLayout->addSpacing(10);
        qtyLayout->addWidget(txtQty);
        qtyLayout->addSpacing(10);
        qtyLayout->addWidget(btnPlus);

        rightLayout->addLayout(qtyLayout);
        rightLayout->addSpacing(15);

        // --- Action Buttons (Row 2) ---
        QHBoxLayout *btnLayout = new QHBoxLayout();

        QPushButton *btnClose = new QPushButton("Đóng", this);
        btnClose->setCursor(Qt::PointingHandCursor);
        btnClose->setFixedHeight(40);
        btnClose->setStyleSheet("QPushButton { background-color: #9ca3af; border: none; padding: 0 30px; border-radius: 5px; color: white; font-weight: bold; font-size: 14px; } QPushButton:hover { background-color: #6b7280; }");

        QPushButton *btnAdd = new QPushButton("Thêm", this);
        btnAdd->setCursor(Qt::PointingHandCursor);
        btnAdd->setFixedHeight(40);
        btnAdd->setStyleSheet("QPushButton { background-color: #16a34a; border: none; padding: 0 40px; border-radius: 5px; font-weight: bold; color: white; font-size: 14px; } QPushButton:hover { background-color: #15803d; }");

        btnLayout->addWidget(btnClose);
        btnLayout->addStretch();
        btnLayout->addWidget(btnAdd);

        rightLayout->addLayout(btnLayout);
        mainLayout->addLayout(rightLayout);

        // Logic
        connect(btnMinus, &QPushButton::clicked, [this]()
                {
            int q = txtQty->text().toInt();
            if (q > 1) txtQty->setText(QString::number(q - 1)); });
        connect(btnPlus, &QPushButton::clicked, [this]()
                {
            int q = txtQty->text().toInt();
            if (q < m_maxQuantity) txtQty->setText(QString::number(q + 1)); });
        connect(btnClose, &QPushButton::clicked, this, &QDialog::reject);
        connect(btnAdd, &QPushButton::clicked, [this]()
                {
            m_quantity = txtQty->text().toInt();
            if (m_quantity > m_maxQuantity) {
                QMessageBox::warning(this, "Lỗi", "Số lượng vượt quá tồn kho!");
                return;
            }
            accept(); });
    }

    int getQuantity() const { return m_quantity; }

private:
    DichVu *m_service;
    int m_quantity;
    int m_maxQuantity;
    QLineEdit *txtQty;
};

// =================================================================================
// ServiceSelectionDialog Implementation
// =================================================================================

ServiceSelectionDialog::ServiceSelectionDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    loadServices();
}

ServiceSelectionDialog::~ServiceSelectionDialog()
{
}

void ServiceSelectionDialog::setupUI()
{
    setWindowTitle("Chọn Dịch Vụ");
    setFixedSize(960, 750);                      // Enforce fixed size
    setStyleSheet("background-color: #f3f4f6;"); // Light gray background

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // ================= TOP FRAME: Selected Services Table (40%) =================
    QFrame *topFrame = new QFrame(this);
    topFrame->setStyleSheet("background-color: white; border-radius: 10px;");
    QVBoxLayout *topLayout = new QVBoxLayout(topFrame);

    selectedTable = new QTableWidget(this);
    selectedTable->setColumnCount(7); // Removed STT
    selectedTable->setHorizontalHeaderLabels({"Hình ảnh", "Tên dịch vụ", "Đơn giá", "Mô tả", "Đơn vị", "Số lượng", "Thành tiền"});

    selectedTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); // Name stretch
    selectedTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch); // Desc stretch

    selectedTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    selectedTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    selectedTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel); // Smooth scrolling

    // Light header style + Remove focus outline
    selectedTable->setStyleSheet(
        "QTableWidget { border: none; gridline-color: #e5e7eb; outline: none; }"
        "QHeaderView::section { background-color: #f3f4f6; color: #111827; padding: 8px; border: none; font-weight: bold; }"
        "QTableWidget::item:focus { border: none; outline: none; }"
        "QTableWidget::item:selected { background-color: #e0f2fe; color: #111827; }");
    selectedTable->verticalHeader()->setVisible(false);
    selectedTable->setIconSize(QSize(50, 50));

    connect(selectedTable, &QTableWidget::cellDoubleClicked, this, &ServiceSelectionDialog::onTableDoubleClicked);

    // Delete Button for Table
    QHBoxLayout *tableFooter = new QHBoxLayout();
    QPushButton *btnDelete = new QPushButton("Xoá dịch vụ", this);
    btnDelete->setCursor(Qt::PointingHandCursor);
    btnDelete->setStyleSheet("QPushButton { background-color: #ef4444; color: white; padding: 8px 15px; border-radius: 5px; font-weight: bold; border: none; } QPushButton:hover { background-color: #dc2626; }");
    connect(btnDelete, &QPushButton::clicked, this, &ServiceSelectionDialog::onRemoveServiceClicked);

    tableFooter->addStretch();
    tableFooter->addWidget(btnDelete);

    topLayout->addWidget(selectedTable);
    topLayout->addLayout(tableFooter);

    // ================= BOTTOM FRAME: Service Grid (60%) =================
    QFrame *bottomFrame = new QFrame(this);
    bottomFrame->setStyleSheet("background-color: white; border-radius: 10px;");
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottomFrame);

    // Search & Filter Bar
    QHBoxLayout *filterLayout = new QHBoxLayout();
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("Tìm kiếm...");
    searchEdit->setStyleSheet("padding: 8px; border: 1px solid #d1d5db; border-radius: 5px; background-color: white; color: #111827;");
    connect(searchEdit, &QLineEdit::textChanged, this, &ServiceSelectionDialog::onSearchTextChanged);

    filterCombo = new QComboBox(this);
    filterCombo->addItem("Tất cả", -1);
    filterCombo->addItem("Đồ uống", (int)LoaiDichVu::DO_UONG);
    filterCombo->addItem("Đồ ăn", (int)LoaiDichVu::DO_AN);
    filterCombo->addItem("Thiết bị", (int)LoaiDichVu::THIET_BI);
    filterCombo->setStyleSheet("padding: 8px; border: 1px solid #d1d5db; border-radius: 5px;");
    connect(filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ServiceSelectionDialog::onFilterChanged);

    filterLayout->addWidget(searchEdit);
    filterLayout->addWidget(filterCombo);
    filterLayout->addStretch();

    // Grid Area
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Disable horizontal scroll

    gridContainer = new QWidget();
    gridLayout = new QGridLayout(gridContainer);
    gridLayout->setSpacing(15);
    gridLayout->setContentsMargins(10, 10, 10, 10);
    gridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    scrollArea->setWidget(gridContainer);

    bottomLayout->addLayout(filterLayout);
    bottomLayout->addWidget(scrollArea);

    // ================= MAIN LAYOUT ASSEMBLY =================
    mainLayout->addWidget(topFrame, 4);
    mainLayout->addWidget(bottomFrame, 6);

    // ================= BOTTOM BUTTONS =================
    QHBoxLayout *footerLayout = new QHBoxLayout();
    QPushButton *btnCancel = new QPushButton("Huỷ bỏ", this);
    btnCancel->setCursor(Qt::PointingHandCursor);
    btnCancel->setStyleSheet("QPushButton { background-color: #6b7280; color: white; padding: 10px 20px; border-radius: 5px; font-weight: bold; border: none; } QPushButton:hover { background-color: #4b5563; }");
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);

    QPushButton *btnAdd = new QPushButton("Thêm vào", this);
    btnAdd->setCursor(Qt::PointingHandCursor);
    btnAdd->setStyleSheet("QPushButton { background-color: #2563eb; color: white; padding: 10px 20px; border-radius: 5px; font-weight: bold; border: none; } QPushButton:hover { background-color: #1d4ed8; }");
    connect(btnAdd, &QPushButton::clicked, this, &ServiceSelectionDialog::onConfirmClicked);

    footerLayout->addStretch();
    footerLayout->addWidget(btnCancel);
    footerLayout->addWidget(btnAdd);

    mainLayout->addLayout(footerLayout);
}

void ServiceSelectionDialog::loadServices()
{
    HeThongQuanLy *system = HeThongQuanLy::getInstance();
    if (!system)
        return;

    QuanLyDichVu *qldv = system->layQuanLyDichVu();
    if (!qldv)
        return;

    allServices.clear();
    const MangDong<DichVu *> &ds = qldv->layDanhSachDichVu();
    for (int i = 0; i < ds.size(); ++i)
    {
        allServices.push_back(ds[i]);
    }

    updateGrid();
}

void ServiceSelectionDialog::updateGrid()
{
    // Clear grid
    QLayoutItem *child;
    while ((child = gridLayout->takeAt(0)) != nullptr)
    {
        if (child->widget())
            delete child->widget();
        delete child;
    }

    // Filter
    displayedServices.clear();
    QString searchText = searchEdit->text().trimmed().toLower();
    int filterType = filterCombo->currentData().toInt();

    for (DichVu *dv : allServices)
    {
        if (!dv)
            continue;
        if (!searchText.isEmpty() && !QString::fromStdString(dv->layTenDichVu()).toLower().contains(searchText))
            continue;
        if (filterType != -1 && (int)dv->layLoaiDichVu() != filterType)
            continue;
        displayedServices.push_back(dv);
    }

    // Populate Grid
    int row = 0;
    int col = 0;
    int maxCols = 4; // 4 columns for wider dialog

    for (DichVu *dv : displayedServices)
    {
        QWidget *card = createServiceCard(dv);
        gridLayout->addWidget(card, row, col);
        col++;
        if (col >= maxCols)
        {
            col = 0;
            row++;
        }
    }
}

// Custom Clickable Frame
class ClickableFrame : public QFrame
{
    Q_OBJECT
public:
    explicit ClickableFrame(QWidget *parent = nullptr) : QFrame(parent) {}
signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
            emit clicked();
        QFrame::mousePressEvent(event);
    }
};

QWidget *ServiceSelectionDialog::createServiceCard(DichVu *service)
{
    ClickableFrame *card = new ClickableFrame();
    card->setFixedSize(210, 260); // Adjusted width for 960px dialog (4 items/row)
    card->setStyleSheet(
        "ClickableFrame { background-color: white; border: 1px solid #e5e7eb; border-radius: 8px; }"
        "ClickableFrame:hover { border: 2px solid #2563eb; }");

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(5);

    // Image
    QLabel *img = new QLabel();
    img->setAlignment(Qt::AlignCenter);
    img->setStyleSheet("background-color: #f3f4f6; border-radius: 5px;");
    img->setFixedHeight(140);

    QString imgPath = "D:/PBL2-/Data/images/" + QString::fromStdString(service->layMaDichVu()) + ".png";
    QPixmap pixmap(imgPath);
    if (!pixmap.isNull())
    {
        img->setPixmap(pixmap.scaled(190, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else
    {
        img->setText("Không có ảnh");
    }

    // Name
    QLabel *name = new QLabel(QString::fromStdString(service->layTenDichVu()));
    name->setWordWrap(true);
    name->setStyleSheet("font-weight: bold; font-size: 14px; border: none; color: #111827;");
    name->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // Unit
    QLabel *unit = new QLabel(QString::fromStdString(service->layDonVi()));
    unit->setStyleSheet("color: #6b7280; font-size: 12px; border: none;");
    unit->setAlignment(Qt::AlignLeft);

    // Calculate available stock (Total - In Cart - Currently Selected)
    int inCartQty = existingCart.value(service->layMaDichVu(), 0);
    int currentSelectedQty = selectedQuantities.value(service->layMaDichVu(), 0);
    int displayStock = service->laySoLuongTon() - inCartQty - currentSelectedQty;
    if (displayStock < 0)
        displayStock = 0;

    QLabel *lblStock = new QLabel(QString("Tồn kho: %1").arg(displayStock));
    lblStock->setStyleSheet("color: #6b7280; font-size: 12px; border: none;");
    lblStock->setAlignment(Qt::AlignLeft);

    // Price
    QLabel *price = new QLabel(QString::number(service->layDonGia(), 'f', 0) + "đ");
    price->setStyleSheet("color: #16a34a; font-weight: bold; font-size: 15px; border: none;");
    price->setAlignment(Qt::AlignLeft);

    layout->addWidget(img);
    layout->addWidget(name);
    layout->addWidget(unit);
    layout->addWidget(lblStock);
    layout->addWidget(price);
    layout->addStretch();

    connect(card, &ClickableFrame::clicked, [this, service]()
            { onServiceCardClicked(service); });

    return card;
}

void ServiceSelectionDialog::onServiceCardClicked(DichVu *service)
{
    if (!service)
        return;

    int currentQty = selectedQuantities.value(service->layMaDichVu(), 0);
    int inCartQty = existingCart.value(service->layMaDichVu(), 0);
    int stock = service->laySoLuongTon();
    int maxQty = stock - currentQty - inCartQty;

    if (maxQty <= 0)
    {
        QMessageBox::warning(this, "Hết hàng", "Sản phẩm này đã hết hàng hoặc bạn đã chọn hết số lượng tồn kho!");
        return;
    }

    openQuantityDialog(service, maxQty);
}

void ServiceSelectionDialog::onTableDoubleClicked(int row, int column)
{
    // Check if row is valid and item exists
    if (row < 0 || row >= selectedTable->rowCount())
        return;

    QTableWidgetItem *nameItem = selectedTable->item(row, 1);
    if (!nameItem)
        return;

    // Name is at column 1 (0: Image, 1: Name)
    QString name = nameItem->text();
    DichVu *target = nullptr;
    for (auto s : allServices)
    {
        if (s && QString::fromStdString(s->layTenDichVu()) == name)
        {
            target = s;
            break;
        }
    }

    if (target)
    {
        int currentQty = selectedQuantities.value(target->layMaDichVu(), 0);
        int inCartQty = existingCart.value(target->layMaDichVu(), 0);
        int maxQty = target->laySoLuongTon() - currentQty - inCartQty;

        if (maxQty <= 0)
        {
            QMessageBox::warning(this, "Hết hàng", "Sản phẩm này đã hết hàng hoặc bạn đã chọn hết số lượng tồn kho!");
            return;
        }

        openQuantityDialog(target, maxQty);
    }
}

void ServiceSelectionDialog::openQuantityDialog(DichVu *service, int maxQty)
{
    if (!service || maxQty <= 0)
        return;

    ServiceQuantityDialog dlg(service, maxQty, this);
    if (dlg.exec() == QDialog::Accepted)
    {
        int newQty = dlg.getQuantity();
        if (newQty > 0)
        {
            selectedQuantities[service->layMaDichVu()] += newQty;
        }
        updateTable();
        updateGrid(); // Refresh grid to update stock display
    }
}

void ServiceSelectionDialog::updateTable()
{
    selectedTable->setRowCount(0);

    // Create a list of IDs to remove (services that no longer exist)
    QList<std::string> idsToRemove;

    for (auto it = selectedQuantities.begin(); it != selectedQuantities.end(); ++it)
    {
        std::string id = it.key();
        int qty = it.value();

        if (qty <= 0)
        {
            idsToRemove.append(id);
            continue;
        }

        DichVu *dv = nullptr;
        for (auto s : allServices)
        {
            if (s && s->layMaDichVu() == id)
            {
                dv = s;
                break;
            }
        }

        if (!dv)
        {
            idsToRemove.append(id);
            continue;
        }
        int row = selectedTable->rowCount();
        selectedTable->insertRow(row);
        selectedTable->setRowHeight(row, 60); // Reduced row height to 60

        // Image (Col 0)
        QTableWidgetItem *imgItem = new QTableWidgetItem();
        QString imgPath = "D:/PBL2-/Data/images/" + QString::fromStdString(dv->layMaDichVu()) + ".png";
        QPixmap pixmap(imgPath);
        if (!pixmap.isNull())
        {
            imgItem->setIcon(QIcon(pixmap));
        }
        else
        {
            imgItem->setText("No Img");
        }
        selectedTable->setItem(row, 0, imgItem);

        // Name (Col 1)
        selectedTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(dv->layTenDichVu())));
        // Price (Col 2)
        selectedTable->setItem(row, 2, new QTableWidgetItem(QString::number(dv->layDonGia(), 'f', 0) + "đ"));
        // Desc (Col 3)
        selectedTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(dv->layMoTa())));
        // Unit (Col 4)
        selectedTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(dv->layDonVi())));
        // Qty (Col 5)
        selectedTable->setItem(row, 5, new QTableWidgetItem(QString::number(qty)));

        double total = dv->layDonGia() * qty;
        // Total (Col 6)
        selectedTable->setItem(row, 6, new QTableWidgetItem(QString::number(total, 'f', 0) + "đ"));
    }

    // Remove invalid entries
    for (const std::string &id : idsToRemove)
    {
        selectedQuantities.remove(id);
    }
}

void ServiceSelectionDialog::setExistingCart(const QMap<std::string, int> &cart)
{
    existingCart = cart;
    // Refresh grid to update stock display
    updateGrid();
}

void ServiceSelectionDialog::onSearchTextChanged(const QString &text) { updateGrid(); }
void ServiceSelectionDialog::onFilterChanged(int index) { updateGrid(); }

void ServiceSelectionDialog::onRemoveServiceClicked()
{
    int row = selectedTable->currentRow();
    if (row < 0 || row >= selectedTable->rowCount())
    {
        QMessageBox::warning(this, "Thông báo", "Vui lòng chọn dịch vụ để xoá!");
        return;
    }

    // Check if item exists at column 1
    QTableWidgetItem *nameItem = selectedTable->item(row, 1);
    if (!nameItem)
        return;

    // Name is at column 1
    QString name = nameItem->text();

    std::string idToRemove = "";
    for (auto s : allServices)
    {
        if (QString::fromStdString(s->layTenDichVu()) == name)
        {
            idToRemove = s->layMaDichVu();
            break;
        }
    }

    if (!idToRemove.empty())
    {
        selectedQuantities.remove(idToRemove);
        updateTable();
        updateGrid(); // Refresh grid to update stock display
    }
}

void ServiceSelectionDialog::onConfirmClicked() { accept(); }

QMap<std::string, int> ServiceSelectionDialog::getSelectedServices() const
{
    return selectedQuantities;
}

#include "ServiceSelectionDialog.moc"
