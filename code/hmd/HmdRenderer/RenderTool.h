#ifndef RENDERTOOL_H
#define RENDERTOOL_H

#include <stdio.h>
#include "../../renderer/qgl.h"


class RenderTool
{
public:
    struct FrameBufferInfo
    {
        GLuint  Fbo;
        GLuint  DepthBuffer;
        GLuint  ColorBuffer;
        int Width;
        int Height;
    };


    static bool CreateFrameBuffer(FrameBufferInfo& rInfo, int width, int height);
    static void ClearFBO(FrameBufferInfo info);
    static void DrawFbos(FrameBufferInfo* pFbos, int fboCount, int windowWidth, int windowHeight, unsigned int shaderProg = 0);

    static GLhandleARB CreateShaderProgram(const char* pVertexShader, const char* pFragmentShader);

};

#endif
