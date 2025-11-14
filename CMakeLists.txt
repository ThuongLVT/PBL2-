cmake_minimum_required(VERSION 3.16)

project(FootballFieldManager VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt Configuration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 packages - Core, Widgets only (NO Charts - no dashboard)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Core/CauTrucDuLieu
    ${CMAKE_SOURCE_DIR}/Core/Models
    ${CMAKE_SOURCE_DIR}/Core/QuanLy
    ${CMAKE_SOURCE_DIR}/Core/ThongKe
    ${CMAKE_SOURCE_DIR}/Core/ThuatToan
    ${CMAKE_SOURCE_DIR}/Core/Utils
    ${CMAKE_SOURCE_DIR}/UI
    ${CMAKE_SOURCE_DIR}/UI/MainWindow
    ${CMAKE_SOURCE_DIR}/UI/Components
    ${CMAKE_SOURCE_DIR}/UI/Pages
    ${CMAKE_SOURCE_DIR}/UI/Dialogs
)

# Source files - Core/CauTrucDuLieu
set(CORE_CAUTRUCDULIEU_SOURCES
    Core/CauTrucDuLieu/ThoiGian.cpp
    Core/CauTrucDuLieu/NgayThang.cpp
    Core/CauTrucDuLieu/NgayGio.cpp
)

set(CORE_CAUTRUCDULIEU_HEADERS
    Core/CauTrucDuLieu/MangDong.h
    Core/CauTrucDuLieu/DanhSachLienKet.h
    Core/CauTrucDuLieu/Ngan.h
    Core/CauTrucDuLieu/HangDoi.h
    Core/CauTrucDuLieu/ThoiGian.h
    Core/CauTrucDuLieu/NgayThang.h
    Core/CauTrucDuLieu/NgayGio.h
)

# Source files - Core/Models
set(CORE_MODELS_SOURCES
    Core/Models/ConNguoi.cpp
    Core/Models/NguoiDung.cpp
    Core/Models/QuanTriVien.cpp
    Core/Models/NhanVien.cpp
    Core/Models/KhachHang.cpp
    Core/Models/San.cpp
    Core/Models/KhungGio.cpp
    Core/Models/DichVu.cpp
    Core/Models/DichVuDat.cpp
    Core/Models/DatSan.cpp
    Core/Models/DonHangDichVu.cpp
    Core/Models/ThanhToan.cpp
    Core/Models/ThanhToanTienMat.cpp
    Core/Models/ThanhToanThe.cpp
    Core/Models/ThanhToanChuyenKhoan.cpp
)

set(CORE_MODELS_HEADERS
    Core/Models/ConNguoi.h
    Core/Models/NguoiDung.h
    Core/Models/QuanTriVien.h
    Core/Models/NhanVien.h
    Core/Models/KhachHang.h
    Core/Models/San.h
    Core/Models/KhungGio.h
    Core/Models/DichVu.h
    Core/Models/DichVuDat.h
    Core/Models/DatSan.h
    Core/Models/DonHangDichVu.h
    Core/Models/ThanhToan.h
    Core/Models/ThanhToanTienMat.h
    Core/Models/ThanhToanThe.h
    Core/Models/ThanhToanChuyenKhoan.h
)

# Source files - Core/QuanLy
set(CORE_QUANLY_SOURCES
    Core/QuanLy/HeThongQuanLy.cpp
    Core/QuanLy/FileManager.cpp
    Core/QuanLy/BackupManager.cpp
    Core/QuanLy/QuanLySan.cpp
    Core/QuanLy/QuanLyKhachHang.cpp
    Core/QuanLy/QuanLyDichVu.cpp
    Core/QuanLy/QuanLyDonHangDichVu.cpp
    Core/QuanLy/QuanLyDatSan.cpp
    Core/QuanLy/QuanLyThanhToan.cpp
)

