Oculus Rift support for Jedi Academy
====================================

I want to share my new project: Oculus Rift DK1 & DK2 support for Jedi Academy (for Windows and Linux)

https://github.com/xLAva/JediAcademyLinux/tree/HmdSupportDK2

This project was on my mind since the source code was released last year. Finally I had some time this summer to get started.

This Oculus Rift support implementation is designed for first person game play. It is possible to switch to 3rd-person view, but the best (and motion sickness free) experience is in first person mode.


## Play the game

### Binary:

There are binaries for Linux and Windows checked into the Github repository, so if you want to just play the Oculus Rift version, download the binary files and start playing right away.

https://github.com/xLAva/JediAcademyLinux/tree/HmdSupportDK2/binary/win32

https://github.com/xLAva/JediAcademyLinux/tree/HmdSupportDK2/binary/linux_i386

Copy all the files from the Windows or Linux platform folder into the root directory of your "Jedi Knight III: Jedi Academy" installation.


### Original Patch:

For those not having the Steam version: you'll need the 1.01 update from here http://help.starwars.com/articles/en_US/FAQ/Where-do-I-find-the-latest-patch-for-Jedi-Knight-Jedi-Academy?section=Star-Wars#patch 


### Additional Asset File (optional):

The additional asset file just contains a new default config with the gamepad already setup (including hmd recenter using the start button). 
Also a small translation string was added for the resolution selection (not needed in HMD mode).
The game runs fine without this asset file.

If you want to use it, download the additional asset file and place it on your "base" folder of the "Jedi Knight III: Jedi Academy" insallation folder.

https://github.com/xLAva/JediAcademyLinux/tree/HmdSupportDK2/binary/base/assets4.pk3


### Platform Specific:

The version was tested on Ubuntu 14.04 and Windows 7.

#### Ubuntu: 

The universe repository has to be activated (Software & Updates)

Needed libraries on Ubuntu 14.04 32bit:
sudo apt-get install libsdl2-2.0-0 libopenal1

Needed libraries on Ubuntu 14.04 64bit:
sudo apt-get install libsdl2-2.0-0:i386 libopenal1:i386 libgl1-mesa-glx:i386

Be sure to mark "jasphmd" as executable and start the game with it.


## Changes and new Features for the Oculus Rift:

* Free look during in-game cut scenes
* Removed all forced camera rotations and a lot of camera movements
* Improved game controller support (SDL2 Xbox Controller layout)
* Auto detection of the Rift display position
* Enabled first person lightsaber mode as default
* Resized UI rendering


## Build the game

This time around I used cmake instead of Code::Blocks. It is configured to work on Linux and Windows. If you want to build the code yourself, just follow the instructions in Build.md.


## More information

For the DK2 support I had to use the OculusSDK. The OpenHMD library just doesn't support all the features. It is still possible to build the game with OpenHMD support. Just follow the instructions in the Build.md file.
The OculusSDK library is loaded during runtime and the game also works fine without the library. 

This version can also be played without any HMD device. All HMD tweaks will be disabled and the game should behave like the original version.

Before I started this project, I had a look at the rift support implementation from LeeN (mtbs3d forum). This helped me to get started. Thx LeeN.

Feel free to contact me: jochen.leopold@model-view.com
