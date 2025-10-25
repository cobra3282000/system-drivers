# Makefile for System Drivers

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0`

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/system-drivers

# Source files
SOURCES = $(SRC_DIR)/main.c \
          $(SRC_DIR)/gui.c \
          $(SRC_DIR)/hardware.c \
          $(SRC_DIR)/driver.c

# Object files
OBJECTS = $(BUILD_DIR)/main.o \
          $(BUILD_DIR)/gui.o \
          $(BUILD_DIR)/hardware.o \
          $(BUILD_DIR)/driver.o

# Installation directories
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
DATADIR = $(PREFIX)/share
DESKTOPDIR = $(DATADIR)/applications
ICONDIR = $(DATADIR)/icons/hicolor/48x48/apps

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

# Compile source files
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c $(INCLUDE_DIR)/gui.h $(INCLUDE_DIR)/privilege.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/gui.o: $(SRC_DIR)/gui.c $(INCLUDE_DIR)/gui.h $(INCLUDE_DIR)/driver.h $(INCLUDE_DIR)/hardware.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/gui.c -o $(BUILD_DIR)/gui.o

$(BUILD_DIR)/hardware.o: $(SRC_DIR)/hardware.c $(INCLUDE_DIR)/hardware.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/hardware.c -o $(BUILD_DIR)/hardware.o

$(BUILD_DIR)/driver.o: $(SRC_DIR)/driver.c $(INCLUDE_DIR)/driver.h $(INCLUDE_DIR)/hardware.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/driver.c -o $(BUILD_DIR)/driver.o

# Install the application
install: $(TARGET)
	@echo "Installing System Drivers..."
	install -Dm755 $(TARGET) $(DESTDIR)$(BINDIR)/system-drivers
	@echo "Creating desktop entry..."
	@mkdir -p $(DESTDIR)$(DESKTOPDIR)
	@echo "[Desktop Entry]" > $(DESTDIR)$(DESKTOPDIR)/system-drivers.desktop
	@echo "Name=System Drivers" >> $(DESTDIR)$(DESKTOPDIR)/system-drivers.desktop
	@echo "Comment=Manage system drivers" >> $(DESTDIR)$(DESKTOPDIR)/system-drivers.desktop
	@echo "Exec=system-drivers" >> $(DESTDIR)$(DESKTOPDIR)/system-drivers.desktop
	@echo "Icon=system-drivers" >> $(DESTDIR)$(DESKTOPDIR)/system-drivers.desktop
	@echo "Terminal=false" >> $(DESTDIR)$(DESKTOPDIR)/system-drivers.desktop
	@echo "Type=Application" >> $(DESTDIR)$(DESKTOPDIR)/system-drivers.desktop
	@echo "Categories=System;Settings;" >> $(DESTDIR)$(DESKTOPDIR)/system-drivers.desktop
	@echo "Installation complete!"

# Uninstall the application
uninstall:
	@echo "Uninstalling System Drivers..."
	rm -f $(DESTDIR)$(BINDIR)/system-drivers
	rm -f $(DESTDIR)$(DESKTOPDIR)/system-drivers.desktop
	@echo "Uninstall complete!"

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Clean complete!"

# Run the application (for testing)
run: $(TARGET)
	@echo "Running System Drivers (requires root)..."
	@sudo $(TARGET)

# Help target
help:
	@echo "System Drivers Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all       - Build the application (default)"
	@echo "  install   - Install the application system-wide"
	@echo "  uninstall - Remove the application"
	@echo "  clean     - Remove build files"
	@echo "  run       - Build and run the application"
	@echo "  help      - Show this help message"
	@echo ""
	@echo "Example usage:"
	@echo "  make              # Build the application"
	@echo "  make install      # Install (may require sudo)"
	@echo "  sudo make install # Install with root privileges"
	@echo "  make clean        # Clean build files"

.PHONY: all directories install uninstall clean run help
