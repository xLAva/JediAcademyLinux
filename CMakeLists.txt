#
# Build procedure:
#                $ mkdir some_build_dir
#                $ cd some_build_dir
#                $ cmake PATH_TO_JK2_SOURCE_DIR
#                $ make
#
# Make sure some_build_dir is different from PATH_TO_JK2_SOURCE_DIR
#

cmake_minimum_required(VERSION 2.6)

project(jka)

option(PANDORA "Set to ON if targeting an OpenPandora device" ${PANDORA})
option(JOYSTICK "Set to ON if you want Joystick support (Linux only)" ${JOYSTICK})
option(CROUCH "Set to ON if you want Toggle Crouch" ${CROUCH})

set(CMAKE_BUILD_TYPE "RELEASE")
# set(CMAKE_BUILD_TYPE "DEBUG")

#set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-write-strings -Wno-pointer-arith -Wno-multichar -Wno-int-to-pointer-cast -Wno-enum-compare -Wno-cpp -Wno-overflow -fexceptions -m32 -ffast-math -DLINUX -DNDEBUG")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-write-strings -Wno-conversion-null -Wno-pointer-arith -Wno-multichar -Wno-int-to-pointer-cast -Wno-enum-compare -Wno-cpp -Wno-overflow -fexceptions -m32 -ffast-math -fno-operator-names -DLINUX -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-write-strings -Wno-pointer-arith -Wno-multichar -Wno-int-to-pointer-cast -Wno-enum-compare -Wno-cpp -fexceptions -DLINUX -DNDEBUG -ffast-math -ftree-vectorize -marm  -fcx-fortran-rules -fstrict-aliasing -Wdouble-promotion -Ofast")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-write-strings -Wno-conversion-null -Wno-pointer-arith -Wno-multichar -Wno-int-to-pointer-cast -Wno-enum-compare -Wno-cpp -fexceptions -fno-operator-names -DLINUX -DNDEBUG -Wstrict-overflow=1 -ffast-math -ftree-vectorize -marm  -fcx-fortran-rules  -fstrict-aliasing -Wdouble-promotion -Ofast")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fsigned-char -fsingle-precision-constant")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fsigned-char -fsingle-precision-constant")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFINAL_BUILD ")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DFINAL_BUILD ")
#set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fsingle-precision-constant")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fsingle-precision-constant")
#set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -flto -fuse-linker-plugin")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto -fuse-linker-plugin")
if(PANDORA)
	add_definitions(-DPANDORA -DHAVE_GLES -DARM -DNEON -DAUTOAIM)
endif()
if (JOYSTICK)
	add_definitions(-DJOYSTICK)
endif()
if (CROUCH)
	add_definitions(-DCROUCH)
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -Wno-write-strings -Wno-pointer-arith -Wno-multichar -Wno-int-to-pointer-cast -Wno-enum-compare -Wno-cpp -Wno-overflow -g -fexceptions -m32 -ffast-math -DLINUX -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-write-strings -Wno-conversion-null -Wno-pointer-arith -Wno-multichar -Wno-int-to-pointer-cast -Wno-enum-compare -Wno-cpp -Wno-overflow -fexceptions -m32 -ffast-math -fno-operator-names -DLINUX -DNDEBUG")

