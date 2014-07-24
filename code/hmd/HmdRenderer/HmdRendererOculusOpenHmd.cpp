#include "HmdRendererOculusOpenHmd.h"
#include "../HmdDevice/HmdDeviceOpenHmd.h"
#include "../../renderer/tr_local.h"
#include "PlatformInfo.h"
#include "../ClientHmd.h"

#include <math.h>


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


HmdRendererOculusOpenHmd::HmdRendererOculusOpenHmd(HmdDeviceOpenHmd *pHmdDeviceOpenHmd)
    :mIsInitialized(false)
    ,mOculusProgram(0)
    ,mWindowWidth(0)
    ,mWindowHeight(0)
    ,mRenderWidth(0)
    ,mRenderHeight(0)
    ,mCurrentFbo(-1)
    ,mpDevice(pHmdDeviceOpenHmd)
    ,mpHmd(NULL)
    ,mInterpupillaryDistance(0)
{
    
}

HmdRendererOculusOpenHmd::~HmdRendererOculusOpenHmd()
{
    
}

bool HmdRendererOculusOpenHmd::Init(int windowWidth, int windowHeight, PlatformInfo platformInfo)
{
    if (mpDevice == NULL || mpDevice->GetHmd() == NULL)
    {
        return false;
    }
    
    mpHmd = mpDevice->GetHmd();
    
    mWindowWidth = windowWidth;
    mWindowHeight = windowHeight;
    
    // use higher render resolution for a better result
    mRenderWidth = 1122; //mWindowWidth / 2;
    mRenderHeight = 1553; //mWindowHeight;

    
    for (int i=0; i<FBO_COUNT; i++)
    {
        bool worked = RenderTool::CreateFrameBuffer(mFboInfos[i], mRenderWidth, mRenderHeight);
        if (!worked)
        {
            return false;
        }
    }
    
    
    mOculusProgram = RenderTool::CreateShaderProgram(GetVertexShader(), GetPixelShader());  
    
    ohmd_device_getf(mpHmd, OHMD_EYE_IPD, &mInterpupillaryDistance);
    
    
    mCurrentFbo = -1;
    mIsInitialized = true;
    
    return true;
}


void HmdRendererOculusOpenHmd::Shutdown()
{
    mpHmd = NULL;
    
    if (!mIsInitialized)
    {
        return;
    }
    
    qglBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    
    mIsInitialized = false;
}

std::string HmdRendererOculusOpenHmd::GetInfo()
{
    return "HmdRendererOculusOpenHmd";
}

bool HmdRendererOculusOpenHmd::HandlesSwap()
{
    return false;
}

bool HmdRendererOculusOpenHmd::GetRenderResolution(int &rWidth, int &rHeight)
{
    if (!mIsInitialized)
    {
        return false;
    }
    
    rWidth = mRenderWidth;
    rHeight = mRenderHeight;
    
    return true;
}


void HmdRendererOculusOpenHmd::BeginRenderingForEye(bool leftEye)
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
    
    if (mCurrentFbo == fboId)
    {
        return;
    }
    
    mCurrentFbo = fboId;
    
	qglBindFramebuffer(GL_FRAMEBUFFER, mFboInfos[mCurrentFbo].Fbo);
    RenderTool::ClearFBO(mFboInfos[mCurrentFbo]);
}

void HmdRendererOculusOpenHmd::EndFrame()
{
    if (!mIsInitialized)
    {
        return;
    }    
    
    RenderTool::DrawFbos(&mFboInfos[0], FBO_COUNT, mWindowWidth, mWindowHeight, mOculusProgram);
}


bool HmdRendererOculusOpenHmd::GetCustomProjectionMatrix(float *rProjectionMatrix, float zNear, float zFar, float fov)
{   
    if (!mIsInitialized || mpHmd == NULL)
    {
        return false;
    }


    // don't use openhmd projection matrix, because setting zNear and zFar is not supported
    // to be clear: it can be set but won't affect the projection matrix creation
    
    //ohmd_float_value type = stereoLeft ? OHMD_LEFT_EYE_GL_PROJECTION_MATRIX : OHMD_RIGHT_EYE_GL_PROJECTION_MATRIX;
    //ohmd_device_getf(mpHmd, type, rProjectionMatrix);
    
    float fov_rad = DEG2RAD(fov);
    //ohmd_device_getf(mpHmd, mCurrentFbo == 0 ? OHMD_LEFT_EYE_FOV : OHMD_RIGHT_EYE_FOV, &fov_rad);  
    
    float aspect = 0;
    // can't use openhmd - bug -> returns fov instead of aspect
    //ohmd_device_getf(mpHmd, stereoLeft ? OHMD_LEFT_EYE_ASPECT_RATIO : OHMD_RIGHT_EYE_ASPECT_RATIO, &aspect);  
    aspect = mWindowWidth / (2.0f* mWindowHeight);
    
    if (fov_rad == 0 || aspect == 0)
    {
        return false;
    }
    
    
    float hScreenSize = 0;
    float lensSeparation = 0;
    ohmd_device_getf(mpHmd, OHMD_SCREEN_HORIZONTAL_SIZE, &hScreenSize);
    ohmd_device_getf(mpHmd, OHMD_LENS_HORIZONTAL_SEPARATION, &lensSeparation);
    
    
    float screenCenter = hScreenSize / 4.0f;
	float lensShift = screenCenter - lensSeparation / 2.0f;
	float projOffset = 4.0f * lensShift / hScreenSize;
    projOffset *= mCurrentFbo == 0 ? 1.f : -1.f;  
    	
    glm::mat4 perspMat = glm::perspective(fov_rad, aspect, zNear, zFar);
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(projOffset, 0, 0));
    perspMat = translate * perspMat;
    
    memcpy(rProjectionMatrix, &perspMat[0][0], sizeof(float)*16);
    
    return true;
}

