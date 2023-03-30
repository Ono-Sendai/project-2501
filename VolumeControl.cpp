/*=====================================================================
VolumeControl.cpp
-----------------
Copyright Nicholas Chapman 2023 -
=====================================================================*/
#include "VolumeControl.h"


#include <utils/ComObHandle.h>
#include <utils/Exception.h>
#include <utils/PlatformUtils.h>
#include <utils/StringUtils.h>
#include <utils/IncludeWindows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

static inline void throwOnError(HRESULT hres)
{
	if(FAILED(hres))
		throw glare::Exception("Error: " + PlatformUtils::COMErrorString(hres));
}


void setSystemVolume(float volume)
{
	ComObHandle<IMMDeviceEnumerator> deviceEnumerator;
	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (void**)&deviceEnumerator.ptr);  
	throwOnError(hr);

	ComObHandle<IMMDevice> defaultDevice;
	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice.ptr);  
	throwOnError(hr);

	ComObHandle<IAudioEndpointVolume> endpointVolume;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (void**)&endpointVolume.ptr);  
	throwOnError(hr);

	hr = endpointVolume->SetMasterVolumeLevelScalar(volume, NULL);  
	throwOnError(hr);
}


float getSystemVolume()
{
	ComObHandle<IMMDeviceEnumerator> deviceEnumerator;
	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (void**)&deviceEnumerator.ptr);  
	throwOnError(hr);

	ComObHandle<IMMDevice> defaultDevice;
	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice.ptr);  
	throwOnError(hr);

	ComObHandle<IAudioEndpointVolume> endpointVolume;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (void**)&endpointVolume.ptr);  
	throwOnError(hr);

	float currentVolume = 0;  
	hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);  
	throwOnError(hr);

	return currentVolume;
}


std::string getSystemVolumeDescription()
{
	return "Current system volume is " + doubleToStringMaxNDecimalPlaces(getSystemVolume(), 2) + ".\n";
}
