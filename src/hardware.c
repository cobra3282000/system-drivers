/*
 * Hardware detection implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/hardware.h"

// Parse lspci output line
static bool parse_pci_line(const char *line, HardwareInfo *hw) {
    // Example line: "01:00.0 VGA compatible controller: NVIDIA Corporation Device 1234"
    char *vga_pos = strstr(line, "VGA compatible controller:");
    char *network_pos = strstr(line, "Network controller:");
    char *ethernet_pos = strstr(line, "Ethernet controller:");
    char *audio_pos = strstr(line, "Audio device:");

    if (vga_pos) {
        vga_pos += strlen("VGA compatible controller:");
        while (*vga_pos == ' ') vga_pos++;

        // Determine vendor
        if (strstr(vga_pos, "NVIDIA") || strstr(vga_pos, "nVidia")) {
            hw->type = HW_GPU_NVIDIA;
            strncpy(hw->vendor, "NVIDIA", sizeof(hw->vendor) - 1);
        } else if (strstr(vga_pos, "AMD") || strstr(vga_pos, "ATI")) {
            hw->type = HW_GPU_AMD;
            strncpy(hw->vendor, "AMD", sizeof(hw->vendor) - 1);
        } else if (strstr(vga_pos, "Intel")) {
            hw->type = HW_GPU_INTEL;
            strncpy(hw->vendor, "Intel", sizeof(hw->vendor) - 1);
        } else {
            hw->type = HW_UNKNOWN;
            strncpy(hw->vendor, "Unknown", sizeof(hw->vendor) - 1);
        }

        strncpy(hw->device, vga_pos, sizeof(hw->device) - 1);
        // Trim newline
        hw->device[strcspn(hw->device, "\n")] = 0;

        return true;
    } else if (network_pos || ethernet_pos) {
        hw->type = HW_NETWORK;
        char *start = network_pos ? network_pos + strlen("Network controller:") :
                                   ethernet_pos + strlen("Ethernet controller:");
        while (*start == ' ') start++;

        strncpy(hw->device, start, sizeof(hw->device) - 1);
        hw->device[strcspn(hw->device, "\n")] = 0;

        // Extract vendor
        char *colon = strchr(start, ':');
        if (colon && colon - start < sizeof(hw->vendor)) {
            strncpy(hw->vendor, start, colon - start);
            hw->vendor[colon - start] = '\0';
        } else {
            strncpy(hw->vendor, "Unknown", sizeof(hw->vendor) - 1);
        }

        return true;
    } else if (audio_pos) {
        hw->type = HW_AUDIO;
        audio_pos += strlen("Audio device:");
        while (*audio_pos == ' ') audio_pos++;

        strncpy(hw->device, audio_pos, sizeof(hw->device) - 1);
        hw->device[strcspn(hw->device, "\n")] = 0;

        // Extract vendor
        char *colon = strchr(audio_pos, ':');
        if (colon && colon - audio_pos < sizeof(hw->vendor)) {
            strncpy(hw->vendor, audio_pos, colon - audio_pos);
            hw->vendor[colon - audio_pos] = '\0';
        } else {
            strncpy(hw->vendor, "Unknown", sizeof(hw->vendor) - 1);
        }

        return true;
    }

    return false;
}

// Scan system for hardware
int scan_hardware(HardwareInfo **hw_list) {
    FILE *fp;
    char line[512];
    int count = 0;
    int capacity = 10;

    *hw_list = malloc(sizeof(HardwareInfo) * capacity);
    if (*hw_list == NULL) {
        return 0;
    }

    // Run lspci command
    fp = popen("lspci", "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to run lspci command\n");
        free(*hw_list);
        *hw_list = NULL;
        return 0;
    }

    // Parse output
    while (fgets(line, sizeof(line), fp) != NULL) {
        HardwareInfo hw;
        memset(&hw, 0, sizeof(HardwareInfo));

        if (parse_pci_line(line, &hw)) {
            // Resize array if needed
            if (count >= capacity) {
                capacity *= 2;
                HardwareInfo *new_list = realloc(*hw_list, sizeof(HardwareInfo) * capacity);
                if (new_list == NULL) {
                    free_hardware_list(*hw_list, count);
                    *hw_list = NULL;
                    pclose(fp);
                    return 0;
                }
                *hw_list = new_list;
            }

            // Extract PCI ID from line
            char pci_id[32] = {0};
            if (sscanf(line, "%31s", pci_id) == 1) {
                strncpy(hw.pci_id, pci_id, sizeof(hw.pci_id) - 1);
            }

            (*hw_list)[count++] = hw;
        }
    }

    pclose(fp);

    printf("Hardware scan complete: found %d devices\n", count);

    return count;
}

// Free hardware list
void free_hardware_list(HardwareInfo *hw_list, int count) {
    if (hw_list != NULL) {
        free(hw_list);
    }
}
