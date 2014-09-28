/**
 * HMD extension for JediAcademy
 *
 *  Copyright 2014 by Jochen Leopold <jochen.leopold@model-view.com>
 */

#ifndef HMDDEVICEOCULUSSDK_H
#define HMDDEVICEOCULUSSDK_H

#include "IHmdDevice.h"

#include <OVR_CAPI.h>



class HmdDeviceOculusSdk : public IHmdDevice
{
public:

    HmdDeviceOculusSdk();
    virtual ~HmdDeviceOculusSdk();

    virtual bool Init(bool allowDummyDevice = false);
    virtual void Shutdown();

    virtual std::string GetInfo();

    virtual bool HasDisplay();
    virtual std::string GetDisplayDeviceName();
    virtual bool GetDisplayPos(int& rX, int& rY);

	virtual bool GetDeviceResolution(int& rWidth, int& rHeight, bool& rIsRotated);
    virtual bool GetOrientationRad(float& rPitch, float& rYaw, float& rRoll);
    virtual bool GetPosition(float& rX, float& rY, float& rZ);


    ovrHmd GetHmd() { return mpHmd; }
    bool IsDebugHmd() { return mUsingDebugHmd; }


private:
    // disable copy constructor
    HmdDeviceOculusSdk(const HmdDeviceOculusSdk&);
    HmdDeviceOculusSdk& operator=(const HmdDeviceOculusSdk&);

    bool mIsInitialized;
    bool mUsingDebugHmd;
    bool mPositionTrackingEnabled;
    ovrHmd mpHmd;

    std::string mInfo;
};

#endif
