#ifndef UDP_SENDER_HPP
#define UDP_SENDER_HPP
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>

class UdpSender {
public:
    UdpSender(const std::string& group, int port);
    ~UdpSender();
    void sendData(const char* buffer, size_t length);
private:
    int socket_fd_;
    sockaddr_in multicast_addr_;
};

#endif // UDP_SENDER_HPP