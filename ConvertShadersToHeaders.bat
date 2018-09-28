@echo
SETLOCAL ENABLEDELAYEDEXPANSION 
SET "sourcedir=data\shaders"
SET "destdir=src\shaders"

FOR /f "delims=" %%a IN ('xcopy /y /L /s "%sourcedir%\*"') DO (
 SET "destfile=%%a"
 SET "destfile=!destfile:*%sourcedir%=%destdir%!"
 IF /i "%%a" neq "!destfile!" (
  FOR %%m IN ("!destfile!") DO (
   MD "%%~dpm" 2>nul
   "ShaderToHeader.exe" "%%a" "%%~dpm%%~na.h"
  )
 )
)
GOTO :EOF