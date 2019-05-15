call ../setpath
ffmpeg -i rtsp://192.168.100.150:8557/h264 -c:v copy -c:a copy streamdump.ts
timeout 10