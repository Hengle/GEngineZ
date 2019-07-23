#include "DX12Pipeline.h"

namespace z {

DX12Pipeline::DX12Pipeline() :
	mRenderTargetNum(0),
	mHasDepthStencil(false) {

}

void DX12Pipeline::SetRenderTargets(int n) {

}

void DX12Pipeline::SetDepthStencil() {

}

void DX12Pipeline::Apply() {
	
	
	// now just create each frame
	Create();
	// use pipeline

}

void DX12Pipeline::Create() {

}


}