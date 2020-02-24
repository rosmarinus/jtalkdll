@echo off
openfiles > NUL 2>&1
if not %ERRORLEVEL% == 0 (
    echo 管理者権限で実行してください
    pause
    goto :eof
)
cd /d %~dp0
set dll=JTalkCOMx64.dll
if not "%PROCESSOR_ARCHITECTURE%" == "x86" (
    if exist "%dll%" (
        echo 解除実行:%dll%
        rem C:\Windows\Microsoft.NET\Framework64\v4.0.30319\regasm.exe %dll% /unregister /nologo /tlb
        C:\Windows\Microsoft.NET\Framework64\v4.0.30319\regasm.exe %dll% /unregister /nologo
        echo 終了コード：%errorlevel%
    )
    if exist "%dll%" (
        echo 登録実行:%dll%
        rem C:\Windows\Microsoft.NET\Framework64\v4.0.30319\regasm.exe %dll% /codebase /nologo /tlb
        C:\Windows\Microsoft.NET\Framework64\v4.0.30319\regasm.exe %dll% /codebase /nologo
        echo 終了コード：%errorlevel%
    )
)
set dll=JTalkCOMx86.dll
if exist "%dll%" (
    echo 解除実行:%dll%
    rem C:\Windows\Microsoft.NET\Framework\v4.0.30319\regasm.exe %dll% /unregister /nologo /tlb
    C:\Windows\Microsoft.NET\Framework\v4.0.30319\regasm.exe %dll% /unregister /nologo
    echo 終了コード：%errorlevel%
)
if exist "%dll%" (
    echo 登録実行:%dll%
    rem C:\Windows\Microsoft.NET\Framework\v4.0.30319\regasm.exe %dll% /codebase /nologo /tlb
    C:\Windows\Microsoft.NET\Framework\v4.0.30319\regasm.exe %dll% /codebase /nologo
    echo 終了コード：%errorlevel%
)

set /p=キーを押すと終了します<NUL
pause >NUL
echo.

