ffmpeg -framerate 25 -i debug/images/%d.png -c:v libx264 -vf "pad=ceil(iw/2)*2:ceil(ih/2)*2" -r 25 -pix_fmt yuv420p debug/output.mp4