bool HmdRendererOculusOpenHmd::GetCustomViewMatrix(float *rViewMatrix, float xPos, float yPos, float zPos, float bodyYaw)
{  
    if (!mIsInitialized)
    {
        return false;
    }    
        
    // get current hmd rotation
    float quat[4];
    ohmd_device_getf(mpHmd, OHMD_ROTATION_QUAT, &quat[0]);
    glm::quat hmdRotation = glm::inverse(glm::quat(quat[3], quat[0], quat[1], quat[2]));
    
    // change hmd orientation to game coordinate system   
    glm::quat convertHmdToGame = glm::rotate(glm::quat(1.0f,0.0f,0.0f,0.0f), (float)DEG2RAD(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    convertHmdToGame = glm::rotate(convertHmdToGame, (float)DEG2RAD(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));      
    
    glm::mat4 hmdRotationMat = glm::mat4_cast(hmdRotation) * glm::mat4_cast(convertHmdToGame);
    
   
    // convert body transform to matrix
    glm::mat4 bodyPosition = glm::translate(glm::mat4(1.0f), glm::vec3(-xPos, -yPos, -zPos));
    glm::quat bodyYawRotation = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), (float)(DEG2RAD(-bodyYaw)), glm::vec3(0.0f, 0.0f, 1.0f));
    
    // create view matrix
    glm::mat4 viewMatrix = hmdRotationMat * glm::mat4_cast(bodyYawRotation) * bodyPosition;
    
    //meter to game unit (game unit = feet*2)
    float meterToGame = 3.28084f*2.0f;
    // apply ipd    
    float halfIPD = mInterpupillaryDistance * 0.5f * meterToGame * (mCurrentFbo == 0 ? 1.0f : -1.0f);
    
    glm::mat4 translateIpd = glm::translate(glm::mat4(1.0f), glm::vec3(halfIPD, 0, 0));
    viewMatrix = translateIpd * viewMatrix;
    
    memcpy(rViewMatrix, &viewMatrix[0][0], sizeof(float) * 16);


    return true;
}

bool HmdRendererOculusOpenHmd::Get2DViewport(int &rX, int &rY, int &rW, int &rH)
{
    // shrink the gui for the HMD display
    float scale = 0.3f;
    float aspect = 1.0f;
    
    rW = mRenderWidth * scale;
    rH = mRenderWidth* scale * aspect;
    
    rX = (mRenderWidth - rW)/2.0f;
    int xOff = mRenderWidth/12.5f;
    xOff *= mCurrentFbo == 0 ? 1 : -1;
    rX += xOff; 
    
    rY = (mRenderHeight - rH)/2;

	return true;
}



const char *HmdRendererOculusOpenHmd::GetVertexShader()
{
    return
    "void main() {\n"
    "   gl_TexCoord[0] = gl_MultiTexCoord0;\n"
    "   gl_Position = ftransform();\n"
    "}";
}

const char *HmdRendererOculusOpenHmd::GetPixelShader()
{
    // only works with DK1
    
    return
            
"#version 120\n"
"\n"            
"            // Taken from mts3d forums, from user fredrik.\n"
"           \n"
"            uniform sampler2D warpTexture;\n"
"            \n"
"            const vec2 LeftLensCenter = vec2(0.2863248, 0.5);\n"
"            const vec2 RightLensCenter = vec2(0.7136753, 0.5);\n"
"            const vec2 LeftScreenCenter = vec2(0.25, 0.5);\n"
"            const vec2 RightScreenCenter = vec2(0.75, 0.5);\n"
"            const vec2 Scale = vec2(0.1469278, 0.2350845);\n"
"            const vec2 ScaleIn = vec2(4, 2.5);\n"
"            //const vec2 Scale = vec2(1.0, 1.0);\n"
"            //const vec2 ScaleIn = vec2(1, 1.0);\n"            
"            const vec4 HmdWarpParam   = vec4(1, 0.22, 0.24, 0);\n"
"            \n"
"            // Scales input texture coordinates for distortion.\n"
"            vec2 HmdWarp(vec2 in01, vec2 LensCenter)\n"
"            {\n"
"                vec2 theta = (in01 - LensCenter) * ScaleIn; // Scales to [-1, 1]\n"
"                float rSq = theta.x * theta.x + theta.y * theta.y;\n"
"                vec2 rvector = theta * (HmdWarpParam.x + HmdWarpParam.y * rSq +\n"
"                    HmdWarpParam.z * rSq * rSq +\n"
"                    HmdWarpParam.w * rSq * rSq * rSq);\n"
"                return LensCenter + Scale * rvector;\n"
"            }\n"
"            \n"
"            void main()\n"
"            {\n"
"                // The following two variables need to be set per eye\n"
"                vec2 LensCenter = gl_FragCoord.x < 640 ? LeftLensCenter : RightLensCenter;\n"
"                vec2 ScreenCenter = gl_FragCoord.x < 640 ? LeftScreenCenter : RightScreenCenter;\n"
"            \n"
"                vec2 oTexCoord = gl_FragCoord.xy / vec2(1280, 800);\n"
"            \n"
"                vec2 tc = HmdWarp(oTexCoord, LensCenter);\n"
"                if (any(bvec2(clamp(tc,ScreenCenter-vec2(0.25,0.5), ScreenCenter+vec2(0.25,0.5)) - tc)))\n"
"                {\n"
"                    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
"                    return;\n"
"                }\n"
"            \n"
"                tc.x = gl_FragCoord.x < 640 ? (2.0 * tc.x) : (2.0 * (tc.x - 0.5));\n"
"                gl_FragColor = texture2D(warpTexture, tc);\n"
"            }";
            

}
