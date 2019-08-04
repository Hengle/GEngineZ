#pragma once
#include <Core/CoreHeader.h>
#include <RHI/RHIConst.h>
#include <Render/RenderConst.h>

namespace z {
class RHIShader;
class RHITexture;
class RHIInputDesc;
class RHIShaderInstance;

// memory manage by material manager
class Material {
public:
	Material(RHIShader*, const std::vector<EFVFormat>&);

	RHIShader* GetShader() {
		return mRHIShader;
	}

	std::vector<EFVFormat> GetFVFs() {
		return mFVFs;
	}

private:
	RefCountPtr<RHIShader> mRHIShader;
	std::vector<EFVFormat> mFVFs;
};


class MaterialInstance : public RefCounter{
public:
	MaterialInstance(Material*);

	void SetParameter(const std::string& key, const float* value, int size);
	void SetParameter(const std::string& key, RHITexture*, uint32_t sampler = SAMPLER_FILTER_LINEAR | SAMPLER_ADDRESS_WRAP);

	Material* GetMaterial() {
		return mParent;
	}

	void DrawIndexed();


public:
	Material* mParent;
	RefCountPtr<RHIShaderInstance> mRHIShaderInstance;
};



class MaterialManager {
public:
	static void LoadShaders(FilePath rootPath);

	static std::string PreProcessingHLSL(const FilePath& codePath);

	static MaterialInstance* GetMaterialInstance(std::string name);

	static bool ParseInputFVF(const std::vector<RHIInputDesc>& inputs, std::vector<EFVFormat>& formats);

	static Material* GetMaterial(const std::string& name) {
		return gMaterials[name];
	}
	
private:
	static RHIShader* CompileShader(const std::string&);
	static std::unordered_map<std::string, Material*> gMaterials;

};

}