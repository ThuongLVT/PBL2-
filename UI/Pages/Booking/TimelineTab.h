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
#include <QListWidget>
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

    /**
     * @brief Reload fields from Core
     */
    void reloadFields();

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
     * @brief Cancel selection (clear pending selection)
     */
    void onCancelSelectionClicked();

    /**
     * @brief Timeline slot selected (drag-to-create)
     */
    void onTimelineSlotSelected(int fieldIndex, int startHour, int startMinute, int durationMinutes);

    /**
     * @brief Existing booking clicked
     */
    void onBookingBlockClicked(DatSan *booking);

    /**
     * @brief Quick add customer button clicked
     */
    void onAddCustomerClicked();

    /**
     * @brief Duration quick select (30/60/90/120 min)
     */
    void onDuration30Clicked();
    void onDuration60Clicked();
    void onDuration90Clicked();
    void onDuration120Clicked();

signals:
    /**
     * @brief Emitted when booking data changes (create/update/delete)
     */
    void bookingDataChanged();

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
    /**
     * @brief Format currency for display
     */
    QString formatCurrency(double amount);

    /**
     * @brief Update calendar to highlight dates with bookings
     */
    void updateCalendarDates();
    void setupFilters();
    bool checkBookingConflict(San *san, const NgayGio &ngayGio, const KhungGio &khungGio);
    void setDuration(int minutes);

private:
    // ===== MAIN LAYOUT =====
    QVBoxLayout *mainLayout;

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
    QLineEdit *nameEdit; // Changed from QLabel to QLineEdit
    QComboBox *fieldCombo;
    QLabel *priceLabel;
    QLabel *depositLabel; // Nh\u00e3n hi\u1ec3n th\u1ecb ti\u1ec1n c\u1ecdc (30% ti\u1ec1n s\u00e2n)
    QComboBox *statusCombo;
    QComboBox *typeCombo;
    QDateEdit *dateEdit;
    QTimeEdit *fromTimeEdit;
    QTimeEdit *toTimeEdit;
    QLabel *durationLabel;
    // Duration quick select
    QPushButton *duration30Btn;
    QPushButton *duration60Btn;
    QPushButton *duration90Btn;
    QPushButton *duration120Btn;
    QTextEdit *noteEdit;

    // Action buttons
    QPushButton *saveBtn;
    QPushButton *deleteBtn;
    // Removed: checkinBtn (no longer needed)

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
