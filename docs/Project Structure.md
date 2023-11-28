# Project Structure

#### This document provides a high-level overview of the project is organized.

The top level folders and files of this project are shown below:
```
vcu-gui
|
|- 📁 .github
|- 📁 .trunk
|- 📁 .vscode
|- 📁 .docs
|- 📁 res
|- 📁 lib
|- 📁 src
|- 📄 .gitignore
|- 📄 .gitmodules
|- 📄 CMakeLists.txt
|- 📄 Jenkinsfile
|- 📄 README.md
```
* `.github` contains notes of rules you should follow when contributing to this project and a template for git pull requests.
* `.trunk` are files used by a linter (a program that checks the quality of your code). **In most cases, this can be ignored.**
* `.vscode` contains *.json* files read by VS code to configure the development environment. **In most cases, this can be ignored.**
* `docs` contain documentations for the project (including README.md and the file you are reading), along with images embedded in them. **In most cases, this can be ignored** 
* `res` contains resource files. Currently, it contans the official SUFST 'Karla' font and icon images for the app.
* `lib` contains the two primary git submodules for this project, which are `juce` and `spline`. 
    * `juce` is the main framework to start the app, create a window for the app to define user interactions.
    * `spline` is used to define lines and curves. This is used to create torque-speed map.
* `src` contains the source code behind the project. **More explanations below.**


## Structure of a JUCE App

The brief structure of a JUCE application can be represented as the diagram below:

* Within a JUCE app, you can one or more **windows**, depicted as *MainWindow* and *SubWindow* below.
* Each window contain **components**. **Components are the building blocks of the UI**. In the case of the VCU-GUI app, the torque map is a component, menubar is a component, etc.


```plaintext
+--------------------------------------------+
|                         JUCE Application   |  
|                                            | 
| +-----------------+   +-----------------+  |
| |     MainWindow  |   |      SubWindow  |  | 
| | +-------------+ |   | +-------------+ |  |
| | | mainContent | |   | |  component1 | |  |
| | +-------------+ |   | |  component2 | |  |
| +-----------------+   | +-------------+ |  |
|                       +-----------------+  |
+--------------------------------------------+
```



## Source Code Structure
How the source code is organized is explained more in this section.

The folder structure for `src` is as below:
```
vcu-gui
|- ...
|- 📁 src
   |----- 📁 config
   |----- 📁 gui
   |      |----- 📁 windows
   |      |      |----- 📄 AboutWindow.cpp
   |      |      |----- 📄 AboutWindow.h
   |      |      |----- 📄 MainWindow.cpp
   |      |      |----- 📄 MainWindow.h
   |      |
   |      |----- 📁 appearance
   |      |      |----- 📄 Colours.h
   |      |      |----- 📄 LookAndFeel.cpp
   |      |      |----- 📄 LookAndFeel.h
   |      |
   |      |----- 📁 menubar
   |
   |
   |----- 📁 utility
   |----- 📄 Application.cpp
   |----- 📄 Application.h
   |----- 📄 CommandManager.h
   |----- 📄 Main.cpp
```

## Top Level Files
#### There are four top level files:
```
📁 src
 |----- 📄 Application.cpp
 |----- 📄 Application.h
 |----- 📄 CommandManager.h
 |----- 📄 Main.cpp
 |
 |.... 
```

* 📄 *Main.cpp* contains a single function `START_JUCE_APPLICATION(Application)` that launches the JUCE app.

* 📄 *Application.h* and 📄*Application.cpp* defines the main application to be passed to 📄*Main.cpp* to be launched.
    * When initialized, create a `mainWindow` object which is implemented in `src/windows/📄MainWindow.cpp`
    * **Defines app-level behaviour**, such as whether to allow for more than one instance of the app to run, what happens when the app is closed, etc.

* 📄 *CommandManager.h* defines a `CommandManager` class.
    * The class contains a set of `CommandIDs`, which are ID that can be mapped to actions such as `ShowAboutWindow`, `ShowPreferencesWndow`, `ShowGitHubRepo`, `CloseWindow` etc.
        * For example, in `src/gui/windows/📄MainWindow.cpp`, we explicitly map `Ctrl+W` to the command ID `CloseWindow`. Then we can map the commandID of `CloseWindow` to call the `closeButtonPressed()` function. Now, pressing `Ctrl+W` on the keyboard will call the `closeButtonPressed()` function and actually close the window.


## GUI Folder
#### This is a large folder responsible for the "graphics" element of the app.

### 📁 `windows`

This directory contains four files:
```
|----- 📁 windows
|      |----- 📄 AboutWindow.cpp
|      |----- 📄 AboutWindow.h
|      |----- 📄 MainWindow.cpp
|      |----- 📄 MainWindow.h
```

* 📄*MainWindow.h* and 📄*MainWindow.cpp* defines a `MainWindow` class used to create the main window for the app. 

