#include "SearchForDisplay.h"

#ifdef USE_SDL2
#ifdef LINUX
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#endif


using namespace std;

bool SearchForDisplay::GetDisplayPosition(string displayName, int resolutionW, int resolutionH, DisplayInfo& rInfo)
{

    // only implemented for SDL2
#ifdef USE_SDL2
    
    int displayId = -1;
    std::string foundDisplayName = "";
    
    int displayCount = SDL_GetNumVideoDisplays();
    for (int i=0; i<displayCount; i++)
    {
		const char* name = SDL_GetDisplayName(i);
        if (displayName.length() > 0)
        {
            if (strcmp(name, displayName.c_str()) == 0)
            {
                displayId = i;
                foundDisplayName = name;
                break;
            }
        }

        SDL_Rect r;
        int ret = SDL_GetDisplayBounds(i, &r);
        if (ret == 0)
        {
            if (r.w == resolutionW && r.h == resolutionH)
            {
                // this is a fallback, if the display name is not correct
                displayId = i;
                foundDisplayName = name;
            }
        }
        else if (ret != 0)
        {
            const char* error = SDL_GetError();
            printf("SDL_GetDisplayBounds failed: %s\n", error);

        }

        //printf("display name: %s\n", displayName);
        //flush(std::cout);
    }

    if (displayId >= 0)
    {
        SDL_Rect r;
        int ret = SDL_GetDisplayBounds(displayId, &r);
        if (ret == 0)
        {
            bool isRotated = false;
            
            // this code only works on Linux for now
            #ifdef LINUX
            if (resolutionW == r.h && resolutionH == r.w)
            {
                isRotated = true;
            }
            #endif

            //printf("display x=%d y=%d\n", r.x, r.y);
            //flush(std::cout);
            
            rInfo.id = displayId;
            rInfo.name = foundDisplayName;
            rInfo.posX = r.x;
            rInfo.posY = r.y;
            rInfo.isRotated = isRotated;
            
            return true;
        }
    }
    
#endif    
    
    return false;

    
}

bool SearchForDisplay::GetDisplayPosition(int posX, int posY, int resolutionW, int resolutionH, DisplayInfo& rInfo)
{
	// only implemented for SDL2
#ifdef USE_SDL2

	int displayCount = SDL_GetNumVideoDisplays();
	for (int i = 0; i<displayCount; i++)
	{
		const char* name = SDL_GetDisplayName(i);

		SDL_Rect r;
		int ret = SDL_GetDisplayBounds(i, &r);
		if (ret == 0)
		{
			if (r.x == posX && r.y == posY)
			{
				// found display pos
				if (r.w == resolutionW && r.h == resolutionH ||
					r.w == resolutionH && r.h == resolutionW)
				{
					// found display with corret resolution on correction position
					rInfo.id = i;
					rInfo.name = name;
					rInfo.posX = posX;
					rInfo.posY = posY;
					rInfo.isRotated = r.w == resolutionH;
					return true;
				}
			}
		}
		else if (ret != 0)
		{
			const char* error = SDL_GetError();
			printf("SDL_GetDisplayBounds failed: %s\n", error);

		}

		//printf("display name: %s\n", displayName);
		//flush(std::cout);
	}

#endif    

	return false;


}

#ifdef USE_TEST_DUMMY_CODE_ASF

// GLFWOculusRiftTest
// (c) cThrough 2014 (Daniel Dekkers)
// Version 2014110800 Based on DK2, OculusSDK 4.0.3 beta

#include <GL/glew.h>
#if defined(_WIN32)
#include <Windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#elif defined(__linux__)
#include <X11/X.h>
#include <X11/extensions/Xrandr.h>
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#endif
#include <GLFW/glfw3.h>
#if !defined(__APPLE__)
#include <GLFW/glfw3native.h>
#endif
#include <OVR.h>
#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>

const bool l_FullScreen = false;
const bool l_MultiSampling = true;
const bool l_Spin = true;

int g_DistortionCaps = 0
| ovrDistortionCap_Vignette
| ovrDistortionCap_Chromatic
| ovrDistortionCap_Overdrive
// | ovrDistortionCap_TimeWarp // Turning this on gives ghosting???
;

