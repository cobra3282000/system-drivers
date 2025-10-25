/*
 * Driver detection and installation header
 */

#ifndef DRIVER_H
#define DRIVER_H

#include <stdbool.h>
#include "hardware.h"

// Driver info structure
typedef struct {
    char name[128];
    char package[128];
    char version[32];
    char description[256];
    HardwareType hw_type;
    bool is_installed;
    bool is_recommended;
    bool needs_reboot;
} DriverInfo;

// Detect available drivers for hardware
int detect_drivers(HardwareInfo *hw_list, int hw_count, DriverInfo **driver_list);

// Install a driver
bool install_driver(DriverInfo *driver);

// Check if driver is installed
bool is_driver_installed(const char *package_name);

// Free driver list
void free_driver_list(DriverInfo *driver_list, int count);

#endif // DRIVER_H
