/**
 * HMD extension for JediAcademy
 *
 *  Copyright 2014 by Jochen Leopold <jochen.leopold@model-view.com>
 */

#ifndef HMDRENDEREROCULUS_H
#define HMDRENDEREROCULUS_H

#include "IHmdRenderer.h"
#include "RenderTool.h"

class HmdRendererOculus : public IHmdRenderer
{
public:
    HmdRendererOculus();
    virtual ~HmdRendererOculus();
    
    virtual bool Init(int windowWidth, int windowHeight, PlatformInfo platformInfo);
    virtual void Shutdown();
    
    virtual std::string GetInfo();
    
    virtual bool HandlesSwap();
    
    virtual bool GetRenderResolution(int& rWidth, int&rHeight);
    
    virtual void BeginRenderingForEye(bool leftEye);
    virtual void EndFrame();
    
    virtual bool GetCustomProjectionMatrix(float* rProjectionMatrix, float zNear, float zFar);
    virtual bool GetCustomViewMatrix(float* rViewMatrix, float xPos, float yPos, float zPos, float bodyYaw);
    
    virtual bool Get2DViewport(int& rX, int& rY, int& rW, int& rH);
    
private:
    const char* GetVertexShader();
    const char* GetPixelShader();
    
    static const int FBO_COUNT = 2;
    RenderTool::FrameBufferInfo mFboInfos[FBO_COUNT];

    bool mIsInitialized;
    
	GLhandleARB  mOculusProgram;
	GLint mOculusCenter;
    
    int mWindowWidth;
    int mWindowHeight;
    int mRenderWidth;
    int mRenderHeight;
    
    int mCurrentFbo;
};

#endif