struct c_FBO
{
public:
	void Create(ovrSizei p_Size)
	{
		glGenFramebuffers(1, &m_FBOId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBOId);

		// The texture we're going to render to...
		glGenTextures(1, &m_TextureId);
		// "Bind" the newly created texture : all future texture functions will modify this texture...
		glBindTexture(GL_TEXTURE_2D, m_TextureId);
		// Give an empty image to OpenGL (the last "0")
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p_Size.w, p_Size.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		// Linear filtering...
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Create Depth Buffer...
		glGenRenderbuffers(1, &m_DepthBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, p_Size.w, p_Size.h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBufferId);

		// Set the texture as our colour attachment #0...
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_TextureId, 0);

		// Set the list of draw buffers...
		GLenum l_GLDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, l_GLDrawBuffers); // "1" is the size of DrawBuffers

		// Check if everything is OK...
		GLenum l_Check = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
		if (l_Check != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("There is a problem with the FBO.\n");
			exit(EXIT_FAILURE);
		}

		// Unbind...
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Cleanup(void)
	{
		// TODO...
	}

	GLuint GetTextureId(void)
	{
		return m_TextureId;
	}

	GLuint GetFBOId(void)
	{
		return m_FBOId;
	}

protected:
private:
	GLuint m_FBOId;
	GLuint m_TextureId;
	GLuint m_DepthBufferId;
};

ovrHmd g_Hmd;
ovrGLConfig g_Cfg;
ovrEyeRenderDesc g_EyeRenderDesc[2];
ovrVector3f g_EyeOffsets[2];
ovrPosef g_EyePoses[2];
ovrTexture g_EyeTextures[2];
OVR::Matrix4f g_ProjectionMatrici[2];
c_FBO g_EyeFBOs[2];

GLfloat l_VAPoints[] =
{
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f
};

GLfloat l_VANormals[] =
{
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f
};

GLuint l_VAIndici[] =
{
	0, 1, 2, 3,
	4, 5, 6, 7,
	8, 9, 10, 11,
	12, 13, 14, 15,
	16, 17, 18, 19,
	20, 21, 22, 23
};

// =============================================================================

static void ErrorCallback(int p_Error, const char* p_Description)
{
	fputs(p_Description, stderr);
}

// =============================================================================

static void KeyCallback(GLFWwindow* p_Window, int p_Key, int p_Scancode, int p_Action, int p_Mods)
{
	if (p_Action == GLFW_PRESS)
	{
		if (p_Key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(p_Window, GL_TRUE);
		}
		else
		if (p_Key == GLFW_KEY_R)
		{
			ovrHmd_RecenterPose(g_Hmd);
		}

		ovrHSWDisplayState l_HasWarningState;
		ovrHmd_GetHSWDisplayState(g_Hmd, &l_HasWarningState);
		if (l_HasWarningState.Displayed) ovrHmd_DismissHSWDisplay(g_Hmd);
	}
}

// =============================================================================

static void WindowSizeCallback(GLFWwindow* p_Window, int p_Width, int p_Height)
{
	g_Cfg.OGL.Header.RTSize.w = p_Width;
	g_Cfg.OGL.Header.RTSize.h = p_Height;

	ovrBool l_ConfigureResult = ovrHmd_ConfigureRendering(g_Hmd, &g_Cfg.Config, g_DistortionCaps, g_Hmd->MaxEyeFov, g_EyeRenderDesc);
	glUseProgram(0); // Avoid OpenGL state leak in ovrHmd_ConfigureRendering...
	if (!l_ConfigureResult)
	{
		printf("Configure failed.\n");
		exit(EXIT_FAILURE);
	}
}

// ============================================================================

void RenderCubeVertexArrays(void)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, l_VAPoints);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, l_VANormals);

	glDrawElements(GL_QUADS, 6 * 4, GL_UNSIGNED_INT, l_VAIndici);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

// ============================================================================

void RenderCubeFixedFunction(void)
{
	// Obsolete, remains as a fall back for the vertex arrays version...
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);

	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);

	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glEnd();
}

// ============================================================================

