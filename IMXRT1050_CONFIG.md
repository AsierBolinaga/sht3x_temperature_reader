# Example configuration for iMXRT1050-EVKB Platform
# Edit pl_config.h to use these settings

## Hardware Configuration

### I2C Connection
- I2C Peripheral: I2C1 (configurable in pl_hw_config.c)
- Clock Frequency: Default from SDK configuration
- SHT3x Address: 0x44 (default) or 0x45

### Pin Configuration
Configure in your SDK pin_mux.c or board configuration:
- I2C1_SCL: Typically GPIO_AD_B1_00
- I2C1_SDA: Typically GPIO_AD_B1_01

## Network Configuration

### lwIP Settings
- Stack: lwIP (lightweight IP)
- Protocol: UDP
- Default Port: 8888

### Network Setup
Configure network settings in your FreeRTOS application:
- IP Address: Static or DHCP
- Gateway: According to your network
- Subnet Mask: According to your network

## FreeRTOS Configuration

### Task Settings
- Main Task Priority: Configurable
- Stack Size: Adequate for UDP/IP operations
- Tick Rate: Typically 1000 Hz

### Required FreeRTOS Components
- FreeRTOS kernel
- lwIP TCP/IP stack
- Ethernet driver (or your network interface)

## SDK Requirements

### NXP MCUXpresso SDK
Required SDK components:
- Device drivers (I2C, GPIO)
- lwIP middleware
- FreeRTOS kernel
- Board support package

### Include Paths
Add to your project:
```
devices/MIMXRT1052/drivers
middleware/lwip
rtos/freertos
board
```

### Linker Configuration
- Flash: BOARD_FLASH (typically XIP from FlexSPI)
- RAM: BOARD_SDRAM or on-chip RAM
- Link script: Provided by SDK

## Platform Selection in pl_config.h
```c
#define PLATFORM_IMXRT1050      /* iMXRT1050-EVKB with FreeRTOS */
/* #define PLATFORM_BEAGLEBONE */
```

## Build Configuration

### Compiler
- Toolchain: GNU ARM Embedded (arm-none-eabi-gcc)
- CPU: Cortex-M7
- FPU: FPv5-D16 (hard float)

### Build Command
```bash
make PLATFORM=IMXRT1050
```

### Flash Programming
Use one of:
- MCUXpresso IDE: Import project and flash
- J-Link: Use J-Link commander
- PyOCD: Use PyOCD flash command
- OpenOCD: Use OpenOCD flash scripts

## Code Integration Steps

### 1. Add Source Files
Add these files to your SDK project:
- main.c
- sht3x.c / sht3x.h
- udp_sender.c / udp_sender.h
- pl_hw_config.c / pl_hw_config.h
- pl_config.h

### 2. Configure pl_hw_config.c

Update the platform-specific sections:

```c
#if USE_FREERTOS
    /* Add your SDK-specific I2C initialization */
    I2C_MasterGetDefaultConfig(&masterConfig);
    I2C_MasterInit(I2C1, &masterConfig, I2C_MASTER_CLK_FREQ);
    
    /* Configure lwIP socket */
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    // ... configure destination address
#endif
```

### 3. Update Makefile

Add SDK paths:
```makefile
CFLAGS += -I$(SDK_PATH)/devices/MIMXRT1052/drivers
CFLAGS += -I$(SDK_PATH)/middleware/lwip/src/include
CFLAGS += -I$(SDK_PATH)/rtos/freertos/include
```

### 4. Main Function Integration

If using FreeRTOS, wrap main() in a task:

```c
void sensor_task(void *pvParameters) {
    main(0, NULL);  // Call the application main
}

int main(void) {
    // SDK initialization
    BOARD_InitPins();
    BOARD_InitBootClocks();
    
    // Create sensor task
    xTaskCreate(sensor_task, "sensor", 2048, NULL, 2, NULL);
    
    // Start FreeRTOS scheduler
    vTaskStartScheduler();
    
    return 0;
}
```

## Troubleshooting

### I2C Issues
- Verify pin configuration in pin_mux
- Check I2C clock frequency
- Ensure pull-up resistors on SDA/SCL (typically 4.7kΩ)

### Network Issues
- Verify lwIP initialization
- Check network cable connection
- Confirm IP configuration
- Review lwIP configuration (lwipopts.h)

### Build Issues
- Ensure ARM toolchain is in PATH
- Verify SDK paths in Makefile
- Check linker script matches your board
- Confirm FreeRTOS configuration

## Example FreeRTOS Configuration

In FreeRTOSConfig.h:
```c
#define configUSE_PREEMPTION              1
#define configUSE_IDLE_HOOK               0
#define configMAX_PRIORITIES              5
#define configMINIMAL_STACK_SIZE          128
#define configTOTAL_HEAP_SIZE             (64 * 1024)
#define configUSE_TIMERS                  1
```

## References

- [NXP MCUXpresso SDK](https://www.nxp.com/design/software/development-software/mcuxpresso-software-and-tools:MCUXPRESSO)
- [FreeRTOS Documentation](https://www.freertos.org/Documentation/RTOS_book.html)
- [lwIP Documentation](https://www.nongnu.org/lwip/)
