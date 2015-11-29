#include "FactoryHmdDevice.h"
#include "HmdDevice/HmdDeviceMouse.h"
#ifdef USE_OPENHMD
#include "HmdDevice/HmdDeviceOpenHmd.h"
#include "HmdRenderer/HmdRendererOculusOpenHmd.h"
#endif

#ifdef USE_OVR
#include "HmdDevice/HmdDeviceOculusSdk.h"
#include "HmdRenderer/HmdRendererOculusSdk.h"
#endif

#ifdef USE_OVR_0_8
#include "HmdDevice/HmdDeviceOculusSdkWin.h"
#include "HmdRenderer/HmdRendererOculusSdkWin.h"
#endif

#include "HmdRenderer/HmdRendererOculus.h"

#include <vector>
#include <memory.h>

using namespace std;

IHmdDevice* FactoryHmdDevice::CreateHmdDevice(HmdLibrary library, bool allowDummyDevice)
{
    vector<IHmdDevice*> devices;


#if defined(USE_OVR) || defined(USE_OVR_0_8)
    if (library == LIB_OVR || library == LIB_UNDEFINED)
    {
        devices.push_back(new HmdDeviceOculusSdk());
    }
#endif

    
#ifdef USE_OPENHMD
    if (library == LIB_OPENHMD || library == LIB_UNDEFINED)
    {
        devices.push_back(new HmdDeviceOpenHmd());
    }
#endif
    
    if (library == LIB_MOUSE_DUMMY)
    {
        // only use mouse dummy if it is forced
        devices.push_back(new HmdDeviceMouse());
    }

    IHmdDevice* pSelectedDevice = NULL;

    for (unsigned int i=0; i<devices.size(); i++)
    {
        if (pSelectedDevice == NULL)
        {
            bool worked = devices[i]->Init(allowDummyDevice);
            if (worked)
            {
                pSelectedDevice = devices[i];
            }
            else
            {
                devices[i]->Shutdown();
            }
        }

        if (pSelectedDevice != devices[i])
        {
            delete devices[i];
            devices[i] = NULL;
        }
    }

    devices.clear();

    return pSelectedDevice;
}

IHmdRenderer* FactoryHmdDevice::CreateRendererForDevice(IHmdDevice* pDevice)
{
    if (pDevice == NULL)
    {
        return NULL;
    }

    int width = 0;
    int height = 0;
    bool isRotated = false;
    bool isExtendedMode = false;

    bool needsRenderer = pDevice->GetDeviceResolution(width, height, isRotated, isExtendedMode);
    if (!needsRenderer)
    {
        return NULL;
    }




#if defined(USE_OVR) || defined(USE_OVR_0_8)
    HmdDeviceOculusSdk* pOculusSdk = dynamic_cast<HmdDeviceOculusSdk*>(pDevice);
    if (pOculusSdk != NULL)
    {
        //HmdRendererOculus* pRenderer = new HmdRendererOculus();
        HmdRendererOculusSdk* pRenderer = new HmdRendererOculusSdk(pOculusSdk);
        return pRenderer;
    }
#endif

#ifdef USE_OPENHMD
    HmdDeviceOpenHmd* pOpenHmd = dynamic_cast<HmdDeviceOpenHmd*>(pDevice);
    if (pOpenHmd != NULL)
    {
        //HmdRendererOculus* pRenderer = new HmdRendererOculus();
        HmdRendererOculusOpenHmd* pRenderer = new HmdRendererOculusOpenHmd(pOpenHmd);
        return pRenderer;
    }
#endif
    
    HmdDeviceMouse* pHmdMouse = dynamic_cast<HmdDeviceMouse*>(pDevice);
    if (pHmdMouse != NULL)
    {
        HmdRendererOculus* pRenderer = new HmdRendererOculus();
        return pRenderer;
    }

    return NULL;
}
