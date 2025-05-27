
#include "../include/TcpServer.hpp"
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>

TcpServer::TcpServer(const std::string &ip, int port) : running_(true)
{
    server_sock_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock_ < 0)
        throw std::runtime_error("TCP socket failed");

    int reuse = 1;
    setsockopt(server_sock_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (bind(server_sock_, (sockaddr *)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("TCP bind failed");

    if (listen(server_sock_, 10) < 0)
        throw std::runtime_error("TCP listen failed");
}

void TcpServer::start()
{
    accept_thread_ = std::thread(&TcpServer::acceptLoop, this);
}

void TcpServer::acceptLoop()
{
    while (running_)
    {
        sockaddr_in client_addr{};
        socklen_t len = sizeof(client_addr);
        int client_sock = accept(server_sock_, (sockaddr *)&client_addr, &len);
        if (client_sock >= 0)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            clients_.push_back(client_sock);
            std::cout << "[+] TCP client connected from "
                      << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << "\n";
        }
    }
}

void TcpServer::broadcast(const char *data, size_t len)
{
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto it = clients_.begin(); it != clients_.end();)
    {
        uint32_t net_len = htonl(len);
        bool failed = false;

        if (send(*it, &net_len, sizeof(net_len), 0) != sizeof(net_len))
            failed = true;
        else if (send(*it, data, len, 0) != (ssize_t)len)
            failed = true;

        if (failed)
        {
            std::cerr << "[-] TCP client disconnected, removing socket " << *it << "\n";
            close(*it);
            it = clients_.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

TcpServer::~TcpServer()
{
    running_ = false;
    if (accept_thread_.joinable())
        accept_thread_.join();
    for (int client : clients_)
        close(client);
    close(server_sock_);
}
