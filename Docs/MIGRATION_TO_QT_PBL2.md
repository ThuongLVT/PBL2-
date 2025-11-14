# Migration to D:\QT_PBL2

## Summary

The QT_FOOTBALL project has been successfully separated from the html_test workspace and moved to a dedicated directory at `D:\QT_PBL2`.

## Changes Made

### 1. Path Updates

All absolute paths in the codebase have been updated from:

- `D:/html_test/QT_FOOTBALL` → `D:/QT_PBL2`
- `D:\html_test\QT_FOOTBALL` → `D:\QT_PBL2`
- `d:\html_test\FootballFieldManager` → `d:\QT_PBL2`

### 2. Files Updated

#### Source Files

- **main.cpp**: Updated `DATA_DIR` constant
- **CustomerManagementPage.cpp**: Updated 3 CSV save paths
- **AccountPage.cpp**: Updated 2 binary data save paths
- **MainWindow.cpp**: Updated data save path on quit
- **HeThongQuanLy.h**: Updated documentation comment

#### Documentation Files

All markdown files in root and `Docs/` folder have been updated with new paths.

#### Scripts

- **check_image.ps1**: Updated image path
- **BUILD_RUN.md**: Updated build and run instructions

### 3. Directory Structure

```
D:\QT_PBL2\
├── Core\               # Business logic & data structures
├── UI\                 # Qt6 GUI components
├── Data\               # CSV data files
├── Docs\               # Documentation
├── build\              # Build output (will be regenerated)
├── main.cpp
├── CMakeLists.txt
└── README.md
```

## How to Build & Run

### Prerequisites

- Qt 6.9.3 (MinGW 64-bit)
- CMake 3.30+
- Ninja build system

### Build Instructions

```bash
cd D:\QT_PBL2
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=C:/Qt/6.9.3/mingw_64
cmake --build build --config Release
```

### Run Instructions

```bash
cd D:\QT_PBL2\build\bin
.\FootballFieldManager.exe
```

## Data Locations

- **CSV Files**: `D:\QT_PBL2\Data\*.csv`
- **Binary Data**: `D:\QT_PBL2\Data\data.bin`
- **Backups**: `D:\QT_PBL2\Data\backup\`

## Important Notes

1. **Build folder**: The `build` folder from the old location contains outdated paths. It's recommended to delete it and rebuild from scratch:

   ```bash
   cd D:\QT_PBL2
   rmdir /s /q build
   cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=C:/Qt/6.9.3/mingw_64
   cmake --build build
   ```

2. **Data migration**: If you have important data in `D:\html_test\QT_FOOTBALL\Data\`, copy it to `D:\QT_PBL2\Data\` before running the application.

3. **Git repository**: If this is a git repository, you may want to update the remote URL or create a new repository.

## Default Login Credentials

- **Admin**: username: `admin`, password: `admin123`
- **Staff1**: username: `staff1`, password: `staff123`
- **Staff2**: username: `staff2`, password: `staff123`

## Migration Date

November 14, 2025

## Original Location

The project was previously located at: `D:\html_test\QT_FOOTBALL\`

---

For more information, see:

- [BUILD_RUN.md](./BUILD_RUN.md) - Build and run instructions
- [README.md](./README.md) - Project overview
- [Docs/QUICKSTART.md](./Docs/QUICKSTART.md) - Quick start guide
