//
// Created by Phillip Mai on 18.03.20.
//

#ifndef MCS_DEV_ROOTSTREAM_H
#define MCS_DEV_ROOTSTREAM_H

#include <string>

#include <opencv2/opencv.hpp>


class rootstream {
public:
    rootstream();
    rootstream(std::string rootstreampath, std::string camip);
    void initstream();
    void reconnectstream();
    cv::Mat readrootframe();

private:
    std::string rootstreampath;
    cv::VideoCapture capture;
    std::string camip;
};

#endif //MCS_DEV_ROOTSTREAM_H
