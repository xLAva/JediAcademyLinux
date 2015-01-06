#include "HmdRendererOculusSdk.h"
#include "../../renderer/tr_local.h"
#include "../HmdDevice/HmdDeviceOculusSdk.h"
#include "PlatformInfo.h"

#include "Util/Util_Render_Stereo.h"
using namespace OVR::Util::Render;

#include <OVR_CAPI_GL.h>

#include <math.h>
#include <iostream>


using namespace OVR;
using namespace std;

HmdRendererOculusSdk::HmdRendererOculusSdk(HmdDeviceOculusSdk* pHmdDeviceOculusSdk)
    :mIsInitialized(false)
    ,mStartedRendering(false)
    ,mEyeId(-1)
    ,mWindowWidth(0)
    ,mWindowHeight(0)
    ,mRenderWidth(0)
    ,mRenderHeight(0)
    ,mpDevice(pHmdDeviceOculusSdk)
    ,mpHmd(NULL)
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


    mWindowWidth = windowWidth;
    mWindowHeight = windowHeight;
    mRenderWidth = windowWidth/2;
    mRenderHeight = windowHeight;



    mpHmd = mpDevice->GetHmd();

    // Configure Stereo settings.
    ovrSizei recommenedTex0Size = ovrHmd_GetFovTextureSize(mpHmd, ovrEye_Left, mpHmd->DefaultEyeFov[0], 1.0f);
    ovrSizei recommenedTex1Size = ovrHmd_GetFovTextureSize(mpHmd, ovrEye_Right, mpHmd->DefaultEyeFov[1], 1.0f);

    mRenderWidth = max(recommenedTex0Size.w, recommenedTex1Size.w);
    mRenderHeight = max(recommenedTex0Size.h, recommenedTex1Size.h);

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


    ovrGLConfig cfg;
    cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
    cfg.OGL.Header.RTSize = mpHmd->Resolution;
    cfg.OGL.Header.Multisample = 1;

#ifdef LINUX
    cfg.OGL.Disp = platformInfo.pDisplay;
    cfg.OGL.Win = platformInfo.WindowId;
#endif

    ovrFovPort eyeFov[2];
    eyeFov[0] = mpHmd->DefaultEyeFov[0];
    eyeFov[1] = mpHmd->DefaultEyeFov[1];

    unsigned distortionCaps = ovrDistortionCap_Chromatic;

    bool worked = ovrHmd_ConfigureRendering(mpHmd, &cfg.Config, distortionCaps, eyeFov, mEyeRenderDesc);
    if (!worked)
    {
        return false;
    }

    qglBindBuffer(GL_ARRAY_BUFFER, 0);
    qglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    Sizei texSize = Sizei(mRenderWidth, mRenderHeight);
    Sizei renderSize = Sizei(mRenderWidth, mRenderHeight);

    ovrGLTextureData* texData = (ovrGLTextureData*)&EyeTexture[0];
    texData->Header.API            = ovrRenderAPI_OpenGL;
    texData->Header.TextureSize    = texSize;
    texData->Header.RenderViewport = Recti(Vector2i(0), renderSize);
    texData->TexId                 = mFboInfos[0].ColorBuffer;

    texData = (ovrGLTextureData*)&EyeTexture[1];
    texData->Header.API            = ovrRenderAPI_OpenGL;
    texData->Header.TextureSize    = texSize;
    texData->Header.RenderViewport = Recti(Vector2i(0), renderSize);
    texData->TexId                 = mFboInfos[1].ColorBuffer;


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


