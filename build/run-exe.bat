@setlocal
@set TMPEXE=Release\TgTake2.exe
@if NOT EXIST %TMPEXE% goto NOEXE
@set TMP3RD=X:\3rdParty.x64

@set TMPQT=D:\Qt5.6.1\5.6\msvc2015_64\bin
@if NOT EXIST %TMPQT%\nul goto NOQT

@set PATH=%TMPQT%;%PATH%

%TMPEXE% %*

@goto END

:NOEXE
@echo Can NOT locate %TMPEXE%! *** FIX ME ***
@goto END

:NOQT
@echo Can NOT locate %TMPQT%! *** FIX ME **
@goto END


:END
