#pragma once

#include "DX12Header.h"
#include "DX12Resource.h"
#include "DX12Buffer.h"
#include <Core/CoreHeader.h>

namespace z {

class DX12Shader;
class DX12Executor;
class DX12Texture;
class DX12Sampler;

// DX12ShaderStage
class DX12ShaderStage : public RHIShaderStage {
public:
	static DX12ShaderStage* FromCompile(const char* data, size_t datalen, ERHIShaderStage stage);
	
	D3D12_SHADER_BYTECODE GetCode() const {
		return { reinterpret_cast<BYTE*>(mBlob->GetBufferPointer()), mBlob->GetBufferSize() };
	}

	ERHIShaderStage GetStage() const override {
		return mStage;
	}

private:
	DX12ShaderStage(ERHIShaderStage stage, ID3D10Blob* blob);

	RefCountPtr<ID3D10Blob> mBlob{ nullptr };
	ERHIShaderStage mStage;
};


// DX12ShaderInstance
class DX12ShaderInstance : public RHIShaderInstance {
public:
	DX12ShaderInstance(DX12Shader* shader);
	virtual ~DX12ShaderInstance();

	void SetParameter(const std::string& key, const void* value, int size) override;
	void SetParameter(const std::string& key, RHITexture*, uint32_t sampler = SAMPLER_FILTER_LINEAR | SAMPLER_ADDRESS_WRAP) override;
	void CloneParametersTo(RHIShaderInstance*) override;

	DX12Shader* GetShader() {
		return mShader;
	}


	std::vector<ID3D12DescriptorHeap*> GetUsedHeap() const;
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> GetDescriptorTable() const;
	void SetTexturesReadable();
	

private:
	RefCountPtr<DX12Shader> mShader;
	
	std::vector<RefCountPtr<DX12ConstantBuffer>> mCBuffers;
	std::vector<RefCountPtr<DX12Texture>> mTextures;
	std::vector<RefCountPtr<DX12Sampler>> mSamplers;
};


// DX12Shader
class DX12Shader : public RHIShader {
public:
	friend class DX12ShaderInstance;
	DX12Shader();
	virtual ~DX12Shader();

	void CombineStage(RHIShaderStage* rhiStage) override {
		DX12ShaderStage* stage = static_cast<DX12ShaderStage*>(rhiStage);
		if (SHADER_STAGE_VERTEX == stage->GetStage()) {
			mStageVS = stage;
		} else if (SHADER_STAGE_PIXEL == stage->GetStage()) {
			mStagePS = stage;
		}
	}

	bool Complete() override;

	template<ERHIShaderStage targetStage>
	DX12ShaderStage* GetStage() {
		if constexpr (SHADER_STAGE_VERTEX == targetStage) {
			return mStageVS;
		} 
		if constexpr (SHADER_STAGE_PIXEL == targetStage) {
			return mStagePS;
		}
	}

	struct CBufferInfo {
		uint32_t index;
		uint32_t size;
	};

	struct TextureInfo {
		uint32_t index;
	};

	struct SamplerInfo {
		uint32_t index;
	};

	struct VariableInfo{
		uint32_t index;	// cbuffer index
		uint32_t offset;
		uint32_t size;
	};

	D3D12_INPUT_LAYOUT_DESC GetInputLayoutDesc();
	const std::vector<ERHIInputSemantic>& GetInputSemantics() const;

	ID3D12RootSignature* GetIRootSignature() const;

	
private:
	void Reflect();
	void ReflectInput(ID3D12ShaderReflection*, const D3D12_SHADER_DESC&);
	void ReflectConstantBuffer(ID3D12ShaderReflection*, const D3D12_SHADER_DESC&);
	void ReflectBoundResource(ID3D12ShaderReflection*, const D3D12_SHADER_DESC&);

	bool Validate();
	void CreateRootSignature();

	RefCountPtr<DX12ShaderStage> mStageVS;
	RefCountPtr<DX12ShaderStage> mStagePS;


	std::vector<ERHIInputSemantic> mInputSemantics;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputELementsDesc;
	RefCountPtr<ID3D12RootSignature> mRootSignature;

	std::unordered_map<std::string, CBufferInfo> mCBufferMap;
	std::unordered_map<std::string, TextureInfo> mTextureMap;
	std::unordered_map<std::string, VariableInfo> mVariableMap;
	std::unordered_map<std::string, SamplerInfo> mSamplerMap;

	std::vector<char*> mInputNameMem;
};

}
