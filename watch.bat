@echo off
set file_name=%1
set prev_checksum=

:loop
for /r "examples" %%f in (*) do call :check_file "%%f"
for /r "src" %%f in (*) do call :check_file "%%f"
goto loop

:check_file
set current_checksum=%~tf%~zf
if not "%current_checksum%"=="%prev_checksum%" (
  set prev_checksum=%current_checksum%
  call run.bat %file_name%
)
goto :eoffile_name=$1
ls examples/* src/* | entr sh -c "./run.sh ${file_name}" 
