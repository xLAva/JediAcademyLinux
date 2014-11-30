#
# Build procedure:
#                $ mkdir some_build_dir
#                $ cd some_build_dir
#                $ cmake PATH_TO_JK3_SOURCE_DIR
#                $ make
#
# Make sure some_build_dir is different from PATH_TO_JK3_SOURCE_DIR
#

cmake_minimum_required(VERSION 2.6)

project(jk3)

set(USE_OCULUS_SDK 1)
set(FORCE_STATIC_OCULUS_SDK 0)

set(USE_OPENHMD 1)

set(USE_SDL2 1)



set(PLATFORM_WIN32 0)
set(PLATFORM_MACOS 0)
set(PLATFORM_LINUX 0)

message("system name: ${CMAKE_SYSTEM_NAME}")

if(WIN32)
    set(PLATFORM_WIN32 1)
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    set(PLATFORM_MACOS 1)
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    set(PLATFORM_LINUX 1)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "RELEASE")
endif()

message("Detected platform: Win32=${PLATFORM_WIN32} MacOS=${PLATFORM_MACOS} Linux=${PLATFORM_LINUX}")

# msvc: Debug or Release is selected inside VisualStudio, so don't show the CMAKE_BUILD_TYPE
if(NOT MSVC)
    message("Build Type: ${CMAKE_BUILD_TYPE}")
endif()

set(CUSTOM_COMPILE_FLAGS_ALL "")



# beginning of the game library

set(GSF_RATL
    Ratl/array_vs.h
    Ratl/bits_vs.h
    Ratl/grid_vs.h
    Ratl/handle_pool_vs.h
    Ratl/hash_pool_vs.h
    Ratl/heap_vs.h
    Ratl/list_vs.h
    Ratl/map_vs.h
    Ratl/pool_vs.h
    Ratl/queue_vs.h
    Ratl/ratl_common.h
    Ratl/scheduler_vs.h
    Ratl/stack_vs.h
    Ratl/string_vs.h
    Ratl/vector_vs.h
    Ratl/ratl.cpp
)

set(GSF_RAVL
    Ravl/CBounds.h
    Ravl/CMatrix.h
    Ravl/CVec.h
    Ravl/CBounds.cpp
    Ravl/CVec.cpp
)

set(GSF_RUFL
    Rufl/hfile.h
    Rufl/hstring.h
    Rufl/random.h
    Rufl/hfile.cpp
    Rufl/hstring.cpp
    Rufl/random.cpp
)

set(GSF_CGAME
    cgame/animtable.h
    cgame/cg_camera.h
    cgame/cg_headers.h
    cgame/cg_lights.h
    cgame/cg_local.h
    cgame/cg_media.h
    cgame/cg_public.h
    cgame/common_headers.h
    cgame/FxParsing.h
    cgame/FxPrimitives.h
    cgame/FxScheduler.h
    cgame/FxSystem.h
    cgame/FxUtil.h
    cgame/strip_objectives.h
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
)

set(GSF_GAME
    game/AI.h
    game/anims.h
    game/bg_local.h
    game/bg_public.h
    game/bset.h
    game/bstate.h
    game/b_local.h
    game/b_public.h
    game/channels.h
    game/characters.h
    game/common_headers.h
    game/dmstates.h
    game/events.h
    game/fields.h
    game/genericparser2.h
    game/ghoul2_shared.h
    game/g_functions.h
    game/g_headers.h
    game/g_items.h
    game/g_local.h
    game/g_nav.h
    game/g_navigator.h
    game/g_public.h
    game/g_roff.h
    game/g_shared.h
    game/g_vehicles.h
    game/hitlocs.h
    game/npc_headers.h
    game/objectives.h
    game/Q3_Interface.h
    game/q_shared.h
    game/say.h
    game/statindex.h
    game/surfaceflags.h
    game/teams.h
    game/weapons.h
    game/wp_saber.h
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
)

