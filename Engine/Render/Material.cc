#include "Material.h"
#include <RHI/RHIResource.h>
#include <RHI/RHIDevice.h>
#include <filesystem>
#include <regex>

namespace z {

std::unordered_map<std::string, Material*> MaterialManager::gMaterials;

// Material
Material::Material(RHIShader* rhiShader) :
	mRHIShader(rhiShader) {
}


// MaterialInstance
MaterialInstance::MaterialInstance(Material* material) :
	mParent(material),
	mRHIShaderInstance(nullptr) {
	mRHIShaderInstance = GDevice->CreateShaderInstance(material->GetShader());
}

MaterialInstance* MaterialManager::GetMaterialInstance(std::string name) {
	Material* m = GetMaterial(name);
	if (m == nullptr) {
		return nullptr;
	}
	return new MaterialInstance(m);

}


void MaterialManager::LoadShaders(FilePath rootPath) {
	for (auto const& entry : std::filesystem::directory_iterator(std::string(rootPath))) {
		if (entry.is_directory()) {
			continue;
		}

		std::string path = entry.path().string();
		std::string name = FilePath(entry.path().string()).FileNameNoExt();
		if (name[0] == '_') {
			continue;
		}
		RHIShader *rhiShader = CompileShader(path);
		if (rhiShader) {
			gMaterials[name] = new Material(rhiShader);
		}
	}
}


std::string MaterialManager::PreProcessingHLSL(const FilePath& codePath) {
	std::string lines = FileReader(codePath).ReadAll();
	std::vector<std::string> strArray;

	std::regex incRegex("#include \"(\\S+)\"");

	auto incBegin = std::sregex_iterator(lines.begin(), lines.end(), incRegex);
	auto incEnd = std::sregex_iterator();

	uint32_t lastPos = 0;
	for (std::sregex_iterator it = incBegin; it != incEnd; ++it) {
		std::smatch match = *it;
		std::string headerPath = FilePath(codePath).ParentDir() / match[1].str();
		std::string headerContent = FileReader(headerPath).ReadAll();

		uint32_t beginPos = match.position();
		if (beginPos > lastPos) {
			strArray.emplace_back(lines.substr(lastPos, beginPos - lastPos));
		}
		strArray.emplace_back(headerContent);
		lastPos = beginPos + match.str().length();
	}
	strArray.emplace_back(lines.substr(lastPos, lines.length() - lastPos));
	std::string shaderStr = "";
	for (const std::string& s : strArray) {
		shaderStr += s + "\r\n";
	}

	return shaderStr;
}

RHIShader* MaterialManager::CompileShader(const std::string &path) {
	Log<LINFO>("Compile Shader", path.c_str());
	std::string shaderStr = PreProcessingHLSL(path);

	RHIShaderStage* stageVS = GDevice->CreateShaderStage(shaderStr.c_str(), shaderStr.length(), SHADER_STAGE_VERTEX);
	RHIShaderStage* stagePS = GDevice->CreateShaderStage(shaderStr.c_str(), shaderStr.length(), SHADER_STAGE_PIXEL);

	RHIShader* shader = nullptr;
	if (stageVS && stagePS) {
		shader = GDevice->CreateShader();
		shader->CombineStage(stageVS);
		shader->CombineStage(stagePS);
		shader->Complete();

		Log<LINFO>("Compile shader succeed!");
	} else {
		Log<LINFO>("Compile shader failed...");
	}
	return shader;
}


}
