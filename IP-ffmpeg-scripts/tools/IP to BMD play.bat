REM way too much latency
call ../setpath
ffmpeg -hwaccel nvdec -threads 2 ^
-max_delay 0 ^
-flags low_delay -fflags nobuffer ^
-i rtsp://192.168.100.150:8557/h264 ^
-flags low_delay ^
-f decklink -pix_fmt uyvy422 "Intensity Pro 4K"
pause