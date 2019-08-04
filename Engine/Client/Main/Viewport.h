#pragma once

#include <Core/CoreHeader.h>

#include <RHI/RHIResource.h>
#include <Render/Material.h>
#include <Render/SubMesh.h>
#include <Render/RenderItem.h>
namespace z {
class Scene;
class RenderScene;

class MaterialInstance;

class Viewport {
public:
	Viewport(uint32_t width, uint32_t height);
	virtual ~Viewport();

	void Resize(int width, int height);
	void Tick();
	void Render();

	void DrawTex();

	
private:
	uint32_t mWidth;
	uint32_t mHeight;

	Scene* mScene;
	RenderScene* mRenderScene;

	RefCountPtr<RHIViewport> mRHIViewport;
	RefCountPtr<RHITexture> ds, rt;
	RefCountPtr<RHITexture> tex;

	RefCountPtr<MaterialInstance> si;


	RefCountPtr<MaterialInstance> material;
	Mesh* mesh;

	std::vector<RefCountPtr<RenderItem>> items;

};


}