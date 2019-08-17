#include "DX12Executor.h"
#include "DX12Device.h"
#include "DX12Texture.h"
#include "DX12View.h"
#include "DX12Shader.h"
#include "DX12PipelineState.h"

namespace z {

// DX12Executor


DX12Executor::DX12Executor(DX12Device* device) :
	mDevice(device),
	mList(device),
	mFlag(0),
	mPSO{nullptr},
	mCurPSO{nullptr}{

}

DX12Executor::~DX12Executor() {

}

void DX12Executor::Reset() {
	mVertexBuffer.Reset();
	mIndexBuffer.Reset();
	mDepthStencil.Reset();
	mRenderTargets.clear();
	mPSO = nullptr;
	mCurPSO = nullptr;
	mFlag = 0;
	mList.Reset();
}
void DX12Executor::Flush() {
	mList.Flush();
}

void DX12Executor::SetPipelineState(DX12PipelineState* pso) {
	mPSO = pso;
	mFlag |= DX12EXE_FLAG_PSO_DIRTY;
}

void DX12Executor::SetRenderTargets(const std::vector<DX12RenderTarget*>& target) {
	mRenderTargets.clear();
	for (size_t i = 0; i < target.size(); i++) {
		mRenderTargets.push_back(target[i]);
	}
	mFlag |= DX12EXE_FLAG_RT_DIRTY;
}

void DX12Executor::SetDepthStencil(DX12DepthStencil* ds) {
	mDepthStencil = ds;
	mFlag |= DX12EXE_FLAG_DS_DIRTY;
}

void DX12Executor::SetVertexBuffer(DX12VertexBuffer* vb) {
	mVertexBuffer = vb;
	mFlag |= DX12EXE_FLAG_VB_DIRTY;
}

void DX12Executor::SetIndexBuffer(DX12IndexBuffer* ib) {
	mIndexBuffer = ib;
	mFlag |= DX12EXE_FLAG_IB_DIRTY;
}

void DX12Executor::ApplyState() {
	if (mFlag & DX12EXE_FLAG_PSO_DIRTY) {
		if (mPSO != mCurPSO) {
			GetCommandList()->SetGraphicsRootSignature(mPSO->GetIRootSignature());
			GetCommandList()->SetPipelineState(mPSO->GetIPipelineState());
			
			mCurPSO = mPSO;
		}

		mFlag &= ~DX12EXE_FLAG_PSO_DIRTY;
	}
	
	// render target
	int rtdsDirty = DX12EXE_FLAG_RT_DIRTY | DX12EXE_FLAG_DS_DIRTY;
	if (mFlag & rtdsDirty) {
		D3D12_CPU_DESCRIPTOR_HANDLE rtHandls[MAX_RT_NUM]{0};
		for (int i = 0; i < mRenderTargets.size(); i++) {
			rtHandls[i] = mRenderTargets[i]->GetRTView()->GetCPUHandle();
			mRenderTargets[i]->SetWritable();
		}
		if (mDepthStencil) {
			mDepthStencil->SetWritable();
			D3D12_CPU_DESCRIPTOR_HANDLE dsHandle = mDepthStencil->GetDSView()->GetCPUHandle();
			GetCommandList()->OMSetRenderTargets((uint32_t)mRenderTargets.size(), rtHandls, 0, &dsHandle);
		} else {
			GetCommandList()->OMSetRenderTargets((uint32_t)mRenderTargets.size(), rtHandls, 0, nullptr);
		}
		mFlag &= ~rtdsDirty;
	}

	// vertex
	if (mFlag & DX12EXE_FLAG_VB_DIRTY) {
		GetCommandList()->IASetVertexBuffers(0, 1, &mVertexBuffer->GetView());
		mFlag &= ~DX12EXE_FLAG_VB_DIRTY;
	}

	// index
	if (mFlag & DX12EXE_FLAG_IB_DIRTY) {
		GetCommandList()->IASetIndexBuffer(&mIndexBuffer->GetView());
		GetCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mFlag &= ~DX12EXE_FLAG_IB_DIRTY;
	}
}

void DX12Executor::DrawShaderInstance(DX12ShaderInstance *shaderInst, uint32_t indexNum, uint32_t baseIndex, uint32_t baseVertex ) {
	ApplyState();

	std::vector<ID3D12DescriptorHeap*> &heap = shaderInst->GetUsedHeap();
	GetCommandList()->SetDescriptorHeaps(heap.size(), heap.data());

	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> &descTable = shaderInst->GetDescriptorTable();
	for (int i = 0; i < descTable.size(); i++) {
		if (descTable[i].ptr != 0) {
			GetCommandList()->SetGraphicsRootDescriptorTable(i, descTable[i]);
		}
	}
	if (indexNum == 0) {
		indexNum = mIndexBuffer->GetSize();
	}

	GetCommandList()->DrawIndexedInstanced(indexNum, 1, baseIndex, baseVertex, 0);
}

void DX12Executor::PrepareShaderInstance(DX12ShaderInstance* shaderInst) {
	ApplyState();

	std::vector<ID3D12DescriptorHeap*>& heap = shaderInst->GetUsedHeap();
	GetCommandList()->SetDescriptorHeaps(heap.size(), heap.data());

	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>& descTable = shaderInst->GetDescriptorTable();
	for (int i = 0; i < descTable.size(); i++) {
		if (descTable[i].ptr != 0) {
			GetCommandList()->SetGraphicsRootDescriptorTable(i, descTable[i]);
		}
	}
}

void DX12Executor::DrawBatchSingle(uint32_t indexNum, uint32_t baseIndex, uint32_t baseVertex) {
	if (indexNum == 0) {
		indexNum = mIndexBuffer->GetSize();
	}
	GetCommandList()->DrawIndexedInstanced(indexNum, 1, baseIndex, baseVertex, 0);
}

std::vector<DXGI_FORMAT> DX12Executor::GetCurRenderTargetsFormat() const {
	std::vector<DXGI_FORMAT> formats;
	formats.reserve(mRenderTargets.size());
	for (DX12RenderTarget* rt : mRenderTargets) {
		formats.push_back(rt->Format);
	}
	return formats;
}

DXGI_FORMAT DX12Executor::GetCurDepthStencilFormat() const {
	if (mDepthStencil) {
		return mDepthStencil->Format;
	}
	return DXGI_FORMAT_UNKNOWN;
}

std::vector<DX12RenderTarget*> DX12Executor::GetCurRenderTargets() const {

	std::vector<DX12RenderTarget*> rts;
	rts.reserve(mRenderTargets.size());
	for (DX12RenderTarget* rt : mRenderTargets) {
		rts.push_back(rt);
	}
	return rts;
}

DX12DepthStencil* DX12Executor::GetCurDepthStencil() const {
	return mDepthStencil;
}



// DX12CommandList
DX12CommandList::DX12CommandList(DX12Device* device) :
	mClosed(false),
	mDevice(device){
	auto idevice = mDevice->GetIDevice();
	// create command objects (queue/allocator/list)
	D3D12_COMMAND_QUEUE_DESC queueDesc{};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Priority = 0;
	queueDesc.NodeMask = 0;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	DX12_CHECK(idevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(mCommandQueue.GetComRef())));
	DX12_CHECK(idevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mCommandAllocator.GetComRef())));
	DX12_CHECK(idevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.GetRef(),
		nullptr,                        // initial pipeline is none
		IID_PPV_ARGS(mCommandList.GetComRef())));

	DX12_CHECK(idevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(mFence.GetComRef())));
	mFenceValue = 0;

	Close();
	Reset();

	Log<LINFO>("Command queue create done.");
}

DX12CommandList::~DX12CommandList() {
	// todo
}

void DX12CommandList::Close() {
	if (!mClosed) {
		DX12_CHECK(mCommandList->Close());
		mClosed = true;
	}
}

void DX12CommandList::Reset() {
	mCommandAllocator->Reset();
	mCommandList->Reset(mCommandAllocator.GetRef(), nullptr);
	mClosed = false;
}

void DX12CommandList::Flush() {
	Close();

	ID3D12CommandList* cmdLists[] = { mCommandList.GetRef() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);

	// flush command and wait compeleted.
	++mFenceValue;
	DX12_CHECK(mCommandQueue->Signal(mFence.GetRef(), mFenceValue));
	if (mFence->GetCompletedValue() < mFenceValue) {
		HANDLE event = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		DX12_CHECK(mFence->SetEventOnCompletion(mFenceValue, event));
		if (event) {
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}
}




}