set(GAME_SOURCE_FILES
                code/Ratl/ratl.cpp
                code/Ravl/CBounds.cpp
                code/Ravl/CVec.cpp
                code/Rufl/hfile.cpp
                code/Rufl/hstring.cpp
                code/Rufl/random.cpp
                code/cgame/FX_ATSTMain.cpp
                code/cgame/FX_Blaster.cpp
                code/cgame/FX_Bowcaster.cpp
                code/cgame/FX_BryarPistol.cpp
                code/cgame/FX_Concussion.cpp
                code/cgame/FX_DEMP2.cpp
                code/cgame/FX_Disruptor.cpp
                code/cgame/FX_Emplaced.cpp
                code/cgame/FX_Flechette.cpp
                code/cgame/FX_HeavyRepeater.cpp
                code/cgame/FX_NoghriShot.cpp
                code/cgame/FX_RocketLauncher.cpp
                code/cgame/FX_TuskenShot.cpp
                code/cgame/FxPrimitives.cpp
                code/cgame/FxScheduler.cpp
                code/cgame/FxSystem.cpp
                code/cgame/FxTemplate.cpp
                code/cgame/FxUtil.cpp
                code/cgame/cg_camera.cpp
                code/cgame/cg_consolecmds.cpp
                code/cgame/cg_credits.cpp
                code/cgame/cg_draw.cpp
                code/cgame/cg_drawtools.cpp
                code/cgame/cg_effects.cpp
                code/cgame/cg_ents.cpp
                code/cgame/cg_event.cpp
                code/cgame/cg_headers.cpp
                code/cgame/cg_info.cpp
                code/cgame/cg_lights.cpp
                code/cgame/cg_localents.cpp
                code/cgame/cg_main.cpp
                code/cgame/cg_marks.cpp
                code/cgame/cg_players.cpp
                code/cgame/cg_playerstate.cpp
                code/cgame/cg_predict.cpp
                code/cgame/cg_scoreboard.cpp
                code/cgame/cg_servercmds.cpp
                code/cgame/cg_snapshot.cpp
                code/cgame/cg_syscalls.cpp
                code/cgame/cg_text.cpp
                code/cgame/cg_view.cpp
                code/cgame/cg_weapons.cpp
                code/game/AI_Animal.cpp
                code/game/AI_AssassinDroid.cpp
                code/game/AI_Atst.cpp
                code/game/AI_BobaFett.cpp
                code/game/AI_Civilian.cpp
                code/game/AI_Default.cpp
                code/game/AI_Droid.cpp
                code/game/AI_GalakMech.cpp
                code/game/AI_Glider.cpp
                code/game/AI_Grenadier.cpp
                code/game/AI_HazardTrooper.cpp
                code/game/AI_Howler.cpp
                code/game/AI_ImperialProbe.cpp
                code/game/AI_Interrogator.cpp
                code/game/AI_Jedi.cpp
                code/game/AI_Mark1.cpp
                code/game/AI_Mark2.cpp
                code/game/AI_MineMonster.cpp
                code/game/AI_Rancor.cpp
                code/game/AI_Remote.cpp
                code/game/AI_RocketTrooper.cpp
                code/game/AI_SaberDroid.cpp
                code/game/AI_SandCreature.cpp
                code/game/AI_Seeker.cpp
                code/game/AI_Sentry.cpp
                code/game/AI_Sniper.cpp
                code/game/AI_Stormtrooper.cpp
                code/game/AI_Tusken.cpp
                code/game/AI_Utils.cpp
                code/game/AI_Wampa.cpp
                code/game/AnimalNPC.cpp
                code/game/FighterNPC.cpp
                code/game/G_Timer.cpp
                code/game/NPC.cpp
                code/game/NPC_behavior.cpp
                code/game/NPC_combat.cpp
                code/game/NPC_goal.cpp
                code/game/NPC_misc.cpp
                code/game/NPC_move.cpp
                code/game/NPC_reactions.cpp
                code/game/NPC_senses.cpp
                code/game/NPC_sounds.cpp
                code/game/NPC_spawn.cpp
                code/game/NPC_stats.cpp
                code/game/NPC_utils.cpp
                code/game/Q3_Interface.cpp
                code/game/SpeederNPC.cpp
                code/game/WalkerNPC.cpp
                code/game/bg_misc.cpp
                code/game/bg_pangles.cpp
                code/game/bg_panimate.cpp
                code/game/bg_pmove.cpp
                code/game/bg_slidemove.cpp
                code/game/bg_vehicleLoad.cpp
                code/game/g_active.cpp
                code/game/g_breakable.cpp
                code/game/g_camera.cpp
                code/game/g_client.cpp
                code/game/g_cmds.cpp
                code/game/g_combat.cpp
                code/game/g_emplaced.cpp
                code/game/g_functions.cpp
                code/game/g_fx.cpp
                code/game/g_headers.cpp
                code/game/g_inventory.cpp
                code/game/g_itemLoad.cpp
                code/game/g_items.cpp
                code/game/g_main.cpp
                code/game/g_mem.cpp
                code/game/g_misc.cpp
                code/game/g_misc_model.cpp
                code/game/g_missile.cpp
                code/game/g_mover.cpp
                code/game/g_nav.cpp
                code/game/g_navigator.cpp
                code/game/g_navnew.cpp
                code/game/g_object.cpp
                code/game/g_objectives.cpp
                code/game/g_rail.cpp
                code/game/g_ref.cpp
                code/game/g_roff.cpp
                code/game/g_savegame.cpp
                code/game/g_session.cpp
                code/game/g_spawn.cpp
                code/game/g_svcmds.cpp
                code/game/g_target.cpp
                code/game/g_trigger.cpp
                code/game/g_turret.cpp
                code/game/g_usable.cpp
                code/game/g_utils.cpp
                code/game/g_vehicles.cpp
                code/game/g_weapon.cpp
                code/game/g_weaponLoad.cpp
                code/game/game.def
                code/game/genericparser2.cpp
                code/game/q_math.cpp
                code/game/q_shared.cpp
                code/game/wp_saber.cpp
                code/game/wp_saberLoad.cpp
                code/icarus/BlockStream.cpp
                code/icarus/IcarusImplementation.cpp
                code/icarus/Sequence.cpp
                code/icarus/Sequencer.cpp
                code/icarus/TaskManager.cpp
                code/qcommon/tri_coll_test.cpp
                code/ui/gameinfo.cpp
)

