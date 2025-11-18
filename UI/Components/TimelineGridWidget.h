/**
 * @file TimelineGridWidget.h
 * @brief Custom widget for timeline grid with drag-to-create booking
 * @details Displays time slots (06:00-23:00) vs fields (Field 5-1 to 5-14)
 *          Supports drag-to-create booking (click and drag vertically)
 *          Displays existing bookings as blue blocks
 *
 * @author Football Field Management System
 * @date 2025-11-14
 */

#ifndef TIMELINEGRIDWIDGET_H
#define TIMELINEGRIDWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QDate>
#include <QPoint>
#include <QRect>
#include <QPainter>
#include <QMouseEvent>
#include "../../Core/QuanLy/HeThongQuanLy.h"
#include "../../Core/Models/DatSan.h"
#include "../../Core/Models/San.h"

/**
 * @struct BookingBlock
 * @brief Represents a booking block on the timeline
 */
struct BookingBlock
{
    DatSan *booking;
    int fieldIndex;
    int startHour;
    int startMinute;
    int durationMinutes;
    QRect rect;   // Rendered rectangle
    QColor color; // Status-based color
    bool isPaid;  // Payment status

    BookingBlock(DatSan *b, int fi, int sh, int sm, int dm)
        : booking(b), fieldIndex(fi), startHour(sh), startMinute(sm), durationMinutes(dm),
          color(QColor("#3b82f6")), isPaid(false) {}
};

/**
 * @class TimelineGridWidget
 * @brief Timeline grid with drag-to-create functionality
 */
class TimelineGridWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimelineGridWidget(QWidget *parent = nullptr);
    ~TimelineGridWidget();

    /**
     * @brief Set selected date
     */
    void setDate(const QDate &date);

    /**
     * @brief Load bookings for current date
     */
    void loadBookings();

    /**
     * @brief Clear pending selection (when user cancels)
     */
    void clearPendingSelection();

signals:
    /**
     * @brief Emitted when user drag-selects a time slot
     * @param fieldIndex Index of selected field
     * @param startHour Starting hour
     * @param startMinute Starting minute
     * @param durationMinutes Duration in minutes (minimum 30)
     */
    void slotSelected(int fieldIndex, int startHour, int startMinute, int durationMinutes);

    /**
     * @brief Emitted when user clicks an existing booking block
     * @param booking The booking that was clicked
     */
    void bookingClicked(DatSan *booking);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

private:
    void drawGrid(QPainter &painter);
    void drawTimeLabels(QPainter &painter);
    void drawFieldHeaders(QPainter &painter);
    void drawBookingBlocks(QPainter &painter);
    void drawDragSelection(QPainter &painter);

    int getFieldIndexAtX(int x) const;
    int getTimeSlotAtY(int y, int &outHour, int &outMinute) const;
    BookingBlock *getBookingBlockAt(const QPoint &pos);
    QColor getBookingColor(DatSan *booking) const;
    bool isBookingPaid(DatSan *booking) const;
    void showBookingContextMenu(const QPoint &pos, BookingBlock *block);

    void calculateGeometry();

private:
    // ===== CONSTANTS =====
    static constexpr int START_HOUR = 6;   // 06:00
    static constexpr int END_HOUR = 22;    // 22:00
    static constexpr int HOUR_HEIGHT = 60; // pixels per hour
    static constexpr int HEADER_HEIGHT = 40;
    static constexpr int TIME_LABEL_WIDTH = 60; // Width for time column
    static constexpr int MIN_FIELD_WIDTH = 100;
    static constexpr int MIN_DURATION_MINUTES = 60; // Cố định 1 tiếng

    // ===== DATA =====
    HeThongQuanLy *system;
    QDate currentDate;
    MangDong<San *> fields;
    QVector<BookingBlock *> bookingBlocks;

    // ===== DRAG STATE =====
    bool isDragging;
    QPoint dragStartPos;
    QPoint dragCurrentPos;
    int dragFieldIndex;
    int dragStartHour;
    int dragStartMinute;

    // ===== PENDING SELECTION =====
    bool hasPendingSelection;
    int pendingFieldIndex;
    int pendingStartHour;
    int pendingStartMinute;
    int pendingDurationMinutes;

    // ===== GEOMETRY =====
    int totalWidth;
    int totalHeight;
    int fieldWidth;
    int numFields;
    int numHours;
};

#endif // TIMELINEGRIDWIDGET_H
