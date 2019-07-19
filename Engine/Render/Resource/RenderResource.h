#pragma once

namespace z {

class RenderResource {
public:
	virtual void InitRHI() = 0;
	virtual void DestroyRHI() = 0;

private:
};

}