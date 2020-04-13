//
// Created by Phillip Mai on 18.03.20.
//

#include "rootstream.h"


rootstream::rootstream() {}; // Default constructor

rootstream::rootstream(std::string rootstreampath, std::string camip, GstRTSPMediaFactory  *controllerfactory) {
    this->rootstreampath = rootstreampath;
    this->camip = camip;
    this->rootfactory = controllerfactory;
    this->createrootstream();

    // OpenCV start
    //this->initstream();
}

void rootstream::getrootlaunchstring() {
    gchar *launchsstart = "( rtspsrc location=";
    gchar *launchsettings = " latency=0 ! rtph264depay ! h264parse ! rtph264pay name=pay0 pt=96 )";

    int srclength = this->rootstreampath.length();
    gchar src_array[srclength + 1];
    strcpy(src_array, this->rootstreampath.c_str());

    this->rootlaunchstring += launchsstart;
    this->rootlaunchstring += src_array;
    this->rootlaunchstring += launchsettings;
    this->rootrtspsrc = rootlaunchstring.c_str();
}

void rootstream::createrootstream() {
    //this->rootrtspsrc = "( rtspsrc location=rtsp://192.168.0.62:554:554/MediaInput/h264/stream_1 ! rtph264depay ! h264parse ! rtph264pay name=pay0 pt=96 )";
    this->getrootlaunchstring();
    this->createrootRTSPserver();
}

void rootstream::createrootRTSPserver() {
    this->rootserver = gst_rtsp_server_new ();

    // this->rootfactory = rootfactory = gst_rtsp_media_factory_new ();
    //gst_rtsp_media_factory_set_launch (this->rootfactory, this->rootrtspsrc);

    //gst_rtsp_media_factory_set_shared (this->rootfactory, TRUE);

    /* get the default mount points from the rootserver */
    this->rootmounts = gst_rtsp_server_get_mount_points (this->rootserver);

    // Generate rootstream rootserver url
    const gchar *slash = "/";
    std::string url = slash + this->camip;
    const gchar *rootstreamurl = url.c_str();

    /* attach the video test signal to the "/test" URL */
    gst_rtsp_mount_points_add_factory (this->rootmounts, rootstreamurl , this->rootfactory);
    g_object_unref (this->rootmounts);

    /* make a mainloop for the default context */
    this->rootloop = g_main_loop_new (NULL, FALSE);

    /* attach the rootserver to the default maincontext */
    gst_rtsp_server_attach (this->rootserver, NULL);

    this->rootcreateRTSPserverstatus = true;
}

void rootstream::startrootstreamserver() {
    // Threading problem
    while (!this->rootcreateRTSPserverstatus) {}
    /* start serving gst-rtsp-rootserver */
    std::cout << "--STREAM READY--\nCamera (" + this->camip + ") -> Connection was successful." << std::endl;
    g_main_loop_run (this->rootloop);
}

bool rootstream::check_g_main_loop_is_running() {
    return g_main_loop_is_running(this->rootloop);
}

/*
void rootstream::initstream() {
    //this->capture = cv::VideoCapture(this->rootstreampath);
    //this->capture = cv::VideoCapture(0);

    if (!capture.isOpened()) {
        std::cout << "Error opening video stream." << std::endl;
        this->reconnectstream();
    } else {
        std::cout << "Cameracontroller: (" + this->camip + ") -> Connected to Rootstream. Start reading root frames." << std::endl;
        this->readrootframe();
    }
}

void rootstream::reconnectstream() {
    while (1) {
        this->capture.release();
        this->capture = cv::VideoCapture(this->rootstreampath);
        std::cout << "Try to reconnect Rootstream." << std::endl;

        if (capture.isOpened()){
            std::cout << "Reconnect video stream succsessful.\n" << std::endl;
            return;
        }
    }
}

cv::Mat rootstream::readrootframe() {
    cv::Mat frame;
    this->capture >> frame;

    return frame;
}
*/