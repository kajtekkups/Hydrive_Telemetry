#ifndef GNSS_HPP
#define GNSS_HPP

#include <string.h>
#include <stdio.h>
#include <time.h>

#include "ubxlib.h"
#include "Pin_Defines.hpp"


static const uDeviceCfg_t gDeviceCfg = {
    .deviceType = U_DEVICE_TYPE_GNSS,
    .deviceCfg = {
        .cfgGnss = {
            .moduleType = U_GNSS_MODULE_TYPE_M8,
            .pinEnablePower = -1,   //no power enable
            .pinDataReady = -1
        },
    },
    .transportType = U_DEVICE_TRANSPORT_TYPE_UART,
    .transportCfg = {
        .cfgUart = {
            .uart = 2,
            .baudRate = 9600,
            .pinTxd = GNSS_TX,
            .pinRxd = GNSS_RX,
            .pinCts = -1,
            .pinRts = -1,
            .pPrefix = NULL // Relevant for Linux only
        },
    },
};

static const uNetworkCfgGnss_t gNetworkCfg = {
    .type = U_NETWORK_TYPE_GNSS,
    .moduleType = U_GNSS_MODULE_TYPE_M8,
    .devicePinPwr = -1,
    .devicePinDataReady = -1
};

void randommowa_funkcja()
{
    // Remove the line below if you want the log printouts from ubxlib
    // uPortLogOff();
    // Initiate ubxlib
    uPortInit();
    uDeviceInit();
    // And the U-blox GNSS module
    int32_t errorCode;
    uDeviceHandle_t deviceHandle;
    printf("\nInitiating the module...\n");
    errorCode = uDeviceOpen(&gDeviceCfg, &deviceHandle);
    if (errorCode == 0) {
        // Bring up the GNSS
        errorCode = uNetworkInterfaceUp(deviceHandle, U_NETWORK_TYPE_GNSS, &gNetworkCfg);
        if (errorCode == 0) {
            printf("Waiting for position.");
            uLocation_t location;
            int tries = 0;

            do {
                printf(".");
                errorCode = uLocationGet(deviceHandle, U_LOCATION_TYPE_GNSS,
                                         NULL, NULL, &location, NULL);
            } while (errorCode == U_ERROR_COMMON_TIMEOUT && tries++ < 4);

            if (errorCode == 0) {
                printf("Position: https://maps.google.com/?q=%d.%07d,%d.%07d\n",
                       location.latitudeX1e7 / 10000000, location.latitudeX1e7 % 10000000,
                       location.longitudeX1e7 / 10000000, location.longitudeX1e7 % 10000000);
                printf("Radius: %d m\n", location.radiusMillimetres / 1000);
                struct tm *t = gmtime((const long*)&location.timeUtc);
                printf("UTC Time: %4d-%02d-%02d %02d:%02d:%02d\n",
                       t->tm_year + 1900, t->tm_mon, t->tm_mday,
                       t->tm_hour, t->tm_min, t->tm_sec);
            } else if (errorCode == U_ERROR_COMMON_TIMEOUT) {
                printf("* Timeout\n");
            } else {
                printf("* Failed to get position: %d\n", errorCode);
            }
            uNetworkInterfaceDown(deviceHandle, U_NETWORK_TYPE_GNSS);
        } else {
            printf("* Failed to bring up the GNSS: %d", errorCode);
        }
        // Close the device
        if (uDeviceClose(deviceHandle, true) != 0) {
            // Device has not responded to power off request, just release resources
            uDeviceClose(deviceHandle, false);
        }
    } else {
        printf("* Failed to initiate the module: %d", errorCode);
    }

    printf("\n== All done ==\n");

}


#endif //GNSS_HPP