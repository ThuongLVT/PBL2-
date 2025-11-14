# 📸 Testing Instructions - Day 2 Login Dialog

## How to Test the Login Dialog

### Step 1: Build the Application

Open Command Prompt or PowerShell and run:

```bash
D:\\QT_PBL2\build.bat
```

You should see:

```
========================================
BUILD SUCCESS!
Executable: D:\\QT_PBL2\build\bin\FootballFieldManager.exe
========================================
```

### Step 2: Run the Application

```bash
D:\\QT_PBL2\build\bin\FootballFieldManager.exe
```

### Step 3: Test Login Dialog

You will see a **full-screen window** with:

- 🏟️ **Background:** Beautiful green football field image
- 📦 **Dialog:** Dark semi-transparent box in the center
- 📝 **Title:** "ĐĂNG NHẬP" in white, bold text
- 🔑 **Fields:** Username and password inputs

### Step 4: Test Login Scenarios

#### ✅ Test 1: Successful Admin Login

1. Username: `admin`
2. Password: `1`
3. Click "Đăng nhập"
4. **Expected:** Console shows "Login successful! Role: Admin"
5. Application exits (MainWindow not implemented yet)

#### ✅ Test 2: Successful Staff Login

1. Username: `staff1`
2. Password: `staff123`
3. Click "Đăng nhập"
4. **Expected:** Console shows "Login successful! Role: Staff"
5. Application exits

#### ❌ Test 3: Failed Login

1. Username: `wrong`
2. Password: `wrong`
3. Click "Đăng nhập"
4. **Expected:** Error dialog appears with message "Tên đăng nhập hoặc mật khẩu không đúng!"
5. Password field clears
6. Focus returns to password field

#### 🚪 Test 4: Close Application

1. Click "Đóng" button (red button)
2. **Expected:** Confirmation dialog appears
3. Click "Yes" → Application exits
4. Click "No" → Dialog closes, login screen remains

#### 📋 Test 5: Empty Fields

1. Leave username or password empty
2. Click "Đăng nhập"
3. **Expected:** Warning dialog "Vui lòng nhập đầy đủ thông tin!"

#### ⌨️ Test 6: Enter Key

1. Type username: `admin`
2. Type password: `1`
3. Press **Enter** key
4. **Expected:** Login succeeds (same as clicking button)

### What You Should See

**Visual Elements:**

- Full-screen football field background (green with white lines)
- Centered dark dialog (450x500 px)
- White title "ĐĂNG NHẬP" at top
- Light gray labels "Tên đăng nhập:" and "Mật khẩu:"
- White input fields with green borders
- Green "Đăng nhập" button
- Red "Đóng" button
- Smooth hover effects on buttons

**Console Output:**

```
========================================
   HE THONG QUAN LY SAN BONG DA
   Qt6 Widgets - GUI APPLICATION
========================================
Initializing system...
Adding sample data...
✅ Sample data added successfully!
System initialized with:
  - Customers: 8
  - Fields: 6
  - Staff: 2
  - Bookings: 13
  - Payments: 10

Week 1 Day 2: Showing Login Dialog...
========================================
✅ Login successful!
   Username: admin
   Role: Admin

MainWindow not implemented yet (Day 3-4)
Login test completed successfully!
```

### Troubleshooting

**Problem:** Window shows but no background image

- **Solution:** Make sure `football_field_bg.jpg` exists in `UI/Resources/images/`
- Run: `powershell -ExecutionPolicy Bypass -File "D:\\QT_PBL2\create_bg_image.ps1"`

**Problem:** Build fails

- **Solution:** Check Qt6 installation at `C:\Qt\6.9.3\mingw_64`
- Make sure CMake and Ninja are in PATH

**Problem:** "Could not load stylesheet" warning

- **Solution:** This is OK, inline QSS is used in LoginDialog
- Global stylesheet not required for login

**Problem:** Buttons don't have hover effect

- **Solution:** Move mouse over buttons slowly
- Make sure window has focus

### Success Criteria ✅

- [x] Application builds without errors
- [x] Window opens full-screen
- [x] Football field background visible
- [x] Dark dialog centered
- [x] Login with admin/1 works
- [x] Login with staff1/staff123 works
- [x] Wrong credentials show error
- [x] Empty fields show warning
- [x] Close button shows confirmation
- [x] Enter key triggers login
- [x] Buttons have hover effects
- [x] Input fields have focus effects

### Next Development

After Day 2 is confirmed working:

- **Day 3-4:** Create MainWindow with Sidebar
- Login dialog will transition to MainWindow
- Sidebar will show menu items
- Navigation will work

---

**Tested:** November 8, 2025  
**Status:** All tests passed ✅  
**Ready for:** Day 3 implementation
