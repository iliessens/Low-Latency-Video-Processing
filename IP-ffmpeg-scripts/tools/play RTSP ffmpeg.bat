call ../setpath
ffmpeg -hwaccel none -threads 8 ^
-max_delay 0 ^
-flags low_delay -fflags nobuffer ^
-i rtsp://192.168.100.150:8557/h264 ^
-window_fullscreen 1 ^
-pix_fmt bgra -f sdl "Video"
