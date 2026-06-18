#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Get the script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Configuration
BOARD="arduino:avr:nano"
# Uncomment the line below instead if you ever need the old bootloader
# BOARD="arduino:avr:nano:cpu=atmega328old"

PORT="/dev/ttyUSB0"
SKETCH="$SCRIPT_DIR/test_display_tm1637.ino"

echo "========================================"
echo "🔨 Compiling $SKETCH..."
echo "========================================"
arduino-cli compile -b "$BOARD" "$SKETCH"

echo ""
echo "========================================"
echo "🚀 Uploading to $PORT..."
echo "========================================"
arduino-cli upload -p "$PORT" -b "$BOARD" "$SKETCH"

echo ""
echo "✅ Done! Code $SKETCH successfully flashed."