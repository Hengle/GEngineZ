#pragma once
#include <Core/CoreHeader.h>
#include <RHI/RHIDevice.h>
#include <RHI/RHIResource.h>
#include <Render/RenderItem.h>

namespace z {

class IMGuiStage {
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

	void Draw(RHITexture* rt) {
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

		rt->SetBlendState(state);

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
		ScreenRenderRect rect;
		rect.width = drawData->DisplaySize.x;
		rect.height = drawData->DisplaySize.y;
		rect.topLeftX = rect.topLeftY = 0.0f;
		mRHIViewport->SetRenderRect(rect);

		mUIItem->material->SetParameter("ProjectionMatrix", (const float*)& vertex_constant_buffer, 16);


		for (int n = 0; n < drawData->CmdListsCount; n++) {
			const ImDrawList* cmd_list = drawData->CmdLists[n];


			std::vector<float> vdata;
			std::vector<uint32_t> idata;

			vdata.resize(sizeof(ImDrawVert) / 4 * cmd_list->VtxBuffer.Size);
			idata.resize(cmd_list->IdxBuffer.Size);
			memcpy(vdata.data(), cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
			memcpy(idata.data(), cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));

			mesh->Indices.push_back(idata);
			mesh->Vertexes = vdata;

			mesh->CreateBuffer();



			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				mUIItem->material->SetParameter("texture0", (RHITexture*)pcmd->TextureId);
				mUIItem->material->DrawIndexed(uiItem->mesh->mVBuffer, uiItem->mesh->mIBuffer[0]);
			}
		}



	}

private:
	RefCountPtr<RenderItem> mUIItem;


};

}