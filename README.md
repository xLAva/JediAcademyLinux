Oculus Rift support for Jedi Academy
====================================

I want to share my new project: Oculus Rift DK1 support for Jedi Academy (for Windows and Linux)

https://github.com/xLAva/JediAcademyLinux/tree/HmdSupportGPL

This project was on my mind since the source code was released last year. Finally I had some time this summer to get started.

This Oculus Rift support implementation is designed for first person game play. It is possible to switch to 3rd-person view, but the best (and motion sickness free) experience is in first person mode.

I haven't tested the whole game yet, but the first 7 levels are working very good.


Play the game
-------------

There are binaries for Linux and Windows checked into the Github repository, so if you want to just play the Oculus Rift version, download the binary files and start playing right away.

https://github.com/xLAva/JediAcademyLinux/tree/HmdSupportGPL/binary/win32

https://github.com/xLAva/JediAcademyLinux/tree/HmdSupportGPL/binary/linux

Copy all the files from the Windows or Linux platform folder into the root directory of your "Jedi Knight III: Jedi Academy" installation and start the "jasphmd" executable.

The version was tested on Ubuntu 14.04 and Windows 7.

Ubuntu: the universe repository has to be activated (Software & Updates)

Needed libraries on Ubuntu 14.04 32bit:
sudo apt-get install libsdl2-2.0-0 libopenal1

Needed libraries on Ubuntu 14.04 64bit:
sudo apt-get install libsdl2-2.0-0:i386 libopenal1:i386 libgl1-mesa-glx:i386


Changes and new Features for the Oculus Rift:
---------------------------------------------

* Free look during in-game cut scenes
* Removed all forced camera rotations and a lot of camera movements
* Improved game controller support (SDL2 Xbox Controller layout)
* Auto detection of the Rift display position
* Enabled first person lightsaber mode as default
* Resized UI rendering


Build the game
--------------

This time around I used cmake instead of Code::Blocks. It is configured to work on Linux and Windows. If you want to build the code yourself, just follow the instructions in Build.md.


More information
----------------

Because of the license issues with the original Oculus Rift SDK, I used the OpenHMD library. It works pretty good, but the original SDK has more high level features. I might experiment with the alternative solution to run the Oculus Rift SDK in a separate process.

There are some hacks inside the code to improve the Oculus Rift experience, which also affects the normal gameplay without a connected HMD device. I plan to replace all hacks with a proper implementation and merge the Oculus Rift changes with the original port.

Before I started this project, I had a look at the rift support implementation from LeeN (mtbs3d forum). This helped me to get started. Thx LeeN.

Feel free to contact me: jochen.leopold@model-view.com
