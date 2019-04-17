REM way too much latency
REM low_latency option causes glitches on screen
call setpath
ffmpeg -hwaccel nvdec -threads 2 ^
-i rtsp://192.168.100.150:8557/h264 ^
-f decklink -pix_fmt uyvy422 "Intensity Pro 4K"
pause