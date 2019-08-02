#include "Shader.h"
#include <RHI/RHIResource.h>
#include <RHI/RHIDevice.h>
#include <filesystem>
#include <regex>

namespace z {

std::unordered_map<std::string, Shader*> ShaderManager::gShaders;

void ShaderManager::LoadShaders(FilePath rootPath) {
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
			gShaders[name] = new Shader(rhiShader);
		}
	}
}


std::string ShaderManager::PreProcessingHLSL(const FilePath& codePath) {
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

ShaderInstance* ShaderManager::GetInstance(std::string name) {
	return nullptr;
}

RHIShader* ShaderManager::CompileShader(const std::string &path) {
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
