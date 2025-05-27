#include "../include/Bridge.hpp"
#include <signal.h>
#include <iostream>

int main() {
    signal(SIGPIPE, SIG_IGN);

    try {
        Bridge bridge("config.json");
        bridge.run();

    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
