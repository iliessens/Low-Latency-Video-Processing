call ../setpath
ffmpeg -hwaccel none -threads 8 -buffer_size 20M ^
-max_delay 0 -fflags nobuffer -flags low_delay ^
-i rtsp://192.168.100.150:8557/h264 -i beach.jpg ^
-filter_complex "[0:v] crop=1/2*in_w:in_h [right];[1:v] crop=1/2*in_w:in_h [left]; [left][right] hstack, setpts=N" ^
-pix_fmt bgra -f sdl "Video"
timeout 10