void HmdRendererOculusSdk::BeginRenderingForEye(bool leftEye)
{
    if (!mIsInitialized)
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
        ovrFrameTiming hmdFrameTiming = ovrHmd_BeginFrame(mpHmd, 0);

        for (int i=0; i<FBO_COUNT; i++)
        {
            qglBindFramebuffer(GL_FRAMEBUFFER, mFboInfos[i].Fbo);
            RenderTool::ClearFBO(mFboInfos[i]);

            mEyes[i] = mpHmd->EyeRenderOrder[i];
            mEyePoses[i] = ovrHmd_GetEyePose(mpHmd, mEyes[i]);
            mCurrentOrientations[i] = Quatf(mEyePoses[i].Orientation);
        }

        qglBindBuffer(GL_ARRAY_BUFFER, 0);
        qglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        qglBindVertexArray(0);
        qglUseProgramObjectARB(0);
    }

    // bind framebuffer
    qglBindFramebuffer(GL_FRAMEBUFFER, mFboInfos[mEyeId].Fbo);


}

void HmdRendererOculusSdk::EndFrame()
{
    if (!mIsInitialized && mEyeId < 0)
    {
        return;
    }

    if (mStartedRendering)
    {
        //render end
        ovrHmd_EndFrame(mpHmd, mEyePoses, EyeTexture);

        // cleanup the OculusSdk state mess
        qglBindBuffer(GL_ARRAY_BUFFER, 0);
        qglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        qglBindVertexArray(0);
        qglUseProgramObjectARB(0);

        mStartedRendering = false;

        // keep for debugging
        //RenderTool::DrawFbos(&mFboInfos[0], FBO_COUNT, mWindowWidth, mWindowHeight);

    }

    mEyeId = -1;
}


bool HmdRendererOculusSdk::GetCustomProjectionMatrix(float* rProjectionMatrix, float zNear, float zFar, float fov)
{
    if (!mIsInitialized)
    {
        return false;
    }

    //float fovRad = DEG2RAD(fov);
    ovrMatrix4f projMatrix = ovrMatrix4f_Projection(mEyeRenderDesc[mEyeId].Fov, zNear, zFar, true);
    ConvertMatrix(projMatrix, rProjectionMatrix);

    return true;
}

bool HmdRendererOculusSdk::GetCustomViewMatrix(float* rViewMatrix, float xPos, float yPos, float zPos, float bodyYaw)
{

    if (!mIsInitialized)
    {
        return false;
    }


    // change hmd orientation to game coordinate system
    Matrix4f hmdRotation = Matrix4f(mCurrentOrientations[mEyeId].Inverted()) * Matrix4f::RotationX(DEG2RAD(-90.0f)) * Matrix4f::RotationZ(DEG2RAD(90));

    // convert body transform to matrix
    Matrix4f bodyYawRotation = Matrix4f::RotationZ(DEG2RAD(-bodyYaw));
    Matrix4f bodyPosition = Matrix4f::Translation(-xPos, -yPos, -zPos);

    mCurrentView = hmdRotation * bodyYawRotation * bodyPosition;

    // apply ipd
    float meterToGame = (3.2808f * 2.0f); // meter to feet * game factor 2
    Vector3f viewAdjust = mEyeRenderDesc[mEyeId].ViewAdjust;
    viewAdjust *= meterToGame;
    mCurrentView = Matrix4f::Translation(viewAdjust) * mCurrentView;

    ConvertMatrix(mCurrentView, rViewMatrix);

    return true;
}

bool HmdRendererOculusSdk::Get2DViewport(int& rX, int& rY, int& rW, int& rH)
{
    // shrink the gui for the HMD display
    float scale = 0.3f;
    float aspect = 1.0f;

    rW = mRenderWidth * scale;
    rH = mRenderWidth* scale * aspect;

    rX = (mRenderWidth - rW)/2.0f;
    int xOff = mRenderWidth/5.0f;
    xOff *= mEyeId == 0 ? 1 : -1;
    rX += xOff;

    rY = (mRenderHeight - rH)/2;
}

void HmdRendererOculusSdk::ConvertMatrix(const Matrix4f& from, float* rTo)
{
    rTo[0] = from.M[0][0];
    rTo[4] = from.M[0][1];
    rTo[8] = from.M[0][2];
    rTo[12] = from.M[0][3];

    rTo[1] = from.M[1][0];
    rTo[5] = from.M[1][1];
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