set(GSF_ICARUS
    icarus/BlockStream.h
    icarus/IcarusImplementation.h
    icarus/IcarusInterface.h
    icarus/Sequence.h
    icarus/Sequencer.h
    icarus/StdAfx.h
    icarus/TaskManager.h
    icarus/BlockStream.cpp
    icarus/IcarusImplementation.cpp
    icarus/Sequence.cpp
    icarus/Sequencer.cpp
    icarus/TaskManager.cpp
)

set(GSF_QCOMMON 
    qcommon/tri_coll_test.cpp
)

set(GSF_UI 
    ui/gameinfo.cpp
)

set(GSF_HMD
    hmd/GameHmd.h
    hmd/GameHmd.cpp
)

set(GSF_MAC
    mac/mac_game_lib.cpp
)

# make folders in MSVC project
source_group(Ratl FILES ${GSF_RATL})
source_group(Ravl FILES ${GSF_RAVL})
source_group(Rufl FILES ${GSF_RUFL})
source_group(cgame FILES ${GSF_CGAME})
source_group(game FILES ${GSF_GAME})
source_group(icarus FILES ${GSF_ICARUS})
source_group(qcommon FILES ${GSF_QCOMMON})
source_group(ui FILES ${GSF_UI})
source_group(hmd FILES ${GSF_HMD})
source_group(mac FILES ${GSF_MAC})

# combine all the client library source files
set(GAME_SOURCE_FILES
    ${GSF_RATL}
    ${GSF_RAVL}
    ${GSF_RUFL}
    ${GSF_CGAME}
    ${GSF_GAME}
    ${GSF_ICARUS}
    ${GSF_QCOMMON}
    ${GSF_UI}
    ${GSF_HMD}
)

if(PLATFORM_MACOS)
    set(GAME_SOURCE_FILES
        ${GAME_SOURCE_FILES}
        ${GSF_MAC}
    )
endif()



# beginning of the game exe

set(SSF_RMG
    RMG/RM_Area.h
    RMG/RM_Headers.h
    RMG/RM_Instance_BSP.h
    RMG/RM_InstanceFile.h
    RMG/RM_Instance_Group.h
    RMG/RM_Instance.h
    RMG/RM_Instance_Random.h
    RMG/RM_Instance_Void.h
    RMG/RM_Manager.h
    RMG/RM_Mission.h
    RMG/RM_Objective.h
    RMG/RM_Path.h
    RMG/RM_Terrain.h
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
)

set(SSF_CLIENT
    client/client.h
    client/client_ui.h
    client/cl_input_hotswap.h
    client/cl_mp3.h
    client/fffx.h
    client/keycodes.h
    client/keys.h
    client/snd_ambient.h
    client/snd_local_console.h
    client/snd_local.h
    client/snd_music.h
    client/snd_public.h
    client/vmachine.h
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
)

set(SSF_FF
    ff/cl_ff.h
    ff/common_headers.h
    ff/ff_ChannelCompound.h
    ff/ff_ChannelSet.h
    ff/ff_ConfigParser.h
    ff/ff_ffset.h
    ff/ff.h
    ff/ff_HandleTable.h
    ff/ff_local.h
    ff/ff_MultiCompound.h
    ff/ff_MultiEffect.h
    ff/ff_MultiSet.h
    ff/ff_public.h
    ff/ff_snd.h
    ff/ff_system.h
    ff/ff_utils.h
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
)

set(SSF_GAME
	game/genericparser2.cpp
	game/q_math.cpp
	game/q_shared.cpp
)

set(SSF_GHOUL2
    ghoul2/G2.h
    ghoul2/ghoul2_gore.h
	ghoul2/G2_API.cpp
	ghoul2/G2_bolts.cpp
	ghoul2/G2_bones.cpp
	ghoul2/G2_misc.cpp
	ghoul2/G2_surfaces.cpp
)

