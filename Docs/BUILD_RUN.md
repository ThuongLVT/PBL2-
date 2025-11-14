# Build & Run Instructions

## Build

```bash
cd D:\QT_PBL2
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=C:/Qt/6.9.3/mingw_64
cmake --build build --config Release
```

## Run

```bash
cd D:\QT_PBL2\build\bin
.\FootballFieldManager.exe
```

## Data Location

- All data is saved to: `D:\QT_PBL2\Data\data.bin`
- Data persists across runs
- No need to delete data when rebuilding

## Login Credentials

- **Admin**: username: `admin`, password: `admin123`
- **Staff1**: username: `staff1`, password: `staff123`
- **Staff2**: username: `staff2`, password: `staff123`
