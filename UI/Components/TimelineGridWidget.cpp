/**
 * @file TimelineGridWidget.cpp
 * @brief Implementation of TimelineGridWidget
 */

#include "TimelineGridWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QScrollBar>
#include <QMenu>
#include <QMessageBox>
#include <QContextMenuEvent>
#include <cmath>
#include <functional>

TimelineGridWidget::TimelineGridWidget(DisplayMode mode, QWidget *parent)
    : QWidget(parent), system(nullptr), currentDate(QDate::currentDate()), isDragging(false), dragFieldIndex(-1), dragStartHour(0), dragStartMinute(0), hasPendingSelection(false), pendingFieldIndex(-1), pendingStartHour(0), pendingStartMinute(0), pendingDurationMinutes(0), totalWidth(0), totalHeight(0), fieldWidth(MIN_FIELD_WIDTH), numFields(0), numHours(END_HOUR - START_HOUR + 1), displayMode(mode)
{
    system = HeThongQuanLy::getInstance();

    setMouseTracking(true);

    // Adjust minimum height based on mode
    if (displayMode == HeaderOnly || displayMode == CornerOnly)
    {
        setMinimumHeight(HEADER_HEIGHT);
        setMaximumHeight(HEADER_HEIGHT);
    }
    else
    {
        setMinimumHeight(numHours * HOUR_HEIGHT);
    }

    setContextMenuPolicy(Qt::DefaultContextMenu);

    // Load fields
    QuanLySan *quanLySan = system->layQuanLySan();
    if (quanLySan)
    {
        fields = quanLySan->layDanhSachSan();
        numFields = fields.size();
    }

    calculateGeometry();
    loadBookings();
}

void TimelineGridWidget::setExcludedBooking(DatSan *booking)
{
    excludedBooking = booking;
    loadBookings(); // Reload to apply changes
}

TimelineGridWidget::~TimelineGridWidget()
{
    // Clean up booking blocks
    for (BookingBlock *block : bookingBlocks)
    {
        delete block;
    }
    bookingBlocks.clear();
}

void TimelineGridWidget::setDate(const QDate &date)
{
    currentDate = date;
    loadBookings();
    update();
}

void TimelineGridWidget::setFields(const MangDong<San *> &newFields)
{
    fields = newFields;
    numFields = fields.size();
    calculateGeometry();
    loadBookings();
    update();
}

void TimelineGridWidget::loadBookings()
{
    // Clear existing blocks
    for (BookingBlock *block : bookingBlocks)
    {
        delete block;
    }
    bookingBlocks.clear();

    // Load bookings from Core for current date
    QuanLyDatSan *quanLyDS = system->layQuanLyDatSan();
    if (!quanLyDS)
    {
        return;
    }

    const MangDong<DatSan *> &allBookings = quanLyDS->layDanhSachDatSan();

    for (int i = 0; i < allBookings.size(); i++)
    {
        DatSan *booking = allBookings[i];
        if (!booking)
            continue;

        // Skip excluded booking (for reschedule)
        if (booking == excludedBooking)
        {
            continue;
        }

        // Check if booking is on current date
        NgayGio ngayGio = booking->getThoiGianDat();
        QDate bookingDate(ngayGio.getNam(), ngayGio.getThang(), ngayGio.getNgay());

        if (bookingDate != currentDate)
        {
            continue;
        }

        // Find field index
        San *san = booking->getSan();
        int fieldIndex = -1;
        for (int j = 0; j < fields.size(); j++)
        {
            if (fields[j] == san)
            {
                fieldIndex = j;
                break;
            }
        }

        if (fieldIndex < 0)
        {
            continue;
        }

        // Get time info
        // FIX: Use KhungGio for start time, not NgayGio (which is just the date)
        KhungGio khungGio = booking->getKhungGio();
        ThoiGian gioBD = khungGio.getGioBatDau();
        ThoiGian gioKT = khungGio.getGioKetThuc();

        int startHour = gioBD.getGio();
        int startMinute = gioBD.getPhut();

        // Calculate duration from start and end time
        int startMinutes = gioBD.getGio() * 60 + gioBD.getPhut();
        int endMinutes = gioKT.getGio() * 60 + gioKT.getPhut();
        int durationMinutes = endMinutes - startMinutes;
        if (durationMinutes < 0)
            durationMinutes = 0;

        // Create booking block
        BookingBlock *block = new BookingBlock(booking, fieldIndex, startHour, startMinute, durationMinutes);

        // ===== SET COLOR BASED ON STATUS =====
        block->color = getBookingColor(booking);
        block->isPaid = isBookingPaid(booking);

        bookingBlocks.append(block);
    }

    update();
}

