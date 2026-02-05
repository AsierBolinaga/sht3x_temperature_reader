#include "udp_sender.h"
#include "pl_hw_config.h"
#include <stdio.h>
#include <string.h>

int udp_sender_init(const char *ip_addr, uint16_t port)
{
    return pl_udp_init(ip_addr, port);
}

int udp_send_temperature(float temperature, float humidity)
{
    char buffer[128];
    int len;
    
    /* Format temperature and humidity data as JSON */
    len = snprintf(buffer, sizeof(buffer),
                   "{\"temperature\":%.2f,\"humidity\":%.2f}",
                   temperature, humidity);
    
    if (len < 0 || (size_t)len >= sizeof(buffer)) {
        fprintf(stderr, "Error: Failed to format temperature data (buffer overflow)\n");
        return -1;
    }
    
    /* Send via UDP */
    return pl_udp_send((uint8_t *)buffer, (uint16_t)len);
}

void udp_sender_close(void)
{
    pl_udp_close();
}
