# VCU Driver Profile Tool
A cross-platform graphical tool for customising and exporting driver profiles for the VCU throttle response.

<p align="center">
  <img src="./docs/C2 Spline.png" width="90%" class="center">
</p>

## TODO: Creating a Profile

## Interpolation Algorithms

<p align="center">
  <img src="./docs/Linear.png" width="45%" class="center">
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  <img src="./docs/C2 Spline.png" width="45%" class="center">
</p>
<!-- &nbsp; <img src="./docs/Hermite Spline.png" width="32%" class="center"> -->

Four algorithms are available for [interpolating](http://paulbourke.net/miscellaneous/interpolation/) between the placed points.
1.  Linear (simplest)
2.  Cosine (useless)
3.  C2 spline (best)
4.  Hermite spline (good)

## TODO: Importing and Exporting Profiles

## Exporting Source Code
The 'export code' button will generate a `uint16_t` C array mapping 2¹⁰ integer input values onto the interpolated curve. This is used as a look-up table in the VCU to map the scaled 10-bit analogue reading from the throttle to the desired output torque.

```C
static const uint16_t driver_profile [1024] = {
  0x0000, 0x002e, 0x005d, 0x008b, 0x00ba, 0x00e9, 0x0117, 0x0146, 
  0x0174, 0x01a3, 0x01d2, 0x0200, 0x022f, 0x025d, 0x028c, 0x02bb, 
  0x02e9, 0x0318, 0x0346, 0x0375, 0x03a3, 0x03d2, 0x0400, 0x042f, 
  0x045d, 0x048c, 0x04ba, 0x04e8, 0x0517, 0x0545, 0x0574, 0x05a2, 
  // ...
};
```

## Contributing
Clone the repo:
```
git clone https://github.com/sufst/vcu-driver-profile
```
Clone / update submodules:
```
git submodule init
git submodule update
```

Install [JUCE](https://juce.com/get-juce) (personal or education) on your system. [JUCE's project configuration tool](https://juce.com/discover/projucer) will generate the necessary project files to build, run and debug a project using a variety of exporters (usually an IDE).
- For macOS, use Xcode.
- For Windows use Visual Studio (untested).
- For Linux use make.

## Dependencies
- [JUCE](https://github.com/juce-framework/JUCE)
- [Spline](https://github.com/ttk592/spline)
