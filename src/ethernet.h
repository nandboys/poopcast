#pragma once

#include <stddef.h>

constexpr size_t MAC_SIZE = 6;
constexpr size_t TYPE_SIZE = 2;
constexpr unsigned char MAC_BROADCAST[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
constexpr unsigned char COOL_TYPE[2] = {0x13, 0x37};

struct eth_header {
    unsigned char dst[MAC_SIZE];
    unsigned char src[MAC_SIZE];
    unsigned char type[TYPE_SIZE];
} __attribute__((packed));

constexpr size_t ETH_HEADER_SIZE = sizeof(struct eth_header);
constexpr size_t ETH_MAX_DATA_SIZE = 1500;
constexpr size_t ETH_MAX_FRAME_SIZE = ETH_HEADER_SIZE + ETH_MAX_DATA_SIZE;

