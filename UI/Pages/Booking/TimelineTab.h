/**
 * @file TimelineTab.h
 * @brief Timeline view tab for booking management
 * @details Layout structure:
 *          - Top 40%: Left 30% calendar, Right 70% booking form
 *          - Bottom 60%: Timeline grid for drag-to-create bookings
 *
 * @author Football Field Management System
 * @date 2025-11-14
 */

#ifndef TIMELINETAB_H
#define TIMELINETAB_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QCalendarWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QDate>
#include "../../Core/QuanLy/HeThongQuanLy.h"
#include "../../Core/Models/DatSan.h"
#include "../../Core/Models/KhachHang.h"
#include "../../Core/Models/San.h"

// Forward declaration
class TimelineGridWidget;

/**
 * @class TimelineTab
 * @brief Timeline view for visual booking management
 */
class TimelineTab : public QWidget
{
    Q_OBJECT

public:
    explicit TimelineTab(QWidget *parent = nullptr);
    ~TimelineTab();

    /**
     * @brief Refresh data from Core
     */
    void refreshData();

private slots:
    /**
     * @brief Date selected in calendar
     */
    void onDateSelected(const QDate &date);

    /**
     * @brief Search customer by phone
     */
    void onPhoneSearchClicked();

    /**
     * @brief Field selection changed
     */
    void onFieldChanged(int index);

    /**
     * @brief Save booking (create or update)
     */
    void onSaveClicked();

    /**
     * @brief Delete booking
     */
    void onDeleteClicked();

    /**
     * @brief Checkin/Match action
     */
    void onCheckinClicked();

    /**
     * @brief Timeline slot selected (drag-to-create)
     */
    void onTimelineSlotSelected(int fieldIndex, int startHour, int startMinute, int durationMinutes);

    /**
     * @brief Existing booking clicked
     */
    void onBookingBlockClicked(DatSan *booking);

private:
    void setupUI();
    void setupConnections();
    void setupCalendarPanel();
    void setupFormPanel();
    void setupTimelinePanel();
    void loadFields();
    void clearForm();
    void populateForm(DatSan *booking);
    void updateDuration();
    QString formatCurrency(double amount);

private:
    // ===== MAIN LAYOUT =====
    QVBoxLayout *mainLayout;
    QSplitter *verticalSplitter;

    // ===== TOP PANEL (40%) =====
    QWidget *topPanel;
    QHBoxLayout *topPanelLayout;

    // Calendar Panel (30% of top)
    QFrame *calendarPanel;
    QVBoxLayout *calendarLayout;
    QCalendarWidget *calendar;
    QDate selectedDate;

    // Form Panel (70% of top)
    QFrame *formPanel;
    QVBoxLayout *formLayout;

    // Form fields
    QLineEdit *phoneEdit;
    QLabel *nameLabel;
    QComboBox *fieldCombo;
    QLabel *priceLabel;
    QComboBox *statusCombo;
    QComboBox *typeCombo;
    QDateEdit *dateEdit;
    QTimeEdit *fromTimeEdit;
    QTimeEdit *toTimeEdit;
    QLabel *durationLabel;
    QTextEdit *noteEdit;

    // Action buttons
    QPushButton *saveBtn;
    QPushButton *deleteBtn;
    QPushButton *checkinBtn;

    // ===== BOTTOM PANEL (60%) =====
    QFrame *timelinePanel;
    QVBoxLayout *timelineLayout;
    TimelineGridWidget *timelineGrid;

    // ===== DATA =====
    HeThongQuanLy *system;
    MangDong<San *> fields;
    DatSan *currentBooking; // Current editing booking
    bool isEditMode;
};

#endif // TIMELINETAB_H
