@echo off
if "%1" == "" (
    echo "Write windeployqt.exe path as argument for script. This file often situated QTDIR\QTVER\COMPILER\bin."
        EXIT /B 1
)

%1\windeployqt.exe qnetsim.exe
