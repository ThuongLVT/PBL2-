#include <iostream>
#include <QApplication>
#include <QFile>
#include <QDebug>

#include "Core/CauTrucDuLieu/MangDong.h"
#include "Core/CauTrucDuLieu/DanhSachLienKet.h"
#include "Core/CauTrucDuLieu/Ngan.h"
#include "Core/CauTrucDuLieu/HangDoi.h"
#include "Core/CauTrucDuLieu/ThoiGian.h"
#include "Core/CauTrucDuLieu/NgayThang.h"
#include "Core/CauTrucDuLieu/NgayGio.h"

// Models - Week 2
#include "Core/Models/ConNguoi.h"
#include "Core/Models/NguoiDung.h"
#include "Core/Models/QuanTriVien.h"
#include "Core/Models/NhanVien.h"
#include "Core/Models/KhachHang.h"
#include "Core/Models/San.h"
#include "Core/Models/KhungGio.h"
#include "Core/Models/DichVu.h"

// Models - Week 3
#include "Core/Models/DichVuDat.h"
#include "Core/Models/DatSan.h"
#include "Core/Models/ThanhToan.h"
#include "Core/Models/ThanhToanTienMat.h"
#include "Core/Models/ThanhToanThe.h"
#include "Core/Models/ThanhToanChuyenKhoan.h"

// System Management - Week 3
#include "Core/QuanLy/HeThongQuanLy.h"
#include "Core/QuanLy/FileManager.h"

// Statistics - Week 3
#include "Core/ThongKe/ThongKe.h"
#include "Core/ThongKe/ThongKeDoanhThu.h"
#include "Core/ThongKe/ThongKeKhachHang.h"

// Algorithms - Week 3
#include "Core/ThuatToan/QuickSort.h"
#include "Core/ThuatToan/MergeSort.h"
#include "Core/ThuatToan/BinarySearchTree.h"
#include "Core/ThuatToan/HashTable.h"

// UI Layer - Qt6 Widgets
#include "UI/Pages/Login/LoginDialog.h"
#include "UI/MainWindow/MainWindow.h" // Week 1 Day 3-4

using namespace std;

// Define data files with absolute path
const string DATA_DIR = "D:/QT_PBL2/Data/";
const string DATA_FILE = DATA_DIR + "data.bin";

int main(int argc, char *argv[])
{
     // Initialize Qt Application (GUI mode)
     QApplication app(argc, argv);

     // Set application info
     app.setApplicationName("Football Field Manager");
     app.setOrganizationName("FootballManager");
     app.setApplicationVersion("1.0.0");

     cout << "========================================" << endl;
     cout << "   HE THONG QUAN LY SAN BONG DA" << endl;
     cout << "   Qt6 Widgets - GUI APPLICATION" << endl;
     cout << "========================================" << endl;

     // Initialize system
     cout << "Initializing system..." << endl;
     HeThongQuanLy *heThong = HeThongQuanLy::getInstance();

     // Try to load existing binary data
     bool dataLoaded = heThong->docHeThong(DATA_FILE);
     if (dataLoaded)
     {
          cout << "✅ Loaded existing data from " << DATA_FILE << endl;
     }
     else
     {
          cout << "⚠️  No data file found - initializing with sample data..." << endl;
          heThong->khoiTaoDuLieuMau();
          heThong->luuHeThong(DATA_FILE);
          cout << "✅ Sample data created and saved to " << DATA_FILE << endl;
     }

     // Create default admin if no admin exists
     if (heThong->layDanhSachQuanTriVien().isEmpty())
     {
          cout << "Creating default admin account..." << endl;
          QuanTriVien *admin = new QuanTriVien("Administrator", "0000000000", "System", "admin", "admin123");
          heThong->themQuanTriVien(admin);
          cout << "⚠️  Default admin created - Username: admin, Password: admin123" << endl;
          cout << "⚠️  PLEASE CHANGE PASSWORD AFTER FIRST LOGIN!" << endl;
     }

     cout << "System initialized with:" << endl;
     cout << "  - Customers: " << heThong->tongSoKhachHang() << endl;
     cout << "  - Fields: " << heThong->tongSoSan() << endl;
     cout << "  - Staff: " << heThong->tongSoNhanVien() << endl;
     cout << "  - Bookings: " << heThong->tongSoDatSan() << endl;
     cout << "  - Payments: " << heThong->tongSoThanhToan() << endl;
     cout << endl;

     // Load stylesheet
     QFile styleFile(":/styles/app.qss");
     if (styleFile.open(QFile::ReadOnly))
     {
          QString styleSheet = QLatin1String(styleFile.readAll());
          app.setStyleSheet(styleSheet);
          styleFile.close();
          cout << "✅ Stylesheet loaded successfully!" << endl;
     }
     else
     {
          cout << "⚠️  Warning: Could not load stylesheet (UI will use default style)" << endl;
     }

     // Week 1 Day 2: Show LoginDialog
     cout << "Week 1 Day 2: Showing Login Dialog..." << endl;
     cout << "========================================" << endl;

     LoginDialog loginDialog;
     if (loginDialog.exec() == QDialog::Accepted)
     {
          cout << "✅ Login successful!" << endl;
          cout << "   Username: " << loginDialog.getLoggedInUsername().toStdString() << endl;
          cout << "   Role: " << (loginDialog.isAdmin() ? "Admin" : "Staff") << endl;
          cout << endl;

          // Week 1 Day 3-4: Show MainWindow after successful login
          cout << "Week 1 Day 3-4: Showing MainWindow..." << endl;

          // Get current user
          QuanTriVien *currentUser = heThong->timQuanTriVien(loginDialog.getLoggedInUsername().toStdString());
          if (!currentUser)
          {
               cout << "❌ Error: Could not retrieve current user!" << endl;
               return 1;
          }

          MainWindow mainWindow(currentUser);
          mainWindow.showMaximized();

          cout << "✅ MainWindow displayed!" << endl;
          cout << "========================================" << endl;

          return app.exec();
     }
     else
     {
          cout << "Login cancelled or failed." << endl;
          return 0;
     }
}
