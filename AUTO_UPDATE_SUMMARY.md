# Auto-Update Implementation Summary

This document summarizes the auto-update functionality added to Unimalen using AppImage and AppImageUpdate.

## What Was Added

### 1. User Interface Components

**UpdateDialog** (`src/ui/UpdateDialog.h` and `src/ui/UpdateDialog.cpp`)
- Dialog window for checking and applying updates
- Integrates with `appimageupdatetool` command-line utility
- Shows update status and progress
- Handles both checking for updates and downloading them

**Help Menu** (in `src/mainwindow.h` and `src/mainwindow.cpp`)
- Added "Help" menu to the main window
- "Check for Updates..." menu item - opens UpdateDialog
- "About Unimalen" menu item - shows application info

### 2. AppImage Infrastructure

**Desktop Entry** (`unimalen.desktop`)
- Required for AppImage integration
- Defines application name, icon, categories
- Used by Linux desktop environments

**Application Icon** (`unimalen.svg`)
- Scalable vector icon for the application
- Shows pencil and paper with colorful strokes
- Used in AppImage and by desktop environments

**Build Script** (`build-appimage.sh`)
- Automated script to create AppImages
- Downloads linuxdeploy and linuxdeploy-plugin-qt
- Bundles Qt6 dependencies automatically
- Creates self-contained executable
- Supports embedding update information for GitHub releases

### 3. Documentation

**APPIMAGE.md**
- Comprehensive guide to building AppImages
- Instructions for enabling auto-updates
- Explanation of update information formats
- Troubleshooting tips
- Version release workflow

**Updated README.md**
- Added AppImage as recommended installation method
- Included build instructions for AppImage
- Listed auto-update as a feature

## How It Works

### For End Users

1. Download `Unimalen-*.AppImage` from GitHub releases
2. Make it executable: `chmod +x Unimalen-*.AppImage`
3. Run it: `./Unimalen-*.AppImage`
4. To update: Go to **Help → Check for Updates**

### For Developers/Maintainers

1. Build AppImage with update info:
   ```bash
   # Edit build-appimage.sh to set UPDATE_INFORMATION
   export UPDATE_INFORMATION="gh-releases-zsync|owner|repo|latest|Unimalen-*-x86_64.AppImage.zsync"

   # Build
   ./build-appimage.sh
   ```

2. Create GitHub release:
   - Tag version (e.g., v1.0.1)
   - Upload both `.AppImage` and `.AppImage.zsync` files

3. Users will be notified when they check for updates

## Technical Details

### Update Mechanism

- **Format**: GitHub Releases with zsync
- **Tool**: appimageupdatetool (external dependency)
- **Protocol**: HTTP/HTTPS
- **Delta Updates**: Only downloads changed bytes using zsync
- **Verification**: Embedded in AppImage metadata

### Update Information Format

```
gh-releases-zsync|owner|repo|latest|pattern
```

Example:
```
gh-releases-zsync|johndoe|unimalen|latest|Unimalen-*-x86_64.AppImage.zsync
```

### File Structure

```
unimalen/
├── src/
│   └── ui/
│       ├── UpdateDialog.h          # Update dialog header
│       └── UpdateDialog.cpp        # Update dialog implementation
├── unimalen.desktop                # Desktop entry file
├── unimalen.svg                    # Application icon
├── build-appimage.sh               # AppImage build script
├── APPIMAGE.md                     # AppImage documentation
└── README.md                       # Updated with AppImage info
```

## Dependencies

### Build-time
- linuxdeploy (downloaded automatically)
- linuxdeploy-plugin-qt (downloaded automatically)
- Qt6 development libraries
- CMake 3.16+
- C++17 compiler

### Runtime (for auto-update)
- appimageupdatetool (user installs separately)
- Internet connection
- Write permissions to AppImage location

## Benefits

1. **Easy Distribution**: Single file, works across Linux distros
2. **No Installation**: Just download and run
3. **Auto-Updates**: Users can update from within the app
4. **Efficient**: Delta updates save bandwidth
5. **Decentralized**: No update server required
6. **Portable**: Run from USB drives, home directory, etc.
7. **Backwards Compatible**: Regular builds still work

## Future Enhancements

Possible improvements:
- Automatic update checks on startup (with user preference)
- Update notifications in the status bar
- Release notes display in update dialog
- Rollback functionality
- Alternative update backends (not just GitHub)

## Testing

To test auto-update:

1. Build an AppImage with old version number
2. Run it and note version in Help → About
3. Build new AppImage with higher version
4. Create GitHub release with new version
5. Run old AppImage
6. Go to Help → Check for Updates
7. Should detect and offer to download new version

## Notes

- Auto-update only works when running from AppImage
- Regular binary builds don't have update functionality
- Update check requires appimageupdatetool installed
- First-time users may need to install appimageupdatetool
- Consider adding installation instructions for appimageupdatetool to documentation
