#pragma once

namespace z {

struct ShaderVertexDesc {
	std::string name;
	ERHIPixelFormat format;
	EVertexLaytoutFlag flag;
};

struct ShaderConstantDesc {

};

struct ShaderTextureDesc {
	int idx;
	std::string name;
	ERHISamplerFitler filter;
};

struct ShaderDesc {
	const std::string shaderName;
	std::vector<ShaderVertexDesc> vertexDesc;
	std::vector<ShaderConstantDesc> constantDesc;
	std::vector<ShaderTextureDesc> textureDesc;
};

}