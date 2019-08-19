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
class Material : public RefCounter {
public:
	Material(RHIShader*);
	~Material();

	RHIShader* GetShader() {
		return mRHIShader;
	}

	void ReplaceShader(RHIShader* shader);

	bool CheckExpired(int age) {
		return age < mAge;
	}

	int GetAge() {
		return mAge;
	}

private:
	RefCountPtr<RHIShader> mRHIShader;
	int mAge;
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

	// !!! method to get shader instance
	RHIShaderInstance* GetShaderInstance();

public:
	Material* mParent;
	RHIRenderState mRState;

private:
	RefCountPtr<RHIShaderInstance> mRHIShaderInstance;
	int mMaterialAge;
};



class MaterialManager {
public:
	static void LoadShaders(FilePath rootPath);
	
	static void ReloadShader(const std::string& name);

	static void ReloadAllShaders();

	static std::string PreProcessingHLSL(const FilePath& codePath);

	static MaterialInstance* GetMaterialInstance(std::string name);

	static Material* GetMaterial(const std::string& name) {
		return gMaterials[name];
	}
	
private:
	static RHIShader* CompileShader(const std::string&);
	static std::unordered_map<std::string, RefCountPtr<Material>> gMaterials;

	static std::string mRootPath;

};

}