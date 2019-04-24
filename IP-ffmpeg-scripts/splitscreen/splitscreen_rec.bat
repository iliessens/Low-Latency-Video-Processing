REM use a previous recording using the streamdump script to simulate a second camera
REM Recorded video will freeze on last frame
call ../setpath
ffmpeg -hwaccel nvdec -threads 4 ^
-max_delay 0 -fflags nobuffer -flags low_delay ^
-i rtsp://192.168.100.150:8557/h264 ^
-fflags nobuffer -flags low_delay -i ../../../streamdump.ts ^
-filter_complex "[0:v]crop=1/2*in_w:in_h [right];[1:v] crop=1/2*in_w:in_h [left]; [left][right] hstack" ^
-pix_fmt bgra -f sdl "Video"
timeout 10