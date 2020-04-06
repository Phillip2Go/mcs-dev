//
// Created by Phillip Mai on 17.03.20.
//

#ifndef CMAKELIBS_SERVER_H
#define CMAKELIBS_SERVER_H

#include <string>
#include <thread>

#include "../data/structs.h"
#include "../cameracontroller/cameracontroller.h"
#include "../../libs/tinyxml2/tinyxml2.h"


class server {
public:
    server();
    server(const char *xml_filename);
    void parsexmldata();
    void initcameras();
    void startcameras();

private:
    tinyxml2::XMLDocument xml_file;
    Camera cameras[maxcams];
    int camcounter;
    int clientstreamcounter;
    cameracontroller thisCameracontrollers[maxcams];
    std::thread getrootframeThread[maxcams];
    std::thread initclientstreamsThread[maxcams];
    std::thread sendrootframeThread[maxcams];
};

#endif //CMAKELIBS_SERVER_H
