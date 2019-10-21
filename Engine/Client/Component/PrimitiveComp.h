#pragma once
#include <Core/CoreHeader.h>
#include <Client/Entity/IComponent.h>

namespace z {

class MaterialInstance;
class RHITexture;
class RenderScene;
class RenderItem;
class RenderMesh;

class PrimitiveComp : public IComponent {
public:
	EComponentType GetType() override { return COMP_PRIMITIVE; }

	PrimitiveComp();

	bool LoadFromFile(std::string file);

	MaterialInstance* LoadMaterialFile(std::string file);

	RHITexture* LoadTextureFile(std::string file);

	bool IsIntersectRay(const math::Vector3F& rayStart, const math::Vector3F& rayDir);

	void CollectRender(SceneCollection*) override;

private:
	// bound box
	math::Box mBoundBox;
	bool mIsDrawBoundBox;
	RefCountPtr<RenderItem> mBoundBoxRenderItem;

	void UpdateBoundBox();


	RefCountPtr<RenderMesh> mRenderMesh;
	std::vector<RefCountPtr<RenderItem>> mRenderItems;

};


}