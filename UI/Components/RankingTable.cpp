#include "RankingTable.h"
#include <QLocale>
#include <QScrollArea>

// ========== CONSTRUCTORS ==========

RankingTable::RankingTable(QWidget *parent)
    : QWidget(parent),
      m_mainLayout(nullptr),
      m_headerLayout(nullptr),
      m_titleLabel(nullptr),
      m_subtitleLabel(nullptr),
      m_table(nullptr),
      m_showRankColumn(true),
      m_showMedals(true),
      m_maxRows(-1),
      m_rowHeight(DEFAULT_ROW_HEIGHT),
      m_headerHeight(DEFAULT_HEADER_HEIGHT),
      m_alternatingColors(true),
      m_hoverHighlight(true),
      m_scrollable(true),
      m_sortColumn(-1),
      m_sortOrder(Qt::DescendingOrder),
      m_shadowEffect(nullptr)
{
    setupUI();
    applyStyles();
}

RankingTable::RankingTable(const QString &title, QWidget *parent)
    : RankingTable(parent)
{
    setTitle(title);
}

RankingTable::~RankingTable()
{
}

// ========== SETUP UI ==========

void RankingTable::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 16);
    m_mainLayout->setSpacing(12);

    // Header section
    m_headerLayout = new QHBoxLayout();
    m_headerLayout->setContentsMargins(20, 20, 20, 8);

    m_titleLabel = new QLabel(this);
    m_titleLabel->setStyleSheet(R"(
        QLabel {
            color: #1e293b;
            font-size: 18px;
            font-weight: 600;
        }
    )");

    m_subtitleLabel = new QLabel(this);
    m_subtitleLabel->setStyleSheet(R"(
        QLabel {
            color: #64748b;
            font-size: 13px;
        }
    )");
    m_subtitleLabel->hide();

    m_headerLayout->addWidget(m_titleLabel);
    m_headerLayout->addStretch();
    m_headerLayout->addWidget(m_subtitleLabel);

    // Table
    m_table = new QTableWidget(this);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setShowGrid(false);
    m_table->setFocusPolicy(Qt::NoFocus);
    m_table->verticalHeader()->setVisible(false);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setHighlightSections(false);
    m_table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    // Connect signals
    connect(m_table, &QTableWidget::cellClicked, this, &RankingTable::onCellClicked);
    connect(m_table, &QTableWidget::cellDoubleClicked, this, &RankingTable::onCellDoubleClicked);
    connect(m_table->horizontalHeader(), &QHeaderView::sectionClicked, this, &RankingTable::onHeaderClicked);

    m_mainLayout->addLayout(m_headerLayout);
    m_mainLayout->addWidget(m_table, 1);

    // Shadow effect
    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setBlurRadius(15);
    m_shadowEffect->setXOffset(0);
    m_shadowEffect->setYOffset(2);
    m_shadowEffect->setColor(QColor(0, 0, 0, 25));
    setGraphicsEffect(m_shadowEffect);
}

