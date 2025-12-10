#ifndef RANKINGTABLE_H
#define RANKINGTABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QScrollBar>
#include <QGraphicsDropShadowEffect>

/**
 * @class RankingTable
 * @brief Modern ranking table component with medal icons for top 3
 *
 * Features:
 * - Automatic medal icons (🥇🥈🥉) for top 3
 * - Alternating row colors
 * - Hover highlight effect
 * - Sortable columns
 * - Customizable column types (text, number, currency, percentage)
 * - Sticky header
 */
class RankingTable : public QWidget
{
    Q_OBJECT

public:
    /**
     * @enum ColumnType
     * @brief Column data type for formatting
     */
    enum ColumnType
    {
        Text,       // Plain text
        Number,     // Formatted number with locale
        Currency,   // Currency with VND suffix
        Percentage, // Percentage with % suffix
        Custom      // Custom format
    };
    Q_ENUM(ColumnType)

    /**
     * @struct ColumnConfig
     * @brief Configuration for each column
     */
    struct ColumnConfig
    {
        QString header;
        ColumnType type;
        int width; // -1 for auto stretch
        bool sortable;
        Qt::Alignment alignment;

        ColumnConfig()
            : type(Text), width(-1), sortable(true), alignment(Qt::AlignLeft | Qt::AlignVCenter) {}
        ColumnConfig(const QString &h, ColumnType t = Text, int w = -1, bool s = true,
                     Qt::Alignment a = Qt::AlignLeft | Qt::AlignVCenter)
            : header(h), type(t), width(w), sortable(s), alignment(a) {}
    };

    explicit RankingTable(QWidget *parent = nullptr);
    RankingTable(const QString &title, QWidget *parent = nullptr);
    ~RankingTable();

    // ===== CONFIGURATION =====
    void setTitle(const QString &title);
    void setSubtitle(const QString &subtitle);
    void setColumns(const QList<ColumnConfig> &columns);
    void addColumn(const ColumnConfig &column);
    void setShowRankColumn(bool show);
    void setShowMedals(bool show);
    void setMaxRows(int maxRows);

    // ===== DATA =====
    void clearData();
    void addRow(const QStringList &values);
    void setData(const QList<QStringList> &data);
    void sortByColumn(int column, Qt::SortOrder order = Qt::DescendingOrder);

    // ===== STYLING =====
    void setRowHeight(int height);
    void setHeaderHeight(int height);
    void setAlternatingRowColors(bool enable);
    void setHoverHighlight(bool enable);
    void setScrollable(bool enable);

    // ===== GETTERS =====
    QString title() const;
    int rowCount() const;
    int columnCount() const;

signals:
    void rowClicked(int row);
    void rowDoubleClicked(int row);
    void sortChanged(int column, Qt::SortOrder order);

private slots:
    void onCellClicked(int row, int column);
    void onCellDoubleClicked(int row, int column);
    void onHeaderClicked(int column);

private:
    void setupUI();
    void applyStyles();
    void updateRankColumn();
    QString formatValue(const QString &value, ColumnType type);
    QString getMedalIcon(int rank);

    // UI Components
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_headerLayout;
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    QTableWidget *m_table;

    // Configuration
    QString m_title;
    QString m_subtitle;
    QList<ColumnConfig> m_columns;
    bool m_showRankColumn;
    bool m_showMedals;
    int m_maxRows;
    int m_rowHeight;
    int m_headerHeight;
    bool m_alternatingColors;
    bool m_hoverHighlight;
    bool m_scrollable;

    // Sort state
    int m_sortColumn;
    Qt::SortOrder m_sortOrder;

    // Shadow effect
    QGraphicsDropShadowEffect *m_shadowEffect;

    // Constants
    static const int DEFAULT_ROW_HEIGHT = 48;
    static const int DEFAULT_HEADER_HEIGHT = 50;
    static const int RANK_COLUMN_WIDTH = 60;
};

#endif // RANKINGTABLE_H
