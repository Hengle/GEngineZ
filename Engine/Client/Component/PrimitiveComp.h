#pragma once
#include <Core/CoreHeader.h>
#include <Client/Entity/IComponent.h>

namespace z {

class MaterialInstance;
class RHITexture;
class RenderScene;
class RenderItem;

class PrimitiveComp : public IComponent {
public:
	EComponentType GetType() override { return COMP_PRIMITIVE; }

	PrimitiveComp();

	bool LoadFromFile(std::string file);

	MaterialInstance* LoadMaterialFile(std::string file);

	RHITexture* LoadTextureFile(std::string file);

	void CollectRenderItems(RenderScene* scn);

private:
	std::vector<RefCountPtr<RenderItem>> mRenderItems;

};


}