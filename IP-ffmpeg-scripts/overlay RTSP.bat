call setpath
ffmpeg -hwaccel nvdec -threads 2 ^
-i rtsp://192.168.100.150:8557/h264 -i 1080_Test_small.tga ^
-fflags nobuffer -flags low_delay -filter_complex "[0:v][1:v]overlay=0:0:enable=1" ^
-pix_fmt bgra -f opengl "Video"
pause