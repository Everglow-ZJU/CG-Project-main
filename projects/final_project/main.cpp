#include <iostream>
#include <cstdlib>

#include "core.h"


int main() {
    try {
        Core app;
        app.run();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...) {
        std::cerr << "Unknown Error" << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}