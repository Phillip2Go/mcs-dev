//
// Created by Phillip Mai on 22.03.20.
//

#include <iostream>
#include "clientstream.h"


clientstream::clientstream() {}; // Default constructor

clientstream::clientstream(Stream clientstream, std::string rootstreampath, std::string camip) {
    this->thisClientstream = clientstream;
    this->rootstreampath = rootstreampath;
    this->camip = camip;
    this->createstream();
}

void clientstream::createstream() {
    if (this->thisClientstream.streamprotocol.compare("rtsp") == 0) {
        gchar *launchsstart = "( rtspsrc location=";
        gchar *launchsettings = " ! rtph264depay ! h264parse ! rtph264pay name=pay0 pt=96 )";

        int srclength = this->rootstreampath.length();
        gchar src_array[srclength + 1];
        strcpy(src_array, this->rootstreampath.c_str());

        this->launchstring += launchsstart;
        this->launchstring += src_array;
        this->launchstring += launchsettings;
        this->rtspsrc = launchstring.c_str();

        this->createRTSPstreams();

    } else if (this->thisClientstream.streamprotocol.compare("ndi") == 0) {
        this->ndisrc = rootstreampath;
    }
}

void clientstream::createRTSPstreams() {
    this->server = gst_rtsp_server_new ();
    //gst_rtsp_server_set_address(this->server, this->camip.c_str());
    gst_rtsp_server_set_service(this->server, reinterpret_cast<const gchar *>(this->thisClientstream.port.c_str()));

    this->factory = factory = gst_rtsp_media_factory_new ();
    gst_rtsp_media_factory_set_launch (this->factory, this->rtspsrc);

    gst_rtsp_media_factory_set_shared(this->factory, TRUE);

    /* get the default mount points from the server */
    this->mounts = gst_rtsp_server_get_mount_points (this->server);

    /* attach the video test signal to the "/test" URL */
    gst_rtsp_mount_points_add_factory (this->mounts, this->thisClientstream.streamname.c_str() , this->factory);
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
    } else if (this->thisClientstream.streamprotocol.compare("ndi") == 0) {

    }
}