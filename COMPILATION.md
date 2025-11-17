# Compilation and Execution Instructions

## Prerequisites

### Required Software
- **Operating System**: Linux or Unix-based system
- **Compiler**: G++ (version 4.8 or higher) with C++11 support
- **Build Tool**: Make utility
- **Terminal**: Any terminal emulator

### Check Prerequisites

```bash
# Check G++ version (should be 4.8+)
g++ --version

# Check Make installation
make --version

# Check C++11 support
g++ -std=c++11 --version
```

If any tool is missing, install build-essential:
```bash
sudo apt-get update
sudo apt-get install build-essential
```

---

## Compilation Steps

### Method 1: Using Makefile (Recommended)

```bash
# Navigate to project directory
cd /path/to/LibraryManagementSystem

# Compile the project
make

# The executable 'library_system' will be created
```

Expected output:
```
g++ -std=c++11 -Wall -Wextra -g -c src/main.cpp -o obj/main.o
g++ -std=c++11 -Wall -Wextra -g -c src/entities/Book.cpp -o obj/entities/Book.o
...
Build successful! Run with: ./library_system
```

### Method 2: Manual Compilation

If Make is not available:

```bash
# Create object directory
mkdir -p obj/entities obj/management obj/utils

# Compile entity classes
g++ -std=c++11 -Wall -Wextra -g -c src/entities/Book.cpp -o obj/entities/Book.o
g++ -std=c++11 -Wall -Wextra -g -c src/entities/User.cpp -o obj/entities/User.o
g++ -std=c++11 -Wall -Wextra -g -c src/entities/Transaction.cpp -o obj/entities/Transaction.o

# Compile management classes
g++ -std=c++11 -Wall -Wextra -g -c src/management/LibraryManager.cpp -o obj/management/LibraryManager.o
g++ -std=c++11 -Wall -Wextra -g -c src/management/AuthManager.cpp -o obj/management/AuthManager.o

# Compile utility classes
g++ -std=c++11 -Wall -Wextra -g -c src/utils/BookBST.cpp -o obj/utils/BookBST.o
g++ -std=c++11 -Wall -Wextra -g -c src/utils/UserHashMap.cpp -o obj/utils/UserHashMap.o
g++ -std=c++11 -Wall -Wextra -g -c src/utils/TransactionList.cpp -o obj/utils/TransactionList.o
g++ -std=c++11 -Wall -Wextra -g -c src/utils/SearchEngine.cpp -o obj/utils/SearchEngine.o
g++ -std=c++11 -Wall -Wextra -g -c src/utils/FileHandler.cpp -o obj/utils/FileHandler.o

# Compile main
g++ -std=c++11 -Wall -Wextra -g -c src/main.cpp -o obj/main.o

# Link all objects
g++ -std=c++11 -Wall -Wextra -g -o library_system \
    obj/main.o \
    obj/entities/Book.o \
    obj/entities/User.o \
    obj/entities/Transaction.o \
    obj/management/LibraryManager.o \
    obj/management/AuthManager.o \
    obj/utils/BookBST.o \
    obj/utils/UserHashMap.o \
    obj/utils/TransactionList.o \
    obj/utils/SearchEngine.o \
    obj/utils/FileHandler.o
```

---

## Running the Program

### Execute the Program

```bash
./library_system
```

Or using Make:

```bash
make run
```

### First Run

On first execution:
1. The system will create a `data/` directory
2. Sample data will be initialized (10 books)
3. You'll see the login screen

Default admin credentials:
- Username: `admin`
- Password: `admin123`

---

## Makefile Targets

```bash
make          # Compile the project
make all      # Same as 'make'
make run      # Compile and run
make clean    # Remove all build files
make rebuild  # Clean and rebuild from scratch
```

### Clean Build

If you encounter issues:

```bash
make clean    # Remove old build files
make          # Rebuild from scratch
```

---

## Compiler Flags Explained

- `-std=c++11`: Use C++11 standard
- `-Wall`: Enable all common warnings
- `-Wextra`: Enable extra warnings
- `-g`: Include debug symbols (for debugging with gdb)

