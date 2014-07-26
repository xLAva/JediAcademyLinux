/**
 * HMD extension for JediAcademy
 *
 *  Copyright 2014 by Jochen Leopold <jochen.leopold@model-view.com>
 */

#ifndef CLIENTHMD_H
#define CLIENTHMD_H

class IHmdDevice;
class IHmdRenderer;

class ClientHmd
{
public:
    ClientHmd();
    ~ClientHmd();

    static ClientHmd* Get();
    static void Destroy();

    void UpdateInputView(float yawDiff, float& rPitch, float& rYaw, float& rRoll);
    void UpdateGame();
    bool GetOrientation(float& rPitch, float& rYaw, float& rRoll);

    IHmdDevice* GetDevice() { return mpDevice; }
    void SetDevice(IHmdDevice* pDevice) { mpDevice = pDevice; }

    IHmdRenderer* GetRenderer() { return mpRenderer; }
    void SetRenderer(IHmdRenderer* pRenderer) { mpRenderer = pRenderer; }

    float GetYawDiff() { return mViewangleDiff; }

private:

    // disable copy constructor
    ClientHmd(const ClientHmd&);
    ClientHmd& operator=(const ClientHmd&);

    IHmdDevice* mpDevice;
    IHmdRenderer* mpRenderer;
    bool mIsInitialized;
    float mLastViewangleYaw;
    float mViewangleDiff;

    static ClientHmd* sClientHmd;
};

#endif
