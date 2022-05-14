# VCU Driver Profile Tool
A cross-platform graphical tool for customising and exporting driver profiles for the VCU throttle response.

<p align="center">
  <img src="./docs/C2 Spline.png" width="90%" class="center">
</p>

## Installing
This tool has been pre-compiled for macOS and Windows as a standalone application. The latest release is available [here](https://github.com/sufst/vcu-driver-profile/releases), or use one of the links below for a direct download:

- [macOS](https://github.com/sufst/vcu-driver-profile/releases/download/v1.0.0/VCU.Driver.Profile.app.zip)
- [Windows](https://github.com/sufst/vcu-driver-profile/releases/download/v1.0.0/VCU.Driver.Profile.exe)

The program can also be compiled and run on Linux using `cmake` and `make` (see: [build process](#Initial-Setup-and-Build-Process)).

## Creating a Profile

The default profile is linear. Points can be added by clicking anywhere on the graph and existing points can be moved by clicking and dragging on them. To delete points, press backspace which will enter delete mode and change the cursor to ⌫ - any points you click on will be deleted until backspace is pressed again to exit delete mode. To reset the entire curve, press the escape key.

<p align="center">
  <img src="./docs/Placing Points.gif" width="32%" class="center">
  &nbsp;
  <img src="./docs/Moving Points.gif" width="32%" class="center">
  &nbsp;
  <img src="./docs/Deleting Points.gif" width="32%" class="center">
</p>

Tips for getting a good profile:
- Make sure the curve is strictly increasing.
- Make sure the curve doesn't exceed the axis bounds - this will cause clipping when the profile is exported.
- Don't place points next to each other with a steep gradient between them - this causes poor interpolation.
- Make sure no two points have the same input (x-axis) coordinate - you can't map one input to two different outputs. Interpolation algorithms also don't like this because the curve becomes discontinuous (or has an infinite gradient).
- (Probably) use the C² spline interpolation algorithm.

A warning will be generated when exporting if there is a major issue with the generated curve.

## Throttle 'Dead-Zone'
The blue region at the left of the curve defines the 'dead-zone' for the throttle. An input in this region will produce no output torque from the motor. A small dead-zone is required to guarantee that the car will not move when the throttle is fully released as slight mechanical misalignments or voltage measurement errors may still produce a small input to the VCU.

<p align="center">
  <img src="./docs/Deadzone.gif" width="50%" class="center">
</p>

## Interpolation Algorithms

[Interpolation](http://paulbourke.net/miscellaneous/interpolation/) is used to work out the values of the throttle curve between manually placed points. The choice of interpolation algorithm will affect the calculated throttle response and change the way the curve behaves when you interact with it.

<p align="center">
  <img src="./docs/Linear.png" width="45%" class="center">
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  <img src="./docs/C2 Spline.png" width="45%" class="center">
</p>

Four algorithms are available, which can be selected from the drop-down list:
1.  Linear (simplest)
2.  Cosine (useless)
3.  C² spline (best)
4.  Hermite spline (good)

## Importing and Exporting Profiles
Created profiles can be exported to an XML document by pressing the 'export profile' button and choosing a location on your computer to save the file. These files can be imported again and edited further by pressing the 'import profile' button, or by dragging the file into the graph area. The placed points the chosen interpolation method are stored in these documents, but not every point on the interpolated curve as these easily can be re-generated by importing the placed points. The deadzone is defined by the value of the first point and does not need to be stored separately.

<p align="center">
  <img src="./docs/Profile.gif" width="60%" class="center">
</p>

## Exporting Source Code
The 'export code' button will generate a `uint16_t` C array mapping 2¹⁰ integer input values onto the interpolated curve and copy it to the clipboard. This is used as a look-up table in the VCU to map the scaled 10-bit analogue reading from the throttle to the desired output torque.

```C
static const uint16_t driver_profile [1024] = {
  0x0000, 0x002e, 0x005d, 0x008b, 0x00ba, 0x00e9, 0x0117, 0x0146, 
  0x0174, 0x01a3, 0x01d2, 0x0200, 0x022f, 0x025d, 0x028c, 0x02bb, 
  0x02e9, 0x0318, 0x0346, 0x0375, 0x03a3, 0x03d2, 0x0400, 0x042f, 
  0x045d, 0x048c, 0x04ba, 0x04e8, 0x0517, 0x0545, 0x0574, 0x05a2, 
  // ...
};
```

## Initial Setup and Build Process
Clone the repo:
```sh
git clone https://github.com/sufst/vcu-driver-profile
```
Clone / update submodules:
```sh
git submodule init
git submodule update
```

The build process is managed by `cmake` which you will need to have installed, along with `make` and a C++ compiler. Before building, create the build directory:
```sh
mkdir build
```
To build for debug:
```sh
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
```
Similarly to build for release:
```sh
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

Tip: for a faster build, run `make -j$(nproc)` to parallelise the build with one job per core on the host machine.


## Dependencies
- [JUCE](https://github.com/juce-framework/JUCE)
- [Spline](https://github.com/ttk592/spline)
