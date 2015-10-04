/**
 * HMD extension for JediAcademy
 *
 *  Copyright 2014 by Jochen Leopold <jochen.leopold@model-view.com>
 */

#ifndef HMDRENDEREROCULUSSDK_H
#define HMDRENDEREROCULUSSDK_H

#include "IHmdRenderer.h"
#include "../../renderer/qgl.h"



#include <OVR_CAPI_0_7_0.h>
#include <Extras/OVR_Math.h>

#include "RenderTool.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

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
    virtual bool GetCustomViewMatrix(float* rViewMatrix, float& xPos, float& yPos, float& zPos, float bodyYaw, bool noPosition);

    virtual bool Get2DViewport(int& rX, int& rY, int& rW, int& rH);

    bool AttachToWindow(void* pWindowHandle);
    void DismissHealthSafetyWarning();

protected:
    static void ConvertMatrix(const ovrMatrix4f& from, float* rTo);

private:
    bool FrameNeedsRendering();
    void HandleSafetyWarning();
    void PreparePlatform();
    
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
    bool mAllowZooming;

    HmdDeviceOculusSdk* mpDevice;
    ovrHmd mpHmd;
    ovrEyeRenderDesc mEyeRenderDesc[2];
    ovrTexture EyeTexture[2];

    ovrEyeType mEyes[2];
    ovrPosef mEyePoses[2];
    ovrFrameTiming mFrameTiming;

    glm::quat mCurrentOrientations[2];
    glm::vec3 mCurrentPosition[2];


};

#endif
