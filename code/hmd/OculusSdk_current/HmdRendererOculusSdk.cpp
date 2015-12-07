#include "HmdRendererOculusSdk.h"
#include "../../renderer/tr_local.h"
#include "HmdDeviceOculusSdk.h"
#include "../HmdRenderer/PlatformInfo.h"

#include <OVR_CAPI.h>
// stupid OVR include bug
//#define OVR_OS_CONSOLE
//#include <Kernel/OVR_Types.h>
#include <Extras/OVR_Math.h>
#include <OVR_CAPI_GL.h>

#include <math.h>
#include <iostream>
#include <algorithm>

#ifdef _WINDOWS
#include <stdlib.h>
#include <dwmapi.h>
#endif

using namespace OVR;
using namespace std;

HmdRendererOculusSdk::HmdRendererOculusSdk(HmdDeviceOculusSdk* pHmdDeviceOculusSdk)
    :mIsInitialized(false)
    ,mStartedFrame(false)
    ,mFrameStartTime(0)
    ,mStartedRendering(false)
    ,mEyeId(-1)
    ,mWindowWidth(0)
    ,mWindowHeight(0)
    ,mRenderWidth(0)
    ,mRenderHeight(0)
    ,mGuiScale(0.5f)
    ,mGuiOffsetFactorX(0)
    ,mDismissHealthSafetyWarning(false)
    ,mAllowZooming(false)
    ,mpDevice(pHmdDeviceOculusSdk)
    ,mpHmd(NULL)
    ,mpMirrorTexture(nullptr)
    ,ReadFBO(0)
{

}

HmdRendererOculusSdk::~HmdRendererOculusSdk()
{

}

bool HmdRendererOculusSdk::Init(int windowWidth, int windowHeight, PlatformInfo platformInfo)
{
    if (mpDevice == NULL || mpDevice->GetHmd() == NULL)
    {
        return false;
    }

    PreparePlatform();

    mWindowWidth = windowWidth;
    mWindowHeight = windowHeight;

    mpHmd = mpDevice->GetHmd();
    ovrHmdDesc desc = mpDevice->GetHmdDesc();

    
    mRenderWidth = desc.Resolution.w/2;
    mRenderHeight = desc.Resolution.h;
    
    if (desc.Type == ovrHmd_DK1)
    {
        mGuiScale = 0.3f;
        mGuiOffsetFactorX = 5.0f;
    }
    else if (desc.Type == ovrHmd_DK2)
    {
        mGuiScale = 0.50f;
        mGuiOffsetFactorX = 0;
        mAllowZooming = true;
    }
    

    // Configure Stereo settings.

    ovrSizei recommenedTex0Size = d_ovr_GetFovTextureSize(mpHmd, ovrEye_Left, desc.DefaultEyeFov[0], 1.0f);
    ovrSizei recommenedTex1Size = d_ovr_GetFovTextureSize(mpHmd, ovrEye_Right, desc.DefaultEyeFov[1], 1.0f);
    
    mRenderWidth = max(recommenedTex0Size.w, recommenedTex1Size.w);
    mRenderHeight = max(recommenedTex0Size.h, recommenedTex1Size.h);

    ovrSizei bufferSize;
    bufferSize.w  = mRenderWidth;
    bufferSize.h = mRenderHeight;
    
    
    printf("HmdRendererOculusSdk: target texture size (%dx%d)\n", mRenderWidth, mRenderHeight);
    flush(std::cout);

    
    for (int i=0; i<FBO_COUNT; i++)
    {
        bool worked = RenderTool::CreateFrameBuffer(mFboInfos[i], mRenderWidth, mRenderHeight);
        if (!worked)
        {
            return false;
        }
    }
    
    
    for (int i=0; i<FBO_COUNT; i++)
    {
        mEyeTextureSetIndex[i] = 0;
        ovrResult worked = d_ovr_CreateSwapTextureSetGL(mpHmd, GL_SRGB8_ALPHA8, mRenderWidth, mRenderHeight, &(mEyeTextureSet[i]));
        if (worked != ovrSuccess)
        {
            return false;
        }
    }

    ovrResult success = ovr_CreateMirrorTextureGL(mpHmd, GL_SRGB8_ALPHA8, mWindowWidth, mWindowHeight, &mpMirrorTexture);
    qglGenFramebuffers(1, &ReadFBO);

    // Initialize VR structures, filling out description.
    ovrEyeRenderDesc eyeRenderDesc[2];
    eyeRenderDesc[0] = ovr_GetRenderDesc(mpHmd, ovrEye_Left, desc.DefaultEyeFov[0]);
    eyeRenderDesc[1] = ovr_GetRenderDesc(mpHmd, ovrEye_Right, desc.DefaultEyeFov[1]);
    mHmdToEyeViewOffset[0] = eyeRenderDesc[0].HmdToEyeViewOffset;
    mHmdToEyeViewOffset[0].x *= 0.5f;
    mHmdToEyeViewOffset[0].y *= 0.5f;
    mHmdToEyeViewOffset[0].z *= 0.5f;
    mHmdToEyeViewOffset[1] = eyeRenderDesc[1].HmdToEyeViewOffset;
    mHmdToEyeViewOffset[1].x *= 0.5f;
    mHmdToEyeViewOffset[1].y *= 0.5f;
    mHmdToEyeViewOffset[1].z *= 0.5f;
   
    // Initialize our single full screen Fov layer.
    mLayerMain.Header.Type      = ovrLayerType_EyeFov;
    mLayerMain.Header.Flags     = 0;
    mLayerMain.ColorTexture[0]  = mEyeTextureSet[0];
    mLayerMain.ColorTexture[1]  = mEyeTextureSet[1];
    mLayerMain.Fov[0]           = eyeRenderDesc[0].Fov;
    mLayerMain.Fov[1]           = eyeRenderDesc[1].Fov;
    mLayerMain.Viewport[0] = Recti(0, 0, bufferSize.w, bufferSize.h);
    mLayerMain.Viewport[1] = Recti(0, 0, bufferSize.w, bufferSize.h);
    // ld.RenderPose and ld.SensorSampleTime are updated later per frame.


    mStartedRendering = false;
    mIsInitialized = true;

    return true;
}


