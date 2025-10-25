# Installation Guide - How System Drivers Works

## Root/Sudo Requirements

**IMPORTANT:** System Drivers MUST run with root privileges to install drivers using pacman.

## Two Ways to Run

### Option 1: Let the program handle sudo (Recommended)
```bash
./bin/system-drivers
```

**What happens:**
1. Program detects it's not running as root
2. Tries `pkexec` first (shows graphical password prompt)
3. Falls back to `sudo` if pkexec fails
4. You enter your password once
5. GUI opens with root privileges

### Option 2: Run with sudo directly
```bash
sudo ./bin/system-drivers
```

**What happens:**
1. You enter your password
2. Program starts with root privileges
3. GUI opens immediately

## How Driver Installation Works

### Installing New Drivers

When you click **Install** on a driver that is NOT installed:

```bash
pacman -S --noconfirm --needed <package>
```

**Flags explained:**
- `-S`: Sync/install package from repositories
- `--noconfirm`: Don't prompt (we already confirmed in GUI)
- `--needed`: Skip if package is already up-to-date
- Root privileges: **Required** - inherited from main program

**Example:**
```bash
pacman -S --noconfirm --needed nvidia
```

### Reinstalling Existing Drivers

When you click **Reinstall** on a driver that IS installed:

```bash
pacman -S --noconfirm <package>
```

**Flags explained:**
- `-S`: Sync/install package
- `--noconfirm`: Don't prompt
- **NO --needed flag**: Forces reinstall even if up-to-date
- Root privileges: **Required** - inherited from main program

**Example:**
```bash
pacman -S --noconfirm nvidia
```

This will:
- Download the package (even if same version)
- Reinstall/overwrite the existing installation
- Fix any corrupted driver files
- Reset driver configurations

## What You'll See During Installation

### In the GUI:
1. **Status bar updates** (bottom of window):
   - "Installing <driver name>..."
   - "Successfully installed <driver name>!"
   - Or: "Failed to install <driver name>"

### In the Terminal:
```
Installing driver: NVIDIA Proprietary Driver (nvidia)
Driver is installed, forcing reinstall...
Running: pacman -S --noconfirm nvidia
Executing pacman command...
[pacman downloads and installs package]
Successfully installed nvidia
```

## Root Privilege Verification

The program checks for root privileges in **two places**:

### 1. At Startup (main.c)
```c
if (geteuid() != 0) {
    // Try pkexec or sudo
}
```

### 2. Before Installation (driver.c)
```c
if (geteuid() != 0) {
    fprintf(stderr, "ERROR: Not running as root!\n");
    return false;
}
```

This ensures pacman will have the privileges it needs.

## Example Session

```bash
$ ./bin/system-drivers
System Drivers requires root privileges.
Attempting to escalate privileges...
[Password prompt appears]
System Drivers starting with root privileges...

[GUI opens]
[Click "Install" on NVIDIA driver]

Terminal output:
Installing driver: NVIDIA Proprietary Driver (nvidia)
Installing new driver...
Running: pacman -S --noconfirm --needed nvidia
Executing pacman command...
resolving dependencies...
looking for conflicting packages...

Packages (1) nvidia-550.120-1

Total Download Size:   62.50 MiB
Total Installed Size:  189.23 MiB

:: Proceed with installation? [Y/n]
[pacman proceeds automatically]
Successfully installed nvidia

[Success dialog appears]
[Status bar shows: "Successfully installed NVIDIA Proprietary Driver!"]
```

## Troubleshooting

### Problem: "Not running as root!"
**Solution:** Run with `sudo ./bin/system-drivers`

### Problem: pacman says "permission denied"
**Solution:** The program lost root privileges. Restart with sudo.

### Problem: Driver installs but doesn't work
**Solution:**
1. Check if reboot is needed (program will prompt)
2. Reboot your system
3. Run: `lsmod | grep nvidia` (or your driver name)

### Problem: "Reinstall" doesn't actually reinstall
**Solution:** This is now fixed! The program uses different commands:
- Install: `pacman -S --noconfirm --needed <pkg>`
- Reinstall: `pacman -S --noconfirm <pkg>` (no --needed flag)

## Testing the Installation

To verify pacman is actually being called:

```bash
# Run the program
sudo ./bin/system-drivers

# In another terminal, watch pacman activity:
sudo tail -f /var/log/pacman.log

# Or monitor network activity:
sudo nethogs

# You'll see pacman downloading and installing packages
```

## Summary

✅ **YES** - The program WILL install drivers using pacman
✅ **YES** - You MUST run with sudo (program handles this automatically)
✅ **YES** - It can install NEW drivers
✅ **YES** - It can REINSTALL existing drivers (overwrites them)
✅ **YES** - Status bar shows real-time progress
✅ **YES** - Root privileges are verified before installation

The system() call inherits the root privileges from the main program, so pacman will have the permissions it needs to install packages!
