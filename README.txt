JediAcademyLinux
================

Jedi Knight III: Jedi Academy (Single Player Linux Port).
https://github.com/xLAva/JediAcademyLinux

The current state is playable without any major problems.
I didn't play the whole game yet, but I tested at least more than half of the levels. 


OpenPandora
===========

This fork is aimed at OpenPandra, so get:
 * ARM compatibilty (using -DARM)
 * Some NEON optimized code (using -DNEON)
 * GLES renderer (using -DHAVE_GLES)
 * Joystick support (using -DJOYSTICK)
 * Toggle Crouch function (using -DCROUCH)
 * Aiming assist, usefull when aiming with joystick or joystick like devices (single player only, there are changes in the server side) (using -DAUTOAIM)
 * OpenPandora support of course (using -DPANDORA), for screen resolution mainly.
 
For the Pandora version, I used only the CMake  make files, I didn't update the codeblock project file. I use the Code::Blocks PND to compile.

For more info on the OpenPandora go here: http://boards.openpandora.org/
Specific thread for Jedi Academy on the OpenPandora here: http://boards.openpandora.org/index.php/topic/13664-jedi-knight-3-jedi-academy/
