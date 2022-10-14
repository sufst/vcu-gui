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
```

Ubuntu ships with `gcc`, but make sure to run `sudo apt upgrade`.

## Windows

CMake:
- Download the `cmake` installer from the [CMake downloads page](https://cmake.org/download/) and run the installer. 
- Make sure that CMake is added to the system PATH for the current user in the install options.
<!-- 
MinGW-W64 and `gcc` toolchain:
- Install MinGW-W64 from its [release page on GitHub](https://github.com/niXman/mingw-builds-binaries/releases). 
- You will need to use 7ZIP to extract the archive, or convert it to a regular ZIP file with [CloudConvert](https://cloudconvert.com/7z-to-zip) or a similar tool and unzip the file.
- Move the `mingw64` folder in the extracted archive to your `Program Files` folder.
- Add the `mingw64/bin` to your system PATH by editing your environment variables from the control panel. -->

## VSCode
- Install the recommended extensions.
- `trunk` is not compatible with Windows, so you will need to run it from WSL.
