#include "DX12Executor.h"
#include "DX12Device.h"
#include "DX12Texture.h"
#include "DX12View.h"
#include "DX12PipelineState.h"

namespace z {

// DX12Executor


DX12Executor::DX12Executor(DX12Device* device) :
	mDevice(device),
	mList(device),
	mFlag(0),
	mCurRootSignature(nullptr),
	mCurPipelineState(nullptr) {

}

DX12Executor::~DX12Executor() {

}

void DX12Executor::Reset() {
	mVertexBuffer.Reset();
	mIndexBuffer.Reset();
	mConstantBufferViews.clear();
	mTextureViews.clear();
	mSamplerViews.clear();
	mDepthStencil.Reset();
	mRenderTargets.clear();
	mPSO.Reset();
	mCurRootSignature = nullptr;
	mCurPipelineState = nullptr;
	mFlag = 0;
	mList.Reset();
}
void DX12Executor::Flush() {
	mList.Flush();
}

void DX12Executor::SetPipelineState(DX12PipelineState* pso) {
	mPSO = pso;
	mConstantBufferViews.clear();
	mTextureViews.clear();
	mSamplerViews.clear();
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


void DX12Executor::SetConstantBuffer(int idx, DX12ConstantBuffer* cb) {
	if (mConstantBufferViews.size() <= idx) {
		mConstantBufferViews.resize(idx + 1);
	}
	mConstantBufferViews[idx] = cb->GetView();
	mFlag |= DX12EXE_FLAG_CB_DIRTY;
}

void DX12Executor::SetTexture(int idx, DX12Texture* cb) {
	if (mTextureViews.size() <= idx) {
		mTextureViews.resize(idx + 1);
		mSamplerViews.resize(idx + 1);
	}
	mTextureViews[idx] = cb->GetSRView();
	mSamplerViews[idx] = cb->GetSamplerView();
	mFlag |= DX12EXE_FLAG_TEX_DIRTY;
}


void DX12Executor::ApplyState() {
	if (mFlag & DX12EXE_FLAG_PSO_DIRTY) {
		// set root signature
		if (mPSO->GetIRootSignature() != mCurRootSignature) {
			GetCommandList()->SetGraphicsRootSignature(mPSO->GetIRootSignature());
			mCurRootSignature = mPSO->GetIRootSignature();
		}

		// set pipelinestate
		if (mPSO->GetIPipelineState() != mCurPipelineState) {
			GetCommandList()->SetPipelineState(mPSO->GetIPipelineState());
			mCurPipelineState = mPSO->GetIPipelineState();
		}
		
		mFlag &= ~DX12EXE_FLAG_PSO_DIRTY;
	}
	
	// render target
	int rtdsDirty = DX12EXE_FLAG_RT_DIRTY | DX12EXE_FLAG_DS_DIRTY;
	if (mFlag & rtdsDirty) {
		D3D12_CPU_DESCRIPTOR_HANDLE rtHandls[MAX_MRT_NUM]{0};
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

	// set descriptor heap first..
	if (mPSO) {
		std::vector<ID3D12DescriptorHeap*> usedHeap;
		auto CheckInUsed = [&usedHeap](const ID3D12DescriptorHeap* heap) {
			for (int i = 0; i < usedHeap.size(); i++) {
				if (usedHeap[i] == heap) return true;
			}
			return false;
		};

		if (mFlag & (DX12EXE_FLAG_CB_DIRTY|DX12EXE_FLAG_TEX_DIRTY)) {
			for (int i = 0; i < mConstantBufferViews.size(); i++) {
				const ID3D12DescriptorHeap* heap = mConstantBufferViews[i]->GetHeap();
				if (!CheckInUsed(heap)) {
 					usedHeap.emplace_back(const_cast<ID3D12DescriptorHeap*>(heap));
				}
			}
			for (int i = 0; i < mTextureViews.size(); i++) {
				const ID3D12DescriptorHeap* heap = mTextureViews[i]->GetHeap();
				if (!CheckInUsed(heap)) {
					usedHeap.emplace_back(const_cast<ID3D12DescriptorHeap*>(heap));
				}
			}
			for (int i = 0; i < mSamplerViews.size(); i++) {
				const ID3D12DescriptorHeap* heap = mSamplerViews[i]->GetHeap();
				if (!CheckInUsed(heap)) {
					usedHeap.emplace_back(const_cast<ID3D12DescriptorHeap*>(heap));
				}
			}

		}
		if (usedHeap.size() > 0) {
			GetCommandList()->SetDescriptorHeaps(usedHeap.size(), usedHeap.data());
		}
	}

	// constant buffer
	if (mFlag & DX12EXE_FLAG_CB_DIRTY) {
		for (int i = 0; i < mConstantBufferViews.size(); i++) {
			GetCommandList()->SetGraphicsRootDescriptorTable(i, mConstantBufferViews[i]->GetGPUHandle());
		}
		mFlag &= ~DX12EXE_FLAG_CB_DIRTY;
	}
	// texture
	if (mFlag & DX12EXE_FLAG_TEX_DIRTY) {
		int srvBase = mPSO->GetUniformLayout()->GetSRVStart();
		int samplerBase = mPSO->GetUniformLayout()->GetSamplerStart();
		for (int i = 0; i < mTextureViews.size(); i++) {
			GetCommandList()->SetGraphicsRootDescriptorTable(srvBase + i, mTextureViews[i]->GetGPUHandle());
			GetCommandList()->SetGraphicsRootDescriptorTable(samplerBase + i, mSamplerViews[i]->GetGPUHandle());
		}
		mFlag &= ~DX12EXE_FLAG_TEX_DIRTY;
	}
}

void DX12Executor::Draw() {
	ApplyState();

	GetCommandList()->DrawIndexedInstanced(mIndexBuffer->GetSize(), 1, 0, 0, 0);

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