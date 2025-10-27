# AppImage Build and Auto-Update Guide

This document explains how to build Unimalen as an AppImage with auto-update functionality.

## Building the AppImage

### Prerequisites

- Qt6 development libraries
- wget (for downloading linuxdeploy tools)
- Internet connection (first build only)

### Build Steps

1. Run the build script:
```bash
./build-appimage.sh
```

This will:
- Build the Unimalen binary
- Download linuxdeploy and linuxdeploy-plugin-qt (if not already present)
- Bundle all Qt dependencies
- Create `Unimalen-*.AppImage`

2. Run the AppImage:
```bash
./Unimalen-*.AppImage
```

## Enabling Auto-Updates

Auto-updates allow users to update the application from within the app using **Help → Check for Updates**.

### Setup for GitHub Releases

1. **Create a GitHub repository** for Unimalen (if not already done)

2. **Edit `build-appimage.sh`** and uncomment the UPDATE_INFORMATION line:
```bash
export UPDATE_INFORMATION="gh-releases-zsync|OWNER|REPO|latest|Unimalen-*-$(uname -m).AppImage.zsync"
```

Replace:
- `OWNER` with your GitHub username
- `REPO` with your repository name

Example:
```bash
export UPDATE_INFORMATION="gh-releases-zsync|johndoe|unimalen|latest|Unimalen-*-x86_64.AppImage.zsync"
```

3. **Rebuild the AppImage** with update information embedded:
```bash
./build-appimage.sh
```

4. **Create a GitHub Release**:
   - Go to your repository on GitHub
   - Click "Releases" → "Create a new release"
   - Tag version (e.g., `v1.0.0`)
   - Upload BOTH files:
     - `Unimalen-*.AppImage`
     - `Unimalen-*.AppImage.zsync`

5. **Test auto-update**:
   - Run the AppImage
   - Go to **Help → Check for Updates**
   - The app will check GitHub for newer versions

## How Auto-Update Works

1. Update information is embedded in the AppImage during build
2. When user clicks "Check for Updates", the app uses `appimageupdatetool`
3. The tool reads the embedded update information
4. It checks GitHub releases for newer versions using the .zsync file
5. If an update is available, it downloads only the changed parts (delta update)
6. The new AppImage replaces the old one
7. User restarts the app to use the new version

## Update Information Formats

### GitHub Releases (recommended)
```
gh-releases-zsync|owner|repo|latest|pattern
```

### Generic ZSync URL
```
zsync|https://example.com/path/to/file.AppImage.zsync
```

### Bintray (deprecated but still supported)
```
bintray-zsync|owner|repo|package|file
```

## Requirements for Users

For auto-update to work, users need:
- `appimageupdatetool` installed on their system
- Internet connection
- Write permissions to the AppImage file location

### Installing appimageupdatetool

```bash
# Download
wget https://github.com/AppImageCommunity/AppImageUpdate/releases/download/continuous/appimageupdatetool-x86_64.AppImage

# Make executable
chmod +x appimageupdatetool-x86_64.AppImage

# Move to PATH
sudo mv appimageupdatetool-x86_64.AppImage /usr/local/bin/appimageupdatetool
```

## Versioning

To release a new version:
1. Update version in `src/mainwindow.cpp` (About dialog)
2. Rebuild AppImage with `./build-appimage.sh`
3. Create new GitHub release with incremented tag (e.g., v1.0.1)
4. Upload both `.AppImage` and `.AppImage.zsync` files
5. Users will be notified of the update when they check

## Troubleshooting

### "appimageupdatetool not found"
- Install appimageupdatetool (see above)
- Or guide users to install it

### "Failed to check for updates"
- Verify UPDATE_INFORMATION is correctly set
- Check that .zsync file is uploaded to GitHub release
- Ensure GitHub release is public

### "Not running from AppImage"
- This message appears when running the binary directly
- Auto-update only works with the AppImage

## Benefits of AppImage + Auto-Update

- **Single file distribution** - no installation required
- **Works across Linux distros** - compatible with most modern Linux systems
- **Delta updates** - only downloads changes, not the entire file
- **Decentralized** - no central server needed
- **User-friendly** - updates happen from within the app
- **Portable** - users can run from anywhere