if(PANDORA)
	add_library(jagamearm SHARED ${GAME_SOURCE_FILES})
	set_target_properties(jagamearm PROPERTIES PREFIX "")
else()
	add_library(jagamex86 SHARED ${GAME_SOURCE_FILES})
	set_target_properties(jagamex86 PROPERTIES PREFIX "")
endif()

set(STARWARS_SOURCE_FILES
	code/RMG/RM_Area.cpp
	code/RMG/RM_Instance.cpp
	code/RMG/RM_InstanceFile.cpp
	code/RMG/RM_Instance_BSP.cpp
	code/RMG/RM_Instance_Group.cpp
	code/RMG/RM_Instance_Random.cpp
	code/RMG/RM_Instance_Void.cpp
	code/RMG/RM_Manager.cpp
	code/RMG/RM_Mission.cpp
	code/RMG/RM_Objective.cpp
	code/RMG/RM_Path.cpp
	code/RMG/RM_Terrain.cpp
	code/client/cl_cgame.cpp
	code/client/cl_cin.cpp
	code/client/cl_console.cpp
	code/client/cl_input.cpp
	code/client/cl_keys.cpp
	code/client/cl_main.cpp
	code/client/cl_mp3.cpp
	code/client/cl_parse.cpp
	code/client/cl_scrn.cpp
	code/client/cl_ui.cpp
	code/client/snd_ambient.cpp
	code/client/snd_dma.cpp
	code/client/snd_mem.cpp
	code/client/snd_mix.cpp
	code/client/snd_music.cpp
	code/client/vmachine.cpp
	code/ff/cl_ff.cpp
	code/ff/ff.cpp
	code/ff/ff_ChannelSet.cpp
	code/ff/ff_ConfigParser.cpp
	code/ff/ff_HandleTable.cpp
	code/ff/ff_MultiCompound.cpp
	code/ff/ff_MultiEffect.cpp
	code/ff/ff_MultiSet.cpp
	code/ff/ff_ffset.cpp
	code/ff/ff_snd.cpp
	code/ff/ff_system.cpp
	code/ff/ff_utils.cpp
	code/game/genericparser2.cpp
	code/game/q_math.cpp
	code/game/q_shared.cpp
	code/ghoul2/G2_API.cpp
	code/ghoul2/G2_bolts.cpp
	code/ghoul2/G2_bones.cpp
	code/ghoul2/G2_misc.cpp
	code/ghoul2/G2_surfaces.cpp
	code/jpeg-6/jcapimin.cpp
	code/jpeg-6/jccoefct.cpp
	code/jpeg-6/jccolor.cpp
	code/jpeg-6/jcdctmgr.cpp
	code/jpeg-6/jchuff.cpp
	code/jpeg-6/jcinit.cpp
	code/jpeg-6/jcmainct.cpp
	code/jpeg-6/jcmarker.cpp
	code/jpeg-6/jcmaster.cpp
	code/jpeg-6/jcomapi.cpp
	code/jpeg-6/jcparam.cpp
	code/jpeg-6/jcphuff.cpp
	code/jpeg-6/jcprepct.cpp
	code/jpeg-6/jcsample.cpp
	code/jpeg-6/jctrans.cpp
	code/jpeg-6/jdapimin.cpp
	code/jpeg-6/jdapistd.cpp
	code/jpeg-6/jdatadst.cpp
	code/jpeg-6/jdatasrc.cpp
	code/jpeg-6/jdcoefct.cpp
	code/jpeg-6/jdcolor.cpp
	code/jpeg-6/jddctmgr.cpp
	code/jpeg-6/jdhuff.cpp
	code/jpeg-6/jdinput.cpp
	code/jpeg-6/jdmainct.cpp
	code/jpeg-6/jdmarker.cpp
	code/jpeg-6/jdmaster.cpp
	code/jpeg-6/jdpostct.cpp
	code/jpeg-6/jdsample.cpp
	code/jpeg-6/jdtrans.cpp
	code/jpeg-6/jerror.cpp
	code/jpeg-6/jfdctflt.cpp
	code/jpeg-6/jidctflt.cpp
	code/jpeg-6/jmemmgr.cpp
	code/jpeg-6/jmemnobs.cpp
	code/jpeg-6/jutils.cpp
	code/linux/linux_console.cpp
	code/linux/linux_glimp.cpp
	code/linux/linux_main.cpp
	code/linux/linux_qgl.cpp
	code/linux/linux_snddma.cpp
	code/linux/linux_gamma.cpp
	code/linux/xvidmode.cpp
	code/mp3code/cdct.c
	code/mp3code/csbt.c
	code/mp3code/csbtb.c
	code/mp3code/csbtl3.c
	code/mp3code/cup.c
	code/mp3code/cupini.c
	code/mp3code/cupl1.c
	code/mp3code/cupl3.c
	code/mp3code/cwin.c
	code/mp3code/cwinb.c
	code/mp3code/cwinm.c
	code/mp3code/hwin.c
	code/mp3code/l3dq.c
	code/mp3code/l3init.c
	code/mp3code/mdct.c
	code/mp3code/mhead.c
	code/mp3code/msis.c
	code/mp3code/towave.c
	code/mp3code/uph.c
	code/mp3code/upsf.c
	code/mp3code/wavep.c
	code/png/png.cpp
	code/qcommon/cm_draw.cpp
	code/qcommon/cm_load.cpp
	code/qcommon/cm_patch.cpp
	code/qcommon/cm_polylib.cpp
	code/qcommon/cm_randomterrain.cpp
	code/qcommon/cm_shader.cpp
	code/qcommon/cm_terrain.cpp
	code/qcommon/cm_terrainmap.cpp
	code/qcommon/cm_test.cpp
	code/qcommon/cm_trace.cpp
	code/qcommon/cmd.cpp
	code/qcommon/common.cpp
	code/qcommon/cvar.cpp
	code/qcommon/files_common.cpp
	code/qcommon/files_pc.cpp
	code/qcommon/md4.cpp
	code/qcommon/msg.cpp
	code/qcommon/net_chan.cpp
	code/qcommon/stringed_ingame.cpp
	code/qcommon/stringed_interface.cpp
	code/qcommon/unzip.cpp
	code/qcommon/z_memman_pc.cpp
	code/renderer/matcomp.c
	code/renderer/tr_WorldEffects.cpp
	code/renderer/tr_arioche.cpp
	code/renderer/tr_backend.cpp
	code/renderer/tr_bsp.cpp
	code/renderer/tr_cmds.cpp
	code/renderer/tr_curve.cpp
	code/renderer/tr_draw.cpp
	code/renderer/tr_font.cpp
	code/renderer/tr_ghoul2.cpp
	code/renderer/tr_image.cpp
	code/renderer/tr_init.cpp
	code/renderer/tr_jpeg_interface.cpp
	code/renderer/tr_light.cpp
	code/renderer/tr_main.cpp
	code/renderer/tr_marks.cpp
	code/renderer/tr_mesh.cpp
	code/renderer/tr_model.cpp
	code/renderer/tr_noise.cpp
	code/renderer/tr_quicksprite.cpp
	code/renderer/tr_scene.cpp
	code/renderer/tr_shade.cpp
	code/renderer/tr_shade_calc.cpp
	code/renderer/tr_shader.cpp
	code/renderer/tr_shadows.cpp
	code/renderer/tr_sky.cpp
	code/renderer/tr_stl.cpp
	code/renderer/tr_surface.cpp
	code/renderer/tr_surfacesprites.cpp
	code/renderer/tr_terrain.cpp
	code/renderer/tr_world.cpp
	code/server/exe_headers.cpp
	code/server/sv_ccmds.cpp
	code/server/sv_client.cpp
	code/server/sv_game.cpp
	code/server/sv_init.cpp
	code/server/sv_main.cpp
	code/server/sv_savegame.cpp
	code/server/sv_snapshot.cpp
	code/server/sv_world.cpp
	code/ui/ui_atoms.cpp
	code/ui/ui_connect.cpp
	code/ui/ui_main.cpp
	code/ui/ui_saber.cpp
	code/ui/ui_shared.cpp
	code/ui/ui_syscalls.cpp
)

