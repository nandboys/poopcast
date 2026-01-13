#include <linux/sockios.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>  //htons
#include <string.h>
#include <linux/if.h>

#include "interface.h"
#include "ethernet.h"

constexpr const char MESSAGE[] = "I am poop";
constexpr size_t MESSAGE_LEN = sizeof(MESSAGE) / sizeof(MESSAGE[0]) - 1;

int main(int argc, char *argv[]) {
    struct if_nameindex *interfaces = if_nameindex();
    if (interfaces == NULL) {
        perror("if_nameindex");
        return 1;
    }

    unsigned int interface_index = prompt_interface_index(interfaces);
    printf("Chosen interface: %u\n", interface_index);

    if_freenameindex(interfaces);

    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    struct ifreq ifr;
    if_indextoname(interface_index, ifr.ifr_name);
    ioctl(sock, SIOCGIFHWADDR, &ifr);

    unsigned char frame_buffer[ETH_MAX_FRAME_SIZE];
    memset(frame_buffer, 0, sizeof(frame_buffer));

    struct eth_header *header_ptr = (struct eth_header *)frame_buffer;
    memcpy(header_ptr->dst, MAC_BROADCAST, MAC_SIZE);
    memcpy(header_ptr->src, ifr.ifr_hwaddr.sa_data , MAC_SIZE);
    memcpy(header_ptr->type, COOL_TYPE, TYPE_SIZE);

    memcpy(frame_buffer + ETH_HEADER_SIZE, MESSAGE, MESSAGE_LEN);

    struct sockaddr_ll sll = {
        .sll_family = AF_PACKET,
        .sll_ifindex = (int)interface_index,
        .sll_halen = (unsigned char)MAC_SIZE
    };
    memcpy(sll.sll_addr, MAC_BROADCAST, MAC_SIZE);

    constexpr size_t TOTAL_LEN = ETH_HEADER_SIZE + MESSAGE_LEN;

    while (true) {
        ssize_t sent = sendto(sock, frame_buffer, TOTAL_LEN, 0, (struct sockaddr *)&sll, sizeof(sll));
        if (sent < 0) {
            perror("sendto");
            return 1;
        } else {
            printf("Pooped %zd bytes onto the wire!\n", sent);
        }
    }

    return 0;
}
