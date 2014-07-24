#ifndef IHMDRENDERER_H
#define IHMDRENDERER_H

#include <string>

struct PlatformInfo;

class IHmdRenderer
{
public:
    virtual bool Init(int windowWidth, int windowHeight, PlatformInfo platformInfo) = 0;
    virtual void Shutdown() = 0;
    
    virtual std::string GetInfo() = 0;
    
    virtual bool HandlesSwap() = 0;
    
    virtual bool GetRenderResolution(int& rWidth, int&rHeight) = 0;
    
    virtual void BindFramebuffer(bool leftEye) = 0;
    virtual void EndFrame() = 0;
    virtual bool GetCustomProjectionMatrix(float* rProjectionMatrix, float zNear, float zFar) = 0;
    virtual bool GetCustomViewMatrix(float* rViewMatrix, float xPos, float yPos, float zPos, float bodyYaw) = 0;
    virtual int GetViewportXOffset() = 0;
    virtual bool Get2DViewport(int& rX, int& rY, int& rW, int& rH) = 0;
    
};

#endif
