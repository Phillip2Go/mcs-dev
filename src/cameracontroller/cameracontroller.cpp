//
// Created by Phillip Mai on 18.03.20.
//

#include "cameracontroller.h"


cameracontroller::cameracontroller() {}; // Default constructor

cameracontroller::cameracontroller(Camera camera) {
    this->thisCamera = camera;
    this->rootstreampath = this->getrootstreampath();
    this->createcontrollerserver();
}

std::string cameracontroller::getrootstreampath() {
    if (this->thisCamera.protocol.compare("rtsp") == 0) {
        return this->thisCamera.protocol + "://" + this->thisCamera.camip + ":554/MediaInput/" + "h264" + "/stream_1";
    } else if (this->thisCamera.protocol.compare("ndi") == 0) {
        return "NDI URL";
    } else {
        std::cout << " --ERROR--\nCamera (" + this->thisCamera.camip + ") -> Connection could not be established.\n" << std::endl;
        return "";
    }
}

void cameracontroller::getcontrollerlaunchstring() {
    gchar *launchsstart = "( rtspsrc location=";
    gchar *launchsettings = " latency=0 ! rtph264depay ! h264parse ! rtph264pay name=pay0 pt=96 )";

    int srclength = this->rootstreampath.length();
    gchar src_array[srclength + 1];
    strcpy(src_array, this->rootstreampath.c_str());

    this->controllerlaunchstring += launchsstart;
    this->controllerlaunchstring += src_array;
    this->controllerlaunchstring+= launchsettings;
    this->controllersrc = controllerlaunchstring.c_str();
}

void cameracontroller::createcontrollerserver() {
    this->getcontrollerlaunchstring();
    this->createcontrollerRTSPserver();
}

void cameracontroller::createcontrollerRTSPserver() {
    this->controllerfactory = gst_rtsp_media_factory_new();

    gst_rtsp_media_factory_set_launch (this->controllerfactory, this->controllersrc);
    gst_rtsp_media_factory_set_shared (this->controllerfactory, TRUE);
}

void cameracontroller::initclientstreams() {
    if (!this->rootstreampath.compare("") == 0) {
        //this->thisRootstream = rootstream(this->rootstreampath, this->thisCamera.camip, this->controllerfactory);
        //rootstream *rootstreamThread = &this->thisRootstream;
        //this->startrootRTSPserverThread = std::thread(&rootstream::startrootstreamserver, rootstreamThread);

        //bool check = this->thisRootstream.check_g_main_loop_is_running();
        //while (check == 0) {check = this->thisRootstream.check_g_main_loop_is_running();}

        //std::this_thread::sleep_for(std::chrono::milliseconds(5000));

        //std::cout << "Cameracontroller: (" + this->thisCamera.camip + ") -> Start init all clientstreams." << std::endl;

        for (int i = 0; i < this->thisCamera.streamcounter; ++i) {
            this->thisClientstreams[i] = clientstream(this->thisCamera.clientstreams[i], this->rootstreampath, this->thisCamera.serverip, this->controllerfactory);

            clientstream *clienstreamThread = &this->thisClientstreams[i];
            this->startstreamserverThread[i] = std::thread(&clientstream::startstreamserver, clienstreamThread);

            // OpenCV threads
            //this->startsendframesThread[i] = std::thread(&clientstream::startsendframes, clienstreamThread);
        }
        this->initclientstreamsStatus = true;

        //std::cout << "Cameracontroller: (" + this->thisCamera.camip + ") -> Init all clientstreams." << std::endl;
        this->startclientstreams();
    }
}

void cameracontroller::startclientstreams() {
    if (!this->rootstreampath.compare("") == 0) {
        //std::cout << "Cameracontroller: (" + this->thisCamera.camip + ") -> Start all clientstreams." << std::endl;
        for (int i = 0; i < this->thisCamera.streamcounter; i++) {
            if (i < 1) {
                //std::cout << "Camera (" + this->thisCamera.camip + ") -> rtsp://localhost:8554/" + this->thisCamera.camip << std::endl;
                //this->startrootRTSPserverThread.join();
            }
            this->startstreamserverThread[i].join();

            // OpenCV threads
            //this->startsendframesThread[i].join();
        }
    }
}

// Rootstream via OpenCV
void cameracontroller::getrootframe() {
    std::cout << "Cameracontroller: (" + this->thisCamera.camip + ") -> getrootframe()." << std::endl;
    while (1) {
        //this->rootframe = this->thisRootstream.readrootframe();
    }
}

void cameracontroller::sendrootframe() {
    // Waiting till all streams init. -> Threading problem.
    while (!this->initclientstreamsStatus) {
        std::cout << "\nCameracontroller: (" + this->thisCamera.camip + ") -> Init clientstreams not ready...\n" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    std::cout << "Cameracontroller: (" + this->thisCamera.camip + ") -> All cientstreams ready." << std::endl;
    std::cout << "Cameracontroller: (" + this->thisCamera.camip + ") -> sendrootframe()." << std::endl;
    for (int i = 0; i < this->thisCamera.streamcounter; i++) {
        std::string streamprotocol = this->thisClientstreams[i].getclientstreamprotocol();

        if (streamprotocol.compare("rtsp") == 0) {

            while (1) {
                //this->thisClientstreams[i].setrootframe(this->rootframe);
            }

        } else if (streamprotocol.compare("ndi") == 0) {

        }
    }
}
