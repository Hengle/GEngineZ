#pragma once
#include <Core/CoreHeader.h>

#include "RenderConst.h"
#include "RenderItem.h"
#include "Material.h"

namespace z {

class SceneCollection : public RefCounter {
public:
	void Reset() {
		mRenderItems.clear();
	}

	void SetCamera(math::Vector3F pos, math::Matrix4F view, math::Matrix4F proj) {
		mCameraPos = pos;
		mViewMatrix = view;
		mViewProjMatrix = view * proj;	
	}

	void PushRenderItem(RenderItem* item) {
		mRenderItems.push_back(item);
	}

	std::vector<RenderItem*> FilterItems(ERenderSet rset) {
		std::vector<RenderItem*> result;
		for (auto& item : mRenderItems) {
			if (item->RenderSet == rset) {
				result.push_back(item);
			}
		}
		return result;
	}

	void RetriveSceneParams(MaterialInstance *material) {
		material->SetParameter("ViewProj", (const float*)& mViewProjMatrix, 16);
		math::Vector4F cameraPos = { mCameraPos, 0.0f };
		material->SetParameter("CameraPos", (const float*)cameraPos.value, 4);

		static std::unordered_map<EShaderParams, std::string> paramMap = {
			{SP_SUN_COLOR, "SunColor"},
			{SP_SUN_DIRECTION, "SunDirection"},
			{SP_AMBIENT_COLOR, "AmbientColor"},
		};
	
		for (size_t i = 0; i < SP_MAX; i++) {
			material->SetParameter(paramMap[(EShaderParams)i], ShaderParams[i].value, 4);
		}
	}


	math::Vector4F ShaderParams[SP_MAX];

private:
	std::vector<RefCountPtr<RenderItem>> mRenderItems;
	
	math::Vector3F mCameraPos;
	math::Matrix4F mViewMatrix;
	math::Matrix4F mViewProjMatrix;

};

}