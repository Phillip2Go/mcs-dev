//
// Created by Phillip Mai on 18.03.20.
//

#ifndef MCS_DEV_ROOTSTREAM_H
#define MCS_DEV_ROOTSTREAM_H

#include <iostream>
#include <string>

#include <gst/gst.h>
#include <glib.h>
#include <gst/rtsp-server/rtsp-server.h>
/*
#include <opencv2/opencv.hpp>
*/

class rootstream {
public:
    rootstream();
    rootstream(std::string rootstreampath, std::string camip, GstRTSPMediaFactory  *controllerfactory);

    // GStreamer functions
    void getrootlaunchstring();
    void createrootstream();
    void createrootRTSPserver();
    void startrootstreamserver();
    bool check_g_main_loop_is_running();

    /*
    // OpenCV functions
    void initstream();
    void reconnectstream();
    cv::Mat readrootframe();
    */
private:
    std::string rootstreampath;
    std::string rootlaunchstring;
    const gchar *rootrtspsrc;
    std::string camip;

    // GStreamer variables
    GstRTSPServer  *rootserver;
    GMainLoop *rootloop;
    GstRTSPMediaFactory  *rootfactory;
    GstRTSPMountPoints *rootmounts;
    bool rootcreateRTSPserverstatus;

    /*
    // OpenCV variables
    cv::VideoCapture capture;
    */
};

#endif //MCS_DEV_ROOTSTREAM_H
