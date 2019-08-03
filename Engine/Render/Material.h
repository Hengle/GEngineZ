#pragma once
#include <Core/CoreHeader.h>

namespace z {
class RHIShader;
class RHIShaderInstance;


class Material {
public:
	Material(RHIShader*);

	RHIShader* GetShader() {
		return mRHIShader;
	}

//private:
	RefCountPtr<RHIShader> mRHIShader;
};

class MaterialInstance {
public:
	MaterialInstance(Material*);

//private:
	Material* mParent;
	RefCountPtr<RHIShaderInstance> mRHIShaderInstance;
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