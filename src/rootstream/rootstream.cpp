//
// Created by Phillip Mai on 18.03.20.
//

#include "rootstream.h"


rootstream::rootstream() {}; // Default constructor

rootstream::rootstream(std::string rootstreampath, std::string camip) {
    this->rootstreampath = rootstreampath;
    this->camip = camip;
    this->initstream();
}

void rootstream::initstream() {
    //this->capture = cv::VideoCapture(this->rootstreampath);
    this->capture = cv::VideoCapture(0);

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
