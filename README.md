Oculus Rift support for Jedi Academy
====================================

This project provides new binaries for Jedi Academy with added Oculus Rift support (for Windows and Linux).

The original game and resources are needed to start the game.


https://github.com/xLAva/JediAcademyLinux/tree/HmdSupportDK2

https://github.com/xLAva/JediAcademyLinux/releases

This Oculus Rift support implementation is designed for first person game play. It is possible to switch to 3rd-person view, but the best (and motion sickness free) experience is in first person mode.


## Changelog

### Changelog 3.1.0

Changes for the VR mode

* changed world scaling again
* fixed missing Headtracking in cutscene
* fixed too dark 3d models in menu with activated volumetric shadow
* Windows:
 * use latest Oculus SDK version for Windows: Oculus SDK 1.3

### Changelog 3.0.0

Changes for the VR mode

* fixed Star Wars text scroll
* fixed stencil shadow
* changed world scaling
* Windows:
 * use latest Oculus SDK version for Windows: Oculus SDK 0.8
 * should work now on Windows 10
 * Menus:
   * use a quad placed in the world for all fullscreen menus
    * correct aspect ratio
    * correct placement of 3D elements embedded in the menus
* Linux:
 * use latest Oculus SDK version for Linux: Oculus SDK 0.5

### Changelog 2.0.1

* smaller crosshair
* use DK2 recommended texture size
* Windows:
 * improved performance by using fullscreen in Extended Mode
   * Extended Mode is the best option on Windows right now
    * Direct Mode still has some performance issues with OpenGL
 * disable window compositor
 * set to higher cpu priority
* Linux:
 * improved performance by setting noVSync

### Changelog 2.0.0

* DK2 support
* Position tracking support
* Head-Mounted-Device recenter using the start button

### Changelog pre 2.0.0

* Free look during in-game cut scenes
* Removed all forced camera rotations and a lot of camera movements
* Improved game controller support (SDL2 Xbox Controller layout)
* Auto detection of the Rift display position
* Enabled first person light-saber mode as default
* Resized UI rendering


## Performance Tips

Some options can be tweaked to boost the rendering performance to make sure the game runs stable framerates in VR mode.

Main Menu -> Setup -> More Video
* set Shadows to Simple
* disable Dynamic Glow
* disable Light Fares


## Play the game

### Binary:

There are binaries for Linux and Windows checked into the Github repository, so if you want to just play the Oculus Rift version, download the binary files and start playing right away.

https://github.com/xLAva/JediAcademyLinux/tree/HmdSupportDK2/binary/win32

https://github.com/xLAva/JediAcademyLinux/tree/HmdSupportDK2/binary/linux_i386

Copy all the files from the Windows or Linux platform folder into the following directory of your "Jedi Knight III: Jedi Academy" installation:

[Jedi Academy Install Folder]/GameData/


### Original Patch:

For those not having the Steam version: you'll need the 1.01 update from here http://help.starwars.com/articles/en_US/FAQ/Where-do-I-find-the-latest-patch-for-Jedi-Knight-Jedi-Academy?section=Star-Wars#patch 


### Additional Asset File (optional):

The additional asset file just contains a new default config with the gamepad already setup (including Head-Mounted-Device recenter using the start button). 
Also a small translation string was added for the resolution selection.
The game runs fine without this asset file.

If you want to use it, download the additional asset file and place it into the following folder of your "Jedi Knight III: Jedi Academy" installation:

https://github.com/xLAva/JediAcademyLinux/tree/HmdSupportDK2/binary/base/assets4.pk3 ->

[Jedi Academy Install Folder]/GameData/base/


### Platform Specific:

The version was tested on Ubuntu 14.04 and Windows 7.

#### Windows:

The **Oculus Runtime 1.3** is needed to run the game in VR mode.


#### Ubuntu: 

The **Oculus Runtime 0.5** is needed to run the game in VR mode.

The universe repository has to be activated (Software & Updates)

Needed libraries on Ubuntu 14.04 32bit:
sudo apt-get install libsdl2-2.0-0 libopenal1

Needed libraries on Ubuntu 14.04 64bit:
sudo apt-get install libsdl2-2.0-0:i386 libopenal1:i386 libgl1-mesa-glx:i386

Be sure to mark "jasphmd" as executable and start the game with it.


## Build the game

If you want to build the code yourself, just follow the instructions in Build.md.


## More information

For the DK2 support I had to use the OculusSDK. The OpenHMD library just doesn't support all the features. It is still possible to build the game with OpenHMD support. Just follow the instructions in the Build.md file.
The OculusSDK library is loaded during runtime and the game also works fine without the library. 

This version can also be played without any Head Mounted Device. All HMD tweaks will be disabled and the game should behave like the original version.

Before I started this project, I had a look at the rift support implementation from LeeN (mtbs3d forum). This helped me to get started. Thx LeeN.

Feel free to contact me: jochen.leopold@model-view.com
