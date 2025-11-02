#!/bin/bash

echo "================================"
echo "3D Game Engine - Headless Mode"
echo "================================"
echo ""

# Check if executable exists
if [ ! -f build/bin/GameEngine ]; then
    echo "ERROR: GameEngine executable not found!"
    echo "Please run build.sh first to compile the project."
    exit 1
fi

# Check if Xvfb is installed
if ! command -v Xvfb &> /dev/null; then
    echo "ERROR: Xvfb is not installed"
    echo ""
    echo "Xvfb (X Virtual Frame Buffer) is required for headless operation."
    echo "Install it with:"
    echo "  sudo apt-get install xvfb"
    echo ""
    exit 1
fi

# Start Xvfb if not already running
if ! pgrep -x "Xvfb" > /dev/null; then
    echo "Starting Xvfb virtual display..."
    Xvfb :99 -screen 0 1024x768x24 > /dev/null 2>&1 &
    XVFB_PID=$!
    sleep 2
    echo "Xvfb started on display :99 (PID: $XVFB_PID)"
else
    echo "Xvfb is already running"
    XVFB_PID=$(pgrep -x "Xvfb")
fi

# Export DISPLAY
export DISPLAY=:99
echo "DISPLAY set to :99"

# Verify OpenGL is available
echo ""
echo "Checking OpenGL availability..."
if command -v glxinfo &> /dev/null; then
    GL_VERSION=$(glxinfo 2>/dev/null | grep "OpenGL version" || echo "Unable to query OpenGL version")
    echo "$GL_VERSION"
else
    echo "glxinfo not found. Install mesa-utils to verify OpenGL support:"
    echo "  sudo apt-get install mesa-utils"
fi

echo ""
echo "Starting Game Engine in headless mode..."
echo ""

# Run the game engine
./build/bin/GameEngine

# Exit status
EXIT_CODE=$?

echo ""
if [ $EXIT_CODE -eq 0 ]; then
    echo "Game Engine closed successfully."
else
    echo "Game Engine exited with code: $EXIT_CODE"
fi

# Note: We don't kill Xvfb here as other processes might be using it
# Users can manually kill it with: pkill Xvfb