set (EXTRA_LIBS ${EXTRA_LIBS} dl)
set (EXTRA_LIBS ${EXTRA_LIBS} openal)
if(PANDORA)
	set(EXTRA_LIBS ${EXTRA_LIBS} GLES_CM EGL)
	set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "-Ofast -marm -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp")
else(PANDORA)
	set (EXTRA_LIBS ${EXTRA_LIBS} GL)
	set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "-m32")
endif(PANDORA)
set (EXTRA_LIBS ${EXTRA_LIBS} X11)
set (EXTRA_LIBS ${EXTRA_LIBS} Xxf86vm)
set (EXTRA_LIBS ${EXTRA_LIBS} Xrandr)
set (EXTRA_LIBS ${EXTRA_LIBS} rt)
set (EXTRA_LIBS ${EXTRA_LIBS} pthread)
set (EXTRA_LIBS ${EXTRA_LIBS} z)

if(PANDORA)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -fpermissive -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Ofast -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp")
	add_definitions(-mcpu=cortex-a8)
	add_definitions(-mfpu=neon)
	add_definitions(-mfloat-abi=softfp)
	add_definitions(-march=armv7-a)
	add_definitions(-ftree-vectorize)
endif(PANDORA)
if(JOYSTICK)
	set (STARWARS_SOURCE_FILES ${STARWARS_SOURCE_FILES} code/linux/linux_joystick.cpp)
endif()

add_executable(jasp ${STARWARS_SOURCE_FILES})
target_link_libraries (jasp  ${EXTRA_LIBS})
set_target_properties (jasp PROPERTIES COMPILE_DEFINITIONS "_JK2EXE;EAX_DISABLED;_M_IX86")