static void SetOpenGLState(void)
{
	// Some state...
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (l_MultiSampling) glEnable(GL_MULTISAMPLE); else glDisable(GL_MULTISAMPLE);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);

	// Material...
	GLfloat l_MaterialSpecular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat l_MaterialShininess[] = { 10.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, l_MaterialSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, l_MaterialShininess);

	// Some (stationary) lights, position will be set every frame separately...
	GLfloat l_Light0Diffuse[] = { 1.0f, 0.8f, 0.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_Light0Diffuse);
	glEnable(GL_LIGHT0);

	GLfloat l_Light1Diffuse[] = { 0.6f, 0.8f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l_Light1Diffuse);
	glEnable(GL_LIGHT1);
}

// ============================================================================

static void SetStaticLightPositions(void)
{
	GLfloat l_Light0Position[] = { 3.0f, 4.0f, 2.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, l_Light0Position);

	GLfloat l_Light1Position[] = { -3.0f, -4.0f, 2.0f, 0.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, l_Light1Position);
}

// =============================================================================

int main(void)
{
	// Initialize LibOVR...
	ovr_Initialize();

	// Check for attached head mounted display...
	g_Hmd = ovrHmd_Create(0);
	if (!g_Hmd) g_Hmd = ovrHmd_CreateDebug(ovrHmd_DK2);

	// Start the sensor which provides the Rift’s pose and motion...
	uint32_t l_SupportedSensorCaps = ovrTrackingCap_Orientation | ovrTrackingCap_Position | ovrTrackingCap_MagYawCorrection;
	uint32_t l_RequiredTrackingCaps = 0;
	ovrBool l_TrackingResult = ovrHmd_ConfigureTracking(g_Hmd, l_SupportedSensorCaps, l_RequiredTrackingCaps);
	if (!l_TrackingResult)
	{
		printf("Could not start tracking...");
		exit(EXIT_FAILURE);
	}

	// Create a window...
	GLFWwindow* l_Window;

	glfwSetErrorCallback(ErrorCallback);

	if (!glfwInit()) exit(EXIT_FAILURE);

	if (l_MultiSampling) glfwWindowHint(GLFW_SAMPLES, 4); else glfwWindowHint(GLFW_SAMPLES, 0);

	// Get the proper monitor, when either in "Extended" or "Direct" mode...
	GLFWmonitor* l_Monitor;
	if (g_Hmd->HmdCaps & ovrHmdCap_ExtendDesktop)
	{
		printf("Running in \"Extended Desktop\" mode...\n");
		int l_Count;
		GLFWmonitor** l_Monitors = glfwGetMonitors(&l_Count);
		switch (l_Count)
		{
		case 0:
			printf("No monitors found, exiting.\n");
			exit(EXIT_FAILURE);
			break;
		case 1:
			printf("Two monitors expected, found only one, using primary...\n");
			l_Monitor = glfwGetPrimaryMonitor();
			break;
		case 2:
			printf("Two monitors found, using second monitor\n");
			l_Monitor = l_Monitors[1];
			break;
		default:
			printf("More than two monitors found, using second monitor\n");
			l_Monitor = l_Monitors[1];
		}
	}
	else
	{
		printf("Running in \"Direct\" mode...\n");
		l_Monitor = glfwGetPrimaryMonitor();
	}

	ovrSizei l_ClientSize;
	if (l_FullScreen)
	{
		l_ClientSize.w = g_Hmd->Resolution.w; // 1920 for DK2...
		l_ClientSize.h = g_Hmd->Resolution.h; // 1080 for DK2...
		// Create a fullscreen window with the Oculus Rift resolution...
		l_Window = glfwCreateWindow(l_ClientSize.w, l_ClientSize.h, "GLFW Oculus Rift Test", l_Monitor, NULL);
	}
	else
	{
		l_ClientSize.w = g_Hmd->Resolution.w / 2; // Something reasonable, smaller, but maintain aspect ratio...
		l_ClientSize.h = g_Hmd->Resolution.h / 2; // Something reasonable, smaller, but maintain aspect ratio...
		l_Window = glfwCreateWindow(l_ClientSize.w, l_ClientSize.h, "GLFW Oculus Rift Test", NULL, NULL);
	}

	// Check if window creation was succesfull...
	if (!l_Window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Make the context current for this window...
	glfwMakeContextCurrent(l_Window);

	// Attach the window in Direct mode...
	if (g_Hmd->HmdCaps & ovrHmdCap_ExtendDesktop)
	{
		// ...
	}
	else
	{
#if defined(_WIN32)
		ovrBool l_AttachResult = ovrHmd_AttachToWindow(g_Hmd, glfwGetWin32Window(l_Window), NULL, NULL);
		if (!l_AttachResult)
		{
			printf("Could not attach to window...");
			exit(EXIT_FAILURE);
		}
#endif
	}

	// Don't forget to initialize Glew, turn glewExperimental on to avoid problem fetching function pointers...
	glewExperimental = GL_TRUE;
	GLenum l_GlewResult = glewInit();
	if (l_GlewResult != GLEW_OK)
	{
		printf("glewInit() error.\n");
		exit(EXIT_FAILURE);
	}

	// Print some info about the OpenGL context...
	int l_Major = glfwGetWindowAttrib(l_Window, GLFW_CONTEXT_VERSION_MAJOR);
	int l_Minor = glfwGetWindowAttrib(l_Window, GLFW_CONTEXT_VERSION_MINOR);
	int l_Profile = glfwGetWindowAttrib(l_Window, GLFW_OPENGL_PROFILE);
	printf("OpenGL: %d.%d ", l_Major, l_Minor);
	if (l_Major >= 3) // Profiles introduced in OpenGL 3.0...
	{
		if (l_Profile == GLFW_OPENGL_COMPAT_PROFILE) printf("GLFW_OPENGL_COMPAT_PROFILE\n"); else printf("GLFW_OPENGL_CORE_PROFILE\n");
	}
	printf("Vendor: %s\n", (char*)glGetString(GL_VENDOR));
	printf("Renderer: %s\n", (char*)glGetString(GL_RENDERER));

	// Create some lights, materials, etc...
	SetOpenGLState();

	// Oculus Rift eye configurations...
	g_Cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
	g_Cfg.OGL.Header.RTSize.w = l_ClientSize.w;
	g_Cfg.OGL.Header.RTSize.h = l_ClientSize.h;
	g_Cfg.OGL.Header.Multisample = (l_MultiSampling ? 1 : 0);
#if defined(_WIN32)
	g_Cfg.OGL.Window = glfwGetWin32Window(l_Window);
	g_Cfg.OGL.DC = GetDC(g_Cfg.OGL.Window);
#elif defined(__linux__)
	l_Cfg.OGL.Win = glfwGetX11Window(l_Window);
	l_Cfg.OGL.Disp = glfwGetX11Display();
#endif

	ovrBool l_ConfigureResult = ovrHmd_ConfigureRendering(g_Hmd, &g_Cfg.Config, g_DistortionCaps, g_Hmd->MaxEyeFov, g_EyeRenderDesc);
	glUseProgram(0); // Avoid OpenGL state leak in ovrHmd_ConfigureRendering...
	if (!l_ConfigureResult)
	{
		printf("Configure failed.\n");
		exit(EXIT_FAILURE);
	}

	// Setup textures for each eye as well as create the eye FBOs, get projection matrici and IPD offsets...
	memset(g_EyeTextures, 0, 2 * sizeof(ovrGLTexture));
	for (int l_EyeIndex = 0; l_EyeIndex<ovrEye_Count; l_EyeIndex++)
	{
		ovrEyeType l_Eye = g_Hmd->EyeRenderOrder[l_EyeIndex]; // Eye order not really needed here but to get an ovrEyeType...

		g_EyeTextures[l_Eye].Header.API = ovrRenderAPI_OpenGL;
		g_EyeTextures[l_Eye].Header.TextureSize = ovrHmd_GetFovTextureSize(g_Hmd, l_Eye, g_Hmd->DefaultEyeFov[l_Eye], 1.0f);
		g_EyeFBOs[l_Eye].Create(g_EyeTextures[l_Eye].Header.TextureSize);
		g_EyeTextures[l_Eye].Header.RenderViewport.Size = g_EyeTextures[l_Eye].Header.TextureSize;
		g_EyeTextures[l_Eye].Header.RenderViewport.Pos.x = 0;
		g_EyeTextures[l_Eye].Header.RenderViewport.Pos.y = 0;
		((ovrGLTexture&)(g_EyeTextures[l_Eye])).OGL.TexId = g_EyeFBOs[l_Eye].GetTextureId();

		g_ProjectionMatrici[l_Eye] = ovrMatrix4f_Projection(g_EyeRenderDesc[l_Eye].Fov, 0.3f, 100.0f, true);
		g_EyeOffsets[l_Eye] = g_EyeRenderDesc[l_Eye].HmdToEyeViewOffset;
	}

	glfwSetKeyCallback(l_Window, KeyCallback);
	glfwSetWindowSizeCallback(l_Window, WindowSizeCallback);

	GLfloat l_SpinX;
	GLfloat l_SpinY;

	// Do a single recenter to calibrate orientation to current state of the Rift...
	ovrHmd_RecenterPose(g_Hmd);

	// Main loop...
	unsigned int l_FrameIndex = 0;
	ovrFrameTiming l_HmdFrameTiming;
	while (!glfwWindowShouldClose(l_Window))
	{
		if (l_Spin)
		{
			l_SpinX = (GLfloat)fmod(glfwGetTime()*17.0, 360.0);
			l_SpinY = (GLfloat)fmod(glfwGetTime()*23.0, 360.0);
		}
		else
		{
			l_SpinX = 30.0f;
			l_SpinY = 40.0f;
		}

		ovrHmd_GetEyePoses(g_Hmd, l_FrameIndex, g_EyeOffsets, g_EyePoses, NULL);
		l_HmdFrameTiming = ovrHmd_BeginFrame(g_Hmd, l_FrameIndex);

		for (int l_EyeIndex = 0; l_EyeIndex<ovrEye_Count; l_EyeIndex++)
		{
			ovrEyeType l_Eye = g_Hmd->EyeRenderOrder[l_EyeIndex];

			// Bind the correct FBO according to current eye...
			glBindFramebuffer(GL_FRAMEBUFFER, g_EyeFBOs[l_Eye].GetFBOId());

			// Clear...
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glViewport(
				g_EyeTextures[l_Eye].Header.RenderViewport.Pos.x,
				g_EyeTextures[l_Eye].Header.RenderViewport.Pos.y,
				g_EyeTextures[l_Eye].Header.RenderViewport.Size.w,
				g_EyeTextures[l_Eye].Header.RenderViewport.Size.h
				);

			// Pass projection matrix on to OpenGL...
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMultMatrixf(&(g_ProjectionMatrici[l_Eye].Transposed().M[0][0]));

			// Create the model-view matrix...
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			// Translate for specific eye based on IPD...
			glTranslatef(g_EyeOffsets[l_Eye].x, g_EyeOffsets[l_Eye].y, g_EyeOffsets[l_Eye].z);

			// Multiply with orientation retrieved from sensor...
			OVR::Quatf l_Orientation = OVR::Quatf(g_EyePoses[l_Eye].Orientation);
			OVR::Matrix4f l_ModelViewMatrix = OVR::Matrix4f(l_Orientation.Inverted());
			glMultMatrixf(&(l_ModelViewMatrix.Transposed().M[0][0]));

			// Translation due to positional tracking (DK2)...
			// glTranslatef(-g_EyePoses[l_Eye].Position.x, -g_EyePoses[l_Eye].Position.y, -g_EyePoses[l_Eye].Position.z);

			// Move the world forward a bit to show the scene in front of us...
			glTranslatef(0.0f, 0.0f, -2.0f);

			// (Re)set the light positions so they don't move along with the cube...
			SetStaticLightPositions();

			// Make the cube spin...
			glRotatef(l_SpinX, 1.0f, 0.0f, 0.0f);
			glRotatef(l_SpinY, 0.0f, 1.0f, 0.0f);

			// Render...
			// RenderCubeFixedFunction();
			RenderCubeVertexArrays();
		}

		// Back to the default framebuffer...
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Do everything, distortion, front/back buffer swap...
		ovrHmd_EndFrame(g_Hmd, g_EyePoses, g_EyeTextures);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Avoid OpenGL state leak in ovrHmd_EndFrame...
		glBindBuffer(GL_ARRAY_BUFFER, 0); // Avoid OpenGL state leak in ovrHmd_EndFrame...

		++l_FrameIndex;

		glfwPollEvents();
	}

	glfwDestroyWindow(l_Window);

	glfwTerminate();

	ovrHmd_Destroy(g_Hmd);
	ovr_Shutdown();

	exit(EXIT_SUCCESS);
}

#endif