/**
 * HMD extension for JediAcademy
 *
 *  Copyright 2014 by Jochen Leopold <jochen.leopold@model-view.com>
 */

#ifndef HMDRENDEREROCULUSSDK_H
#define HMDRENDEREROCULUSSDK_H

#include "IHmdRenderer.h"
#include "../../renderer/qgl.h"

#include <OVR_CAPI.h>

// stupid OVR include bug
#define OVR_OS_CONSOLE
#include <Kernel/OVR_Math.h>

#include "RenderTool.h"

class HmdDeviceOculusSdk;

class HmdRendererOculusSdk : public IHmdRenderer
{
public:
    HmdRendererOculusSdk(HmdDeviceOculusSdk* pHmdDeviceOculusSdk);
    virtual ~HmdRendererOculusSdk();

    virtual bool Init(int windowWidth, int windowHeight, PlatformInfo platformInfo);
    virtual void Shutdown();

    virtual std::string GetInfo();

    virtual bool HandlesSwap();
    virtual bool GetRenderResolution(int& rWidth, int& rHeight);

    virtual void StartFrame();
    virtual void BeginRenderingForEye(bool leftEye);
    virtual void EndFrame();

    virtual bool GetCustomProjectionMatrix(float* rProjectionMatrix, float zNear, float zFar, float fov);
    virtual bool GetCustomViewMatrix(float* rViewMatrix, float& xPos, float& yPos, float& zPos, float bodyYaw);

    virtual bool Get2DViewport(int& rX, int& rY, int& rW, int& rH);

    bool AttachToWindow(void* pWindowHandle);
    void DismissHealthSafetyWarning();

protected:
    static void ConvertMatrix(const OVR::Matrix4f& from, float* rTo);

private:
    bool FrameNeedsRendering();
    void HandleSafetyWarning();
    
    static const int FBO_COUNT = 2;
    RenderTool::FrameBufferInfo mFboInfos[FBO_COUNT];

    bool mIsInitialized;
    bool mStartedFrame;
    double mFrameStartTime;
    bool mStartedRendering;
    int mEyeId;

    int mWindowWidth;
    int mWindowHeight;

    int mRenderWidth;
    int mRenderHeight;

    float mGuiScale;
    float mGuiOffsetFactorX;

    bool mDismissHealthSafetyWarning;

    HmdDeviceOculusSdk* mpDevice;
    ovrHmd mpHmd;
    ovrEyeRenderDesc mEyeRenderDesc[2];
    ovrTexture EyeTexture[2];

    ovrEyeType mEyes[2];
    ovrPosef mEyePoses[2];
    ovrFrameTiming mFrameTiming;

    OVR::Matrix4f mCurrentProj;
    OVR::Matrix4f mCurrentView;
    OVR::Quatf mCurrentOrientations[2];
	OVR::Vector3f mCurrentPosition[2];


};

#endif
