REM use a previous recording using the streamdump script to simulate a second camera
REM Recorded video will freeze on last frame
call ../setpath
ffmpeg -threads 4 ^
-max_delay 0 -fflags nobuffer -flags low_delay ^
-i rtsp://192.168.100.150:8557/h264 ^
-i ../../../streamdump.ts ^
-filter_complex "[1:v]scale=iw/4:ih/4, drawbox=color=red@0.5, pad=w=iw*4:h=ih*4:x=ow-iw:y=oh-ih:color=0x00000000[pip]; [0:v][pip]overlay=0:0:enable=1" ^
-fflags nobuffer -flags low_delay -pix_fmt bgra -f sdl "Video"
timeout 10