set(SSF_JPEG-6
    jpeg-6/jchuff.h
    jpeg-6/jconfig.h
    jpeg-6/jdct.h
    jpeg-6/jdhuff.h
    jpeg-6/jerror.h
    jpeg-6/jinclude.h
    jpeg-6/jmemsys.h
    jpeg-6/jmorecfg.h
    jpeg-6/jpegint.h
    jpeg-6/jpeglib.h
    jpeg-6/jversion.h
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
)

set(SSF_MP3CODE
    mp3code/config.h
    mp3code/htable.h
    mp3code/jdw.h
    mp3code/l3.h
    mp3code/mhead.h
    mp3code/mp3struct.h
    mp3code/port.h
    mp3code/small_header.h
    mp3code/tableawd.h
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
)

set(SSF_PNG
    png/png.h
	png/png.cpp
)

set(SSF_QCOMMON
    qcommon/chash.h
    qcommon/cm_draw.h
    qcommon/cm_landscape.h
    qcommon/cm_local.h
    qcommon/cm_patch.h
    qcommon/cm_polylib.h
    qcommon/cm_public.h
    qcommon/cm_randomterrain.h
    qcommon/cm_terrainmap.h
    qcommon/files.h
    qcommon/fixedmap.h
    qcommon/hstring.h
    qcommon/MiniHeap.h
    qcommon/platform.h
    qcommon/qcommon.h
    qcommon/qfiles.h
    qcommon/sstring.h
    qcommon/stringed_ingame.h
    qcommon/stringed_interface.h
    qcommon/stv_version.h
    qcommon/tags.h
    qcommon/timing.h
    qcommon/unzip.h
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
)

set(SSF_RENDERER
    renderer/amd3d.h
    renderer/glext_console.h
    renderer/glext.h
    renderer/matcomp.h
    renderer/mdx_format.h
    renderer/qgl_console.h
    renderer/qgl.h
    renderer/qgl_linked.h
    renderer/tr_font.h
    renderer/tr_jpeg_interface.h
    renderer/tr_landscape.h
    renderer/tr_local.h
    renderer/tr_public.h
    renderer/tr_quicksprite.h
    renderer/tr_stl.h
    renderer/tr_types.h
    renderer/tr_WorldEffects.h
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
)

set(SSF_SERVER
    server/exe_headers.h
    server/server.h
	server/exe_headers.cpp
	server/sv_ccmds.cpp
	server/sv_client.cpp
	server/sv_game.cpp
	server/sv_init.cpp
	server/sv_main.cpp
	server/sv_savegame.cpp
	server/sv_snapshot.cpp
	server/sv_world.cpp
)

set(SSF_UI
    ui/gameinfo.h
    ui/menudef.h
    ui/ui_local.h
    ui/ui_public.h
    ui/ui_shared.h
    ui/ui_splash.h
	ui/ui_atoms.cpp
	ui/ui_connect.cpp
	ui/ui_main.cpp
	ui/ui_saber.cpp
	ui/ui_shared.cpp
	ui/ui_syscalls.cpp
)

set(SSF_HMD
	hmd/ClientHmd.h
	hmd/ClientHmd.cpp
	hmd/FactoryHmdDevice.h
	hmd/FactoryHmdDevice.cpp
    hmd/SearchForDisplay.h
    hmd/SearchForDisplay.cpp
)

set(SSF_HMD_DEVICE
	hmd/HmdDevice/IHmdDevice.h
	hmd/HmdDevice/HmdDeviceMouse.h
	hmd/HmdDevice/HmdDeviceMouse.cpp
)

set(SSF_HMD_RENDERER
    hmd/HmdRenderer/IHmdRenderer.h
    hmd/HmdRenderer/HmdRendererOculus.h
    hmd/HmdRenderer/HmdRendererOculus.cpp
    hmd/HmdRenderer/PlatformInfo.h
    hmd/HmdRenderer/RenderTool.h
    hmd/HmdRenderer/RenderTool.cpp
)

