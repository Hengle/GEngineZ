#pragma once
#include <Core/CoreHeader.h>

namespace z {

class MaterialInstance;
class RHITexture;
class RenderScene;
class RenderItem;

class Primitive : public RefCounter {
public:
	Primitive();

	bool LoadFromFile(std::string file);

	MaterialInstance* LoadMaterialFile(std::string file);

	RHITexture* LoadTextureFile(std::string file);

	void CollectRenderItems(RenderScene* scn);

private:
	std::vector<RefCountPtr<RenderItem>> mRenderItems;

};


}