#ifndef DATA_DECODER_HPP
#define DATA_DECODER_HPP
#include "DataStructures.hpp"

#include <cstddef>

void decodeAndPrint(const char* buffer, size_t length);
void printIndexData(const IndexDataT* data);
void printMarketWatchData(const MarketWatchDataT* data);

#endif  // DATA_DECODER_HPP
