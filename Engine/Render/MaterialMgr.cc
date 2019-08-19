#include "Material.h"
#include <Render/RenderConst.h>
#include <RHI/RHIResource.h>
#include <RHI/RHIDevice.h>

#include <filesystem>
#include <regex>

namespace z {

std::unordered_map<std::string, RefCountPtr<Material>> MaterialManager::gMaterials;
std::string MaterialManager::mRootPath;


// MaterialManager
MaterialInstance* MaterialManager::GetMaterialInstance(std::string name) {
	Material* m = GetMaterial(name);
	if (m == nullptr) {
		return GetMaterialInstance(MATERIAL_ERROR);
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
		RHIShader* rhiShader = CompileShader(path);
		if (rhiShader) {
			// check input
			gMaterials[name] = new Material(rhiShader);

		}
	}
	mRootPath = rootPath;
}

void MaterialManager::ReloadShader(const std::string& name) {
	FilePath path = FilePath(mRootPath) / (name + ".hlsl");
	Log<LINFO>("Start reload shader ", name);

	RHIShader* rhiShader = CompileShader(path);
	if (rhiShader == nullptr) {
		Log<LERROR>("Reload shader failed!!!");
		return;
	}

	if (gMaterials.count(name) == 0) {
		Log<LINFO>("New shader, just add it...");
		gMaterials[name] = new Material(rhiShader);
		return;
	}


	Material* material = gMaterials[name];
	material->ReplaceShader(rhiShader);

	GDevice->ReloadShaders();
	Log<LINFO>("Shader reload success...");
}

void MaterialManager::ReloadAllShaders() {
	for (auto const& entry : std::filesystem::directory_iterator(std::string(mRootPath))) {
		if (entry.is_directory()) {
			continue;
		}

		std::string path = entry.path().string();
		std::string name = FilePath(entry.path().string()).FileNameNoExt();
		if (name[0] == '_') {
			continue;
		}

		ReloadShader(name);
	}
}


std::string MaterialManager::PreProcessingHLSL(const FilePath& codePath) {
	std::string lines = FileReader(codePath).ReadAll();
	std::vector<std::string> strArray;

	std::regex incRegex("#include \"(\\S+)\"");

	auto incBegin = std::sregex_iterator(lines.begin(), lines.end(), incRegex);
	auto incEnd = std::sregex_iterator();

	ptrdiff_t lastPos = 0;
	for (std::sregex_iterator it = incBegin; it != incEnd; ++it) {
		std::smatch match = *it;
		std::string headerPath = FilePath(codePath).ParentDir() / match[1].str();
		std::string headerContent = FileReader(headerPath).ReadAll();

		ptrdiff_t beginPos = match.position();
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

RHIShader* MaterialManager::CompileShader(const std::string& path) {
	Log<LINFO>("Compile Shader", path.c_str());
	std::string shaderStr = PreProcessingHLSL(path);

	RHIShaderStage* stageVS = GDevice->CreateShaderStage(shaderStr.c_str(), shaderStr.length(), SHADER_STAGE_VERTEX);
	RHIShaderStage* stagePS = GDevice->CreateShaderStage(shaderStr.c_str(), shaderStr.length(), SHADER_STAGE_PIXEL);

	RHIShader* shader = nullptr;
	if (stageVS && stagePS) {
		shader = GDevice->CreateShader();
		shader->CombineStage(stageVS);
		shader->CombineStage(stagePS);
		if (shader->Complete()) {
			Log<LINFO>("Compile shader succeed!");
		} else {
			Log<LERROR>("Compile shader succeed! but validate failed...");
		}
	} else {
		Log<LINFO>("Compile shader failed...");
	}
	return shader;
}


}