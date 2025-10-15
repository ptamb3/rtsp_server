all:
	gcc -g rtsp_server.c -o rtsp_server `/usr/bin/pkg-config gstreamer-1.0 gstreamer-rtsp-1.0 gstreamer-rtsp-server-1.0 --libs --cflags` 
