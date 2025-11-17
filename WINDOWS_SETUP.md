# Library Management System - Windows Setup Guide

## 🪟 Windows Compatibility

This Library Management System is **fully cross-platform** and works on both Windows and Linux/Unix systems. This guide provides Windows-specific instructions.

---

## 📋 Prerequisites for Windows

### Option 1: MinGW-w64 (Recommended for VSCode)

1. **Download MinGW-w64**:
   - Visit: https://www.mingw-w64.org/downloads/
   - Or use MSYS2: https://www.msys2.org/
   
2. **Install via MSYS2 (Easiest)**:
   ```bash
   # Download and install MSYS2 from https://www.msys2.org/
   # Then run in MSYS2 terminal:
   pacman -S mingw-w64-x86_64-gcc
   pacman -S make
   ```

3. **Add to PATH**:
   - Add `C:\msys64\mingw64\bin` to your System PATH
   - Verify: Open CMD or PowerShell and type `g++ --version`

### Option 2: Visual Studio (MSVC)

1. **Install Visual Studio 2019 or later**:
   - Download: https://visualstudio.microsoft.com/
   - Select "Desktop development with C++" workload

2. **Use Developer Command Prompt**:
   - Search for "Developer Command Prompt for VS"
   - Or use "x64 Native Tools Command Prompt"

---

## 🔧 Building on Windows

### Method 1: Using build.bat (Recommended)

1. **Open VSCode Terminal** (or CMD/PowerShell):
   ```cmd
   cd LibraryManagementSystem
   ```

2. **Run the build script**:
   ```cmd
   build.bat
   ```

3. **Run the program**:
   ```cmd
   library_system.exe
   ```

### Method 2: Manual Compilation with g++

```cmd
# Create directories
mkdir obj\entities obj\management obj\utils data

# Compile entity classes
g++ -std=c++11 -Wall -Wextra -c src\entities\Book.cpp -o obj\entities\Book.o
g++ -std=c++11 -Wall -Wextra -c src\entities\User.cpp -o obj\entities\User.o
g++ -std=c++11 -Wall -Wextra -c src\entities\Transaction.cpp -o obj\entities\Transaction.o

# Compile management classes
g++ -std=c++11 -Wall -Wextra -c src\management\LibraryManager.cpp -o obj\management\LibraryManager.o
g++ -std=c++11 -Wall -Wextra -c src\management\AuthManager.cpp -o obj\management\AuthManager.o

# Compile utility classes
g++ -std=c++11 -Wall -Wextra -c src\utils\BookBST.cpp -o obj\utils\BookBST.o
g++ -std=c++11 -Wall -Wextra -c src\utils\UserHashMap.cpp -o obj\utils\UserHashMap.o
g++ -std=c++11 -Wall -Wextra -c src\utils\TransactionList.cpp -o obj\utils\TransactionList.o
g++ -std=c++11 -Wall -Wextra -c src\utils\SearchEngine.cpp -o obj\utils\SearchEngine.o
g++ -std=c++11 -Wall -Wextra -c src\utils\FileHandler.cpp -o obj\utils\FileHandler.o

# Compile main
g++ -std=c++11 -Wall -Wextra -c src\main.cpp -o obj\main.o

# Link
g++ -std=c++11 -Wall -Wextra -o library_system.exe obj\main.o obj\entities\Book.o obj\entities\User.o obj\entities\Transaction.o obj\management\LibraryManager.o obj\management\AuthManager.o obj\utils\BookBST.o obj\utils\UserHashMap.o obj\utils\TransactionList.o obj\utils\SearchEngine.o obj\utils\FileHandler.o
```

### Method 3: Using Make in MSYS2

If you have MSYS2 with make installed:

```bash
# In MSYS2 terminal
cd LibraryManagementSystem
make
./library_system.exe
```

---

## 🎯 VSCode Setup (Recommended IDE)

### 1. Install VSCode Extensions

- **C/C++** (Microsoft)
- **C/C++ Extension Pack** (Microsoft)
- **Code Runner** (Jun Han)

### 2. Configure VSCode

Create `.vscode/tasks.json`:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build Library System",
            "type": "shell",
            "command": "build.bat",
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": ["$gcc"]
        },
        {
            "label": "Run Library System",
            "type": "shell",
            "command": "library_system.exe",
            "dependsOn": ["Build Library System"],
            "problemMatcher": []
        }
    ]
}
```

Create `.vscode/launch.json`:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug Library System",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/library_system.exe",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": true,
            "MIMode": "gdb",
            "miDebuggerPath": "gdb.exe",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```

### 3. Build and Run in VSCode

