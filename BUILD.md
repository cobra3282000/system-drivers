# Building System Drivers

## Quick Start

### Prerequisites

Install required dependencies on Arch Linux:

```bash
sudo pacman -S base-devel gtk3 pkg-config
```

### Build

```bash
# Clone or navigate to the repository
cd system-drivers

# Build the application
make

# The executable will be created at: bin/system-drivers
```

### Install

```bash
# Install system-wide (requires root)
sudo make install

# The application will be installed to /usr/local/bin/
# A desktop entry will be created for your application menu
```

### Run

After installation, you can run the application in several ways:

1. **From Application Menu**: Search for "System Drivers" in your desktop environment's application launcher

2. **From Terminal**:
   ```bash
   system-drivers
   ```

3. **During Development** (without installing):
   ```bash
   make run
   ```

### Uninstall

```bash
sudo make uninstall
```

## Build Targets

- `make` or `make all` - Build the application
- `make install` - Install the application
- `make uninstall` - Remove the application
- `make clean` - Remove build files
- `make run` - Build and run (for testing)
- `make help` - Show available targets

## Project Structure

```
system-drivers/
├── src/                  # Source files
│   ├── main.c           # Main entry point with privilege check
│   ├── gui.c            # GTK GUI implementation
│   ├── hardware.c       # Hardware detection (lspci)
│   └── driver.c         # Driver detection and installation
├── include/             # Header files
│   ├── gui.h
│   ├── hardware.h
│   ├── driver.h
│   └── privilege.h
├── build/               # Build artifacts (created during build)
├── bin/                 # Compiled executables (created during build)
├── Makefile            # Build configuration
├── README.md           # Project documentation
└── BUILD.md            # This file
```

## Development

### Compiling Individual Files

The Makefile handles dependencies automatically, but you can also compile individual files:

```bash
# Compile main.c
gcc -Wall -Wextra -O2 -std=c11 `pkg-config --cflags gtk+-3.0` -c src/main.c -o build/main.o

# Link all objects
gcc build/*.o -o bin/system-drivers `pkg-config --libs gtk+-3.0`
```

### Debugging

Build with debug symbols:

```bash
# Modify CFLAGS in Makefile or add -g flag
gcc -g -Wall src/*.c -o bin/system-drivers `pkg-config --cflags --libs gtk+-3.0`

# Run with gdb
sudo gdb bin/system-drivers
```

### Testing Hardware Detection

Test the hardware scanner without GUI:

```bash
# View detected hardware
lspci | grep -E "VGA|Network|Audio"
```

## Troubleshooting

### Build Errors

**Error: gtk/gtk.h: No such file or directory**
```bash
# Install GTK3 development files
sudo pacman -S gtk3
```

**Error: pkg-config not found**
```bash
# Install pkg-config
sudo pacman -S pkg-config
```

### Runtime Issues

**Error: Could not escalate privileges**
- Ensure `pkexec` or `sudo` is installed
- Run directly with: `sudo ./bin/system-drivers`

**No drivers detected**
- Verify `lspci` is installed: `sudo pacman -S pciutils`
- Check hardware detection: `lspci`

**Installation fails**
- Ensure internet connection is active
- Update package database: `sudo pacman -Sy`
- Check pacman logs: `/var/log/pacman.log`
