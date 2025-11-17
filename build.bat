@echo off
REM Library Management System - Windows Build Script
REM For use with MinGW g++ or Microsoft Visual C++

echo ========================================
echo Library Management System - Build Script
echo ========================================
echo.

REM Check if g++ is available
where g++ >nul 2>nul
if %errorlevel% neq 0 (
    echo ERROR: g++ compiler not found!
    echo Please install MinGW-w64 or add it to your PATH
    echo Download from: https://www.mingw-w64.org/
    pause
    exit /b 1
)

echo [1/4] Checking g++ version...
g++ --version
echo.

echo [2/4] Creating output directories...
if not exist obj mkdir obj
if not exist obj\entities mkdir obj\entities
if not exist obj\management mkdir obj\management
if not exist obj\utils mkdir obj\utils
if not exist data mkdir data
echo Directories created.
echo.

echo [3/4] Compiling source files...

REM Compile entity classes
echo Compiling Book.cpp...
g++ -std=c++11 -Wall -Wextra -c src\entities\Book.cpp -o obj\entities\Book.o
if %errorlevel% neq 0 goto :compile_error

echo Compiling User.cpp...
g++ -std=c++11 -Wall -Wextra -c src\entities\User.cpp -o obj\entities\User.o
if %errorlevel% neq 0 goto :compile_error

echo Compiling Transaction.cpp...
g++ -std=c++11 -Wall -Wextra -c src\entities\Transaction.cpp -o obj\entities\Transaction.o
if %errorlevel% neq 0 goto :compile_error

REM Compile management classes
echo Compiling LibraryManager.cpp...
g++ -std=c++11 -Wall -Wextra -c src\management\LibraryManager.cpp -o obj\management\LibraryManager.o
if %errorlevel% neq 0 goto :compile_error

echo Compiling AuthManager.cpp...
g++ -std=c++11 -Wall -Wextra -c src\management\AuthManager.cpp -o obj\management\AuthManager.o
if %errorlevel% neq 0 goto :compile_error

REM Compile utility classes
echo Compiling BookBST.cpp...
g++ -std=c++11 -Wall -Wextra -c src\utils\BookBST.cpp -o obj\utils\BookBST.o
if %errorlevel% neq 0 goto :compile_error

echo Compiling UserHashMap.cpp...
g++ -std=c++11 -Wall -Wextra -c src\utils\UserHashMap.cpp -o obj\utils\UserHashMap.o
if %errorlevel% neq 0 goto :compile_error

echo Compiling TransactionList.cpp...
g++ -std=c++11 -Wall -Wextra -c src\utils\TransactionList.cpp -o obj\utils\TransactionList.o
if %errorlevel% neq 0 goto :compile_error

echo Compiling SearchEngine.cpp...
g++ -std=c++11 -Wall -Wextra -c src\utils\SearchEngine.cpp -o obj\utils\SearchEngine.o
if %errorlevel% neq 0 goto :compile_error

echo Compiling FileHandler.cpp...
g++ -std=c++11 -Wall -Wextra -c src\utils\FileHandler.cpp -o obj\utils\FileHandler.o
if %errorlevel% neq 0 goto :compile_error

REM Compile main
echo Compiling main.cpp...
g++ -std=c++11 -Wall -Wextra -c src\main.cpp -o obj\main.o
if %errorlevel% neq 0 goto :compile_error

echo.
echo [4/4] Linking executable...
g++ -std=c++11 -Wall -Wextra -o library_system.exe obj\main.o obj\entities\Book.o obj\entities\User.o obj\entities\Transaction.o obj\management\LibraryManager.o obj\management\AuthManager.o obj\utils\BookBST.o obj\utils\UserHashMap.o obj\utils\TransactionList.o obj\utils\SearchEngine.o obj\utils\FileHandler.o

if %errorlevel% neq 0 goto :link_error

echo.
echo ========================================
echo BUILD SUCCESSFUL!
echo ========================================
echo.
echo Executable created: library_system.exe
echo Run with: library_system.exe
echo.
pause
exit /b 0

:compile_error
echo.
echo ========================================
echo COMPILATION ERROR!
echo ========================================
echo.
echo Failed to compile source files.
echo Please check the error messages above.
echo.
pause
exit /b 1

:link_error
echo.
echo ========================================
echo LINKING ERROR!
echo ========================================
echo.
echo Failed to link object files.
echo Please check the error messages above.
echo.
pause
exit /b 1
