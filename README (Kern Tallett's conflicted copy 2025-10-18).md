# Unimalen

A lightweight drawing and painting application designed for creating zines and digital artwork.

## Features

- **Multi-page support** - Create zines with up to 24 pages
- **Layer system** - Up to 3 layers per page for organized artwork
- **Drawing tools** - Pencil, brush, spray, eraser, and more
- **Shape tools** - Lines, squares, rounded rectangles, ovals (filled and outlined)
- **Advanced tools** - Bezier curves, fill bucket, lasso selection
- **Scissors tool** - Cut images into movable pieces
- **Text support** - Movable text with multiple fonts
- **Color palette** - Copic pastel color selection
- **Pattern fills** - Various fill patterns for creative effects
- **Page navigation** - Easy navigation between pages with keyboard shortcuts
- **File formats** - Save as PNG or OpenRaster (ORA) format
- **Zine export** - Export multi-page documents as zine folders

## Installation

### Option 1: AppImage (Recommended)

Download the latest AppImage from the [Releases](https://github.com/yourusername/unimalen/releases) page:

```bash
# Download the AppImage
wget https://github.com/yourusername/unimalen/releases/download/latest/Unimalen-x86_64.AppImage

# Make it executable
chmod +x Unimalen-*.AppImage

# Run it
./Unimalen-*.AppImage
```

**Features:**
- Single-file distribution, no installation required
- Works across all modern Linux distributions
- Built-in auto-update functionality (Help → Check for Updates)
- Portable - run from anywhere

### Option 2: Build from Source

#### Prerequisites

- CMake 3.16 or higher
- Qt6 (Core, Widgets, Gui, LinguistTools)
- C++17 compatible compiler

#### Build Instructions

```bash
cd build
cmake ..
make -j4
```

The compiled executable will be `./unimalen`

#### Dependencies (Debian/Ubuntu)

```bash
sudo apt install cmake qt6-base-dev qt6-tools-dev qt6-tools-dev-tools
```

#### Dependencies (Fedora)

```bash
sudo dnf install cmake qt6-qtbase-devel qt6-qttools-devel
```

### Option 3: Build AppImage

To create an AppImage with auto-update support:

```bash
./build-appimage.sh
```

See [APPIMAGE.md](APPIMAGE.md) for detailed instructions on building and configuring auto-updates.

## Usage

### Keyboard Shortcuts

- **Ctrl+N** - New file
- **Ctrl+O** - Open file
- **Ctrl+S** - Save file
- **Ctrl+Shift+S** - Save as
- **Ctrl+Z** - Undo
- **Ctrl+Y** - Redo
- **Ctrl+X** - Cut
- **Ctrl+C** - Copy
- **Ctrl+V** - Paste
- **Ctrl+Shift+P** - Add new page
- **Ctrl+Left/Right** - Navigate between pages
- **Ctrl+1/2/3...9** - Switch between tabs

### Tools

Select tools from the toolbar on the left side of the window. The scissors tool allows you to cut through artwork and move the pieces independently.

## License

This project is licensed under the **GNU General Public License v3.0** (GPL-3.0).

You are free to use, modify, and distribute this software under the terms of the GPL-3.0 license. See the [LICENSE](LICENSE) file for details, or visit [https://www.gnu.org/licenses/gpl-3.0.html](https://www.gnu.org/licenses/gpl-3.0.html).

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.
