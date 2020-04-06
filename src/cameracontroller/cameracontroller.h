//
// Created by Phillip Mai on 18.03.20.
//

#ifndef MCS_DEV_CAMERACONTROLLER_H
#define MCS_DEV_CAMERACONTROLLER_H

#include <string>
#include <thread>

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

private:
    Camera thisCamera;
    std::string rootstreampath;
    rootstream thisRootstream;
    cv::Mat rootframe;
    clientstream thisClientstreams[maxstreams];
    std::thread startclientstreamsThread[maxstreams];
};

#endif //MCS_DEV_CAMERACONTROLLER_H
