//
// Created by Phillip Mai on 18.03.20.
//

#include "cameracontroller.h"


cameracontroller::cameracontroller() {}; // Default constructor

cameracontroller::cameracontroller(Camera camera) {
    this->thisCamera = camera;
    this->rootstreampath = this->getrootstreampath();
    this->thisRootstream = rootstream(this->rootstreampath, this->thisCamera.camip);
}

std::string cameracontroller::getrootstreampath() {
    if (this->thisCamera.protocol == "rtsp") {
        return "rtsp://" + this->thisCamera.protocol + ":554/MediaInput/" + "RTSP_CODEC" + "/stream_1";
    } else if (this->thisCamera.protocol == "ndi") {
        return "NDI URL";
    } else {
        std::cout << "Protocol incorrect. Rootstreamconnection fail.\n" << std::endl;
        return "";
    }
}

void cameracontroller::getrootframe() {
    std::cout << "(" + this->thisCamera.camip + ") -> getrootframe()" << std::endl;
    while (1) {
        this->rootframe = this->thisRootstream.readrootframe();
    }
}

void cameracontroller::initclientstreams() {
    for (int i = 0; i < this->thisCamera.streamcounter; ++i) {
        this->thisClientstreams[i] = clientstream(this->thisCamera.clientstreams[i]);

        clientstream *clienstreamThread = &this->thisClientstreams[i];
        this->startclientstreamsThread[i] = std::thread(&clientstream::startstream, clienstreamThread);
    }

    std::cout << "Cameracontroller: (" + this->thisCamera.camip + ") -> Init all clientstreams." << std::endl;
    this->startclientstreams();
}

void cameracontroller::startclientstreams() {
    std::cout << "Cameracontroller: (" + this->thisCamera.camip + ") -> Start all clientstreams.\n" << std::endl;
    for (int i = 0; i < this->thisCamera.streamcounter; i++) {
        this->startclientstreamsThread[i].join();
    }
}
