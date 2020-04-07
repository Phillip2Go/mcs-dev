//
// Created by Phillip Mai on 18.03.20.
//

#include "rootstream.h"


rootstream::rootstream() {}; // Default constructor

rootstream::rootstream(std::string rootstreampath, std::string camip) {
    this->rootstreampath = rootstreampath;
    this->camip = camip;

    this->createrootstream();

    // OpenCV start
    //this->initstream();
}

void rootstream::getrootlaunchstring() {
    gchar *launchsstart = "( rtspsrc location=";
    gchar *launchsettings = " ! rtph264depay ! h264parse ! rtph264pay name=pay0 pt=96 )";

    int srclength = this->rootstreampath.length();
    gchar src_array[srclength + 1];
    strcpy(src_array, this->rootstreampath.c_str());

    this->rootlaunchstring += launchsstart;
    this->rootlaunchstring += src_array;
    this->rootlaunchstring += launchsettings;
    this->rootrtspsrc = rootlaunchstring.c_str();
}

void rootstream::createrootstream() {
    this->getrootlaunchstring();
    this->createrootRTSPserver();
}

void rootstream::createrootRTSPserver() {
    this->server = gst_rtsp_server_new ();

    this->factory = factory = gst_rtsp_media_factory_new ();
    gst_rtsp_media_factory_set_launch (this->factory, this->rootrtspsrc);

    gst_rtsp_media_factory_set_shared(this->factory, TRUE);

    /* get the default mount points from the server */
    this->mounts = gst_rtsp_server_get_mount_points (this->server);

    // Generate rootstream server url
    const gchar *slash = "/";
    std::string url = slash + this->camip;
    const gchar *rootstreamurl = url.c_str();

    /* attach the video test signal to the "/test" URL */
    gst_rtsp_mount_points_add_factory (this->mounts, rootstreamurl , this->factory);
    g_object_unref (this->mounts);

    /* make a mainloop for the default context */
    this->loop = g_main_loop_new (NULL, FALSE);

    /* attach the server to the default maincontext */
    gst_rtsp_server_attach (this->server, NULL);

    this->createRTSPserverstatus = true;
}

void rootstream::startrootstreamserver() {
    // Thread error
    while (!this->createRTSPserverstatus) {}
    /* start serving gst-rtsp-server */
    g_main_loop_run (this->loop);
}

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
