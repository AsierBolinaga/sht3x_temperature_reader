# Example configuration for BeagleBone Platform
# Edit pl_config.h to use these settings

## Hardware Configuration

### I2C Connection (I2C-1)
- SDA: Pin P9_18
- SCL: Pin P9_17  
- VDD: 3.3V (Pin P9_3)
- GND: Ground (Pin P9_1)

### I2C Device
- Device: /dev/i2c-1
- SHT3x Address: 0x44 (default, ADDR pin to GND) or 0x45 (ADDR pin to VDD)

## Network Configuration

### UDP Settings
- Default Destination IP: 192.168.1.100
- Default Port: 8888
- Format: JSON

### Example UDP Message
```json
{"temperature":23.45,"humidity":45.67}
```

## Application Settings

### Timing
- Temperature Read Interval: 1000 ms (1 second)
- I2C Timeout: Default kernel setting
- UDP Send Timeout: Non-blocking

## Platform Selection in pl_config.h
```c
#define PLATFORM_BEAGLEBONE     /* BeagleBone with Linux */
/* #define PLATFORM_IMXRT1050 */
```

## Build Command
```bash
make PLATFORM=BEAGLEBONE
```

## Run Command
```bash
./sht3x_temperature_reader [IP_ADDRESS] [PORT]
```

## Troubleshooting

### Enable I2C on BeagleBone
```bash
# Check if I2C is enabled
ls /dev/i2c-*

# If not found, edit /boot/uEnv.txt and uncomment:
# dtb_overlay=/lib/firmware/BB-I2C1-00A0.dtbo
# Then reboot
```

### Detect SHT3x Sensor
```bash
# Install i2c-tools if not already installed
sudo apt-get install i2c-tools

# Scan I2C bus
i2cdetect -y -r 1

# You should see 44 or 45 in the output grid
```

### Test UDP Reception
```bash
# On receiving computer
nc -lu 8888
```