set(SSF_HMD_QUAKE3
    hmd/Quake3/ViewParamsHmdUtility.h
    hmd/Quake3/ViewParamsHmdUtility.cpp
)


if(USE_OCULUS_SDK)
    set(SSF_HMD_DEVICE ${SSF_HMD_DEVICE}
        hmd/HmdDevice/HmdDeviceOculusSdk.h
        hmd/HmdDevice/HmdDeviceOculusSdk.cpp
    )
    set(SSF_HMD_RENDERER ${SSF_HMD_RENDERER}
        hmd/HmdRenderer/HmdRendererOculusSdk.h
        hmd/HmdRenderer/HmdRendererOculusSdk.cpp
    )

    set(CUSTOM_COMPILE_FLAGS_ALL "${CUSTOM_COMPILE_FLAGS_ALL} -DUSE_OVR")


        if(PLATFORM_WIN32)
			include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/OculusSDK/OculusSDK/LibOVR/Src")

			if (FORCE_STATIC_OCULUS_SDK)
				set(CUSTOM_COMPILE_FLAGS_ALL "${CUSTOM_COMPILE_FLAGS_ALL} -DFORCE_STATIC_OCULUS_SDK")

				set(SSF_HMD_LIBOVRDYNAMIC ${SSF_HMD_LIBOVRDYNAMIC}
					hmd/LibOvrDynamic/oculus_static.h
				)

				link_directories("C:/WinDDK/7600.16385.1/lib/ATL/i386")
				set(EXTRA_LIBS ${EXTRA_LIBS} wsock32 ws2_32)

				set(EXTRA_LIBS ${EXTRA_LIBS} debug ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/OculusSDK/OculusSDK/LibOVR/Lib/Win32/VS2013/libovrd.lib)
				set(EXTRA_LIBS ${EXTRA_LIBS} optimized ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/OculusSDK/OculusSDK/LibOVR/Lib/Win32/VS2013/libovr.lib)
			else()

				set(SSF_HMD_LIBOVRDYNAMIC ${SSF_HMD_LIBOVRDYNAMIC}
					hmd/LibOvrDynamic/oculus_dynamic.h
					hmd/LibOvrDynamic/oculus_dynamic.cpp
					hmd/LibOvrDynamic/ovr_dynamic_funcs.h
				)

			endif()

        elseif(PLATFORM_LINUX)
			include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/ovr_sdk_linux_0.4.3/LibOVR/Src")

			if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
				set(EXTRA_LIBS ${EXTRA_LIBS} ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/ovr_sdk_linux_0.4.3/LibOVR/Lib/Linux/Debug/i386/libovr.a)
			else()
				set(EXTRA_LIBS ${EXTRA_LIBS} ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/ovr_sdk_linux_0.4.3/LibOVR/Lib/Linux/Release/i386/libovr.a)
			endif()

			set(EXTRA_LIBS ${EXTRA_LIBS} udev)
			set(EXTRA_LIBS ${EXTRA_LIBS} Xinerama)
        elseif(PLATFORM_MACOS)
        include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/OculusSDK/LibOVR/Src")

                if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
                        set(EXTRA_LIBS ${EXTRA_LIBS} ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/OculusSDK/LibOVR/Lib/MacOS/Debug/i386/libovr.a)
                else()
                        set(EXTRA_LIBS ${EXTRA_LIBS} ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/OculusSDK/LibOVR/Lib/MacOS/Release/i386/libovr.a)
                endif()

                set(EXTRA_LIBS ${EXTRA_LIBS} udev)
                set(EXTRA_LIBS ${EXTRA_LIBS} Xinerama)
	endif()

endif()

