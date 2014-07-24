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
    
    virtual std::string GetDisplayDeviceName();
    virtual int GetDisplayId();
    virtual bool GetDisplayPos(int& rX, int&rY);
    
    virtual bool GetDeviceResolution(int& rWidth, int& rHeight);
    virtual bool GetOrientationRad(float& rPitch, float& rYaw, float& rRoll);

    ohmd_device* GetHmd() { return mpHmd; }
    
protected:
    void ConvertQuatToEuler(const float* quat, float& rYaw, float& rPitch, float& rRoll);
    
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
};

#endif
