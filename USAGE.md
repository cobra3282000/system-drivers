# System Drivers - Usage Guide

## How It Works

System Drivers detects hardware and shows available drivers from the Arch repositories.

## Running the Program

**You MUST use sudo:**

```bash
sudo ./bin/system-drivers
```

Or let it auto-escalate:
```bash
./bin/system-drivers
```

## Button States

### "Install" Button (Green/Active)
- Driver is **NOT** installed
- Click to install the driver
- Uses: `pacman -S --noconfirm --needed <package>`
- Status bar shows progress
- Driver actually installs!

### "Installed" Button (Gray/Disabled)
- Driver is **already** installed
- Button is disabled (grayed out)
- Cannot be clicked
- Does nothing
- No reinstall option

## Example Session

```
$ sudo ./bin/system-drivers
System Drivers starting with root privileges...

[GUI opens showing drivers]

Driver List:
┌─────────────────────────────────────────────┐
│ NVIDIA Proprietary Driver (nvidia)         │
│ Latest NVIDIA proprietary graphics driver  │
│ Recommended                      [Install] │ ← Click this
├─────────────────────────────────────────────┤
│ NVIDIA Settings (nvidia-settings)          │
│ Configuration tool                         │
│                                 [Installed] │ ← Can't click
└─────────────────────────────────────────────┘

[Click Install on nvidia driver]

Confirmation Dialog:
  Install NVIDIA Proprietary Driver?
  Package: nvidia
  Latest NVIDIA proprietary graphics driver

  [Yes] [No]

[Click Yes]

Status Bar: "Installing NVIDIA Proprietary Driver..."

Terminal Output:
Installing driver: NVIDIA Proprietary Driver (nvidia)
Running: pacman -S --noconfirm --needed nvidia
Executing pacman command...
resolving dependencies...
looking for conflicting packages...

Packages (1) nvidia-550.120-1

Total Download Size:   62.50 MiB
Total Installed Size:  189.23 MiB

:: Downloading packages...
[download progress]
Successfully installed nvidia

Status Bar: "Successfully installed NVIDIA Proprietary Driver!"

[Success dialog appears]
[Driver list refreshes - button now shows "Installed"]
```

## What Actually Happens

1. Click **Install** button
2. Confirmation dialog appears
3. Click **Yes**
4. Status bar updates: "Installing..."
5. Program runs: `pacman -S --noconfirm --needed <package>`
6. Terminal shows full pacman output
7. Package downloads and installs
8. Status bar updates: "Successfully installed!"
9. Success dialog appears
10. List refreshes - button changes to "Installed"

## After Installation

If the driver needs a reboot:
- Dialog appears: "Driver installation requires a system reboot. Would you like to reboot now?"
- Click **Yes** to reboot immediately
- Click **No** to reboot later

## Supported Drivers

### GPU Drivers
- **NVIDIA**: nvidia, nvidia-lts, nvidia-settings
- **AMD**: xf86-video-amdgpu, vulkan-radeon, mesa
- **Intel**: xf86-video-intel, vulkan-intel, mesa

### Other
- **Network**: linux-firmware
- **Audio**: sof-firmware

## Troubleshooting

### "Not running as root!"
Run with: `sudo ./bin/system-drivers`

### Install button doesn't appear
Driver is already installed. Button shows "Installed" and is disabled.

### Want to reinstall a driver?
Use pacman directly:
```bash
sudo pacman -S nvidia
```

### Need to remove a driver?
Use pacman:
```bash
sudo pacman -R nvidia
```
Then refresh the driver list in System Drivers.

## Summary

- **Install** button = Driver not installed, click to install
- **Installed** button = Driver already installed, cannot click
- Must use sudo
- Actually installs drivers via pacman
- Status bar shows progress
- Terminal shows full output
