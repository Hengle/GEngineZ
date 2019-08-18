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



RHITexture* RHIDevice::CreateTexture2D(ERHIPixelFormat format, uint32_t sizeX, uint32_t sizeY, uint8_t mips, const uint8_t* data) {
	RHITextureDesc desc;
	desc.NumMips = mips;
	desc.SizeX = sizeX;
	desc.SizeY = sizeY;
	desc.SizeZ = 1;
	desc.Format = format;
	desc.Dimension = TEX_DIMENSION_2D;
	return CreateTexture(desc, data);
}


std::vector<RHIResource*> RHIResource::gWaitDelReousrce;




}