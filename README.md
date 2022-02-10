# VCU Driver Profile Tool
A cross-platform graphical tool for customising and exporting driver profiles for the VCU throttle response.

<p align="center">
  <img src="./docs/C2 Spline.png" width="60%" class="center">
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

## TODO: Exporting Source Code


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