void HmdRendererOculusSdk::Shutdown()
{
    if (!mIsInitialized)
    {
        return;
    }

    qglDeleteFramebuffers(1, &ReadFBO);

    mpHmd = NULL;

    mIsInitialized = false;
}

std::string HmdRendererOculusSdk::GetInfo()
{
    return "HmdRendererOculusSdk";
}

bool HmdRendererOculusSdk::HandlesSwap()
{
    return true;
}

bool HmdRendererOculusSdk::GetRenderResolution(int& rWidth, int& rHeight)
{
    if (!mIsInitialized)
    {
        return false;
    }

    rWidth = mRenderWidth;
    rHeight = mRenderHeight;

    return true;
}

void HmdRendererOculusSdk::StartFrame()
{
    mStartedFrame = true;
    

}


void HmdRendererOculusSdk::BeginRenderingForEye(bool leftEye)
{
    if (!mIsInitialized || !mStartedFrame)
    {
        return;
    }

    int fboId = 0;
    if (!leftEye && FBO_COUNT > 1)
    {
        fboId = 1;
    }

    mEyeId = fboId;

    if (!mStartedRendering)
    {
        // render begin
        mStartedRendering = true;
        
        double displayMidpointSeconds = d_ovr_GetPredictedDisplayTime(mpHmd, 0);
        ovrTrackingState hmdState = d_ovr_GetTrackingState(mpHmd, displayMidpointSeconds, ovrTrue);
        d_ovr_CalcEyePoses(hmdState.HeadPose.ThePose, mHmdToEyeViewOffset, mLayerMain.RenderPose);
                
        for (int i=0; i<FBO_COUNT; i++)
        {
            mEyeTextureSet[i]->CurrentIndex = (mEyeTextureSet[i]->CurrentIndex + 1) % mEyeTextureSet[i]->TextureCount;
            ovrGLTexture* pTex = (ovrGLTexture*)&mEyeTextureSet[i]->Textures[mEyeTextureSet[i]->CurrentIndex];
            
            qglBindFramebuffer(GL_FRAMEBUFFER, mFboInfos[i].Fbo);
            qglFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  pTex->OGL.TexId, 0);



            //ovrQuatf orientation = mEyePoses[i].Orientation;
            //mCurrentOrientations[i] = glm::quat(orientation.w, orientation.x, orientation.y, orientation.z);
            
            //ovrVector3f position = mEyePoses[i].Position;
            //mCurrentPosition[i] = glm::vec3(position.x, position.y, position.z);
        }

        qglBindBuffer(GL_ARRAY_BUFFER, 0);
        qglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        qglBindVertexArray(0);
        qglUseProgramObjectARB(0);
        //qglDisable(GL_FRAMEBUFFER_SRGB);
        qglFrontFace(GL_CW);
        
        qglEnable(GL_FRAMEBUFFER_SRGB);
    }

    // bind framebuffer
    qglBindFramebuffer(GL_FRAMEBUFFER, mFboInfos[mEyeId].Fbo);
    RenderTool::ClearFBO(mFboInfos[mEyeId]);
}

