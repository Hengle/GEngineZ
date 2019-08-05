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
Material::Material(RHIShader* rhiShader, const std::vector<EFVFormat>& fvfs) :
	mRHIShader(rhiShader) {
	mFVFs = fvfs;
}


// MaterialInstance
MaterialInstance::MaterialInstance(Material* material) :
	mParent(material),
	mRHIShaderInstance(nullptr) {
	mRHIShaderInstance = GDevice->CreateShaderInstance(material->GetShader());
}


void MaterialInstance::SetParameter(const std::string& key, const float* value, int size) {
	mRHIShaderInstance->SetParameter(key, value, size);
}

void MaterialInstance::SetParameter(const std::string& key, RHITexture* tex, uint32_t sampler) {
	mRHIShaderInstance->SetParameter(key, tex, sampler);
}


void MaterialInstance::DrawIndexed() {
	GDevice->DrawIndexed(mRHIShaderInstance, 0);
}

// MaterialManager
MaterialInstance* MaterialManager::GetMaterialInstance(std::string name) {
	Material* m = GetMaterial(name);
	if (m == nullptr) {
		return nullptr;
	}
	return new MaterialInstance(m);
}


bool MaterialManager::ParseInputFVF(const std::vector<RHIInputDesc>& inputs, std::vector<EFVFormat>& formats) {

#define CHECK_AND_APPEND_FVF(input, name, index, fvf) {						\
if (input.SemanticName == #name && input.SemanticIndex == index && GetRHIPixelFormatSize(input.Format) == GetFVFSize(fvf)) { \
	formats.push_back(fvf);												\
	continue;															\
}}

	for (const RHIInputDesc& input : inputs) {
		CHECK_AND_APPEND_FVF(input, POSITION, 0, FVF_XYZ);
		CHECK_AND_APPEND_FVF(input, NORMAL, 0, FVF_NORMAL);
		CHECK_AND_APPEND_FVF(input, TEXCOORD, 0, FVF_UV0);
		CHECK_AND_APPEND_FVF(input, TEXCOORD, 1, FVF_UV1);
		return false;
	}
#undef CHECK_AND_APPEND_FVF

	return true;
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
			std::vector<RHIInputDesc>& inputs = rhiShader->GetRHIInputsDesc();
			std::vector<EFVFormat> fvfs;
			if (ParseInputFVF(inputs, fvfs)) {
				gMaterials[name] = new Material(rhiShader, fvfs);
			} else {
				Log<LERROR>("Shader input format not supported.....");
			}
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