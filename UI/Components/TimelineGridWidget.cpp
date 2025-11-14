/**
 * @file TimelineGridWidget.cpp
 * @brief Implementation of TimelineGridWidget
 */

#include "TimelineGridWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QScrollBar>
#include <QDebug>
#include <cmath>

TimelineGridWidget::TimelineGridWidget(QWidget *parent)
    : QWidget(parent), system(nullptr), currentDate(QDate::currentDate()), isDragging(false), dragFieldIndex(-1), dragStartHour(0), dragStartMinute(0), totalWidth(0), totalHeight(0), fieldWidth(MIN_FIELD_WIDTH), numFields(0), numHours(END_HOUR - START_HOUR + 1)
{
    system = HeThongQuanLy::getInstance();

    setMouseTracking(true);
    setMinimumHeight(numHours * HOUR_HEIGHT + HEADER_HEIGHT);

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
        int startHour = ngayGio.getGio();
        int startMinute = ngayGio.getPhut();

        KhungGio khungGio = booking->getKhungGio();
        // Calculate duration from start and end time
        ThoiGian gioBD = khungGio.getGioBatDau();
        ThoiGian gioKT = khungGio.getGioKetThuc();
        int startMinutes = gioBD.getGio() * 60 + gioBD.getPhut();
        int endMinutes = gioKT.getGio() * 60 + gioKT.getPhut();
        int durationMinutes = endMinutes - startMinutes;
        if (durationMinutes < 0)
            durationMinutes = 0;

        // Create booking block
        BookingBlock *block = new BookingBlock(booking, fieldIndex, startHour, startMinute, durationMinutes);
        bookingBlocks.append(block);
    }

    update();
}

void TimelineGridWidget::calculateGeometry()
{
    if (numFields > 0)
    {
        fieldWidth = MIN_FIELD_WIDTH;
    }

    totalWidth = numFields * fieldWidth;
    totalHeight = HEADER_HEIGHT + numHours * HOUR_HEIGHT;

    setMinimumWidth(totalWidth);
    setMinimumHeight(totalHeight);
}

void TimelineGridWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Background
    painter.fillRect(rect(), QColor(255, 255, 255));

    drawGrid(painter);
    drawTimeLabels(painter);
    drawFieldHeaders(painter);
    drawBookingBlocks(painter);

    if (isDragging)
    {
        drawDragSelection(painter);
    }
}

void TimelineGridWidget::drawGrid(QPainter &painter)
{
    painter.setPen(QPen(QColor(229, 231, 235), 1)); // #e5e7eb

    // Vertical lines (field columns)
    for (int i = 0; i <= numFields; i++)
    {
        int x = i * fieldWidth;
        painter.drawLine(x, HEADER_HEIGHT, x, totalHeight);
    }

    // Horizontal lines (time rows)
    for (int i = 0; i <= numHours; i++)
    {
        int y = HEADER_HEIGHT + i * HOUR_HEIGHT;
        painter.drawLine(0, y, totalWidth, y);
    }
}

void TimelineGridWidget::drawTimeLabels(QPainter &painter)
{
    painter.setPen(QColor(107, 114, 128)); // #6b7280
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    // Draw time labels beside each horizontal line
    for (int i = 0; i <= numHours; i++)
    {
        int hour = START_HOUR + i;
        QString label = QString("%1:00").arg(hour, 2, 10, QChar('0'));

        int y = HEADER_HEIGHT + i * HOUR_HEIGHT;

        // Draw label on left side of the line
        painter.drawText(5, y - 3, label);
    }
}

void TimelineGridWidget::drawFieldHeaders(QPainter &painter)
{
    painter.setPen(QColor(31, 41, 55)); // #1f2937
    QFont font = painter.font();
    font.setPointSize(9);
    font.setBold(true);
    painter.setFont(font);

    // Header background
    painter.fillRect(0, 0, totalWidth, HEADER_HEIGHT, QColor(243, 244, 246));

    for (int i = 0; i < numFields && i < fields.size(); i++)
    {
        San *san = fields[i];
        QString fieldName = QString::fromStdString(san->layTenSan());

        int x = i * fieldWidth;
        QRect headerRect(x, 0, fieldWidth, HEADER_HEIGHT);

        painter.drawText(headerRect, Qt::AlignCenter, fieldName);
    }
}

