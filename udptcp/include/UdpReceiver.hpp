#pragma once
#include <string>
#include <netinet/in.h>
#include <cstddef>
class UdpReceiver {
public:
    UdpReceiver(const std::string& group, int port);
    ~UdpReceiver();
    ssize_t receive(char* buffer, size_t buffer_size);

private:
    int sock_;
    ip_mreq mreq_;
};