* 📄 *AboutWindow.h* and 📄*AboutWindow.cpp* defines a `AboutWindow` class used to create the "About" window for the app, which appears when the user clicks on **"Help" > "About VCU-GUI"** on the menubar.

<details>
<summary> <b>Details about Implementation</b></summary>

These windows (`MainWindow` and `AboutWindow`) are created by inheriting from `juce`'s built-in classes, such as `DocumentWindow` and `DialogWindow`. These classes provide member functions to perform a variety of functions, such as:

- `setContentOwned()` or `setContentNonOwned()` to include components like 
menubars and graphs to be seen in the window. 
- `setVisible()` to make the window visible
- `setResizeLimits()` to define the minimum and maximum window size


Additionally, the main window also inherits from the class `ApplicationCommandTarget`. This means this window is now a target to commands - it will listen to commands such as `Ctrl+W`. The behaviour of these commands are explicitly coded in the source file.
</details>

### 📁 `appearance`

This directory contains three files:
```
|----- 📁 appearance
        |----- 📄 Colours.h
        |----- 📄 LookAndFeel.cpp
        |----- 📄 LookAndFeel.h
```

* 📄 *LookAndFeel.h* and 📄 *LookAndFeel.cpp*, as its name suggests, defines the look and feel of the app, such as the global font of the app.
* 📄*Colours.h* just defines a set of preset colors and initialize them as ARGB hexadecimals.

### 📁 `menubar`

This directory contains two files:
```
|----- 📁 menubar
        |----- 📄 MenuBar.h
        |----- 📄 MenuBar.cpp
```

* 📄 *Menubar.h* and 📄 *Menubar.cpp* defines the menubar, which has four items:
*File*, *View*, *Window* and *Help*.

<details>
<summary> <b> Details about Implementation</b> </summary>

The menubar inherits from `juce`'s built-in classes *MenuBarModel* and *ApplicationCommandTarget*.

- Inheriting `MenuBarModel` allow the items of the menubar to be defined, such as using `getMenuBarNames()` to register the name for each item.
- Inheriting `ApplicationCommandTarget` allow the menubar to be targeted by commands, such
as `Ctrl+W` to close, `Ctrl+F` to fullscreen, etc.

If the system is MAC, the MenuBar class is configured differently, since the
app menubar will reside at the global system menubar that runs at the top of 
the screen.
</details>

### 📁 `components`
This is a folder containing components that can be added to the windows defined
in `src/gui/windows`.

* 📄 `MainComponent.h` & `MainComponent.cpp` 

    * The primary component of the primary window defined in `MainWindow`. This component consists of multiple tabs defined in `TabbedComponent`.

* 📄 `TabbedComponent.h` & `TabbedComponent.cpp`

    * Defines a `TabbedComponent` class so that multiple tabs can be added to `MainComponent`.

## Utility Folder

The utility folder contain mathematical functions that is usually used when plotting points onto the torque map.

This directory contains four files:
```
|----- 📁 utility
        |----- 📄 clip.h
        |----- 📄 Interpolator.h
        |----- 📄 linspace.h
        |----- 📄 PointComparator.h
```

 ### 📄 *clips.h* 
 This header file defines and implements a single `clip()` function that takes
 three inputs (*value*, *min*, *max*). 
 
 If *value* is not between *min* and *max*, it will then be clipped to *min* or 
 *max* depending on which is closer. If *value* is between *min* and *max* then
the function just returns the original value.

### 📄 *Interpolator.h* 
This header file defines multiple classes related to interpolation - when points are added to the curve, these classes are responsible for plotting the curves in between the points.

Defines a base `Interpolator` class, and create additional specific interpolator classes that derives from it depending on the line type, such as **linear**, **spline** and **cosine**.

### 📄 *linspace.h* 
This header file defines and implements a single `linspace` function that takes
three inputs (*start*, *end*, *num*).

Returns a vector of *num* number of elements equally spaced between *start* and *end*.

### 📄 *PointComparator.h* 
This header file defines and implements a single `PointComparator` class. This
has a single method `compareElements` which take two input coordinates and
compare their X coordinates, returning:

- `-1` if the x-coordinate of point 1 is less than point 2
- `+1` if the x-coordinate of point 1 is more than point 2

This can be used to arrange newly-added points in an ascending order.


## Config Folder
This is another large folder.

### 📄 `Constrainers.h`

### 📄 `DataModel.h` & `DataModel.cpp`


### 📄 `Metadata.h` & `Metadata.cpp`
📄 `Metadata.h` creates a structure called `Metadata` that stores the configuration, version number and other stats of the app.  This is to be used by the "Metadata" tab defined within `src/gui/components/MainComponent`.

### 📄 `TorqueMap.h` & `TorqueMap.cpp`

### 📄 `ValueTreeObjectList.h`


