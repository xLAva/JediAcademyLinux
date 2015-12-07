#include "FactoryHmdDevice.h"
#include "HmdDevice/HmdDeviceMouse.h"
#ifdef USE_OPENHMD
#include "HmdDevice/HmdDeviceOpenHmd.h"
#include "HmdRenderer/HmdRendererOculusOpenHmd.h"
#endif

#ifdef USE_OVR_0_6
#include "OculusSdk_0.6/HmdDeviceOculusSdk.h"
#include "OculusSdk_0.6/HmdRendererOculusSdk.h"
#endif

#ifdef USE_OVR_CURRENT
#include "OculusSdk_current/HmdDeviceOculusSdk.h"
#include "OculusSdk_current/HmdRendererOculusSdk.h"
#endif

#include "HmdRenderer/HmdRendererOculus.h"

#include <vector>
#include <memory.h>

using namespace std;

IHmdDevice* FactoryHmdDevice::CreateHmdDevice(HmdLibrary library, bool allowDummyDevice)
{
    vector<IHmdDevice*> devices;


#ifdef USE_OVR
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




#ifdef USE_OVR
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
