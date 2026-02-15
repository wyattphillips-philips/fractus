#include "App.h"

#include <cstdlib>
#include <exception>
#include <iostream>

int main() {
    try {
        App app;
        return app.run();
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
    }

    return EXIT_FAILURE;
}
