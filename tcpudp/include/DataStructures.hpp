#pragma once

#include <cstdint>

// Protocol Enums
enum RequestType : uint16_t {
  
    UPDATE      = 4,
    INDEXUPDATE = 5,
};

// Basic Type Definitions
using TokenT    = uint32_t;
using PriceT    = uint32_t;
using QuantityT = uint32_t;

// Constants
constexpr int MarketWatchLadderCount = 5;
constexpr int TimestampLength        = 50;

#pragma pack(push, 2) // Set structure alignment to 2 bytes

// Common Header
struct Header {
    RequestType _type;
    uint16_t    _size;
};



// Market Data Subscribe/Unsubscribe
struct MarketDataRequest {
    Header _header;
    TokenT _token;
};

// Ladder Price Point
struct PricePointsT {
    PriceT    _price;
    QuantityT _quantity;
    uint32_t  _order;
};

// Market Data Structure
struct MarketWatchDataT {
    PricePointsT _bid[MarketWatchLadderCount];
    PricePointsT _ask[MarketWatchLadderCount];

    TokenT    _token;
    QuantityT _lastTradeQuantity;
    PriceT    _averageTradePrice;
    PriceT    _lastTradePrice;

    PriceT _lowDPR;
    PriceT _highDPR;

    PriceT _lowLPP;
    PriceT _highLPP;

    PriceT _open;
    PriceT _high;
    PriceT _low;
    PriceT _close;
    float  _percentageChange;

    QuantityT _totalBuyQuantity;
    QuantityT _totalSellQuantity;
    QuantityT _volumeTradedToday;
    QuantityT _openInterest;

    char _lastTradeTime[TimestampLength];
};

// Index Data Structure
struct IndexDataT {
    PriceT _value;
    PriceT _open;
    PriceT _high;
    PriceT _low;
    PriceT _close;
    PriceT _yearlyHigh;
    PriceT _yearlyLow;
    float  _percentageChange;
    char   _name[21];
};

// UDP message wrappers (used to detect message type)
struct MarketDataUpdate {
    Header           _header;
    MarketWatchDataT _data;
};

struct IndexDataUpdate {
    Header     _header;
    IndexDataT _data;
};

#pragma pack(pop) // Restore packing
