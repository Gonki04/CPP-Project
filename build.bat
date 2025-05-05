@echo off
setlocal enabledelayedexpansion

:: Check if ANSI colors are supported
    set "RED=[91m"
    set "GREEN=[92m"
    set "YELLOW=[93m"
    set "BLUE=[94m"
    set "MAGENTA=[95m"
    set "CYAN=[96m"
    set "WHITE=[97m"
    set "RESET=[0m"


:: Main Menu
:main
cls
echo %BLUE%=================================%RESET%
echo %YELLOW%    Project Build System%RESET%
echo %BLUE%=================================%RESET%
echo.
echo %WHITE%Select an option:%RESET%
echo %GREEN%[1]%RESET% Install Dependencies
echo %GREEN%[2]%RESET% Build and Run Project
echo %GREEN%[3]%RESET% Exit
echo.

choice /c 123 /n /m "Enter your choice: "
if errorlevel 3 exit /b
if errorlevel 2 goto build_run
if errorlevel 1 goto install_deps

:: Install Dependencies
:install_deps
cls
echo %BLUE%======== INSTALL DEPENDENCIES ========%RESET%
echo.

:: Check for git
where git >nul 2>nul
if %errorlevel% neq 0 (
    echo %RED%Git is not installed or not in PATH%RESET%
    echo %YELLOW%Please install Git from https://git-scm.com/%RESET%
    pause
    goto main
)

:: Check for CMake
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo %RED%CMake is not installed%RESET%
    echo %YELLOW%Attempting to install CMake...%RESET%
    
    powershell -Command "Start-Process -Verb RunAs -Wait -FilePath 'msiexec' -ArgumentList '/i','https://github.com/Kitware/CMake/releases/download/v4.0.1/cmake-4.0.1-windows-x86_64.msi','/quiet','/qn','/norestart','ADD_CMAKE_TO_PATH=System'"
    if %errorlevel% neq 0 (
        echo %RED%Failed to install CMake automatically%RESET%
        echo %YELLOW%Please install CMake manually from https://cmake.org/download/%RESET%
        pause
        goto main
    )
    
    echo %GREEN%CMake installed successfully!%RESET%
    set "PATH=%PATH%;C:\Program Files\CMake\bin"
)

:: Install Vcpkg if needed
if not defined VCPKG_ROOT (
    echo %YELLOW%Installing vcpkg...%RESET%
    
    if not exist "vcpkg" (
        git clone https://github.com/Microsoft/vcpkg.git
        if %errorlevel% neq 0 (
            echo %RED%Failed to clone vcpkg repository%RESET%
            pause
            goto main
        )
    )
    

    
    set "VCPKG_ROOT=%cd%\vcpkg"
    echo %GREEN%Vcpkg installed at: %VCPKG_ROOT%%RESET%
) else (
    echo %GREEN%Using existing vcpkg at: %VCPKG_ROOT%%RESET%
)

:: Install dependencies from vcpkg.json
if exist vcpkg.json (
    echo %YELLOW%Installing packages from vcpkg.json...%RESET%
    "%VCPKG_ROOT%\vcpkg" install --triplet x64-windows
    if %errorlevel% neq 0 (
        echo %RED%Failed to install vcpkg dependencies%RESET%
        pause
        goto main
    )
) else (
    echo %YELLOW%No vcpkg.json found - skipping package installation%RESET%
)

:: Update git submodules
echo %YELLOW%Updating git submodules...%RESET%
git submodule update --init --recursive
if %errorlevel% neq 0 (
    echo %RED%Failed to update submodules%RESET%
    pause
    goto main
)

echo %GREEN%All dependencies installed successfully!%RESET%
pause
goto main

:: Build and Run Project
:build_run
cls
echo %BLUE%======== BUILD AND RUN PROJECT ========%RESET%
echo.

    if exist vcpkg/ (
       
       set "VCPKG_ROOT=%cd%\vcpkg"
)

:: Verify VCPKG_ROOT is set
if not defined VCPKG_ROOT (
    echo %RED%VCPKG_ROOT is not set! Run 'Install Dependencies' first.%RESET%
    


    set "VCPKG_ROOT=%cd%\vcpkg"
    pause
    goto main
)

:: Create build directory
if not exist build mkdir build
cd build

:: Run CMake
echo %YELLOW%Running CMake...%RESET%
cmake -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ..
if %errorlevel% neq 0 (
    echo %RED%CMake configuration failed%RESET%
    cd ..
    pause
    goto main
)

:: Build project
echo %YELLOW%Building project...%RESET%
cmake --build . --config Debug
if %errorlevel% neq 0 (
    echo %RED%Build failed%RESET%
    cd ..
    pause
    goto main
)

:: Run executable
echo %YELLOW%Running application...%RESET%
if exist Debug\CPP_Project.exe (
    start Debug\CPP_Project.exe
) else (
    echo %RED%Executable not found: CPP_Project.exe%RESET%
)

cd ..
echo %GREEN%Build and run completed!%RESET%
pause
goto main