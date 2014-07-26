/**
 * HMD extension for JediAcademy
 *
 *  Copyright 2014 by Jochen Leopold <jochen.leopold@model-view.com>
 */

#ifndef HMDDEVICEOPENHMD_H
#define HMDDEVICEOPENHMD_H

#include "IHmdDevice.h"
#include <openhmd.h>

class HmdDeviceOpenHmd : public IHmdDevice
{
public:

    HmdDeviceOpenHmd();
    virtual ~HmdDeviceOpenHmd();

    virtual bool Init(bool allowDummyDevice = false);
    virtual void Shutdown();

    virtual std::string GetInfo();

    virtual bool HasDisplay();
    virtual std::string GetDisplayDeviceName();
    virtual bool GetDisplayPos(int& rX, int& rY);

    virtual bool GetDeviceResolution(int& rWidth, int& rHeight);
    virtual bool GetOrientationRad(float& rPitch, float& rYaw, float& rRoll);

    ohmd_device* GetHmd() { return mpHmd; }

protected:
    void ConvertQuatToEuler(const float* quat, float& rYaw, float& rPitch, float& rRoll);
    void DetectDisplay();

private:
    // disable copy constructor
    HmdDeviceOpenHmd(const HmdDeviceOpenHmd&);
    HmdDeviceOpenHmd& operator=(const HmdDeviceOpenHmd&);

    bool mIsInitialized;
    ohmd_context* mpCtx;
    ohmd_device* mpHmd;

    std::string mInfo;
    int mDisplayWidth;
    int mDisplayHeight;

    int mDisplayId;
    int mDisplayX;
    int mDisplayY;
    std::string mDisplayDeviceName;
};

#endif
