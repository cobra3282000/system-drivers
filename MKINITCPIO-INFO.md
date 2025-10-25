# Automatic Kernel Initramfs Rebuild

## What Was Added

The program now automatically runs `mkinitcpio -P` after installing drivers that need a reboot (kernel modules).

## Why This Is Important

### What is mkinitcpio?

**mkinitcpio** = Make Initial RAM Filesystem

- Builds the initial RAM disk image
- Includes kernel modules needed at boot
- Required for new drivers to work properly
- Must be rebuilt when kernel modules change

### Without mkinitcpio:

```
1. Install nvidia-dkms
2. Reboot
3. Driver not loaded (missing from initramfs)
4. Black screen or no GPU acceleration
5. Must manually run: sudo mkinitcpio -P
6. Reboot again
```

### With mkinitcpio (automatic):

```
1. Install nvidia-dkms
2. mkinitcpio -P runs automatically ✓
3. Initramfs rebuilt with new driver ✓
4. Reboot
5. Driver loads properly ✓
```

## When It Runs

The program runs `mkinitcpio -P` only when:
- Driver installation succeeds
- Driver has `needs_reboot = true` (kernel modules)

### Drivers that trigger mkinitcpio:
✅ NVIDIA Complete Driver (nvidia-dkms)
✅ NVIDIA Standard Driver (nvidia)
✅ NVIDIA LTS Driver (nvidia-lts)
✅ AMD AMDGPU Driver (xf86-video-amdgpu)
✅ Intel Graphics Driver (xf86-video-intel)

### Drivers that DON'T trigger mkinitcpio:
❌ nvidia-settings (just a GUI tool)
❌ vulkan drivers (userspace libraries)
❌ mesa (userspace libraries)
❌ firmware packages (loaded by kernel)

## What You'll See

### Complete Installation Output:

```
=== Installing Driver ===
Driver: NVIDIA Complete Driver
Package: nvidia-dkms lib32-nvidia-utils nvidia-settings
========================

Root privileges confirmed (UID: 0)

Syncing package database...
:: Synchronizing package databases...

Executing: pacman -S --noconfirm --needed --overwrite '*' nvidia-dkms lib32-nvidia-utils nvidia-settings
-----------------------------------
resolving dependencies...
looking for conflicting packages...

Packages (3) nvidia-dkms-550.120-1 ...

[downloads and installs]
-----------------------------------
Command exit code: 0

✓ Successfully installed nvidia-dkms lib32-nvidia-utils nvidia-settings

=== Rebuilding Kernel Initramfs ===
Running: mkinitcpio -P
This may take a minute...
-----------------------------------
==> Building image from preset: /etc/mkinitcpio.d/linux.preset: 'default'
  -> -k /boot/vmlinuz-linux -c /etc/mkinitcpio.conf -g /boot/initramfs-linux.img
==> Starting build: 6.5.9-arch2-1
  -> Running build hook: [base]
  -> Running build hook: [udev]
  -> Running build hook: [autodetect]
  -> Running build hook: [modconf]
  -> Running build hook: [kms]
  -> Running build hook: [keyboard]
  -> Running build hook: [keymap]
  -> Running build hook: [consolefont]
  -> Running build hook: [block]
  -> Running build hook: [filesystems]
  -> Running build hook: [fsck]
==> Generating module dependencies
==> Creating gzip-compressed initcpio image: /boot/initramfs-linux.img
==> Image generation successful

==> Building image from preset: /etc/mkinitcpio.d/linux.preset: 'fallback'
  -> -k /boot/vmlinuz-linux -c /etc/mkinitcpio.conf -g /boot/initramfs-linux-fallback.img -S autodetect
[fallback image builds]
==> Image generation successful
-----------------------------------
✓ Kernel initramfs rebuilt successfully

[Reboot dialog appears]
```

## Status Bar Updates

1. "Installing <driver>..."
2. "Successfully installed <driver>!"
3. "Rebuilding initramfs..." (GUI continues to respond)
4. "Ready."

## If mkinitcpio Fails

```
⚠ Warning: mkinitcpio failed (exit code: 256)
You may need to run manually: sudo mkinitcpio -P
```

**Installation still succeeds**, but you'll need to run mkinitcpio manually:

```bash
sudo mkinitcpio -P
```

**Common reasons for failure:**
- Missing kernel headers
- Disk full
- Corrupted initramfs config
- Missing dependencies

## Manual Verification

After installation and reboot, verify:

```bash
# Check if driver module is in initramfs
lsinitcpio /boot/initramfs-linux.img | grep nvidia

# Should show:
# usr/lib/modules/6.5.9-arch2-1/kernel/drivers/video/nvidia.ko.xz
```

## The Complete Flow

```
┌─────────────────────────────────────────────┐
│ User clicks "Install" on NVIDIA driver      │
└─────────────────┬───────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────┐
│ GUI: Confirmation dialog                    │
└─────────────────┬───────────────────────────┘
                  │ [Yes]
                  ▼
┌─────────────────────────────────────────────┐
│ Status: "Installing..."                     │
│ Terminal: pacman -Sy                        │
│ Terminal: pacman -S --overwrite '*' nvidia  │
└─────────────────┬───────────────────────────┘
                  │ [Success]
                  ▼
┌─────────────────────────────────────────────┐
│ Status: "Rebuilding initramfs..."           │
│ Terminal: mkinitcpio -P                     │
│ Builds: /boot/initramfs-linux.img           │
│ Builds: /boot/initramfs-linux-fallback.img  │
└─────────────────┬───────────────────────────┘
                  │ [Success]
                  ▼
┌─────────────────────────────────────────────┐
│ Status: "Successfully installed!"           │
│ Dialog: "Reboot required"                   │
└─────────────────┬───────────────────────────┘
                  │ [Reboot]
                  ▼
┌─────────────────────────────────────────────┐
│ System reboots with new driver in initramfs│
│ Driver loads properly at boot               │
│ GPU works! ✓                                │
└─────────────────────────────────────────────┘
```

## Comparison with Manual Installation

### Without System Drivers:
```bash
sudo pacman -S nvidia-dkms lib32-nvidia-utils nvidia-settings
sudo mkinitcpio -P
sudo reboot
```
**3 commands** + manual reboot

### With System Drivers:
```bash
sudo ./bin/system-drivers
# Click Install
# Click Reboot
```
**1 command** + GUI clicks

Everything is automated! ✨

## Summary

✅ **Automatic**: mkinitcpio -P runs after driver install
✅ **Smart**: Only runs for kernel module drivers
✅ **Safe**: Doesn't fail installation if mkinitcpio fails
✅ **Visible**: Shows full output in terminal
✅ **Complete**: Rebuilds all kernel presets

The program now handles the complete driver installation workflow including initramfs rebuild! 🎉
