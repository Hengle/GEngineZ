#pragma once

#include "RHIConst.h"

namespace z {

class RHIDevice {
public:
	RHIDevice();
	virtual ~RHIDevice();


};

extern RHIDevice* GDevice;


}