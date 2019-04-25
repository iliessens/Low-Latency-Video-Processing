REM use a previous recording using the streamdump script to simulate a second camera
REM Recorded video will freeze on last frame
REM SET PATH=%PATH%;"C:\Users\Student\Downloads\Tools\ffmpeg-20190225-f948082-win64-static\bin"
call ../setpath.bat
ffmpeg -threads 8 -hwaccel none -buffer_size 20M ^
-max_delay 0 -fflags nobuffer -flags low_delay ^
-i rtsp://192.168.100.150:8557/h264 ^
-fflags nobuffer -flags low_delay -max_delay 0 ^
-i ../../../streamdump.ts ^
-filter_complex "[0:v]crop=1/2*in_w:in_h [left];[1:v] crop=1/2*in_w:in_h [right]; [left][right] hstack,fps=50" ^
-pix_fmt bgra -f sdl "Video"
timeout 10
exit