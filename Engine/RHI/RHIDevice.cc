#include "RHIDevice.h"
#include "RHIResource.h"

#include <RHIDX12/DX12Device.h>
#include <Core/Common/Singleton.h>

namespace z {

RHIDevice* GDevice = nullptr;

RHIDevice::RHIDevice() {
	InitializeSingleton<RHIDevice>(GDevice, this);
}

RHIDevice::~RHIDevice() {
	FinalizeSingleton<RHIDevice>(GDevice, this);
}


std::vector<RHIResource*> RHIResource::gWaitDelReousrce;




}