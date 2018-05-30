@setlocal
@REM 20161016 - Fork of TG SF source
@set TMPPRJ=TgTake2
@REM set TMPPRJ=TerraGUI
@echo Build %TMPPRJ% project, in 64-bits
@set TMPLOG=bldlog-1.txt
@set BLDDIR=%CD%
@set TMPROOT=X:
@set VCVERS=14
@set SET_BAT=%ProgramFiles(x86)%\Microsoft Visual Studio %VCVERS%.0\VC\vcvarsall.bat
@if NOT EXIST "%SET_BAT%" goto NOBAT
@if NOT EXIST %TMPROOT%\nul goto NOROOT
@set TMPSRC=..
@if NOT EXIST %TMPSRC%\CMakeLists.txt goto NOCM
@set TMPINST=%TMPROOT%\3rdParty.x64

@if /I "%PROCESSOR_ARCHITECTURE%" EQU "AMD64" (
@REM This is OK
) ELSE (
 @if /I "%PROCESSOR_ARCHITECTURE%" EQU "x86_64" (
@REM THis is OK
 ) ELSE (
@echo ERROR: Appears 64-bit is NOT available... aborting...
@goto ISERR
 )
)
@if NOT EXIST %TMPINST%\nul goto NOINST

@echo Doing build output to %TMPLOG%
@echo Doing build output to %TMPLOG% > %TMPLOG%

@echo Doing: 'call "%SET_BAT%" %PROCESSOR_ARCHITECTURE%'
@echo Doing: 'call "%SET_BAT%" %PROCESSOR_ARCHITECTURE%' >> %TMPLOG%
@call "%SET_BAT%" %PROCESSOR_ARCHITECTURE% >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR0

@cd %BLDDIR%

:DNARCH

@REM ############################################
@REM NOTE: SPECIAL INSTALL LOCATION
@REM Adjust to suit your environment
@REM ##########################################
@REM set TMPINST=F:\Projects\software.x64
@set TMPOPTS=-DCMAKE_INSTALL_PREFIX=%TMPINST%
@set TMPOPTS=%TMPOPTS% -G "Visual Studio %VCVERS% Win64"
@set TMPOPTS=%TMPOPTS% -DBUILD_WITH_QT5:BOOL=ON

:RPT
@if "%~1x" == "x" goto GOTCMD
@set TMPOPTS=%TMPOPTS% %1
@shift
@goto RPT
:GOTCMD

@call chkmsvc %TMPPRJ%

@REM Setup Qt 5 64-bit D:\Qt5.6.1\5.6\msvc2015_64
@call setupQt5.6
@if exist tempsp.bat @del tempsp.bat
@call setupQt564 -N
@if NOT EXIST tempsp.bat goto NOQTSU
@call tempsp
@echo Setup Qt 5 64-bits %Qt5_DIR%
@echo Setup Qt 5 64-bits %Qt5_DIR% >> %TMPLOG%

@echo Begin %DATE% %TIME%, output to %TMPLOG%
@echo Begin %DATE% %TIME% >> %TMPLOG%

@echo Doing: 'cmake %TMPSRC% %TMPOPTS%'
@echo Doing: 'cmake %TMPSRC% %TMPOPTS%' >> %TMPLOG%
@cmake %TMPSRC% %TMPOPTS% >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR1

@echo Doing: 'cmake --build . --config debug'
@echo Doing: 'cmake --build . --config debug' >> %TMPLOG%
@cmake --build . --config debug >> %TMPLOG%
@if ERRORLEVEL 1 goto ERR2

@echo Doing: 'cmake --build . --config release'
@echo Doing: 'cmake --build . --config release' >> %TMPLOG%
@cmake --build . --config release >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR3
:DNREL

@echo Appears a successful build
@echo.
@echo No INSTALL configured at this time
@echo Fix PATH to point to Qt 5 64-bit DLLS, and maybe other 3rdParty.x64 DLLS used...
@echo In my case that is D:\Qt5.6.1\5.6\msvc2015_64, and X:\3rdParty.x64\bin
@echo.
@goto END

@echo Note install location %TMPINST%
@ask *** CONTINUE with install? *** Only y continues
@if ERRORLEVEL 2 goto NOASK
@if ERRORLEVEL 1 goto DOINST
@echo Skipping install to %TMPINST% at this time...
@echo.
@goto END
:NOASK
@echo ask not found in path...
@echo *** CONTINUE with install? *** Only ctrl+c aborts
@echo *** Note all other keys continue ***
@echo.
@pause

:DOINST
@REM cmake -P cmake_install.cmake
@echo Doing: 'cmake --build . --config debug --target INSTALL'
@echo Doing: 'cmake --build . --config debug --target INSTALL' >> %TMPLOG%
@cmake --build . --config debug --target INSTALL >> %TMPLOG% 2>&1

@echo Doing: 'cmake --build . --config release --target INSTALL'
@echo Doing: 'cmake --build . --config release --target INSTALL' >> %TMPLOG%
@cmake --build . --config release --target INSTALL >> %TMPLOG% 2>&1

@fa4 " -- " %TMPLOG%

@echo Done build and install of %TMPPRJ%...

@goto END

:NOBAT
@echo Can NOT locate MSVC setup batch "%SET_BAT%"! *** FIX ME ***
@goto ISERR

:NOQTSU
@echo Error: can NOT locate 'tempsp.bat' to setup path! *** FIX ME *** 
@goto ISERR

:NOROOT
@echo Can NOT locate %TMPROOT%! *** FIX ME ***
@goto ISERR

:NOCM
@echo Can NOT locate %TMPSRC%\CMakeLists.txt! *** FIX ME ***
@goto ISERR

:NOINST
@echo Can NOT locate directory %TMPINST%! *** FIX ME ***
@goto ISERR

:ERR0
@echo MSVC %VCVERS% setup error
@goto ISERR

:ERR1
@echo cmake config, generation error
@goto ISERR

:ERR2
@echo debug build error
@goto ISERR

:ERR3
@fa4 "mt.exe : general error c101008d:" %TMPLOG% >nul
@if ERRORLEVEL 1 goto ERR32
:ERR33
@echo release build error
@goto ISERR
:ERR32
@echo Stupid error... trying again...
@echo Doing: 'cmake --build . --config release'
@echo Doing: 'cmake --build . --config release' >> %TMPLOG%
@cmake --build . --config release >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR33
@goto DNREL

:ISERR
@endlocal
@exit /b 1

:END
@endlocal
@exit /b 0

@REM eof
