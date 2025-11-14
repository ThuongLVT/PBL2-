# 🔧 BUILD INSTRUCTIONS - Chi tiết

## 📋 Mục lục

1. [Build với VS Code + CMake](#build-với-vs-code--cmake)
2. [Build với Qt Creator + CMake](#build-với-qt-creator--cmake)
3. [Build với Qt Creator + qmake](#build-với-qt-creator--qmake)
4. [Build từ Command Line](#build-từ-command-line)
5. [Troubleshooting](#troubleshooting)

---

## Build với VS Code + CMake

### Bước 1: Cài đặt Extensions

Mở VS Code và cài các extension sau:

- **C/C++** (Microsoft)
- **CMake Tools** (Microsoft)
- **CMake** (twxs)

### Bước 2: Cấu hình CMake

1. Mở folder `FootballFieldManager` trong VS Code
2. VS Code sẽ tự động phát hiện `CMakeLists.txt`
3. Chọn kit compiler (MinGW hoặc MSVC):
   - `Ctrl+Shift+P` → "CMake: Select a Kit"
   - Chọn: `GCC x.x.x mingw64` hoặc `Visual Studio ...`

### Bước 3: Build

**Cách 1: Sử dụng Tasks**

```bash
Ctrl+Shift+B → Chọn "CMake: Build"
```

**Cách 2: Sử dụng CMake Tools**

```bash
# Configure
Ctrl+Shift+P → "CMake: Configure"

# Build
Ctrl+Shift+P → "CMake: Build"

# Run
Ctrl+Shift+P → "CMake: Run Without Debugging"
```

**Cách 3: Sử dụng Command Line trong VS Code Terminal**

```bash
# Configure
cmake -B build -S . -G "MinGW Makefiles"

# Build
cmake --build build

# Run
build\bin\FootballFieldManager.exe
```

### Bước 4: Debug

1. Đặt breakpoint tại dòng code cần debug
2. `F5` hoặc `Ctrl+Shift+D` → Start Debugging
3. Chọn configuration: "(gdb) Launch"

---

## Build với Qt Creator + CMake

### Bước 1: Mở Project

1. Mở Qt Creator
2. `File` → `Open File or Project...`
3. Chọn file `CMakeLists.txt` trong folder `FootballFieldManager`

### Bước 2: Cấu hình Kit

1. Qt Creator sẽ hiển thị dialog "Configure Project"
2. Chọn kit(s) phù hợp:
   - **Desktop Qt 6.x.x MinGW 64-bit**
   - hoặc **Desktop Qt 6.x.x MSVC2019 64bit**
3. Click "Configure Project"

### Bước 3: Build & Run

```bash
# Build
Ctrl+B

# Run
Ctrl+R

# Clean
Build menu → Clean All

# Rebuild
Build menu → Rebuild All
```

### Tips Qt Creator:

- **Switch Build Configuration**: Click "Debug" or "Release" ở góc dưới trái
- **View Build Output**: Click "4 Compile Output" ở dưới
- **Issues**: Click "1 Issues" để xem errors/warnings

---

## Build với Qt Creator + qmake

### Bước 1: Mở Project

1. Mở Qt Creator
2. `File` → `Open File or Project...`
3. Chọn file `FootballFieldManager.pro`

### Bước 2: Build & Run

```bash
# Build
Ctrl+B

# Run
Ctrl+R
```

**Note**: qmake là phương pháp legacy. Khuyến nghị dùng CMake.

---

## Build từ Command Line

### Windows (MinGW)

```bash
# 1. Mở Command Prompt hoặc PowerShell
cd d:\QT_FOOTBALL

# 2. Configure CMake
cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

# 3. Build
cmake --build build --config Release -j 4

# 4. Run
build\bin\FootballFieldManager.exe
```

### Windows (MSVC)

```bash
# 1. Mở Developer Command Prompt for VS
cd d:\QT_FOOTBALL

# 2. Configure CMake
cmake -B build -S . -G "Visual Studio 17 2022" -A x64

# 3. Build
cmake --build build --config Release

# 4. Run
build\bin\Release\FootballFieldManager.exe
```

### Linux/macOS

```bash
# 1. Terminal
cd /path/to/FootballFieldManager

# 2. Configure
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

# 3. Build
cmake --build build -j $(nproc)

# 4. Run
./build/bin/FootballFieldManager
```

---

## Build Options

### Debug vs Release

```bash
# Debug (với debug symbols, chậm hơn)
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug

# Release (tối ưu hóa, nhanh hơn)
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
```

### Parallel Build (nhanh hơn)

```bash
# Windows MinGW
cmake --build build -j 4

# Windows MSVC
cmake --build build --config Release -- /m:4

# Linux/macOS
cmake --build build -j $(nproc)
```

### Verbose Output (xem command chi tiết)

```bash
cmake --build build --verbose
```

### Clean Build

```bash
# Clean only
cmake --build build --target clean

# Clean + Rebuild
cmake --build build --clean-first
```

---

## Troubleshooting

### ❌ "Qt6 not found"

**Giải pháp:**

```bash
# Set Qt6_DIR environment variable
set Qt6_DIR=C:\Qt\6.9.3\mingw_64\lib\cmake\Qt6

# Hoặc dùng CMAKE_PREFIX_PATH
cmake -B build -S . -DCMAKE_PREFIX_PATH=C:\Qt\6.9.3\mingw_64
```

### ❌ "CMake not found"

**Giải pháp:**

- Cài CMake: https://cmake.org/download/
- Hoặc dùng CMake đi kèm Qt Creator: `C:\Qt\Tools\CMake_64\bin`
- Thêm vào PATH

### ❌ "Generator not found" (MinGW)

**Giải pháp:**

```bash
# Kiểm tra MinGW đã cài
where mingw32-make

# Nếu chưa có, cài MinGW từ Qt hoặc https://www.mingw-w64.org/
```

### ❌ Build errors về missing headers

**Giải pháp:**

```bash
# Clean build directory
rm -rf build

# Reconfigure
cmake -B build -S .

# Rebuild
cmake --build build
```

### ❌ "undefined reference to Qt6::..."

**Giải pháp:**

- Kiểm tra Qt đã link đúng chưa
- Trong CMakeLists.txt, đảm bảo có:

```cmake
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts
    ...
)
```

### ❌ Console không hiển thị output

**Giải pháp:**

- Trên Windows, chạy từ Command Prompt/PowerShell
- Hoặc trong CMakeLists.txt thêm:

```cmake
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE FALSE  # Force console window
    )
endif()
```

---

## Useful Commands

```bash
# Xem CMake configuration
cmake -B build -S . -LA

# Generate compile_commands.json (for IntelliSense)
cmake -B build -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Install to system
cmake --install build --prefix /usr/local

# Run tests (khi có)
ctest --test-dir build

# CPack (tạo installer)
cpack --config build/CPackConfig.cmake
```

---

## IDE-specific Tips

### VS Code

- **IntelliSense**: Đợi CMake configure xong, sau đó `C_Cpp.default.configurationProvider` sẽ tự động dùng CMake
- **Integrated Terminal**: `Ctrl+` ` ` để mở terminal ngay trong VS Code
- **Problems Panel**: `Ctrl+Shift+M` để xem errors/warnings

### Qt Creator

- **Build Directory**: Mặc định là `build-FootballFieldManager-Desktop_Qt_X_X_X_MinGW_64_bit-Debug`
- **Environment Variables**: Tools → Options → Kits → Environment
- **Qt Designer**: Double-click file `.ui` để mở designer

---

**Happy Coding! 🚀**
