#include "Material.h"
#include <RHI/RHIResource.h>
#include <RHI/RHIUtil.h>
#include <RHI/RHIDevice.h>

#include "RenderConst.h"

namespace z {


// Material
Material::Material(RHIShader* rhiShader) :
	mRHIShader(rhiShader),
	mAge(0) {
}

Material::~Material() {}


void Material::ReplaceShader(RHIShader* shader) {
	++mAge;
	mRHIShader = shader;
}

// MaterialInstance
MaterialInstance::MaterialInstance(Material* material) :
	mParent(material),
	mRHIShaderInstance(nullptr),
	mMaterialAge(material->GetAge()) {
	mRState.Value = 0;
	mRState.EnableDepthTest = 1;
	mRState.EnableDepthWrite = 1;
	mRState.FillMode = RS_FILL_SOLID;
	mRState.CullMode = RS_CULL_BACK;
	mRState.DepthCompFunc = RS_COMP_FUNC_LESS;

	mRHIShaderInstance = GDevice->CreateShaderInstance(material->GetShader());
}


void MaterialInstance::SetParameter(const std::string& key, const void* value, int size) {
	GetShaderInstance()->SetParameter(key, value, size);
}

void MaterialInstance::SetParameter(const std::string& key, RHITexture* tex, uint32_t sampler) {
	GetShaderInstance()->SetParameter(key, tex, sampler);
}

RHIShaderInstance* MaterialInstance::GetShaderInstance() {
	if (mMaterialAge < mParent->GetAge()) {
		mMaterialAge = mParent->GetAge();

		// replace shader instance with new shader
		RHIShaderInstance* inst = GDevice->CreateShaderInstance(mParent->GetShader());
		mRHIShaderInstance->CloneParametersTo(inst);
		mRHIShaderInstance = inst;

		Log<LINFO>("Shader instance expried, replaced.");
	}
	return mRHIShaderInstance;
}



}
