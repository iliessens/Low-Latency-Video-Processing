call setpath
ffplay -fflags nobuffer -flags low_delay -framedrop -strict experimental rtsp://192.168.100.150:8555/mjpeg -sync ext
REM rtsp://192.168.100.150:8555/mjpeg
pause