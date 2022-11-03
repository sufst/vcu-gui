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

Visual Studio build tools:
- Download the installer from [this page](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022).
- Start the installer and select "Desktop development with C++" and "Universal Windows Platform build tools".
- Uncheck all the "optional" SDKs **except MSVC** for a minimal install.
- Locate the `msbuild` executable and add it to your path - it will be in a place something like `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\`.

Visual Studio Debugger:
- Download the Windows SDK from [this page](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/debugger-download-tools).
- Select Debugging Tools for Windows
- Check the for the `cdb` executable to ensure installation has been successful - it will be in `C:\Program Files (x86)\Windows Kits\10\Debuggers\x64`.

To actually build on Windows, the process is a bit different.
1. Open Powershell and navigate to `vcu-gui/build`.
2. Run `cmake ..` from this folder, it should build `juceaide` and generate the file `VCU-GUI.sln`.
3. Run `msbuild ./VCU-GUI.sln` to build the project.

## VSCode
- Install the recommended extensions.
- `trunk` is not compatible with Windows, so you will need to run it from WSL.
