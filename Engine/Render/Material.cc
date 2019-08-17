#include "Material.h"
#include <RHI/RHIResource.h>
#include <RHI/RHIDevice.h>
#include <RHI/RHIUtil.h>
#include <filesystem>
#include <regex>

#include "RenderConst.h"

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
	mRState.Value = 0;
	mRState.EnableDepthTest = 1;
	mRState.EnableDepthWrite = 1;
	mRState.FillMode = RS_FILL_SOLID;
	mRState.CullMode = RS_CULL_BACK;
	mRState.DepthCompFunc = RS_COMP_FUNC_LESS;

	mRHIShaderInstance = GDevice->CreateShaderInstance(material->GetShader());
}


void MaterialInstance::SetParameter(const std::string& key, const float* value, int size) {
	mRHIShaderInstance->SetParameter(key, value, size);
}

void MaterialInstance::SetParameter(const std::string& key, RHITexture* tex, uint32_t sampler) {
	mRHIShaderInstance->SetParameter(key, tex, sampler);
}


void MaterialInstance::DrawIndexed(RHIVertexBuffer* vb, RHIIndexBuffer *ib) {
	GDevice->DrawIndexed(mRHIShaderInstance, vb, ib, mRState);
}

// MaterialManager
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
			// check input
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
