//
// Created by Phillip Mai on 18.03.20.
//

#ifndef MCS_DEV_CAMERACONTROLLER_H
#define MCS_DEV_CAMERACONTROLLER_H

#include <string>
#include <thread>
#include <chrono>

#include <gst/gst.h>
#include <glib.h>
#include <gst/rtsp-server/rtsp-server.h>

#include "../data/structs.h"
#include "../rootstream/rootstream.h"
#include "../clientstream/clientstream.h"


class cameracontroller {
public:
    cameracontroller();
    cameracontroller(Camera camera);
    std::string getrootstreampath();
    void getrootframe();
    void sendrootframe();
    void initclientstreams();
    void startclientstreams();

    // GStreamer functions
    void getcontrollerlaunchstring();
    void createcontrollerserver();
    void createcontrollerRTSPserver();

private:
    Camera thisCamera;
    std::string rootstreampath;
    rootstream thisRootstream;
    //cv::Mat rootframe;
    bool initclientstreamsStatus;
    clientstream thisClientstreams[maxstreams];
    std::thread startstreamserverThread[maxstreams];
    std::thread startsendframesThread[maxstreams];
    std::thread startrootRTSPserverThread;

    // GStreamer variables
    std::string controllerlaunchstring;
    const gchar * controllersrc;
    GstRTSPMediaFactory  *controllerfactory;
};

#endif //MCS_DEV_CAMERACONTROLLER_H
