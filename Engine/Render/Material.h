#pragma once
#include <Core/CoreHeader.h>
#include <RHI/RHIConst.h>
#include <Render/RenderConst.h>

namespace z {
class RHIShader;
class RHITexture;
class RHIVertexBuffer;
class RHIIndexBuffer;
class RHIShaderInstance;

// memory manage by material manager
class Material {
public:
	Material(RHIShader*);

	RHIShader* GetShader() {
		return mRHIShader;
	}

private:
	RefCountPtr<RHIShader> mRHIShader;
};


class MaterialInstance : public RefCounter{
public:
	MaterialInstance(Material*);

	void SetParameter(const std::string& key, const float* value, int size);
	void SetParameter(const std::string& key, RHITexture*, uint32_t sampler = SAMPLER_FILTER_LINEAR | SAMPLER_ADDRESS_WRAP);

	Material* GetMaterial() {
		return mParent;
	}


	void SetFillMode(ERHIRenderState rs) {
		mRState.FillMode = rs;
	}

	void SetCullMode(ERHIRenderState rs) {
		mRState.CullMode = rs;
	}

	void SetEnableDepthWrite(bool enable) {
		mRState.EnableDepthWrite = enable ? 1 : 0;
	}

	void SetEnableDepthTest(bool enable) {
		mRState.EnableDepthTest = enable ? 1 : 0;
	}

	void SetEnableStencil(bool enable) {
		mRState.EnableStencil = enable ? 1 : 0;
	}

public:
	Material* mParent;
	RefCountPtr<RHIShaderInstance> mRHIShaderInstance;
	RHIRenderState mRState;
};



class MaterialManager {
public:
	static void LoadShaders(FilePath rootPath);

	static std::string PreProcessingHLSL(const FilePath& codePath);

	static MaterialInstance* GetMaterialInstance(std::string name);

	static Material* GetMaterial(const std::string& name) {
		return gMaterials[name];
	}
	
private:
	static RHIShader* CompileShader(const std::string&);
	static std::unordered_map<std::string, Material*> gMaterials;

};

}