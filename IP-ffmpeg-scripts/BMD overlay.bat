call setpath
ffmpeg ^
-flags low_delay -fflags nobuffer ^
-format_code Hp50 -raw_format argb -f decklink -i "Intensity Pro 4K" ^
-i 1080_Test_small.tga ^
-filter_complex "[0:v][1:v]overlay=0:0:enable=1" ^
-flags low_delay -pix_fmt uyvy422 ^
-f decklink -preroll 0.04 "Intensity Pro 4K"
pause