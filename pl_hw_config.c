#include "pl_hw_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if USE_LINUX
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/ioctl.h>
    #include <linux/i2c-dev.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <time.h>
#elif USE_FREERTOS
    /* FreeRTOS includes */
    #include "FreeRTOS.h"
    #include "task.h"
    /* Add other necessary iMXRT1050 includes here */
    /* #include "fsl_i2c.h" */
    /* #include "lwip/sockets.h" */
    /* #include "lwip/inet.h" */
#endif

/* Platform-specific variables */
#if USE_LINUX
static int i2c_fd = -1;
static int udp_socket = -1;
static struct sockaddr_in udp_dest_addr;
#elif USE_FREERTOS
/* Add FreeRTOS-specific variables here */
#endif

int pl_hw_init(void)
{
#if USE_LINUX
    /* Linux initialization */
    return 0;
#elif USE_FREERTOS
    /* FreeRTOS initialization */
    /* Initialize clocks, pins, etc. */
    return 0;
#endif
}

int pl_i2c_init(void)
{
#if USE_LINUX
    /* Open I2C device (usually /dev/i2c-1 on BeagleBone) */
    i2c_fd = open("/dev/i2c-1", O_RDWR);
    if (i2c_fd < 0) {
        perror("Failed to open I2C device");
        return -1;
    }
    return 0;
#elif USE_FREERTOS
    /* Initialize I2C peripheral on iMXRT1050 */
    /* i2c_master_config_t masterConfig; */
    /* I2C_MasterGetDefaultConfig(&masterConfig); */
    /* I2C_MasterInit(I2C1, &masterConfig, I2C_MASTER_CLK_FREQ); */
    return 0;
#endif
}

int pl_i2c_write(uint8_t addr, const uint8_t *data, uint16_t len)
{
#if USE_LINUX
    if (ioctl(i2c_fd, I2C_SLAVE, addr) < 0) {
        perror("Failed to set I2C slave address");
        return -1;
    }
    
    if (write(i2c_fd, data, len) != len) {
        perror("Failed to write to I2C device");
        return -1;
    }
    return 0;
#elif USE_FREERTOS
    /* Use iMXRT1050 I2C driver */
    /* i2c_master_transfer_t transfer; */
    /* transfer.slaveAddress = addr; */
    /* transfer.data = (uint8_t *)data; */
    /* transfer.dataSize = len; */
    /* transfer.direction = kI2C_Write; */
    /* return I2C_MasterTransferBlocking(I2C1, &transfer); */
    return 0;
#endif
}

int pl_i2c_read(uint8_t addr, uint8_t *data, uint16_t len)
{
#if USE_LINUX
    if (ioctl(i2c_fd, I2C_SLAVE, addr) < 0) {
        perror("Failed to set I2C slave address");
        return -1;
    }
    
    if (read(i2c_fd, data, len) != len) {
        perror("Failed to read from I2C device");
        return -1;
    }
    return 0;
#elif USE_FREERTOS
    /* Use iMXRT1050 I2C driver */
    /* i2c_master_transfer_t transfer; */
    /* transfer.slaveAddress = addr; */
    /* transfer.data = data; */
    /* transfer.dataSize = len; */
    /* transfer.direction = kI2C_Read; */
    /* return I2C_MasterTransferBlocking(I2C1, &transfer); */
    return 0;
#endif
}

int pl_udp_init(const char *ip_addr, uint16_t port)
{
#if USE_LINUX
    /* Create UDP socket */
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket < 0) {
        perror("Failed to create UDP socket");
        return -1;
    }
    
    /* Setup destination address */
    memset(&udp_dest_addr, 0, sizeof(udp_dest_addr));
    udp_dest_addr.sin_family = AF_INET;
    udp_dest_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, ip_addr, &udp_dest_addr.sin_addr) <= 0) {
        perror("Invalid IP address");
        close(udp_socket);
        udp_socket = -1;
        return -1;
    }
    
    return 0;
#elif USE_FREERTOS
    /* Initialize lwIP UDP socket */
    /* udp_socket = socket(AF_INET, SOCK_DGRAM, 0); */
    /* Setup destination address similar to Linux */
    return 0;
#endif
}

int pl_udp_send(const uint8_t *data, uint16_t len)
{
#if USE_LINUX
    if (udp_socket < 0) {
        fprintf(stderr, "UDP socket not initialized\n");
        return -1;
    }
    
    ssize_t sent = sendto(udp_socket, data, len, 0,
                          (struct sockaddr *)&udp_dest_addr,
                          sizeof(udp_dest_addr));
    if (sent < 0) {
        perror("Failed to send UDP packet");
        return -1;
    }
    if (sent != (ssize_t)len) {
        fprintf(stderr, "Partial UDP send: %zd of %u bytes\n", sent, len);
        return -1;
    }
    return 0;
#elif USE_FREERTOS
    /* Send using lwIP */
    /* sendto(udp_socket, data, len, 0, &udp_dest_addr, sizeof(udp_dest_addr)); */
    return 0;
#endif
}

void pl_udp_close(void)
{
#if USE_LINUX
    if (udp_socket >= 0) {
        close(udp_socket);
        udp_socket = -1;
    }
#elif USE_FREERTOS
    /* Close lwIP socket */
    /* if (udp_socket >= 0) { */
    /*     close(udp_socket); */
    /* } */
#endif
}

void pl_i2c_close(void)
{
#if USE_LINUX
    if (i2c_fd >= 0) {
        close(i2c_fd);
        i2c_fd = -1;
    }
#elif USE_FREERTOS
    /* Deinitialize I2C peripheral */
#endif
}

void pl_hw_cleanup(void)
{
    pl_udp_close();
    pl_i2c_close();
}

void pl_delay_ms(uint32_t ms)
{
#if USE_LINUX
    usleep(ms * 1000);
#elif USE_FREERTOS
    vTaskDelay(pdMS_TO_TICKS(ms));
#endif
}

uint32_t pl_get_tick_ms(void)
{
#if USE_LINUX
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#elif USE_FREERTOS
    return (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
#endif
}