QColor TimelineGridWidget::getBookingColor(DatSan *booking) const
{
    if (!booking)
        return QColor("#3b82f6"); // Default blue

    // D\u00e0nh s\u00e1ch m\u00e0u s\u1eaffc \u0111a d\u1ea1ng (tr\u1eeb \u0111en x\u00e1m)
    // S\u1eed d\u1ee5ng m\u00e3 \u0111\u1eb7t s\u00e2n \u0111\u1ec3 ch\u1ecdn m\u00e0u nh\u1ea5t qu\u00e1n
    static const QVector<QColor> colors = {
        QColor("#3b82f6"), // Blue
        QColor("#16a34a"), // Green
        QColor("#f59e0b"), // Orange
        QColor("#8b5cf6"), // Purple
        QColor("#ec4899"), // Pink
        QColor("#06b6d4"), // Cyan
        QColor("#10b981"), // Emerald
        QColor("#f97316"), // Deep Orange
        QColor("#6366f1"), // Indigo
        QColor("#14b8a6"), // Teal
        QColor("#a855f7"), // Violet
        QColor("#0ea5e9"), // Sky
    };

    // S\u1eed d\u1ee5ng hash c\u1ee7a m\u00e3 \u0111\u1eb7t s\u00e2n \u0111\u1ec3 ch\u1ecdn m\u00e0u
    std::string bookingId = booking->getMaDatSan();
    std::hash<std::string> hasher;
    size_t hash = hasher(bookingId);
    int colorIndex = hash % colors.size();

    return colors[colorIndex];
}

bool TimelineGridWidget::isBookingPaid(DatSan *booking) const
{
    if (!booking)
        return false;

    // TODO: Check payment status from Core
    // For now, assume not paid
    return false;
}

void TimelineGridWidget::calculateGeometry()
{
    if (numFields > 0)
    {
        fieldWidth = MIN_FIELD_WIDTH;
    }

    // Calculate dimensions based on mode
    switch (displayMode)
    {
    case Full:
        totalWidth = TIME_LABEL_WIDTH + numFields * fieldWidth;
        totalHeight = HEADER_HEIGHT + numHours * HOUR_HEIGHT;
        break;
    case HeaderOnly:
        totalWidth = numFields * fieldWidth;
        totalHeight = HEADER_HEIGHT;
        break;
    case TimeOnly:
        totalWidth = TIME_LABEL_WIDTH;
        totalHeight = numHours * HOUR_HEIGHT;
        break;
    case GridOnly:
        totalWidth = numFields * fieldWidth;
        totalHeight = numHours * HOUR_HEIGHT;
        break;
    case CornerOnly:
        totalWidth = TIME_LABEL_WIDTH;
        totalHeight = HEADER_HEIGHT;
        break;
    }

    setMinimumWidth(totalWidth);
    setMinimumHeight(totalHeight);

    // For HeaderOnly and CornerOnly, fix the height
    if (displayMode == HeaderOnly || displayMode == CornerOnly)
    {
        setMaximumHeight(HEADER_HEIGHT);
    }
    // For TimeOnly and CornerOnly, fix the width
    if (displayMode == TimeOnly || displayMode == CornerOnly)
    {
        setMaximumWidth(TIME_LABEL_WIDTH);
    }
}

void TimelineGridWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Background
    painter.fillRect(rect(), QColor(255, 255, 255));

    switch (displayMode)
    {
    case Full:
        drawGrid(painter);
        drawTimeLabels(painter);
        drawFieldHeaders(painter);
        drawBookingBlocks(painter);
        break;
    case HeaderOnly:
        drawFieldHeaders(painter);
        break;
    case TimeOnly:
        drawTimeLabels(painter);
        break;
    case GridOnly:
        drawGrid(painter);
        drawBookingBlocks(painter);
        break;
    case CornerOnly:
        // Draw "Time" in top-left corner
        painter.setPen(QColor(31, 41, 55)); // #1f2937
        QFont font = painter.font();
        font.setPointSize(9);
        font.setBold(true);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter, "Time");
        break;
    }

    // Draw pending selection (if exists) - Only in Full or GridOnly mode
    if ((displayMode == Full || displayMode == GridOnly) && hasPendingSelection)
    {
        int xOffset = (displayMode == Full) ? TIME_LABEL_WIDTH : 0;
        int yOffset = (displayMode == Full) ? HEADER_HEIGHT : 0;

        int x = xOffset + pendingFieldIndex * fieldWidth;
        int startY = yOffset + (pendingStartHour - START_HOUR) * HOUR_HEIGHT + (pendingStartMinute * HOUR_HEIGHT / 60);
        int height = pendingDurationMinutes * HOUR_HEIGHT / 60;

        QRect selectionRect(x + 2, startY, fieldWidth - 4, height);

        // Orange semi-transparent fill
        QColor fillColor(249, 115, 22, 120); // #f97316 with alpha 120
        painter.fillRect(selectionRect, fillColor);

        // Solid orange border
        painter.setPen(QPen(QColor(249, 115, 22), 3, Qt::SolidLine));
        painter.drawRect(selectionRect);

        // Label: "Nhấn Lưu để xác nhận"
        painter.setPen(QColor(255, 255, 255));
        QFont font = painter.font();
        font.setPointSize(9);
        font.setBold(true);
        painter.setFont(font);

        QString labelText = "Nhấn Lưu để xác nhận";
        QRect textRect = selectionRect.adjusted(4, 4, -4, -4);
        painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, labelText);
    }
}

void TimelineGridWidget::drawGrid(QPainter &painter)
{
    painter.setPen(QPen(QColor(229, 231, 235), 1)); // #e5e7eb

    int xOffset = (displayMode == Full) ? TIME_LABEL_WIDTH : 0;
    int yOffset = (displayMode == Full) ? HEADER_HEIGHT : 0;

    // Vertical line after Time column (only in Full mode)
    if (displayMode == Full)
    {
        painter.drawLine(TIME_LABEL_WIDTH, 0, TIME_LABEL_WIDTH, totalHeight);
    }

    // Vertical lines (field columns)
    for (int i = 0; i <= numFields; i++)
    {
        int x = xOffset + i * fieldWidth;
        painter.drawLine(x, 0, x, totalHeight);
    }

    // Horizontal lines (time rows)
    for (int i = 0; i <= numHours; i++)
    {
        int y = yOffset + i * HOUR_HEIGHT;
        painter.drawLine(0, y, totalWidth, y);
    }
}

void TimelineGridWidget::drawTimeLabels(QPainter &painter)
{
    painter.setPen(QColor(107, 114, 128)); // #6b7280
    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);

    int yOffset = (displayMode == Full) ? HEADER_HEIGHT : 0;

    // Draw time labels in left column
    for (int i = 0; i < numHours; i++)
    {
        int hour = START_HOUR + i;
        QString label = QString("%1:00").arg(hour, 2, 10, QChar('0'));

        int y = yOffset + i * HOUR_HEIGHT;
        QRect timeRect(0, y, TIME_LABEL_WIDTH, HOUR_HEIGHT);

        // Draw label centered in the time cell
        painter.drawText(timeRect, Qt::AlignCenter, label);
    }
}

