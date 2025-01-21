@echo off
REM

gcc -o jeu.exe src\\*.c

IF %ERRORLEVEL% EQU 0 (
    echo Compilation : Succès 
    jeu.exe levels\\meatshield.txt

) ELSE (
    echo Compilation : Bon ça marche pas
)

pause
