#!/bin/bash

# Build script for creating Unimalen AppImage with auto-update support

set -e

echo "Building Unimalen AppImage..."

# Build the application first
echo "Step 1: Building application..."
./build.sh

# Create AppDir structure
echo "Step 2: Creating AppDir structure..."
rm -rf AppDir
mkdir -p AppDir/usr/bin
mkdir -p AppDir/usr/share/applications
mkdir -p AppDir/usr/share/icons/hicolor/scalable/apps
mkdir -p AppDir/usr/share/unimalen/fonts

# Copy the binary
cp build/unimalen AppDir/usr/bin/

# Copy desktop file and icon
cp unimalen.desktop AppDir/usr/share/applications/
cp unimalen.svg AppDir/usr/share/icons/hicolor/scalable/apps/
cp unimalen.svg AppDir/

# Copy fonts
if [ -d "fonts" ]; then
    cp -r fonts/* AppDir/usr/share/unimalen/fonts/
fi

# Download linuxdeploy and linuxdeploy-plugin-qt if not present
echo "Step 3: Downloading linuxdeploy tools..."
if [ ! -f "linuxdeploy-$(uname -m).AppImage" ]; then
    wget -c "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-$(uname -m).AppImage"
    chmod +x linuxdeploy-$(uname -m).AppImage
fi

if [ ! -f "linuxdeploy-plugin-qt-$(uname -m).AppImage" ]; then
    wget -c "https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-$(uname -m).AppImage"
    chmod +x linuxdeploy-plugin-qt-$(uname -m).AppImage
fi

# Set environment variables for Qt plugin
export QT_SELECT=qt6
export QML_SOURCES_PATHS=$(pwd)/src

# Create AppImage
echo "Step 4: Creating AppImage..."
./linuxdeploy-$(uname -m).AppImage \
    --appdir AppDir \
    --executable AppDir/usr/bin/unimalen \
    --desktop-file unimalen.desktop \
    --icon-file unimalen.svg \
    --plugin qt \
    --output appimage

# Add update information (for GitHub releases)
# Format: gh-releases-zsync|owner|repo|latest|AppImage-*-x86_64.AppImage.zsync
# Example: gh-releases-zsync|yourusername|unimalen|latest|Unimalen-*-x86_64.AppImage.zsync
#
# To enable updates, uncomment and modify the following:
# export UPDATE_INFORMATION="gh-releases-zsync|yourusername|unimalen|latest|Unimalen-*-$(uname -m).AppImage.zsync"
#
# Then rebuild with:
# ./linuxdeploy-$(uname -m).AppImage \
#     --appdir AppDir \
#     --executable AppDir/usr/bin/unimalen \
#     --desktop-file unimalen.desktop \
#     --icon-file unimalen.svg \
#     --plugin qt \
#     --output appimage

echo "AppImage created successfully!"
echo "You can run it with: ./Unimalen-$(uname -m).AppImage"
echo ""
echo "To enable auto-updates:"
echo "1. Set up GitHub releases for your repository"
echo "2. Uncomment and configure UPDATE_INFORMATION in this script"
echo "3. Upload the .AppImage and .AppImage.zsync files to GitHub releases"
echo "4. The app will then be able to check for and download updates automatically"
