#include "HmdDeviceOculusSdk.h"
#include "../SearchForDisplay.h"

#define OVR_OS_CONSOLE
#include "Kernel/OVR_Math.h"
#include "Kernel/OVR_Threads.h"

#include <string>
#include <iostream>
#include <cstdio>

using namespace OVR;

HmdDeviceOculusSdk::HmdDeviceOculusSdk()
    :mIsInitialized(false)
    ,mUsingDebugHmd(false)
    ,mPositionTrackingEnabled(false)
    ,mIsRotated(false)
    ,mpHmd(NULL)
{

}

HmdDeviceOculusSdk::~HmdDeviceOculusSdk()
{

}

bool HmdDeviceOculusSdk::Init(bool allowDummyDevice)
{
    if (mIsInitialized)
    {
        return true;
    }

    bool debugPrint = true;

    if (debugPrint)
    {
        printf("ovr init ...\n");
    }

#if !defined(FORCE_STATIC_OCULUS_SDK)
    ovr_dynamic_load_result result = oculus_dynamic_load(NULL);
    if (result != OVR_DYNAMIC_RESULT_SUCCESS)
    {
        printf("ovr: could not load library\n");
        return false;
    }
#endif

#if defined(FORCE_STATIC_OCULUS_SDK) && defined(OVR_OS_WIN32)
    OVR::Thread::SetCurrentPriority(Thread::HighestPriority);

    if(OVR::Thread::GetCPUCount() >= 4) // Don't do this unless there are at least 4 processors, otherwise the process could hog the machine.
    {
        SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    }
#endif
    
    d_ovr_Initialize();

    if (debugPrint)
    {
        printf("Create device ...\n");
    }

    mpHmd = d_ovrHmd_Create(0);
    mUsingDebugHmd = false;

    if (!mpHmd)
    {
        if (allowDummyDevice)
        {
            // no hmd detected, create debugging device
            mpHmd = d_ovrHmd_CreateDebug(ovrHmd_DK1);
            mUsingDebugHmd = true;
        }

        if (!mpHmd)
        {
            d_ovr_Shutdown();

            if (debugPrint)
            {
                printf("ovr init ... failed.\n");
                flush(std::cout);
            }

            return false;
        }
    }

    mPositionTrackingEnabled = (mpHmd->TrackingCaps & ovrTrackingCap_Position) ? true : false;

    // Start the sensor which provides the Rift’s pose and motion.
    d_ovrHmd_ConfigureTracking(mpHmd, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position, ovrTrackingCap_Orientation);


    if (debugPrint)
    {
        printf("DeviceInfo Monitor: %s\n", mpHmd->DisplayDeviceName);
    }

    mInfo = "HmdDeviceOculusSdk:";

    if (strlen(mpHmd->ProductName) > 0)
    {
        mInfo += " ";
        mInfo += mpHmd->ProductName;
    }

    if (strlen(mpHmd->ProductName) > 0)
    {
        mInfo += " ";
        mInfo += mpHmd->Manufacturer;
    }

    if (mUsingDebugHmd)
    {
        mInfo += " (Debug)";
    }

    

#ifdef LINUX
	// use the display name to get the rotation information

	std::string displayName = mpHmd->DisplayDeviceName;

	// at least on Linux the provided display name is not the same as the SDL2 display name
	// hardcode the correct names for DK1 and DK2
	switch (mpHmd->Type)
	{
	case ovrHmd_DK1:
		displayName = "Rift DK 7\"";
		break;
	case ovrHmd_DK2:
		displayName = "Rift DK2 6\"";
		break;
	}

	// we only need the rotation information
	SearchForDisplay::DisplayInfo rInfo;

    bool worked = SearchForDisplay::GetDisplayPosition(displayName, mpHmd->Resolution.w, mpHmd->Resolution.h, rInfo);
    if (worked)
    {
        mIsRotated = rInfo.isRotated;
    }
    
#endif

    mIsInitialized = true;

    if (debugPrint)
    {
        printf("ovr init ... done.\n");
        flush(std::cout);
    }

    return true;
}

void HmdDeviceOculusSdk::Shutdown()
{
    if (!mIsInitialized)
    {
        return;
    }

    mInfo = "";

    d_ovrHmd_Destroy(mpHmd);
    mpHmd = NULL;

    d_ovr_Shutdown();

    mIsInitialized = false;
}

std::string HmdDeviceOculusSdk::GetInfo()
{
    return mInfo;
}

bool HmdDeviceOculusSdk::HasDisplay()
{
    if (!mIsInitialized || mpHmd->Resolution.w <= 0)
    {
        return false;
    }

    return true;
}

std::string HmdDeviceOculusSdk::GetDisplayDeviceName()
{
    return mpHmd->DisplayDeviceName;
}

bool HmdDeviceOculusSdk::GetDisplayPos(int& rX, int& rY)
{
    rX = mpHmd->WindowsPos.x;
    rY = mpHmd->WindowsPos.y;

    return true;
}

bool HmdDeviceOculusSdk::GetDeviceResolution(int& rWidth, int& rHeight, bool& rIsRotated)
{
    if (!mIsInitialized || mpHmd->Resolution.w <= 0)
    {
        return false;
    }

    rWidth = mpHmd->Resolution.w;
    rHeight = mpHmd->Resolution.h;
	rIsRotated = mIsRotated;

    return true;
}

bool HmdDeviceOculusSdk::GetOrientationRad(float& rPitch, float& rYaw, float& rRoll)
{
    if (!mIsInitialized || mpHmd == NULL)
    {
        return false;
    }

    // Query the HMD for the sensor state at a given time.
    ovrTrackingState ss = d_ovrHmd_GetTrackingState(mpHmd, 0.0);
    if ((ss.StatusFlags & ovrStatus_OrientationTracked))
    {
        OVR::Quatf orientation = OVR::Quatf(ss.HeadPose.ThePose.Orientation);
        orientation.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&rYaw, &rPitch, &rRoll);

        //printf("pitch: %.2f yaw: %.2f roll: %.2f\n", rPitch, rYaw, rRoll);

        return true;
    }

    return false;

}


bool HmdDeviceOculusSdk::GetPosition(float &rX, float &rY, float &rZ)
{
    if (!mIsInitialized || mpHmd == NULL || !mPositionTrackingEnabled)
    {
        return false;
    }

    // Query the HMD for the sensor state at a given time.
    ovrTrackingState ss = d_ovrHmd_GetTrackingState(mpHmd, 0.0);
    if ((ss.StatusFlags & ovrStatus_PositionTracked))
    {
        OVR::Vector3f pos = OVR::Vector3f(ss.HeadPose.ThePose.Position);
        rX = pos.x;
        rY = pos.y;
        rZ = pos.z;

        //printf("pitch: %.2f yaw: %.2f roll: %.2f\n", rPitch, rYaw, rRoll);

        return true;
    }

    return false;
}

void HmdDeviceOculusSdk::Recenter()
{
    d_ovrHmd_RecenterPose(mpHmd);
}
