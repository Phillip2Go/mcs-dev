//
// Created by Phillip Mai on 17.03.20.
//

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

#include "server.h"

using namespace tinyxml2;

static std::string getElementText(tinyxml2::XMLElement *_element);
static std::string getElementText(tinyxml2::XMLElement *_element) {
    std::string value;
    if (_element != NULL) {
        value = _element->GetText();
    }

    return value;
}


server::server() {}; // Default constructor

server::server(const char *xml_filename) {
    std::cout << "MCS 2.0 by Phillip Mai & Dorian Paeth.\n" << std::endl;
    std::cout << "Start server initialization." << std::endl;

    this->xml_file.LoadFile(xml_filename);
    if (xml_file.ErrorID() == 0) this->parsexmldata();

    this->initcameras();
}

void server::parsexmldata() {
    this->camcounter = 0;
    tinyxml2::XMLElement *pRoot = this->xml_file.FirstChildElement("configXML")->FirstChildElement("camera");

    while (pRoot) {
        this->clientstreamcounter = 0;
        if (this->camcounter > 9) {
            std::cout << "Maximum number of cameras reached. break;\n" << std::endl;
            break;
        }

        Camera *thisCam = new Camera();
        if (pRoot->FirstChildElement("protocol")->GetText() != nullptr) {
            thisCam->protocol = getElementText(pRoot->FirstChildElement("protocol"));
        } if (pRoot->FirstChildElement("serverip")->GetText() != nullptr) {
            thisCam->serverip = getElementText(pRoot->FirstChildElement("serverip"));
        } if (pRoot->FirstChildElement("camip")->GetText() != nullptr) {
            thisCam->camip = getElementText(pRoot->FirstChildElement("camip"));
        } if (pRoot->FirstChildElement("authentication")->GetText() != nullptr) {
            thisCam->authentication = getElementText(pRoot->FirstChildElement("authentication"));
        } if (pRoot->FirstChildElement("width")->GetText() != nullptr) {
            thisCam->width = getElementText(pRoot->FirstChildElement("width"));
        } if (pRoot->FirstChildElement("height")->GetText() != nullptr) {
            thisCam->height = getElementText(pRoot->FirstChildElement("height"));
        } if (pRoot->FirstChildElement("camfps")->GetText() != nullptr) {
            thisCam->camfps = getElementText(pRoot->FirstChildElement("camfps"));
        } if (pRoot->FirstChildElement("at_serverip")->GetText() != nullptr) {
            thisCam->at_serverip = getElementText(pRoot->FirstChildElement("at_serverip"));
        } if (pRoot->FirstChildElement("at_camid")->GetText() != nullptr) {
            thisCam->at_camid = getElementText(pRoot->FirstChildElement("at_camid"));
        }

        tinyxml2::XMLElement *streamRoot = pRoot->FirstChildElement("streams")->FirstChildElement("stream");

        while (streamRoot) {
            if (this->clientstreamcounter > 14) {
                std::cout << "Maximum number of clientstreams reached. break;\n" << std::endl;
                break;
            }

            Stream *thisStream = new Stream();
            if (streamRoot->FirstChildElement("streamprotocol")->GetText() != nullptr) {
                thisStream->streamprotocol = getElementText(streamRoot->FirstChildElement("streamprotocol"));
            } if (streamRoot->FirstChildElement("port")->GetText() != nullptr) {
                thisStream->port = getElementText(streamRoot->FirstChildElement("port"));
            } if (streamRoot->FirstChildElement("fps")->GetText() != nullptr) {
                thisStream->fps = getElementText(streamRoot->FirstChildElement("fps"));
            } if (streamRoot->FirstChildElement("streamname")->GetText() != nullptr) {
                thisStream->streamname = getElementText(streamRoot->FirstChildElement("streamname"));
            } if (streamRoot->FirstChildElement("width")->GetText() != nullptr) {
                thisStream->width = getElementText(streamRoot->FirstChildElement("width"));
            } if (streamRoot->FirstChildElement("height")->GetText() != nullptr) {
                thisStream->height = getElementText(streamRoot->FirstChildElement("height"));
            } if (streamRoot->FirstChildElement("rectangles")->GetText() != nullptr) {
                std::string rectanglescheck = getElementText(streamRoot->FirstChildElement("rectangles"));
                if (rectanglescheck.compare("true") == 0) {
                    thisStream->rectangles = true;
                } else {
                    thisStream->rectangles = false;
                }
            }

            thisCam->clientstreams[clientstreamcounter] = *thisStream;
            this->clientstreamcounter += 1;

            streamRoot = streamRoot->NextSiblingElement("stream");
        }

        thisCam->streamcounter = this->clientstreamcounter;
        this->cameras[this->camcounter] = *thisCam;
        this->camcounter += 1;

        pRoot = pRoot->NextSiblingElement("camera");
    }

    std::cout << "Finished XML to camera parse.\n" << std::endl;
}

void server::initcameras() {
    for (int i = 0; i < this->camcounter; i++) {
        this->thisCameracontrollers[i] = cameracontroller(this->cameras[i]);

        cameracontroller *controllerThread = &this->thisCameracontrollers[i];

        // Rootstream via OpenCV
        //this->getrootframeThread[i] = std::thread(&cameracontroller::getrootframe, controllerThread);
        //this->sendrootframeThread[i] = std::thread(&cameracontroller::sendrootframe, controllerThread);

        this->initclientstreamsThread[i] = std::thread(&cameracontroller::initclientstreams, controllerThread);
    }

    this->startcameras();
}

void server::startcameras() {
    for (int i = 0; i < this->camcounter; i++) {

        // Rootstream via OpenCV
        //this->getrootframeThread[i].join();
        //this->sendrootframeThread[i].join();

        this->initclientstreamsThread[i].join();
    }
}
