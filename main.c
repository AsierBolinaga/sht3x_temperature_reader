#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pl_config.h"
#include "pl_hw_config.h"
#include "sht3x.h"
#include "udp_sender.h"

/* Default UDP destination (can be overridden via command line) */
#define DEFAULT_UDP_IP "192.168.1.100"

int main(int argc, char *argv[])
{
    float temperature, humidity;
    int ret;
    const char *udp_ip = DEFAULT_UDP_IP;
    uint16_t udp_port = UDP_PORT;
    
    /* Parse command line arguments */
    if (argc > 1) {
        udp_ip = argv[1];
    }
    if (argc > 2) {
        long port_value = atol(argv[2]);
        if (port_value <= 0 || port_value > 65535) {
            fprintf(stderr, "Error: Invalid port number. Must be between 1 and 65535.\n");
            return 1;
        }
        udp_port = (uint16_t)port_value;
    }
    
    printf("SHT3x Temperature Reader\n");
    printf("========================\n");
#ifdef PLATFORM_IMXRT1050
    printf("Platform: iMXRT1050-EVKB (FreeRTOS)\n");
#elif defined(PLATFORM_BEAGLEBONE)
    printf("Platform: BeagleBone (Linux)\n");
#endif
    printf("UDP Destination: %s:%u\n", udp_ip, udp_port);
    printf("Reading interval: %d ms\n\n", TEMPERATURE_READ_INTERVAL_MS);
    
    /* Initialize hardware platform */
    if (pl_hw_init() != 0) {
        fprintf(stderr, "Failed to initialize hardware platform\n");
        return 1;
    }
    
    /* Initialize SHT3x sensor */
    printf("Initializing SHT3x sensor...\n");
    ret = sht3x_init();
    if (ret != SHT3X_OK) {
        fprintf(stderr, "Failed to initialize SHT3x sensor (error: %d)\n", ret);
        return 1;
    }
    printf("SHT3x sensor initialized successfully\n");
    
    /* Initialize UDP sender */
    printf("Initializing UDP sender...\n");
    ret = udp_sender_init(udp_ip, udp_port);
    if (ret != 0) {
        fprintf(stderr, "Failed to initialize UDP sender (error: %d)\n", ret);
        return 1;
    }
    printf("UDP sender initialized successfully\n\n");
    
    /* Main loop */
    printf("Starting temperature monitoring...\n");
    while (1) {
        /* Read temperature and humidity */
        ret = sht3x_read(&temperature, &humidity);
        if (ret == SHT3X_OK) {
            printf("Temperature: %.2f°C, Humidity: %.2f%%\n",
                   temperature, humidity);
            
            /* Send data via UDP */
            ret = udp_send_temperature(temperature, humidity);
            if (ret != 0) {
                fprintf(stderr, "Failed to send UDP packet (error: %d)\n", ret);
            } else {
                printf("Data sent via UDP\n");
            }
        } else {
            fprintf(stderr, "Failed to read from SHT3x sensor (error: %d)\n", ret);
        }
        
        /* Wait before next reading */
        pl_delay_ms(TEMPERATURE_READ_INTERVAL_MS);
    }
    
    /* Cleanup (will never reach here in this example) */
    pl_hw_cleanup();
    
    return 0;
}
