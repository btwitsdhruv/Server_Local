#include "DataStructures.hpp"
#include <iostream>
#include <iomanip>
#include <cstring>

void printIndexData(const IndexDataT* data) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Index Data:\n";
    std::cout << " Name: " << std::string(data->_name, strnlen(data->_name, sizeof(data->_name))) << "\n";
    std::cout << " Value: " << data->_value << "\n";
    std::cout << " Open: " << data->_open << "\n";
    std::cout << " High: " << data->_high << "\n";
    std::cout << " Low: " << data->_low << "\n";
    std::cout << " Close: " << data->_close << "\n";
    std::cout << " Yearly High: " << data->_yearlyHigh << "\n";
    std::cout << " Yearly Low: " << data->_yearlyLow << "\n";
    std::cout << " Percentage Change: " << data->_percentageChange << "%\n";
}

void printMarketWatchData(const MarketWatchDataT* data) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Market Watch Data:\n";
    std::cout << " Token: " << data->_token << "\n";
    std::cout << " Last Trade Quantity: " << data->_lastTradeQuantity << "\n";
    std::cout << " Last Trade Price: " << data->_lastTradePrice << "\n";
    std::cout << " Avg Trade Price: " << data->_averageTradePrice << "\n";
    std::cout << " Open: " << data->_open << " High: " << data->_high << " Low: " << data->_low << " Close: " << data->_close << "\n";
    std::cout << " % Change: " << data->_percentageChange << "%\n";
    std::cout << " Volume: " << data->_volumeTradedToday << " OI: " << data->_openInterest << "\n";
    std::cout << " Last Trade Time: " << std::string(data->_lastTradeTime, strnlen(data->_lastTradeTime, sizeof(data->_lastTradeTime))) << "\n";
}
