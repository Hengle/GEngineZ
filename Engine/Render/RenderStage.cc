#include "RenderStage.h"

namespace z {

std::vector<RenderStage> RenderStage::gStageStack;

void RenderStage::BeginStage(const std::string& name) {

	RenderStage stage(name);
	if (gStageStack.size() > 0) {
		gStageStack.back().CopyToStage(stage);
	}

	gStageStack.push_back(stage);
}

void RenderStage::EndStage(bool recover) {
	CHECK(gStageStack.size() > 0);
	gStageStack.pop_back();
	if (gStageStack.size() > 0) {
		gStageStack.back().mDirtyFlag = DIRTY_ALL;
	}
}

RenderStage* RenderStage::CurStage() {
	if (gStageStack.size() == 0) {
		return nullptr;
	}
	return &gStageStack.back();
}


void RenderStage::Apply() {
	if (gStageStack.size() > 0) {
		gStageStack.back().RecoverDevice();
	}
}



}