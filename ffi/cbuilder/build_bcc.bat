set RADSTUDIO_PATH=C:\Program Files (x86)\Embarcadero\Studio\19.0
set BCC=bcc32c.exe
rem set BCC=bcc32.exe
set DLL=jtalk
if not exist "%DLL%.dll" goto :eof
"%RADSTUDIO_PATH%\bin\implib.exe" %DLL%bcc.lib %DLL%.dll
"%RADSTUDIO_PATH%\bin\%BCC%" jtd_bcc.cpp jtalkbcc.cpp %DLL%bcc.lib
