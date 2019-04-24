call ../setpath
ffmpeg -hwaccel nvdec -threads 4 ^
-max_delay 0 ^
-init_hw_device opencl=gpu:0.0 -filter_hw_device gpu ^
-i rtsp://192.168.100.150:8557/h264 ^
-fflags nobuffer -flags low_delay ^
-filter_complex "[0:v]format=bgra, hwupload, avgblur_opencl=3, hwdownload, format=bgra" ^
-pix_fmt bgra -f opengl "Video"
pause