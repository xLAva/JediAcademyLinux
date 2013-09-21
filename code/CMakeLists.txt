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

project(jk2)

set(CMAKE_BUILD_TYPE "RELEASE")
# set(CMAKE_BUILD_TYPE "DEBUG")

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-write-strings -Wno-pointer-arith -Wno-multichar -Wno-int-to-pointer-cast -Wno-enum-compare -Wno-cpp -Wno-overflow -fexceptions -m32 -ffast-math -DLINUX -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-write-strings -Wno-conversion-null -Wno-pointer-arith -Wno-multichar -Wno-int-to-pointer-cast -Wno-enum-compare -Wno-cpp -Wno-overflow -fexceptions -m32 -ffast-math -fno-operator-names -DLINUX -DNDEBUG")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -Wno-write-strings -Wno-pointer-arith -Wno-multichar -Wno-int-to-pointer-cast -Wno-enum-compare -Wno-cpp -Wno-overflow -g -fexceptions -m32 -ffast-math -DLINUX -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-write-strings -Wno-conversion-null -Wno-pointer-arith -Wno-multichar -Wno-int-to-pointer-cast -Wno-enum-compare -Wno-cpp -Wno-overflow -fexceptions -m32 -ffast-math -fno-operator-names -DLINUX -DNDEBUG")

set(GAME_SOURCE_FILES
                Ratl/ratl.cpp
                Ravl/CBounds.cpp
                Ravl/CVec.cpp
                Rufl/hfile.cpp
                Rufl/hstring.cpp
                Rufl/random.cpp
                cgame/FX_ATSTMain.cpp
                cgame/FX_Blaster.cpp
                cgame/FX_Bowcaster.cpp
                cgame/FX_BryarPistol.cpp
                cgame/FX_Concussion.cpp
                cgame/FX_DEMP2.cpp
                cgame/FX_Disruptor.cpp
                cgame/FX_Emplaced.cpp
                cgame/FX_Flechette.cpp
                cgame/FX_HeavyRepeater.cpp
                cgame/FX_NoghriShot.cpp
                cgame/FX_RocketLauncher.cpp
                cgame/FX_TuskenShot.cpp
                cgame/FxPrimitives.cpp
                cgame/FxScheduler.cpp
                cgame/FxSystem.cpp
                cgame/FxTemplate.cpp
                cgame/FxUtil.cpp
                cgame/cg_camera.cpp
                cgame/cg_consolecmds.cpp
                cgame/cg_credits.cpp
                cgame/cg_draw.cpp
                cgame/cg_drawtools.cpp
                cgame/cg_effects.cpp
                cgame/cg_ents.cpp
                cgame/cg_event.cpp
                cgame/cg_headers.cpp
                cgame/cg_info.cpp
                cgame/cg_lights.cpp
                cgame/cg_localents.cpp
                cgame/cg_main.cpp
                cgame/cg_marks.cpp
                cgame/cg_players.cpp
                cgame/cg_playerstate.cpp
                cgame/cg_predict.cpp
                cgame/cg_scoreboard.cpp
                cgame/cg_servercmds.cpp
                cgame/cg_snapshot.cpp
                cgame/cg_syscalls.cpp
                cgame/cg_text.cpp
                cgame/cg_view.cpp
                cgame/cg_weapons.cpp
                game/AI_Animal.cpp
                game/AI_AssassinDroid.cpp
                game/AI_Atst.cpp
                game/AI_BobaFett.cpp
                game/AI_Civilian.cpp
                game/AI_Default.cpp
                game/AI_Droid.cpp
                game/AI_GalakMech.cpp
                game/AI_Glider.cpp
                game/AI_Grenadier.cpp
                game/AI_HazardTrooper.cpp
                game/AI_Howler.cpp
                game/AI_ImperialProbe.cpp
                game/AI_Interrogator.cpp
                game/AI_Jedi.cpp
                game/AI_Mark1.cpp
                game/AI_Mark2.cpp
                game/AI_MineMonster.cpp
                game/AI_Rancor.cpp
                game/AI_Remote.cpp
                game/AI_RocketTrooper.cpp
                game/AI_SaberDroid.cpp
                game/AI_SandCreature.cpp
                game/AI_Seeker.cpp
                game/AI_Sentry.cpp
                game/AI_Sniper.cpp
                game/AI_Stormtrooper.cpp
                game/AI_Tusken.cpp
                game/AI_Utils.cpp
                game/AI_Wampa.cpp
                game/AnimalNPC.cpp
                game/FighterNPC.cpp
                game/G_Timer.cpp
                game/NPC.cpp
                game/NPC_behavior.cpp
                game/NPC_combat.cpp
                game/NPC_goal.cpp
                game/NPC_misc.cpp
                game/NPC_move.cpp
                game/NPC_reactions.cpp
                game/NPC_senses.cpp
                game/NPC_sounds.cpp
                game/NPC_spawn.cpp
                game/NPC_stats.cpp
                game/NPC_utils.cpp
                game/Q3_Interface.cpp
                game/SpeederNPC.cpp
                game/WalkerNPC.cpp
                game/bg_misc.cpp
                game/bg_pangles.cpp
                game/bg_panimate.cpp
                game/bg_pmove.cpp
                game/bg_slidemove.cpp
                game/bg_vehicleLoad.cpp
                game/g_active.cpp
                game/g_breakable.cpp
                game/g_camera.cpp
                game/g_client.cpp
                game/g_cmds.cpp
                game/g_combat.cpp
                game/g_emplaced.cpp
                game/g_functions.cpp
                game/g_fx.cpp
                game/g_headers.cpp
                game/g_inventory.cpp
                game/g_itemLoad.cpp
                game/g_items.cpp
                game/g_main.cpp
                game/g_mem.cpp
                game/g_misc.cpp
                game/g_misc_model.cpp
                game/g_missile.cpp
                game/g_mover.cpp
                game/g_nav.cpp
                game/g_navigator.cpp
                game/g_navnew.cpp
                game/g_object.cpp
                game/g_objectives.cpp
                game/g_rail.cpp
                game/g_ref.cpp
                game/g_roff.cpp
                game/g_savegame.cpp
                game/g_session.cpp
                game/g_spawn.cpp
                game/g_svcmds.cpp
                game/g_target.cpp
                game/g_trigger.cpp
                game/g_turret.cpp
                game/g_usable.cpp
                game/g_utils.cpp
                game/g_vehicles.cpp
                game/g_weapon.cpp
                game/g_weaponLoad.cpp
                game/game.def
                game/genericparser2.cpp
                game/q_math.cpp
                game/q_shared.cpp
                game/wp_saber.cpp
                game/wp_saberLoad.cpp
                icarus/BlockStream.cpp
                icarus/IcarusImplementation.cpp
                icarus/Sequence.cpp
                icarus/Sequencer.cpp
                icarus/TaskManager.cpp
                qcommon/tri_coll_test.cpp
                ui/gameinfo.cpp
)