set(CORE_QUANLY_HEADERS
    Core/QuanLy/HeThongQuanLy.h
    Core/QuanLy/FileManager.h
    Core/QuanLy/BackupManager.h
    Core/QuanLy/QuanLySan.h
    Core/QuanLy/QuanLyKhachHang.h
    Core/QuanLy/QuanLyDichVu.h
    Core/QuanLy/QuanLyDonHangDichVu.h
    Core/QuanLy/QuanLyDatSan.h
    Core/QuanLy/QuanLyThanhToan.h
)

# Source files - Core/ThongKe
set(CORE_THONGKE_SOURCES
    Core/ThongKe/ThongKe.cpp
    Core/ThongKe/ThongKeDoanhThu.cpp
    Core/ThongKe/ThongKeKhachHang.cpp
)

set(CORE_THONGKE_HEADERS
    Core/ThongKe/ThongKe.h
    Core/ThongKe/ThongKeDoanhThu.h
    Core/ThongKe/ThongKeKhachHang.h
)

# Source files - Core/ThuatToan (header-only templates)
set(CORE_THUATTOAN_HEADERS
    Core/ThuatToan/QuickSort.h
    Core/ThuatToan/MergeSort.h
    Core/ThuatToan/BinarySearchTree.h
    Core/ThuatToan/HashTable.h
)

# Source files - Core/Utils (header-only)
set(CORE_UTILS_SOURCES
    Core/Utils/CSVHandler.cpp
)

set(CORE_UTILS_HEADERS
    Core/Utils/FileHelper.h
    Core/Utils/CSVHandler.h
)

# ============================================
# UI SOURCES (Qt6 Widgets) - Week 1-3 Implementation
# ============================================

# UI Sources will be added as we build each component
# Week 1: MainWindow, Sidebar, Login
# Week 2: Booking pages
# Week 3: Other CRUD pages

# Week 1 Day 2: Login Dialog
# Week 1 Day 3-4: MainWindow, Sidebar, Header, Placeholder Pages
set(UI_SOURCES
    ${CMAKE_SOURCE_DIR}/UI/Pages/Login/LoginDialog.cpp
    ${CMAKE_SOURCE_DIR}/UI/MainWindow/MainWindow.cpp
    ${CMAKE_SOURCE_DIR}/UI/Components/Sidebar.cpp
    ${CMAKE_SOURCE_DIR}/UI/Components/Header.cpp
    ${CMAKE_SOURCE_DIR}/UI/Pages/Booking/BookingListPage.cpp
    ${CMAKE_SOURCE_DIR}/UI/Pages/Payment/PaymentListPage.cpp
    ${CMAKE_SOURCE_DIR}/UI/Pages/Field/FieldManagementPage.cpp
    ${CMAKE_SOURCE_DIR}/UI/Pages/Customer/CustomerManagementPage.cpp
    ${CMAKE_SOURCE_DIR}/UI/Pages/Service/ServiceManagementPage.cpp
    ${CMAKE_SOURCE_DIR}/UI/Pages/Service/ServiceOrderWidget.cpp
    ${CMAKE_SOURCE_DIR}/UI/Pages/Service/ServiceManagementWidget.cpp
    ${CMAKE_SOURCE_DIR}/UI/Pages/Staff/StaffManagementPage.cpp
    ${CMAKE_SOURCE_DIR}/UI/Pages/Statistics/StatisticsPage.cpp
    ${CMAKE_SOURCE_DIR}/UI/Pages/Account/AccountPage.cpp
    ${CMAKE_SOURCE_DIR}/UI/Dialogs/EditPersonalInfoDialog.cpp
    ${CMAKE_SOURCE_DIR}/UI/Dialogs/ChangePasswordDialog.cpp
    ${CMAKE_SOURCE_DIR}/UI/Dialogs/FieldDialog.cpp
    ${CMAKE_SOURCE_DIR}/UI/Dialogs/MaintenanceDialog.cpp
    ${CMAKE_SOURCE_DIR}/UI/Dialogs/AddCustomerDialog.cpp
)

