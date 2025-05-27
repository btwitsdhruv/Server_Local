

#pragma once
#include <string>
#include <vector>
#include <thread>
#include <mutex>

class TcpServer {
public:
    TcpServer(const std::string& ip, int port);
    ~TcpServer();

    void start();
    void broadcast(const char* data, size_t len);


private:
    void acceptLoop();

    int server_sock_;
    std::vector<int> clients_;
    std::mutex mutex_;
    std::thread accept_thread_;
    bool running_;
};

