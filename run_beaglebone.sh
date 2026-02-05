#!/bin/bash
# Example script to run the SHT3x Temperature Reader on BeagleBone
# 
# Usage:
#   ./run_beaglebone.sh [UDP_IP] [UDP_PORT]
#
# Defaults:
#   UDP_IP: 192.168.1.100
#   UDP_PORT: 8888

# Default values
UDP_IP=${1:-"192.168.1.100"}
UDP_PORT=${2:-"8888"}

echo "================================"
echo "SHT3x Temperature Reader"
echo "================================"
echo "Platform: BeagleBone (Linux)"
echo "UDP Destination: $UDP_IP:$UDP_PORT"
echo ""

# Check if I2C device exists
if [ ! -e /dev/i2c-1 ]; then
    echo "ERROR: I2C device /dev/i2c-1 not found!"
    echo "Please ensure I2C is enabled on your BeagleBone."
    echo ""
    echo "To enable I2C on BeagleBone:"
    echo "  1. Edit /boot/uEnv.txt"
    echo "  2. Uncomment the line: dtb_overlay=/lib/firmware/BB-I2C1-00A0.dtbo"
    echo "  3. Reboot the BeagleBone"
    exit 1
fi

# Check if the binary exists
if [ ! -f ./sht3x_temperature_reader ]; then
    echo "ERROR: Binary not found. Building..."
    make PLATFORM=BEAGLEBONE
    if [ $? -ne 0 ]; then
        echo "Build failed!"
        exit 1
    fi
fi

# Run the application
echo "Starting temperature reader..."
echo "Press Ctrl+C to stop"
echo ""

./sht3x_temperature_reader "$UDP_IP" "$UDP_PORT"