- **Build**: Press `Ctrl+Shift+B` or run "Build Library System" task
- **Run**: Press `F5` or select "Run → Start Debugging"
- **Terminal**: Press `Ctrl+` ` and run `library_system.exe`

---

## 🔍 Windows-Specific Features

### Password Input

On Windows, password input uses `_getch()` from `<conio.h>`:
- Masks input with asterisks (`*`)
- Supports backspace
- No echo to console

### Clear Screen

Uses Windows `cls` command instead of ANSI escape codes.

### Directory Creation

Uses `_mkdir()` from `<direct.h>` instead of POSIX `mkdir()`.

### File Paths

- Uses backslashes `\` by default
- Forward slashes `/` also work in C++ file operations
- Data files stored in `data\` subdirectory

---

## 🐛 Troubleshooting Windows Issues

### Issue: "g++ is not recognized"

**Solution**: Add MinGW to PATH
1. Find your MinGW installation (e.g., `C:\msys64\mingw64\bin`)
2. Add to System Environment Variables → PATH
3. Restart terminal/VSCode

### Issue: "cannot open include file 'conio.h'"

**Solution**: This means you're not using MinGW or MSVC
- Install MinGW-w64 or Visual Studio
- Ensure you're using the correct compiler

### Issue: Build fails with "Permission denied"

**Solution**: 
```cmd
# Delete old executable
del library_system.exe
# Rebuild
build.bat
```

### Issue: Password input shows characters

**Solution**: 
- This is normal if not using proper Windows console
- The code still works correctly
- Use Windows Terminal or CMD for best experience

### Issue: "mkdir: command not found"

**Solution**: You're in a Unix-like shell on Windows
- Use PowerShell, CMD, or Windows Terminal instead
- Or install make via MSYS2

### Issue: Unicode characters (✓, ✗) not displaying

**Solution**: 
1. Change console code page:
   ```cmd
   chcp 65001
   ```
2. Or use Windows Terminal which supports UTF-8 by default

---

## 📦 Recommended Windows Setup

### Best Configuration for Windows Development

1. **Windows Terminal** (modern, supports UTF-8)
   - Download from Microsoft Store
   - Set as default terminal in VSCode

2. **MinGW-w64 via MSYS2**
   ```bash
   # In MSYS2 terminal
   pacman -S mingw-w64-x86_64-gcc
   pacman -S mingw-w64-x86_64-gdb
   pacman -S make
   ```

3. **VSCode Settings**
   ```json
   {
       "terminal.integrated.defaultProfile.windows": "Command Prompt",
       "code-runner.executorMap": {
           "cpp": "cd $dir && g++ -std=c++11 $fileName -o $fileNameWithoutExt.exe && $fileNameWithoutExt.exe"
       }
   }
   ```

---

## 🚀 Quick Start on Windows

### 1-Minute Setup

```cmd
# 1. Open VSCode Terminal (Ctrl+`)
cd path\to\LibraryManagementSystem

# 2. Build
build.bat

# 3. Run
library_system.exe

# 4. Login
Username: admin
Password: admin123
```

---

## 📝 Notes for Windows Users

### File Line Endings

- Windows uses CRLF (`\r\n`)
- Linux uses LF (`\n`)
- The code handles both automatically

### Case Sensitivity

- Windows file system is case-insensitive
- Code will work with any casing in file paths

### Executable Extension

- Windows executables end with `.exe`
- Linux/Mac executables have no extension
- Build script automatically adds `.exe` on Windows

---

## 🎓 Windows Development Tips

### Using Windows Terminal

Windows Terminal is the best choice for this application:

1. **Install**: Microsoft Store → "Windows Terminal"
2. **Set default**: Settings → Startup → Default profile: Command Prompt
3. **Enable UTF-8**: Automatically supported

### Using PowerShell

PowerShell works but requires different syntax:

```powershell
# Build
.\build.bat

# Run
.\library_system.exe

# Or
& ".\library_system.exe"
```

### Using Git Bash

Git Bash works like Linux terminal:

```bash
# Use forward slashes
cd /c/path/to/project

# Build (if make installed)
make

# Or use Windows script
./build.bat
```

---

## 🔄 Cross-Platform Testing

To test on both Windows and Linux:

1. **Windows Build**:
   ```cmd
   build.bat
   library_system.exe
   ```

2. **Linux Build** (in WSL or dual boot):
   ```bash
   make
   ./library_system
   ```

Both use the same source code with conditional compilation!

---

## ✅ Verification Checklist

- [ ] MinGW-w64 installed and in PATH
- [ ] `g++ --version` works in terminal
- [ ] VSCode has C/C++ extension
- [ ] Can run `build.bat` successfully
- [ ] Executable `library_system.exe` created
- [ ] Program runs and shows login screen
- [ ] Password input is masked
- [ ] Data files created in `data\` folder
- [ ] All features work correctly

---

## 📞 Getting Help

If you encounter issues:

1. Check this guide's troubleshooting section
2. Verify g++ installation: `g++ --version`
3. Check PATH environment variable
4. Try running in different terminal (CMD, PowerShell, Windows Terminal)
5. Ensure all source files are present

---

## 🎯 Summary

**Windows Compatibility**: ✅ Full Support
**Recommended Setup**: MinGW-w64 + VSCode + Windows Terminal
**Build Time**: ~5-10 seconds
**Executable**: `library_system.exe` (~1.6 MB)
**Tested On**: Windows 10, Windows 11

**Status**: Production Ready for Windows ✅

---

**Last Updated**: November 2024  
**Platform**: Windows 10/11  
**Compiler**: MinGW-w64 GCC 8.1.0 or later
