//
// Created by Phillip Mai on 22.03.20.
//

#include "clientstream.h"


clientstream::clientstream() {}; // Default constructor

clientstream::clientstream(Stream clientstream) {
    this->thisClientstream = clientstream;

    this->createRTSPstreams();
}

void clientstream::createRTSPstreams() {
    this->server = gst_rtsp_server_new ();
    this->factory = factory = gst_rtsp_media_factory_new ();
    gst_rtsp_media_factory_set_launch (this->factory,
                                       "( videotestsrc ! x264enc ! rtph264pay pt=96 name=pay0 )");
    gst_rtsp_media_factory_set_shared(this->factory, TRUE);

    /* get the default mount points from the server */
    this->mounts = gst_rtsp_server_get_mount_points (this->server);

    /* attach the video test signal to the "/test" URL */
    gst_rtsp_mount_points_add_factory (this->mounts, "/test", this->factory);
    g_object_unref (this->mounts);

    /* make a mainloop for the default context */
    this->loop = g_main_loop_new (NULL, FALSE);

    /* attach the server to the default maincontext */
    gst_rtsp_server_attach (this->server, NULL);
}

void clientstream::startstream() {
    if (this->thisClientstream.streamprotocol.compare("rtsp") == 0) {
        /* start serving */
        g_main_loop_run (this->loop);
    }
}
