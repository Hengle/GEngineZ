#pragma once
#include <Core/CoreHeader.h>
#include <Render/RenderConst.h>
#include <Render/Material.h>
#include <Render/Mesh.h>

namespace z {

struct RenderUnit : RefCounter{
	ERenderSet renderSet;

	RefCountPtr<Mesh> mesh;
	RefCountPtr<MaterialInstance> material;


};

}