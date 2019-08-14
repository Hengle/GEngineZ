#include <Core/CoreHeader.h>
#include <Render/Material.h>
#include <sstream>
#include <iostream>
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

void PrintErrorLine(std::string shader, const char* error) {
	int begin = 0;
	const char* s = error;
	while (*s && *s++ != '(');
	
	int line = 0;
	for (const char* e = s; *e && *e != ','; line = (*e++ - '0') + line * 10) {}

	std::stringstream st(shader);
	std::string lineStr;
	for (int i = 1; std::getline(st, lineStr); i++) {
		if (i >= line - 3 && i <= line + 3) {
			std::cout << i << ": " << lineStr << std::endl;
		}
	}
	std::cout << std::endl;
}

void Process(const FilePath& path) {
	Log<LINFO>("Compole", path);

	std::string name = path.FileNameNoExt();
	std::string shader = MaterialManager::PreProcessingHLSL(path);
	for (int i = 0; i < sizeof(entryInfo) / sizeof(EntryInfo); i++) {
		const EntryInfo& info = entryInfo[i];
		Log<LINFO>("Compile", info.entry);

		RefCountPtr<ID3D10Blob> blob{ nullptr }, error{ nullptr };
		D3D_SHADER_MACRO defines{ nullptr, nullptr };
		HRESULT hr = D3DCompile(shader.c_str(), shader.length(), nullptr, &defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			info.entry, info.target, D3DCOMPILE_DEBUG, 0, blob.GetComRef(), error.GetComRef());
		if (FAILED(hr) || error) {
			PrintErrorLine(shader, (char*)error->GetBufferPointer());
			std::cout << (char*)error->GetBufferPointer() << std::endl;

			CHECK(0);
		}
	}
	Log<LINFO>("success");

}

using namespace z;

int main(int argc, char* argv[]) {
	const char* name = argv[1];

	// Get Root Path
	char exePath[MAX_PATH];
	ZeroMemory(exePath, MAX_PATH);
	::GetModuleFileName(NULL, exePath, MAX_PATH);
	int blen = strlen("Binary");
	int begin = strlen(exePath) - blen;
	for (; begin >= 0; begin--) {
		if (strncmp(exePath + begin, "Binary", blen) == 0)
			break;
	}
	exePath[begin] = 0;

	z::FilePath root(exePath);
	std::string shaderFile = std::string(argv[1]) + ".hlsl";
	z::FilePath shaderPath = root / "Content/Engine/Shader/hlsl" / shaderFile;

	Process(shaderPath);



	//std::cout << result << std::endl;
	return 0;
}