#include "TcpReceiver.hpp"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


TcpReceiver::TcpReceiver(const std::string& ip, int port)
    : server_ip_(ip), server_port_(port), socket_fd_(-1) {
    socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd_ < 0) {
        throw std::runtime_error("Failed to create TCP socket: " + std::string(strerror(errno)));
    }

    // Set SO_REUSEADDR to allow quick restarts
    int opt = 1;
    if (setsockopt(socket_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Warning: Failed to set SO_REUSEADDR: " << strerror(errno) << "\n";
    }

    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(server_port_);
    if (inet_pton(AF_INET, server_ip_.c_str(), &server_addr_.sin_addr) <= 0) {
        close();
        throw std::runtime_error("Invalid TCP server IP: " + server_ip_);
    }
}


TcpReceiver::~TcpReceiver() {
    close();
}

void TcpReceiver::connect() {
    if (socket_fd_ < 0) {
        throw std::runtime_error("Socket is closed, cannot connect");
    }

    if (::connect(socket_fd_, (sockaddr*)&server_addr_, sizeof(server_addr_)) < 0) {
        throw std::runtime_error("TCP connect failed: " + std::string(strerror(errno)));
    }
    std::cout << "[*] Connected to TCP server " << server_ip_ << ":" << server_port_ << "\n";
}

ssize_t TcpReceiver::receiveData(char* buffer, size_t buffer_size, uint32_t& data_len) {
    if (socket_fd_ < 0) {
        throw std::runtime_error("Attempted to receive on closed socket");
    }

    uint32_t net_len;
    ssize_t received = recv(socket_fd_, &net_len, sizeof(net_len), MSG_WAITALL);
    if (received == 0) {
        throw std::runtime_error("TCP connection closed by peer");
    } else if (received < 0) {
        throw std::runtime_error("TCP receive error (length): " + std::string(strerror(errno)));
    }

    data_len = ntohl(net_len);
    if (data_len > buffer_size) {
        throw std::runtime_error("Incoming data length (" + std::to_string(data_len) + ") exceeds buffer size (" + std::to_string(buffer_size) + ")");
    }

    received = recv(socket_fd_, buffer, data_len, MSG_WAITALL);
    if (received == 0) {
        throw std::runtime_error("TCP connection closed by peer during data receive");
    } else if (received < 0) {
        throw std::runtime_error("TCP receive error (data): " + std::string(strerror(errno)));
    }


    std::cout << "[>] Received " << received << " bytes from TCP server\n";
    return received;
}

void TcpReceiver::close() {
    if (socket_fd_ >= 0) {
        ::shutdown(socket_fd_, SHUT_RDWR);
        ::close(socket_fd_);
        socket_fd_ = -1;
        std::cout << "[*] TCP socket closed\n";
    }
}
