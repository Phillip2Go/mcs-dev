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
    for (int i = 0; i < this->thisCamera.streamcounter; ++i) {
        this->thisClientstreams[i] = clientstream(this->thisCamera.clientstreams[i], this->rootstreampath, this->thisCamera.camip);

        clientstream *clienstreamThread = &this->thisClientstreams[i];
        this->startclientstreamsThread[i] = std::thread(&clientstream::startstream, clienstreamThread);
    }

    std::cout << "Cameracontroller: (" + this->thisCamera.camip + ") -> Init all clientstreams." << std::endl;
    this->startclientstreams();
}

void cameracontroller::startclientstreams() {
    std::cout << "Cameracontroller: (" + this->thisCamera.camip + ") -> Start all clientstreams." << std::endl;
    for (int i = 0; i < this->thisCamera.streamcounter; i++) {
        this->startclientstreamsThread[i].join();
    }
}
