call setpath.bat
REM command only gives gray screen
echo === This script is not functional ===
gst-launch-1.0 rtspsrc location=rtsp://192.168.100.150:8557/h264 ! rtph264depay ! h264parse ! decodebin ! autovideosink
timeout 10