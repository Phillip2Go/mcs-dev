//
// Created by Phillip Mai on 22.03.20.
//

#ifndef MCS_DEV_CLIENTSTREAM_H
#define MCS_DEV_CLIENTSTREAM_H

#include <gst/gst.h>
#include <glib.h>
#include <gst/rtsp-server/rtsp-server.h>

#include "../data/structs.h"


class clientstream {
public:
    clientstream();
    clientstream(Stream clientstream, std::string rootstreampath, std::string camip);
    void createstream();
    void createRTSPstreams();
    void startstream();

private:
    Stream thisClientstream;
    std::string rootstreampath;
    std::string camip;
    std::string launchstring;
    const gchar *rtspsrc;
    std::string ndisrc;

    // GStreamer variables
    GstRTSPServer  *server;
    GMainLoop *loop;
    GstRTSPMediaFactory  *factory;
    GstRTSPMountPoints *mounts;
};

#endif //MCS_DEV_CLIENTSTREAM_H
