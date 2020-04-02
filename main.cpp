#include <iostream>
#include <gst/gst.h>
#include "src/server/server.h"
#include "includes/ndi/Processing.NDI.Lib.h"

int main(int argc, char *argv[]) {
    std::cout << "Welcome to:" << std::endl;
    gst_init (&argc, &argv);
    server server("config.xml");

    //NDI TEST
    // Not required, but "correct" (see the SDK documentation.
    if (!NDIlib_initialize()) return 0;

    // We are going to create an NDI finder that locates sources on the network.
    NDIlib_find_instance_t pNDI_find = NDIlib_find_create_v2();
    if (!pNDI_find) return 0;

    // Run for one minute
    using namespace std::chrono;
    for (const auto start = high_resolution_clock::now(); high_resolution_clock::now() - start < minutes(1);)
    {	// Wait up till 5 seconds to check for new sources to be added or removed
        if (!NDIlib_find_wait_for_sources(pNDI_find, 5000/* 5 seconds */))
        { printf("No change to the sources found.\n"); continue; }

        // Get the updated list of sources
        uint32_t no_sources = 0;
        const NDIlib_source_t* p_sources = NDIlib_find_get_current_sources(pNDI_find, &no_sources);

        // Display all the sources.
        printf("Network sources (%u found).\n", no_sources);
        for (uint32_t i = 0; i < no_sources; i++)
            printf("%u. %s\n", i + 1, p_sources[i].p_ndi_name);
    }

    // Destroy the NDI finder
    NDIlib_find_destroy(pNDI_find);

    // Finished
    NDIlib_destroy();

    // Success. We are done
    return 0;

}
