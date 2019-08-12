#pragma once
#include <Core/CoreHeader.h>
#include <Render/RenderConst.h>
#include <Render/Material.h>
#include <Render/Mesh.h>

namespace z {

struct RenderItem : RefCounter{
	ERenderSet renderSet;

	math::Matrix4F WorldMatrix;
	RefCountPtr<Mesh> mesh;
	int meshIdx;
	RefCountPtr<MaterialInstance> material;

};

}