call setpath.bat
gst-launch-1.0 -v decklinkvideosrc device-number=0 mode=1080p50 ! autovideosink
timeout 10