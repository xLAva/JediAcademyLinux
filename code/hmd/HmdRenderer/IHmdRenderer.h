/**
 * HMD extension for JediAcademy
 *
 *  Copyright 2014 by Jochen Leopold <jochen.leopold@model-view.com>
 */

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
    
    // has to be called before rendering or any call to eye dependend methodes like GetCustomProjectionMatrix
    virtual void BeginRenderingForEye(bool leftEye) = 0;
    virtual void EndFrame() = 0;
    
    virtual bool GetCustomProjectionMatrix(float* rProjectionMatrix, float zNear, float zFar) = 0;
    virtual bool GetCustomViewMatrix(float* rViewMatrix, float xPos, float yPos, float zPos, float bodyYaw) = 0;
    
    virtual bool Get2DViewport(int& rX, int& rY, int& rW, int& rH) = 0;
    
};

#endif
