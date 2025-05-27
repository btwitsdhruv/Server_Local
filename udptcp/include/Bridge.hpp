#pragma once
#include "UdpReceiver.hpp"
#include "TcpServer.hpp"
#include<string>

class Bridge {
public:
    Bridge(const std::string& config_path);
    void run();

private:
    UdpReceiver* udp_;
    TcpServer* tcp_;
};
