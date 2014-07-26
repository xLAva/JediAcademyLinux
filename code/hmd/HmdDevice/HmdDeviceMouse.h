/**
 * HMD extension for JediAcademy
 *
 *  Copyright 2014 by Jochen Leopold <jochen.leopold@model-view.com>
 */

#ifndef HMDDEVICEMOUSE_H
#define HMDDEVICEMOUSE_H

#include "IHmdDevice.h"

class HmdDeviceMouse : public IHmdDevice
{
public:

    HmdDeviceMouse();
    virtual ~HmdDeviceMouse();

    virtual bool Init(bool allowDummyDevice = false);
    virtual void Shutdown();

    virtual std::string GetInfo();

    virtual bool HasDisplay();
    virtual std::string GetDisplayDeviceName();
    virtual bool GetDisplayPos(int& rX, int& rY);

    virtual bool GetDeviceResolution(int& rWidth, int& rHeight);
    virtual bool GetOrientationRad(float& rPitch, float& rYaw, float& rRoll);

    void SetOrientationDeg(float pitch, float yaw, float roll);
    void GetOrientationDeg(float& rPitch, float& rYaw, float& rRoll);

private:
    // disable copy constructor
    HmdDeviceMouse(const HmdDeviceMouse&);
    HmdDeviceMouse& operator=(const HmdDeviceMouse&);

    float mPitch;
    float mYaw;
    float mRoll;
};

#endif
