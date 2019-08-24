#include "RenderStage.h"

namespace z {

std::vector<RenderStage> RenderStage::gStageStack;

void RenderStage::BeginStage(const std::string& name) {

	RenderStage stage(name);

	gStageStack.push_back(stage);
}

void RenderStage::EndStage(bool recover) {
	CHECK(gStageStack.size() > 0);
	gStageStack.pop_back();
	if (recover) {
		RecoverStage();
	}
}

RenderStage* RenderStage::CurStage() {
	if (gStageStack.size() == 0) {
		return nullptr;
	}
	return &gStageStack.back();
}

void RenderStage::RecoverStage() {
	if (gStageStack.size() == 0) {
		return;
	}
	gStageStack.back().RecoverDevice();
}



}