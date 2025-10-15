#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

#define DEFAULT_RTSP_PORT "8554"

static gboolean timeout(GstRTSPServer *server) {
  GstRTSPSessionPool *pool = gst_rtsp_server_get_session_pool(server);
  gst_rtsp_session_pool_cleanup(pool);
  g_object_unref(pool);
  return TRUE;
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

  mounts = gst_rtsp_server_get_mount_points(server);

  // Create a media factory for the input MP4 file
  factory = gst_rtsp_media_factory_new();
  /*gst_rtsp_media_factory_set_launch(factory,
      "( filesrc location=%s ! qtdemux name=demux "
      "demux.video_0 ! queue ! avdec_h264 ! x264enc tune=zerolatency ! rtph264pay name=pay0 pt=96 )");
  */
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

  g_print("Stream ready at rtsp://127.0.0.1:%s/test\n", DEFAULT_RTSP_PORT);
  g_main_loop_run(loop);

  return 0;
}

