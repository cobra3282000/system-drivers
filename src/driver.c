/*
 * Driver detection and installation implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/driver.h"
#include "../include/hardware.h"

// Driver database - maps hardware types to driver packages
typedef struct {
    HardwareType hw_type;
    const char *vendor_match;  // NULL means any vendor
    const char *package_name;
    const char *driver_name;
    const char *description;
    bool needs_reboot;
    bool is_recommended;
} DriverMapping;

static const DriverMapping driver_db[] = {
    // NVIDIA drivers - Complete stack with DKMS and 32-bit support
    {HW_GPU_NVIDIA, NULL, "nvidia-dkms lib32-nvidia-utils nvidia-settings", "NVIDIA Complete Driver",
     "NVIDIA driver with DKMS modules and 32-bit support", true, true},
    {HW_GPU_NVIDIA, NULL, "nvidia", "NVIDIA Standard Driver",
     "Standard NVIDIA proprietary graphics driver", true, false},
    {HW_GPU_NVIDIA, NULL, "nvidia-lts", "NVIDIA LTS Driver",
     "NVIDIA driver for LTS kernel", true, false},

    // AMD drivers
    {HW_GPU_AMD, NULL, "xf86-video-amdgpu", "AMDGPU Driver",
     "Open source AMD graphics driver", true, true},
    {HW_GPU_AMD, NULL, "vulkan-radeon", "AMD Vulkan Driver",
     "Vulkan support for AMD GPUs", false, true},
    {HW_GPU_AMD, NULL, "mesa", "Mesa 3D Graphics",
     "Open source 3D graphics library", false, true},

    // Intel drivers
    {HW_GPU_INTEL, NULL, "xf86-video-intel", "Intel Graphics Driver",
     "Intel integrated graphics driver", true, true},
    {HW_GPU_INTEL, NULL, "vulkan-intel", "Intel Vulkan Driver",
     "Vulkan support for Intel GPUs", false, true},
    {HW_GPU_INTEL, NULL, "mesa", "Mesa 3D Graphics",
     "Open source 3D graphics library", false, true},

    // Network drivers (common packages)
    {HW_NETWORK, NULL, "linux-firmware", "Linux Firmware",
     "Firmware files for Linux kernel drivers", false, true},

    // Audio drivers
    {HW_AUDIO, NULL, "sof-firmware", "Sound Open Firmware",
     "Firmware for modern audio hardware", false, true},
};

static const int driver_db_size = sizeof(driver_db) / sizeof(DriverMapping);

// Check if a package (or multiple packages) is installed
// For multiple packages separated by spaces, checks if ALL are installed
bool is_driver_installed(const char *package_name) {
    char packages[256];
    strncpy(packages, package_name, sizeof(packages) - 1);
    packages[sizeof(packages) - 1] = '\0';

    // Check each package (space-separated)
    char *token = strtok(packages, " ");
    while (token != NULL) {
        char command[256];
        snprintf(command, sizeof(command), "pacman -Q %s > /dev/null 2>&1", token);
        if (system(command) != 0) {
            // At least one package not installed
            return false;
        }
        token = strtok(NULL, " ");
    }

    // All packages are installed
    return true;
}

// Detect available drivers for hardware
int detect_drivers(HardwareInfo *hw_list, int hw_count, DriverInfo **driver_list) {
    int count = 0;
    int capacity = 20;

    *driver_list = malloc(sizeof(DriverInfo) * capacity);
    if (*driver_list == NULL) {
        return 0;
    }

    // Track which packages we've already added (avoid duplicates)
    char **added_packages = malloc(sizeof(char*) * capacity);
    if (added_packages == NULL) {
        free(*driver_list);
        *driver_list = NULL;
        return 0;
    }

    // Scan through all hardware
    for (int i = 0; i < hw_count; i++) {
        HardwareInfo *hw = &hw_list[i];

        // Find matching drivers in database
        for (int j = 0; j < driver_db_size; j++) {
            const DriverMapping *mapping = &driver_db[j];

            // Check if hardware type matches
            if (mapping->hw_type != hw->type) {
                continue;
            }

            // Check if vendor matches (if specified)
            if (mapping->vendor_match != NULL &&
                strstr(hw->vendor, mapping->vendor_match) == NULL) {
                continue;
            }

            // Check if already added
            bool already_added = false;
            for (int k = 0; k < count; k++) {
                if (strcmp(added_packages[k], mapping->package_name) == 0) {
                    already_added = true;
                    break;
                }
            }

            if (already_added) {
                continue;
            }

            // Resize arrays if needed
            if (count >= capacity) {
                capacity *= 2;
                DriverInfo *new_list = realloc(*driver_list, sizeof(DriverInfo) * capacity);
                char **new_packages = realloc(added_packages, sizeof(char*) * capacity);

                if (new_list == NULL || new_packages == NULL) {
                    free(*driver_list);
                    free(added_packages);
                    *driver_list = NULL;
                    return 0;
                }

                *driver_list = new_list;
                added_packages = new_packages;
            }

            // Create driver info
            DriverInfo driver;
            memset(&driver, 0, sizeof(DriverInfo));

            strncpy(driver.name, mapping->driver_name, sizeof(driver.name) - 1);
            strncpy(driver.package, mapping->package_name, sizeof(driver.package) - 1);
            strncpy(driver.description, mapping->description, sizeof(driver.description) - 1);
            driver.hw_type = mapping->hw_type;
            driver.needs_reboot = mapping->needs_reboot;
            driver.is_recommended = mapping->is_recommended;

            // Check if installed
            driver.is_installed = is_driver_installed(mapping->package_name);

            // Get version if installed
            if (driver.is_installed) {
                char command[256];
                snprintf(command, sizeof(command),
                        "pacman -Q %s 2>/dev/null | awk '{print $2}'",
                        mapping->package_name);

                FILE *fp = popen(command, "r");
                if (fp != NULL) {
                    if (fgets(driver.version, sizeof(driver.version), fp) != NULL) {
                        driver.version[strcspn(driver.version, "\n")] = 0;
                    }
                    pclose(fp);
                }
            } else {
                strncpy(driver.version, "Not installed", sizeof(driver.version) - 1);
            }

            (*driver_list)[count] = driver;
            added_packages[count] = (char*)mapping->package_name;
            count++;
        }
    }

    free(added_packages);

    printf("Driver detection complete: found %d drivers\n", count);

    return count;
}

// Install a driver
bool install_driver(DriverInfo *driver) {
    printf("\n=== Installing Driver ===\n");
    printf("Driver: %s\n", driver->name);
    printf("Package: %s\n", driver->package);
    printf("========================\n\n");

    // Verify we're running as root
    if (geteuid() != 0) {
        fprintf(stderr, "ERROR: Not running as root! Cannot install drivers.\n");
        fprintf(stderr, "Current UID: %d (should be 0)\n", geteuid());
        return false;
    }

    printf("Root privileges confirmed (UID: %d)\n", geteuid());

    // First, sync the database
    printf("\nSyncing package database...\n");
    fflush(stdout);
    int sync_result = system("pacman -Sy --noconfirm");
    if (sync_result != 0) {
        fprintf(stderr, "WARNING: Database sync failed (exit code: %d)\n", sync_result);
        fprintf(stderr, "Continuing anyway...\n");
    }

    // Build pacman install command with --overwrite to handle file conflicts
    char command[512];
    snprintf(command, sizeof(command),
            "pacman -S --noconfirm --needed --overwrite '*' %s",
            driver->package);

    printf("\nExecuting: %s\n", command);
    printf("-----------------------------------\n");
    fflush(stdout);

    int result = system(command);

    printf("-----------------------------------\n");
    printf("Command exit code: %d\n", result);

    if (result == 0) {
        driver->is_installed = true;
        printf("\n✓ Successfully installed %s\n", driver->package);

        // If this is a driver that needs reboot (kernel modules), rebuild initramfs
        if (driver->needs_reboot) {
            printf("\n=== Rebuilding Kernel Initramfs ===\n");
            printf("Running: mkinitcpio -P\n");
            printf("This may take a minute...\n");
            printf("-----------------------------------\n");
            fflush(stdout);

            int mkinit_result = system("mkinitcpio -P");

            printf("-----------------------------------\n");
            if (mkinit_result == 0) {
                printf("✓ Kernel initramfs rebuilt successfully\n\n");
            } else {
                fprintf(stderr, "⚠ Warning: mkinitcpio failed (exit code: %d)\n", mkinit_result);
                fprintf(stderr, "You may need to run manually: sudo mkinitcpio -P\n\n");
                // Don't fail the installation, just warn
            }
        }

        return true;
    } else {
        // Decode the exit code
        int actual_exit = WEXITSTATUS(result);
        fprintf(stderr, "\n✗ Failed to install %s\n", driver->package);
        fprintf(stderr, "System return code: %d\n", result);
        fprintf(stderr, "Actual exit status: %d\n", actual_exit);
        fprintf(stderr, "\nPossible reasons:\n");
        fprintf(stderr, "  - Package '%s' not found in repositories\n", driver->package);
        fprintf(stderr, "  - Network connection issue\n");
        fprintf(stderr, "  - Package database out of sync\n");
        fprintf(stderr, "  - Conflicting packages\n");
        fprintf(stderr, "\nTry manually: sudo pacman -S %s\n\n", driver->package);
        return false;
    }
}

// Free driver list
void free_driver_list(DriverInfo *driver_list, int count) {
    if (driver_list != NULL) {
        free(driver_list);
    }
}
