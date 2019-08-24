#pragma once
#include <Core/CoreHeader.h>
#include <RHI/RHIDevice.h>
#include <RHI/RHIResource.h>
#include <Render/RenderItem.h>
#include <Render/Mesh.h>
#include <Render/Renderer.h>
#include <Render/RenderStage.h>
#include <imgui/imgui.h>

#include "IMGuiStep.h"

namespace z {

void IMGuiStep::Init() {
	ImGuiIO& io = ImGui::GetIO();

	// upload font texture
	unsigned char* pixels;
	int width, height;
	io.Fonts->AddFontDefault();
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	RHITexture* tex = GDevice->CreateTexture2D(PF_R8G8B8A8, width, height, 1, pixels);
	io.Fonts->TexID = (ImTextureID)tex;
	mTexs.push_back(tex);

	mItem = new RenderItem();

	// create dynamic mesh
	mItem->Mesh = new RenderMesh(true);
	mItem->Mesh->SetVertexSemantics({ SEMANTIC_POSITION2D, SEMANTIC_UV0, SEMANTIC_COLOR });
	mItem->Mesh->SetIndexStride(4);

	// create material
	mItem->Material = MaterialManager::GetMaterialInstance("IMGui");
	mItem->Material->SetCullMode(RS_CULL_NONE);
	mItem->Material->SetEnableDepthTest(false);
	mItem->Material->SetEnableDepthWrite(true);
}

void IMGuiStep::Resize(uint32_t width, uint32_t height) {

}

void IMGuiStep::Render(Renderer* ren) {
	ImGui::Render();
	ImDrawData* drawData = ImGui::GetDrawData();

	// begin ui rende stage
	RenderStageScope("IMGui");

	// merge vertex and index
	uint32_t totalV = 0, totalI = 0;
	for (size_t i = 0; i < drawData->CmdListsCount; i++) {
		const ImDrawList* cmdList = drawData->CmdLists[i];
		mItem->Mesh->CopyVertex(totalV * sizeof(ImDrawVert), cmdList->VtxBuffer.Size * sizeof(ImDrawVert), (const char*)cmdList->VtxBuffer.Data, i);
		mItem->Mesh->CopyIndex(totalI * sizeof(ImDrawIdx), cmdList->IdxBuffer.Size * sizeof(ImDrawIdx), (const char*)cmdList->IdxBuffer.Data, i);

		totalV += cmdList->VtxBuffer.Size;
		totalI += cmdList->IdxBuffer.Size;
	}

	if (totalV <= 0 || totalI <= 0) {
		return;
	}
	mItem->Mesh->Complete(drawData->CmdListsCount, drawData->CmdListsCount);

	// blend state
	RenderStage::CurStage()->SetBlendState({
		true,
		BLEND_FACTOR_SRC_ALPHA, BLEND_FACTOR_INV_SRC_ALPHA, BLEND_OP_ADD,
		BLEND_FACTOR_SRC_ALPHA, BLEND_FACTOR_INV_SRC_ALPHA, BLEND_OP_ADD
		}
	);

	// viewport
	RenderStage::CurStage()->SetRenderRect({ 0.f, 0.f, drawData->DisplaySize.x, drawData->DisplaySize.y, 0.0f, 1.0f });

	// parameter mvp
	{
		float l = drawData->DisplayPos.x;
		float r = drawData->DisplayPos.x + drawData->DisplaySize.x;
		float t = drawData->DisplayPos.y;
		float b = drawData->DisplayPos.y + drawData->DisplaySize.y;
		math::Matrix4F ortho = math::MatrixOrtho2D(l, r, b, t);
		mItem->Material->SetParameter("ProjectionMatrix", (const float*)& ortho, 16);
	}

	// draw all ui mesh
	int Voff = 0, Ioff = 0;
	ImVec2 clip_off = drawData->DisplayPos;
	for (int i = 0; i < drawData->CmdListsCount; i++) {
		const ImDrawList* cmdList = drawData->CmdLists[i];

		// set mesh group to draw
		mItem->SetMeshIndexGroup(i);
		mItem->SetMeshVertexGroup(i);

		for (int cmd = 0; cmd < cmdList->CmdBuffer.Size; cmd++) {
			const ImDrawCmd* pcmd = &cmdList->CmdBuffer[cmd];
			mItem->Material->SetParameter("texture0", (RHITexture*)pcmd->TextureId);
			RenderStage::CurStage()->SetScissorRect({
				(int)(pcmd->ClipRect.x - clip_off.x),
				(int)(pcmd->ClipRect.y - clip_off.y),
				(int)(pcmd->ClipRect.z - clip_off.x),
				(int)(pcmd->ClipRect.w - clip_off.y) });

			// draw with specific index count and offset
			mItem->CustomDraw(pcmd->ElemCount, pcmd->IdxOffset, pcmd->VtxOffset);
		}
	}
}

}
