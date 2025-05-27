#ifndef TCP_RECEIVER_HPP
#define TCP_RECEIVER_HPP
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>

class TcpReceiver {
public:
    TcpReceiver(const std::string& ip, int port);
    ~TcpReceiver();
    void connect();
    ssize_t receiveData(char* buffer, size_t buffer_size, uint32_t& data_len);
    void close();
private:
    std::string server_ip_;
    int server_port_;
    int socket_fd_;
    sockaddr_in server_addr_;
};

#endif // TCP_RECEIVER_HPP