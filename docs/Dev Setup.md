# Development Environment Setup 

## Tools
- git
- CMake
- C++ compiler (`clang`, `gcc` or `msvc`)
- [Trunk](https://trunk.io)

## macOS

Install Xcode command line tools with:
```sh
xcode-select --install
```

Install `trunk` with:
```sh
curl https://get.trunk.io -fsSL | bash
```

Install `cmake` from the disk image on the [CMake downloads page](https://cmake.org/download/) or using [Homebrew](https://brew.sh):
```sh
brew install cmake
```

## Linux (Ubuntu)

Install `git` with:

```sh
sudo apt install git 
```

Install `cmake` with:

```sh
sudo apt install cmake
```

Install `trunk` with: 
```sh
curl https://get.trunk.io -fsSL | bash
```

Install JUCE build dependencies with:
```sh
sudo apt install libasound2-dev
sudo apt install libfreetype6-dev
sudo apt install libcurl4-openssl-dev
sudo apt install libwebkit2gtk-4.0-dev
```ls

Ubuntu ships with `gcc`, but make sure to run `sudo apt upgrade`.

## Windows
TBD - we know it is possible to build the app on Windows, it was done successfully for release v1.0.0.
For now, a Linux VM or WSL is probably necessary.

## VSCode
- Install the recommended extensions.
- `trunk` is not compatible with Windows, so you will need to run it from WSL.
