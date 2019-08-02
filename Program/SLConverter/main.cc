#include <Core/CoreHeader.h>
#include <Render/Shader.h>
#include <HLSLcc/hlslcc.h>

#include <d3d12.h>
#include <D3Dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace z;

struct EntryInfo {
	const char* entry;
	const char* target;
	const char* suffix;
};

static EntryInfo entryInfo[] = {
	{"VS", "vs_5_0", "vs"},
	{"PS", "ps_5_0", "ps"},
};

void Process(const FilePath& path) {
	std::string name = path.FileNameNoExt();
	std::string shader = ShaderManager::PreProcessingHLSL(path);
	
	for (int i = 0; i < sizeof(entryInfo) / sizeof(EntryInfo); i++) {
		const EntryInfo& info = entryInfo[i];
		RefCountPtr<ID3D10Blob> blob{ nullptr }, error{ nullptr };
		D3D_SHADER_MACRO defines{ nullptr, nullptr };
		HRESULT hr = D3DCompile(shader.c_str(), shader.length(), nullptr, &defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			info.entry, info.target, D3DCOMPILE_DEBUG, 0, blob.GetComRef(), error.GetComRef());
		if (FAILED(hr)) {
			std::cout << (char*)error->GetBufferPointer() << std::endl;
		}
		assert(SUCCEEDED(hr));

		uint32_t flags = 0;
		const GlExtensions ext;
		GLSLCrossDependencyData* dep = nullptr;
		HLSLccSamplerPrecisionInfo precInfo;
		HLSLccReflection reflectionCB;

		GLSLShader result;
		TranslateHLSLFromMem((const char*)blob->GetBufferPointer(), flags, LANG_DEFAULT, &ext, dep, precInfo, reflectionCB, &result);
		std::cout << result.GLSLLanguage << std::endl;
		std::cout << result.shaderType << std::endl;
		std::cout << result.sourceCode << std::endl;
	}

}

int main(int argc, char* argv[]) {
	Process("E:/Code/GameZ/Content/Shader/hlsl/test.hlsl");



	//std::cout << result << std::endl;
	return 0;
}