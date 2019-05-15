REM add my ffmpeg install folder to path temporarily

REM clear screen
echo off
cls

SET ffpath=C:\Users\Student\Documents\ff-shared
echo FFmpeg path is %ffpath%

REM check path first
IF NOT EXIST %ffpath% (
echo Path not found!
exit /B
)

SET PATH=%PATH%;%ffpath%
echo PATH set