#!/bin/bash

echo "================================"
echo "3D Game Engine - Running..."
echo "================================"
echo ""

# Check if executable exists
if [ ! -f build/bin/GameEngine ]; then
    echo "ERROR: GameEngine executable not found!"
    echo "Please run build.sh first to compile the project."
    exit 1
fi

# Run the game engine
echo "Starting Game Engine..."
echo ""
./build/bin/GameEngine

echo ""
echo "Game Engine closed."
