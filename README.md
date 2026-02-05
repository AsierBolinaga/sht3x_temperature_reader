# SHT3x Temperature Reader

A cross-platform C application for reading temperature and humidity from the SHT3x sensor and transmitting data via UDP. This project supports both **iMXRT1050-EVKB** (with FreeRTOS) and **BeagleBone** (with Linux) platforms through a portability layer.

## Features

- **Cross-platform support**: Works on iMXRT1050-EVKB (FreeRTOS) and BeagleBone (Linux)
- **SHT3x sensor driver**: I2C communication with CRC-8 checksum verification
- **UDP transmission**: Sends temperature data in JSON format via UDP
- **Portability layer**: Platform-specific code is isolated in `pl_config.h` and `pl_hw_config.c/h`
- **Configurable**: Easy platform switching and customizable UDP destination

## Hardware Requirements

### BeagleBone (Linux)
- BeagleBone Black or similar Linux-based board
- SHT3x sensor connected via I2C (typically I2C-1)
- Network connection for UDP transmission

### iMXRT1050-EVKB (FreeRTOS)
- iMXRT1050-EVKB development board
- SHT3x sensor connected via I2C
- FreeRTOS configured with lwIP for networking

## Software Architecture

### Portability Layer
The application uses a portability layer to abstract platform-specific functionality:

- **pl_config.h**: Platform selection and common configuration
- **pl_hw_config.h**: Hardware abstraction layer interface
- **pl_hw_config.c**: Platform-specific implementations

### Application Modules
- **sht3x.c/h**: SHT3x sensor driver with I2C communication
- **udp_sender.c/h**: UDP transmission module
- **main.c**: Main application logic (platform-independent)

## Building

### BeagleBone (Linux)

1. **Default build** (BeagleBone):
   ```bash
   make
   ```

2. **Or explicitly specify platform**:
   ```bash
   make PLATFORM=BEAGLEBONE
   ```

3. **Install** (optional):
   ```bash
   sudo make install
   ```

### iMXRT1050-EVKB (FreeRTOS)

1. **Configure platform** in `pl_config.h`:
   ```c
   #define PLATFORM_IMXRT1050      /* iMXRT1050-EVKB with FreeRTOS */
   /* #define PLATFORM_BEAGLEBONE */
   ```

2. **Build**:
   ```bash
   make PLATFORM=IMXRT1050
   ```

3. **Note**: For iMXRT1050, you need to:
   - Install ARM GCC toolchain (`arm-none-eabi-gcc`)
   - Configure FreeRTOS and SDK paths in the Makefile
   - Add appropriate linker scripts and startup code

## Configuration

### Platform Selection
Edit `pl_config.h` to select the target platform:

```c
/* Uncomment ONE of the following */
#define PLATFORM_IMXRT1050      /* iMXRT1050-EVKB with FreeRTOS */
/* #define PLATFORM_BEAGLEBONE */   /* BeagleBone with Linux */
```

### Sensor Configuration
In `pl_config.h`:
```c
#define SHT3X_I2C_ADDRESS 0x44  /* SHT3x I2C address (0x44 or 0x45) */
#define TEMPERATURE_READ_INTERVAL_MS 1000  /* Reading interval in ms */
```

### UDP Configuration
In `pl_config.h`:
```c
#define UDP_PORT 8888  /* Default UDP port */
```

## Usage

### BeagleBone

1. **Connect SHT3x sensor** to I2C-1:
   - SDA: Pin P9_18
   - SCL: Pin P9_17
   - VDD: 3.3V
   - GND: Ground

2. **Run with default settings** (sends to 192.168.1.100:8888):
   ```bash
   ./sht3x_temperature_reader
   ```

3. **Specify UDP destination**:
   ```bash
   ./sht3x_temperature_reader <IP_ADDRESS> <PORT>
   ```
   
   Example:
   ```bash
   ./sht3x_temperature_reader 192.168.1.50 9000
   ```

### Testing UDP Reception

On the receiving computer, you can use `netcat` to verify UDP packets:

```bash
nc -lu 8888
```

You should see JSON messages like:
```json
{"temperature":23.45,"humidity":45.67}
```

## Data Format

Temperature and humidity data is sent via UDP in JSON format:

```json
{
  "temperature": 23.45,
  "humidity": 45.67
}
```

- **temperature**: Temperature in degrees Celsius
- **humidity**: Relative humidity in percentage

## File Structure

```
.
├── main.c              # Main application (platform-independent)
├── sht3x.c             # SHT3x sensor driver
├── sht3x.h             # SHT3x sensor interface
├── udp_sender.c        # UDP transmission module
├── udp_sender.h        # UDP transmission interface
├── pl_config.h         # Platform selection and configuration
├── pl_hw_config.c      # Platform-specific implementations
├── pl_hw_config.h      # Hardware abstraction layer interface
├── Makefile            # Build system
└── README.md           # This file
```

## Porting to New Platforms

To port this application to a new platform:

1. Add platform definition in `pl_config.h`
2. Implement platform-specific functions in `pl_hw_config.c`:
   - `pl_hw_init()`: Initialize hardware
   - `pl_i2c_init()`, `pl_i2c_write()`, `pl_i2c_read()`: I2C operations
   - `pl_udp_init()`, `pl_udp_send()`, `pl_udp_close()`: UDP operations
   - `pl_delay_ms()`, `pl_get_tick_ms()`: Timing functions
3. Update Makefile with platform-specific compiler and linker settings
4. No changes needed to `main.c`, `sht3x.c`, or `udp_sender.c`

## Troubleshooting

### I2C Communication Errors
- Verify I2C device path (Linux: `/dev/i2c-1`)
- Check sensor connections and power supply
- Verify I2C address (0x44 or 0x45 depending on ADDR pin)
- Test I2C with `i2cdetect -y -r 1` on Linux

### UDP Transmission Issues
- Verify network connectivity
- Check firewall settings on receiving computer
- Ensure correct IP address and port
- Use `tcpdump` or Wireshark to capture UDP packets

### Build Errors
- Ensure correct toolchain is installed
- For iMXRT1050: Configure FreeRTOS and SDK paths in Makefile
- Check platform selection in `pl_config.h` matches Makefile

## License

This project is provided as-is for educational and development purposes.

## Author

BeagleBone and iMXRT1050-EVKB cross-platform implementation
