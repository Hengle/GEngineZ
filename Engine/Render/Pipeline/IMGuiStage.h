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
		unsigned char* pixels;
		int width, height;
		io.Fonts->AddFontDefault();
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		// Upload texture
		{
			RHITextureDesc desc;
			desc.sizeX = width;
			desc.sizeY = height;
			desc.sizeZ = 1;
			desc.format = PF_R8G8B8A8;
			desc.dimension = TEX_DIMENSION_2D;
			desc.numMips = 1;
			RHITexture* tex = GDevice->CreateTexture(desc, pixels);

			io.Fonts->TexID = (ImTextureID)tex;

		}

		mUIItem = new RenderItem();
		mUIItem->mesh = new Mesh();
		mUIItem->mesh->Stride = sizeof(ImDrawVert);
		mUIItem->mesh->Semantics = { SEMANTIC_POSITION2D, SEMANTIC_UV0, SEMANTIC_COLOR };

		mUIItem->material = MaterialManager::GetMaterialInstance("IMGui");
		mUIItem->material->SetCullMode(RS_CULL_NONE);
		mUIItem->material->SetEnableDepthTest(false);
		mUIItem->material->SetEnableDepthWrite(true);


	}

	void Draw(Renderer *ren) {
		ImGui::Render();
		ImDrawData* drawData = ImGui::GetDrawData();

		RHIBlendState state;
		state.Enable = true;
		state.SrcBlend = BLEND_FACTOR_SRC_ALPHA;
		state.DestBlend = BLEND_FACTOR_INV_SRC_ALPHA;
		state.BlendOp = BLEND_OP_ADD;
		state.SrcBlendAlpha = BLEND_FACTOR_INV_SRC_ALPHA;
		state.DestBlendAlpha = BLEND_FACTOR_ZERO;
		state.BlendOpAlpha = BLEND_OP_ADD;

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

		mUIItem->material->SetParameter("ProjectionMatrix", (const float*)& vertex_constant_buffer, 16);

		Mesh* mesh = mUIItem->mesh;
		// merge vertex and index
		uint32_t totalV = 0, totalI = 0;
		bool needExpandV = false, needExpandI = false;
		for (size_t i = 0; i < drawData->CmdListsCount; i++) {
			const ImDrawList* cmdList = drawData->CmdLists[i];
			totalV += cmdList->VtxBuffer.Size;
			totalI += cmdList->IdxBuffer.Size;
			if (totalV * (sizeof(ImDrawVert) / 4) > mesh->Vertexes.size()) {
				mesh->Vertexes.resize(totalV * sizeof(ImDrawVert) / 4 + 1000);
				needExpandV = true;
			}
			if (totalI > mesh->Indices.size()) {
				mesh->Indices.resize(totalI + 3000);
				needExpandI = true;
			}
			memcpy((char*)mesh->Vertexes.data() + (totalV - cmdList->VtxBuffer.Size) * sizeof(ImDrawVert), cmdList->VtxBuffer.Data, cmdList->VtxBuffer.Size * sizeof(ImDrawVert));
			memcpy((char*)mesh->Indices.data() + (totalI - cmdList->IdxBuffer.Size) * sizeof(ImDrawIdx), cmdList->IdxBuffer.Data, cmdList->IdxBuffer.Size * sizeof(ImDrawIdx));
		}

		if (totalV <= 0 || totalI <= 0) {
			return;
		}

		if (needExpandV) {
			mesh->mVBuffer = GDevice->CreateVertexBuffer(mesh->Vertexes.size() / (mesh->Stride / 4), mesh->Semantics, mesh->Vertexes.data(), true);
		} else {
			void *p = mesh->mVBuffer->MapBuffer();
			memcpy(p, mesh->Vertexes.data(), mesh->Vertexes.size() * 4);
			mesh->mVBuffer->UnMapBuffer();
		}

		if (needExpandI) {
			mesh->mIBuffer = GDevice->CreateIndexBuffer(mesh->Indices.size(), 4, mesh->Indices.data(), true);
		} else {
			void* p = mesh->mIBuffer->MapBuffer();
			memcpy(p, mesh->Indices.data(), mesh->Indices.size() * 4);
			mesh->mIBuffer->UnMapBuffer();
		}

		int Voff = 0, Ioff = 0;

		ImVec2 clip_off = drawData->DisplayPos;
		for (int n = 0; n < drawData->CmdListsCount; n++) {
			const ImDrawList* cmdList = drawData->CmdLists[n];

			for (int cmd_i = 0; cmd_i < cmdList->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmdList->CmdBuffer[cmd_i];
				mUIItem->material->SetParameter("texture0", (RHITexture*)pcmd->TextureId);
				ren->GetViewport()->SetScissorRect({ 
					(int)(pcmd->ClipRect.x - clip_off.x), 
					(int)(pcmd->ClipRect.y - clip_off.y), 
					(int)(pcmd->ClipRect.z - clip_off.x), 
					(int)(pcmd->ClipRect.w - clip_off.y) });
				GDevice->DrawIndexed(mUIItem->material->mRHIShaderInstance, mUIItem->mesh->mVBuffer, mUIItem->mesh->mIBuffer,
					mUIItem->material->mRState, pcmd->ElemCount, pcmd->IdxOffset + Ioff, pcmd->VtxOffset + Voff);


			}
			Voff += cmdList->VtxBuffer.Size;
			Ioff += cmdList->IdxBuffer.Size;
		}



	}

private:
	RefCountPtr<RenderItem> mUIItem;


};

}