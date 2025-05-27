#include "DataDecoder.hpp"
#include "DataStructures.hpp"
#include <iostream>

void decodeAndPrint(const char* buffer, size_t length) {
    if (length < sizeof(Header)) {
        std::cerr << "Buffer too small for Header\n";
        return;
    }

    // Interpret buffer as Header to read type and size
    const Header* header = reinterpret_cast<const Header*>(buffer);

    // Check full buffer length matches expected size
    if (length < sizeof(Header) + header->_size) {
        std::cerr << "Incomplete data for payload\n";
        return;
    }

    switch (header->_type) {
        case RequestType::INDEXUPDATE:
            if (header->_size == sizeof(IndexDataT)) {
                // Full message is Header + IndexDataT, reinterpret as IndexDataUpdate
                const IndexDataUpdate* msg = reinterpret_cast<const IndexDataUpdate*>(buffer);
                printIndexData(&msg->_data);
            } else {
                std::cerr << "Size mismatch for IndexDataT\n";
            }
            break;

        case RequestType::UPDATE:
            if (header->_size == sizeof(MarketWatchDataT)) {
                // Full message is Header + MarketWatchDataT, reinterpret as MarketDataUpdate
                const MarketDataUpdate* msg = reinterpret_cast<const MarketDataUpdate*>(buffer);
                printMarketWatchData(&msg->_data);
            } else {
                std::cerr << "Size mismatch for MarketWatchDataT\n";
            }
            break;

        default:
            std::cerr << "Unknown RequestType: " << static_cast<int>(header->_type) << "\n";
            break;
    }
}


