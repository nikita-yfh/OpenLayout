# OpenLayout

OpenLayout is a free, cross-platform clone of Sprint-Layout (PCB layout editor).

This branch (`qt`) is built with **Qt 5** and OpenGL.

## Requirements

**Debian / Ubuntu** (incl. Ubuntu 24.04):
```
sudo apt install build-essential cmake git \
    qtbase5-dev qtbase5-dev-tools libqt5opengl5-dev \
    libgl1-mesa-dev mesa-common-dev
```
`qtbase5-dev` already provides the print-support module used by the print feature.

**Arch Linux / Manjaro:**
```
sudo pacman -S base-devel cmake git qt5-base mesa
```

> Note: older instructions referenced wxWidgets 3.0 — that is no longer used and
> those packages have been dropped from current distributions.

## Build

```
git clone https://github.com/nikita-yfh/OpenLayout.git
cd OpenLayout
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Run

The executable is produced in the `build` directory:
```
./openlayout
```

On a **Wayland** session the app selects the XWayland (`xcb`) Qt platform by
itself to avoid `QOpenGLWidget` rendering artefacts. To force a platform
manually:
```
QT_QPA_PLATFORM=xcb ./openlayout
```

## Features

- Drawing tools: tracks, THT/SMD pads, polygons/zones, circles, connections.
- Edit: select (incl. rubber-band), move, copy/cut/paste, duplicate, delete,
  group/ungroup, rotate, mirror, align, snap to grid, undo/redo.
- Layers: active-layer selector, per-layer visibility, move selection to a layer.
- Multiple boards with a tab bar; board properties; project info.
- File format: Sprint-Layout `.lay6`.
- Export: Gerber (RS-274X), Excellon drill, isolation milling (approximate),
  images (PNG/JPG/BMP/GIF), printing/PDF, element macros.
- Import: Gerber (this project's own dialect) and element macros.
- Bitmap underlay reference image.
