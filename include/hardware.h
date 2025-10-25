/*
 * Hardware detection header
 */

#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdbool.h>

// Hardware types
typedef enum {
    HW_GPU_NVIDIA,
    HW_GPU_AMD,
    HW_GPU_INTEL,
    HW_NETWORK,
    HW_AUDIO,
    HW_UNKNOWN
} HardwareType;

// Hardware info structure
typedef struct {
    HardwareType type;
    char vendor[128];
    char device[128];
    char pci_id[32];
} HardwareInfo;

// Scan system for hardware
int scan_hardware(HardwareInfo **hw_list);

// Free hardware list
void free_hardware_list(HardwareInfo *hw_list, int count);

#endif // HARDWARE_H
