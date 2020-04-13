//
// Created by Phillip Mai on 22.03.20.
//

#ifndef MCS_DEV_STRUCTS_H
#define MCS_DEV_STRUCTS_H

#include <string>

const int maxcams = 10;
const int maxstreams = 15;

struct Stream {
    std::string streamprotocol;
    std::string port;
    std::string fps;
    std::string streamname;
    std::string width;
    std::string height;
    bool rectangles;
};

struct Camera {
    std::string protocol;
    std::string serverip;
    std::string camip;
    std::string authentication;
    std::string width;
    std::string height;
    std::string camfps;
    std::string at_serverip;
    std::string at_camid;
    int streamcounter;
    Stream clientstreams[maxstreams];
};

#endif //MCS_DEV_STRUCTS_H
