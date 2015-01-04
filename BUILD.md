How to build the code
=====================

Checkout the code from the git repository:
https://github.com/xLAva/JediAcademyLinux/tree/HMD-DK2

Checkout OpenHmd from the git repository inside the 3rdparty/OpenHmd folder (inside the JediAcademy folder structure)
https://github.com/xLAva/OpenHmd


Needed packages on Ubuntu 14.04 64bit:
--------------------------------------

sudo apt-get install cmake g++-multilib libsdl2-dev:i386 libglm-dev libhidapi-dev:i386 libopenal-dev:i386 libhidapi-libusb0:i386


Needed packages on Windows:
---------------------------

Visual Studio C++ (Express is enough)

Place this libraries in the 3rdparty folder:
SDL2: https://www.libsdl.org/release/SDL2-2.0.3-win32-x86.zip
GLM: http://sourceforge.net/projects/ogl-math/files/latest/download?source=files
hidapi: https://github.com/downloads/signal11/hidapi/hidapi-0.7.0.zip


Start building the project:
---------------------------

Create a new directory beside the repository folder.
Call this command inside the new folder:

cmake ../HMD-DK2/code

After cmake is finished follow the last step.

On Linux:
make

On Windows: 
Open the VisualStudio solution and build the project

