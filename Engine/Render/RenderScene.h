#pragma once

#include <Core/CoreHeader.h>
#include "RenderItem.h"

namespace z {


struct RenderScene : public RefCounter {
public:;

	void Reset() {}


	math::Matrix4F ViewMatrix;
	math::Matrix4F ViewProjMatrix;

	std::vector<RefCountPtr<RenderItem>> RenderItems;
};

}