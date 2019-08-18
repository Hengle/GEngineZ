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
	~EnvComp();

	void SetSky(std::string shader, std::string texPath);


	void CollectRenderItems(RenderScene* scn);


	math::Vector3F& GetSunRotater() {
		return mSunRotater;
	}

	void SetSunRotater(const math::Vector3F& v) {
		mSunRotater = v;
	}

	math::Vector3F& GetSunColor() {
		return mSunColor;
	}

	void SetSunColor(const math::Vector3F& v) {
		mSunColor = v;
	}

	math::Vector3F& GetAmbientColor() {
		return mAmbientColor;
	}

	void SetAmbientColor(const math::Vector3F& v) {
		mAmbientColor = v;
	}



private:
	RefCountPtr<RenderItem> mSkyItem;

	math::Vector3F mSunRotater;
	math::Vector3F mSunColor;
	math::Vector3F mAmbientColor;

};


}