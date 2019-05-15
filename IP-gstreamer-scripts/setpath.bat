REM add my gstreamer install folder to path temporarily

REM clear screen
echo off
cls

SET gpath=C:\gstreamer\1.0\x86_64\bin
echo Gstreamer path is %gpath%

REM check path first
IF NOT EXIST %gpath% (
echo Path not found!
exit /B
)

SET PATH=%PATH%;%gpath%
echo PATH set