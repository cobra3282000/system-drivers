#!/bin/bash
# Test script to verify pacman installation works

echo "=== System Drivers Installation Test ==="
echo ""
echo "This will test if the program correctly calls pacman."
echo "The program will be run with root privileges."
echo ""
echo "When the GUI opens:"
echo "1. Click on an Install/Reinstall button next to a driver"
echo "2. Watch the terminal for pacman output"
echo "3. Watch the status bar at the bottom of the window"
echo ""
echo "Starting System Drivers..."
echo ""

sudo ./bin/system-drivers