add_library(jagamex86 SHARED ${GAME_SOURCE_FILES})
set_target_properties(jagamex86 PROPERTIES PREFIX "")

set(STARWARS_SOURCE_FILES
	RMG/RM_Area.cpp
	RMG/RM_Instance.cpp
	RMG/RM_InstanceFile.cpp
	RMG/RM_Instance_BSP.cpp
	RMG/RM_Instance_Group.cpp
	RMG/RM_Instance_Random.cpp
	RMG/RM_Instance_Void.cpp
	RMG/RM_Manager.cpp
	RMG/RM_Mission.cpp
	RMG/RM_Objective.cpp
	RMG/RM_Path.cpp
	RMG/RM_Terrain.cpp
	client/cl_cgame.cpp
	client/cl_cin.cpp
	client/cl_console.cpp
	client/cl_input.cpp
	client/cl_keys.cpp
	client/cl_main.cpp
	client/cl_mp3.cpp
	client/cl_parse.cpp
	client/cl_scrn.cpp
	client/cl_ui.cpp
	client/snd_ambient.cpp
	client/snd_dma.cpp
	client/snd_mem.cpp
	client/snd_mix.cpp
	client/snd_music.cpp
	client/vmachine.cpp
	ff/cl_ff.cpp
	ff/ff.cpp
	ff/ff_ChannelSet.cpp
	ff/ff_ConfigParser.cpp
	ff/ff_HandleTable.cpp
	ff/ff_MultiCompound.cpp
	ff/ff_MultiEffect.cpp
	ff/ff_MultiSet.cpp
	ff/ff_ffset.cpp
	ff/ff_snd.cpp
	ff/ff_system.cpp
	ff/ff_utils.cpp
	game/genericparser2.cpp
	game/q_math.cpp
	game/q_shared.cpp
	ghoul2/G2_API.cpp
	ghoul2/G2_bolts.cpp
	ghoul2/G2_bones.cpp
	ghoul2/G2_misc.cpp
	ghoul2/G2_surfaces.cpp
	jpeg-6/jcapimin.cpp
	jpeg-6/jccoefct.cpp
	jpeg-6/jccolor.cpp
	jpeg-6/jcdctmgr.cpp
	jpeg-6/jchuff.cpp
	jpeg-6/jcinit.cpp
	jpeg-6/jcmainct.cpp
	jpeg-6/jcmarker.cpp
	jpeg-6/jcmaster.cpp
	jpeg-6/jcomapi.cpp
	jpeg-6/jcparam.cpp
	jpeg-6/jcphuff.cpp
	jpeg-6/jcprepct.cpp
	jpeg-6/jcsample.cpp
	jpeg-6/jctrans.cpp
	jpeg-6/jdapimin.cpp
	jpeg-6/jdapistd.cpp
	jpeg-6/jdatadst.cpp
	jpeg-6/jdatasrc.cpp
	jpeg-6/jdcoefct.cpp
	jpeg-6/jdcolor.cpp
	jpeg-6/jddctmgr.cpp
	jpeg-6/jdhuff.cpp
	jpeg-6/jdinput.cpp
	jpeg-6/jdmainct.cpp
	jpeg-6/jdmarker.cpp
	jpeg-6/jdmaster.cpp
	jpeg-6/jdpostct.cpp
	jpeg-6/jdsample.cpp
	jpeg-6/jdtrans.cpp
	jpeg-6/jerror.cpp
	jpeg-6/jfdctflt.cpp
	jpeg-6/jidctflt.cpp
	jpeg-6/jmemmgr.cpp
	jpeg-6/jmemnobs.cpp
	jpeg-6/jutils.cpp
	linux/linux_console.cpp
	linux/linux_gamma.cpp
	linux/linux_glimp.cpp
	linux/linux_main.cpp
	linux/linux_qgl.cpp
	linux/linux_snddma.cpp
	linux/xvidmode.cpp
	mp3code/cdct.c
	mp3code/csbt.c
	mp3code/csbtb.c
	mp3code/csbtl3.c
	mp3code/cup.c
	mp3code/cupini.c
	mp3code/cupl1.c
	mp3code/cupl3.c
	mp3code/cwin.c
	mp3code/cwinb.c
	mp3code/cwinm.c
	mp3code/hwin.c
	mp3code/l3dq.c
	mp3code/l3init.c
	mp3code/mdct.c
	mp3code/mhead.c
	mp3code/msis.c
	mp3code/towave.c
	mp3code/uph.c
	mp3code/upsf.c
	mp3code/wavep.c
	png/png.cpp
	qcommon/cm_draw.cpp
	qcommon/cm_load.cpp
	qcommon/cm_patch.cpp
	qcommon/cm_polylib.cpp
	qcommon/cm_randomterrain.cpp
	qcommon/cm_shader.cpp
	qcommon/cm_terrain.cpp
	qcommon/cm_terrainmap.cpp
	qcommon/cm_test.cpp
	qcommon/cm_trace.cpp
	qcommon/cmd.cpp
	qcommon/common.cpp
	qcommon/cvar.cpp
	qcommon/files_common.cpp
	qcommon/files_pc.cpp
	qcommon/md4.cpp
	qcommon/msg.cpp
	qcommon/net_chan.cpp
	qcommon/stringed_ingame.cpp
	qcommon/stringed_interface.cpp
	qcommon/unzip.cpp
	qcommon/z_memman_pc.cpp
	renderer/matcomp.c
	renderer/tr_WorldEffects.cpp
	renderer/tr_arioche.cpp
	renderer/tr_backend.cpp
	renderer/tr_bsp.cpp
	renderer/tr_cmds.cpp
	renderer/tr_curve.cpp
	renderer/tr_draw.cpp
	renderer/tr_font.cpp
	renderer/tr_ghoul2.cpp
	renderer/tr_image.cpp
	renderer/tr_init.cpp
	renderer/tr_jpeg_interface.cpp
	renderer/tr_light.cpp
	renderer/tr_main.cpp
	renderer/tr_marks.cpp
	renderer/tr_mesh.cpp
	renderer/tr_model.cpp
	renderer/tr_noise.cpp
	renderer/tr_quicksprite.cpp
	renderer/tr_scene.cpp
	renderer/tr_shade.cpp
	renderer/tr_shade_calc.cpp
	renderer/tr_shader.cpp
	renderer/tr_shadows.cpp
	renderer/tr_sky.cpp
	renderer/tr_stl.cpp
	renderer/tr_surface.cpp
	renderer/tr_surfacesprites.cpp
	renderer/tr_terrain.cpp
	renderer/tr_world.cpp
	server/exe_headers.cpp
	server/sv_ccmds.cpp
	server/sv_client.cpp
	server/sv_game.cpp
	server/sv_init.cpp
	server/sv_main.cpp
	server/sv_savegame.cpp
	server/sv_snapshot.cpp
	server/sv_world.cpp
	ui/ui_atoms.cpp
	ui/ui_connect.cpp
	ui/ui_main.cpp
	ui/ui_saber.cpp
	ui/ui_shared.cpp
	ui/ui_syscalls.cpp
)

set (EXTRA_LIBS ${EXTRA_LIBS} dl)
set (EXTRA_LIBS ${EXTRA_LIBS} openal)
set (EXTRA_LIBS ${EXTRA_LIBS} GL)
set (EXTRA_LIBS ${EXTRA_LIBS} X11)
set (EXTRA_LIBS ${EXTRA_LIBS} Xxf86vm)
set (EXTRA_LIBS ${EXTRA_LIBS} Xrandr)
set (EXTRA_LIBS ${EXTRA_LIBS} rt)
set (EXTRA_LIBS ${EXTRA_LIBS} pthread)
set (EXTRA_LIBS ${EXTRA_LIBS} z)
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "-m32")

add_executable(jasp ${STARWARS_SOURCE_FILES})
target_link_libraries (jasp  ${EXTRA_LIBS})
set_target_properties (jasp PROPERTIES COMPILE_DEFINITIONS "_JK2EXE;EAX_DISABLED;_M_IX86")
