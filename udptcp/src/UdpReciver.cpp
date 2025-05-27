#include "../include/UdpReceiver.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <cstring>

UdpReceiver::UdpReceiver(const std::string& group, int port) {

    sock_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_ < 0)
        throw std::runtime_error("UDP socket creation failed");

    int reuse = 1;
    if (setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
        throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");

    sockaddr_in local_addr{};
    std::memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(port);
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    

    if (bind(sock_, reinterpret_cast<sockaddr*>(&local_addr), sizeof(local_addr)) < 0)
        throw std::runtime_error("UDP bind failed");

    std::memset(&mreq_, 0, sizeof(mreq_));
    mreq_.imr_multiaddr.s_addr = inet_addr(group.c_str());
    mreq_.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(sock_, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq_, sizeof(mreq_)) < 0)
        throw std::runtime_error("UDP group join failed");

    std::cout << "[UDP] Ready to receive packets on group " << group << ", port " << port << std::endl;
}

UdpReceiver::~UdpReceiver() {
    setsockopt(sock_, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq_, sizeof(mreq_));
    if (sock_ >= 0) {
        close(sock_);
    }
}

ssize_t UdpReceiver::receive(char* buffer, size_t buffer_size) {
    ssize_t bytes_received = recvfrom(sock_, buffer, buffer_size, 0, nullptr, nullptr);
    if (bytes_received > 0) {
        std::cout << "[UDP] Received " << bytes_received << " bytes\n";
    } else if (bytes_received == 0) {
        std::cout << "[UDP] Received 0 bytes (no data)\n";
    } else {
        perror("[UDP] recvfrom error");
    }
    return bytes_received;
}
