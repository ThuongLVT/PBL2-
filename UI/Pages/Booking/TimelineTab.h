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
#include <QCompleter>
#include <QStringListModel>

// Forward declaration
class TimelineGridWidget;
class QScrollArea;

/**
 * @class TimelineTab
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
     * @brief Filter fields based on type and area
     */
    void onFilterChanged();

    /**
     * @brief Field changed event
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

public slots:
    /**
     * @brief Fill form with booking data for rescheduling
     * @param booking Booking to reschedule
     */
    void loadBookingForReschedule(DatSan *booking);

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
    void filterFieldsByType(LoaiSan loaiSan);
    void loadAllFields();

    // Helper for sync scrolling
    void syncScrollBars();

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
    QComboBox *filterTypeCombo; // New filter combo for field type
    QComboBox *filterAreaCombo; // New filter combo for area
    QLabel *priceLabel;
    QLabel *depositLabel; // Nhãn hiển thị tiền cọc (30% tiền sân)
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
    QPushButton *addCustomerBtn; // New button

    // ===== BOTTOM PANEL (60%) =====
    QFrame *timelinePanel;
    QHBoxLayout *timelineLayout;

    // Widgets for fixed headers
    TimelineGridWidget *cornerWidget;
    TimelineGridWidget *headerWidget;
    TimelineGridWidget *timeWidget;
    TimelineGridWidget *timelineGrid;

    QScrollArea *headerScrollArea;
    QScrollArea *timeScrollArea;
    QScrollArea *gridScrollArea;

    // ===== DATA =====
    HeThongQuanLy *system;
    MangDong<San *> fields;
    MangDong<San *> allFields;   // Store all fields before filtering
    DatSan *currentBooking;      // Current editing booking
    DatSan *reschedulingBooking; // Booking being rescheduled
    bool isEditMode;
    bool isRescheduleMode;
    bool isNewCustomerPending; // Flag for new customer mode
    QCompleter *phoneCompleter;
    QCompleter *nameCompleter;
};

#endif // TIMELINETAB_H
