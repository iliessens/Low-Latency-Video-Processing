call ../setpath
ffmpeg -threads 4 -hwaccel nvdec -re ^
-max_delay 0 ^
-init_hw_device opencl=gpu:0.0 -filter_hw_device gpu ^
-i "C:\Users\Student\Downloads\Testdata\big_buck_bunny_1080p_h264.mov" ^
-i 1080_Test.tga ^
-fflags nobuffer -flags low_delay ^
-filter_complex "[0:v]format=argb, hwupload[bg]; [1:v]format=argb, hwupload[fg]; [bg][fg]overlay_opencl, hwdownload, format=argb" ^
-f sdl "Video"
pause