if(USE_OPENHMD)
    set(SSF_HMD_DEVICE ${SSF_HMD_DEVICE}
        hmd/HmdDevice/HmdDeviceOpenHmd.h
        hmd/HmdDevice/HmdDeviceOpenHmd.cpp
    )
    set(SSF_HMD_RENDERER ${SSF_HMD_RENDERER}
        hmd/HmdRenderer/HmdRendererOculusOpenHmd.h
        hmd/HmdRenderer/HmdRendererOculusOpenHmd.cpp
    )

    set(CUSTOM_COMPILE_FLAGS_ALL "${CUSTOM_COMPILE_FLAGS_ALL} -DUSE_OPENHMD -DOHMD_STATIC")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/OpenHMD/include")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/OpenHMD/src")


    if(PLATFORM_WIN32)
        add_library(imp_openhmd STATIC IMPORTED)
        set_property(TARGET imp_openhmd PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/OpenHMD/msvc/Release/openhmd.lib")
        set(EXTRA_LIBS ${EXTRA_LIBS} imp_openhmd)
    elseif(PLATFORM_LINUX)
        if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
            set(EXTRA_LIBS ${EXTRA_LIBS} "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/OpenHMD/linux/debug/i386/libopenhmd.a")
        else()
            set(EXTRA_LIBS ${EXTRA_LIBS} "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/OpenHMD/linux/release/i386/libopenhmd.a")
        endif()
    elseif(PLATFORM_MACOS)
        if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
            set(EXTRA_LIBS ${EXTRA_LIBS} "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/OpenHMD/macos/debug/i386/libopenhmd.a")
        else()
            set(EXTRA_LIBS ${EXTRA_LIBS} "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/OpenHMD/macos/release/i386/libopenhmd.a")
        endif()
    endif()


    if(UNIX)
            set(EXTRA_LIBS ${EXTRA_LIBS} hidapi-libusb)
    else()
            add_library(imp_hidapi STATIC IMPORTED)
            set_property(TARGET imp_hidapi PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/hidapi-0.7.0/windows/Release/hidapi.lib")
            set(EXTRA_LIBS ${EXTRA_LIBS} imp_hidapi)
    endif()
endif()


# make folders in MSVC project
source_group(RMG FILES ${SSF_RMG})
source_group(client FILES ${SSF_CLIENT})
source_group(ff FILES ${SSF_FF})
source_group(game FILES ${SSF_GAME})
source_group(ghoul2 FILES ${SSF_GHOUL2})
source_group(jpeg-6 FILES ${SSF_JPEG-6})
source_group(mp3code FILES ${SSF_MP3CODE})
source_group(mp3code FILES ${SSF_PNG})
source_group(qcommon FILES ${SSF_QCOMMON})
source_group(qcommon FILES ${SSF_RENDERER})
source_group(server FILES ${SSF_SERVER})
source_group(ui FILES ${SSF_UI})
source_group(hmd FILES ${SSF_HMD})
source_group(hmd\\HmdDevice FILES ${SSF_HMD_DEVICE})
source_group(hmd\\HmdRenderer FILES ${SSF_HMD_RENDERER})
source_group(hmd\\LibOvrDynamic FILES ${SSF_HMD_LIBOVRDYNAMIC})


# combine all common source files for the game exe
set(STARWARS_SOURCE_FILES ${STARWARS_SOURCE_FILES}
    ${SSF_RMG}
    ${SSF_CLIENT}
    ${SSF_FF}
    ${SSF_GAME}
    ${SSF_GHOUL2}
    ${SSF_JPEG-6}
    ${SSF_MP3CODE}
    ${SSF_PNG}
    ${SSF_QCOMMON}
    ${SSF_RENDERER}
    ${SSF_SERVER}
    ${SSF_UI}
    ${SSF_HMD}
    ${SSF_HMD_DEVICE}
    ${SSF_HMD_RENDERER}
    ${SSF_HMD_QUAKE3}
	${SSF_HMD_LIBOVRDYNAMIC}
)


# now handle platfom specific stuff
if(USE_SDL2)

    # platform independend sdl stuff
    set(SSF_SDL2 ${SSF_SDL2}
        sdl/sdl_local.h
        sdl/sdl_glw.h
        sdl/sdl_gamma.cpp
        sdl/sdl_glimp.cpp
        sdl/sdl_qgl.cpp
        sdl/sdl_snddma.cpp
    )

    source_group(sdl FILES ${SSF_SDL2})
    set(STARWARS_SOURCE_FILES ${STARWARS_SOURCE_FILES} ${SSF_SDL2})

    set(CUSTOM_COMPILE_FLAGS_ALL "${CUSTOM_COMPILE_FLAGS_ALL} -DUSE_SDL2")

        if(PLATFORM_WIN32)
        # windows sdl

		set(SSF_WIN32 ${SSF_WIN32}
			win32/win_main.cpp
			win32/win_shared.cpp
			win32/win_syscon.cpp
		)

		include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/SDL2-2.0.3/include")

		add_library(imp_sdl2 STATIC IMPORTED)
		set_property(TARGET imp_sdl2 PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/SDL2-2.0.3/lib/x86/SDL2.lib")

		set(EXTRA_LIBS ${EXTRA_LIBS} imp_sdl2)
        elseif(PLATFORM_MACOS)
        # mac sdl

                set(SSF_MACOS ${SSF_MACOS}
                        sdl/sdl_console.cpp
                        mac/mac_main_sdl.cpp
                )


               include_directories( /Library/Frameworks )
               FIND_LIBRARY(SDL2_LIBRARY SDL2)
               MARK_AS_ADVANCED (SDL2_LIBRARY)
               set(EXTRA_LIBS ${EXTRA_LIBS} ${SDL2_LIBRARY})

        else()
        # linux sdl

		set(SSF_LINUX ${SSF_LINUX}
			sdl/sdl_console.cpp
			linux/linux_main.cpp
		)

		set(EXTRA_LIBS ${EXTRA_LIBS} SDL2)
	endif()
else()
    if(PLATFORM_LINUX)
        set(SSF_LINUX ${SSF_LINUX}
            linux/linux_local.h
            linux/linux_glw.h
            linux/linux_console.cpp
            linux/linux_gamma.cpp
            linux/linux_glimp.cpp
            linux/linux_main.cpp
            linux/linux_qgl.cpp
            linux/linux_snddma.cpp
            linux/xvidmode.cpp
        )
    endif()
endif()

if(PLATFORM_WIN32)
    # we only need this on windows

	set(SSF_ZLIB32
		zlib32/deflate.cpp
		zlib32/deflate.h
		zlib32/inflate.cpp
		zlib32/inflate.h
		zlib32/zip.h
		zlib32/zipcommon.cpp
	)

    source_group(zlib32 FILES ${SSF_ZLIB32})
    set(STARWARS_SOURCE_FILES ${STARWARS_SOURCE_FILES} ${SSF_ZLIB32})
endif()


# add platform specific files
source_group(win32 FILES ${SSF_WIN32})
source_group(linux FILES ${SSF_LINUX})
source_group(mac FILES ${SSF_MACOS})
set(STARWARS_SOURCE_FILES ${STARWARS_SOURCE_FILES} ${SSF_WIN32} ${SSF_LINUX} ${SSF_MACOS})

IF(APPLE)
   INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
   FIND_LIBRARY(COCOA_LIBRARY Cocoa)
   FIND_LIBRARY(OpenGL_LIBRARY OpenGL )
   find_library(OpenAL_LIBRARY OpenAL)
   MARK_AS_ADVANCED (COCOA_LIBRARY
                     OpenGL_LIBRARY
                     OpenAL_LIBRARY)
   SET(EXTRA_LIBS ${EXTRA_LIBS} ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${OpenAL_LIBRARY})
ENDIF (APPLE)

# finally set the compiler flags

if(MSVC)
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/glm")

	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${CUSTOM_COMPILE_FLAGS_ALL} -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CUSTOM_COMPILE_FLAGS_ALL} -DNDEBUG")

	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${CUSTOM_COMPILE_FLAGS_ALL} -DNDEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CUSTOM_COMPILE_FLAGS_ALL} -DNDEBUG")

    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} ${CMAKE_C_FLAGS_RELEASE}")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${CMAKE_CXX_FLAGS_RELEASE}")

