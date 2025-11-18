/**
 * @file TimelineGridWidget.cpp
 * @brief Implementation of TimelineGridWidget
 */

#include "TimelineGridWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QScrollBar>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QContextMenuEvent>
#include <cmath>
#include <functional>

TimelineGridWidget::TimelineGridWidget(QWidget *parent)
    : QWidget(parent), system(nullptr), currentDate(QDate::currentDate()), isDragging(false), dragFieldIndex(-1), dragStartHour(0), dragStartMinute(0), hasPendingSelection(false), pendingFieldIndex(-1), pendingStartHour(0), pendingStartMinute(0), pendingDurationMinutes(0), totalWidth(0), totalHeight(0), fieldWidth(MIN_FIELD_WIDTH), numFields(0), numHours(END_HOUR - START_HOUR + 1)
{
    system = HeThongQuanLy::getInstance();

    setMouseTracking(true);
    setMinimumHeight(numHours * HOUR_HEIGHT + HEADER_HEIGHT);
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

    // D\u00e0nh s\u00e1ch m\u00e0u s\u1eafc \u0111a d\u1ea1ng (tr\u1eeb \u0111en x\u00e1m)
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

    // Add TIME_LABEL_WIDTH to total width
    totalWidth = TIME_LABEL_WIDTH + numFields * fieldWidth;
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

    // Draw pending selection (if exists)
    if (hasPendingSelection)
    {
        int x = TIME_LABEL_WIDTH + pendingFieldIndex * fieldWidth;
        int startY = HEADER_HEIGHT + (pendingStartHour - START_HOUR) * HOUR_HEIGHT + (pendingStartMinute * HOUR_HEIGHT / 60);
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

    // Vertical line after Time column
    painter.drawLine(TIME_LABEL_WIDTH, 0, TIME_LABEL_WIDTH, totalHeight);

    // Vertical lines (field columns)
    for (int i = 0; i <= numFields; i++)
    {
        int x = TIME_LABEL_WIDTH + i * fieldWidth;
        painter.drawLine(x, HEADER_HEIGHT, x, totalHeight);
    }

    // Horizontal lines (time rows) - chỉ vẽ từ cột sân, không vẽ trong cột giờ
    for (int i = 0; i <= numHours; i++)
    {
        int y = HEADER_HEIGHT + i * HOUR_HEIGHT;
        painter.drawLine(TIME_LABEL_WIDTH, y, totalWidth, y);
    }
}

void TimelineGridWidget::drawTimeLabels(QPainter &painter)
{
    painter.setPen(QColor(107, 114, 128)); // #6b7280
    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);

    // Draw time labels in left column
    for (int i = 0; i < numHours; i++)
    {
        int hour = START_HOUR + i;
        QString label = QString("%1:00").arg(hour, 2, 10, QChar('0'));

        int y = HEADER_HEIGHT + i * HOUR_HEIGHT;
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

    // Draw "Time" in top-left corner
    QRect timeHeaderRect(0, 0, TIME_LABEL_WIDTH, HEADER_HEIGHT);
    painter.drawText(timeHeaderRect, Qt::AlignCenter, "Time");

    // Draw field headers
    for (int i = 0; i < numFields && i < fields.size(); i++)
    {
        San *san = fields[i];
        QString fieldName = QString::fromStdString(san->layTenSan());

        int x = TIME_LABEL_WIDTH + i * fieldWidth;
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

        // Calculate position (add TIME_LABEL_WIDTH offset)
        int x = TIME_LABEL_WIDTH + block->fieldIndex * fieldWidth;

        // Calculate y from time
        int totalMinutesFromStart = (block->startHour - START_HOUR) * 60 + block->startMinute;
        int y = HEADER_HEIGHT + (totalMinutesFromStart * HOUR_HEIGHT) / 60;

        // Calculate height from duration
        int height = (block->durationMinutes * HOUR_HEIGHT) / 60;

        // Store rect for click detection
        block->rect = QRect(x + 2, y + 2, fieldWidth - 4, height - 4);

        // ===== DRAW WITH STATUS COLOR =====
        QColor blockColor = block->color;
        blockColor.setAlpha(180); // Semi-transparent
        painter.fillRect(block->rect, blockColor);

        // Border
        painter.setPen(QPen(blockColor, 2));
        painter.drawRect(block->rect);

        // ===== HIỂN THỊ SĐT KHÁCH HÀNG (TO, Ở GIỮA) =====
        painter.setPen(QColor(255, 255, 255));
        QFont phoneFont = painter.font();
        phoneFont.setPointSize(11); // Kích thước lớn
        phoneFont.setBold(true);
        painter.setFont(phoneFont);

        KhachHang *customer = block->booking->getKhachHang();
        if (customer)
        {
            QString phoneNumber = QString::fromStdString(customer->laySoDienThoai());
            QRect phoneRect = block->rect.adjusted(4, 4, -4, -4);
            painter.drawText(phoneRect, Qt::AlignCenter | Qt::TextWordWrap, phoneNumber);
        }

        // Time - ở dưới cùng
        phoneFont.setPointSize(8);
        painter.setFont(phoneFont);
        QString timeText = QString("%1:%2")
                               .arg(block->startHour, 2, 10, QChar('0'))
                               .arg(block->startMinute, 2, 10, QChar('0'));
        QRect timeRect = block->rect.adjusted(4, 0, -4, -4);
        painter.drawText(timeRect, Qt::AlignBottom | Qt::AlignHCenter, timeText);
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

    QPoint pos = event->pos();

    // Check if clicked on existing booking
    BookingBlock *clickedBlock = getBookingBlockAt(pos);
    if (clickedBlock)
    {
        emit bookingClicked(clickedBlock->booking);
        return;
    }

    // Check if clicked in valid grid area (below header and after time column)
    if (pos.y() < HEADER_HEIGHT || pos.x() < TIME_LABEL_WIDTH)
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
    // Check if right-click on booking block
    BookingBlock *block = getBookingBlockAt(event->pos());
    if (block && block->booking)
    {
        showBookingContextMenu(event->globalPos(), block);
    }
}

void TimelineGridWidget::showBookingContextMenu(const QPoint &pos, BookingBlock *block)
{
    QMenu menu(this);

    QAction *viewAction = menu.addAction("📋 Xem chi tiết");
    QAction *checkinAction = menu.addAction("✓ Check-in nhanh");
    QAction *paymentAction = menu.addAction("💳 Thanh toán");
    QAction *serviceAction = menu.addAction("🍺 Đặt thêm dịch vụ");
    menu.addSeparator();
    QAction *cancelAction = menu.addAction("🗑️ Hủy đặt sân");

    // Style menu
    menu.setStyleSheet(
        "QMenu { background-color: white; border: 1px solid #e5e7eb; border-radius: 6px; padding: 4px; } "
        "QMenu::item { padding: 6px 20px; font-size: 12px; } "
        "QMenu::item:selected { background-color: #f0fdf4; color: #16a34a; } "
        "QMenu::separator { height: 1px; background: #e5e7eb; margin: 4px 0; }");

    QAction *selected = menu.exec(pos);

    if (selected == viewAction)
    {
        emit bookingClicked(block->booking);
    }
    else if (selected == checkinAction)
    {
        QMessageBox::information(this, "Check-in", "Chức năng check-in nhanh sẽ được triển khai sau!");
    }
    else if (selected == paymentAction)
    {
        QMessageBox::information(this, "Thanh toán", "Chức năng thanh toán sẽ được triển khai ở Tab Table View!");
    }
    else if (selected == serviceAction)
    {
        QMessageBox::information(this, "Dịch vụ", "Chức năng đặt thêm dịch vụ sẽ được triển khai sau!");
    }
    else if (selected == cancelAction)
    {
        emit bookingClicked(block->booking);
    }
}

int TimelineGridWidget::getFieldIndexAtX(int x) const
{
    // Check if x is in time label column
    if (x < TIME_LABEL_WIDTH)
    {
        return -1;
    }

    // Calculate field index from x position (after time column)
    int fieldIndex = (x - TIME_LABEL_WIDTH) / fieldWidth;

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
