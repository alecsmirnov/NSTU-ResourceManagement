set LANG_STANDARD=/std:c++17
set DEFINES=/D "LIB_EXPORTS" /D "_CRT_SECURE_NO_WARNINGS"
set COMPILE_OPT=%LANG_STANDARD% %DEFINES% /nologo /MDd /EHsc
set PROJECT_LIB=kernel32.lib user32.lib gdi32.lib advapi32.lib

if exist *.exe del *.exe
if exist *.dll del *.dll

cl %COMPILE_OPT% main.cpp %PROJECT_LIB% /link /out:Info.exe
cl %COMPILE_OPT% /LD Lib.cpp Cache.cpp CpuId.cpp %PROJECT_LIB% /link /out:Lib.dll

del *.obj *.lib *.exp