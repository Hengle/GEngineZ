#pragma once
#include <Core/CoreHeader.h>

namespace z {
class RHIShader;

class ShaderInstance {
	 
};

class Shader {
public:
	Shader(RHIShader*) {}

};



class ShaderManager {
public:
	static void LoadShaders(FilePath rootPath);

	static std::string PreProcessingHLSL(const FilePath& codePath);

	static ShaderInstance* GetInstance(std::string name);
	
private:
	static RHIShader* CompileShader(const std::string&);
	static std::unordered_map<std::string, Shader*> gShaders;

};

}