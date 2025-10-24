all:
	gcc -g rtsp_server.c -o rtsp_server `/usr/bin/pkg-config gstreamer-1.0 gstreamer-rtsp-1.0 gstreamer-rtsp-server-1.0 --libs --cflags` 

install:
	mkdir -p /usr/local/bin/
	cp rtsp_server /usr/local/bin/
	chmod +x /usr/local/bin/rtsp_server

clean:
	rm -f rtsp_server
