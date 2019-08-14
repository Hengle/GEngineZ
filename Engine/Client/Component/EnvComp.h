#pragma once
#include <Core/CoreHeader.h>
#include <Client/Entity/IComponent.h>

namespace z {

class MaterialInstance;
class RHITexture;
class RenderScene;
class RenderItem;

class EnvComp : public IComponent {
public:
	EComponentType GetType() override { return COMP_ENV; }

	EnvComp();

	void SetSky(std::string shader, std::string texPath);


	void CollectRenderItems(RenderScene* scn);
private:
	RefCountPtr<RenderItem> mSkyItem;

	math::Vector3F mSunDirection;
	math::Vector3F mSunColor;

};


}