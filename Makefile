# Makefile for SHT3x Temperature Reader
# Supports both BeagleBone (Linux) and iMXRT1050 (FreeRTOS)

# Project name
PROJECT = sht3x_temperature_reader

# Source files
SRCS = main.c sht3x.c udp_sender.c pl_hw_config.c

# Object files
OBJS = $(SRCS:.c=.o)

# Platform selection (default: BeagleBone)
# Set PLATFORM=IMXRT1050 for iMXRT1050 build
PLATFORM ?= BEAGLEBONE

# Compiler settings
ifeq ($(PLATFORM),BEAGLEBONE)
    CC = gcc
    CFLAGS = -Wall -Wextra -O2 -DPLATFORM_BEAGLEBONE
    LDFLAGS = -lrt
else ifeq ($(PLATFORM),IMXRT1050)
    # For iMXRT1050, you would typically use arm-none-eabi-gcc
    CC = arm-none-eabi-gcc
    CFLAGS = -Wall -Wextra -O2 -DPLATFORM_IMXRT1050
    CFLAGS += -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16
    # Add FreeRTOS and SDK include paths here
    # CFLAGS += -I/path/to/freertos/include
    # CFLAGS += -I/path/to/sdk/devices/MIMXRT1052/drivers
    LDFLAGS = 
    # Add linker script and libraries here
    # LDFLAGS += -T MIMXRT1052xxxxx_flexspi_nor.ld
else
    $(error Unknown PLATFORM: $(PLATFORM). Use BEAGLEBONE or IMXRT1050)
endif

# Default target
all: $(PROJECT)

# Build executable
$(PROJECT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Build complete for platform: $(PLATFORM)"

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(PROJECT)
	@echo "Clean complete"

# Install (for BeagleBone)
install: $(PROJECT)
ifeq ($(PLATFORM),BEAGLEBONE)
	install -m 755 $(PROJECT) /usr/local/bin/
	@echo "Installed to /usr/local/bin/"
else
	@echo "Install target only supported for BeagleBone platform"
endif

# Help target
help:
	@echo "SHT3x Temperature Reader Build System"
	@echo "======================================"
	@echo ""
	@echo "Targets:"
	@echo "  all      - Build the project (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  install  - Install binary (BeagleBone only)"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Platform Selection:"
	@echo "  make PLATFORM=BEAGLEBONE  - Build for BeagleBone (default)"
	@echo "  make PLATFORM=IMXRT1050   - Build for iMXRT1050-EVKB"
	@echo ""
	@echo "Examples:"
	@echo "  make                       - Build for BeagleBone"
	@echo "  make PLATFORM=IMXRT1050    - Build for iMXRT1050"
	@echo "  make clean                 - Clean build files"

.PHONY: all clean install help
