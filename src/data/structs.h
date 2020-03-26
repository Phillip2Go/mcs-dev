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
    std::string streamname;
    std::string streamresolution;
    bool rectangles;
};

struct Camera {
    std::string protocol;
    std::string camip;
    std::string authentication;
    std::string camresolution;
    std::string camfps;
    std::string at_serverip;
    std::string at_camid;
    int streamcounter;
    Stream clientstreams[maxstreams];
};

#endif //MCS_DEV_STRUCTS_H
