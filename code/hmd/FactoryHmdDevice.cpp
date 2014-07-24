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

#include "HmdRenderer/HmdRendererOculus.h"

#include <vector>
#include <memory.h>

using namespace std;

IHmdDevice* FactoryHmdDevice::CreateHmdDevice()
{
    vector<IHmdDevice*> devices;

#ifdef USE_OPENHMD
    devices.push_back(new HmdDeviceOpenHmd());   
#endif    
    
#ifdef USE_OVR
    devices.push_back(new HmdDeviceOculusSdk());   
#endif
    
#ifdef OVR_SIM_MOUSE
    devices.push_back(new HmdDeviceMouse());
#endif
    
    IHmdDevice* pSelectedDevice = NULL;
    
    for (int i=0; i<devices.size(); i++)
    {
        if (pSelectedDevice == NULL)
        {
            bool worked = devices[i]->Init();
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

IHmdRenderer *FactoryHmdDevice::CreateRendererForDevice(IHmdDevice *pDevice)
{
    if (pDevice == NULL)
    {
        return NULL;
    }
    
    int width = 0;
    int height = 0;
    
    bool needsRenderer = pDevice->GetDeviceResolution(width, height);
    if (!needsRenderer)
    {
        return NULL;
    }
    

#ifdef USE_OPENHMD
    HmdDeviceOpenHmd* pOpenHmd = dynamic_cast<HmdDeviceOpenHmd*>(pDevice);
    if (pOpenHmd != NULL)
    {
        //HmdRendererOculus* pRenderer = new HmdRendererOculus();
        HmdRendererOculusOpenHmd* pRenderer = new HmdRendererOculusOpenHmd(pOpenHmd);
        return pRenderer;
    }
#endif    
    
#ifdef USE_OVR
    HmdDeviceOculusSdk* pOculusSdk = dynamic_cast<HmdDeviceOculusSdk*>(pDevice);
    if (pOculusSdk != NULL)
    {
        //HmdRendererOculus* pRenderer = new HmdRendererOculus();
        HmdRendererOculusSdk* pRenderer = new HmdRendererOculusSdk(pOculusSdk);
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
