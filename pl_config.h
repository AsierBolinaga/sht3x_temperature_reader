#ifndef PL_CONFIG_H
#define PL_CONFIG_H

/* Platform Selection - Uncomment ONE of the following OR define via compiler flag */
#if !defined(PLATFORM_IMXRT1050) && !defined(PLATFORM_BEAGLEBONE)
    /* Default platform if not defined via compiler */
    /* #define PLATFORM_IMXRT1050 */  /* iMXRT1050-EVKB with FreeRTOS */
    #define PLATFORM_BEAGLEBONE     /* BeagleBone with Linux */
#endif

/* Platform-specific includes based on selection */
#ifdef PLATFORM_IMXRT1050
    #define USE_FREERTOS 1
    #define USE_LINUX 0
#elif defined(PLATFORM_BEAGLEBONE)
    #define USE_FREERTOS 0
    #define USE_LINUX 1
#else
    #error "No platform defined! Please define either PLATFORM_IMXRT1050 or PLATFORM_BEAGLEBONE"
#endif

/* Common configuration */
#define SHT3X_I2C_ADDRESS 0x44
#define UDP_PORT 8888
#define TEMPERATURE_READ_INTERVAL_MS 1000

#endif /* PL_CONFIG_H */
