# VCU Driver Profile Tool
A graphical tool for customising and exporting driver profiles for the VCU.

## Usage

### Creating a Profile

### Interpolation Algorithms

<p align="center">
  <img src="./docs/Linear.png" width="30%" class="center">
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  <img src="./docs/C2 Spline.png" width="30%" class="center">
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  <img src="./docs/Hermite Spline.png" width="30%" class="center">
</p>

Four algorithms are available for interpolating between the placed points.
- Linear
- Cosine
- C2 spline
- Hermite

### Exporting Profiles

### Exporting Source Code


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
- For Linux use make (untested).

## Dependencies
- [JUCE](https://github.com/juce-framework/JUCE)
- [Spline](https://github.com/ttk592/spline)
