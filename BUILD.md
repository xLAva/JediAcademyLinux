How to build the code
=====================

Checkout the code from the git repository:
https://github.com/xLAva/JediAcademyLinux/tree/HmdSupportDK2

It is possible to build the game without any VR support. 
For VR support at least one of the following libraries is needed.

### OculusSDK (optional)
The needed OculusSDKs are added as submodules in the 3rdparty folder (inside the JediAcademy folder structure)
This folders contain 
* all OculusSDK platform files merged together
* prebuild binaries 
* dynamic lib versions (based on https://www.jspenguin.org/software/ovrsdk/ - thx Jared Stafford)

Submodule git repository:
https://github.com/xLAva/OculusSDK


### OpenHmd (optional)
Checkout OpenHmd from my git repository into the 3rdparty/OpenHmd folder (inside the JediAcademy folder structure)
https://github.com/xLAva/OpenHmd

OpenHmd doesn't support all DK2 features at the moment and the DK2 Windows support looks broken right now.



Needed packages on Ubuntu 14.04 64bit:
--------------------------------------

I'm using a 64bit system, but this port is still a 32 bit program!

sudo apt-get install libsdl2-dev:i386 libglm-dev libhidapi-dev:i386 libopenal-dev:i386 libhidapi-libusb0:i386

sudo apt-get install cmake g++-multilib



Needed packages on Windows:
---------------------------

Visual Studio C++ (Express is enough)

Place this libraries in the 3rdparty folder:
* SDL2: https://www.libsdl.org/release/SDL2-2.0.3-win32-x86.zip
* GLM: http://sourceforge.net/projects/ogl-math/files/latest/download?source=files

Only needed for OpenHmd:
* hidapi: https://github.com/downloads/signal11/hidapi/hidapi-0.7.0.zip


Start building the project:
---------------------------

Create a new directory beside the repository folder.
Call this command inside the new folder:

cmake ../HmdSupportDK2/code

After cmake is finished follow the last step.

#### On Linux:
make

#### On Windows: 
Open the VisualStudio solution and build the project

#### On Mac:
... sorry. Mac is not supported at the moment.
