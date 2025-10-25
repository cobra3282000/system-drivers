# Debugging Error 256

## What Error 256 Means

Error 256 from `system()` = Exit code 1 from pacman

This means pacman failed. Common reasons:

## Fixed in This Version

The program now:

1. **Shows detailed output** - You'll see exactly what pacman says
2. **Syncs database first** - Runs `pacman -Sy` before installing
3. **Shows root status** - Confirms UID=0
4. **Decodes exit codes** - Shows actual pacman exit status
5. **Removes 2>&1** - So you see real pacman output in terminal

## What You'll See Now

```
=== Installing Driver ===
Driver: NVIDIA Proprietary Driver
Package: nvidia
========================

Root privileges confirmed (UID: 0)

Syncing package database...
:: Synchronizing package databases...
[database sync output]

Executing: pacman -S --noconfirm --needed nvidia
-----------------------------------
resolving dependencies...
looking for conflicting packages...

Packages (1) nvidia-550.120-1

Total Download Size:   62.50 MiB
Total Installed Size:  189.23 MiB

:: Proceeding with installation...
[installation output]
-----------------------------------
Command exit code: 0

✓ Successfully installed nvidia
```

## If It Still Fails

You'll see detailed error info:

```
-----------------------------------
Command exit code: 256

✗ Failed to install nvidia
System return code: 256
Actual exit status: 1

Possible reasons:
  - Package 'nvidia' not found in repositories
  - Network connection issue
  - Package database out of sync
  - Conflicting packages

Try manually: sudo pacman -S nvidia
```

## Manual Testing

If you get error 256, try these commands manually:

### 1. Test if package exists
```bash
pacman -Ss nvidia
```

### 2. Sync database
```bash
sudo pacman -Sy
```

### 3. Try installing manually
```bash
sudo pacman -S nvidia
```

### 4. Check for conflicts
```bash
pacman -Qi nvidia
```

## Common Causes of Error 256

### Package doesn't exist
```bash
# Wrong:
pacman -S nvidi    # Typo

# Right:
pacman -S nvidia
```

**Solution:** Check the package name in our driver database (src/driver.c)

### Database out of sync
```bash
# Update database
sudo pacman -Sy
```

**Solution:** Program now does this automatically!

### Network issue
```bash
# Test connection
ping archlinux.org
```

**Solution:** Check your internet connection

### No mirrors configured
```bash
# Check mirrors
cat /etc/pacman.d/mirrorlist
```

**Solution:** Configure pacman mirrors

### Package conflicts
```bash
# Check what's blocking
sudo pacman -S nvidia
```

**Solution:** Resolve conflicts manually, then try again

## Testing the Fix

Run the program and watch the terminal:

```bash
sudo ./bin/system-drivers
```

Click "Install" on any driver and watch the terminal output. You'll now see:

✓ Root privilege confirmation
✓ Database sync
✓ Full pacman output
✓ Detailed error messages if it fails
✓ Actual exit codes decoded

## Still Getting Error 256?

Copy the terminal output and:

1. Check what pacman actually said
2. Look for "error:" messages from pacman
3. Try the manual command shown in the error
4. Check `/var/log/pacman.log` for more details

```bash
sudo tail -50 /var/log/pacman.log
```
