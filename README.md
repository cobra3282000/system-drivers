# System Drivers

A lightweight desktop GUI driver management application for Arch Linux and Arch-based distributions (including Acreetinos OS). System Drivers automatically scans for available hardware drivers, detects updates, and provides a graphical interface for easy driver installation.

## Features

- **Automatic Driver Detection**: Scans system hardware and identifies available drivers
- **Video Driver Support**: Detects and manages graphics drivers (NVIDIA, AMD, Intel)
- **Graphical Interface**: Clean, user-friendly desktop application for driver selection
- **Smart Reboot Detection**: Automatically determines when a system reboot is required
- **Minimal Dependencies**: Written in C with GTK for performance and low overhead
- **Desktop Integration**: Native desktop application with system tray support

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

Launch the application from your desktop environment:

- **Application Menu**: Find "System Drivers" in your application launcher
- **Command Line**: Run `system-drivers` from terminal
- **Desktop**: Double-click the System Drivers desktop icon

The application will open a GUI window that:
1. Scans your system hardware automatically
2. Displays available and updated drivers in a list
3. Shows driver details and recommendations
4. Allows you to select drivers using checkboxes
5. Installs selected drivers with a progress indicator
6. Prompts for reboot if necessary after installation

### GUI Features

- **Driver List View**: Browse available drivers with status indicators
- **Details Panel**: View driver information, version, and compatibility
- **Search Filter**: Quick search to find specific drivers
- **Automatic Updates**: Option to check for driver updates on startup
- **System Tray Icon**: Minimize to system tray for background operation
- **Notifications**: Desktop notifications for driver availability

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
- GTK3 (graphical interface)
- polkit (for privilege escalation)

### Build Dependencies
- gcc
- make
- pkg-config
- gtk3-dev (GTK3 development libraries)
- libnotify-dev (desktop notifications)

## Architecture

System Drivers is built with modularity in mind:

- **Hardware Scanner**: Detects installed hardware components using hwinfo/lspci
- **Driver Database**: Maintains mappings between hardware and available drivers
- **Package Manager Interface**: Handles driver installation via pacman
- **Reboot Manager**: Determines when reboots are necessary
- **GTK GUI**: Native desktop interface built with GTK3
- **Polkit Integration**: Secure privilege escalation for driver installation without sudo prompts

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

- [x] Desktop GUI interface using GTK3
- [ ] Automatic background scanning service
- [ ] Driver backup and rollback functionality
- [ ] Support for AUR drivers
- [ ] Kernel module management
- [ ] Enhanced desktop notifications for driver updates
- [ ] Multi-language support
- [ ] Dark theme support
- [ ] Driver performance metrics

## Troubleshooting

### Driver not detected
- Click the "Refresh" button in the application to rescan hardware
- Manually check hardware: `sudo hwinfo --short`
- Check application logs from the Help menu or at `/var/log/system-drivers.log`

### Installation fails
- Ensure you have an active internet connection
- Update package databases: `sudo pacman -Sy`
- Check logs in the application's log viewer or `/var/log/system-drivers.log`
- Verify you granted permission when polkit prompted for authentication

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
