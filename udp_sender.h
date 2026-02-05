#ifndef UDP_SENDER_H
#define UDP_SENDER_H

#include <stdint.h>

/**
 * @brief Initialize UDP sender
 * @param ip_addr Destination IP address
 * @param port Destination port
 * @return 0 on success, negative on error
 */
int udp_sender_init(const char *ip_addr, uint16_t port);

/**
 * @brief Send temperature data via UDP
 * @param temperature Temperature value in Celsius
 * @param humidity Humidity value in %
 * @return 0 on success, negative on error
 */
int udp_send_temperature(float temperature, float humidity);

/**
 * @brief Close UDP sender
 */
void udp_sender_close(void);

#endif /* UDP_SENDER_H */
