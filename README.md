# OpenLayout
OpenLayout is free cross-platform clone of Sprint Layout

**Installing requirements**

Debian/Ubuntu:
```
sudo apt install libwxbase3.0-dev libwxgtk3.0-dev
```
Arch Linux/Manjaro:
```
sudo pacman -S wxgtk-common wxgtk3 wxgtk2
```
**Build**

```
mkdir build
cd build
cmake ..
make -j$(nproc)
```

**Installing**

```
sudo make install
```

