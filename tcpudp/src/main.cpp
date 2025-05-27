#include "TcpReceiver.hpp"
#include "UdpSender.hpp"
#include "DataDecoder.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <csignal>
#include <cstdlib>

using json = nlohmann::json;

volatile sig_atomic_t running = 1;

void signal_handler(int) {
    running = 0;
}

int main() {
    // Setup signal handling
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    try {
        // Load configuration
        std::ifstream config_file("../config/client_config.json");
        if (!config_file.is_open()) {
            throw std::runtime_error("Failed to open client_config.json");
        }

        json config;
        config_file >> config;

        // Validate configuration
        if (!config.contains("tcp_server_ip") || !config.contains("tcp_server_port") ||
            !config.contains("udp_multicast_group") || !config.contains("udp_port")) {
            throw std::runtime_error("Missing required configuration fields");
        }

        std::string tcp_ip = config["tcp_server_ip"].get<std::string>();
        int tcp_port = config["tcp_server_port"].get<int>();
        std::string udp_group = config["udp_multicast_group"].get<std::string>();
        int udp_port = config["udp_port"].get<int>();

        // Initialize components
        TcpReceiver tcp_receiver(tcp_ip, tcp_port);
        UdpSender udp_sender(udp_group, udp_port);

        // Connect to TCP server
        tcp_receiver.connect();

        // Main loop
        constexpr size_t BUFFER_SIZE = 2048;
        char buffer[BUFFER_SIZE];
        uint32_t data_len;

        while (running) {
            ssize_t received = tcp_receiver.receiveData(buffer, BUFFER_SIZE, data_len);
            if (received > 0) {
                decodeAndPrint(buffer, data_len); 
            }
            udp_sender.sendData(buffer, received);
        }

        std::cout << "[*] Shutting down gracefully\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}