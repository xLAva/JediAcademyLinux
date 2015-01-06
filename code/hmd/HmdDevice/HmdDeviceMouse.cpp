#include "HmdDeviceMouse.h"
#include "../../client/client.h"

#include "../../game/q_shared.h"

using namespace std;

HmdDeviceMouse::HmdDeviceMouse()
    :mPitch(0)
    ,mYaw(0)
    ,mRoll(0)
{

}

HmdDeviceMouse::~HmdDeviceMouse()
{

}

bool HmdDeviceMouse::Init(bool allowDummyDevice)
{
    return true;
}

void HmdDeviceMouse::Shutdown()
{

}

string HmdDeviceMouse::GetInfo()
{
    return "HmdDeviceMouse: (Simulated Device using mouse inputs)";
}

bool HmdDeviceMouse::HasDisplay()
{
    return false;
}

string HmdDeviceMouse::GetDisplayDeviceName()
{
    return "";
}

bool HmdDeviceMouse::GetDisplayPos(int& rX, int& rY)
{
    rX = 0;
    rY = 0;
    return false;
}

bool HmdDeviceMouse::GetDeviceResolution(int& rWidth, int& rHeight)
{
    return false;

    //    rWidth = 1280;
    //    rHeight = 800;

    //    return true;
}

bool HmdDeviceMouse::GetOrientationRad(float& rPitch, float& rYaw, float& rRoll)
{
    rPitch = DEG2RAD(-mPitch);
    rYaw = DEG2RAD(mYaw);
    rRoll = DEG2RAD(-mRoll);

    return true;
}

void HmdDeviceMouse::SetOrientationDeg(float pitch, float yaw, float roll)
{
    mPitch = pitch;
    mYaw = yaw;
    mRoll = roll;
}

void HmdDeviceMouse::GetOrientationDeg(float& rPitch, float& rYaw, float& rRoll)
{
    rPitch = mPitch;
    rYaw = mYaw;
    rRoll = mRoll;
}


bool HmdDeviceMouse::GetPosition(float &rX, float &rY, float &rZ)
{
    return false;
}
