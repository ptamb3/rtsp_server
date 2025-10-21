#include <stdio.h>
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>

#define DEFAULT_RTSP_PORT "8554"

static gboolean timeout(GstRTSPServer *server) {
  GstRTSPSessionPool *pool = gst_rtsp_server_get_session_pool(server);
  gst_rtsp_session_pool_cleanup(pool);
  g_object_unref(pool);
  return TRUE;
}

void print_local_ip() {
  struct ifaddrs *ifaddr, *ifa;
  char ip[INET_ADDRSTRLEN];

  if (getifaddrs(&ifaddr) == -1) {
    perror("getifaddrs");
    return;
  }

  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET)
      continue;

    struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
    inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN);

    if (strcmp(ifa->ifa_name, "lo") != 0) {
      printf("Stream ready at rtsp://%s:%s/test\n", ip, DEFAULT_RTSP_PORT);
    }
  }

  freeifaddrs(ifaddr);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    g_printerr("Usage: %s <video_file.mp4>\n", argv[0]);
    return -1;
  }

  GMainLoop *loop;
  GstRTSPServer *server;
  GstRTSPMountPoints *mounts;
  GstRTSPMediaFactory *factory;
  gchar *filepath = argv[1];

  gst_init(&argc, &argv);
  loop = g_main_loop_new(NULL, FALSE);
  server = gst_rtsp_server_new();
  g_object_set(server, "service", DEFAULT_RTSP_PORT, NULL);
  gst_rtsp_server_set_address(server, "0.0.0.0");

  mounts = gst_rtsp_server_get_mount_points(server);
  factory = gst_rtsp_media_factory_new();

  gchar *pipeline_str = g_strdup_printf(
    "( filesrc location=%s ! qtdemux name=demux "
    "demux.video_0 ! queue ! avdec_h264 ! x264enc tune=zerolatency ! rtph264pay name=pay0 pt=96 )",
    filepath);
  gst_rtsp_media_factory_set_launch(factory, pipeline_str);
  g_free(pipeline_str);

  gst_rtsp_media_factory_set_shared(factory, TRUE);
  gst_rtsp_mount_points_add_factory(mounts, "/test", factory);
  g_object_unref(mounts);

  gst_rtsp_server_attach(server, NULL);
  g_timeout_add_seconds(2, (GSourceFunc)timeout, server);

  print_local_ip();

  g_main_loop_run(loop);
  return 0;
}

