#include "ClientHmd.h"
#include "HmdDevice/IHmdDevice.h"

#include "../game/q_shared.h"
#include "../client/vmachine.h"

#include <memory>
#include <algorithm>

ClientHmd* ClientHmd::sClientHmd = NULL;

ClientHmd::ClientHmd()
    :mpDevice(NULL)
    ,mIsInitialized(false)
    ,mLastViewangleYaw(0)
    ,mViewangleDiff(0)
{

}

ClientHmd::~ClientHmd()
{

}

ClientHmd* ClientHmd::Get()
{
    if (sClientHmd == NULL)
    {
        sClientHmd = new ClientHmd();
    }

    return sClientHmd;
}

void ClientHmd::Destroy()
{
    if (sClientHmd == NULL)
    {
        return;
    }

    delete sClientHmd;
    sClientHmd = NULL;
}

void ClientHmd::UpdateInputView(float yawDiff, float& rPitch, float& rYaw, float& rRoll)
{
    if (mpDevice == NULL)
    {
        return;
    }

    if (!mIsInitialized)
    {
        mIsInitialized = true;
        mLastViewangleYaw = rYaw;
    }

    mViewangleDiff += yawDiff;
    mViewangleDiff = fmod(mViewangleDiff, 360.0f);

    mLastViewangleYaw = rYaw;

    float pitch = 0;
    float yaw = 0;
    float roll = 0;

    bool worked = GetOrientation(pitch, yaw, roll);

    if (!worked)
    {
        return;
    }

    rPitch = pitch;

    rPitch = std::max(rPitch, -80.0f);
    rPitch = std::min(rPitch, 80.0f);

    rYaw = yaw + mViewangleDiff;
    mLastViewangleYaw = rYaw;
}

void ClientHmd::UpdateGame()
{
    if (mpDevice == NULL)
    {
        return;
    }

    float pitch = 0;
    float yaw = 0;
    float roll = 0;

    bool worked = GetOrientation(pitch, yaw, roll);

    if (!worked)
    {
        return;
    }

    //printf("pitch: %.2f yaw: %.2f roll: %.2f\n", pitch, yaw, roll);

    yaw += mViewangleDiff;

    VM_Call(CG_HMD_UPDATE, &pitch, &yaw, &roll);
}


bool ClientHmd::GetOrientation(float& rPitch, float& rYaw, float& rRoll)
{
    if (mpDevice == NULL)
    {
        return false;
    }

    bool worked = mpDevice->GetOrientationRad(rPitch, rYaw, rRoll);
    if (!worked)
    {
        return false;
    }

    rPitch = RAD2DEG(-rPitch);
    rYaw = RAD2DEG(rYaw);
    rRoll = RAD2DEG(-rRoll);

    return true;
}
