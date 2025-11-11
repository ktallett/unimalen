# Erstelle - Drawing and Painting Application

A lightweight drawing and painting application built with C++ and Qt 6 for creating zines and digital artwork.

Designed for artists creating zines, comics, and illustrations with a focus on simplicity and essential drawing tools.

## Features

- **Multi-page support** - Create zines with up to 24 pages
- **Layer system** - Multiple layers per page for organized artwork
- **Drawing tools** - Pencil, brush, spray, eraser, and more
- **Shape tools** - Lines, squares, rounded rectangles, ovals (filled and outlined)
- **Advanced tools** - Bezier curves, fill bucket, lasso selection
- **Scissors tool** - Cut images into movable pieces
- **Text support** - Movable text with multiple fonts
- **Color palette** - Copic pastel color selection
- **Pattern fills** - Various fill patterns for creative effects
- **File formats** - Save as PNG or OpenRaster (ORA) format

## Installation

### Prerequisites

- Qt 6.2+ development libraries
- CMake 3.16+
- C++17 compatible compiler

### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install qt6-base-dev qt6-tools-dev cmake build-essential

mkdir -p build
cd build
cmake ..
make -j4
./erstelle
```

### Linux (Fedora/CentOS)

```bash
sudo dnf install qt6-qtbase-devel qt6-qttools-devel cmake gcc-c++

mkdir -p build
cd build
cmake ..
make -j4
./erstelle
```

### macOS

```bash
brew install qt@6 cmake

mkdir -p build
cd build
cmake -DCMAKE_PREFIX_PATH=/opt/homebrew/opt/qt@6 ..
make -j4
./erstelle
```

### Windows

1. Install [Qt 6](https://www.qt.io/download)
2. Install [CMake](https://cmake.org/download/)
3. Open Qt Creator and import the CMakeLists.txt file
4. Build and run

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

Select tools from the toolbar on the left side of the window:
- **Pencil** - Freehand drawing
- **Brush** - Soft brush strokes
- **Spray** - Spray paint effect
- **Eraser** - Remove artwork
- **Line** - Draw straight lines
- **Bezier** - Create curved paths
- **Scissors** - Cut through artwork and move pieces independently
- **Fill** - Fill enclosed areas with color
- **Lasso** - Select irregular areas
- **Shapes** - Rectangles, rounded rectangles, and ovals

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

---
