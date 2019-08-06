#pragma once
#include <Core/CoreHeader.h>
#include <Render/RenderConst.h>
#include <Render/Material.h>
#include <Render/SubMesh.h>

namespace z {

struct RenderItem : RefCounter{
	ERenderSet renderSet;

	RefCountPtr<Mesh> mesh;
	RefCountPtr<MaterialInstance> material;


};

}