
#include "GameHmd.h"
#include <memory>
#include <memory.h>

GameHmd* GameHmd::sGameHmd = NULL;

GameHmd::GameHmd()
    :mIsInitialized(false)
    ,mPitch(0)
    ,mYaw(0)
    ,mRoll(0)
{

}

GameHmd* GameHmd::Get()
{
    if (sGameHmd == NULL)
    {
        sGameHmd = new GameHmd();
    }

    return sGameHmd;
}

void GameHmd::Destroy()
{
    if (sGameHmd == NULL)
    {
        return;
    }

    delete sGameHmd;
    sGameHmd = NULL;
}

void GameHmd::UpdateHmd(float* pitch, float* yaw, float* roll)
{
    mIsInitialized = true;
    mPitch = *pitch;
    mYaw = *yaw;
    mRoll = *roll;
}

bool GameHmd::GetOrientation(float& pitch, float& yaw, float& roll)
{
    if (!mIsInitialized)
    {
        return false;
    }

    pitch = mPitch;
    yaw = mYaw;
    roll = mRoll;
    return true;
}
