#include "../include/Bridge.hpp"
#include "../include/MarketDataTypes.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

constexpr size_t expected_size = sizeof(IndexDataT);

Bridge::Bridge(const std::string &config_path)
{
    std::ifstream file(config_path);
    if (!file)
        throw std::runtime_error("Cannot open config file");

    json cfg;
    file >> cfg;

    udp_ = new UdpReceiver(cfg["udp_multicast_group"], cfg["udp_port"]);
    tcp_ = new TcpServer(cfg["tcp_server_ip"], cfg["tcp_server_port"]);
    tcp_->start();
}


void Bridge::run()
{
    char buffer[2048];
    while (true)
    {
        ssize_t len = udp_->receive(buffer, sizeof(buffer));
        if (len > static_cast<ssize_t>(sizeof(Header)))
        {
            auto *header = reinterpret_cast<Header *>(buffer);
            uint16_t expected_total = sizeof(Header) + header->_size;

            if (len != expected_total)
            {
                std::cerr << "[UDP] Header size mismatch: header._size = " << header->_size
                          << ", expected total = " << expected_total
                          << ", but received " << len << " bytes.\n";
                continue;
            }

            // switch (header->_type)
            // {
            // case INDEXUPDATE:
            // {
            //     if (header->_size == sizeof(IndexDataT))
            //     {
            //         auto *indexData = reinterpret_cast<IndexDataT *>(buffer + sizeof(Header));

            //         std::cout << "[UDP] IndexDataT:\n";
            //         std::cout << "  _name: " << indexData->_name << "\n";
            //         std::cout << "  _value: " << indexData->_value << "\n";
            //         std::cout << "  _open: " << indexData->_open << "\n";
            //         std::cout << "  _high: " << indexData->_high << "\n";
            //         std::cout << "  _low: " << indexData->_low << "\n";
            //         std::cout << "  _close: " << indexData->_close << "\n";
            //         std::cout << "  _yearlyHigh: " << indexData->_yearlyHigh << "\n";
            //         std::cout << "  _yearlyLow: " << indexData->_yearlyLow << "\n";
            //         std::cout << "  _percentageChange: " << indexData->_percentageChange << "\n";

            //         tcp_->broadcast(buffer, len);
            //     }
            //     else
            //     {
            //         std::cerr << "[UDP] IndexUpdate size mismatch\n";
            //     }
            //     break;
            // }

            // case UPDATE:
            // {
            //     if (header->_size == sizeof(MarketWatchDataT))
            //     {
            //         auto *data = reinterpret_cast<MarketWatchDataT *>(buffer + sizeof(Header));

            //         std::cout << "[UDP] MarketWatchDataT:\n";
            //         std::cout << "  _token: " << data->_token << "\n";
            //         std::cout << "  _lastTradeQuantity: " << data->_lastTradeQuantity << "\n";
            //         std::cout << "  _averageTradePrice: " << data->_averageTradePrice << "\n";
            //         std::cout << "  _lastTradePrice: " << data->_lastTradePrice << "\n";
            //         std::cout << "  _lowDPR: " << data->_lowDPR << "\n";
            //         std::cout << "  _highDPR: " << data->_highDPR << "\n";
            //         std::cout << "  _lowLPP: " << data->_lowLPP << "\n";
            //         std::cout << "  _highLPP: " << data->_highLPP << "\n";
            //         std::cout << "  _open: " << data->_open << "\n";
            //         std::cout << "  _high: " << data->_high << "\n";
            //         std::cout << "  _low: " << data->_low << "\n";
            //         std::cout << "  _close: " << data->_close << "\n";
            //         std::cout << "  _percentageChange: " << data->_percentageChange << "\n";
            //         std::cout << "  _totalBuyQuantity: " << data->_totalBuyQuantity << "\n";
            //         std::cout << "  _totalSellQuantity: " << data->_totalSellQuantity << "\n";
            //         std::cout << "  _volumeTradedToday: " << data->_volumeTradedToday << "\n";
            //         std::cout << "  _openInterest: " << data->_openInterest << "\n";
            //         std::cout << "  _lastTradeTime: " << data->_lastTradeTime << "\n";

            //         std::cout << "  _bid ladder:\n";
            //         for (int i = 0; i < MarketWatchLadderCount; ++i)
            //         {
            //             std::cout << "    [Bid " << i << "] Price: " << data->_bid[i]._price
            //                       << ", Qty: " << data->_bid[i]._quantity << "\n";
            //         }

            //         std::cout << "  _ask ladder:\n";
            //         for (int i = 0; i < MarketWatchLadderCount; ++i)
            //         {
            //             std::cout << "    [Ask " << i << "] Price: " << data->_ask[i]._price
            //                       << ", Qty: " << data->_ask[i]._quantity << "\n";
            //         }

            //         tcp_->broadcast(buffer, len);
            //     }
            //     else
            //     {
            //         std::cerr << "[UDP] MarketWatchDataT size mismatch\n";
            //     }
            //     break;
            // }

            // default:
            //     std::cerr << "[UDP] Unknown request type: " << header->_type << "\n";
            //     break;
            // }

            switch (header->_type)
            {
            case INDEXUPDATE:
            {
                if (header->_size == sizeof(IndexDataT))
                {
                    tcp_->broadcast(buffer, len);
                }
                break;
            }

            case UPDATE:
            {
                if (header->_size == sizeof(MarketWatchDataT))
                {
                    tcp_->broadcast(buffer, len);
                }
                break;
            }

            default:
                break;
            }
        }
        else
        {
            std::cerr << "[UDP] Received packet too small: " << len << " bytes\n";
        }
    }
}
