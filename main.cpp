/**
 * @file main.cpp
 * @brief Entry point - Hệ Thống Quản Lý Sân Bóng Đá
 * @author Lê Viết Thương & Đinh Khang Ninh
 */

#include <iostream>
#include <QApplication>
#include <QFile>

// Data Structures
#include "Core/CauTrucDuLieu/MangDong.h"
#include "Core/CauTrucDuLieu/DanhSachLienKet.h"
#include "Core/CauTrucDuLieu/NgayGio.h"

// Models
#include "Core/Models/ConNguoi.h"
#include "Core/Models/NguoiDung.h"
#include "Core/Models/QuanTriVien.h"
#include "Core/Models/NhanVien.h"
#include "Core/Models/KhachHang.h"
#include "Core/Models/San.h"
#include "Core/Models/KhungGio.h"
#include "Core/Models/DichVu.h"
#include "Core/Models/DichVuDat.h"
#include "Core/Models/DatSan.h"

// Business Logic
#include "Core/QuanLy/HeThongQuanLy.h"

// Statistics
#include "Core/ThongKe/ThongKe.h"
#include "Core/ThongKe/ThongKeDoanhThu.h"
#include "Core/ThongKe/ThongKeKhachHang.h"

// Algorithms
#include "Core/ThuatToan/QuickSort.h"
#include "Core/ThuatToan/MergeSort.h"
#include "Core/ThuatToan/HashTable.h"

// UI
#include "UI/Pages/Login/LoginDialog.h"
#include "UI/MainWindow/MainWindow.h"

using namespace std;

// Data file path
const string DATA_FILE = "D:/PBL2-/Data/data.bin";
const int LOGOUT_CODE = 888;

/**
 * @brief Load data from CSV files
 */
void loadSystemData(HeThongQuanLy *heThong)
{
     cout << "\n[Loading Data]" << endl;

     // Load main data
     if (heThong->docHeThong(DATA_FILE))
          cout << "  ✓ Main data loaded" << endl;

     // Load staff
     if (heThong->docNhanVienCSV("nhanvien.csv"))
          cout << "  ✓ Staff: " << heThong->layQuanLyNhanVien()->tongSoNhanVien() << endl;

     // Load admin
     if (heThong->layQuanLyNhanVien()->docAdminCSV("admin.csv"))
          cout << "  ✓ Admin accounts loaded" << endl;

     // Summary
     cout << "\n[System Summary]" << endl;
     cout << "  Customers: " << heThong->tongSoKhachHang() << endl;
     cout << "  Fields:    " << heThong->tongSoSan() << endl;
     cout << "  Bookings:  " << heThong->tongSoDatSan() << endl;
}

/**
 * @brief Load application stylesheet
 */
void loadStylesheet(QApplication &app)
{
     QFile styleFile(":/styles/app.qss");
     if (styleFile.open(QFile::ReadOnly))
     {
          app.setStyleSheet(QLatin1String(styleFile.readAll()));
          styleFile.close();
     }
}

/**
 * @brief Get logged in user from system
 */
NguoiDung *getLoggedInUser(HeThongQuanLy *heThong, const LoginDialog &loginDialog)
{
     QString username = loginDialog.getLoggedInUsername();

     if (loginDialog.isAdmin())
          return heThong->layQuanLyNhanVien()->timQuanTriVienTheoUsername(username.toStdString());

     return heThong->layQuanLyNhanVien()->timNhanVienTheoUsername(username.toStdString());
}

int main(int argc, char *argv[])
{
     QApplication app(argc, argv);
     app.setApplicationName("Football Field Manager");
     app.setOrganizationName("FootballManager");
     app.setApplicationVersion("1.0.0");

     cout << "========================================" << endl;
     cout << "   HE THONG QUAN LY SAN BONG DA        " << endl;
     cout << "========================================" << endl;

     // Initialize system (Singleton)
     HeThongQuanLy *heThong = HeThongQuanLy::getInstance();
     loadSystemData(heThong);
     loadStylesheet(app);

     // Main application loop (supports logout/re-login)
     int exitCode = 0;
     do
     {
          LoginDialog loginDialog;

          if (loginDialog.exec() != QDialog::Accepted)
               break;

          NguoiDung *currentUser = getLoggedInUser(heThong, loginDialog);
          if (!currentUser)
          {
               cerr << "Error: Could not retrieve user!" << endl;
               return 1;
          }

          cout << "\n✓ Login: " << currentUser->layHoTen() << endl;

          MainWindow mainWindow(currentUser);
          mainWindow.showMaximized();
          exitCode = app.exec();

     } while (exitCode == LOGOUT_CODE);

     return exitCode;
}
