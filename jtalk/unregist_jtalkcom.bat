@echo off
openfiles > NUL 2>&1
if not %ERRORLEVEL% == 0 (
echo 管理者権限で実行してください
pause
goto :eof
)
cd /d %~dp0
if "%PROCESSOR_ARCHITECTURE%" == "x86" (
C:\Windows\Microsoft.NET\Framework\v4.0.30319\regasm.exe JTalkCOMx86.dll /unregister
) else (
C:\Windows\Microsoft.NET\Framework64\v4.0.30319\regasm.exe JTalkCOMx64.dll /unregister
)
set /p=キーを押すと終了します<NUL
pause >NUL
echo.
