//
// Created by Phillip Mai on 22.03.20.
//

#ifndef MCS_DEV_CLIENTSTREAM_H
#define MCS_DEV_CLIENTSTREAM_H

#include <thread>
#include <chrono>

#include <gst/gst.h>
#include <glib.h>
#include <gst/rtsp-server/rtsp-server.h>
/*
#include <opencv2/opencv.hpp>
*/
#include "../data/structs.h"


class clientstream {
public:
    clientstream();
    clientstream(Stream clientstream, std::string rootstreampath, std::string serverip, GstRTSPMediaFactory  *controllerfactory);
    void getlaunchstring();
    void createstream();
    void createRTSPserver();
    void startstreamserver();
    void startsendframes();

    std::string getclientstreamprotocol();
    /*
    void setrootframe(cv::Mat frame);
     */

private:
    Stream thisClientstream;
    std::string rootstreampath;
    std::string serverip;
    std::string camip;
    std::string streamprotocol;
    std::string launchstring;
    const gchar *rtspsrc;
    std::string ndisrc;
    bool createRTSPserverstatus;

    // GStreamer variables
    GstRTSPServer  *server;
    GMainLoop *loop;
    GstRTSPMediaFactory  *factory;
    GstRTSPMountPoints *mounts;

    /*
    // OpenCV variables
    cv::Mat rootframe;
    cv::VideoWriter writer;
     */
};

#endif //MCS_DEV_CLIENTSTREAM_H
