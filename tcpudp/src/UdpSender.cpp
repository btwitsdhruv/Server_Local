#include "UdpSender.hpp"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <unistd.h>

UdpSender::UdpSender(const std::string& group, int port) : socket_fd_(-1) {
    socket_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd_ < 0) {
        throw std::runtime_error("Failed to create UDP socket: " + std::string(strerror(errno)));
    }

    int ttl = 1;
    if (setsockopt(socket_fd_, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0) {
        close(socket_fd_);
        throw std::runtime_error("Failed to set multicast TTL: " + std::string(strerror(errno)));
    }

    multicast_addr_.sin_family = AF_INET;
    multicast_addr_.sin_port = htons(port);
    multicast_addr_.sin_addr.s_addr = inet_addr(group.c_str());
    if (multicast_addr_.sin_addr.s_addr == INADDR_NONE) {
        close(socket_fd_);
        throw std::runtime_error("Invalid multicast group address: " + group);
    }

    std::cout << "[*] UDP multicast setup for " << group << ":" << port << "\n";
}

UdpSender::~UdpSender() {
    if (socket_fd_ >= 0) {
        close(socket_fd_);
    }
}

void UdpSender::sendData(const char* buffer, size_t length) {
    if (sendto(socket_fd_, buffer, length, 0, (sockaddr*)&multicast_addr_, sizeof(multicast_addr_)) < 0) {
        throw std::runtime_error("UDP sendto failed: " + std::string(strerror(errno)));
    }
    std::cout << "[>] Forwarded " << length << " bytes to UDP multicast\n";
}