void HmdRendererOculusSdk::EndFrame()
{
    if (!mIsInitialized || !mStartedFrame)
    {
        return;
    }

    //HandleSafetyWarning();
    
    if (mStartedFrame)
    {
        GLboolean depth_test = qglIsEnabled(GL_DEPTH_TEST);
        GLboolean blend = qglIsEnabled(GL_BLEND);
        GLboolean texture_2d = qglIsEnabled(GL_TEXTURE_2D);
        GLboolean texture_coord_array = qglIsEnabled(GL_TEXTURE_COORD_ARRAY);
        GLboolean color_array = qglIsEnabled(GL_COLOR_ARRAY);
        GLint viewport[4];
        GLint scissor[4];
        GLint texture;
        qglGetIntegerv(GL_VIEWPORT, viewport);
        qglGetIntegerv(GL_SCISSOR_BOX, scissor);
        qglGetIntegerv(GL_TEXTURE_BINDING_2D, &texture);
        
        qglViewport(0, 0, mWindowWidth, mWindowHeight);
        qglScissor(0, 0, mWindowWidth, mWindowHeight);
        
        // set state
        qglBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        
        qglDisable(GL_DEPTH_TEST);
        qglDisable(GL_BLEND);
        qglEnable(GL_TEXTURE_2D);
        qglDisableClientState(GL_TEXTURE_COORD_ARRAY);
        qglDisableClientState(GL_COLOR_ARRAY);
        qglDisableClientState(GL_VERTEX_ARRAY);

        ovrLayerHeader* layers =  &mLayerMain.Header;
        ovrResult result = ovr_SubmitFrame(mpHmd, 0, nullptr, &layers, 1);




        /*
        ovrGLTexture* pTex = (ovrGLTexture*)mpMirrorTexture;
        // Get source texture dimensions
        qglBindTexture(GL_TEXTURE_2D, pTex->OGL.TexId);

        //GLint sourceWidth = 0;
        //GLint sourceHeight = 0;
        //qglGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &sourceWidth);
        //qglGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &sourceHeight);

        //qglDrawBuffer(GL_BACK);

        // setup read buffer
        qglBindFramebuffer(GL_READ_FRAMEBUFFER, ReadFBO);


        qglFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTex->OGL.TexId, 0);
        qglFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);


        // Do the blt
        qglBlitFramebuffer(0, mWindowHeight, mWindowWidth, 0,
            0, 0, mWindowWidth, mWindowHeight,
            GL_COLOR_BUFFER_BIT, GL_NEAREST);
            */
        // restore the old state
        qglUseProgramObjectARB(0);
        
        if (depth_test)
        {
            qglEnable(GL_DEPTH_TEST);
        }
        if (blend)
        {
            qglEnable(GL_BLEND);
        }
        if (!texture_2d)
        {
            qglDisable(GL_TEXTURE_2D);
        }
        if (!texture_coord_array)
        {
            qglDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
        if (color_array)
        {
            qglEnableClientState(GL_COLOR_ARRAY);
        }
        
        qglEnableClientState(GL_VERTEX_ARRAY);
        
        qglViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
        qglScissor(scissor[0], scissor[1], scissor[2], scissor[3]);
        qglBindTexture(GL_TEXTURE_2D, texture);


        mStartedFrame = false;
        mStartedRendering = false;

        // keep for debugging
        //RenderTool::DrawFbos(&mFboInfos[0], FBO_COUNT, mWindowWidth, mWindowHeight);
    }
    
    mEyeId = -1;
}


// Determine whether this frame needs rendering based on time-warp timing and flags.
bool HmdRendererOculusSdk::FrameNeedsRendering()
{
    double curtime = mFrameStartTime;
    
    static double   lastUpdate          = 0.0;    
    double          renderInterval      = 0.0;
    double          timeSinceLast       = curtime - lastUpdate;
    bool            updateRenderedView  = true;

    

    if ( (timeSinceLast < 0.0) || ((float)timeSinceLast > renderInterval) )
    {
        // This allows us to do "fractional" speeds, e.g. 45fps rendering on a 60fps display.
        lastUpdate += renderInterval;
        if ( timeSinceLast > 5.0 )
        {
            // renderInterval is probably tiny (i.e. "as fast as possible")
            lastUpdate = curtime;
        }

        updateRenderedView = true;
    }
    else
    {
        updateRenderedView = false;
    }

    return updateRenderedView;
}



bool HmdRendererOculusSdk::GetCustomProjectionMatrix(float* rProjectionMatrix, float zNear, float zFar, float fov)
{
    if (!mIsInitialized)
    {
        return false;
    }

    ovrFovPort fovPort = mLayerMain.Fov[mEyeId];
    
    // ugly hardcoded default value
    if (mAllowZooming && fov < 124)
    {
        // this calculation only works on DK2 at the moment
        
        // something needs zooming
        float fovRad = DEG2RAD(fov);
        float tanVal = tanf(fovRad * 0.5f);
        fovPort.DownTan = tanVal;
        fovPort.LeftTan = tanVal;
        fovPort.RightTan = tanVal;
        fovPort.UpTan = tanVal;
    }
    

    ovrMatrix4f projMatrix = ovrMatrix4f_Projection(fovPort, zNear, zFar, ovrProjection_RightHanded);
    ConvertMatrix(projMatrix, rProjectionMatrix);

    return true;
}

bool HmdRendererOculusSdk::GetCustomViewMatrix(float* rViewMatrix, float& xPos, float& yPos, float& zPos, float bodyYaw, bool noPosition)
{

    if (!mIsInitialized)
    {
        return false;
    }

    ovrQuatf orientation = mLayerMain.RenderPose[mEyeId].Orientation;
    glm::quat currentOrientation = glm::quat(orientation.w, orientation.x, orientation.y, orientation.z);
    
    ovrVector3f position = mLayerMain.RenderPose[mEyeId].Position;
    glm::vec3 currentPosition = glm::vec3(position.x, position.y, position.z);
    
    // get current hmd rotation 
    glm::quat hmdRotation = glm::inverse(currentOrientation);

    // change hmd orientation to game coordinate system
    glm::quat convertHmdToGame = glm::rotate(glm::quat(1.0f,0.0f,0.0f,0.0f), (float)DEG2RAD(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    convertHmdToGame = glm::rotate(convertHmdToGame, (float)DEG2RAD(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 hmdRotationMat = glm::mat4_cast(hmdRotation) * glm::mat4_cast(convertHmdToGame);


    // convert body transform to matrix
    glm::mat4 bodyPosition = glm::translate(glm::mat4(1.0f), glm::vec3(-xPos, -yPos, -zPos));
    glm::quat bodyYawRotation = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), (float)(DEG2RAD(-bodyYaw)), glm::vec3(0.0f, 0.0f, 1.0f));

    
    float meterToGame =  52.4928f;// 26.2464f; //(3.2808f * 8.0f * 2.0f); // meter to feet * QuakeIII engine factor 8 * JA level factor 2
    glm::vec3 hmdPos;
    hmdPos.x = currentPosition.z * meterToGame;
    hmdPos.y = currentPosition.x * meterToGame;
    hmdPos.z = currentPosition.y * -meterToGame;
    
    glm::mat4 hmdPosition = glm::translate(glm::mat4(1.0f), hmdPos);    
    
    // create view matrix
    glm::mat4 viewMatrix;
    if (noPosition)
    {
        viewMatrix = hmdRotationMat * glm::mat4_cast(bodyYawRotation) * bodyPosition;
    }
    else
    {
        viewMatrix = hmdRotationMat * hmdPosition* glm::mat4_cast(bodyYawRotation) * bodyPosition;
    }


    
    memcpy(rViewMatrix, &viewMatrix[0][0], sizeof(float) * 16);
    
    
    if (noPosition)
    {
        return true;
    }
    
    // add hmd offset to body pos

    glm::quat bodyYawRotationReverse = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), (float)(DEG2RAD(bodyYaw)), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 offsetMat = glm::mat4_cast(bodyYawRotationReverse) * hmdPosition;
    glm::vec3 offsetPos = glm::vec3(offsetMat[3]);
    
    //Vector3f hmdPos2 = Vector3f(hmdPos.x, hmdPos.y, hmdPos.z);
    
    //Matrix4f bodyYawRotationReverse = Matrix4f::RotationZ(DEG2RAD(bodyYaw));
    //Vector3f offsetPos = (bodyYawRotationReverse * Matrix4f::Translation(hmdPos2)).GetTranslation();
    
    /// TODO: do we need this?
    offsetPos *= -1;

    xPos += offsetPos.x;
    yPos += offsetPos.y;
    zPos += offsetPos.z;

    return true;
}

bool HmdRendererOculusSdk::Get2DViewport(int& rX, int& rY, int& rW, int& rH)
{
    // shrink the gui for the HMD display
    float aspect = 1.0f;

    rW = mRenderWidth *mGuiScale;
    rH = mRenderWidth *mGuiScale * aspect;

    rX = (mRenderWidth - rW)/2.0f;
    int xOff = mGuiOffsetFactorX > 0 ? (mRenderWidth / mGuiOffsetFactorX) : 0;
    xOff *= mEyeId == 0 ? 1 : -1;
    rX += xOff;

    rY = (mRenderHeight - rH) / 2;

    return true;
}

bool HmdRendererOculusSdk::AttachToWindow(void* pWindowHandle)
{
#ifdef _WINDOWS
    //if (mpDevice == NULL || mpDevice->GetHmd() == NULL)
    //{
    //    return false;
    //}
    //
    //if (!(mpDevice->GetHmd()->HmdCaps & ovrHmdCap_ExtendDesktop))
    //{
    //    d_ovrHmd_AttachToWindow(mpDevice->GetHmd(), pWindowHandle, NULL, NULL);
    //}
    
    return false;
#else
    return false;
#endif
}

void HmdRendererOculusSdk::DismissHealthSafetyWarning()
{
    mDismissHealthSafetyWarning = true;
}

void HmdRendererOculusSdk::ConvertMatrix(const ovrMatrix4f& from, float* rTo)
{
    rTo[0] = from.M[0][0];
    rTo[4] = from.M[0][1];
    rTo[8] = from.M[0][2];
    rTo[12] = from.M[0][3];

    rTo[1] = from.M[1][0];
    rTo[5] = -from.M[1][1];
    rTo[9] = from.M[1][2];
    rTo[13] = from.M[1][3];

    rTo[2] = from.M[2][0];
    rTo[6] = from.M[2][1];
    rTo[10] = from.M[2][2];
    rTo[14] = from.M[2][3];

    rTo[3] = from.M[3][0];
    rTo[7] = from.M[3][1];
    rTo[11] = from.M[3][2];
    rTo[15] = from.M[3][3];
}

void HmdRendererOculusSdk::PreparePlatform()
{
#ifdef _WINDOWS
    // disable composition on windows (because of some OpenGL issues)
    typedef HRESULT (WINAPI *PFNDWMENABLECOMPOSITIONPROC) (UINT);
    PFNDWMENABLECOMPOSITIONPROC DwmEnableComposition;

    HINSTANCE HInstDwmapi = LoadLibraryW( L"dwmapi.dll" );

    if (HInstDwmapi)
    {
        DwmEnableComposition = (PFNDWMENABLECOMPOSITIONPROC)GetProcAddress( HInstDwmapi, "DwmEnableComposition" );
        if (DwmEnableComposition)
        {
            DwmEnableComposition(DWM_EC_DISABLECOMPOSITION);
        }

        FreeLibrary(HInstDwmapi);
        HInstDwmapi = NULL;
    }
#endif
}
