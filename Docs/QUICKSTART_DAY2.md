# 🚀 Quick Start - Football Field Manager

## ✅ Day 2 Completed - Login Dialog

### Build & Run

```bash
# Build the project
D:\\QT_PBL2\build.bat

# Run the application
D:\\QT_PBL2\build\bin\FootballFieldManager.exe
```

Or use the run script:

```bash
D:\\QT_PBL2\run.bat
```

### Login Credentials

**Admin:**

- Username: `admin`
- Password: `1`

**Staff:**

- Username: `staff1`
- Password: `staff123`

### Features Implemented ✅

#### Day 1: Project Setup

- ✅ UI folder structure created
- ✅ CMakeLists.txt configured with Qt6 Widgets
- ✅ Resources.qrc with icons and images
- ✅ app.qss stylesheet with green theme (#16a34a)

#### Day 2: Login Dialog

- ✅ Full-screen login with football field background
- ✅ Dark semi-transparent dialog (rgba(0,0,0,180))
- ✅ Username and password fields
- ✅ Login button (green theme)
- ✅ Close button (red theme) - exits application
- ✅ Authentication with HeThongQuanLy::getInstance()
- ✅ Error messages with QMessageBox
- ✅ QSS styling applied

### Project Structure

```
QT_FOOTBALL/
├── build/                              # Build output
│   └── bin/
│       └── FootballFieldManager.exe   # Main executable
├── UI/
│   ├── Pages/
│   │   └── Login/
│   │       ├── LoginDialog.h          # Login dialog header
│   │       └── LoginDialog.cpp        # Login dialog implementation
│   └── Resources/
│       ├── images/
│       │   └── football_field_bg.jpg  # Background image
│       ├── icons/                      # SVG icons
│       ├── styles/
│       │   └── app.qss                # Global stylesheet
│       └── resources.qrc              # Qt resource file
├── Core/                               # Business logic (completed in Week 1-4)
├── build.bat                           # Build script
├── run.bat                            # Run script
└── create_bg_image.ps1                # PowerShell script to generate background

```

### Next Steps 📋

**Day 3-4: MainWindow với Sidebar**

- [ ] Create MainWindow.h/cpp
- [ ] Create Sidebar.h/cpp with collapsible menu
- [ ] Implement navigation with QStackedWidget
- [ ] Add Header.h/cpp with user info
- [ ] Role-based menu (Admin vs Staff)

### Technical Details

**Qt Version:** 6.9.3  
**C++ Standard:** C++17  
**Build System:** CMake + Ninja  
**Compiler:** MinGW 64-bit  
**UI Framework:** Qt6 Widgets (NO Qt Charts)

### Color Palette

```css
Primary Green:   #16a34a  /* Main brand color */
Green Dark:      #15803d  /* Hover states */
Green Light:     #22c55e  /* Accents */
Success Green:   #10b981  /* Success messages */
Danger Red:      #ef4444  /* Errors, cancel */
Text Dark:       #1f2937  /* Headers */
Background:      #f9fafb  /* Light backgrounds */
```

### Screenshot

When you run the application, you'll see:

1. Full-screen football field background (1920x1080)
2. Centered dark dialog with login form
3. Green "Đăng nhập" button
4. Red "Đóng" button to exit

After successful login with `admin/1`, the console shows:

```
✅ Login successful!
   Username: admin
   Role: Admin
```

---

**Status:** ✅ Day 2 Complete - Ready for Day 3  
**Last Updated:** November 8, 2025  
**Next:** MainWindow with Sidebar (Day 3-4)