set(UI_HEADERS
    ${CMAKE_SOURCE_DIR}/UI/Pages/Login/LoginDialog.h
    ${CMAKE_SOURCE_DIR}/UI/MainWindow/MainWindow.h
    ${CMAKE_SOURCE_DIR}/UI/Components/Sidebar.h
    ${CMAKE_SOURCE_DIR}/UI/Components/Header.h
    ${CMAKE_SOURCE_DIR}/UI/Pages/Booking/BookingListPage.h
    ${CMAKE_SOURCE_DIR}/UI/Pages/Payment/PaymentListPage.h
    ${CMAKE_SOURCE_DIR}/UI/Pages/Field/FieldManagementPage.h
    ${CMAKE_SOURCE_DIR}/UI/Pages/Customer/CustomerManagementPage.h
    ${CMAKE_SOURCE_DIR}/UI/Pages/Service/ServiceManagementPage.h
    ${CMAKE_SOURCE_DIR}/UI/Pages/Service/ServiceOrderWidget.h
    ${CMAKE_SOURCE_DIR}/UI/Pages/Service/ServiceManagementWidget.h
    ${CMAKE_SOURCE_DIR}/UI/Pages/Staff/StaffManagementPage.h
    ${CMAKE_SOURCE_DIR}/UI/Pages/Statistics/StatisticsPage.h
    ${CMAKE_SOURCE_DIR}/UI/Pages/Account/AccountPage.h
    ${CMAKE_SOURCE_DIR}/UI/Dialogs/EditPersonalInfoDialog.h
    ${CMAKE_SOURCE_DIR}/UI/Dialogs/ChangePasswordDialog.h
    ${CMAKE_SOURCE_DIR}/UI/Dialogs/FieldDialog.h
    ${CMAKE_SOURCE_DIR}/UI/Dialogs/MaintenanceDialog.h
    ${CMAKE_SOURCE_DIR}/UI/Dialogs/AddCustomerDialog.h
)

# Resources (Week 1 Day 1)
set(UI_RESOURCES
    ${CMAKE_SOURCE_DIR}/UI/Resources/resources.qrc
)

# Main entry point
set(MAIN_SOURCES
    main.cpp
)

# Combine Core sources
set(CORE_SOURCES
    ${CORE_CAUTRUCDULIEU_SOURCES}
    ${CORE_MODELS_SOURCES}
    ${CORE_QUANLY_SOURCES}
    ${CORE_THONGKE_SOURCES}
    ${CORE_UTILS_SOURCES}
)

set(CORE_HEADERS
    ${CORE_CAUTRUCDULIEU_HEADERS}
    ${CORE_MODELS_HEADERS}
    ${CORE_QUANLY_HEADERS}
    ${CORE_THONGKE_HEADERS}
    ${CORE_THUATTOAN_HEADERS}
    ${CORE_UTILS_HEADERS}
)

# Combine all sources
set(PROJECT_SOURCES
    ${CORE_SOURCES}
    ${UI_SOURCES}
    ${MAIN_SOURCES}
)

set(PROJECT_HEADERS
    ${CORE_HEADERS}
    ${UI_HEADERS}
)

set(PROJECT_RESOURCES
    ${UI_RESOURCES}
)

# ============================================
# EXECUTABLE: Qt6 Widgets Application
# ============================================
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_RESOURCES}
)

# Link Qt libraries (NO Charts - no dashboard)
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
)

# Compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# Output directories - thống nhất build output
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin
)

# Install targets
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Copy Data directory to build - COMMENTED OUT to prevent CMake infinite loop
# file(COPY ${CMAKE_SOURCE_DIR}/Data DESTINATION ${CMAKE_BINARY_DIR}/bin)
# Data files should be copied manually or via custom command

# Print configuration info
message(STATUS "==================================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Qt6 Found: ${Qt6_FOUND}")
message(STATUS "==================================")
