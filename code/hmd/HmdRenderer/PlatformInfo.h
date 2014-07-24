#ifndef PLATFORMINFO_H
#define PLATFORMINFO_H

struct PlatformInfo
{
public:
    int WindowWidth;
    int WindowHeight;
    
#ifdef LINUX
    Display* pDisplay;
    Window WindowId;
#endif
};


#endif
