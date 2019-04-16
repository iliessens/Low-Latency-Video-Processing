call setpath
ffplay -fflags nobuffer -flags low_delay -framedrop -strict experimental udp://192.168.100.100@224.1.1.1:3000 -sync ext
pause