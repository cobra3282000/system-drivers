# System Drivers

A lightweight driver management utility for Arch Linux and Arch-based distributions (including Acreetinos OS). System Drivers automatically scans for available hardware drivers, detects updates, and provides a simple interface for installation.

## Features

- **Automatic Driver Detection**: Scans system hardware and identifies available drivers
- **Video Driver Support**: Detects and manages graphics drivers (NVIDIA, AMD, Intel)
- **Interactive Selection**: User-friendly interface to select which drivers to install
- **Smart Reboot Detection**: Automatically determines when a system reboot is required
- **Minimal Dependencies**: Written in C for performance and low overhead
- **Desktop Integration**: Designed to run seamlessly on desktop environments

## Supported Drivers

- Graphics/Video drivers (NVIDIA proprietary, AMD AMDGPU, Intel)
- Network drivers
- Audio drivers
- Other hardware-specific drivers available in Arch repositories

## Installation

### Prerequisites

- Arch Linux or Arch-based distribution (Acreetinos OS)
- GCC compiler
- Make
- Root/sudo privileges for driver installation

### Building from Source

```bash
git clone https://github.com/yourusername/system-drivers.git
cd system-drivers
make
sudo make install
```

## Usage

Run the application with root privileges:

```bash
sudo system-drivers
```

The application will:
1. Scan your system hardware
2. Check for available and updated drivers
3. Present a list of drivers with installation options
4. Allow you to select which drivers to install
5. Prompt for reboot if necessary after installation

### Command Line Options

```bash
sudo system-drivers [OPTIONS]

Options:
  -h, --help        Display this help message
  -s, --scan        Scan for drivers without installing
  -v, --verbose     Enable verbose output
  -y, --yes         Auto-confirm installations (use with caution)
```

## Configuration

Configuration file location: `/etc/system-drivers/config.conf`

Example configuration:
```ini
[general]
auto_scan=true
check_interval=weekly

[drivers]
include_proprietary=true
include_experimental=false

[notifications]
enable=true
```

## Requirements

### Runtime Dependencies
- pacman (package manager)
- hwinfo or lspci (hardware detection)
- systemd (for reboot management)

### Build Dependencies
- gcc
- make
- pkg-config
- gtk3-dev (for GUI support, optional)

## Architecture

System Drivers is built with modularity in mind:

- **Hardware Scanner**: Detects installed hardware components
- **Driver Database**: Maintains mappings between hardware and available drivers
- **Package Manager Interface**: Handles driver installation via pacman
- **Reboot Manager**: Determines when reboots are necessary
- **User Interface**: Interactive terminal or GUI for driver selection

## Development

### Project Structure

```
system-drivers/
├── src/           # Source code
├── include/       # Header files
├── lib/           # Libraries
├── tests/         # Unit tests
├── docs/          # Documentation
└── README.md      # This file
```

### Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Coding Standards

- Follow Linux kernel coding style
- Keep functions focused and modular
- Add comments for complex logic
- Write unit tests for new features

## Security

Driver installation requires root privileges. System Drivers:
- Validates all package sources
- Verifies package signatures
- Provides clear information about what will be installed
- Never auto-installs without user confirmation (unless --yes flag is used)

## License

This project is licensed under the GPL-3.0 License - see the LICENSE file for details.

## Acknowledgments

- Arch Linux community for excellent driver packaging
- Hardware detection tools: hwinfo, lspci
- Acreetinos OS team

## Roadmap

- [ ] GUI interface using GTK
- [ ] Automatic background scanning service
- [ ] Driver backup and rollback functionality
- [ ] Support for AUR drivers
- [ ] Kernel module management
- [ ] Desktop notifications for driver updates
- [ ] Multi-language support

## Troubleshooting

### Driver not detected
```bash
# Manually scan hardware
sudo hwinfo --short
# Run in verbose mode
sudo system-drivers --verbose
```

### Installation fails
- Ensure you have an active internet connection
- Update package databases: `sudo pacman -Sy`
- Check logs: `/var/log/system-drivers.log`

### System won't reboot
The application only prompts for reboot - it doesn't force it. Manually reboot:
```bash
sudo reboot
```

## Support

- Issue Tracker: https://github.com/yourusername/system-drivers/issues
- Documentation: https://github.com/yourusername/system-drivers/wiki
- Discussions: https://github.com/yourusername/system-drivers/discussions

## Authors

- Your Name - Initial work

## Changelog

See [CHANGELOG.md](CHANGELOG.md) for version history and updates.
