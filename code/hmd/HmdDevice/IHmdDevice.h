#ifndef IHMDDEVICE_H
#define IHMDDEVICE_H

#include <string>

class IHmdDevice
{
public:

    virtual bool Init() = 0;
    virtual void Shutdown() = 0;
    
    virtual std::string GetInfo() = 0;
    
    virtual std::string GetDisplayDeviceName() = 0;
    virtual int GetDisplayId() = 0;
    virtual bool GetDisplayPos(int& rX, int&rY) = 0;

    // return false if no display is used
    virtual bool GetDeviceResolution(int& rWidth, int& rHeight) = 0;
    virtual bool GetOrientationRad(float& rPitch, float& rYaw, float& rRoll) = 0;

};

#endif