void TimelineGridWidget::drawBookingBlocks(QPainter &painter)
{
    for (BookingBlock *block : bookingBlocks)
    {
        if (!block)
            continue;

        // Calculate position
        int x = block->fieldIndex * fieldWidth;

        // Calculate y from time
        int totalMinutesFromStart = (block->startHour - START_HOUR) * 60 + block->startMinute;
        int y = HEADER_HEIGHT + (totalMinutesFromStart * HOUR_HEIGHT) / 60;

        // Calculate height from duration
        int height = (block->durationMinutes * HOUR_HEIGHT) / 60;

        // Store rect for click detection
        block->rect = QRect(x + 2, y + 2, fieldWidth - 4, height - 4);

        // Draw booking block
        painter.fillRect(block->rect, QColor(59, 130, 246, 180)); // #3b82f6 with alpha

        // Border
        painter.setPen(QPen(QColor(59, 130, 246), 2));
        painter.drawRect(block->rect);

        // Text info
        painter.setPen(QColor(255, 255, 255));
        QFont font = painter.font();
        font.setPointSize(8);
        font.setBold(true);
        painter.setFont(font);

        // Customer name
        KhachHang *customer = block->booking->getKhachHang();
        if (customer)
        {
            QString customerName = QString::fromStdString(customer->layHoTen());
            QRect textRect = block->rect.adjusted(4, 4, -4, -4);
            painter.drawText(textRect, Qt::AlignTop | Qt::AlignHCenter, customerName);
        }

        // Time
        QString timeText = QString("%1:%2")
                               .arg(block->startHour, 2, 10, QChar('0'))
                               .arg(block->startMinute, 2, 10, QChar('0'));
        QRect timeRect = block->rect.adjusted(4, 0, -4, -4);
        painter.drawText(timeRect, Qt::AlignBottom | Qt::AlignHCenter, timeText);
    }
}

void TimelineGridWidget::drawDragSelection(QPainter &painter)
{
    if (dragFieldIndex < 0 || dragFieldIndex >= numFields)
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
    int snappedHeight = std::max(HOUR_HEIGHT / 2, ((height) / (HOUR_HEIGHT / 2)) * (HOUR_HEIGHT / 2));

    QRect selectionRect(x + 2, snappedY + 2, fieldWidth - 4, snappedHeight - 4);

    // Draw semi-transparent green overlay
    painter.fillRect(selectionRect, QColor(22, 163, 74, 100)); // #16a34a with alpha

    // Border
    painter.setPen(QPen(QColor(22, 163, 74), 2, Qt::DashLine));
    painter.drawRect(selectionRect);
}

void TimelineGridWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
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
    if (pos.y() < HEADER_HEIGHT)
    {
        return;
    }

    int fieldIndex = getFieldIndexAtX(pos.x());
    if (fieldIndex < 0)
    {
        return;
    }

    // Start dragging
    isDragging = true;
    dragStartPos = pos;
    dragCurrentPos = pos;
    dragFieldIndex = fieldIndex;

    int hour, minute;
    getTimeSlotAtY(pos.y(), hour, minute);
    dragStartHour = hour;
    dragStartMinute = minute;

    update();
}

void TimelineGridWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!isDragging)
    {
        return;
    }

    dragCurrentPos = event->pos();
    update();
}

void TimelineGridWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (!isDragging || event->button() != Qt::LeftButton)
    {
        return;
    }

    isDragging = false;

    // Calculate duration
    int startY = dragStartPos.y();
    int endY = dragCurrentPos.y();

    if (endY < startY)
    {
        std::swap(startY, endY);
    }

    int startHour, startMinute;
    getTimeSlotAtY(startY, startHour, startMinute);

    int endHour, endMinute;
    getTimeSlotAtY(endY, endHour, endMinute);

    // Calculate duration in minutes
    int durationMinutes = (endHour * 60 + endMinute) - (startHour * 60 + startMinute);

    // Enforce minimum duration
    if (durationMinutes < MIN_DURATION_MINUTES)
    {
        durationMinutes = MIN_DURATION_MINUTES;
    }

    // Emit signal
    emit slotSelected(dragFieldIndex, startHour, startMinute, durationMinutes);

    update();
}

int TimelineGridWidget::getFieldIndexAtX(int x) const
{
    if (x < 0)
    {
        return -1;
    }

    int fieldIndex = x / fieldWidth;

    if (fieldIndex < 0 || fieldIndex >= numFields)
    {
        return -1;
    }

    return fieldIndex;
}

int TimelineGridWidget::getTimeSlotAtY(int y, int &outHour, int &outMinute) const
{
    if (y < HEADER_HEIGHT)
    {
        outHour = START_HOUR;
        outMinute = 0;
        return 0;
    }

    int relativeY = y - HEADER_HEIGHT;
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
