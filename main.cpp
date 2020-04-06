#include <iostream>
#include <gst/gst.h>
#include "src/server/server.h"
#include "includes/ndi/Processing.NDI.Lib.h"

int main(int argc, char *argv[]) {
    std::cout << "Welcome to:" << std::endl;
    gst_init (&argc, &argv);
    server server("config.xml");
}