void TimelineGridWidget::drawFieldHeaders(QPainter &painter)
{
    painter.setPen(QColor(31, 41, 55)); // #1f2937
    QFont font = painter.font();
    font.setPointSize(9);
    font.setBold(true);
    painter.setFont(font);

    // Header background - màu trắng
    painter.fillRect(0, 0, totalWidth, HEADER_HEIGHT, QColor(255, 255, 255));

    int xOffset = (displayMode == Full) ? TIME_LABEL_WIDTH : 0;

    // Draw "Time" in top-left corner (Only in Full mode)
    if (displayMode == Full)
    {
        QRect timeHeaderRect(0, 0, TIME_LABEL_WIDTH, HEADER_HEIGHT);
        painter.drawText(timeHeaderRect, Qt::AlignCenter, "Time");
    }

    // Draw field headers
    for (int i = 0; i < numFields && i < fields.size(); i++)
    {
        San *san = fields[i];
        QString fieldName = QString::fromStdString(san->layTenSan());

        int x = xOffset + i * fieldWidth;
        QRect headerRect(x, 0, fieldWidth, HEADER_HEIGHT);

        painter.drawText(headerRect, Qt::AlignCenter, fieldName);
    }
}

void TimelineGridWidget::drawBookingBlocks(QPainter &painter)
{
    int xOffset = (displayMode == Full) ? TIME_LABEL_WIDTH : 0;
    int yOffset = (displayMode == Full) ? HEADER_HEIGHT : 0;

    for (BookingBlock *block : bookingBlocks)
    {
        if (!block)
            continue;

        // Calculate position
        int x = xOffset + block->fieldIndex * fieldWidth;

        // Calculate y from time
        int totalMinutesFromStart = (block->startHour - START_HOUR) * 60 + block->startMinute;
        int y = yOffset + (totalMinutesFromStart * HOUR_HEIGHT) / 60;

        // Calculate height from duration
        int height = (block->durationMinutes * HOUR_HEIGHT) / 60;

        // Store rect for click detection
        block->rect = QRect(x + 2, y + 2, fieldWidth - 4, height - 4);

        // ===== DRAW WITH STATUS COLOR =====
        QColor blockColor = block->color;
        blockColor.setAlpha(200); // Increased opacity for better visibility
        painter.fillRect(block->rect, blockColor);

        // Border
        painter.setPen(QPen(blockColor.darker(120), 2));
        painter.drawRect(block->rect);

        // ===== HIỂN THỊ THÔNG TIN: GIỜ ĐÁ, TÊN SÂN, SĐT (Thứ tự mới) =====
        // Helper lambda to draw text (Normal White, No Shadow as requested)
        auto drawTextNormal = [&](const QRect &r, const QString &text, const QFont &f)
        {
            painter.setFont(f);
            painter.setPen(Qt::white);
            painter.drawText(r, Qt::AlignCenter, text);
        };

        // 1. Giờ đá (Line 1) - WHITE
        QFont timeFont = painter.font();
        timeFont.setPointSize(9);
        timeFont.setBold(true);

        int endHour = block->startHour + (block->startMinute + block->durationMinutes) / 60;
        int endMinute = (block->startMinute + block->durationMinutes) % 60;

        QString timeText = QString("%1:%2 - %3:%4")
                               .arg(block->startHour, 2, 10, QChar('0'))
                               .arg(block->startMinute, 2, 10, QChar('0'))
                               .arg(endHour, 2, 10, QChar('0'))
                               .arg(endMinute, 2, 10, QChar('0'));

        QRect line1Rect = block->rect.adjusted(4, 4, -4, -block->rect.height() * 2 / 3);
        drawTextNormal(line1Rect, timeText, timeFont);

        // 2. Tên sân (Line 2) - WHITE
        QFont fieldFont = painter.font();
        fieldFont.setPointSize(9);
        fieldFont.setBold(true);

        QString fieldName = "";
        if (block->fieldIndex >= 0 && block->fieldIndex < fields.size())
        {
            fieldName = QString::fromStdString(fields[block->fieldIndex]->layTenSan());
        }

        QRect line2Rect = block->rect.adjusted(4, block->rect.height() / 3, -4, -block->rect.height() / 3);
        drawTextNormal(line2Rect, fieldName, fieldFont);

        // 3. SĐT (Line 3) - WHITE
        QFont phoneFont = painter.font();
        phoneFont.setPointSize(10);
        phoneFont.setBold(true);

        KhachHang *customer = block->booking->getKhachHang();
        QString phoneNumber = "";
        if (customer)
        {
            phoneNumber = QString::fromStdString(customer->laySoDienThoai());
        }

        QRect line3Rect = block->rect.adjusted(4, block->rect.height() * 2 / 3, -4, -4);
        drawTextNormal(line3Rect, phoneNumber, phoneFont);
    }
}