else()
    set(DISABLE_WARNINGS_C "-Wno-write-strings -Wno-pointer-arith -Wno-multichar -Wno-int-to-pointer-cast -Wno-enum-compare -Wno-overflow")
    set(DISABLE_WARNINGS_CXX "-Wno-write-strings -Wno-pointer-arith -Wno-multichar -Wno-int-to-pointer-cast -Wno-enum-compare -Wno-overflow -Wno-conversion-null")

    set(COMPILER_FLAGS_C "-m32 -fexceptions -ffast-math")
    set(COMPILER_FLAGS_CXX "-m32 -fexceptions -ffast-math -fno-operator-names ")

    if(PLATFORM_LINUX)
        #set NDEBUG even on debug builds - asserts are killing us otherwise
        set(CUSTOM_COMPILE_FLAGS_ALL "${CUSTOM_COMPILE_FLAGS_ALL} -DLINUX -DNDEBUG")

        set(DISABLE_WARNINGS_C "${DISABLE_WARNINGS_C} -Wno-cpp")
        set(DISABLE_WARNINGS_CXX "${DISABLE_WARNINGS_CXX} -Wno-cpp")

    endif()

    if(PLATFORM_MACOS)
        include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/glm")
        add_definitions(-arch i386)
        set(ldflags "${ldflags} -arch i386")
    endif()

	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${CUSTOM_COMPILE_FLAGS_ALL} ${DISABLE_WARNINGS_C} ${COMPILER_FLAGS_C}")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CUSTOM_COMPILE_FLAGS_ALL} ${DISABLE_WARNINGS_CXX} ${COMPILER_FLAGS_CXX}")

	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g ${CUSTOM_COMPILE_FLAGS_ALL} ${DISABLE_WARNINGS_C} ${COMPILER_FLAGS_C}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g ${CUSTOM_COMPILE_FLAGS_ALL} ${DISABLE_WARNINGS_CXX} ${COMPILER_FLAGS_CXX}")
