//
// Created by Phillip Mai on 18.03.20.
//

#include "cameracontroller.h"


cameracontroller::cameracontroller() {}; // Default constructor

cameracontroller::cameracontroller(Camera camera) {
    this->thisCamera = camera;
    this->rootstreampath = this->getrootstreampath();

    // Rootstream via OpenCV
    this->thisRootstream = rootstream(this->rootstreampath, this->thisCamera.camip);
}

std::string cameracontroller::getrootstreampath() {
    if (this->thisCamera.protocol.compare("rtsp") == 0) {
        return this->thisCamera.protocol + "://" + this->thisCamera.camip + ":554/MediaInput/" + "h264" + "/stream_1";
    } else if (this->thisCamera.protocol.compare("ndi") == 0) {
        return "NDI URL";
    } else {
        std::cout << "Protocol incorrect. Rootstreamconnection fail.\n" << std::endl;
        return "";
    }
}

// Rootstream via OpenCV
void cameracontroller::getrootframe() {
    std::cout << "Cameracontroller: (" + this->thisCamera.camip + ") -> getrootframe()." << std::endl;
    while (1) {
        this->rootframe = this->thisRootstream.readrootframe();
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
                this->thisClientstreams[i].setrootframe(this->rootframe);
            }

        } else if (streamprotocol.compare("ndi") == 0) {

        }
    }
}

void cameracontroller::initclientstreams() {
    rootstream *rootstreamThread = &this->thisRootstream;
    this->startrootRTSPserverThread = std::thread(&rootstream::startrootstreamserver, rootstreamThread);

    bool check = this->thisRootstream.check_g_main_loop_is_running();
    while (check == 0) {check = this->thisRootstream.check_g_main_loop_is_running();}

    std::cout << "Cameracontroller: (" + this->thisCamera.camip + ") -> Start init all clientstreams." << std::endl;

    for (int i = 0; i < this->thisCamera.streamcounter; ++i) {
        this->thisClientstreams[i] = clientstream(this->thisCamera.clientstreams[i], this->rootstreampath, this->thisCamera.camip);

        clientstream *clienstreamThread = &this->thisClientstreams[i];
        //this->startstreamserverThread[i] = std::thread(&clientstream::startstreamserver, clienstreamThread);

        // OpenCV threads
        //this->startsendframesThread[i] = std::thread(&clientstream::startsendframes, clienstreamThread);
    }
    this->initclientstreamsStatus = true;

    std::cout << "Cameracontroller: (" + this->thisCamera.camip + ") -> Init all clientstreams." << std::endl;
    this->startclientstreams();
}

void cameracontroller::startclientstreams() {
    std::cout << "Cameracontroller: (" + this->thisCamera.camip + ") -> Start all clientstreams." << std::endl;
    for (int i = 0; i < this->thisCamera.streamcounter; i++) {
        if (i < 1) this->startrootRTSPserverThread.join();
        //this->startstreamserverThread[i].join();

        // OpenCV threads
        //this->startsendframesThread[i].join();
    }
}
