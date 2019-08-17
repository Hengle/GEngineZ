#pragma once

#include "DX12Header.h"
namespace z {

class DX12RenderTarget;
class DX12DepthStencil;
class DX12PipelineState;
class DX12CommandList;
class DX12Device;
class DX12VertexBuffer;
class DX12IndexBuffer;
class DX12ShaderInstance;


enum ExecutorStateFlag {
	DX12EXE_FLAG_PSO_DIRTY = 0x0001,
	DX12EXE_FLAG_RT_DIRTY  = 0x0002,
	DX12EXE_FLAG_DS_DIRTY  = 0x0004,
	DX12EXE_FLAG_IB_DIRTY  = 0x0008,
	DX12EXE_FLAG_VB_DIRTY  = 0x0010,
	DX12EXE_FLAG_CB_DIRTY  = 0x0020,
	DX12EXE_FLAG_TEX_DIRTY = 0x0040,
};


// DX12CommandList
class DX12CommandList : public NonCopyable {
public:
	DX12CommandList(DX12Device *device);
	~DX12CommandList();

	void Flush();
	void Reset();
	void Close();

	ID3D12GraphicsCommandList* operator->() const {
		return mCommandList;
	}

	ID3D12GraphicsCommandList* GetCommandList() {
		return mCommandList.GetRef();
	}

	ID3D12CommandQueue* GetCommandQueue() {
		return mCommandQueue.GetRef();
	}

	ID3D12GraphicsCommandList* DoCommand() {
		if (mClosed) {
			Reset();
		}
		return mCommandList.GetRef();
	}


private:
	// command relative
	RefCountPtr<ID3D12CommandQueue> mCommandQueue;
	RefCountPtr<ID3D12CommandAllocator> mCommandAllocator;
	RefCountPtr<ID3D12GraphicsCommandList> mCommandList;
	// fence
	RefCountPtr<ID3D12Fence> mFence;
	UINT64 mFenceValue;
	bool mClosed;
	// device
	DX12Device* mDevice;
};


// DX12Executor
class DX12Executor {
public:
	DX12Executor(DX12Device*);
	~DX12Executor();

	void SetPipelineState(DX12PipelineState*);
	void SetRenderTargets(const std::vector<DX12RenderTarget*>&);
	void SetDepthStencil(DX12DepthStencil*);
	void SetVertexBuffer(DX12VertexBuffer*);
	void SetIndexBuffer(DX12IndexBuffer*);
	void ApplyState();

	void DrawShaderInstance(DX12ShaderInstance*, uint32_t indexNum = 0, uint32_t baseIndex = 0, uint32_t baseVertex = 0);

	void PrepareShaderInstance(DX12ShaderInstance*);
	void DrawBatchSingle(uint32_t indexNum, uint32_t baseIndex, uint32_t baseVertex);

	std::vector<DXGI_FORMAT> GetCurRenderTargetsFormat() const;
	DXGI_FORMAT GetCurDepthStencilFormat() const;

	std::vector<DX12RenderTarget*> GetCurRenderTargets() const;
	DX12DepthStencil* GetCurDepthStencil() const;

	// CommandList
	ID3D12CommandQueue* GetCommandQueue() {
		return mList.GetCommandQueue();
	}
	ID3D12GraphicsCommandList* GetCommandList() {
		return mList.GetCommandList();
	}

	ID3D12GraphicsCommandList* DoCommand() {
		return mList.DoCommand();
	}

	void Reset();

	void Flush();
	
	void FlushAndReset() {
		Flush();
		Reset();
	}

private:
	std::vector<RefCountPtr<DX12RenderTarget>> mRenderTargets;
	RefCountPtr<DX12DepthStencil> mDepthStencil;
	RefCountPtr<DX12VertexBuffer> mVertexBuffer;
	RefCountPtr<DX12IndexBuffer> mIndexBuffer;


	DX12PipelineState* mPSO;
	DX12PipelineState* mCurPSO;

	int mFlag;

	DX12Device* mDevice;
	DX12CommandList mList;
};




// begin viewport -> set render target

// switch render target and draw..

// pass, render to screen -> set render target

// end viewport ->


/*
render->
	begin viewport 0 -> set render target 0
		-> draw to render target 00, 01, 02
		-> merge to redenr target 0
	end viewport 0 ->

	begin viewport 1 -> set render target 1
	end viewport 1

	begin viewport 2 -> set render target 2
		-> draw
		-> draw
	end viewport 2
*/

/* 
Executor
set pipeline state
set render target
set depth stencil
may apply()
for (...) {

	set vertex
	set index
	set ...
	draw -> apply()
}

Execute


*/



}