endif()



add_library(jagamehmdx86 SHARED ${GAME_SOURCE_FILES})
set_target_properties(jagamehmdx86 PROPERTIES PREFIX "")

if(PLATFORM_WIN32)
	set (EXECUTABLE_FLAG WIN32)
	set (EXTRA_LIBS ${EXTRA_LIBS} ${CMAKE_CURRENT_SOURCE_DIR}/OpenAL32.lib)
	set (EXTRA_LIBS ${EXTRA_LIBS} winmm.lib)
	set (EXTRA_LIBS ${EXTRA_LIBS} wsock32.lib)
	#set (EXTRA_LIBS ${EXTRA_LIBS} ALut.lib)
else()
    if(PLATFORM_LINUX)
        set (EXTRA_LIBS ${EXTRA_LIBS} X11)
        set (EXTRA_LIBS ${EXTRA_LIBS} Xxf86vm)
        set (EXTRA_LIBS ${EXTRA_LIBS} Xrandr)
        set (EXTRA_LIBS ${EXTRA_LIBS} openal)
        set (EXTRA_LIBS ${EXTRA_LIBS} GL)
        set (EXTRA_LIBS ${EXTRA_LIBS} rt)
    endif()

    set (EXTRA_LIBS ${EXTRA_LIBS} dl)
    set (EXTRA_LIBS ${EXTRA_LIBS} pthread)
    set (EXTRA_LIBS ${EXTRA_LIBS} z)
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "-m32")
endif()




add_executable(jasphmd ${EXECUTABLE_FLAG} ${STARWARS_SOURCE_FILES})
target_link_libraries (jasphmd  ${EXTRA_LIBS})
set_target_properties (jasphmd PROPERTIES COMPILE_DEFINITIONS "_JK2EXE;_M_IX86")