For release build (without debug symbols):

```bash
# Modify CXXFLAGS in Makefile to:
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# Then rebuild
make rebuild
```

---

## Troubleshooting Compilation

### Error: "command not found: g++"

**Solution**: Install G++
```bash
sudo apt-get install g++
```

### Error: "command not found: make"

**Solution**: Install Make
```bash
sudo apt-get install make
```

### Error: "C++11 features not supported"

**Solution**: Update G++ to version 4.8 or higher
```bash
sudo apt-get update
sudo apt-get upgrade g++
```

### Error: "undefined reference to..."

**Solution**: This is a linking error. Ensure all object files are linked:
```bash
make clean
make
```

### Error: "permission denied: ./library_system"

**Solution**: Make the file executable
```bash
chmod +x library_system
```

### Error: Multiple definition errors

**Solution**: You may have compiled twice. Clean and rebuild:
```bash
make clean
make
```

### Warning: Unused variable/parameter

These are safe warnings. The code compiles successfully with `-Wall -Wextra`.
To suppress warnings (not recommended):
```bash
# Modify CXXFLAGS to:
CXXFLAGS = -std=c++11 -w
```

---

## Verification

### Check Compilation Success

```bash
# Check if executable exists
ls -lh library_system

# Should show something like:
# -rwxr-xr-x 1 user user 1.6M Nov 17 12:00 library_system
```

### Check Object Files

```bash
# Check if all object files were created
find obj/ -name "*.o"

# Should list 11 object files
```

### Test Run

```bash
# Run the program
./library_system

# You should see the login screen
# Press Ctrl+C to exit if needed
```

---

## File Structure After Compilation

```
LibraryManagementSystem/
├── library_system          # Executable (1.6 MB)
├── obj/                    # Object files directory
│   ├── main.o
│   ├── entities/
│   │   ├── Book.o
│   │   ├── User.o
│   │   └── Transaction.o
│   ├── management/
│   │   ├── LibraryManager.o
│   │   └── AuthManager.o
│   └── utils/
│       ├── BookBST.o
│       ├── UserHashMap.o
│       ├── TransactionList.o
│       ├── SearchEngine.o
│       └── FileHandler.o
├── data/                   # Created on first run
│   ├── books.txt
│   ├── users.txt
│   └── transactions.txt
└── [source files...]
```

---

## Performance Build (Optional)

For maximum performance (no debugging):

```bash
# Edit Makefile - change CXXFLAGS to:
CXXFLAGS = -std=c++11 -O3 -DNDEBUG

# Rebuild
make rebuild
```

This enables:
- `-O3`: Maximum optimization
- `-DNDEBUG`: Disable assert statements

**Note**: Use debug build (`-g`) for development, release build (`-O3`) for production.

---

## Cross-Platform Notes

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install build-essential
```

### Linux (Fedora/RHEL)
```bash
sudo yum install gcc-c++ make
```

### macOS
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Or install via Homebrew
brew install gcc make
```

### Windows (WSL)
```bash
# Use Windows Subsystem for Linux
# Then follow Linux instructions
sudo apt-get install build-essential
```

### Windows (MinGW)
```bash
# Install MinGW-w64
# Use g++ from MinGW, commands are the same
mingw32-make
```

---

## Automated Build Script (Optional)

Create `build.sh`:

```bash
#!/bin/bash
echo "Building Library Management System..."
make clean
make
if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "Run with: ./library_system"
else
    echo "Build failed!"
    exit 1
fi
```

Make executable and run:
```bash
chmod +x build.sh
./build.sh
```

---

## Quick Reference

```bash
# Complete build from scratch
make clean && make

# Build and run
make run

# Check for errors only (don't create executable)
make -n

# Parallel build (faster on multi-core)
make -j4

# Verbose output
make VERBOSE=1
```

---

**Build Time**: ~5-10 seconds on modern hardware  
**Executable Size**: ~1.6 MB  
**Object Files**: 11 files, ~500 KB total  

**Success Indicators**:
✓ No error messages during compilation  
✓ "Build successful!" message displayed  
✓ `library_system` executable created  
✓ Program runs without crashes
