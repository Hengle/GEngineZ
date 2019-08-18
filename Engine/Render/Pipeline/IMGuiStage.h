#pragma once
#include <Core/CoreHeader.h>
#include <RHI/RHIDevice.h>
#include <RHI/RHIResource.h>
#include <Render/RenderItem.h>
#include <Render/Mesh.h>
#include <Render/Renderer.h>

#include <imgui/imgui.h>

namespace z {

class IMGuiStage : public RefCounter{
public:
	void Init() {
		ImGuiIO& io = ImGui::GetIO();

		// upload font texture
		unsigned char* pixels;
		int width, height;
		io.Fonts->AddFontDefault();
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		RHITexture *tex = GDevice->CreateTexture2D(PF_R8G8B8A8, width, height, 1, pixels);
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

	void Draw(Renderer *ren) {
		ImGui::Render();
		ImDrawData* drawData = ImGui::GetDrawData();

		RHIBlendState state;
		state.Enable         = true;
		state.SrcBlend       = BLEND_FACTOR_SRC_ALPHA;
		state.DestBlend      = BLEND_FACTOR_INV_SRC_ALPHA;
		state.BlendOp        = BLEND_OP_ADD;
		state.SrcBlendAlpha  = BLEND_FACTOR_INV_SRC_ALPHA;
		state.DestBlendAlpha = BLEND_FACTOR_ZERO;
		state.BlendOpAlpha   = BLEND_OP_ADD;

		ren->GetViewport()->GetBackBuffer()->SetBlendState(state);

		struct VERTEX_CONSTANT_BUFFER
		{
			float   mvp[4][4];
		};
		VERTEX_CONSTANT_BUFFER vertex_constant_buffer;
		{
			float L = drawData->DisplayPos.x;
			float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
			float T = drawData->DisplayPos.y;
			float B = drawData->DisplayPos.y + drawData->DisplaySize.y;
			float mvp[4][4] =
			{
				{ 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
				{ 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
				{ 0.0f,         0.0f,           0.5f,       0.0f },
				{ (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
			};
			memcpy(&vertex_constant_buffer.mvp, mvp, sizeof(mvp));
		}

		ren->GetViewport()->SetRenderRect({0.f, 0.f, drawData->DisplaySize.x, drawData->DisplaySize.y, 0.0f, 1.0f});

		mItem->Material->SetParameter("ProjectionMatrix", (const float*)& vertex_constant_buffer, 16);

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
				ren->GetViewport()->SetScissorRect({ 
					(int)(pcmd->ClipRect.x - clip_off.x), 
					(int)(pcmd->ClipRect.y - clip_off.y), 
					(int)(pcmd->ClipRect.z - clip_off.x), 
					(int)(pcmd->ClipRect.w - clip_off.y) });

				// draw with specific index count and offset
				mItem->CustomDraw(pcmd->ElemCount, pcmd->IdxOffset, pcmd->VtxOffset);
			}
		}
	}

private:
	RefCountPtr<RenderItem> mItem;
	std::vector<RefCountPtr<RHITexture>> mTexs;


};

}