void TimelineGridWidget::drawDragSelection(QPainter &painter)
{
    if (!isDragging || dragFieldIndex < 0 || dragFieldIndex >= numFields)
    {
        return;
    }

    int x = dragFieldIndex * fieldWidth;

    int startY = dragStartPos.y();
    int currentY = dragCurrentPos.y();

    int y = std::min(startY, currentY);
    int height = std::abs(currentY - startY);

    // Snap to 30-minute intervals
    int snappedY = ((y - HEADER_HEIGHT) / (HOUR_HEIGHT / 2)) * (HOUR_HEIGHT / 2) + HEADER_HEIGHT;
    int snappedHeight = ((height) / (HOUR_HEIGHT / 2)) * (HOUR_HEIGHT / 2);

    if (snappedHeight < HOUR_HEIGHT / 2)
    {
        snappedHeight = HOUR_HEIGHT / 2; // Minimum 30 minutes
    }

    // Draw semi-transparent green rectangle
    QRect selectionRect(x + 2, snappedY, fieldWidth - 4, snappedHeight);

    // Fill with green alpha
    QColor fillColor(22, 163, 74, 100); // #16a34a with alpha 100
    painter.fillRect(selectionRect, fillColor);

    // Draw green border
    painter.setPen(QPen(QColor(22, 163, 74), 2, Qt::DashLine)); // Dashed border
    painter.drawRect(selectionRect);

    // Draw time label
    int hour, minute;
    getTimeSlotAtY(snappedY, hour, minute);
    int durationMins = snappedHeight * 60 / HOUR_HEIGHT;

    QString timeText = QString("%1:%2 (%3 phút)")
                           .arg(hour, 2, 10, QChar('0'))
                           .arg(minute, 2, 10, QChar('0'))
                           .arg(durationMins);

    painter.setPen(QColor(255, 255, 255));
    QFont font = painter.font();
    font.setPointSize(9);
    font.setBold(true);
    painter.setFont(font);

    QRect textRect = selectionRect.adjusted(4, 4, -4, -4);
    painter.drawText(textRect, Qt::AlignCenter, timeText);
}

void TimelineGridWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    // Only allow interaction in Full or GridOnly mode
    if (displayMode != Full && displayMode != GridOnly)
    {
        return;
    }

    // Check if locked (selection active)
    if (property("isLocked").toBool())
    {
        // Optional: Emit signal or show tooltip that it's locked
        return;
    }

    QPoint pos = event->pos();

    // Check if clicked on existing booking
    BookingBlock *clickedBlock = getBookingBlockAt(pos);
    if (clickedBlock)
    {
        emit bookingClicked(clickedBlock->booking);
        return;
    }

    // Check if clicked in valid grid area
    int yOffset = (displayMode == Full) ? HEADER_HEIGHT : 0;
    int xOffset = (displayMode == Full) ? TIME_LABEL_WIDTH : 0;

    if (pos.y() < yOffset || pos.x() < xOffset)
    {
        return;
    }

    int fieldIndex = getFieldIndexAtX(pos.x());
    if (fieldIndex < 0)
    {
        return;
    }

    // Get clicked time slot
    int hour, minute;
    getTimeSlotAtY(pos.y(), hour, minute);

    // Cố định duration = 60 phút (1 tiếng)
    int durationMinutes = 60;

    // Save as pending selection
    hasPendingSelection = true;
    pendingFieldIndex = fieldIndex;
    pendingStartHour = hour;
    pendingStartMinute = 0; // Luôn bắt đầu từ đầu giờ
    pendingDurationMinutes = durationMinutes;

    // Emit signal for form to populate
    emit slotSelected(pendingFieldIndex, pendingStartHour, pendingStartMinute, pendingDurationMinutes);

    update();
}

void TimelineGridWidget::mouseMoveEvent(QMouseEvent *event)
{
    // Không cần drag nữa, bỏ qua
    Q_UNUSED(event);
}

void TimelineGridWidget::mouseReleaseEvent(QMouseEvent *event)
{
    // Không cần drag nữa, bỏ qua
    Q_UNUSED(event);
}

void TimelineGridWidget::clearPendingSelection()
{
    hasPendingSelection = false;
    pendingFieldIndex = -1;
    pendingStartHour = 0;
    pendingStartMinute = 0;
    pendingDurationMinutes = 0;
    update();
}

void TimelineGridWidget::contextMenuEvent(QContextMenuEvent *event)
{
    // Context menu removed as requested
    Q_UNUSED(event);
}

void TimelineGridWidget::showBookingContextMenu(const QPoint &pos, BookingBlock *block)
{
    // Function kept but empty to satisfy header declaration if any, or just remove body
    Q_UNUSED(pos);
    Q_UNUSED(block);
}

int TimelineGridWidget::getFieldIndexAtX(int x) const
{
    int xOffset = (displayMode == Full) ? TIME_LABEL_WIDTH : 0;

    // Check if x is in time label column (only for Full mode)
    if (displayMode == Full && x < TIME_LABEL_WIDTH)
    {
        return -1;
    }

    // Calculate field index from x position
    int fieldIndex = (x - xOffset) / fieldWidth;

    if (fieldIndex < 0 || fieldIndex >= numFields)
    {
        return -1;
    }

    return fieldIndex;
}

int TimelineGridWidget::getTimeSlotAtY(int y, int &outHour, int &outMinute) const
{
    int yOffset = (displayMode == Full) ? HEADER_HEIGHT : 0;

    if (displayMode == Full && y < HEADER_HEIGHT)
    {
        outHour = START_HOUR;
        outMinute = 0;
        return 0;
    }

    int relativeY = y - yOffset;
    int totalMinutes = (relativeY * 60) / HOUR_HEIGHT;

    // Snap to 30-minute intervals
    totalMinutes = (totalMinutes / 30) * 30;

    outHour = START_HOUR + totalMinutes / 60;
    outMinute = totalMinutes % 60;

    // Clamp to valid range
    if (outHour >= END_HOUR)
    {
        outHour = END_HOUR;
        outMinute = 0;
    }

    return totalMinutes;
}

BookingBlock *TimelineGridWidget::getBookingBlockAt(const QPoint &pos)
{
    for (BookingBlock *block : bookingBlocks)
    {
        if (block && block->rect.contains(pos))
        {
            return block;
        }
    }
    return nullptr;
}

QSize TimelineGridWidget::sizeHint() const
{
    return QSize(totalWidth, totalHeight);
}

QSize TimelineGridWidget::minimumSizeHint() const
{
    return QSize(numFields * MIN_FIELD_WIDTH,
                 HEADER_HEIGHT + numHours * HOUR_HEIGHT);
}
