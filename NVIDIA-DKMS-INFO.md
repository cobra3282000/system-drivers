# NVIDIA DKMS Driver Installation

## What Gets Installed

When you click "Install" on the **NVIDIA Complete Driver**, the program will install:

### Complete NVIDIA Stack:
```bash
pacman -S --noconfirm --needed nvidia-dkms lib32-nvidia-utils nvidia-settings
```

### Package Breakdown:

1. **nvidia-dkms**
   - NVIDIA kernel modules with DKMS support
   - Automatically rebuilds driver when kernel updates
   - Works with multiple kernel versions
   - More flexible than standard nvidia package

2. **lib32-nvidia-utils**
   - 32-bit NVIDIA libraries
   - Required for 32-bit games and applications
   - Steam games compatibility
   - Wine/Proton support

3. **nvidia-settings**
   - NVIDIA control panel
   - GPU configuration tool
   - Monitor setup and management
   - Performance settings

## Why DKMS?

**DKMS (Dynamic Kernel Module Support)**

Benefits:
- Automatic driver rebuild on kernel updates
- No manual intervention needed
- Works with custom kernels
- Safer kernel upgrades

vs Standard nvidia package:
- Standard: Breaks on kernel updates, needs manual reinstall
- DKMS: Automatically rebuilds on kernel updates

## How It Shows in the GUI

```
┌─────────────────────────────────────────────────────────┐
│ NVIDIA Complete Driver                                  │
│ (nvidia-dkms lib32-nvidia-utils nvidia-settings)        │
│ NVIDIA driver with DKMS modules and 32-bit support      │
│ Recommended                                  [Install]  │
├─────────────────────────────────────────────────────────┤
│ NVIDIA Standard Driver                                  │
│ (nvidia)                                                │
│ Standard NVIDIA proprietary graphics driver             │
│                                              [Install]  │
├─────────────────────────────────────────────────────────┤
│ NVIDIA LTS Driver                                       │
│ (nvidia-lts)                                            │
│ NVIDIA driver for LTS kernel                            │
│                                              [Install]  │
└─────────────────────────────────────────────────────────┘
```

## Installation Process

When you click Install:

```
=== Installing Driver ===
Driver: NVIDIA Complete Driver
Package: nvidia-dkms lib32-nvidia-utils nvidia-settings
========================

Root privileges confirmed (UID: 0)

Syncing package database...
:: Synchronizing package databases...

Executing: pacman -S --noconfirm --needed nvidia-dkms lib32-nvidia-utils nvidia-settings
-----------------------------------
resolving dependencies...
looking for conflicting packages...

Packages (3):
  nvidia-dkms-550.120-1
  lib32-nvidia-utils-550.120-1
  nvidia-settings-550.120-1

Total Download Size:   92.30 MiB
Total Installed Size:  245.67 MiB

:: Proceeding with installation...
[downloads and installs all 3 packages]
-----------------------------------

✓ Successfully installed nvidia-dkms lib32-nvidia-utils nvidia-settings
```

## Installation Detection

The program checks if **ALL** packages are installed:

- If all 3 packages installed → Button shows "Installed" (disabled)
- If any package missing → Button shows "Install" (clickable)

## After Installation

1. **Reboot Required**
   - Dialog will appear: "Would you like to reboot now?"
   - Reboot is necessary for driver to load

2. **Verify Installation**
   ```bash
   # Check NVIDIA driver loaded
   nvidia-smi

   # Check kernel modules
   lsmod | grep nvidia

   # Should see:
   # nvidia_drm, nvidia_modeset, nvidia_uvm, nvidia
   ```

3. **Open NVIDIA Settings**
   ```bash
   nvidia-settings
   ```

## Alternative Options

If you don't want the complete stack:

### Option 1: Standard Driver (no DKMS)
- Select "NVIDIA Standard Driver"
- Only installs: `nvidia`
- Smaller download
- Manual rebuild needed after kernel updates

### Option 2: LTS Driver
- Select "NVIDIA LTS Driver"
- Only installs: `nvidia-lts`
- For LTS kernel users

## Troubleshooting

### DKMS build fails
```bash
# Check DKMS status
dkms status

# Rebuild manually
sudo dkms install nvidia-dkms/550.120
```

### Missing 32-bit libraries
If you only installed nvidia-dkms without lib32-nvidia-utils:
```bash
sudo pacman -S lib32-nvidia-utils
```

### nvidia-settings not working
```bash
# Reinstall
sudo pacman -S nvidia-settings
```

## Manual Installation

If you prefer to install manually:

```bash
# Complete stack (recommended)
sudo pacman -S nvidia-dkms lib32-nvidia-utils nvidia-settings

# Minimal (DKMS only)
sudo pacman -S nvidia-dkms

# Standard (no DKMS)
sudo pacman -S nvidia
```

## Summary

✅ **Recommended**: NVIDIA Complete Driver
- Installs: nvidia-dkms + lib32-nvidia-utils + nvidia-settings
- Best compatibility
- Automatic kernel updates
- 32-bit game support
- GUI configuration tool

The program installs all three packages together in a single pacman command!