void RankingTable::applyStyles()
{
    // Ensure widget paints background
    setAutoFillBackground(true);

    setStyleSheet(R"(
        RankingTable {
            background-color: white;
            border-radius: 12px;
            border: 1px solid #e2e8f0;
        }
    )");

    m_table->setStyleSheet(QString(R"(
        QTableWidget {
            background-color: white;
            border: none;
            border-radius: 0 0 12px 12px;
        }
        QTableWidget::item {
            padding: 8px 12px;
            border-bottom: 1px solid #f1f5f9;
        }
        QTableWidget::item:selected {
            background-color: #eff6ff;
            color: #1e293b;
        }
        QTableWidget::item:hover {
            background-color: #f8fafc;
        }
        QHeaderView::section {
            background-color: #f8fafc;
            color: #475569;
            font-weight: 600;
            font-size: 12px;
            text-transform: uppercase;
            padding: 12px 12px;
            border: none;
            border-bottom: 2px solid #e2e8f0;
        }
        QHeaderView::section:hover {
            background-color: #f1f5f9;
        }
        QScrollBar:vertical {
            background: #f1f5f9;
            width: 8px;
            border-radius: 4px;
            margin: 0;
        }
        QScrollBar::handle:vertical {
            background: #cbd5e1;
            border-radius: 4px;
            min-height: 30px;
        }
        QScrollBar::handle:vertical:hover {
            background: #94a3b8;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0;
        }
    )"));
}

// ========== CONFIGURATION ==========

void RankingTable::setTitle(const QString &title)
{
    m_title = title;
    m_titleLabel->setText(title);
}

void RankingTable::setSubtitle(const QString &subtitle)
{
    m_subtitle = subtitle;
    m_subtitleLabel->setText(subtitle);
    m_subtitleLabel->setVisible(!subtitle.isEmpty());
}

void RankingTable::setColumns(const QList<ColumnConfig> &columns)
{
    m_columns = columns;

    int totalColumns = m_showRankColumn ? columns.size() + 1 : columns.size();
    m_table->setColumnCount(totalColumns);

    QStringList headers;
    if (m_showRankColumn)
    {
        headers << "Rank";
    }
    for (const auto &col : columns)
    {
        headers << col.header;
    }
    m_table->setHorizontalHeaderLabels(headers);

    // Set column widths
    int colIndex = 0;
    if (m_showRankColumn)
    {
        m_table->setColumnWidth(colIndex++, RANK_COLUMN_WIDTH);
    }
    for (const auto &col : columns)
    {
        if (col.width > 0)
        {
            m_table->setColumnWidth(colIndex, col.width);
        }
        colIndex++;
    }

    // Set header height
    m_table->horizontalHeader()->setFixedHeight(m_headerHeight);
}

void RankingTable::addColumn(const ColumnConfig &column)
{
    m_columns.append(column);
    setColumns(m_columns);
}

void RankingTable::setShowRankColumn(bool show)
{
    m_showRankColumn = show;
    setColumns(m_columns);
}

void RankingTable::setShowMedals(bool show)
{
    m_showMedals = show;
    updateRankColumn();
}

void RankingTable::setMaxRows(int maxRows)
{
    m_maxRows = maxRows;
}

// ========== DATA ==========

void RankingTable::clearData()
{
    m_table->setRowCount(0);
}

void RankingTable::addRow(const QStringList &values)
{
    if (m_maxRows > 0 && m_table->rowCount() >= m_maxRows)
    {
        return;
    }

    int row = m_table->rowCount();
    m_table->insertRow(row);
    m_table->setRowHeight(row, m_rowHeight);

    int colIndex = 0;

    // Rank column
    if (m_showRankColumn)
    {
        QString rankText = getMedalIcon(row + 1);
        QTableWidgetItem *rankItem = new QTableWidgetItem(rankText);
        rankItem->setTextAlignment(Qt::AlignCenter);

        // Style based on rank
        if (row < 3)
        {
            QFont font = rankItem->font();
            font.setPointSize(16);
            rankItem->setFont(font);
        }
        else
        {
            rankItem->setText(QString::number(row + 1));
            QFont font = rankItem->font();
            font.setBold(true);
            rankItem->setFont(font);
            rankItem->setForeground(QColor("#64748b"));
        }

        m_table->setItem(row, colIndex++, rankItem);
    }

    // Data columns
    for (int i = 0; i < values.size() && i < m_columns.size(); ++i)
    {
        QString formattedValue = formatValue(values[i], m_columns[i].type);
        QTableWidgetItem *item = new QTableWidgetItem(formattedValue);
        item->setTextAlignment(m_columns[i].alignment);

        // Style for currency/number columns
        if (m_columns[i].type == Currency || m_columns[i].type == Number)
        {
            QFont font = item->font();
            font.setBold(true);
            item->setFont(font);
        }

        m_table->setItem(row, colIndex++, item);
    }

    // Apply alternating colors
    if (m_alternatingColors && row % 2 == 1)
    {
        for (int c = 0; c < m_table->columnCount(); ++c)
        {
            QTableWidgetItem *item = m_table->item(row, c);
            if (item)
            {
                item->setBackground(QColor("#fafafa"));
            }
        }
    }
}

void RankingTable::setData(const QList<QStringList> &data)
{
    clearData();
    for (const auto &row : data)
    {
        addRow(row);
    }
}

void RankingTable::sortByColumn(int column, Qt::SortOrder order)
{
    m_sortColumn = column;
    m_sortOrder = order;
    m_table->sortItems(column, order);
    updateRankColumn();
}

// ========== STYLING ==========

void RankingTable::setRowHeight(int height)
{
    m_rowHeight = height;
    for (int i = 0; i < m_table->rowCount(); ++i)
    {
        m_table->setRowHeight(i, height);
    }
}

void RankingTable::setHeaderHeight(int height)
{
    m_headerHeight = height;
    m_table->horizontalHeader()->setFixedHeight(height);
}

void RankingTable::setAlternatingRowColors(bool enable)
{
    m_alternatingColors = enable;
}

void RankingTable::setHoverHighlight(bool enable)
{
    m_hoverHighlight = enable;
}

void RankingTable::setScrollable(bool enable)
{
    m_scrollable = enable;
    if (enable)
    {
        m_table->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        m_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else
    {
        m_table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

// ========== GETTERS ==========

QString RankingTable::title() const { return m_title; }
int RankingTable::rowCount() const { return m_table->rowCount(); }
int RankingTable::columnCount() const { return m_table->columnCount(); }

// ========== SLOTS ==========

void RankingTable::onCellClicked(int row, int column)
{
    Q_UNUSED(column);
    emit rowClicked(row);
}

void RankingTable::onCellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    emit rowDoubleClicked(row);
}

void RankingTable::onHeaderClicked(int column)
{
    // Skip rank column from sorting
    if (m_showRankColumn && column == 0)
    {
        return;
    }

    // Toggle sort order if same column
    if (column == m_sortColumn)
    {
        m_sortOrder = (m_sortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
    }
    else
    {
        m_sortColumn = column;
        m_sortOrder = Qt::DescendingOrder;
    }

    m_table->sortItems(column, m_sortOrder);
    updateRankColumn();
    emit sortChanged(column, m_sortOrder);
}

// ========== PRIVATE METHODS ==========

void RankingTable::updateRankColumn()
{
    if (!m_showRankColumn)
        return;

    for (int row = 0; row < m_table->rowCount(); ++row)
    {
        QTableWidgetItem *item = m_table->item(row, 0);
        if (item)
        {
            QString rankText = getMedalIcon(row + 1);
            if (row < 3)
            {
                item->setText(rankText);
                QFont font = item->font();
                font.setPointSize(16);
                item->setFont(font);
            }
            else
            {
                item->setText(QString::number(row + 1));
                QFont font = item->font();
                font.setPointSize(11);
                font.setBold(true);
                item->setFont(font);
                item->setForeground(QColor("#64748b"));
            }
        }
    }
}

QString RankingTable::formatValue(const QString &value, ColumnType type)
{
    switch (type)
    {
    case Number:
    {
        bool ok;
        double num = value.toDouble(&ok);
        if (ok)
        {
            QLocale locale(QLocale::Vietnamese);
            return locale.toString(num, 'f', 0);
        }
        return value;
    }
    case Currency:
    {
        bool ok;
        double num = value.toDouble(&ok);
        if (ok)
        {
            QLocale locale(QLocale::Vietnamese);
            return locale.toString(num, 'f', 0) + " VND";
        }
        return value;
    }
    case Percentage:
    {
        bool ok;
        double num = value.toDouble(&ok);
        if (ok)
        {
            return QString::number(num, 'f', 1) + "%";
        }
        return value;
    }
    case Text:
    case Custom:
    default:
        return value;
    }
}

QString RankingTable::getMedalIcon(int rank)
{
    if (!m_showMedals)
    {
        return QString::number(rank);
    }

    switch (rank)
    {
    case 1:
        return QString::fromUtf8("🥇");
    case 2:
        return QString::fromUtf8("🥈");
    case 3:
        return QString::fromUtf8("🥉");
    default:
        return QString::number(rank);
    }
}
