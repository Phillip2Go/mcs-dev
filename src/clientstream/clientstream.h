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
    clientstream(Stream clientstream);
    void createRTSPstreams();
    void startstream();

private:
    Stream thisClientstream;
    // Rtsp variables
    GstRTSPServer  *server;
    GMainLoop *loop;
    GstRTSPMediaFactory  *factory;
    GstRTSPMountPoints *mounts;
};

#endif //MCS_DEV_CLIENTSTREAM_H
